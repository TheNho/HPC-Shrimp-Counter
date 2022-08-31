// LogHistory.cpp : implementation file
// LogHistory.h: Header file

 // #include "pch.h" // pch.h error
#include "stdafx.h"
#include "BasicDemo.h"
#include "LogHistory.h"
//#include "afxdialogex.h"

// LogHistory dialog

IMPLEMENT_DYNAMIC(LogHistory, CDialogEx)

LogHistory::LogHistory(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOG_VIEW_DIALOG, pParent)
{

}

LogHistory::~LogHistory()
{
}

void LogHistory::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOG_HISTORY_LIST, m_listCtrl);
}


BEGIN_MESSAGE_MAP(LogHistory, CDialogEx)
	ON_BN_CLICKED(ID_LOG_OK, &LogHistory::OnBnClickedLogOk)
	ON_BN_CLICKED(ID_LOG_CLEAR_DATA, &LogHistory::OnBnClickedLogClearData)
END_MESSAGE_MAP()

// global filename
extern CString global_filename;
BOOL LogHistory::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// Set font
	//CFont Font_big_log;
	//Font_big_log.CreatePointFont(10, _T("Microsoft Sans Serif"));
	//GetDlgItem(IDC_LOG_HISTORY_LIST)->SetFont(&Font_big_log);

	// Set the icon for this dialog. The framework does this automatically
	// when the application's main window is not a dialog
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_LOG_HISTORY_ICON);
	SetIcon(m_hIcon, TRUE);         // Set big icon
	SetIcon(m_hIcon, FALSE);         // Set small icon

	// Ask Mfc to create/insert a column
	//UpdateData(TRUE);
	m_listCtrl.SetExtendedStyle(LVS_EX_GRIDLINES); //LVS_EX_CHECKBOXES; LVS_EX_FULLROWSELECT; 
	m_listCtrl.InsertColumn(
		0,              // Rank of item 
		L"ID",          // Caption for this header 
		LVCFMT_LEFT,    // Relative position of items under header 
		70);           // Width of items under header

	m_listCtrl.InsertColumn(1, L"Date", LVCFMT_CENTER, 150);
	m_listCtrl.InsertColumn(2, L"Time", LVCFMT_CENTER, 150);
	m_listCtrl.InsertColumn(3, L"Number", LVCFMT_CENTER, 150); // LVCFMT_LEFT

	// Load and read data from txt file in each line
	// Date  || Time || Number
	vector<CString> vectorResult_line_data;
	CStdioFile Log_StdFile;
	CFileException Log_ex;
	CString lineText;
	// open the source file for reading and save as vector
	if (!Log_StdFile.Open(global_filename, CFile::modeNoTruncate | CFile::modeRead, &Log_ex)) {
		CString error;
		error.Format(L"Cannot open file " + global_filename + L"!\nCause = % d", Log_ex.m_cause);
		AfxMessageBox(error);
		int message = AfxMessageBox(L"Create new file " + global_filename + L"?", MB_YESNO);
		if (message == IDYES) {
			Log_StdFile.Open(global_filename, CFile::modeCreate, &Log_ex);
			Log_StdFile.Close();
		}
		// close window
		this->SendMessage(WM_CLOSE);
		return FALSE;

	}
	else {
		//int iLineCount = 0;
		vectorResult_line_data.clear();
		while (Log_StdFile.ReadString(lineText)) {
			// delete the only \n data in file
			if (lineText == L"") {
				continue;
			}
			else {
				vectorResult_line_data.push_back(lineText);
				//iLineCount++;
			}
		}
		Log_StdFile.Close();
	}
	
	int nItem;
	for (int k = 0; k < vectorResult_line_data.size(); ++k) { // read data from one line and show
		CString data_ = vectorResult_line_data[k];
		CString rid; // row index
		rid.Format(L"%d", k + 1);
		nItem = m_listCtrl.InsertItem(0, rid); // column 0 -> index
		CString sToken; // content
		int icols = 0; // substring index to extract
		int max_cols = 3;// max columns
		while (AfxExtractSubString(sToken, data_, icols, ' ') && icols < max_cols) {
			m_listCtrl.SetItemText(nItem, // row
								   icols+1, // column
								   sToken); //data
											
			icols++;
		}
	}
	
	return TRUE; // return TRUE unless you set the focus to a control
}
// LogHistory message handlers

void LogHistory::OnBnClickedLogOk() {
	// close window
	this->SendMessage(WM_CLOSE);
	return;
}

void LogHistory::OnBnClickedLogClearData() {
	int message_box = AfxMessageBox(L"Clear All Data!!!!!!!!!!!!!!", MB_YESNO);
	if (message_box == IDYES) {
		CStdioFile Log_StdFile;
		CFileException Log_ex;
		if (!Log_StdFile.Open(global_filename, CFile::modeCreate , &Log_ex)) {
			CString error;
			error.Format(L"Cannot clear data file Result!\nCause = %d", Log_ex.m_cause);
			AfxMessageBox(error);
			return;
		}
		else {
			Log_StdFile.Close();
			// show reulst
			m_listCtrl.DeleteAllItems();
			return;
		}
	}
	else {
		return;
	}
	
}

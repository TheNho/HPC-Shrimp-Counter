#pragma once
#include <vector>

using namespace std;
// LogHistory dialog

class LogHistory : public CDialogEx
{
	DECLARE_DYNAMIC(LogHistory)

public:
	LogHistory(CWnd* pParent = nullptr);   // standard constructor
	virtual ~LogHistory();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOG_VIEW_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listCtrl;
	afx_msg void OnBnClickedLogOk();
	afx_msg void OnBnClickedLogClearData();
};

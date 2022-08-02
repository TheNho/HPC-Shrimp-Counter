// Setting_Window.cpp : implementation file
// This code implemented by The Nho 2022

//#include "pch.h" // error, replace by stdafx.h
//#include "afxdialogex.h"
#include "stdafx.h"
#include "BasicDemo.h"
#include "Setting_Window.h"



// Setting_Window dialog

IMPLEMENT_DYNAMIC(Setting_Window, CDialogEx)

Setting_Window::Setting_Window(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Setting_DIALOG, pParent)
	// Initial the firts display
	, setting_blur_method(_T("MEDIAN"))
	, setting_blur_kernel(5)
	, setting_morpho_type(_T("Open"))
	, setting_morpho_kernel(3)
	, setting_bsg_method(_T("MOG2"))
	, setting_bsg_threshold(10)
	, setting_bsg_shadow(_T("False"))
	, setting_bsg_history(500)
	, setting_iou_threshold(0.3)
	, setting_min_hits(1)
	, setting_max_age(5)
	, setting_line_position(400)
	, setting_max_distance(400) // square (distance^2)
	, setting_avg_area(100)
	, setting_min_area(10)
	, setting_max_area(200)
	, setting_min_width(1)
	, setting_min_height(3)
	, setting_adaptiveThreshold_method(_T("MEAN_C"))
	, setting_adaptiveThreshold_KSize(19)
	, setting_adaptiveThreshold_C(5)
	, setting_tolerance_x(5)
	, setting_adaptiveThreshold_Checked(FALSE) // FALSE -> checked
	, setting_bsg_Checked(TRUE)
	, setting_MyTracking_Checked(FALSE)
	, setting_SORTTracking_Checked(TRUE)
{
		
}

Setting_Window::~Setting_Window()
{
	update_setting = false;
}

void Setting_Window::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_CBString(pDX, ID_BLUR_MEDTHOD, setting_blur_method);
	DDX_Text(pDX, ID_BLUR_KERNEL, setting_blur_kernel);
	DDX_CBString(pDX, ID_MORPHO_METHOD, setting_morpho_type);
	DDX_Text(pDX, ID_MORPHO_KERNEL, setting_morpho_kernel);
	DDX_CBString(pDX, ID_BSG_METHOD, setting_bsg_method);
	DDX_Text(pDX, ID_BSG_THRESHOLD, setting_bsg_threshold);
	DDX_CBString(pDX, ID_BSG_SHADOW, setting_bsg_shadow);
	DDX_Text(pDX, ID_BSG_HISTORY, setting_bsg_history);
	DDX_Text(pDX, ID_IOU_THRESHOLD, setting_iou_threshold);
	DDX_Text(pDX, ID_MIN_HITS, setting_min_hits);
	DDX_Text(pDX, ID_MAX_AGE, setting_max_age);
	DDX_Text(pDX, ID_LINE_POSITION, setting_line_position);
	DDX_Text(pDX, ID_MAX_DISTANCE, setting_max_distance);
	DDX_Text(pDX, ID_AVG_AREA, setting_avg_area);
	DDX_Text(pDX, ID_MIN_AREA, setting_min_area);
	DDX_Text(pDX, ID_MIN_WIDTH, setting_min_width);
	DDX_Text(pDX, ID_MIN_HEIGHT, setting_min_height);
	DDX_CBString(pDX, ID_ADAPTIVETHRESHOLD_METHOD, setting_adaptiveThreshold_method);
	DDX_Text(pDX, ID_ADAPTIVETHRESHOLD_KSIZE, setting_adaptiveThreshold_KSize);
	DDX_Text(pDX, ID_ADAPTIVETHRESHOLD_C, setting_adaptiveThreshold_C);
	DDX_Text(pDX, ID_MAX_AREA, setting_max_area);
	DDX_Text(pDX, IDC_TOLERANCE_X, setting_tolerance_x);
	DDX_Radio(pDX, IDC_RADIO_ADAPTIVETHRESHOLD, setting_adaptiveThreshold_Checked);
	DDX_Radio(pDX, IDC_RADIO_BACKGROUNDSUBTRACTION, setting_bsg_Checked);
	DDX_Radio(pDX, IDC_RADIO_My_Tracking, setting_MyTracking_Checked);
	DDX_Radio(pDX, IDC_RADIO_SORT_TRACKING, setting_SORTTracking_Checked);
}

// initial window
BOOL Setting_Window::OnInitDialog() {
	CDialogEx::OnInitDialog();
	// initial setting set
	if (setting_adaptiveThreshold_Checked == FALSE) {
		EnableAdaptiveThreshold(TRUE);
		EnableBackgroundSubtraction(FALSE);
	}
	else if (setting_bsg_Checked== FALSE){
		EnableAdaptiveThreshold(FALSE);
		EnableBackgroundSubtraction(TRUE);
	}
	if (setting_MyTracking_Checked==FALSE){
		EnableMyTracking(TRUE);
		EnableSORTTracking(FALSE);
	}
	else if (setting_SORTTracking_Checked==FALSE){
		EnableMyTracking(FALSE);
		EnableSORTTracking(TRUE);
	}
	HICON hIconS = AfxGetApp()->LoadIcon(IDI_SETTING_ICON);
	SetIcon(hIconS, TRUE);
	SetIcon(hIconS, FALSE);
	return TRUE;
}
BEGIN_MESSAGE_MAP(Setting_Window, CDialogEx)
	ON_BN_CLICKED(ID_OK, &Setting_Window::OnBnClickedOk)
	ON_BN_CLICKED(ID_CANCEL, &Setting_Window::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO_BACKGROUNDSUBTRACTION, &Setting_Window::OnBnClickedRadioBackgroundsubtraction)
	ON_BN_CLICKED(IDC_RADIO_ADAPTIVETHRESHOLD, &Setting_Window::OnBnClickedRadioAdaptivethreshold)
	ON_BN_CLICKED(IDC_RADIO_My_Tracking, &Setting_Window::OnBnClickedRadioMyTracking)
	ON_BN_CLICKED(IDC_RADIO_SORT_TRACKING, &Setting_Window::OnBnClickedRadioSortTracking)
END_MESSAGE_MAP()


// Setting_Window message handlers
// void load_setting(){
// // load setting from txt file
// }

// void save_setting(){
// save setting to txt file
//}

// check input setting when press button ok, if not display error
// bool check_input_setting(){
//}

void Setting_Window::OnBnClickedOk() {
	bool ret = CheckParameters();
	// Check input setting, if failse axfmessage(), return
	if (ret) {
		// TODO: Add your control notification handler code here
		//bool ret = UpdateData(TRUE);
		UpdateData(TRUE); // update wintext to variables
		update_setting = true;
		//close window
		this->SendMessage(WM_CLOSE);
		return;
	}
	else {
		// AfxMessageBox(L"Error parameters!");
		return;
	}
}

void Setting_Window::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	update_setting = false;
	// close window
	this->SendMessage(WM_CLOSE);
}

bool Setting_Window::CheckParameters() {
	// Check image processing parameters
	vector<CString> cblur_method = { L"MEDIAN", L"AVG", L"GAUSS" };
	vector<CString> cblur_kernel = { L"3", L"5", L"7", L"9" };
	vector<CString> cmorpho_method = { L"Dilate", L"Erode", L"Open", L"Close" };
	vector<CString> cmorpho_kernel = { L"3", L"5", L"7", L"9" };
	CString get_cblur_method, get_cblur_kernel, get_cmorpho_method, get_cmorpho_kernel;
	GetDlgItem(ID_BLUR_MEDTHOD)->GetWindowTextW(get_cblur_method);
	GetDlgItem(ID_BLUR_KERNEL)->GetWindowTextW(get_cblur_kernel);
	GetDlgItem(ID_MORPHO_METHOD)->GetWindowTextW(get_cmorpho_method);
	GetDlgItem(ID_MORPHO_KERNEL)->GetWindowTextW(get_cmorpho_kernel);
	// blur method
	if (find(cblur_method.begin(), cblur_method.end(), get_cblur_method) == cblur_method.end()) {
		AfxMessageBox(L"Blur Method Error!");
		return false;
	}
	// blur kernel
	if (find(cblur_kernel.begin(), cblur_kernel.end(), get_cblur_kernel) == cblur_kernel.end()) {
		AfxMessageBox(L"Blur Kernel Error!");
		return false;
	}
	// morpho method
	if (find(cmorpho_method.begin(), cmorpho_method.end(), get_cmorpho_method) == cmorpho_method.end()) {
		AfxMessageBox(L"Morphological Method Error!");
		return false;
	}
	// morpho kernel
	if (find(cmorpho_kernel.begin(), cmorpho_kernel.end(), get_cmorpho_kernel) == cmorpho_kernel.end()) {
		AfxMessageBox(L"Morphological Kernel Error!");
		return false;
	}

	// sementation to binary
	if (IsDlgButtonChecked(IDC_RADIO_BACKGROUNDSUBTRACTION)) {
		// Check Background Subtraction Parameters
		vector<CString> cbsg_method = { L"MOG2", L"MOG"};
		vector<CString> cbsg_shadow = { L"True", L"False"};
		CString get_bsg_method, get_bsg_shadow, get_bsg_threshold,  get_bsg_history;
		GetDlgItem(ID_BSG_METHOD)->GetWindowTextW(get_bsg_method);
		GetDlgItem(ID_BSG_SHADOW)->GetWindowTextW(get_bsg_shadow);
		GetDlgItem(ID_BSG_THRESHOLD)->GetWindowTextW(get_bsg_threshold);
		GetDlgItem(ID_BSG_HISTORY)->GetWindowTextW(get_bsg_history);
		//bsg method
		if (find(cbsg_method.begin(), cbsg_method.end(), get_bsg_method) == cbsg_method.end()) {
			AfxMessageBox(L"Background Subtraction Method Error!");
			return false;
		}
		//bsg shadow
		if (find(cbsg_shadow.begin(), cbsg_shadow.end(), get_bsg_shadow) == cbsg_shadow.end()) {
			AfxMessageBox(L"Background Subtraction Shadow Error!");
			return false;
		}
		//bsg threshold
		if (CheckFloat(get_bsg_threshold) == true) {
			if (_ttof(get_bsg_threshold) <= 0 || _ttof(get_bsg_threshold) >= 255) {
				AfxMessageBox(L"Background Subtraction Threshold must be (0,255)!");
				return false;
			}
		}
		else {
			AfxMessageBox(L"Background Subtraction Threshold Error!");
			return false;
		}
		// bsg history
		if (CheckInt(get_bsg_history) == false) {
			AfxMessageBox(L"Background Subtraction History must be Int!");
			return false;
		}
	}
	else if (IsDlgButtonChecked(IDC_RADIO_ADAPTIVETHRESHOLD)) {
		// check adaptive threshold
		vector<CString> adaptiveThreshold_method = { L"MEAN_C", L"GAUSSIAN_C" };
		CString get_adaptiveThreshold_method, get_adaptiveThreshold_KSize, get_adaptiveThreshold_C;
		GetDlgItem(ID_ADAPTIVETHRESHOLD_METHOD)->GetWindowTextW(get_adaptiveThreshold_method);
		GetDlgItem(ID_ADAPTIVETHRESHOLD_KSIZE)->GetWindowTextW(get_adaptiveThreshold_KSize);
		GetDlgItem(ID_ADAPTIVETHRESHOLD_C)->GetWindowTextW(get_adaptiveThreshold_C);
		if (find(adaptiveThreshold_method.begin(), adaptiveThreshold_method.end(), get_adaptiveThreshold_method) == adaptiveThreshold_method.end()) {
			AfxMessageBox(L"Adaptive Threshold Method Error!!");
			return false;
		}
		if (CheckInt(get_adaptiveThreshold_KSize) == false) {
			AfxMessageBox(L"Adaptive Threshold KSize must be Int!");
			return false;
		}
		if (CheckInt(get_adaptiveThreshold_C) == false) {
			AfxMessageBox(L"Adaptive Threshold C must be Int!");
			return false;
		}
		if (get_adaptiveThreshold_method == L"GAUSSIAN_C") {
			if (_ttoi(get_adaptiveThreshold_KSize) >= 33) {
				AfxMessageBox(L"KSize GAUSSIAN_C must be < 33!");
				return false;
			}
		}
		if (_ttoi(get_adaptiveThreshold_KSize) % 2 == 0) {
			AfxMessageBox(L"KSize must be odd number!");
			return false;
		}
	}
	else {
		AfxMessageBox(L"Segmentation to Binary Method Error!");
		return false;
	}
	
	// tracking
	if (IsDlgButtonChecked(IDC_RADIO_SORT_TRACKING)) {
		//Check SORT Tracking Parameters
		CString get_iou_threshold, get_min_hits, get_max_age;
		GetDlgItem(ID_IOU_THRESHOLD)->GetWindowTextW(get_iou_threshold);
		GetDlgItem(ID_MIN_HITS)->GetWindowTextW(get_min_hits);
		GetDlgItem(ID_MAX_AGE)->GetWindowTextW(get_max_age);
		// Sort iou threshold
		if (CheckFloat(get_iou_threshold) == true) {
			if (_ttof(get_iou_threshold) <= 0 || _ttof(get_iou_threshold) >= 1) {
				AfxMessageBox(L"IoU Threshold must be (0,1)!");
				return false;
			}
		}
		else {
			AfxMessageBox(L"IoU Threshold Error!");
			return false;
		}
		// sort min hits
		if (CheckInt(get_min_hits) == false) {
			AfxMessageBox(L"Min Hits must be Int!");
			return false;
		}
		// sort max age
		if (CheckInt(get_max_age) == false) {
			AfxMessageBox(L"Max Age must be Int!");
			return false;
		}
	}
	else if (IsDlgButtonChecked(IDC_RADIO_My_Tracking)) {
		CString get_tolerrance_x;
		GetDlgItem(IDC_TOLERANCE_X)->GetWindowTextW(get_tolerrance_x);
		if (CheckInt(get_tolerrance_x) == false) {
			AfxMessageBox(L"Tolerance X must be Int!");
			return false;
		}
	}
	else {
		AfxMessageBox(L"Tracking Method Error!");
		return false;
	}
	
	//Check counting parameters
	CString get_line_position, get_max_square_distance;
	GetDlgItem(ID_LINE_POSITION)->GetWindowTextW(get_line_position);
	GetDlgItem(ID_MAX_DISTANCE)->GetWindowTextW(get_max_square_distance);
	// line position
	if (CheckInt(get_line_position) == true) {
		if (_ttof(get_line_position) <= 0 || _ttof(get_line_position) >= 480) {
			AfxMessageBox(L"Line Position must be Int (0, 480)!");
			return false;
		}
	}
	else {
		AfxMessageBox(L"Line Position must be Int!");
		return false;
	}
	// max square distance
	if (CheckFloat(get_max_square_distance) == false) {
		AfxMessageBox(L"Max Square Distance must be float!");
		return false;
	}

	// Check Detection parameters
	CString get_avg_area, get_min_area, get_max_area, get_min_width, get_min_height;
	GetDlgItem(ID_AVG_AREA)->GetWindowTextW(get_avg_area);
	GetDlgItem(ID_MIN_AREA)->GetWindowTextW(get_min_area);
	GetDlgItem(ID_MAX_AREA)->GetWindowTextW(get_max_area);
	GetDlgItem(ID_MIN_WIDTH)->GetWindowTextW(get_min_width);
	GetDlgItem(ID_MIN_HEIGHT)->GetWindowTextW(get_min_height);
	// Avg area
	if (CheckFloat(get_avg_area) == false) {
		AfxMessageBox(L"Avg Area must be Float!");
		return false;
	}
	// max area
	if (CheckFloat(get_max_area) == false) {
		AfxMessageBox(L"Max Area must be Float!");
		return false;
	}
	// min area
	if (CheckFloat(get_min_area) == false) {
		AfxMessageBox(L"Min Area must be Float!");
		return false;
	}
	// min width
	if (CheckInt(get_min_width) == false) {
		AfxMessageBox(L"Min Width must be Int!");
		return false;
	}
	// min height
	if (CheckInt(get_min_height) == false) {
		AfxMessageBox(L"Min Height must be Int!");
		return false;
	}
	// all check done!
	return true;
}

bool Setting_Window::CheckInt(CString text) {
	if (text == L"")
		return false;
	if (text.SpanIncluding(L"0123456789") == text)
		return true;
	else
		return false;
}
bool Setting_Window::CheckFloat(CString text) {
	if (text == L"") 
		return false;
	if (text.SpanIncluding(L"0123456789.") == text && text.Replace(L".", L"0") < 2)
		return true;
	else
		return false;
}

void Setting_Window::EnableBackgroundSubtraction(BOOL CHECKED) {
	GetDlgItem(ID_BSG_METHOD)->EnableWindow(CHECKED);
	GetDlgItem(ID_BSG_THRESHOLD)->EnableWindow(CHECKED);
	GetDlgItem(ID_BSG_SHADOW)->EnableWindow(CHECKED);
	GetDlgItem(ID_BSG_HISTORY)->EnableWindow(CHECKED);
}
void Setting_Window::EnableAdaptiveThreshold(BOOL CHECKED) {
	GetDlgItem(ID_ADAPTIVETHRESHOLD_METHOD)->EnableWindow(CHECKED);
	GetDlgItem(ID_ADAPTIVETHRESHOLD_KSIZE)->EnableWindow(CHECKED);
	GetDlgItem(ID_ADAPTIVETHRESHOLD_C)->EnableWindow(CHECKED);
}
void Setting_Window::EnableMyTracking(BOOL CHECKED) {
	GetDlgItem(IDC_TOLERANCE_X)->EnableWindow(CHECKED);
}
void Setting_Window::EnableSORTTracking(BOOL CHECKED) {
	GetDlgItem(ID_IOU_THRESHOLD)->EnableWindow(CHECKED);
	GetDlgItem(ID_MIN_HITS)->EnableWindow(CHECKED);
	GetDlgItem(ID_MAX_AGE)->EnableWindow(CHECKED);

}

void Setting_Window::OnBnClickedRadioBackgroundsubtraction()
{
	// TODO: Add your control notification handler code here
	EnableAdaptiveThreshold(FALSE);
	EnableBackgroundSubtraction(TRUE);
}
void Setting_Window::OnBnClickedRadioAdaptivethreshold()
{
	// TODO: Add your control notification handler code here
	EnableAdaptiveThreshold(TRUE);
	EnableBackgroundSubtraction(FALSE);
}
void Setting_Window::OnBnClickedRadioMyTracking()
{
	// TODO: Add your control notification handler code here
	EnableMyTracking(TRUE);
	EnableSORTTracking(FALSE);
}
void Setting_Window::OnBnClickedRadioSortTracking()
{
	// TODO: Add your control notification handler code here
	EnableSORTTracking(TRUE);
	EnableMyTracking(FALSE);
}

// BOOL radio button bi loi
// ban dau khoi tao nhan gia tri TRUE -> khong check
// nhan bo check nhan gia tri -1, checked nhan gia tri 1
// khong check, nhan gia tri 0 = FALSE
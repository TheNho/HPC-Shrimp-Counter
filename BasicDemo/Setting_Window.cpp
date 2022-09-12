// Setting_Window.cpp : implementation file
// This code is implemented by The Nho 2022

//#include "pch.h" // error, replaced by stdafx.h
//#include "afxdialogex.h"
#include "stdafx.h"
#include "BasicDemo.h"
#include "Setting_Window.h"

// Setting_Window dialog
IMPLEMENT_DYNAMIC(Setting_Window, CDialogEx)

// Global variables getting current parameters on the first initial window
extern CString blur_method;
extern int blur_kernel;
extern CString segment_binary_method;
extern CString bgs_method;
extern float bgs_threshold;
extern CString bgs_shadows;
extern int bgs_history;
extern float bsg_learning_rate;
extern CString adaptiveThreshold_method;
extern int adaptiveThreshold_KSize;
extern int adaptiveThreshold_C;
extern CString morphological_method;
extern int morphological_kernel;
extern int morphological_iterations;

extern int line_position;

extern float  distance_threshold;
extern int min_hits;
extern int max_age;

extern double min_area;
extern double max_area;
extern int min_width;
extern int min_height;
extern int max_width;
extern int max_height;
// Image
extern CString flip_image;
extern double image_frame_rate;
extern double image_gain;
extern double image_exposure_time;
// ROI
extern CString ROI_Point_Left_Above;
extern CString ROI_Point_Left_Below;
extern CString ROI_Point_Right_Above;
extern CString ROI_Point_Right_Below;

Setting_Window::Setting_Window(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Setting_DIALOG, pParent)
	// Initial the firts display
	, setting_blur_method(blur_method)
	, setting_blur_kernel(blur_kernel)
	, setting_morpho_type(morphological_method)
	, setting_morpho_kernel(morphological_kernel)
	, setting_morpho_iterations(morphological_iterations)
	, setting_bsg_method(bgs_method)
	, setting_bsg_threshold(bgs_threshold)
	, setting_bsg_shadow(bgs_shadows)
	, setting_bsg_history(bgs_history)
	, setting_bsg_learning_rate(bsg_learning_rate)
	, setting_distance_threshold(distance_threshold)
	, setting_min_hits(min_hits)
	, setting_max_age(max_age)
	, setting_line_position(line_position)
	, setting_min_area(min_area)
	, setting_max_area(max_area)
	, setting_min_width(min_width)
	, setting_min_height(min_height)
	, setting_adaptiveThreshold_method(adaptiveThreshold_method)
	, setting_adaptiveThreshold_KSize(adaptiveThreshold_KSize)
	, setting_adaptiveThreshold_C(adaptiveThreshold_C)
	, setting_adaptiveThreshold_Checked(TRUE) // FALSE -> checked
	, setting_bsg_Checked(TRUE)
	, setting_max_width(max_width)
	, setting_max_height(max_height)
	, setting_flip_image(flip_image)
	, setting_Point_Left_Above(ROI_Point_Left_Above)
	, setting_Point_Left_Below(ROI_Point_Left_Below)
	, setting_Point_Right_Above(ROI_Point_Right_Above)
	, setting_Point_Right_Below(ROI_Point_Right_Below)
	, setting_dir_data_train_svm(_T(""))
	, setting_image_gain(image_gain)
	, setting_image_frame_rate(image_frame_rate)
	, setting_image_exposure_time(image_exposure_time)
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
	DDX_Text(pDX, ID_DISTANCE_THRESHOLD, setting_distance_threshold);
	DDX_Text(pDX, ID_MIN_HITS, setting_min_hits);
	DDX_Text(pDX, ID_MAX_AGE, setting_max_age);
	DDX_Text(pDX, ID_LINE_POSITION, setting_line_position);
	DDX_Text(pDX, ID_MIN_AREA, setting_min_area);
	DDX_Text(pDX, ID_MIN_WIDTH, setting_min_width);
	DDX_Text(pDX, ID_MIN_HEIGHT, setting_min_height);
	DDX_CBString(pDX, ID_ADAPTIVETHRESHOLD_METHOD, setting_adaptiveThreshold_method);
	DDX_Text(pDX, ID_ADAPTIVETHRESHOLD_KSIZE, setting_adaptiveThreshold_KSize);
	DDX_Text(pDX, ID_ADAPTIVETHRESHOLD_C, setting_adaptiveThreshold_C);
	DDX_Text(pDX, ID_MAX_AREA, setting_max_area);
	DDX_Radio(pDX, IDC_RADIO_ADAPTIVETHRESHOLD, setting_adaptiveThreshold_Checked);
	DDX_Radio(pDX, IDC_RADIO_BACKGROUNDSUBTRACTION, setting_bsg_Checked);
	DDX_Text(pDX, IDC_MORPHO_ITERATIONS, setting_morpho_iterations);
	DDX_Text(pDX, ID_MAX_WIDTH, setting_max_width);
	DDX_Text(pDX, ID_MAX_HEIGHT, setting_max_height);
	DDX_CBString(pDX, ID_FLIP_IMAGE, setting_flip_image);
	DDX_Text(pDX, ID_BSG_LEARNING_RATE, setting_bsg_learning_rate);
	DDX_Text(pDX, IDC_POINT_LEFT_ABOVE, setting_Point_Left_Above);
	DDX_Text(pDX, IDC_POINT_LEFT_BELOW, setting_Point_Left_Below);
	DDX_Text(pDX, IDC_POINT_RIGHT_ABOVE, setting_Point_Right_Above);
	DDX_Text(pDX, IDC_POINT_RIGHT_BELOW, setting_Point_Right_Below);
	DDX_Text(pDX, IDC_EDIT_DIR_DATA_TRAIN_SVM, setting_dir_data_train_svm);
	DDX_Text(pDX, IDC_EDIT_IMAGE_GIAN, setting_image_gain);
	DDX_Text(pDX, IDC_EDIT_IMAGE_FRAME_RATE, setting_image_frame_rate);
	DDX_Text(pDX, IDC_EDIT_IMAGE_EXPOSURE_TIME, setting_image_exposure_time);
}

// initial window
BOOL Setting_Window::OnInitDialog() {
	CDialogEx::OnInitDialog();
	// initial setting set
	setting_segment_binary_method = segment_binary_method;
	if (setting_segment_binary_method == L"Adaptive Threshold") {
		setting_adaptiveThreshold_Checked = FALSE;
		setting_bsg_Checked = TRUE;
		EnableAdaptiveThreshold(TRUE);
		EnableBackgroundSubtraction(FALSE);
	}
	else if (setting_segment_binary_method == L"Background Subtraction"){
		setting_adaptiveThreshold_Checked = TRUE;
		setting_bsg_Checked = FALSE;
		EnableAdaptiveThreshold(FALSE);
		EnableBackgroundSubtraction(TRUE);
	}
	HICON hIconS = AfxGetApp()->LoadIcon(IDI_SETTING_ICON);
	SetIcon(hIconS, TRUE);
	SetIcon(hIconS, FALSE);

	//UpdateData(TRUE); // update data from window to variables
	UpdateData(FALSE); // update data from variables to window
	
	return TRUE;
}
BEGIN_MESSAGE_MAP(Setting_Window, CDialogEx)
	ON_BN_CLICKED(ID_OK, &Setting_Window::OnBnClickedOk)
	ON_BN_CLICKED(ID_CANCEL, &Setting_Window::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO_BACKGROUNDSUBTRACTION, &Setting_Window::OnBnClickedRadioBackgroundsubtraction)
	ON_BN_CLICKED(IDC_RADIO_ADAPTIVETHRESHOLD, &Setting_Window::OnBnClickedRadioAdaptivethreshold)
	ON_BN_CLICKED(ID_SAVE, &Setting_Window::OnBnClickedSave)
	ON_BN_CLICKED(ID_LOAD, &Setting_Window::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_BUTTON_TRAIN_SVM, &Setting_Window::OnBnClickedButtonTrainSvm)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_FILE_TRAIN_SVM, &Setting_Window::OnBnClickedButtonLoadFileTrainSvm)
	ON_BN_CLICKED(IDC_BUTTON_SET_DEFAULT, &Setting_Window::OnBnClickedButtonSetDefault)
END_MESSAGE_MAP()

void Setting_Window::OnBnClickedOk() {
	bool ret = CheckParameters();
	// Check input setting, if failse axfmessage(), return
	if (ret) {
		// TODO: Add your control notification handler code here
		//bool ret = UpdateData(TRUE);
		if (svm_trainned==true)
			svm->save("SVM.xml");
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

bool Setting_Window::CheckParameters() { // Check parameters in current window
	// Check flip image
	vector<CString> cflip_image = { L"None", L"X", L"Y" };
	CString get_flip_image;
	GetDlgItem(ID_FLIP_IMAGE)->GetWindowTextW(get_flip_image);
	if (find(cflip_image.begin(), cflip_image.end(), get_flip_image) == cflip_image.end()) {
		AfxMessageBox(L"Flip Image Method Error!");
		return false;
	}
	// Check image processing parameters
	vector<CString> cblur_method = { L"AVG", L"GAUSS" };
	vector<CString> cblur_kernel = { L"1", L"3", L"5", L"7", L"9" };
	vector<CString> cmorpho_method = { L"Dilate", L"Erode", L"Open", L"Close" };
	vector<CString> cmorpho_kernel = { L"1", L"3", L"5", L"7", L"9" };
	CString get_cblur_method, get_cblur_kernel, get_cmorpho_method, get_cmorpho_kernel, get_cmorpho_iterations;
	GetDlgItem(ID_BLUR_MEDTHOD)->GetWindowTextW(get_cblur_method);
	GetDlgItem(ID_BLUR_KERNEL)->GetWindowTextW(get_cblur_kernel);
	GetDlgItem(ID_MORPHO_METHOD)->GetWindowTextW(get_cmorpho_method);
	GetDlgItem(ID_MORPHO_KERNEL)->GetWindowTextW(get_cmorpho_kernel);
	GetDlgItem(IDC_MORPHO_ITERATIONS)->GetWindowTextW(get_cmorpho_iterations);
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
	// morpho iterations
	if (CheckInt(get_cmorpho_iterations) == true) {
		if (_ttoi(get_cmorpho_iterations) <= 0) {
			AfxMessageBox(L"Morphological iterations must be Positive!");
			return false;
		}
	}
	else {
		AfxMessageBox(L"Morphological iterations must be Int!");
		return false;
	}
	// sementation to binary
	if (IsDlgButtonChecked(IDC_RADIO_BACKGROUNDSUBTRACTION)) {
		// Check Background Subtraction Parameters
		vector<CString> cbsg_method = { L"MOG2"};
		vector<CString> cbsg_shadow = { L"True", L"False"};
		CString get_bsg_method, get_bsg_shadow, get_bsg_threshold,  get_bsg_history, get_bsg_learning_rate;
		GetDlgItem(ID_BSG_METHOD)->GetWindowTextW(get_bsg_method);
		GetDlgItem(ID_BSG_SHADOW)->GetWindowTextW(get_bsg_shadow);
		GetDlgItem(ID_BSG_THRESHOLD)->GetWindowTextW(get_bsg_threshold);
		GetDlgItem(ID_BSG_HISTORY)->GetWindowTextW(get_bsg_history);
		GetDlgItem(ID_BSG_LEARNING_RATE)->GetWindowTextW(get_bsg_learning_rate);
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
			AfxMessageBox(L"Background Subtraction Threshold Float!");
			return false;
		}
		// bsg history
		if (CheckInt(get_bsg_history) == true) {
			if (_ttoi(get_bsg_history) <=0) {
				AfxMessageBox(L"Background Subtraction History Positive");
				return false;
			}
		}
		else {
			AfxMessageBox(L"Background Subtraction History must be Int!");
			return false;
		}
		// bsg learning rate
		if (CheckFloat(get_bsg_learning_rate) == true) {
			if (_ttof(get_bsg_learning_rate) > 1) {
				AfxMessageBox(L"Background Subtraction Learning Rate must be (0,1)\nNegative for auto Learning Rate");
				return false;
			}
		}
		else {
			AfxMessageBox(L"Background Subtraction Learning Rate must be Float!");
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
		if (_ttoi(get_adaptiveThreshold_C) <= 0) {
			AfxMessageBox(L"Adaptive Threshold C must be Positive!");
			return false;
		}
		if (get_adaptiveThreshold_method == L"GAUSSIAN_C") {
			if (_ttoi(get_adaptiveThreshold_KSize) >= 33) {
				AfxMessageBox(L"KSize GAUSSIAN_C must be < 33!");
				return false;
			}
		}
		if (_ttoi(get_adaptiveThreshold_KSize) <= 0) {
			AfxMessageBox(L"KSize must be Positive!");
			return false;
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
	
	//Check SORT Tracking Parameters
	CString get_distance_threshold, get_min_hits, get_max_age;
	GetDlgItem(ID_DISTANCE_THRESHOLD)->GetWindowTextW(get_distance_threshold);
	GetDlgItem(ID_MIN_HITS)->GetWindowTextW(get_min_hits);
	GetDlgItem(ID_MAX_AGE)->GetWindowTextW(get_max_age);
	// Sort Distance threshold
	if (CheckFloat(get_distance_threshold) == true) {
		if (_ttof(get_distance_threshold) <= 0) {
			AfxMessageBox(L"Distance Threshold must be Positive!");
			return false;
		}
	}
	else {
		AfxMessageBox(L"Distance Threshold must be Float!");
		return false;
	}
	// Sort min hits
	if (CheckInt(get_min_hits) == false) {
		AfxMessageBox(L"Min Hits must be Int!");
		return false;
	}
	if (_ttoi(get_min_hits) <= 0) {
		AfxMessageBox(L"Min Hits must be Positive!");
		return false;
	}
	// sort max age
	if (CheckInt(get_max_age) == false) {
		AfxMessageBox(L"Max Age must be Int!");
		return false;
	}
	if (_ttoi(get_max_age) <= 0) {
		AfxMessageBox(L"Max Age must be Positive!");
		return false;
	}
	
	//Check counting parameters
	CString get_line_position;
	GetDlgItem(ID_LINE_POSITION)->GetWindowTextW(get_line_position);
	// line position
	if (CheckInt(get_line_position) == true) {
		if (_ttoi(get_line_position) <= 0 || _ttoi(get_line_position) >= Image_Height) {
			CString message;
			message.Format(L"Line Position must be Int (0, %d)!", Image_Height);
			AfxMessageBox(message);
			return false;
		}
	}
	else {
		AfxMessageBox(L"Line Position must be Int!");
		return false;
	}
	// Check Detection parameters
	CString get_min_area, get_max_area, get_min_width, get_max_width, get_min_height, get_max_height;
	GetDlgItem(ID_MIN_AREA)->GetWindowTextW(get_min_area);
	GetDlgItem(ID_MAX_AREA)->GetWindowTextW(get_max_area);
	GetDlgItem(ID_MIN_WIDTH)->GetWindowTextW(get_min_width);
	GetDlgItem(ID_MIN_HEIGHT)->GetWindowTextW(get_min_height);
	GetDlgItem(ID_MAX_WIDTH)->GetWindowTextW(get_max_width);
	GetDlgItem(ID_MAX_HEIGHT)->GetWindowTextW(get_max_height);
	// max area
	if (CheckFloat(get_max_area) == false) {
		AfxMessageBox(L"Max Area must be Float!");
		return false;
	}
	if (_ttof(get_max_area) <= 0) {
		AfxMessageBox(L"Max Area must be Postitive!");
		return false;
	}
	// min area
	if (CheckFloat(get_min_area) == false) {
		AfxMessageBox(L"Min Area must be Float!");
		return false;
	}
	if (_ttof(get_min_area) <= 0) {
		AfxMessageBox(L"Min Area must be Postitive!");
		return false;
	}
	// min width
	if (CheckInt(get_min_width) == false) {
		AfxMessageBox(L"Min Width must be Int!");
		return false;
	}
	if (_ttoi(get_min_width) <= 0) {
		AfxMessageBox(L"Min Width must be Postitive!");
		return false;
	}
	// max width
	if (CheckInt(get_max_width) == false) {
		AfxMessageBox(L"Max Width must be Int!");
		return false;
	}
	if (_ttoi(get_max_width) <= 0) {
		AfxMessageBox(L"Max Width must be Postitive!");
		return false;
	}
	// min height
	if (CheckInt(get_min_height) == false) {
		AfxMessageBox(L"Min Height must be Int!");
		return false;
	}
	if (_ttoi(get_min_height) <= 0) {
		AfxMessageBox(L"Min Height must be Postitive!");
		return false;
	}
	// max height
	if (CheckInt(get_max_height) == false) {
		AfxMessageBox(L"Max Height must be Int!");
		return false;
	}
	if (_ttoi(get_max_height) <= 0) {
		AfxMessageBox(L"Max Height must be Postitive!");
		return false;
	}
	// Check ROI
	CString get_Point_LA, get_Point_LB, get_Point_RA, get_Point_RB;
	GetDlgItem(IDC_POINT_LEFT_ABOVE)->GetWindowTextW(get_Point_LA);
	GetDlgItem(IDC_POINT_LEFT_BELOW)->GetWindowTextW(get_Point_LB);
	GetDlgItem(IDC_POINT_RIGHT_ABOVE)->GetWindowTextW(get_Point_RA);
	GetDlgItem(IDC_POINT_RIGHT_BELOW)->GetWindowTextW(get_Point_RB);
	if (get_Point_LA.Replace(L",", L";") != 1) {
		AfxMessageBox(L"Point Left Above Error!");
		return false;
	}
	if (get_Point_LB.Replace(L",", L";") != 1) {
		AfxMessageBox(L"Point Left Below Error!");
		return false;
	}
	if (get_Point_RA.Replace(L",", L";") != 1) {
		AfxMessageBox(L"Point Right Above Error!");
		return false;
	}
	if (get_Point_RB.Replace(L",", L";") != 1) {
		AfxMessageBox(L"Point Right Below Error!");
		return false;
	}
	CString sToken;
	AfxExtractSubString(sToken, get_Point_LA, 0, ';');
	if (!CheckInt(sToken) || _ttoi(sToken) < 0 || _ttoi(sToken) > Image_Width) {
		AfxMessageBox(L"Point Left Above Error!");
		return false;
	}
	AfxExtractSubString(sToken, get_Point_LA, 1, ';');
	if (!CheckInt(sToken) || _ttoi(sToken) < 0 || _ttoi(sToken) > Image_Height) {
		AfxMessageBox(L"Point Left Above Error!");
		return false;
	}
	AfxExtractSubString(sToken, get_Point_LB, 0, ';');
	if (!CheckInt(sToken) || _ttoi(sToken) < 0 || _ttoi(sToken) > Image_Width) {
		AfxMessageBox(L"Point Left Below Error!");
		return false;
	}
	AfxExtractSubString(sToken, get_Point_LB, 1, ';');
	if (!CheckInt(sToken) || _ttoi(sToken) < 0 || _ttoi(sToken) > Image_Height) {
		AfxMessageBox(L"Point Left Below Error!");
		return false;
	}
	AfxExtractSubString(sToken, get_Point_RA, 0, ';');
	if (!CheckInt(sToken) || _ttoi(sToken) < 0 || _ttoi(sToken) > Image_Width) {
		AfxMessageBox(L"Point Right Above Error!");
		return false;
	}
	AfxExtractSubString(sToken, get_Point_RA, 1, ';');
	if (!CheckInt(sToken) || _ttoi(sToken) < 0 || _ttoi(sToken) > Image_Height) {
		AfxMessageBox(L"Point Right Above Error!");
		return false;
	}
	AfxExtractSubString(sToken, get_Point_RB, 0, ';');
	if (!CheckInt(sToken) || _ttoi(sToken) < 0 || _ttoi(sToken) > Image_Width) {
		AfxMessageBox(L"Point Right Below Error!");
		return false;
	}
	AfxExtractSubString(sToken, get_Point_RB, 1, ';');
	if (!CheckInt(sToken) || _ttoi(sToken) < 0 || _ttoi(sToken) > Image_Height) {
		AfxMessageBox(L"Point Right Below Error!");
		return false;
	}
	// Check Image
	CString get_frame_rate, get_gian, get_exposure_time;
	GetDlgItem(IDC_EDIT_IMAGE_FRAME_RATE)->GetWindowTextW(get_frame_rate);
	GetDlgItem(IDC_EDIT_IMAGE_GIAN)->GetWindowTextW(get_gian);
	GetDlgItem(IDC_EDIT_IMAGE_EXPOSURE_TIME)->GetWindowTextW(get_exposure_time);
	if (CheckFloat(get_frame_rate) == true) {
		if (_ttof(get_frame_rate) < 0 || _ttof(get_frame_rate) > 815) {
			AfxMessageBox(L"Frame Rate Error!");
			return false;
		}
	}
	else {
		AfxMessageBox(L"Frame Rate Error!");
		return false;
	}
	if (CheckFloat(get_gian) == true) {
		if (_ttof(get_gian) < 0 || _ttof(get_gian) > 15) {
			AfxMessageBox(L"Gain Error!");
			return false;
		}
	}
	else {
		AfxMessageBox(L"Gain Error!");
		return false;
	}
	if (CheckFloat(get_exposure_time) == true) {
		if (_ttof(get_exposure_time) < 0) {
			AfxMessageBox(L"Exposure Time Error!");
			return false;
		}
	}
	else {
		AfxMessageBox(L"Gian Time Error!");
		return false;
	}
	// all check done!
	return true;
}

bool Setting_Window::CheckInt(CString text) {
	if (text == L"")
		return false;
	if (text.SpanIncluding(L"-0123456789") == text)
		return true;
	else
		return false;
}
bool Setting_Window::CheckFloat(CString text) {
	if (text == L"") 
		return false;
	if (text.SpanIncluding(L"-0123456789.") == text && text.Replace(L".", L"0") < 2)
		return true;
	else
		return false;
}

// Enable/ Disable some elements
void Setting_Window::EnableBackgroundSubtraction(BOOL CHECKED) {
	GetDlgItem(ID_BSG_METHOD)->EnableWindow(CHECKED);
	GetDlgItem(ID_BSG_THRESHOLD)->EnableWindow(CHECKED);
	GetDlgItem(ID_BSG_SHADOW)->EnableWindow(CHECKED);
	GetDlgItem(ID_BSG_HISTORY)->EnableWindow(CHECKED);
	GetDlgItem(ID_BSG_LEARNING_RATE)->EnableWindow(CHECKED);
}
void Setting_Window::EnableAdaptiveThreshold(BOOL CHECKED) {
	GetDlgItem(ID_ADAPTIVETHRESHOLD_METHOD)->EnableWindow(CHECKED);
	GetDlgItem(ID_ADAPTIVETHRESHOLD_KSIZE)->EnableWindow(CHECKED);
	GetDlgItem(ID_ADAPTIVETHRESHOLD_C)->EnableWindow(CHECKED);
}

void Setting_Window::OnBnClickedRadioBackgroundsubtraction()
{
	// TODO: Add your control notification handler code here
	EnableAdaptiveThreshold(FALSE);
	EnableBackgroundSubtraction(TRUE);
	setting_segment_binary_method = L"Background Subtraction";
}
void Setting_Window::OnBnClickedRadioAdaptivethreshold()
{
	// TODO: Add your control notification handler code here
	EnableAdaptiveThreshold(TRUE);
	EnableBackgroundSubtraction(FALSE);
	setting_segment_binary_method = L"Adaptive Threshold";
}

CString Setting_Window::get_parameters_from_window() {
	// Get parameters to file
	CString data;
	data = data + L"Hao Phuong - Parameters File" + L"\n"; // signature // line 0
	// Blur
	CString blur_data;
	data = data + L"Blur_Method:" + setting_blur_method + L"\n"; // line 1
	blur_data.Format(L"%d", setting_blur_kernel);
	data = data + L"Blur_Kernel:" + blur_data + L"\n"; // line 2
	data = data + L"Mopho_Method:" + setting_morpho_type + L"\n"; // line 3
	blur_data.Format(L"%d", setting_morpho_kernel);
	data = data + L"Morpho_Kernel:" + blur_data + L"\n"; // line 4
	blur_data.Format(L"%d", setting_morpho_iterations);
	data = data + L"Morpho_iterations:" + blur_data + L"\n"; // line 5
	// detection
	CString detection_data;
	detection_data.Format(L"%f", setting_min_area);
	data = data + L"Min_Area:" + detection_data + L"\n"; // line 6
	detection_data.Format(L"%f", setting_max_area);
	data = data + L"Max_Area:" + detection_data + L"\n"; // line 7
	detection_data.Format(L"%d", setting_min_width);
	data = data + L"Min_Width:" + detection_data + L"\n"; // line 8
	detection_data.Format(L"%d", setting_max_width);
	data = data + L"Max_Width:" + detection_data + L"\n"; // line 9
	detection_data.Format(L"%d", setting_min_height);
	data = data + L"Min_Height:" + detection_data + L"\n"; // line 10
	detection_data.Format(L"%d", setting_max_height);
	data = data + L"Max_Height:" + detection_data + L"\n"; // line 11
	// counting
	CString counting_data;
	counting_data.Format(L"%d", setting_line_position);
	data = data + L"Line_Position:" + counting_data + L"\n"; // line 12
	// detetion method
	data = data + "Segment_To_Binary_Method:" + setting_segment_binary_method + L"\n"; // line 13
	CString data_detection_method;
	// Background Subtraction
	data = data + L"Background_Subtraction_Method:" + setting_bsg_method + L"\n"; // line 14
	data = data + L"Background_Subtraction_Shadow:" + setting_bsg_shadow + L"\n"; // line 15
	data_detection_method.Format(L"%f", setting_bsg_threshold);
	data = data + L"Background_Subtraction_Threshold:" + data_detection_method + L"\n"; // line 16
	data_detection_method.Format(L"%d", setting_bsg_history);
	data = data + L"Background_Subtraction_History:" + data_detection_method + L"\n"; // line 17
	data_detection_method.Format(L"%f", setting_bsg_learning_rate);
	data = data + L"Background_Subtraction_Learning_Rate:" + data_detection_method + L"\n"; // line 18
	// Adaptive Threshod
	data = data + L"Adaptive_Threshod_Method:" + setting_adaptiveThreshold_method + L"\n"; // line 19
	data_detection_method.Format(L"%d", setting_adaptiveThreshold_KSize);
	data = data + L"KSize:" + data_detection_method + L"\n"; // line 20
	data_detection_method.Format(L"%d", setting_adaptiveThreshold_C);
	data = data + L"C:" + data_detection_method + L"\n"; // line 21
	// SORT tracking
	CString SORT_data;
	SORT_data.Format(L"%f", setting_distance_threshold);
	data = data + L"Distance_Threshold:" + SORT_data + L"\n"; // line 22
	SORT_data.Format(L"%d", setting_min_hits);
	data = data + L"Min_Hits:" + SORT_data + L"\n"; // line 23
	SORT_data.Format(L"%d", setting_max_age);
	data = data + L"Max_Age:" + SORT_data + L"\n"; // line 24
	// Flip image
	data = data + L"Flip_Image:" + setting_flip_image + L"\n"; // line 25
	// ROI parameters
	data = data + L"ROI_Point_Left_Above:" +setting_Point_Left_Above + L"\n"; // line 26
	data = data + L"ROI_Point_Left_Below:" +setting_Point_Left_Below + L"\n"; // line 27
	data = data + L"ROI_Point_Right_Above:" +setting_Point_Right_Above + L"\n"; // line 28
	data = data + L"ROI_Point_Right_Below:" +setting_Point_Right_Below + L"\n"; // line 29
	// Image parameters
	CString Image_data;
	Image_data.Format(L"%f", setting_image_frame_rate);
	data = data + L"Image_Frame_Rate:" + Image_data + L"\n"; // line 30
	Image_data.Format(L"%f", setting_image_gain);
	data = data + L"Image_Gain:" + Image_data + L"\n"; // line 31
	Image_data.Format(L"%f", setting_image_exposure_time);
	data = data + L"Image_Exposure_Time:" + Image_data + L"\n"; // line 32

	return data;
}
void Setting_Window::OnBnClickedSave() {
	bool ret = CheckParameters();
	if (ret == false) {
		return;
	}
	UpdateData(TRUE);
	LPCTSTR pszFilter = _T("Parameters(*.parameters)|*.parameters||");
	CFileDialog dlgFile(FALSE, _T("parameters"), _T("Untitled.parameters"), OFN_OVERWRITEPROMPT, pszFilter, AfxGetMainWnd());
	if (IDOK == dlgFile.DoModal()) {
		try {
			CStdioFile file(dlgFile.GetPathName(), CFile::modeCreate | CFile::modeWrite | CFile::typeText);
			if (dlgFile.GetFileExt() == _T("parameters")) {
				CString save_data = get_parameters_from_window();
				file.WriteString(save_data);
			}
			file.Close();
		}
		catch (CFileException* pe) {
			CString error;
			error.Format(L"File could not be saved!\nCause = %d ", pe->m_cause);
			AfxMessageBox(error);
			pe->Delete();
		}	
	}
	return;
}

BOOL Setting_Window::get_parameters_from_file(CString setting_filename) {
	CStdioFile Paras_File;
	CFileException Log_ex;
	vector<CString> vector_get_parameters;
	// get all line text save to vector
	if (!Paras_File.Open(setting_filename, CFile::modeNoTruncate | CFile::modeRead, &Log_ex)) { 
		CString error;
		error.Format(L"Cannot open file Setting!\nCause = %d!", Log_ex.m_cause);
		AfxMessageBox(error);
		return FALSE;
	}
	else {
		// check empty file
		if (Paras_File.GetLength() == 0) {
			AfxMessageBox(L"File is empty!");
			return FALSE;
		}
		CString lineText;
		vector_get_parameters.clear(); // dont need
		// Read data in each line and save in vector_get_parameters
		while (Paras_File.ReadString(lineText)) { // error if file empty
			// delete the only \n data in file
			if (lineText == L"") {
				continue;
			}
			else {
				vector_get_parameters.push_back(lineText);
			}
		}
		Paras_File.Close();
	}
	// Check signature
	CString data_ = vector_get_parameters[0];
	if (data_ != L"Hao Phuong - Parameters File") {
		AfxMessageBox(L"File is not Hao Phuong parameters file!");
		return FALSE;
	}
	// Get all parameters
	data_ = vector_get_parameters[1];
	data_.Replace(L"Blur_Method:", L"");
	setting_blur_method = data_;

	data_ = vector_get_parameters[2];
	data_.Replace(L"Blur_Kernel:", L"");
	setting_blur_kernel = _ttoi(data_);

	data_ = vector_get_parameters[3];
	data_.Replace(L"Mopho_Method:", L"");
	setting_morpho_type = data_;

	data_ = vector_get_parameters[4];
	data_.Replace(L"Morpho_Kernel:", L"");
	setting_morpho_kernel = _ttoi(data_);

	data_ = vector_get_parameters[5];
	data_.Replace(L"Morpho_iterations:", L"");
	setting_morpho_iterations = _ttoi(data_);

	data_ = vector_get_parameters[6];
	data_.Replace(L"Min_Area:", L"");
	setting_min_area = _ttoi(data_);

	data_ = vector_get_parameters[7];
	data_.Replace(L"Max_Area:", L"");
	setting_max_area = _ttoi(data_);

	data_ = vector_get_parameters[8];
	data_.Replace(L"Min_Width:", L"");
	setting_min_width = _ttoi(data_);

	data_ = vector_get_parameters[9];
	data_.Replace(L"Max_Width:", L"");
	setting_max_width = _ttoi(data_);

	data_ = vector_get_parameters[10];
	data_.Replace(L"Min_Height:", L"");
	setting_min_height = _ttoi(data_);

	data_ = vector_get_parameters[11];
	data_.Replace(L"Max_Height:", L"");
	setting_max_height = _ttoi(data_);

	data_ = vector_get_parameters[12];
	data_.Replace(L"Line_Position:", L"");
	setting_line_position = _ttoi(data_);

	data_ = vector_get_parameters[13];
	data_.Replace(L"Segment_To_Binary_Method:", L"");
	setting_segment_binary_method = data_;
	if (setting_segment_binary_method == L"Background Subtraction") {
		setting_adaptiveThreshold_Checked = TRUE;
		setting_bsg_Checked = FALSE;
		EnableAdaptiveThreshold(FALSE);
		EnableBackgroundSubtraction(TRUE);

		CString data_BSG = vector_get_parameters[14];
		data_BSG.Replace(L"Background_Subtraction_Method:", L"");
		setting_bsg_method = data_BSG;

		data_BSG = vector_get_parameters[15];
		data_BSG.Replace(L"Background_Subtraction_Shadow:", L"");
		setting_bsg_shadow = data_BSG;

		data_BSG = vector_get_parameters[16];
		data_BSG.Replace(L"Background_Subtraction_Threshold:", L"");
		setting_bsg_threshold = _ttof(data_BSG);

		data_BSG = vector_get_parameters[17];
		data_BSG.Replace(L"Background_Subtraction_History:", L"");
		setting_bsg_history = _ttoi(data_BSG);

		data_BSG = vector_get_parameters[18];
		data_BSG.Replace(L"Background_Subtraction_Learning_Rate:", L"");
		setting_bsg_learning_rate = _ttof(data_BSG);
	}
	else if (setting_segment_binary_method == L"Adaptive Threshold") {
		setting_adaptiveThreshold_Checked = FALSE;
		setting_bsg_Checked = TRUE;
		EnableAdaptiveThreshold(TRUE);
		EnableBackgroundSubtraction(FALSE);

		CString data_AT = vector_get_parameters[19];
		data_AT.Replace(L"Adaptive_Threshod_Method:", L"");
		setting_adaptiveThreshold_method = data_AT;

		data_AT = vector_get_parameters[20];
		data_AT.Replace(L"KSize:", L"");
		setting_adaptiveThreshold_KSize = _ttoi(data_AT);

		data_AT = vector_get_parameters[21];
		data_AT.Replace(L"C:", L"");
		setting_adaptiveThreshold_C = _ttoi(data_AT);
	}
	else {
		AfxMessageBox(L"Segment to Binary Method in Setting File Error!");
		return FALSE;
	}

	CString data_SORT = vector_get_parameters[22];
	data_SORT.Replace(L"Distance_Threshold:", L"");
	setting_distance_threshold = _ttof(data_SORT);

	data_SORT = vector_get_parameters[23];
	data_SORT.Replace(L"Min_Hits:", L"");
	setting_min_hits = _ttoi(data_SORT);

	data_SORT = vector_get_parameters[24];
	data_SORT.Replace(L"Max_Age:", L"");
	setting_max_age = _ttoi(data_SORT);
	
	CString data_flip_image;
	data_flip_image = vector_get_parameters[25];
	data_flip_image.Replace(L"Flip_Image:", L"");
	setting_flip_image = data_flip_image;

	CString ROI_data;
	ROI_data = vector_get_parameters[26];
	ROI_data.Replace(L"ROI_Point_Left_Above:", L"");
	setting_Point_Left_Above = ROI_data;
	ROI_data = vector_get_parameters[27];
	ROI_data.Replace(L"ROI_Point_Left_Below:", L"");
	setting_Point_Left_Below = ROI_data;
	ROI_data = vector_get_parameters[28];
	ROI_data.Replace(L"ROI_Point_Right_Above:", L"");
	setting_Point_Right_Above = ROI_data;
	ROI_data = vector_get_parameters[29];
	ROI_data.Replace(L"ROI_Point_Right_Below:", L"");
	setting_Point_Right_Below = ROI_data;

	CString Image_data;
	Image_data = vector_get_parameters[30];
	Image_data.Replace(L"Image_Frame_Rate:", L"");
	setting_image_frame_rate = _ttof(Image_data);
	Image_data = vector_get_parameters[31];
	Image_data.Replace(L"Image_Gain:", L"");
	setting_image_gain = _ttof(Image_data);
	Image_data = vector_get_parameters[32];
	Image_data.Replace(L"Image_Exposure_Time:", L"");
	setting_image_exposure_time = _ttof(Image_data);

	return TRUE;
}
void Setting_Window::OnBnClickedLoad() {
	
	LPCTSTR pszFilter = _T("Parameters(*.parameters)|*.parameters||");
	CFileDialog parasFile(TRUE, _T("parameters"), NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, pszFilter);
	if (IDOK == parasFile.DoModal()) {
		//CStdioFile file(parasFile.GetFileName(), CFile::modeRead);
		if (parasFile.GetFileExt() == _T("parameters")) {
			BOOL ret1 = get_parameters_from_file(parasFile.GetFolderPath() + L"\\" + parasFile.GetFileName());
			if (TRUE == ret1) {
				UpdateData(FALSE); // update parameters to window
				bool ret2 = CheckParameters();
				if (true == ret2) {
					AfxMessageBox(L"Load Setting Success!!!!");
				}
			}
			else {
				UpdateData(TRUE); //update wintext to parameters to save current window
			}
		}
		else {
			AfxMessageBox(L"Error while loading file!!!!");
		}
	}
	return;
}

bool Setting_Window::load_data_to_train_SVM(CString direction) { // Tested -> OK
	// format in file
	// label hu[0] hu[1] hu[2] hu[3] hu[4] hu[5] hu[6] \n
	svm->setType(ml::SVM::C_SVC);
	svm->setKernel(ml::SVM::RBF);
	svm->setTermCriteria(TermCriteria(TermCriteria::EPS + cv::TermCriteria::MAX_ITER,
									  1000,	// number iteration to compute
									  1e-6));	// the desired accuracy
	String file_name = CStringA(direction); // convert CString to cv::String
	Ptr<ml::TrainData> PtrTrainData = ml::TrainData::loadFromCSV(file_name, 
																	0, // headerLineCount
																	0, // response Start Idx 
																	1, // response End Idx 
																	String(), 
																	32, // delimiter, 32 for " "
																	63); //missch, 63 for "?"
	if (PtrTrainData == nullptr) { // check load file to PtTrainData
		AfxMessageBox(L"File Error!");
		return false;
	}
	PtrTrainData->setTrainTestSplitRatio(0.8);
	bool ret = svm->train(PtrTrainData);
	if (ret) { 
		AfxMessageBox(L"Trainning SVM Success");
		return true;
	}
	return false;
}

void Setting_Window::OnBnClickedButtonTrainSvm()
{
	UpdateData(TRUE);
	svm_trainned = load_data_to_train_SVM(setting_dir_data_train_svm);
	// TODO: Add your control notification handler code here
}

void Setting_Window::OnBnClickedButtonLoadFileTrainSvm()
{
	LPCTSTR pszFilter = _T("SVMDataFile(*.svm)|*.svm||");
	CFileDialog parasFile(TRUE, _T("svm"), NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, pszFilter);
	if (IDOK == parasFile.DoModal()) {
		if (parasFile.GetFileExt() == _T("svm")) {
			setting_dir_data_train_svm = parasFile.GetFolderPath() + L"\\" + parasFile.GetFileName();
			UpdateData(FALSE); // update parameters to window
		}
	}
}

// BOOL radio button
// ban dau khoi tao nhan gia tri TRUE -> khong check
// nhan bo check nhan gia tri -1, checked nhan gia tri 1
// khong check, nhan gia tri 0 = FALSE

void Setting_Window::OnBnClickedButtonSetDefault()
{
	// TODO: Add your control notification handler code here
}

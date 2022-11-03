// Setting_Window.cpp : implementation file
// This code is implemented by The Nho 2022

//#include "pch.h" // error, replaced by stdafx.h
//#include "afxdialogex.h"
#include "stdafx.h"
#include "BasicDemo.h"
#include "Setting_Window.h"

// Setting_Window dialog
IMPLEMENT_DYNAMIC(Setting_Window, CDialogEx)

// To initial display parameters
extern Global_Parameters paras;
Setting_Window::Setting_Window(CWnd* pParent )
	: CDialogEx(IDD_Setting_DIALOG, pParent)
	// Initial the firts display
	, setting_blur_method(paras.blur_method)
	, setting_blur_kernel(paras.blur_kernel)
	, setting_morpho_type(paras.morphological_method)
	, setting_morpho_kernel(paras.morphological_kernel)
	, setting_morpho_iterations(paras.morphological_iterations)
	, setting_bsg_method(paras.bgs_method)
	, setting_bsg_threshold(paras.bgs_threshold)
	, setting_bsg_history(paras.bgs_history)
	, setting_bsg_learning_rate(paras.bsg_learning_rate)
	, setting_distance_threshold(paras.distance_threshold)
	, setting_min_hits(paras.min_hits)
	, setting_max_age(paras.max_age)
	, setting_line_position(paras.line_position)
	, setting_min_area(paras.min_area)
	, setting_max_area(paras.max_area)
	, setting_min_width(paras.min_width)
	, setting_min_height(paras.min_height)
	, setting_max_width(paras.max_width)
	, setting_max_height(paras.max_height)
	, setting_flip_image(paras.flip_image)
	, setting_dir_data_train_svm(_T(""))
	, setting_image_gain(paras.image_gain)
	, setting_image_frame_rate(paras.image_frame_rate)
	, setting_image_exposure_time(paras.image_exposure_time)
	, setting_anpha(paras.anpha)
	, setting_beta(paras.beta)
	, setting_bsg_background_ratio(paras.background_ratio)
{
		
}

Setting_Window::~Setting_Window()
{
	update_setting = false;
}

// Mapping parrameters
void Setting_Window::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_CBString(pDX, ID_BLUR_MEDTHOD, setting_blur_method);
	DDX_Text(pDX, ID_BLUR_KERNEL, setting_blur_kernel);
	DDX_CBString(pDX, ID_MORPHO_METHOD, setting_morpho_type);
	DDX_Text(pDX, ID_MORPHO_KERNEL, setting_morpho_kernel);
	DDX_Text(pDX, IDC_MORPHO_ITERATIONS, setting_morpho_iterations);
	DDX_CBString(pDX, ID_BSG_METHOD, setting_bsg_method);
	DDX_Text(pDX, ID_BSG_THRESHOLD, setting_bsg_threshold);
	DDX_Text(pDX, ID_BSG_HISTORY, setting_bsg_history);
	DDX_Text(pDX, ID_DISTANCE_THRESHOLD, setting_distance_threshold);
	DDX_Text(pDX, ID_MIN_HITS, setting_min_hits);
	DDX_Text(pDX, ID_MAX_AGE, setting_max_age);
	DDX_Text(pDX, ID_LINE_POSITION, setting_line_position);
	DDX_Text(pDX, ID_MIN_AREA, setting_min_area);
	DDX_Text(pDX, ID_MIN_WIDTH, setting_min_width);
	DDX_Text(pDX, ID_MIN_HEIGHT, setting_min_height);
	DDX_Text(pDX, ID_MAX_AREA, setting_max_area);
	DDX_Text(pDX, ID_MAX_WIDTH, setting_max_width);
	DDX_Text(pDX, ID_MAX_HEIGHT, setting_max_height);
	DDX_CBString(pDX, ID_FLIP_IMAGE, setting_flip_image);
	DDX_Text(pDX, ID_BSG_LEARNING_RATE, setting_bsg_learning_rate);
	DDX_Text(pDX, IDC_EDIT_DIR_DATA_TRAIN_SVM, setting_dir_data_train_svm);
	DDX_Text(pDX, IDC_EDIT_IMAGE_GIAN, setting_image_gain);
	DDX_Text(pDX, IDC_EDIT_IMAGE_FRAME_RATE, setting_image_frame_rate);
	DDX_Text(pDX, IDC_EDIT_IMAGE_EXPOSURE_TIME, setting_image_exposure_time);
	DDX_Text(pDX, ID_SETTING_ANPHA, setting_anpha);
	DDX_Text(pDX, ID_SETTING_BETA, setting_beta);
	DDX_Text(pDX, ID_BSG_BACKGROUND_RATIO, setting_bsg_background_ratio);
}

// initial window
BOOL Setting_Window::OnInitDialog() {
	CDialogEx::OnInitDialog();
	// initial setting set
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
	ON_BN_CLICKED(ID_SAVE, &Setting_Window::OnBnClickedSave)
	ON_BN_CLICKED(ID_LOAD, &Setting_Window::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_BUTTON_TRAIN_SVM, &Setting_Window::OnBnClickedButtonTrainSvm)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_FILE_TRAIN_SVM, &Setting_Window::OnBnClickedButtonLoadFileTrainSvm)
	ON_BN_CLICKED(IDC_BUTTON_SET_DEFAULT, &Setting_Window::OnBnClickedButtonSetDefault)
END_MESSAGE_MAP()

void Setting_Window::OnBnClickedOk() {
	bool ret = CheckParameters(); // check wintext
	if (ret) {
		// TODO: Add your control notification handler code here
		if (svm_trainned==true)
			svm->save("SVM.xml");
		UpdateData(TRUE); // update wintext to variables
		update_setting = true;
		this->SendMessage(WM_CLOSE); //close window
		return;
	}
	else {
		return;
	}
}

void Setting_Window::OnBnClickedCancel(){
	// TODO: Add your control notification handler code here
	update_setting = false;
	// close window
	this->SendMessage(WM_CLOSE);
}

bool Setting_Window::CheckParameters() { // Check parameters in current window
	// Flip image
	vector<CString> cflip_image = { L"None", L"X", L"Y" };
	CString get_flip_image;
	GetDlgItem(ID_FLIP_IMAGE)->GetWindowTextW(get_flip_image);
	if (find(cflip_image.begin(), cflip_image.end(), get_flip_image) == cflip_image.end()) {
		AfxMessageBox(L"Flip Image Method Error!");
		return false;
	}
	//Camera Parameters
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
	// Enhance Image
	CString get_anpha, get_beta;
	GetDlgItem(ID_SETTING_ANPHA)->GetWindowTextW(get_anpha);
	GetDlgItem(ID_SETTING_BETA)->GetWindowTextW(get_beta);
	if (CheckFloat(get_anpha) == false) {
		AfxMessageBox(L"anhpa must be Float!");
		return false;
	}
	if (CheckFloat(get_beta) == false) {
		AfxMessageBox(L"beta must be Float!");
		return false;
	}
	// Check blur image
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
	// Check Background Subtraction Parameters
	vector<CString> cbsg_method = { L"MOG2"};
	CString get_bsg_method, get_bsg_background_ratio, get_bsg_threshold,  get_bsg_history, get_bsg_learning_rate;
	GetDlgItem(ID_BSG_METHOD)->GetWindowTextW(get_bsg_method);
	GetDlgItem(ID_BSG_BACKGROUND_RATIO)->GetWindowTextW(get_bsg_background_ratio);
	GetDlgItem(ID_BSG_THRESHOLD)->GetWindowTextW(get_bsg_threshold);
	GetDlgItem(ID_BSG_HISTORY)->GetWindowTextW(get_bsg_history);
	GetDlgItem(ID_BSG_LEARNING_RATE)->GetWindowTextW(get_bsg_learning_rate);
	//bsg method
	if (find(cbsg_method.begin(), cbsg_method.end(), get_bsg_method) == cbsg_method.end()) {
		AfxMessageBox(L"Background Subtraction Method Error!");
		return false;
	}
	//bsg background ratio
	if (CheckFloat(get_bsg_background_ratio) == true) {
		if (_ttof(get_bsg_background_ratio) <= 0 || _ttof(get_bsg_background_ratio) >= 1) {
			AfxMessageBox(L"Background Ratio must be (0,1)!");
			return false;
		}
	}
	else {
		AfxMessageBox(L"Background Ratio must be Float!");
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
	
	//Check SORT Tracking Parameters
	CString get_distance_threshold, get_min_hits, get_max_age;
	GetDlgItem(ID_DISTANCE_THRESHOLD)->GetWindowTextW(get_distance_threshold);
	GetDlgItem(ID_MIN_HITS)->GetWindowTextW(get_min_hits);
	GetDlgItem(ID_MAX_AGE)->GetWindowTextW(get_max_age);
	// SORT Distance threshold
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
	// SORT min hits
	if (CheckInt(get_min_hits) == false) {
		AfxMessageBox(L"Min Hits must be Int!");
		return false;
	}
	if (_ttoi(get_min_hits) <= 0) {
		AfxMessageBox(L"Min Hits must be Positive!");
		return false;
	}
	// SORT max age
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

CString Setting_Window::get_parameters_from_window() {
	// Get parameters to file
	CString data, temp_data;
	data = data + L"Hao Phuong - Parameters File" + L"\n"; // signature // line 0
	data = data + L"Flip_Image:" + setting_flip_image + L"\n"; // line 1
	temp_data.Format(L"%f", setting_image_frame_rate);
	data = data + L"Image_Frame_Rate:" + temp_data + L"\n"; // line 2
	temp_data.Format(L"%f", setting_image_gain);
	data = data + L"Image_Gain:" + temp_data + L"\n"; // line 3
	temp_data.Format(L"%f", setting_image_exposure_time);
	data = data + L"Image_Exposure_Time:" + temp_data + L"\n"; // line 4
	data = data + L"Blur_Method:" + setting_blur_method + L"\n"; // line 5
	temp_data.Format(L"%d", setting_blur_kernel);
	data = data + L"Blur_Kernel:" + temp_data + L"\n"; // line 6
	temp_data.Format(L"%f", setting_anpha);
	data = data + L"Image_Enhanced_anpha:" + temp_data + L"\n"; // line 7
	temp_data.Format(L"%f", setting_beta);
	data = data + L"Image_Enhanced_beta:" + temp_data + L"\n"; // line 8
	data = data + L"Background_Subtraction_Method:" + setting_bsg_method + L"\n"; // line 9
	temp_data.Format(L"%f", setting_bsg_threshold);
	data = data + L"Background_Subtraction_Threshold:" + temp_data + L"\n"; // line 10
	temp_data.Format(L"%f", setting_bsg_background_ratio);
	data = data + L"Background_Subtraction_Ratio:" + temp_data + L"\n"; // line 11
	temp_data.Format(L"%d", setting_bsg_history);
	data = data + L"Background_Subtraction_History:" + temp_data + L"\n"; // line 12
	temp_data.Format(L"%f", setting_bsg_learning_rate);
	data = data + L"Background_Subtraction_Learning_Rate:" + temp_data + L"\n"; // line 13
	data = data + L"Mopho_Method:" + setting_morpho_type + L"\n"; // line 14
	temp_data.Format(L"%d", setting_morpho_kernel);
	data = data + L"Morpho_Kernel:" + temp_data + L"\n"; // line 15
	temp_data.Format(L"%d", setting_morpho_iterations);
	data = data + L"Morpho_iterations:" + temp_data + L"\n"; // line 16
	temp_data.Format(L"%d", setting_line_position);
	data = data + L"Line_Position:" + temp_data + L"\n"; // line 17
	temp_data.Format(L"%f", setting_distance_threshold);
	data = data + L"Distance_Threshold:" + temp_data + L"\n"; // line 18
	temp_data.Format(L"%d", setting_min_hits);
	data = data + L"Min_Hits:" + temp_data + L"\n"; // line 19
	temp_data.Format(L"%d", setting_max_age);
	data = data + L"Max_Age:" + temp_data + L"\n"; // line 20
	temp_data.Format(L"%f", setting_min_area);
	data = data + L"Min_Area:" + temp_data + L"\n"; // line 21
	temp_data.Format(L"%f", setting_max_area);
	data = data + L"Max_Area:" + temp_data + L"\n"; // line 22
	temp_data.Format(L"%d", setting_min_width);
	data = data + L"Min_Width:" + temp_data + L"\n"; // line 23
	temp_data.Format(L"%d", setting_max_width);
	data = data + L"Max_Width:" + temp_data + L"\n"; // line 24
	temp_data.Format(L"%d", setting_min_height);
	data = data + L"Min_Height:" + temp_data + L"\n"; // line 25
	temp_data.Format(L"%d", setting_max_height);
	data = data + L"Max_Height:" + temp_data + L"\n"; // line 26

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
	data_ = vector_get_parameters[1];
	data_.Replace(L"Flip_Image:", L"");
	setting_flip_image = data_;

	data_ = vector_get_parameters[2];
	data_.Replace(L"Image_Frame_Rate:", L"");
	setting_image_frame_rate = _ttof(data_);

	data_ = vector_get_parameters[3];
	data_.Replace(L"Image_Gain:", L"");
	setting_image_gain = _ttof(data_);

	data_ = vector_get_parameters[4];
	data_.Replace(L"Image_Exposure_Time:", L"");
	setting_image_exposure_time = _ttof(data_);

	data_ = vector_get_parameters[5];
	data_.Replace(L"Blur_Method:", L"");
	setting_blur_method = data_;
	data_ = vector_get_parameters[6];
	data_.Replace(L"Blur_Kernel:", L"");
	setting_blur_kernel = _ttoi(data_);

	data_ = vector_get_parameters[7];
	data_.Replace(L"Image_Enhanced_anpha:", L"");
	setting_anpha = _ttoi(data_);
	data_ = vector_get_parameters[8];
	data_.Replace(L"Image_Enhanced_beta:", L"");
	setting_beta = _ttoi(data_);

	data_ = vector_get_parameters[9];
	data_.Replace(L"Background_Subtraction_Method:", L"");
	setting_bsg_method = data_;
	data_ = vector_get_parameters[10];
	data_.Replace(L"Background_Subtraction_Threshold:", L"");
	setting_bsg_threshold = _ttof(data_);
	data_ = vector_get_parameters[11];
	data_.Replace(L"Background_Subtraction_Ratio:", L"");
	setting_bsg_background_ratio = _ttof(data_);
	data_ = vector_get_parameters[12];
	data_.Replace(L"Background_Subtraction_History:", L"");
	setting_bsg_history = _ttoi(data_);
	data_ = vector_get_parameters[13];
	data_.Replace(L"Background_Subtraction_Learning_Rate:", L"");
	setting_bsg_learning_rate = _ttof(data_);

	data_ = vector_get_parameters[14];
	data_.Replace(L"Mopho_Method:", L"");
	setting_morpho_type = data_;
	data_ = vector_get_parameters[15];
	data_.Replace(L"Morpho_Kernel:", L"");
	setting_morpho_kernel = _ttoi(data_);
	data_ = vector_get_parameters[16];
	data_.Replace(L"Morpho_iterations:", L"");
	setting_morpho_iterations = _ttoi(data_);

	data_ = vector_get_parameters[17];
	data_.Replace(L"Line_Position:", L"");
	setting_line_position = _ttoi(data_);

	data_ = vector_get_parameters[18];
	data_.Replace(L"Distance_Threshold:", L"");
	setting_distance_threshold = _ttof(data_);
	data_ = vector_get_parameters[19];
	data_.Replace(L"Min_Hits:", L"");
	setting_min_hits = _ttoi(data_);
	data_ = vector_get_parameters[20];
	data_.Replace(L"Max_Age:", L"");
	setting_max_age = _ttoi(data_);

	data_ = vector_get_parameters[21];
	data_.Replace(L"Min_Area:", L"");
	setting_min_area = _ttoi(data_);
	data_ = vector_get_parameters[22];
	data_.Replace(L"Max_Area:", L"");
	setting_max_area = _ttoi(data_);
	data_ = vector_get_parameters[23];
	data_.Replace(L"Min_Width:", L"");
	setting_min_width = _ttoi(data_);
	data_ = vector_get_parameters[24];
	data_.Replace(L"Max_Width:", L"");
	setting_max_width = _ttoi(data_);
	data_ = vector_get_parameters[25];
	data_.Replace(L"Min_Height:", L"");
	setting_min_height = _ttoi(data_);
	data_ = vector_get_parameters[26];
	data_.Replace(L"Max_Height:", L"");
	setting_max_height = _ttoi(data_);

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

void Setting_Window::OnBnClickedButtonTrainSvm(){
	UpdateData(TRUE);
	svm_trainned = load_data_to_train_SVM(setting_dir_data_train_svm);
	// TODO: Add your control notification handler code here
}

void Setting_Window::OnBnClickedButtonLoadFileTrainSvm(){
	LPCTSTR pszFilter = _T("SVMDataFile(*.svm)|*.svm||");
	CFileDialog parasFile(TRUE, _T("svm"), NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, pszFilter);
	if (IDOK == parasFile.DoModal()) {
		if (parasFile.GetFileExt() == _T("svm")) {
			setting_dir_data_train_svm = parasFile.GetFolderPath() + L"\\" + parasFile.GetFileName();
			UpdateData(FALSE); // update parameters to window
		}
	}
}

void Setting_Window::OnBnClickedButtonSetDefault(){
	// Save parameters to file DefaultSettings.parameters
	bool ret = CheckParameters();
	if (ret == false) {
		return;
	}
	UpdateData(TRUE);
	try {
		CStdioFile file(L"DefaultSettings.parameters", CFile::modeCreate | CFile::modeWrite | CFile::typeText);
		CString save_data = get_parameters_from_window();
		file.WriteString(save_data);
		file.Close();
		AfxMessageBox(L"Set default success!");
		}
	catch (CFileException* pe) {
		CString error;
		error.Format(L"File could not be saved!\nCause = %d ", pe->m_cause);
		AfxMessageBox(error);
		pe->Delete();
	}
	return;
}

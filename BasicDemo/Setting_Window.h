#pragma once
#include <vector>
#include<opencv2/core.hpp>
#include <opencv2/ml.hpp>

using namespace cv;
using namespace std;
// Setting_Window dialog

class Setting_Window : public CDialogEx
{
	DECLARE_DYNAMIC(Setting_Window)

public:
	Setting_Window(CWnd* pParent = nullptr);   // standard constructor
	virtual ~Setting_Window();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Setting_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	//CString setting_counting_method;
	
	bool update_setting = false;

	CString setting_blur_method;
	int setting_blur_kernel;

	CString setting_morpho_type;
	int setting_morpho_kernel;
	int setting_morpho_iterations;

	CString setting_bsg_method;
	float setting_bsg_threshold;
	int setting_bsg_history;
	float setting_bsg_learning_rate;
	float setting_bsg_background_ratio;

	float setting_distance_threshold;
	int setting_min_hits;
	int setting_max_age;

	int setting_line_position;

	double setting_min_area;
	double setting_max_area;
	int setting_min_width;
	int setting_min_height;
	int setting_max_width;
	int setting_max_height;

	CString setting_flip_image;
	double setting_image_gain;
	double setting_image_frame_rate;
	double setting_image_exposure_time;
	double setting_anpha;
	double setting_beta;
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
private:
	bool Setting_Window::CheckParameters();
	bool Setting_Window::CheckInt(CString text);
	bool Setting_Window::CheckFloat(CString text);
	bool Setting_Window::load_data_to_train_SVM(CString direction);
	
public:
	BOOL get_parameters_from_file(CString setting_filename);
	CString get_parameters_from_window();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	
private:
	CString setting_dir_data_train_svm;
	Ptr<ml::SVM> svm = ml::SVM::create();
	bool svm_trainned = false;
public:
	afx_msg void OnBnClickedButtonTrainSvm();
	afx_msg void OnBnClickedButtonLoadFileTrainSvm();
	afx_msg void OnBnClickedButtonSetDefault();

};

#pragma once
#include <vector>

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
	bool update_setting = false;
	CString setting_blur_method;
	int setting_blur_kernel;
	CString setting_morpho_type;
	int setting_morpho_kernel;
	CString setting_bsg_method;
	float setting_bsg_threshold;
	CString setting_bsg_shadow;
	int setting_bsg_history;
	float setting_iou_threshold;
	int setting_min_hits;
	int setting_max_age;
	int setting_line_position;
	float setting_max_distance;
	double setting_avg_area;
	double setting_min_area;
	int setting_min_width;
	int setting_min_height;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
private:
	bool Setting_Window::CheckParameters();
	bool Setting_Window::CheckInt(CString text);
	bool Setting_Window::CheckFloat(CString text);
	void Setting_Window::EnableBackgroundSubtraction(BOOL CHECKED);
	void Setting_Window::EnableAdaptiveThreshold(BOOL CHECKED);
	void Setting_Window::EnableMyTracking(BOOL CHECKED);
	void Setting_Window::EnableSORTTracking(BOOL CHECKED);
public:
	CString setting_adaptiveThreshold_method;
	int setting_adaptiveThreshold_KSize;
	int setting_adaptiveThreshold_C;
	double setting_max_area;
	int setting_tolerance_x;
	BOOL setting_adaptiveThreshold_Checked;
	BOOL setting_bsg_Checked;
	BOOL setting_MyTracking_Checked;
	BOOL setting_SORTTracking_Checked;
	afx_msg void OnBnClickedRadioBackgroundsubtraction();
	afx_msg void OnBnClickedRadioAdaptivethreshold();
	afx_msg void OnBnClickedRadioMyTracking();
	afx_msg void OnBnClickedRadioSortTracking();
};

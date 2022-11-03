// BasicDemo.h : PROJECT_NAME

#pragma once
#ifndef __AFXWIN_H__
	#error "ERORR!!!!!!!"
#endif

#include "resource.h"		

#define Image_Width 640
#define Image_Height 480

class CBasicDemoApp : public CWinAppEx
{
public:
	CBasicDemoApp();

	public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
struct Global_Parameters
{
	CString global_filename;

	// blur image
	CString blur_method;
	int blur_kernel;

	// Background Subtraction paprameters
	CString bgs_method;
	float bgs_threshold;
	int bgs_history;
	float bsg_learning_rate;
	float background_ratio;

	// Morphological Parameters
	CString morphological_method;
	int morphological_kernel;
	int morphological_iterations;
	// Counting
	int line_position;
	// SORT parameters
	float  distance_threshold;
	int min_hits;
	int max_age;
	// Detection parameters
	double min_area;
	double max_area;
	int min_width;
	int min_height;
	int max_width;
	int max_height;
	// Image parameters
	CString flip_image;
	double image_frame_rate;
	double image_gain;
	double image_exposure_time;
	// Image Enhance Parameters
	double anpha;
	double beta;
};
extern CBasicDemoApp theApp;

// BasicDemo.cpp: implementation file
#include "stdafx.h"
#include "BasicDemo.h"
#include "BasicDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CBasicDemoApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

// CBasicDemoApp
CBasicDemoApp::CBasicDemoApp()
{
	// TODO: InitInstance 
}

// CBasicDemoApp global
CBasicDemoApp theApp;

// global filename variable using in all windows
CString global_filename;
CString flip_image;
// Parameters Setting default in the first run
// blur image
CString blur_method;
int blur_kernel;

// Background Subtraction paprameters
CString bgs_method;
float bgs_threshold;
CString bgs_shadows;
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
double image_frame_rate;
double image_gain;
double image_exposure_time;
// Image Enhance Parameters
double anpha;
double beta;

BOOL CBasicDemoApp::InitInstance()
{
	//Windows XP 
	//ComCtl32.dll 
	//InitCommonControlsEx()
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	SetRegistryKey(_T("HPC SHRIMPS COUNTER APP"));
	
	// Open Main windown
	CBasicDemoDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Press OK
		 
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Press Cancel
		 
	}

	return FALSE;
}

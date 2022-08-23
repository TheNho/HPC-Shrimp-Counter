
// BasicDemo.cpp: implementation file

#include "stdafx.h"
#include "BasicDemo.h"
#include "BasicDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBasicDemoApp

BEGIN_MESSAGE_MAP(CBasicDemoApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CBasicDemoApp

CBasicDemoApp::CBasicDemoApp()
{
	// TODO: 
	// InitInstance 
}

// CBasicDemoApp global
CBasicDemoApp theApp;

// global filename variable using in all window
CString global_filename;
CString flip_image;
// Parameters Setting default in the first run
	// blur image
CString blur_method;
int blur_kernel;
// segmentation to binary
CString segment_binary_method;
// Background Subtraction paprameters
CString bgs_method;
float bgs_threshold;
CString bgs_shadows;
int bgs_history;
float bsg_learning_rate;
// Adaptive Threshold Parameters
CString adaptiveThreshold_method;
int adaptiveThreshold_KSize;
int adaptiveThreshold_C;
// Morphological Parameters
CString morphological_method;
int morphological_kernel;
int morphological_iterations;

// Counting
int line_position;
float max_distance;
CString counting_method;
// SORT parameters
float  distance_threshold;
int min_hits;
int max_age;
// my simple tracking parameter
int tolerance_x;
// Detection parameters
double avg_area;
double min_area;
double max_area;
int min_width;
int min_height;
int max_width;
int max_height;

// CBasicDemoApp 
BOOL CBasicDemoApp::InitInstance()
{
	//  Windows XP 
	//ComCtl32.dll 
	//InitCommonControlsEx()
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 
	// 
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();


	SetRegistryKey(_T("HPC SHRIMP COUNTER APP"));
	
	// Open Main windown
	CBasicDemoDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 
		 
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 
		 
	}

	//  FALSE 
	  
	
	return FALSE;
}

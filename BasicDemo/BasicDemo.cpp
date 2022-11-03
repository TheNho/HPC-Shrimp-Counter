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
// Global parameters
Global_Parameters paras;

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

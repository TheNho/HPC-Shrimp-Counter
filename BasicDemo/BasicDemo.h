// BasicDemo.h : PROJECT_NAME

#pragma once
// TODO
#ifndef __AFXWIN_H__
	#error "ERORR!!!!!!!"
#endif

#include "resource.h"		

#define Image_Width 640
#define Image_Height 480

// CBasicDemoApp:
// BasicDemo.cpp

class CBasicDemoApp : public CWinAppEx
{
public:
	CBasicDemoApp();

	public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CBasicDemoApp theApp;

//extern CString global_save_filename = L"C:/Users/ADMIN/Desktop/Result.txt"; //error
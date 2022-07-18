
// GOL.h : main header file for the GOL application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CGOLApp:
// See GOL.cpp for the implementation of this class
//

class CGOLApp : public CWinApp
{
public:
	CGOLApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGOLApp theApp;

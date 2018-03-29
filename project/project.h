
// project.h : main header file for the project application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

// commit test
// CprojectApp:
// See project.cpp for the implementation of this class
//

class CprojectApp : public CWinAppEx
{
public:
	CprojectApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CprojectApp theApp;

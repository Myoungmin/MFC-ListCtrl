
// MFC-ListCtrl.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMFCListCtrlApp:
// See MFC-ListCtrl.cpp for the implementation of this class
//

class CMFCListCtrlApp : public CWinApp
{
public:
	CMFCListCtrlApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMFCListCtrlApp theApp;


// ImageProcessing.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// ImageProcessing:
// See ImageProcessing.cpp for the implementation of this class
//

class ImageProcessing : public CWinApp
{
public:
	ImageProcessing();
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
};

extern ImageProcessing theApp;
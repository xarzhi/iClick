
// iClick.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once



#include "resource.h"		// 主符号


// CiClickApp:
// 有关此类的实现，请参阅 iClick.cpp
//

class CiClickApp : public CWinApp
{
public:
	CiClickApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CiClickApp theApp;

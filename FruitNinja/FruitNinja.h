
// FruitNinja.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFruitNinjaApp: 
// �йش����ʵ�֣������ FruitNinja.cpp
//

class CFruitNinjaApp : public CWinApp
{
public:
	CFruitNinjaApp();
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFruitNinjaApp theApp;
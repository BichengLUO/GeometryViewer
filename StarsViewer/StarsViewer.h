
// StarsViewer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CStarsViewerApp: 
// �йش����ʵ�֣������ StarsViewer.cpp
//

class CStarsViewerApp : public CWinApp
{
public:
	CStarsViewerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CStarsViewerApp theApp;
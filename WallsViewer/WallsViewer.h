
// WallsViewer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CWallsViewerApp: 
// �йش����ʵ�֣������ WallsViewer.cpp
//

class CWallsViewerApp : public CWinApp
{
public:
	CWallsViewerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CWallsViewerApp theApp;
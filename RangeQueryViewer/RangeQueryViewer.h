
// RangeQueryViewer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRangeQueryViewerApp: 
// �йش����ʵ�֣������ RangeQueryViewer.cpp
//

class CRangeQueryViewerApp : public CWinApp
{
public:
	CRangeQueryViewerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRangeQueryViewerApp theApp;
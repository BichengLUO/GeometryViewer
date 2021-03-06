
// ConvexHullViewerDlg.h : 头文件
//

#pragma once

#include <vector>
#include "afxwin.h"

typedef struct _point
{
	double x;
	double y;
	_point() : x(0.0), y(0.0) {}
	_point(double xp, double yp) : x(xp), y(yp){}
} point;

typedef std::vector<point> points;
typedef std::vector<int> convex_hull;


// CConvexHullViewerDlg 对话框
class CConvexHullViewerDlg : public CDialogEx
{
// 构造
public:
	CConvexHullViewerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CONVEXHULLVIEWER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	points pts;
	convex_hull ch;
	double dim_min;
	double dim_max;
	bool show_convex_hull;
	bool show_point_id;
	double scale;
	point transform;
	point drag_start_transform;
	bool dragging;
	CPoint drag_start_pos;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	double scale_dim_x(double v);
	double scale_dim_y(double v);
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpenPoints();
	afx_msg void OnBnClickedButtonOpenConvexHull();
	afx_msg void OnBnClickedCheckShowConvexHull();
	afx_msg void OnBnClickedButtonResetView();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CButton show_convex_hull_btn;
	CButton show_point_id_btn;
	afx_msg void OnBnClickedCheckShowPointId();
};

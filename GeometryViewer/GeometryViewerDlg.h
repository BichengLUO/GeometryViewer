
// GeometryViewerDlg.h : 头文件
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

typedef struct _triangle
{
	int i;
	int j;
	int k;
	_triangle() : i(-1), j(-1), k(-1) {}
	_triangle(int ip, int jp, int kp) : i(ip), j(jp), k(kp) {}
} triangle;

typedef std::vector<triangle> triangles;

// CGeometryViewerDlg 对话框
class CGeometryViewerDlg : public CDialogEx
{
// 构造
public:
	CGeometryViewerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GEOMETRYVIEWER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	points polygon_points;
	double polygon_dim_min;
	double polygon_dim_max;
	triangles mesh_triangles;
	bool show_mesh;
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
	afx_msg void OnBnClickedButtonOpenPolygon();
	afx_msg void OnBnClickedButtonOpenMesh();
	// Show mesh
	CButton show_mesh_btn;
	afx_msg void OnBnClickedCheckShowMesh();
//	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButtonResetView();
	afx_msg void OnBnClickedRadioShowAsPolygon();
	afx_msg void OnBnClickedRadioShowAsPoints();
	afx_msg void OnBnClickedCheckShowPointId();
};
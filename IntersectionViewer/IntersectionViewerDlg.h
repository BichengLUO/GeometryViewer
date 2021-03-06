
// IntersectionViewerDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include <vector>

typedef struct _point
{
	double x;
	double y;
	_point() : x(0.0), y(0.0) {}
	_point(double xp, double yp) : x(xp), y(yp){}
} point, vect;
typedef std::vector<point> points;

typedef struct _seg
{
	point start;
	point end;
	_seg() : start(), end() {}
	_seg(point p1, point p2) : start(p1), end(p2) {}
} seg;
typedef std::vector<seg> segs;

typedef struct _ray
{
	point start;
	vect dir;
	_ray() : start(), dir() {}
	_ray(point p1, point p2) : start(p1), dir(p2) {}
} ray;
typedef std::vector<ray> rays;

typedef struct _line
{
	point pos;
	vect dir;
	_line() : pos(), dir() {}
	_line(point p1, point p2) : pos(p1), dir(p2) {}
} line;
typedef std::vector<line> lines;

typedef struct _circle
{
	point center;
	double radius;
	_circle() : center(), radius(0) {}
	_circle(point p, double r) : center(p), radius(r) {}
} circle;
typedef std::vector<circle> circles;

// CIntersectionViewerDlg 对话框
class CIntersectionViewerDlg : public CDialogEx
{
// 构造
public:
	CIntersectionViewerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_INTERSECTIONVIEWER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	segs ss;
	rays rs;
	lines ls;
	circles cs;
	points its;
	
	double dim_min;
	double dim_max;
	bool show_inter;
	bool show_label;
	bool sh_element_label;
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
	afx_msg void OnBnClickedButtonOpenScene();
	afx_msg void OnBnClickedButtonOpenIntersection();
	// Show intersection
	CButton show_intersection;
	afx_msg void OnBnClickedButtonResetView();
	afx_msg void OnBnClickedCheckShowIntersection();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedCheckShowLabel();
	CButton show_label_btn;
	afx_msg void OnBnClickedCheckShowElementLabel();
	CButton show_element_label;
};

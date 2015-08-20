
// WindowingQueryViewerDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include <vector>
#include <set>

typedef struct _point
{
	double x;
	double y;
	_point() : x(0.0), y(0.0) {}
	_point(double xp, double yp) : x(xp), y(yp){}
} point;

#define ORI_UNDEFINED -1
#define ORI_UP 0
#define ORI_DOWN 1
#define ORI_LEFT 2
#define ORI_RIGHT 3

typedef struct _AP_seg
{
	double x;
	double y;
	double length;
	int orientation;
	_AP_seg() : x(0), y(0), length(0), orientation(ORI_UNDEFINED) {}
	_AP_seg(double xp, double yp, double leng, double ori) : x(xp), y(yp), length(leng), orientation(ori) {}
} AP_seg;

typedef std::vector<AP_seg> AP_segs;

typedef struct _window_query
{
	double x;
	double y;
	double w;
	double h;
	_window_query() : x(0), y(0), w(0), h(0) {}
	_window_query(double xp, double yp, double wp, double hp) : x(xp), y(yp), w(wp), h(hp) {}
} window_query;

typedef std::vector<window_query> window_queries;

// CWindowingQueryViewerDlg 对话框
class CWindowingQueryViewerDlg : public CDialogEx
{
// 构造
public:
	CWindowingQueryViewerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_WINDOWINGQUERYVIEWER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	AP_segs segs;
	window_queries queries;
	std::vector<std::set<int>> queries_result;
	double dim_min;
	double dim_max;
	bool show_results;
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
	void AP_seg_end_point(const AP_seg &seg, double *xp, double *yp);
	void redraw();
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButtonOpenSegmentsQueries();
	afx_msg void OnBnClickedButtonOpenResults();
	afx_msg void OnBnClickedCheckShowResults();
	afx_msg void OnBnClickedButtonResetView();
	CButton show_results_btn;
	CListBox queries_list_box;
	afx_msg void OnLbnSelchangeListQueries();
};

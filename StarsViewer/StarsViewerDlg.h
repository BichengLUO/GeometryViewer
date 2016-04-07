
// StarsViewerDlg.h : 头文件
//

#pragma once
#include <vector>
#include <set>
#include "afxwin.h"

typedef struct _point
{
	double x;
	double y;
	_point() : x(0.0), y(0.0) {}
	_point(double xp, double yp) : x(xp), y(yp){}
} point;

typedef std::vector<point> points;

typedef struct _query
{
	double x1;
	double y1;
	double x2;
	double y2;
	_query() : x1(0.0), y1(0.0), x2(0.0), y2(0.0) {}
	_query(double xp1, double yp1, double xp2, double yp2) : x1(xp1), y1(yp1), x2(xp2), y2(yp2) {}
} query;

typedef std::vector<query> queries;

// CStarsViewerDlg 对话框
class CStarsViewerDlg : public CDialogEx
{
// 构造
public:
	CStarsViewerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_STARSVIEWER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	points stars;
	queries segments;
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
	void redraw();
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpenStarsQueries();
	afx_msg void OnBnClickedButtonOpenResults();
	afx_msg void OnBnClickedCheckShowResults();
	afx_msg void OnBnClickedButtonResetView();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLbnSelchangeListQueries();
	CButton show_results_btn;
	CListBox queries_list_box;
};

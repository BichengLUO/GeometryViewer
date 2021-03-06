
// RoomViewerDlg.h : 头文件
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
typedef std::vector<int> path;

// CRoomViewerDlg 对话框
class CRoomViewerDlg : public CDialogEx
{
// 构造
public:
	CRoomViewerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ROOMVIEWER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	points polygon_points;
	point start;
	point end;
	double polygon_dim_min;
	double polygon_dim_max;
	path shortest_path;
	bool show_path;
	bool show_corner_id;
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
	afx_msg void OnBnClickedButtonOpenRoomMap();
	afx_msg void OnBnClickedButtonOpenPath();
	afx_msg void OnBnClickedCheckShowPath();
	afx_msg void OnBnClickedButtonResetView();
	CButton show_path_btn;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	CButton show_corner_id_btn;
	afx_msg void OnBnClickedCheckShowCornerId();
};

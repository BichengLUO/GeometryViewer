
// PartyViewerDlg.h : 头文件
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
} point;

typedef std::vector<point> points;

// CPartyViewerDlg 对话框
class CPartyViewerDlg : public CDialogEx
{
// 构造
public:
	CPartyViewerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PARTYVIEWER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	points boys_pts;
	points girls_pts;
	double polygon_dim_min;
	double polygon_dim_max;
	std::vector<std::pair<int, int> > pairs;
	bool show_pairs;
	bool show_person_id;
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
	afx_msg void OnBnClickedButtonOpenPartyMap();
	afx_msg void OnBnClickedButtonOpenPairs();
	CButton show_pairs_check;
	afx_msg void OnBnClickedCheckShowPersonId();
	afx_msg void OnBnClickedCheckShowPairs();
	CButton show_person_id_check;
	afx_msg void OnBnClickedButtonResetView();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

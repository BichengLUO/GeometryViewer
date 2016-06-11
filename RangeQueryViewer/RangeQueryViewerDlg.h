
// RangeQueryViewerDlg.h : ͷ�ļ�
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

// CRangeQueryViewerDlg �Ի���
class CRangeQueryViewerDlg : public CDialogEx
{
// ����
public:
	CRangeQueryViewerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RANGEQUERYVIEWER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	points pts;
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

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	double scale_dim_x(double v);
	double scale_dim_y(double v);
	void redraw();
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpenPointsQueries();
	afx_msg void OnBnClickedButtonOpenResults();
	afx_msg void OnBnClickedButtonResetView();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLbnSelchangeListQueries();
	afx_msg void OnBnClickedCheckShowResults();
	CListBox queries_list_box;
};


// WallsViewerDlg.h : ͷ�ļ�
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
} point, query;
typedef std::vector<query> queries;

typedef struct _seg
{
	double x;
	double y;
	double u;
	double v;
	_seg() : x(0.0), y(0.0) {}
	_seg(double xp, double yp, double up, double vp) : x(xp), y(yp), u(up), v(vp) {}
} seg;
typedef std::vector<seg> segs;

// CWallsViewerDlg �Ի���
class CWallsViewerDlg : public CDialogEx
{
// ����
public:
	CWallsViewerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_WALLSVIEWER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	segs segments;
	queries point_queries;
	std::vector<int> queries_result;
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
	afx_msg void OnBnClickedButtonOpenSegmentsQueries();
	afx_msg void OnBnClickedButtonOpenResults();
	afx_msg void OnBnClickedButtonResetView();
	afx_msg void OnBnClickedCheckShowResults();
	CListBox queries_list_box;
	CButton show_results_btn;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLbnSelchangeListQueries();
};

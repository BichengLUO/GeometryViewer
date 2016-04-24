
// GreatWallDlg.h : ͷ�ļ�
//

#pragma once
#include <vector>

typedef struct _point2d
{
	int x;
	int y;
	_point2d(int xp, int yp) : x(xp), y(yp){}
} point2d;

typedef std::vector<point2d> points;
typedef std::vector<int> guardians;

// CGreatWallDlg �Ի���
class CGreatWallDlg : public CDialogEx
{
// ����
public:
	CGreatWallDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GREATWALL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	points pts;
	guardians gds;
	std::vector<int> hull;
	BOOL first_run;
	BOOL show_guardians;
	BOOL show_upper_hull;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckShowGuardians();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void redraw();
	void generate_guardians();
	bool to_left(point2d p1, point2d p2, point2d p3);
	bool to_left_on(point2d p1, point2d p2, point2d p3);
	afx_msg void OnBnClickedCheckShowUpperHull();
	afx_msg void OnBnClickedButtonUndo();
};

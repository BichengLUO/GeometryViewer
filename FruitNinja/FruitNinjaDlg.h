
// FruitNinjaDlg.h : ͷ�ļ�
//

#pragma once
#include <vector>

typedef struct _point2d
{
	int x;
	int y;
	_point2d() : x(0), y(0) {}
	_point2d(int xp, int yp) : x(xp), y(yp) {}
} point2d;

typedef struct _segment
{
	int x;
	int y;
	int len;
	_segment(int xp, int yp, int lenp) : x(xp), y(yp), len(lenp){}
} segment;

typedef std::vector<segment> segments;

// CFruitNinjaDlg �Ի���
class CFruitNinjaDlg : public CDialogEx
{
// ����
public:
	CFruitNinjaDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FRUITNINJA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	point2d mouse_pos;
	segments sgmts;
	BOOL first_run;

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
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void redraw();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonClear();
};

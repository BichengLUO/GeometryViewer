
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

typedef struct _point2df
{
	double x;
	double y;
	_point2df() : x(0), y(0) {}
	_point2df(double xp, double yp) : x(xp), y(yp) {}
} point2df;

typedef struct _segment
{
	int x;
	int y;
	int len;
	_segment(int xp, int yp, int lenp) : x(xp), y(yp), len(lenp){}
} segment;

typedef std::vector<segment> segments;
typedef std::vector<point2df> hull;

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
	hull convex_hull;
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
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonClear();
	void redraw();
	void draw_string(Graphics* pMemGraphics, TCHAR *str, int x, int y, int width, int height, Brush *brush);
	void draw_convex_hull(Graphics* pMemGraphics, Pen *pen, Brush *brush);
	void update_conve_hull();
	void intersect(double x1, double y1, double x2, double y2, double *x, double *y);
	bool is_intersect(point2df p1, point2df p2, double a, double b);
	void intersect(point2df p1, point2df p2, double a, double b, double *x, double *y);
	void cut_convex_hull(double a, double b, bool top);
};

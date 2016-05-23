
// GreatWallDlg.h : 头文件
//

#pragma once
#include <vector>
#include <random>

typedef long long ll;

typedef struct _point2d
{
	ll x;
	ll y;
	_point2d(ll xp, ll yp) : x(xp), y(yp){}
} point2d;

typedef std::vector<point2d> points;
typedef std::vector<int> guardians;


// CGreatWallDlg 对话框
class CGreatWallDlg : public CDialogEx
{
// 构造
public:
	CGreatWallDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GREATWALL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	points pts;
	guardians gds;
	std::vector<int> hull;
	BOOL show_guardians;
	BOOL show_upper_hull;
	BOOL show_coordinates;
	ll range;
	bool import_mode;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	int generate_guardians_pts(const points &pts);
	ll random_int(std::mt19937 &rng, ll min, ll max);
	points random_input(int count);
	points convex_input(int count);
	points concave_input(int count);
	points huge_convex_input(int count);
	points huge_concave_input(int count);
	points tangent_input(int count);
	bool to_left(point2d p1, point2d p2, point2d p3);
	bool to_left_on(point2d p1, point2d p2, point2d p3);
	afx_msg void OnBnClickedCheckShowUpperHull();
	afx_msg void OnBnClickedButtonUndo();
	void draw_string(Graphics* pMemGraphics, TCHAR *str, int x, int y, int width, int height, Brush *brush);
	afx_msg void OnBnClickedCheckShowCoordinates();
	afx_msg void OnBnClickedButtonGenerate();
	afx_msg void OnBnClickedButtonImport();
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

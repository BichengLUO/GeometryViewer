
// FruitNinjaDlg.h : 头文件
//

#pragma once
#include <vector>
#include <random>

typedef long long ll;

typedef struct _point2d
{
	ll x;
	ll y;
	_point2d() : x(0), y(0) {}
	_point2d(ll xp, ll yp) : x(xp), y(yp) {}
} point2d;

class rat
{
private:
	ll gcd(ll u, ll v) {
		while (v != 0) {
			ll r = u % v;
			u = v;
			v = r;
		}
		return u;
	}
	void reduce()
	{
		ll g = gcd(abs(x), abs(y));
		x /= g;
		y /= g;
	}
public:
	ll x;
	ll y;
	rat() : x(0), y(1) {}
	rat(ll xp) : x(xp), y(1) {}
	rat(ll xp, ll yp) : x(xp), y(yp)
	{
		if (x == 0)
			y = 1;
		else
		{
			if (y < 0)
			{
				y = -y;
				x = -x;
			}
			reduce();
		}
	}
	rat operator- (const rat &o) const
	{
		return rat(x * o.y - y * o.x, y * o.y);
	}
	rat operator+ (const rat &o) const
	{
		return rat(x * o.y + y * o.x, y * o.y);
	}
	rat operator* (const rat &o) const
	{
		return rat(x * o.x, y * o.y);
	}
	rat operator/ (const rat &o) const
	{
		return rat(x * o.y, y * o.x);
	}
	bool operator< (const rat &o) const
	{
		return x * o.y < y * o.x;
	}
	bool operator<= (const rat &o) const
	{
		return x * o.y <= y * o.x;
	}
	bool operator>= (const rat &o) const
	{
		return x * o.y >= y * o.x;
	}
	bool operator!= (const rat &o) const
	{
		return x != o.x || y != o.y;
	}
	double to_double() const
	{
		return x / (double)y;
	}
};

typedef struct _point2df
{
	rat x;
	rat y;
	_point2df() : x(), y() {}
	_point2df(rat xp, rat yp) : x(xp), y(yp) {}
} point2df;

typedef struct _segment
{
	ll x;
	ll y;
	ll len;
	_segment(ll xp, ll yp, ll lenp) : x(xp), y(yp), len(lenp){}
} segment;

typedef std::vector<segment> segments;
typedef std::vector<point2df> hull;

// CFruitNinjaDlg 对话框
class CFruitNinjaDlg : public CDialogEx
{
// 构造
public:
	CFruitNinjaDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FRUITNINJA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	point2d mouse_pos;
	segments sgmts;
	hull convex_hull;
	std::vector<hull> hull_history;
	double min_x, min_y, max_x, max_y;
	double imp_x_a, imp_x_b, imp_y_a, imp_y_b;
	Rect dg;
	BOOL in_dg;
	BOOL show_coordinates;
	Point dg_point;
	double dg_point_a, dg_point_b;
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
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonClear();
	void redraw();
	void draw_string(Graphics* pMemGraphics, TCHAR *str, int x, int y, int width, int height, Brush *brush);
	void draw_convex_hull(Graphics* pMemGraphics, Pen *pen, Brush *brush);

	char gen_convex_hull_sgmts(const segments &input);
	void update_convex_hull();
	void intersect(rat x1, rat y1, rat x2, rat y2, rat *x, rat *y);
	bool is_intersect(point2df p1, point2df p2, rat a, rat b);
	void intersect(point2df p1, point2df p2, rat a, rat b, rat *x, rat *y);
	hull cut_convex_hull(const hull &ch, rat a, rat b, bool top);

	ll random_int(std::mt19937 &rng, ll min, ll max);
	segments yes_input(int count);
	segments no_input(int count);
	segments degenerated_yes_input(int count);
	segments degenerated_no_input(int count);

	afx_msg void OnBnClickedButtonUndo();
	afx_msg void OnBnClickedCheckShowCoordinates();
	afx_msg void OnBnClickedButtonImport();
	afx_msg void OnBnClickedButtonGenerate();
};

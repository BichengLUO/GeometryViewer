
// FruitNinjaDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FruitNinja.h"
#include "FruitNinjaDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFruitNinjaDlg 对话框



CFruitNinjaDlg::CFruitNinjaDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFruitNinjaDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFruitNinjaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFruitNinjaDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CFruitNinjaDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_UNDO, &CFruitNinjaDlg::OnBnClickedButtonUndo)
	ON_BN_CLICKED(IDC_CHECK_SHOW_COORDINATES, &CFruitNinjaDlg::OnBnClickedCheckShowCoordinates)
END_MESSAGE_MAP()


// CFruitNinjaDlg 消息处理程序

BOOL CFruitNinjaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

	min_x = std::numeric_limits<double>::max();
	max_x = std::numeric_limits<double>::min();
	min_y = min_x, max_y = max_x;

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)  (((a) < (b)) ? (a) : (b))
#endif

	CRect rect;
	GetClientRect(&rect);
	dg = Rect(rect.Width() - 405, rect.Height() - 355, 400, 300);
	in_dg = FALSE;
	show_coordinates = FALSE;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFruitNinjaDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFruitNinjaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CRect rect;
		GetClientRect(&rect);
		Pen pen(Color::Black);
		Pen pen_gray(Color::Gray);
		Pen pen_thick(Color::Black, 3);
		Pen pen_green_thick(Color::Green, 3);
		Pen dash_pen_gray(Color::Gray, 3);
		dash_pen_gray.SetDashStyle(DashStyleDash);
		SolidBrush brush_red(Color::Red);
		SolidBrush brush_green(Color::Green);
		SolidBrush brush_green_alpha(Color::MakeARGB(100, 0, 155, 0));
		SolidBrush brush_black(Color::Black);
		SolidBrush brush_black_alpha(Color::MakeARGB(100, 0, 0, 0));
		SolidBrush brush_gray(Color::Gray);
		LinearGradientBrush brush_ab(Rect(0, 0, rect.Width(), rect.Height()), Color::Aqua, Color::Blue, LinearGradientModeHorizontal);
		LinearGradientBrush brush_or(Rect(0, 0, rect.Width(), rect.Height()), Color::Orange, Color::Red, LinearGradientModeHorizontal);
		LinearGradientBrush brush_yo(Rect(0, 0, rect.Width(), rect.Height()), Color::Yellow, Color::Orange, LinearGradientModeHorizontal);
		SolidBrush brush_white(Color::White);
		SolidBrush brush_background(Color::MakeARGB(255, 240, 240, 240));
		SolidBrush brush_white_alpha(Color::MakeARGB(200, 255, 255, 255));

		Bitmap pMemBitmap(rect.Width(), rect.Height());
		Graphics* pMemGraphics = Graphics::FromImage(&pMemBitmap);
		pMemGraphics->SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);
		pMemGraphics->FillRectangle(&brush_background, 0, 0, rect.Width(), rect.Height());
		pMemGraphics->FillRectangle(&brush_white, 0, 0, rect.Width(), rect.Height() - 50);

		CPaintDC dc(this);
		Graphics graphics(dc.m_hDC);

		WCHAR count_title[128];
		wsprintf(count_title, L"Size of Segments = %d", sgmts.size());
		draw_string(pMemGraphics, count_title, 5, 5, 500, 20, &brush_black);
		int green_sg_count = 0;
		for (auto &seg : sgmts)
		{
			int x = seg.x;
			int y = seg.y;
			int len = seg.len;
			if (len > 0)
			{
				if (in_dg && dg_point_a * x + dg_point_b >= y && dg_point_a * x + dg_point_b <= y + len)
				{
					pMemGraphics->DrawLine(&pen_green_thick, x, y, x, y + len);
					green_sg_count++;
				}
				else
					pMemGraphics->DrawLine(&pen_thick, x, y, x, y + len);
				pMemGraphics->FillEllipse(&brush_black, x - 4, y + len - 4, 8, 8);
				pMemGraphics->FillEllipse(&brush_black, x - 4, y - 4, 8, 8);
				if (show_coordinates)
				{
					WCHAR pt_title[128];
					wsprintf(pt_title, L"(%d,%d)", x, y);
					if (in_dg && dg_point_a * x + dg_point_b >= y && dg_point_a * x + dg_point_b <= y + len)
						pMemGraphics->FillRectangle(&brush_green_alpha, x + 5, y + 10, 70, 20);
					else
						pMemGraphics->FillRectangle(&brush_black_alpha, x + 5, y + 10, 70, 20);
					draw_string(pMemGraphics, pt_title, x + 10, y + 10, 70, 20, &brush_white);

					wsprintf(pt_title, L"(%d,%d)", x, y + len);
					if (in_dg && dg_point_a * x + dg_point_b >= y && dg_point_a * x + dg_point_b <= y + len)
						pMemGraphics->FillRectangle(&brush_green_alpha, x + 5, y + len + 10, 70, 20);
					else
						pMemGraphics->FillRectangle(&brush_black_alpha, x + 5, y + len + 10, 70, 20);
					draw_string(pMemGraphics, pt_title, x + 10, y + len + 10, 70, 20, &brush_white);
				}
			}
			else
			{
				pMemGraphics->DrawLine(&dash_pen_gray, x, y, x, mouse_pos.y);
				pMemGraphics->FillEllipse(&brush_gray, x - 4, mouse_pos.y - 4, 8, 8);
				pMemGraphics->FillEllipse(&brush_gray, x - 4, y - 4, 8, 8);
			}
		}
		if (in_dg)
		{
			int dg_x1 = -10;
			int dg_y1 = dg_point_a * dg_x1 + dg_point_b;
			int dg_x2 = rect.Width() + 10;
			int dg_y2 = dg_point_a * dg_x2 + dg_point_b;
			pMemGraphics->DrawLine(&dash_pen_gray, dg_x1, dg_y1, dg_x2, dg_y2);

			WCHAR count_title[128];
			wsprintf(count_title, L"Size of Segments Crossed by the Line = %d", green_sg_count);
			draw_string(pMemGraphics, count_title, 5, 25, 500, 20, &brush_black);
		}
		if (convex_hull.size() > 0 || sgmts.size() <= 2)
			pMemGraphics->FillRectangle(&brush_ab, dg.X, dg.Y - 25, dg.Width, 25);
		else
			pMemGraphics->FillRectangle(&brush_or, dg.X, dg.Y - 25, dg.Width, 25);
		pMemGraphics->DrawRectangle(&pen_gray, dg.X, dg.Y - 25, dg.Width, 25);
		WCHAR dg_title[128];
		wsprintf(dg_title, L"Duality Graph: Size of Points on the Hull = %d", convex_hull.size());
		draw_string(pMemGraphics, dg_title, dg.X + 5, dg.Y - 20, dg.Width, 20, &brush_white);
		pMemGraphics->FillRectangle(&brush_white_alpha, dg);
		pMemGraphics->DrawRectangle(&pen_gray, dg);
		draw_convex_hull(pMemGraphics, &pen, &brush_yo);
		if (in_dg)
		{
			pMemGraphics->DrawEllipse(&pen_thick, dg_point.X - 5, dg_point.Y - 5, 10, 10);
			if (green_sg_count == sgmts.size())
				pMemGraphics->FillEllipse(&brush_green, dg_point.X - 5, dg_point.Y - 5, 10, 10);
			else
				pMemGraphics->FillEllipse(&brush_red, dg_point.X - 5, dg_point.Y - 5, 10, 10);
		}
		delete pMemGraphics;
		graphics.DrawImage(&pMemBitmap, 0, 0);
	}
}

void CFruitNinjaDlg::draw_string(Graphics* pMemGraphics, TCHAR *str, int x, int y, int width, int height, Brush *brush)
{
	// Initialize arguments.
	Gdiplus::Font myFont(L"Arial", 10);
	StringFormat format;

	// Draw string.
	pMemGraphics->DrawString(
		str,
		wcslen(str),
		&myFont,
		RectF(x, y, width, height),
		&format,
		brush);
}

void CFruitNinjaDlg::draw_convex_hull(Graphics* pMemGraphics, Pen *pen, Brush *brush)
{
	CRect rect;
	GetClientRect(&rect);
	SolidBrush brush_black(Color::Black);
	SolidBrush brush_brown(Color::Brown);
	Pen dash_pen_gray(Color::Gray);
	dash_pen_gray.SetDashStyle(DashStyleDash);
	for (int i = 0; i < hull_history.size(); i++)
	{
		Point *pts = new Point[hull_history[i].size()];
		for (int j = 0; j < hull_history[i].size(); j++)
		{
			double x = hull_history[i][j].x;
			double y = hull_history[i][j].y;
			pts[j].X = (x - min_x) / (max_x - min_x) * (dg.Width - 20) + dg.X + 10;
			pts[j].Y = (y - min_y) / (max_y - min_y) * (dg.Height - 20) + dg.Y + 10;
		}
		pMemGraphics->DrawPolygon(&dash_pen_gray, pts, hull_history[i].size());
		delete[] pts;
	}
	if (convex_hull.size() > 0)
	{
		Point *pts = new Point[convex_hull.size()];
		for (int i = 0; i < convex_hull.size(); i++)
		{
			double x = convex_hull[i].x;
			double y = convex_hull[i].y;
			pts[i].X = (x - min_x) / (double)(max_x - min_x) * (dg.Width - 20) + dg.X + 10;
			pts[i].Y = (y - min_y) / (double)(max_y - min_y) * (dg.Height - 20) + dg.Y + 10;
		}
		pMemGraphics->FillPolygon(brush, pts, convex_hull.size());
		pMemGraphics->DrawPolygon(pen, pts, convex_hull.size());
		for (int i = 0; i < convex_hull.size(); i++)
		{
			pMemGraphics->FillEllipse(&brush_brown, pts[i].X - 3, pts[i].Y - 3, 6, 6);
			pMemGraphics->DrawEllipse(pen, pts[i].X - 3, pts[i].Y - 3, 6, 6);
		}
		delete[] pts;
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFruitNinjaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CFruitNinjaDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	return TRUE;
}


void CFruitNinjaDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (!dg.Contains(Point(point.x, point.y)))
	{
		if (sgmts.size() > 0)
		{
			auto last_seg = sgmts.end() - 1;
			if (last_seg->len == 0)
			{
				if (point.y != last_seg->y)
				{
					last_seg->len = abs(point.y - last_seg->y);
					if (point.y < last_seg->y)
						last_seg->y = point.y;
					update_convex_hull();
				}
			}
			else
				sgmts.push_back(segment(point.x, point.y, 0));
		}
		else
			sgmts.push_back(segment(point.x, point.y, 0));
		redraw();
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CFruitNinjaDlg::update_convex_hull()
{
	if (sgmts.size() == 2)
	{
		int x1 = sgmts[0].x;
		int y1 = sgmts[0].y;
		int len1 = sgmts[0].len;
		int x2 = sgmts[1].x;
		int y2 = sgmts[1].y;
		int len2 = sgmts[1].len;

		double xx1, yy1;
		intersect(x1, y1, x2, y2, &xx1, &yy1);
		double xx2, yy2;
		intersect(x1, y1, x2, y2 + len2, &xx2, &yy2);
		double xx3, yy3;
		intersect(x1, y1 + len1, x2, y2 + len2, &xx3, &yy3);
		double xx4, yy4;
		intersect(x1, y1 + len1, x2, y2, &xx4, &yy4);

		convex_hull.push_back(point2df(xx1, yy1));
		convex_hull.push_back(point2df(xx2, yy2));
		convex_hull.push_back(point2df(xx3, yy3));
		convex_hull.push_back(point2df(xx4, yy4));

		for (int i = 0; i < convex_hull.size(); i++)
		{
			double x = convex_hull[i].x;
			double y = convex_hull[i].y;
			min_x = min(min_x, x);
			max_x = max(max_x, x);
			min_y = min(min_y, y);
			max_y = max(max_y, y);
		}
	}
	else if (sgmts.size() > 2)
	{
		int x = sgmts.end()[-1].x;
		int y = sgmts.end()[-1].y;
		int len = sgmts.end()[-1].len;

		hull new_convex_hull = cut_convex_hull(convex_hull, x, y, false);
		new_convex_hull = cut_convex_hull(new_convex_hull, x, y + len, true);
		hull_history.push_back(convex_hull);
		convex_hull = new_convex_hull;
	}
}

void CFruitNinjaDlg::intersect(double x1, double y1, double x2, double y2, double *x, double *y)
{
	*x = (y2 - y1) / (x1 - x2);
	*y = (x1 * y2 - x2 * y1) / (x1 - x2);
}

hull CFruitNinjaDlg::cut_convex_hull(const hull &ch, double a, double b, bool top)
{
	hull new_convex_hull;
	for (int i = 0; i < ch.size(); i++)
	{
		point2df p1 = ch[i];
		point2df p2 = ch[(i + 1) % ch.size()];
		if ((top && a * p1.x + b >= p1.y) || (!top && a * p1.x + b <= p1.y))
			new_convex_hull.push_back(p1);
		if (is_intersect(p1, p2, a, b))
		{
			double x, y;
			intersect(p1, p2, a, b, &x, &y);
			new_convex_hull.push_back(point2df(x, y));
		}
	}
	return new_convex_hull;
}

bool CFruitNinjaDlg::is_intersect(point2df p1, point2df p2, double a, double b)
{
	double y_diff1 = a * p1.x + b - p1.y;
	double y_diff2 = a * p2.x + b - p2.y;
	return y_diff1 * y_diff2 <= 0 && y_diff2 != 0;
}

void CFruitNinjaDlg::intersect(point2df p1, point2df p2, double a, double b, double *x, double *y)
{
	double a2 = (p1.y - p2.y) / (p1.x - p2.x);
	double b2 = (p1.x * p2.y - p2.x * p1.y) / (p1.x - p2.x);
	intersect(a, b, a2, b2, x, y);
}

void CFruitNinjaDlg::redraw()
{
	CRect rect;
	GetClientRect(&rect);
	rect.bottom -= 50;
	InvalidateRect(rect);
}

void CFruitNinjaDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	mouse_pos = point2d(point.x, point.y);
	if (dg.Contains(Point(point.x, point.y)))
	{
		dg_point = Point(point.x, point.y);
		in_dg = TRUE;
		dg_point_a = -((dg_point.X - 10 - dg.X) / (double)(dg.Width - 20) * (max_x - min_x) + min_x);
		dg_point_b = (dg_point.Y - 10 - dg.Y) / (double)(dg.Height - 20) * (max_y - min_y) + min_y;
		redraw();
	}
	else
	{
		if (in_dg)
		{
			in_dg = FALSE;
			redraw();
		}
		else
		{
			in_dg = FALSE;
			if (sgmts.size() > 0)
			{
				auto last_seg = sgmts.end() - 1;
				if (last_seg->len == 0)
					redraw();
			}
		}
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CFruitNinjaDlg::OnBnClickedButtonClear()
{
	// TODO:  在此添加控件通知处理程序代码
	sgmts.clear();
	convex_hull.clear();
	hull_history.clear();
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

	min_x = std::numeric_limits<double>::max();
	max_x = std::numeric_limits<double>::min();
	min_y = min_x, max_y = max_x;

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)  (((a) < (b)) ? (a) : (b))
#endif
	redraw();
}


void CFruitNinjaDlg::OnBnClickedButtonUndo()
{
	// TODO:  在此添加控件通知处理程序代码
	if (sgmts.size() > 0)
		sgmts.pop_back();
	if (hull_history.size() > 0)
	{
		convex_hull = hull_history.end()[-1];
		hull_history.pop_back();
	}
	else
	{
		convex_hull.clear();
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

		min_x = std::numeric_limits<double>::max();
		max_x = std::numeric_limits<double>::min();
		min_y = min_x, max_y = max_x;

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)  (((a) < (b)) ? (a) : (b))
#endif
	}
	redraw();
}


void CFruitNinjaDlg::OnBnClickedCheckShowCoordinates()
{
	// TODO:  在此添加控件通知处理程序代码
	if (((CButton*)GetDlgItem(IDC_CHECK_SHOW_COORDINATES))->GetCheck())
		show_coordinates = TRUE;
	else
		show_coordinates = FALSE;
	redraw();
}

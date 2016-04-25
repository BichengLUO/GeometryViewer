
// GreatWallDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GreatWall.h"
#include "GreatWallDlg.h"
#include "afxdialogex.h"
#include <stack>

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


// CGreatWallDlg 对话框



CGreatWallDlg::CGreatWallDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGreatWallDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGreatWallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGreatWallDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK_SHOW_GUARDIANS, &CGreatWallDlg::OnBnClickedCheckShowGuardians)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CGreatWallDlg::OnBnClickedButtonClear)
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_CHECK_SHOW_UPPER_HULL, &CGreatWallDlg::OnBnClickedCheckShowUpperHull)
	ON_BN_CLICKED(IDC_BUTTON_UNDO, &CGreatWallDlg::OnBnClickedButtonUndo)
END_MESSAGE_MAP()


// CGreatWallDlg 消息处理程序

BOOL CGreatWallDlg::OnInitDialog()
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
	show_guardians = FALSE;
	show_upper_hull = FALSE;
	first_run = TRUE;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGreatWallDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGreatWallDlg::OnPaint()
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
		Pen dash_pen_red(Color::Red, 3);
		dash_pen_red.SetDashStyle(DashStyleDash);
		Pen dash_pen_orange(Color::Orange, 3);
		dash_pen_orange.SetDashStyle(DashStyleDash);
		SolidBrush brush_red(Color::Red);
		SolidBrush brush_black(Color::Black);
		LinearGradientBrush brush_aqua(Rect(0, 0, rect.Width(), rect.Height()), Color::Aqua, Color::Blue, LinearGradientModeHorizontal);
		SolidBrush brush_white(Color::White);

		Bitmap pMemBitmap(rect.Width(), rect.Height() - 50);
		Graphics* pMemGraphics = Graphics::FromImage(&pMemBitmap);
		pMemGraphics->SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);
		pMemGraphics->FillRectangle(&brush_white, 0, 0, rect.Width(), rect.Height() - 50);

		CPaintDC dc(this);
		Graphics graphics(dc.m_hDC);

		if (first_run)
		{
			first_run = FALSE;
			return;
		}
		if (pts.size() > 0)
		{
			if (pts.size() > 1)
			{
				Point *array = new Point[pts.size() + 2];
				for (int i = 0; i < pts.size(); i++)
					array[i] = Point(pts[i].x, pts[i].y);
				array[pts.size()] = Point(array[pts.size() - 1].X, 800);
				array[pts.size() + 1] = Point(array[0].X, 800);
				pMemGraphics->FillPolygon(&brush_aqua, array, pts.size() + 2);
				delete[] array;
			}
			if (show_upper_hull)
			{
				for (int i = 0; i < hull.size() - 1; i++)
				{
					int x = pts[hull[i]].x;
					int y = pts[hull[i]].y;
					int nx = pts[hull[i + 1]].x;
					int ny = pts[hull[i + 1]].y;

					pMemGraphics->DrawLine(&dash_pen_red, x, y, nx, ny);
				}
				if (hull.size() > 1)
				{
					int lx = pts[hull.end()[-2]].x;
					int ly = pts[hull.end()[-2]].y;
					int x = pts[hull.end()[-1]].x;
					int y = pts[hull.end()[-1]].y;
					double len = sqrt((x - lx)*(x - lx) + (y - ly)*(y - ly));
					int nx = x + 60 * (x - lx) / len;
					int ny = y + 60 * (y - ly) / len;

					pMemGraphics->DrawLine(&dash_pen_orange, x, y, nx, ny);
				}
			}

			double x = pts[0].x;
			double y = pts[0].y;
			pMemGraphics->FillEllipse(&brush_red, x - 5, y - 5, 10, 10);
			for (int i = 1; i < pts.size(); i++)
			{
				int x = pts[i].x;
				int y = pts[i].y;

				int nx = pts[i - 1].x;
				int ny = pts[i - 1].y;

				pMemGraphics->FillEllipse(&brush_black, x - 3, y - 3, 6, 6);
				pMemGraphics->DrawLine(&pen, x, y, nx, ny);
			}

			if (show_guardians)
			{
				for (int i = 0; i < gds.size(); i++)
				{
					int x = pts[gds[i]].x;
					int y = pts[gds[i]].y;

					pMemGraphics->FillEllipse(&brush_white, x - 5, y - 5, 10, 10);
					pMemGraphics->DrawEllipse(&pen, x - 5, y - 5, 10, 10);
				}
			}
		}
		delete pMemGraphics;
		graphics.DrawImage(&pMemBitmap, 0, 0);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGreatWallDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGreatWallDlg::redraw()
{
	CRect rect;
	GetClientRect(&rect);
	rect.bottom -= 50;
	InvalidateRect(rect);
}

void CGreatWallDlg::OnBnClickedCheckShowGuardians()
{
	// TODO:  在此添加控件通知处理程序代码
	if (((CButton*)GetDlgItem(IDC_CHECK_SHOW_GUARDIANS))->GetCheck())
		show_guardians = TRUE;
	else
		show_guardians = FALSE;
	redraw();
}


void CGreatWallDlg::OnBnClickedButtonClear()
{
	// TODO:  在此添加控件通知处理程序代码
	pts.clear();
	gds.clear();

	redraw();
}


void CGreatWallDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (pts.size() == 0 || point.x < pts[pts.size() - 1].x)
	{
		pts.push_back(point2d(point.x, point.y));
		generate_guardians();

		redraw();
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


BOOL CGreatWallDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	return TRUE;
}

void CGreatWallDlg::generate_guardians()
{
	hull.clear();
	gds.clear();
	hull.push_back(0);
	gds.push_back(0);
	for (int i = 1; i < pts.size() - 1; i++)
	{
		while (hull.size() > 1 && !to_left_on(pts[hull.end()[-2]], pts[hull.end()[-1]], pts[i]))
			hull.pop_back();
		hull.push_back(i);
		if (to_left(pts[hull.end()[-2]], pts[hull.end()[-1]], pts[i + 1]))
			gds.push_back(i);
	}
}

bool CGreatWallDlg::to_left(point2d p1, point2d p2, point2d p3)
{
	return (p3.x - p1.x) * (p2.y - p1.y) - (p2.x - p1.x) * (p3.y - p1.y) > 0;
}

bool CGreatWallDlg::to_left_on(point2d p1, point2d p2, point2d p3)
{
	return (p3.x - p1.x) * (p2.y - p1.y) - (p2.x - p1.x) * (p3.y - p1.y) >= 0;
}


void CGreatWallDlg::OnBnClickedCheckShowUpperHull()
{
	// TODO:  在此添加控件通知处理程序代码
	if (((CButton*)GetDlgItem(IDC_CHECK_SHOW_UPPER_HULL))->GetCheck())
		show_upper_hull = TRUE;
	else
		show_upper_hull = FALSE;
	redraw();
}


void CGreatWallDlg::OnBnClickedButtonUndo()
{
	// TODO:  在此添加控件通知处理程序代码
	pts.pop_back();
	generate_guardians();

	redraw();
}

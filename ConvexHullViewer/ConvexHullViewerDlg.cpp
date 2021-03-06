
// ConvexHullViewerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ConvexHullViewer.h"
#include "ConvexHullViewerDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <string>
#include <sstream>

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


// CConvexHullViewerDlg 对话框



CConvexHullViewerDlg::CConvexHullViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CConvexHullViewerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CConvexHullViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_SHOW_CONVEX_HULL, show_convex_hull_btn);
	DDX_Control(pDX, IDC_CHECK_SHOW_POINT_ID, show_point_id_btn);
}

BEGIN_MESSAGE_MAP(CConvexHullViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_POINTS, &CConvexHullViewerDlg::OnBnClickedButtonOpenPoints)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_CONVEX_HULL, &CConvexHullViewerDlg::OnBnClickedButtonOpenConvexHull)
	ON_BN_CLICKED(IDC_CHECK_SHOW_CONVEX_HULL, &CConvexHullViewerDlg::OnBnClickedCheckShowConvexHull)
	ON_BN_CLICKED(IDC_BUTTON_RESET_VIEW, &CConvexHullViewerDlg::OnBnClickedButtonResetView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_CHECK_SHOW_POINT_ID, &CConvexHullViewerDlg::OnBnClickedCheckShowPointId)
END_MESSAGE_MAP()


// CConvexHullViewerDlg 消息处理程序

BOOL CConvexHullViewerDlg::OnInitDialog()
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
	GetDlgItem(IDC_BUTTON_OPEN_CONVEX_HULL)->EnableWindow(false);
	show_convex_hull_btn.EnableWindow(false);
	show_convex_hull_btn.SetCheck(1);
	show_point_id_btn.SetCheck(1);
	show_convex_hull = true;
	show_point_id = true;
	scale = 1.0;
	transform = point();
	drag_start_transform = transform;
	dragging = false;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CConvexHullViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CConvexHullViewerDlg::OnPaint()
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
		CPaintDC dc(this);

		CRect r;
		GetClientRect(&r);

		CBitmap memBitmap;
		CBitmap* pOldBmp = NULL;
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		memBitmap.CreateCompatibleBitmap(&dc, r.right, r.bottom);
		pOldBmp = memDC.SelectObject(&memBitmap);
		memDC.FillSolidRect(r, dc.GetBkColor());
		CDC *old_pDC = &dc;
		//创建一个内存中的图像

		CPen *pOldPen;
		CBrush *pOldBrush;

		CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
		pOldPen = memDC.SelectObject(&pen);

		if (show_convex_hull)
		{
			CBrush brush1(RGB(210, 200, 255));
			pOldBrush = memDC.SelectObject(&brush1);
			CPoint *ch_pts = new CPoint[ch.size()];
			for (int i = 0; i < ch.size(); i++)
				ch_pts[i] = CPoint(scale_dim_x(pts[ch[i]].x), scale_dim_y(pts[ch[i]].y));
			memDC.Polygon(ch_pts, ch.size());
			delete[] ch_pts;
			pOldBrush = memDC.SelectObject(&brush1);
		}

		TCHAR buffer[100];
		SetBkMode(memDC, TRANSPARENT);
		
		CBrush brush2(RGB(100, 255, 100));
		CBrush brush3(RGB(255, 100, 100));
		for (int i = 0; i < pts.size(); i++)
		{
			if (std::find(ch.begin(), ch.end(), i) == ch.end())
				pOldBrush = memDC.SelectObject(&brush2);
			else
				pOldBrush = memDC.SelectObject(&brush3);

			double x = scale_dim_x(pts[i].x);
			double y = scale_dim_y(pts[i].y);
			memDC.Ellipse(x - 3, y - 3, x + 3, y + 3);
			if (show_point_id)
			{
				memDC.SetTextColor(RGB(0, 0, 0));
				wsprintf(buffer, _T("[%d]"), i + 1);
				memDC.TextOut(x, y + 6.0, buffer);
			}
			memDC.SelectObject(pOldBrush);
		}

		memDC.SelectObject(pOldPen);
		dc.BitBlt(r.left, r.top, r.right, r.bottom, &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pOldBmp);
		memDC.DeleteDC();
		memBitmap.DeleteObject();
		DeleteDC(memDC);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CConvexHullViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

double CConvexHullViewerDlg::scale_dim_x(double v)
{
	return transform.x + 135 - 250 * (scale - 1.0) + 500 * scale * (v - dim_min) / (dim_max - dim_min);
}

double CConvexHullViewerDlg::scale_dim_y(double v)
{
	return transform.y + 20 - 250 * (scale - 1.0) + 500 * scale * (v - dim_min) / (dim_max - dim_min);
}

void CConvexHullViewerDlg::OnBnClickedButtonOpenPoints()
{
	// TODO:  在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		pts.clear();
		std::ifstream input(dlg.GetPathName(), std::ifstream::in);
		std::string line;
		double max = -9999, min = 9999;
		std::getline(input, line);
		std::istringstream niss(line);
		int count = 0;
		niss >> count;
		for (int i = 0; i < count; i++)
		{
			std::getline(input, line);
			std::istringstream iss(line);
			double x, y;
			iss >> x >> y;
			pts.push_back(point(x, y));
			if (x > max) max = x;
			if (y > max) max = y;
			if (x < min) min = x;
			if (y < min) min = y;
		}
		dim_max = max;
		dim_min = min;

		GetDlgItem(IDC_BUTTON_OPEN_CONVEX_HULL)->EnableWindow(TRUE);
		Invalidate();
	}
	else
		return;
}


void CConvexHullViewerDlg::OnBnClickedButtonOpenConvexHull()
{
	// TODO:  在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		ch.clear();
		std::ifstream input(dlg.GetPathName(), std::ifstream::in);
		std::string line;
		while (std::getline(input, line))
			ch.push_back(stoi(line));
		show_convex_hull_btn.EnableWindow(TRUE);
		Invalidate();
	}
	else
		return;
}


void CConvexHullViewerDlg::OnBnClickedCheckShowConvexHull()
{
	// TODO:  在此添加控件通知处理程序代码
	if (show_convex_hull_btn.GetCheck() == 0)
		show_convex_hull = false;
	else
		show_convex_hull = true;
	Invalidate();
}


void CConvexHullViewerDlg::OnBnClickedButtonResetView()
{
	// TODO:  在此添加控件通知处理程序代码
	scale = 1.0;
	transform = point();
	drag_start_transform = point();
	Invalidate();
}


void CConvexHullViewerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	dragging = true;
	drag_start_pos = point;
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CConvexHullViewerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	dragging = false;
	drag_start_transform = transform;
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CConvexHullViewerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (dragging)
	{
		transform.x = drag_start_transform.x + point.x - drag_start_pos.x;
		transform.y = drag_start_transform.y + point.y - drag_start_pos.y;
		Invalidate();
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


BOOL CConvexHullViewerDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	scale *= 1 + zDelta / 2000.0;
	if (scale <= 0) scale = 0;
	Invalidate();
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


BOOL CConvexHullViewerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	return TRUE;
}


void CConvexHullViewerDlg::OnBnClickedCheckShowPointId()
{
	// TODO:  在此添加控件通知处理程序代码
	if (show_point_id_btn.GetCheck() == 0)
		show_point_id = false;
	else
		show_point_id = true;
	Invalidate();
}

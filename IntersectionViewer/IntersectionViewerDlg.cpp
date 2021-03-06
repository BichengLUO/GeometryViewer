
// IntersectionViewerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IntersectionViewer.h"
#include "IntersectionViewerDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <string>
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MIN2(a,b) ((a)<(b)?(a):(b))
#define MIN4(a,b,c,d) (MIN2((a),(b))<MIN2((c),(d))?MIN2((a),(b)):MIN2((c),(d)))
#define MAX2(a,b) ((a)>(b)?(a):(b))
#define MAX4(a,b,c,d) (MAX2((a),(b))>MAX2((c),(d))?MAX2((a),(b)):MAX2((c),(d)))

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


// CIntersectionViewerDlg 对话框



CIntersectionViewerDlg::CIntersectionViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIntersectionViewerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIntersectionViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_SHOW_INTERSECTION, show_intersection);
	DDX_Control(pDX, IDC_CHECK_SHOW_LABEL, show_label_btn);
	DDX_Control(pDX, IDC_CHECK_SHOW_ELEMENT_LABEL, show_element_label);
}

BEGIN_MESSAGE_MAP(CIntersectionViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_SCENE, &CIntersectionViewerDlg::OnBnClickedButtonOpenScene)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_INTERSECTION, &CIntersectionViewerDlg::OnBnClickedButtonOpenIntersection)
	ON_BN_CLICKED(IDC_BUTTON_RESET_VIEW, &CIntersectionViewerDlg::OnBnClickedButtonResetView)
	ON_BN_CLICKED(IDC_CHECK_SHOW_INTERSECTION, &CIntersectionViewerDlg::OnBnClickedCheckShowIntersection)
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_CHECK_SHOW_LABEL, &CIntersectionViewerDlg::OnBnClickedCheckShowLabel)
	ON_BN_CLICKED(IDC_CHECK_SHOW_ELEMENT_LABEL, &CIntersectionViewerDlg::OnBnClickedCheckShowElementLabel)
END_MESSAGE_MAP()


// CIntersectionViewerDlg 消息处理程序

BOOL CIntersectionViewerDlg::OnInitDialog()
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

	GetDlgItem(IDC_BUTTON_OPEN_INTERSECTION)->EnableWindow(false);
	show_intersection.EnableWindow(false);
	show_intersection.SetCheck(1);
	show_label_btn.SetCheck(1);
	show_element_label.SetCheck(1);
	show_inter = true;
	show_label = true;
	sh_element_label = true;
	scale = 1.0;
	transform = point();
	drag_start_transform = transform;
	dragging = false;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CIntersectionViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CIntersectionViewerDlg::OnPaint()
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
		TCHAR buffer[20];
		SetBkMode(memDC, TRANSPARENT);
		CPen pen_s(PS_SOLID, 2, RGB(0, 0, 0));
		pOldPen = memDC.SelectObject(&pen_s);
		for (int i = 0; i < ss.size(); i++)
		{
			double x1 = scale_dim_x(ss[i].start.x);
			double y1 = scale_dim_y(ss[i].start.y);
			double x2 = scale_dim_x(ss[i].end.x);
			double y2 = scale_dim_y(ss[i].end.y);

			memDC.MoveTo(x1, y1);
			memDC.LineTo(x2, y2);

			if (sh_element_label)
			{
				memDC.SetTextColor(RGB(0, 0, 0));
				swprintf(buffer, _T("S%d"), i);
				memDC.TextOut((x1 + x2) / 2.0, (y1 + y2) / 2.0 + 5.0, buffer);
			}
		}
		memDC.SelectObject(pOldPen);

		CPen pen_r(PS_SOLID, 2, RGB(200, 0, 0));
		pOldPen = memDC.SelectObject(&pen_r);
		for (int i = 0; i < rs.size(); i++)
		{
			double x1 = scale_dim_x(rs[i].start.x);
			double y1 = scale_dim_y(rs[i].start.y);
			double x2;
			double y2;
			if (rs[i].dir.x > 0)
			{
				x2 = 1000;
				y2 = scale_dim_y(rs[i].start.y) + rs[i].dir.y * (x2 - scale_dim_x(rs[i].start.x)) / rs[i].dir.x;
			}
			else if (rs[i].dir.x < 0)
			{
				x2 = 0;
				y2 = scale_dim_y(rs[i].start.y) - rs[i].dir.y * scale_dim_x(rs[i].start.x) / rs[i].dir.x;
			}
			else if (rs[i].dir.y > 0)
			{
				x2 = x1;
				y2 = 1000;
			}
			else if (rs[i].dir.y < 0)
			{
				x2 = x1;
				y2 = -1000;
			}

			memDC.MoveTo(x1, y1);
			memDC.LineTo(x2, y2);
			if (sh_element_label)
			{
				memDC.SetTextColor(RGB(200, 0, 0));
				swprintf(buffer, _T("R%d"), i);
				memDC.TextOut(x1, y1 + 5.0, buffer);
			}
		}
		memDC.SelectObject(pOldPen);

		CPen pen_l(PS_SOLID, 2, RGB(0, 200, 0));
		pOldPen = memDC.SelectObject(&pen_l);
		for (int i = 0; i < ls.size(); i++)
		{
			if (ls[i].dir.x == 0)
			{
				double x = scale_dim_x(ls[i].pos.x);
				memDC.MoveTo(x, -1000);
				memDC.LineTo(x, 1000);
			}
			else
			{
				double x1 = 0;
				double y1 = scale_dim_y(ls[i].pos.y) - ls[i].dir.y * scale_dim_x(ls[i].pos.x) / ls[i].dir.x;
				double x2 = 1000;
				double y2 = scale_dim_y(ls[i].pos.y) + ls[i].dir.y * (x2 - scale_dim_x(ls[i].pos.x)) / ls[i].dir.x;

				memDC.MoveTo(x1, y1);
				memDC.LineTo(x2, y2);
			}
			if (sh_element_label)
			{
				memDC.SetTextColor(RGB(0, 200, 0));
				swprintf(buffer, _T("L%d"), i);
				memDC.TextOut(scale_dim_x(ls[i].pos.x), scale_dim_y(ls[i].pos.y) + 5.0, buffer);
			}
		}
		memDC.SelectObject(pOldPen);

		CPen pen_c(PS_SOLID, 2, RGB(0, 0, 200));
		pOldPen = memDC.SelectObject(&pen_c);
		memDC.SelectObject(GetStockObject(HOLLOW_BRUSH));
		for (int i = 0; i < cs.size(); i++)
		{
			double x = scale_dim_x(cs[i].center.x);
			double y = scale_dim_y(cs[i].center.y);
			double r = scale_dim_x(cs[i].center.x) - scale_dim_x(cs[i].center.x - cs[i].radius);
			memDC.Ellipse(x - r, y - r, x + r, y + r);
			if (sh_element_label)
			{
				memDC.SetTextColor(RGB(0, 0, 200));
				swprintf(buffer, _T("C%d"), i);
				memDC.TextOut(x - 5.0, y - 5.0, buffer);
			}
		}
		memDC.SelectObject(pOldPen);

		if (show_inter)
		{
			CPen pen_i(PS_SOLID, 1, RGB(0, 0, 0));
			pOldPen = memDC.SelectObject(&pen_i);
			CBrush brush2(RGB(255, 100, 100));
			pOldBrush = memDC.SelectObject(&brush2);
			for (int i = 0; i < its.size(); i++)
			{
				double x = scale_dim_x(its[i].x);
				double y = scale_dim_y(its[i].y);
				memDC.Ellipse(x - 4, y - 4, x + 4, y + 4);
				if (show_label)
				{
					memDC.SetTextColor(RGB(0, 0, 0));
					swprintf(buffer, _T("[%d]"), i);
					memDC.TextOut(x, y + 6.0, buffer);
				}
			}
			memDC.SelectObject(pOldPen);
			memDC.SelectObject(pOldBrush);
		}

		dc.BitBlt(r.left, r.top, r.right, r.bottom, &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pOldBmp);
		memDC.DeleteDC();
		memBitmap.DeleteObject();
		DeleteDC(memDC);
	}
}

double CIntersectionViewerDlg::scale_dim_x(double v)
{
	return transform.x + 135 - 250 * (scale - 1.0) + 500 * scale * (v - dim_min) / (dim_max - dim_min);
}

double CIntersectionViewerDlg::scale_dim_y(double v)
{
	return transform.y + 20 - 250 * (scale - 1.0) + 500 * scale * (v - dim_min) / (dim_max - dim_min);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CIntersectionViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CIntersectionViewerDlg::OnBnClickedButtonOpenScene()
{
	// TODO:  在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("Scene Files (*.txt)|*.txt|All Files (*.*)|*.*||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		ss.clear();
		rs.clear();
		ls.clear();
		cs.clear();

		std::ifstream input(dlg.GetPathName(), std::ifstream::in);
		std::string l;
		double max = -9999, min = 9999;
		std::getline(input, l);
		std::istringstream niss(l);
		int nseg, nray, nline, ncircle;
		niss >> nseg >> nray >> nline >> ncircle;
		for (int i = 0; i < nseg; i++)
		{
			std::getline(input, l);
			std::istringstream iss(l);
			double x1, y1, x2, y2;
			iss >> x1 >> y1 >> x2 >> y2;
			ss.push_back(seg(point(x1, y1), point(x2, y2)));
			double tmin = MIN4(x1, y1, x2, y2);
			min = tmin < min ? tmin : min;
			double tmax = MAX4(x1, y1, x2, y2);
			max = tmax > max ? tmax : max;
		}
		for (int i = 0; i < nray; i++)
		{
			std::getline(input, l);
			std::istringstream iss(l);
			double x1, y1, x2, y2;
			iss >> x1 >> y1 >> x2 >> y2;
			rs.push_back(ray(point(x1, y1), vect(x2 - x1, y2 - y1)));
			double tmin = MIN2(x1, y1);
			min = tmin < min ? tmin : min;
			double tmax = MAX2(x1, y1);
			max = tmax > max ? tmax : max;
		}
		for (int i = 0; i < nline; i++)
		{
			std::getline(input, l);
			std::istringstream iss(l);
			double x1, y1, x2, y2;
			iss >> x1 >> y1 >> x2 >> y2;
			ls.push_back(line(point(x1, y1), vect(x2 - x1, y2 - y1)));
			double tmin = MIN2(x1, y1);
			min = tmin < min ? tmin : min;
			double tmax = MAX2(x1, y1);
			max = tmax > max ? tmax : max;
		}
		for (int i = 0; i < ncircle; i++)
		{
			std::getline(input, l);
			std::istringstream iss(l);
			double x, y, r;
			iss >> x >> y >> r;
			cs.push_back(circle(point(x, y), r));
			double tmin = MIN4(x, y, x - r, y - r);
			min = tmin < min ? tmin : min;
			double tmax = MAX4(x, y, x + r, y + r);
			max = tmax > max ? tmax : max;
		}
		dim_max = max;
		dim_min = min;

		GetDlgItem(IDC_BUTTON_OPEN_INTERSECTION)->EnableWindow(TRUE);
		Invalidate();
	}
	else
		return;
}


void CIntersectionViewerDlg::OnBnClickedButtonOpenIntersection()
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
		its.clear();
		std::ifstream input(dlg.GetPathName(), std::ifstream::in);
		std::string line;
		while (std::getline(input, line))
		{
			std::istringstream iss(line);
			double x, y;
			iss >> x >> y;
			its.push_back(point(x, y));
		}
		show_intersection.EnableWindow(TRUE);
		Invalidate();
	}
	else
		return;
}


void CIntersectionViewerDlg::OnBnClickedButtonResetView()
{
	// TODO:  在此添加控件通知处理程序代码
	scale = 1.0;
	transform = point();
	drag_start_transform = point();
	Invalidate();
}


void CIntersectionViewerDlg::OnBnClickedCheckShowIntersection()
{
	// TODO:  在此添加控件通知处理程序代码
	if (show_intersection.GetCheck() == 0)
		show_inter = false;
	else
		show_inter = true;
	Invalidate();
}


BOOL CIntersectionViewerDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	scale *= 1 + zDelta / 2000.0;
	if (scale <= 0) scale = 0;
	Invalidate();
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CIntersectionViewerDlg::OnMouseMove(UINT nFlags, CPoint point)
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

void CIntersectionViewerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	dragging = true;
	drag_start_pos = point;
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CIntersectionViewerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	dragging = false;
	drag_start_transform = transform;
	CDialogEx::OnLButtonUp(nFlags, point);
}


BOOL CIntersectionViewerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	return TRUE;
}


void CIntersectionViewerDlg::OnBnClickedCheckShowLabel()
{
	// TODO:  在此添加控件通知处理程序代码
	if (show_label_btn.GetCheck() == 0)
		show_label = false;
	else
		show_label = true;
	Invalidate();
}


void CIntersectionViewerDlg::OnBnClickedCheckShowElementLabel()
{
	// TODO:  在此添加控件通知处理程序代码
	if (show_element_label.GetCheck() == 0)
		sh_element_label = false;
	else
		sh_element_label = true;
	Invalidate();
}

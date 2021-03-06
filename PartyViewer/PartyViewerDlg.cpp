
// PartyViewerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PartyViewer.h"
#include "PartyViewerDlg.h"
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


// CPartyViewerDlg 对话框



CPartyViewerDlg::CPartyViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPartyViewerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPartyViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_SHOW_PAIRS, show_pairs_check);
	DDX_Control(pDX, IDC_CHECK_SHOW_PERSON_ID, show_person_id_check);
}

BEGIN_MESSAGE_MAP(CPartyViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_PARTY_MAP, &CPartyViewerDlg::OnBnClickedButtonOpenPartyMap)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_PAIRS, &CPartyViewerDlg::OnBnClickedButtonOpenPairs)
	ON_BN_CLICKED(IDC_CHECK_SHOW_PERSON_ID, &CPartyViewerDlg::OnBnClickedCheckShowPersonId)
	ON_BN_CLICKED(IDC_CHECK_SHOW_PAIRS, &CPartyViewerDlg::OnBnClickedCheckShowPairs)
	ON_BN_CLICKED(IDC_BUTTON_RESET_VIEW, &CPartyViewerDlg::OnBnClickedButtonResetView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CPartyViewerDlg 消息处理程序

BOOL CPartyViewerDlg::OnInitDialog()
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
	GetDlgItem(IDC_BUTTON_OPEN_PAIRS)->EnableWindow(false);
	show_pairs_check.EnableWindow(false);
	show_pairs_check.SetCheck(1);
	show_person_id_check.SetCheck(1);
	show_pairs = true;
	show_person_id = true;
	scale = 1.0;
	transform = point();
	drag_start_transform = transform;
	dragging = false;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPartyViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPartyViewerDlg::OnPaint()
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

		CPen pen2(PS_SOLID, 1, RGB(200, 0, 0));
		pOldPen = memDC.SelectObject(&pen2);
		if (show_pairs && pairs.size() > 0)
		{
			for (int i = 0; i < pairs.size(); i++)
			{
				int girl_id = pairs[i].first;
				int boy_id = pairs[i].second;

				double gx = scale_dim_x(girls_pts[girl_id].x);
				double gy = scale_dim_y(girls_pts[girl_id].y);
				double bx = scale_dim_x(boys_pts[boy_id].x);
				double by = scale_dim_y(boys_pts[boy_id].y);

				memDC.MoveTo(gx, gy);
				memDC.LineTo(bx, by);
			}
		}
		memDC.SelectObject(pOldPen);

		CPen pen1(PS_SOLID, 1, RGB(0, 0, 0));
		pOldPen = memDC.SelectObject(&pen1);
		if (boys_pts.size() > 0)
		{
			CBrush brush1(RGB(100, 255, 100));
			pOldBrush = memDC.SelectObject(&brush1);
			for (int i = 0; i < boys_pts.size(); i++)
			{
				double x = scale_dim_x(boys_pts[i].x);
				double y = scale_dim_y(boys_pts[i].y);
				memDC.Ellipse(x - 5, y - 5, x + 5, y + 5);

				if (show_person_id)
				{
					SetBkMode(memDC, TRANSPARENT);
					TCHAR buffer[100];
					wsprintf(buffer, _T("[%d]"), i);
					memDC.TextOut(x + 10.0, y + 5.0, buffer);
				}
			}
			memDC.SelectObject(pOldBrush);
		}
		if (girls_pts.size() > 0)
		{
			CBrush brush2(RGB(255, 100, 100));
			pOldBrush = memDC.SelectObject(&brush2);
			for (int i = 0; i < girls_pts.size(); i++)
			{
				double x = scale_dim_x(girls_pts[i].x);
				double y = scale_dim_y(girls_pts[i].y);
				memDC.Ellipse(x - 3, y - 3, x + 3, y + 3);

				if (show_person_id)
				{
					SetBkMode(memDC, TRANSPARENT);
					TCHAR buffer[100];
					wsprintf(buffer, _T("[%d]"), i);
					memDC.TextOut(x + 10.0, y + 5.0, buffer);
				}
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

double CPartyViewerDlg::scale_dim_x(double v)
{
	return transform.x + 135 - 250 * (scale - 1.0) + 500 * scale * (v - polygon_dim_min) / (polygon_dim_max - polygon_dim_min);
}

double CPartyViewerDlg::scale_dim_y(double v)
{
	return transform.y + 20 - 250 * (scale - 1.0) + 500 * scale * (v - polygon_dim_min) / (polygon_dim_max - polygon_dim_min);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPartyViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPartyViewerDlg::OnBnClickedButtonOpenPartyMap()
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
		boys_pts.clear();
		girls_pts.clear();
		pairs.clear();
		std::string line;
		double max = -9999, min = 9999;
		std::ifstream input(dlg.GetPathName(), std::ifstream::in);
		std::getline(input, line);
		std::istringstream biss(line);
		int bcount = 0;
		biss >> bcount;
		for (int i = 0; i < bcount; i++)
		{
			std::getline(input, line);
			std::istringstream iss(line);
			double x, y;
			iss >> x >> y;
			boys_pts.push_back(point(x, y));
			if (x > max) max = x;
			if (y > max) max = y;
			if (x < min) min = x;
			if (y < min) min = y;
		}
		std::getline(input, line);
		std::istringstream giss(line);
		int gcount = 0;
		giss >> gcount;
		for (int i = 0; i < gcount; i++)
		{
			std::getline(input, line);
			std::istringstream iss(line);
			double x, y;
			iss >> x >> y;
			girls_pts.push_back(point(x, y));
			if (x > max) max = x;
			if (y > max) max = y;
			if (x < min) min = x;
			if (y < min) min = y;
		}
		polygon_dim_max = max;
		polygon_dim_min = min;

		GetDlgItem(IDC_BUTTON_OPEN_PAIRS)->EnableWindow(TRUE);
		Invalidate();
	}
	else
		return;
}


void CPartyViewerDlg::OnBnClickedButtonOpenPairs()
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
		pairs.clear();
		std::string line;
		std::ifstream input(dlg.GetPathName(), std::ifstream::in);
		for (int i = 0; i < girls_pts.size(); i++)
		{
			std::getline(input, line);
			pairs.push_back(std::make_pair(i, stoi(line)));
		}

		show_pairs_check.EnableWindow(TRUE);
		Invalidate();
	}
	else
		return;
}


void CPartyViewerDlg::OnBnClickedCheckShowPersonId()
{
	// TODO:  在此添加控件通知处理程序代码
	if (show_person_id_check.GetCheck() == 0)
		show_person_id = false;
	else
		show_person_id = true;
	Invalidate();
}


void CPartyViewerDlg::OnBnClickedCheckShowPairs()
{
	// TODO:  在此添加控件通知处理程序代码
	if (show_pairs_check.GetCheck() == 0)
		show_pairs = false;
	else
		show_pairs = true;
	Invalidate();
}


void CPartyViewerDlg::OnBnClickedButtonResetView()
{
	// TODO:  在此添加控件通知处理程序代码
	scale = 1.0;
	transform = point();
	drag_start_transform = point();
	Invalidate();
}


void CPartyViewerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	dragging = true;
	drag_start_pos = point;
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CPartyViewerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	dragging = false;
	drag_start_transform = transform;
	CDialogEx::OnLButtonUp(nFlags, point);
}


BOOL CPartyViewerDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	scale *= 1 + zDelta / 2000.0;
	if (scale <= 0) scale = 0;
	Invalidate();
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


BOOL CPartyViewerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	return TRUE;
}


void CPartyViewerDlg::OnMouseMove(UINT nFlags, CPoint point)
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

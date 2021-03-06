
// RoomViewerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RoomViewer.h"
#include "RoomViewerDlg.h"
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


// CRoomViewerDlg 对话框



CRoomViewerDlg::CRoomViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRoomViewerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRoomViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_SHOW_PATH, show_path_btn);
	DDX_Control(pDX, IDC_CHECK_SHOW_CORNER_ID, show_corner_id_btn);
}

BEGIN_MESSAGE_MAP(CRoomViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_ROOM_MAP, &CRoomViewerDlg::OnBnClickedButtonOpenRoomMap)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_PATH, &CRoomViewerDlg::OnBnClickedButtonOpenPath)
	ON_BN_CLICKED(IDC_CHECK_SHOW_PATH, &CRoomViewerDlg::OnBnClickedCheckShowPath)
	ON_BN_CLICKED(IDC_BUTTON_RESET_VIEW, &CRoomViewerDlg::OnBnClickedButtonResetView)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_CHECK_SHOW_CORNER_ID, &CRoomViewerDlg::OnBnClickedCheckShowCornerId)
END_MESSAGE_MAP()


// CRoomViewerDlg 消息处理程序

BOOL CRoomViewerDlg::OnInitDialog()
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
	GetDlgItem(IDC_BUTTON_OPEN_PATH)->EnableWindow(false);
	show_path_btn.EnableWindow(false);
	show_path_btn.SetCheck(1);
	show_corner_id_btn.SetCheck(1);
	show_path = true;
	show_corner_id = true;
	scale = 1.0;
	transform = point();
	drag_start_transform = transform;
	dragging = false;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRoomViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CRoomViewerDlg::OnPaint()
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

		CBrush brush1(RGB(200, 200, 255));
		pOldBrush = memDC.SelectObject(&brush1);
		if (polygon_points.size() > 0)
		{
			CPoint *pts = new CPoint[polygon_points.size()];
			for (int i = 0; i < polygon_points.size(); i++)
			{
				double x = scale_dim_x(polygon_points[i].x);
				double y = scale_dim_y(polygon_points[i].y);
				pts[i] = CPoint(x, y);
			}
			memDC.Polygon(pts, polygon_points.size());
			delete[] pts;
		}
		memDC.SelectObject(pOldBrush);

		if (show_path && shortest_path.size() > 0)
		{
			CPen pen(PS_SOLID, 3, RGB(200, 0, 0));
			pOldPen = memDC.SelectObject(&pen);
			memDC.MoveTo(scale_dim_x(start.x), scale_dim_y(start.y));
			for (int i = 0; i < shortest_path.size(); i++)
			{
				int pid = shortest_path[i];
				if (pid != -1 && pid != -2)
				{
					double sx = scale_dim_x(polygon_points[pid].x);
					double sy = scale_dim_y(polygon_points[pid].y);
					memDC.LineTo(sx, sy);
				}
			}
			memDC.LineTo(scale_dim_x(end.x), scale_dim_y(end.y));
			memDC.SelectObject(pOldPen);
		}

		if (show_corner_id)
		{
			CBrush brush2(RGB(100, 255, 100));
			pOldBrush = memDC.SelectObject(&brush2);
			for (int i = 0; i < polygon_points.size(); i++)
			{
				double x = scale_dim_x(polygon_points[i].x);
				double y = scale_dim_y(polygon_points[i].y);
				memDC.Ellipse(x - 3, y - 3, x + 3, y + 3);

				SetBkMode(memDC, TRANSPARENT);
				TCHAR buffer[100];
				wsprintf(buffer, _T("[%d]"), i);
				memDC.TextOut(x + 10.0, y + 5.0, buffer);
			}
			memDC.SelectObject(pOldBrush);
		}

		if (polygon_points.size() > 0)
		{
			CBrush brush2(RGB(255, 100, 100));
			pOldBrush = memDC.SelectObject(&brush2);
			double sx = scale_dim_x(start.x);
			double sy = scale_dim_y(start.y);
			memDC.Ellipse(sx - 5, sy - 5, sx + 5, sy + 5);

			double ex = scale_dim_x(end.x);
			double ey = scale_dim_y(end.y);
			memDC.Ellipse(ex - 5, ey - 5, ex + 5, ey + 5);

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

double CRoomViewerDlg::scale_dim_x(double v)
{
	return transform.x + 135 - 250 * (scale - 1.0) + 500 * scale * (v - polygon_dim_min) / (polygon_dim_max - polygon_dim_min);
}

double CRoomViewerDlg::scale_dim_y(double v)
{
	return transform.y + 20 - 250 * (scale - 1.0) + 500 * scale * (v - polygon_dim_min) / (polygon_dim_max - polygon_dim_min);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRoomViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRoomViewerDlg::OnBnClickedButtonOpenRoomMap()
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
		polygon_points.clear();
		shortest_path.clear();
		std::ifstream input(dlg.GetPathName(), std::ifstream::in);
		std::string line;
		double max = -9999, min = 9999;
		std::getline(input, line);
		std::istringstream seiss(line);
		double sx, sy, ex, ey;
		seiss >> sx >> sy >> ex >> ey;
		start = point(sx, sy);
		end = point(ex, ey);

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
			polygon_points.push_back(point(x, y));
			if (x > max) max = x;
			if (y > max) max = y;
			if (x < min) min = x;
			if (y < min) min = y;
		}
		polygon_dim_max = max;
		polygon_dim_min = min;

		GetDlgItem(IDC_BUTTON_OPEN_PATH)->EnableWindow(TRUE);
		Invalidate();
	}
	else
		return;
}


void CRoomViewerDlg::OnBnClickedButtonOpenPath()
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
		shortest_path.clear();
		std::ifstream input(dlg.GetPathName(), std::ifstream::in);
		std::string line;
		shortest_path.push_back(-1);
		while (std::getline(input, line))
		{
			if (line[0] != 's' && line[0] != 'e')
				shortest_path.push_back(stoi(line));
		}
		shortest_path.push_back(-2);
		show_path_btn.EnableWindow(TRUE);
		Invalidate();
	}
	else
		return;
}


void CRoomViewerDlg::OnBnClickedCheckShowPath()
{
	// TODO:  在此添加控件通知处理程序代码
	if (show_path_btn.GetCheck() == 0)
		show_path = false;
	else
		show_path = true;
	Invalidate();
}


void CRoomViewerDlg::OnBnClickedButtonResetView()
{
	// TODO:  在此添加控件通知处理程序代码
	scale = 1.0;
	transform = point();
	drag_start_transform = point();
	Invalidate();
}


BOOL CRoomViewerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	return TRUE;
}


void CRoomViewerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	dragging = true;
	drag_start_pos = point;
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CRoomViewerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	dragging = false;
	drag_start_transform = transform;
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CRoomViewerDlg::OnMouseMove(UINT nFlags, CPoint point)
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


BOOL CRoomViewerDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	scale *= 1 + zDelta / 2000.0;
	if (scale <= 0) scale = 0;
	Invalidate();
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void CRoomViewerDlg::OnBnClickedCheckShowCornerId()
{
	// TODO:  在此添加控件通知处理程序代码
	if (show_corner_id_btn.GetCheck() == 0)
		show_corner_id = false;
	else
		show_corner_id = true;
	Invalidate();
}

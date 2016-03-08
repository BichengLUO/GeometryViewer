
// WallsViewerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WallsViewer.h"
#include "WallsViewerDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX2(a,b) ((a)>(b)?(a):(b))
#define MAX4(a,b,c,d) (MAX2((a),(b))>MAX2((c),(d))?MAX2((a),(b)):MAX2((c),(d)))
#define MIN2(a,b) ((a)<(b)?(a):(b))
#define MIN4(a,b,c,d) (MIN2((a),(b))<MIN2((c),(d))?MIN2((a),(b)):MIN2((c),(d)))

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


// CWallsViewerDlg 对话框



CWallsViewerDlg::CWallsViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWallsViewerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWallsViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_CHECK_SHOW_RESULTS, showResultsBtn);
	DDX_Control(pDX, IDC_LIST_QUERIES, queries_list_box);
	DDX_Control(pDX, IDC_CHECK_SHOW_RESULTS, show_results_btn);
}

BEGIN_MESSAGE_MAP(CWallsViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_SEGMENTS_QUERIES, &CWallsViewerDlg::OnBnClickedButtonOpenSegmentsQueries)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_RESULTS, &CWallsViewerDlg::OnBnClickedButtonOpenResults)
	ON_BN_CLICKED(IDC_BUTTON_RESET_VIEW, &CWallsViewerDlg::OnBnClickedButtonResetView)
	ON_BN_CLICKED(IDC_CHECK_SHOW_RESULTS, &CWallsViewerDlg::OnBnClickedCheckShowResults)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_ERASEBKGND()
	ON_LBN_SELCHANGE(IDC_LIST_QUERIES, &CWallsViewerDlg::OnLbnSelchangeListQueries)
END_MESSAGE_MAP()


// CWallsViewerDlg 消息处理程序

BOOL CWallsViewerDlg::OnInitDialog()
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
	GetDlgItem(IDC_BUTTON_OPEN_RESULTS)->EnableWindow(false);
	show_results_btn.EnableWindow(false);
	show_results_btn.SetCheck(1);
	show_results = true;
	scale = 1.0;
	transform = point();
	drag_start_transform = transform;
	dragging = false;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWallsViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CWallsViewerDlg::OnPaint()
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
		r.bottom -= 50;
		r.right -= 250;

		CBitmap memBitmap;
		CBitmap* pOldBmp = NULL;
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		memBitmap.CreateCompatibleBitmap(&dc, r.right, r.bottom);
		pOldBmp = memDC.SelectObject(&memBitmap);
		memDC.FillSolidRect(r, dc.GetBkColor());
		CDC *old_pDC = &dc;
		//创建一个内存中的图像
		TCHAR buffer[100];
		SetBkMode(memDC, TRANSPARENT);

		CPen *pOldPen;
		CBrush *pOldBrush;

		CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
		pOldPen = memDC.SelectObject(&pen);
		int selected = queries_list_box.GetCurSel();
		for (int i = 0; i < point_queries.size(); i++)
		{
			CBrush brush2(RGB(255, 200, 100));
			pOldBrush = memDC.SelectObject(&brush2);
			double x = scale_dim_x(point_queries[i].x);
			double y = scale_dim_y(point_queries[i].y);
			memDC.Ellipse(x - 3, y - 3, x + 3, y + 3);
			memDC.SelectObject(pOldBrush);
		}
		memDC.SelectObject(pOldPen);

		CBrush brush1(RGB(255, 100, 100));
		CPen redPen(PS_SOLID, 2, RGB(255, 0, 0));
		CPen grayPen(PS_SOLID, 1, RGB(180, 180, 180));
		CPen blackPen(PS_SOLID, 1, RGB(0, 0, 0));
		pOldBrush = memDC.SelectObject(&brush1);
		for (int i = 0; i < segments.size(); i++)
		{
			if (show_results && selected != -1 && selected < queries_result.size() &&
				queries_result[selected] != i)
			{
				pOldPen = memDC.SelectObject(&redPen);
				memDC.SetTextColor(RGB(255, 0, 0));
			}
			else if (show_results && selected != -1)
			{
				pOldPen = memDC.SelectObject(&grayPen);
				memDC.SetTextColor(RGB(180, 180, 180));
			}
			else
			{
				pOldPen = memDC.SelectObject(&blackPen);
				memDC.SetTextColor(RGB(0, 0, 0));
			}
			double x = scale_dim_x(segments[i].x);
			double y = scale_dim_y(segments[i].y);
			double u = scale_dim_x(segments[i].u);
			double v = scale_dim_y(segments[i].v);

			memDC.MoveTo(x, y);
			memDC.LineTo(u, v);

			memDC.Ellipse(x - 3, y - 3, x + 3, y + 3);
			memDC.Ellipse(u - 3, v - 3, u + 3, v + 3);
			wsprintf(buffer, _T("[%d]"), i + 1);
			memDC.TextOut((x + u) / 2.0, (y + v) / 2.0 + 3.0, buffer);

			memDC.SelectObject(pOldPen);
		}
		memDC.SelectObject(pOldBrush);

		dc.BitBlt(r.left, r.top, r.right, r.bottom, &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pOldBmp);
		memDC.DeleteDC();
		memBitmap.DeleteObject();
		DeleteDC(memDC);
	}
}

double CWallsViewerDlg::scale_dim_x(double v)
{
	return transform.x + 135 - 250 * (scale - 1.0) + 500 * scale * (v - dim_min) / (dim_max - dim_min);
}

double CWallsViewerDlg::scale_dim_y(double v)
{
	return transform.y + 20 - 250 * (scale - 1.0) + 500 * scale * (v - dim_min) / (dim_max - dim_min);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWallsViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CWallsViewerDlg::OnBnClickedButtonOpenSegmentsQueries()
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
		segments.clear();
		point_queries.clear();
		queries_result.clear();
		queries_list_box.ResetContent();
		std::ifstream input(dlg.GetPathName(), std::ifstream::in);
		std::string line;
		double max = -9999, min = 9999;
		std::getline(input, line);
		int segs_count, queries_count;
		std::istringstream iss(line);
		iss >> segs_count >> queries_count;
		for (int i = 0; i < segs_count; i++)
		{
			std::getline(input, line);
			std::istringstream iss1(line);
			double x, y, u, v;
			iss1 >> x >> y >> u >> v;
			segments.push_back(seg(x, -y, u, -v));
			max = MAX4(x, -y, u, -v) > max ? MAX4(x, -y, u, -v) : max;
			min = MIN4(x, -y, u, -v) < min ? MIN4(x, -y, u, -v) : min;
		}
		for (int i = 0; i < queries_count; i++)
		{
			std::getline(input, line);
			std::istringstream iss2(line);
			double x, y;
			iss2 >> x >> y;
			point_queries.push_back(query(x, -y));
			max = MAX2(x, -y) > max ? MAX2(x, -y) : max;
			min = MIN2(x, -y) < min ? MIN2(x, -y) : min;
			TCHAR label[100];
			swprintf(label, _T("[%2d] %.1f, %.1f"), i + 1, x, y);
			queries_list_box.InsertString(queries_list_box.GetCount(), label);
		}
		dim_max = max;
		dim_min = min;
		GetDlgItem(IDC_BUTTON_OPEN_RESULTS)->EnableWindow(TRUE);
		redraw();
	}
	else
		return;
}


void CWallsViewerDlg::OnBnClickedButtonOpenResults()
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
		queries_result.clear();
		std::ifstream input(dlg.GetPathName(), std::ifstream::in);
		std::string line;
		while (std::getline(input, line))
		{
			std::stringstream ss(line);
			int seg_id;
			ss >> seg_id;
			queries_result.push_back(seg_id);
		}
		show_results_btn.EnableWindow(TRUE);
		queries_list_box.SetCurSel(0);
		Invalidate();
	}
	else
		return;
}


void CWallsViewerDlg::OnBnClickedButtonResetView()
{
	// TODO:  在此添加控件通知处理程序代码
	scale = 1.0;
	transform = point();
	drag_start_transform = point();
	redraw();
}


void CWallsViewerDlg::OnBnClickedCheckShowResults()
{
	// TODO:  在此添加控件通知处理程序代码
	if (show_results_btn.GetCheck() == 0)
		show_results = false;
	else
		show_results = true;
	redraw();
}


void CWallsViewerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	dragging = true;
	drag_start_pos = point;
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CWallsViewerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	dragging = false;
	drag_start_transform = transform;
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CWallsViewerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (dragging)
	{
		transform.x = drag_start_transform.x + point.x - drag_start_pos.x;
		transform.y = drag_start_transform.y + point.y - drag_start_pos.y;
		redraw();
	}
	CRect r;
	GetClientRect(&r);
	r.bottom -= 50;
	r.right -= 250;
	if (point.x < r.right && point.y < r.bottom)
		GetDlgItem(IDC_BUTTON_OPEN_RESULTS)->SetFocus();
	CDialogEx::OnMouseMove(nFlags, point);
}


BOOL CWallsViewerDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	scale *= 1 + zDelta / 2000.0;
	if (scale <= 0) scale = 0;
	redraw();
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CWallsViewerDlg::redraw()
{
	CRect r;
	GetClientRect(&r);
	r.bottom -= 50;
	r.right -= 250;
	InvalidateRect(r);
}


BOOL CWallsViewerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	return TRUE;
}


void CWallsViewerDlg::OnLbnSelchangeListQueries()
{
	// TODO:  在此添加控件通知处理程序代码
	redraw();
}

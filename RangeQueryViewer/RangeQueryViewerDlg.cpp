
// RangeQueryViewerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RangeQueryViewer.h"
#include "RangeQueryViewerDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <string>
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


// CRangeQueryViewerDlg 对话框



CRangeQueryViewerDlg::CRangeQueryViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRangeQueryViewerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRangeQueryViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_QUERIES, queries_list_box);
}

BEGIN_MESSAGE_MAP(CRangeQueryViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_POINTS_QUERIES, &CRangeQueryViewerDlg::OnBnClickedButtonOpenPointsQueries)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_RESULTS, &CRangeQueryViewerDlg::OnBnClickedButtonOpenResults)
	ON_BN_CLICKED(IDC_BUTTON_RESET_VIEW, &CRangeQueryViewerDlg::OnBnClickedButtonResetView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_ERASEBKGND()
	ON_LBN_SELCHANGE(IDC_LIST_QUERIES, &CRangeQueryViewerDlg::OnLbnSelchangeListQueries)
	ON_BN_CLICKED(IDC_CHECK_SHOW_RESULTS, &CRangeQueryViewerDlg::OnBnClickedCheckShowResults)
END_MESSAGE_MAP()


// CRangeQueryViewerDlg 消息处理程序

BOOL CRangeQueryViewerDlg::OnInitDialog()
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
	GetDlgItem(IDC_BUTTON_OPEN_RESULTS)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_SHOW_RESULTS)->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_SHOW_RESULTS))->SetCheck(TRUE);
	show_results = true;
	scale = 1.0;
	transform = point();
	drag_start_transform = transform;
	dragging = false;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRangeQueryViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CRangeQueryViewerDlg::OnPaint()
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

		CPen *pOldPen;
		CBrush *pOldBrush;

		CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
		pOldPen = memDC.SelectObject(&pen);
		int selected = queries_list_box.GetCurSel();
		if (selected != -1)
		{
			CBrush brush2(RGB(255, 200, 100));
			pOldBrush = memDC.SelectObject(&brush2);
			double x = scale_dim_x(queries[selected].x);
			double y = scale_dim_y(queries[selected].y);
			double xp = scale_dim_x(queries[selected].x + queries[selected].w);
			double yp = scale_dim_y(queries[selected].y + queries[selected].h);
			memDC.Rectangle(x, y, xp, yp);
			memDC.SelectObject(pOldBrush);
		}
		memDC.SelectObject(pOldPen);

		CBrush brush1(RGB(255, 100, 100));
		CPen redPen(PS_SOLID, 2, RGB(255, 0, 0));
		CPen grayPen(PS_SOLID, 1, RGB(180, 180, 180));
		CPen blackPen(PS_SOLID, 1, RGB(0, 0, 0));
		pOldBrush = memDC.SelectObject(&brush1);
		for (int i = 0; i < pts.size(); i++)
		{
			if (show_results && selected != -1 && selected < queries_result.size() &&
				queries_result[selected].find(i) != queries_result[selected].end())
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
			double x = scale_dim_x(pts[i].x);
			double y = scale_dim_y(pts[i].y);

			memDC.Ellipse(x - 3, y - 3, x + 3, y + 3);

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

double CRangeQueryViewerDlg::scale_dim_x(double v)
{
	return transform.x + 135 - 250 * (scale - 1.0) + 500 * scale * (v - dim_min) / (dim_max - dim_min);
}

double CRangeQueryViewerDlg::scale_dim_y(double v)
{
	return transform.y + 20 - 250 * (scale - 1.0) + 500 * scale * (v - dim_min) / (dim_max - dim_min);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRangeQueryViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRangeQueryViewerDlg::OnBnClickedButtonOpenPointsQueries()
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
		queries.clear();
		queries_result.clear();
		queries_list_box.ResetContent();
		std::ifstream input(dlg.GetPathName(), std::ifstream::in);
		std::string line;
		double max = -9999, min = 9999;
		std::getline(input, line);
		int pts_count, queries_count;
		std::istringstream iss(line);
		iss >> pts_count >> queries_count;
		for (int i = 0; i < pts_count; i++)
		{
			std::getline(input, line);
			std::istringstream iss(line);
			double x, y;
			iss >> x >> y;
			pts.push_back(point(x, y));
			max = MAX2(x, y) > max ? MAX2(x, y) : max;
			min = MIN2(x, y) < min ? MIN2(x, y) : min;
		}
		for (int i = 0; i < queries_count; i++)
		{
			std::getline(input, line);
			std::istringstream iss2(line);
			double x1, y1, x2, y2;
			iss2 >> x1 >> y1 >> x2 >> y2;
			queries.push_back(window_query(x1, y1, x2 - x1, y2 - y1));
			max = MAX2(x2, y2) > max ? MAX2(x2, y2) : max;
			min = MIN2(x1, y1) < min ? MIN2(x1, y1) : min;
			TCHAR label[100];
			swprintf(label, _T("[%2d] %.1f, %.1f, %.1f, %.1f"), i + 1, x1, y1, x2 - x1, y2 - y1);
			queries_list_box.InsertString(queries_list_box.GetCount(), label);
		}
		dim_max = max;
		dim_min = min;
		GetDlgItem(IDC_BUTTON_OPEN_RESULTS)->EnableWindow(TRUE);
		redraw();
	}
	else return;
}


void CRangeQueryViewerDlg::OnBnClickedButtonOpenResults()
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
			std::set<int> range;
			if (line[0] != 'N')
			{
				std::stringstream ss(line);
				int seg_id;
				while (ss >> seg_id)
					range.insert(seg_id);
			}
			queries_result.push_back(range);
		}
		GetDlgItem(IDC_CHECK_SHOW_RESULTS)->EnableWindow(TRUE);
		queries_list_box.SetCurSel(0);
		Invalidate();
	}
	else
		return;
}


void CRangeQueryViewerDlg::OnBnClickedButtonResetView()
{
	// TODO:  在此添加控件通知处理程序代码
	scale = 1.0;
	transform = point();
	drag_start_transform = point();
	redraw();
}


void CRangeQueryViewerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	dragging = true;
	drag_start_pos = point;
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CRangeQueryViewerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	dragging = false;
	drag_start_transform = transform;
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CRangeQueryViewerDlg::OnMouseMove(UINT nFlags, CPoint point)
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


BOOL CRangeQueryViewerDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	scale *= 1 + zDelta / 2000.0;
	if (scale <= 0) scale = 0;
	redraw();
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CRangeQueryViewerDlg::redraw()
{
	CRect r;
	GetClientRect(&r);
	r.bottom -= 50;
	r.right -= 250;
	InvalidateRect(r);
}


BOOL CRangeQueryViewerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	return TRUE;
}


void CRangeQueryViewerDlg::OnLbnSelchangeListQueries()
{
	// TODO:  在此添加控件通知处理程序代码
	redraw();
}


void CRangeQueryViewerDlg::OnBnClickedCheckShowResults()
{
	// TODO:  在此添加控件通知处理程序代码
	if (((CButton*)GetDlgItem(IDC_CHECK_SHOW_RESULTS))->GetCheck() == FALSE)
		show_results = false;
	else
		show_results = true;
	redraw();
}

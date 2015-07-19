
// GeometryViewerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GeometryViewer.h"
#include "GeometryViewerDlg.h"
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


// CGeometryViewerDlg 对话框



CGeometryViewerDlg::CGeometryViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGeometryViewerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGeometryViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGeometryViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_POLYGON, &CGeometryViewerDlg::OnBnClickedButtonOpenPolygon)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_MESH, &CGeometryViewerDlg::OnBnClickedButtonOpenMesh)
END_MESSAGE_MAP()


// CGeometryViewerDlg 消息处理程序

BOOL CGeometryViewerDlg::OnInitDialog()
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

	GetDlgItem(IDC_BUTTON_OPEN_MESH)->EnableWindow(false);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGeometryViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGeometryViewerDlg::OnPaint()
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

		//创建一个内存中的图像
		int width = 770;
		int height = 540;

		CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
		CPen *pOldPen = dc.SelectObject(&pen);

		CBrush brush1(RGB(100, 100, 255));
		CBrush *pOldBrush = dc.SelectObject(&brush1);
		if (polygon_points.size() > 0)
		{
			CPoint *pts = new CPoint[polygon_points.size()];
			for (int i = 0; i < polygon_points.size(); i++)
			{
				double x = scale_dim_x(polygon_points[i].x);
				double y = scale_dim_y(polygon_points[i].y);
				pts[i] = CPoint(x, y);
			}
			dc.Polygon(pts, polygon_points.size());
			delete[] pts;
		}
		dc.SelectObject(pOldBrush);

		CBrush brush2(RGB(100, 255, 100));
		pOldBrush = dc.SelectObject(&brush2);
		for (int i = 0; i < polygon_points.size(); i++)
		{
			double x = scale_dim_x(polygon_points[i].x);
			double y = scale_dim_y(polygon_points[i].y);
			dc.Ellipse(x - 3, y - 3, x + 3, y + 3);
		}
		dc.SelectObject(pOldBrush);
		dc.SelectObject(pOldPen);

		LOGBRUSH logBrush;
		logBrush.lbStyle = BS_SOLID;
		logBrush.lbColor = RGB(255, 255, 255);
		CPen dash_pen(PS_DASH | PS_GEOMETRIC | PS_ENDCAP_ROUND, 2, &logBrush);
		pOldPen = dc.SelectObject(&dash_pen);
		for (int i = 0; i < mesh_triangles.size(); i++)
		{
			int ti = mesh_triangles[i].i;
			int tj = mesh_triangles[i].j;
			int tk = mesh_triangles[i].k;

			double xi = scale_dim_x(polygon_points[ti].x);
			double yi = scale_dim_y(polygon_points[ti].y);
			double xj = scale_dim_x(polygon_points[tj].x);
			double yj = scale_dim_y(polygon_points[tj].y);
			double xk = scale_dim_x(polygon_points[tk].x);
			double yk = scale_dim_y(polygon_points[tk].y);

			CPoint tri_pts[4] = { CPoint(xi, yi), CPoint(xj, yj), CPoint(xk, yk), CPoint(xi, yi) };
			dc.Polyline(tri_pts, 4);
		}
		dc.SelectObject(pOldPen);
	}
}

double CGeometryViewerDlg::scale_dim_x(double v)
{
	return 135 + 500 * (v - polygon_dim_min) / (polygon_dim_max - polygon_dim_min);
}

double CGeometryViewerDlg::scale_dim_y(double v)
{
	return 20 + 500 * (v - polygon_dim_min) / (polygon_dim_max - polygon_dim_min);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGeometryViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGeometryViewerDlg::OnBnClickedButtonOpenPolygon()
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
		std::ifstream input(dlg.GetPathName(), std::ifstream::in);
		std::string line;
		double max = 0, min = 9999;
		while (std::getline(input, line))
		{
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
		Invalidate();

		GetDlgItem(IDC_BUTTON_OPEN_MESH)->EnableWindow(true);
	}
	else
		return;
}


void CGeometryViewerDlg::OnBnClickedButtonOpenMesh()
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
		mesh_triangles.clear();
		std::ifstream input(dlg.GetPathName(), std::ifstream::in);
		std::string line;
		while (std::getline(input, line))
		{
			std::istringstream iss(line);
			int i, j, k;
			iss >> i >> j >> k;
			mesh_triangles.push_back(triangle(i, j, k));
		}
		Invalidate();
	}
	else
		return;
}

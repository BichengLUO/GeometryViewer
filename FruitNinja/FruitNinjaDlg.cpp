
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
	first_run = TRUE;
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
		Pen pen_thick(Color::Black, 2);
		Pen dash_pen_gray(Color::Gray, 2);
		dash_pen_gray.SetDashStyle(DashStyleDash);
		SolidBrush brush_red(Color::Red);
		SolidBrush brush_black(Color::Black);
		SolidBrush brush_gray(Color::Gray);
		LinearGradientBrush brush_aqua(Rect(0, 0, rect.Width(), rect.Height()), Color::Aqua, Color::Blue, LinearGradientModeHorizontal);
		SolidBrush brush_white(Color::White);
		SolidBrush brush_white_alpha(Color::MakeARGB(200, 255, 255, 255));

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
		for (auto &seg : sgmts)
		{
			int x = seg.x;
			int y = seg.y;
			int len = seg.len;
			if (len > 0)
			{
				pMemGraphics->DrawLine(&pen_thick, x, y, x, y + len);
				pMemGraphics->FillEllipse(&brush_black, x - 4, y + len - 4, 8, 8);
				pMemGraphics->FillEllipse(&brush_black, x - 4, y - 4, 8, 8);
			}
			else
			{
				pMemGraphics->DrawLine(&dash_pen_gray, x, y, x, mouse_pos.y);
				pMemGraphics->FillEllipse(&brush_gray, x - 4, mouse_pos.y, 8, 8);
				pMemGraphics->FillEllipse(&brush_gray, x - 4, y - 4, 8, 8);
			}
		}
		draw_string(pMemGraphics, L"Dual Graph", rect.Width() - 305, rect.Height() - 275, 300, 20);
		pMemGraphics->FillRectangle(&brush_white_alpha, rect.Width() - 305, rect.Height() - 255, 300, 200);
		pMemGraphics->DrawRectangle(&pen, rect.Width() - 305, rect.Height() - 255, 300, 200);
		graphics.DrawImage(&pMemBitmap, 0, 0);
	}
}

void CFruitNinjaDlg::draw_string(Graphics* pMemGraphics, TCHAR *str, int x, int y, int width, int height)
{
	// Initialize arguments.
	Gdiplus::Font myFont(L"Arial", 12);
	StringFormat format;
	SolidBrush blackBrush(Color(255, 0, 0, 0));

	// Draw string.
	pMemGraphics->DrawString(
		str,
		11,
		&myFont,
		RectF(x, y, width, height),
		&format,
		&blackBrush);
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
	if (sgmts.size() > 0)
	{
		auto last_seg = sgmts.end() - 1;
		if (last_seg->len == 0 && point.y != last_seg->y)
		{
			last_seg->len = abs(point.y - last_seg->y);
			if (point.y < last_seg->y)
				last_seg->y = point.y;
		}
		else
			sgmts.push_back(segment(point.x, point.y, 0));
	}
	else
		sgmts.push_back(segment(point.x, point.y, 0));
	redraw();
	CDialogEx::OnLButtonDown(nFlags, point);
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
	if (sgmts.size() > 0)
	{
		auto last_seg = sgmts.end() - 1;
		if (last_seg->len == 0)
			redraw();
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CFruitNinjaDlg::OnBnClickedButtonClear()
{
	// TODO:  在此添加控件通知处理程序代码
	sgmts.clear();
	redraw();
}

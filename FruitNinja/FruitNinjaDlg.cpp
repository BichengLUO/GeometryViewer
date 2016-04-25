
// FruitNinjaDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FruitNinja.h"
#include "FruitNinjaDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CFruitNinjaDlg �Ի���



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


// CFruitNinjaDlg ��Ϣ��������

BOOL CFruitNinjaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵������ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ����Ӷ���ĳ�ʼ������
	first_run = TRUE;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի���������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFruitNinjaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CRect rect;
		GetClientRect(&rect);
		Pen pen(Color::Black, 2);
		Pen dash_pen_gray(Color::Gray, 2);
		dash_pen_gray.SetDashStyle(DashStyleDash);
		SolidBrush brush_red(Color::Red);
		SolidBrush brush_black(Color::Black);
		SolidBrush brush_gray(Color::Gray);
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
		for (auto &seg : sgmts)
		{
			int x = seg.x;
			int y = seg.y;
			int len = seg.len;
			if (len > 0)
			{
				pMemGraphics->DrawLine(&pen, x, y, x, y + len);
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
		graphics.DrawImage(&pMemBitmap, 0, 0);
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFruitNinjaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CFruitNinjaDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  �ڴ�������Ϣ������������/�����Ĭ��ֵ
	return TRUE;
}


void CFruitNinjaDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ�������Ϣ������������/�����Ĭ��ֵ
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
	// TODO:  �ڴ�������Ϣ������������/�����Ĭ��ֵ
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
	// TODO:  �ڴ����ӿؼ�֪ͨ�����������
	sgmts.clear();
	redraw();
}
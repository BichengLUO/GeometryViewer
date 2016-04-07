
// StarsViewerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "StarsViewer.h"
#include "StarsViewerDlg.h"
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


// CStarsViewerDlg �Ի���



CStarsViewerDlg::CStarsViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStarsViewerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStarsViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_SHOW_RESULTS, show_results_btn);
	DDX_Control(pDX, IDC_LIST_QUERIES, queries_list_box);
}

BEGIN_MESSAGE_MAP(CStarsViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_STARS_QUERIES, &CStarsViewerDlg::OnBnClickedButtonOpenStarsQueries)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_RESULTS, &CStarsViewerDlg::OnBnClickedButtonOpenResults)
	ON_BN_CLICKED(IDC_CHECK_SHOW_RESULTS, &CStarsViewerDlg::OnBnClickedCheckShowResults)
	ON_BN_CLICKED(IDC_BUTTON_RESET_VIEW, &CStarsViewerDlg::OnBnClickedButtonResetView)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_LBN_SELCHANGE(IDC_LIST_QUERIES, &CStarsViewerDlg::OnLbnSelchangeListQueries)
END_MESSAGE_MAP()


// CStarsViewerDlg ��Ϣ�������

BOOL CStarsViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	GetDlgItem(IDC_BUTTON_OPEN_RESULTS)->EnableWindow(false);
	show_results_btn.EnableWindow(false);
	show_results_btn.SetCheck(1);
	show_results = true;
	scale = 1.0;
	transform = point();
	drag_start_transform = transform;
	dragging = false;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CStarsViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CStarsViewerDlg::OnPaint()
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
		//����һ���ڴ��е�ͼ��
		TCHAR buffer[100];
		SetBkMode(memDC, TRANSPARENT);

		CPen *pOldPen;
		CBrush *pOldBrush;

		int selected = queries_list_box.GetCurSel();
		if (selected != -1)
		{
			double x1 = scale_dim_x(segments[selected].x1);
			double y1 = scale_dim_y(segments[selected].y1);
			double x2 = scale_dim_x(segments[selected].x2);
			double y2 = scale_dim_y(segments[selected].y2);

			CPen pen3(PS_DASH, 1, RGB(100, 100, 100));
			pOldPen = memDC.SelectObject(&pen3);
			CBrush brush3(RGB(200, 200, 255));
			pOldBrush = memDC.SelectObject(&brush3);

			POINT pts[4];
			pts[0].x = x1;
			pts[0].y = y1;
			pts[1].x = x2;
			pts[1].y = y2;
			pts[2].x = x2;
			pts[2].y = -10;
			pts[3].x = x1;
			pts[3].y = -10;
			memDC.Polygon(pts, 4);

			memDC.SelectObject(pOldPen);
			memDC.SelectObject(pOldBrush);

			CPen pen2(PS_SOLID, 2, RGB(50, 50, 50));
			pOldPen = memDC.SelectObject(&pen2);
			CBrush brush2(RGB(100, 255, 100));
			pOldBrush = memDC.SelectObject(&brush2);
			memDC.MoveTo(x1, y1);
			memDC.LineTo(x2, y2);
			memDC.Ellipse(x1 - 5, y1 - 5, x1 + 5, y1 + 5);
			memDC.Ellipse(x2 - 5, y2 - 5, x2 + 5, y2 + 5);
			memDC.SelectObject(pOldPen);
			memDC.SelectObject(pOldBrush);
		}

		CPen pen1(PS_SOLID, 1, RGB(0, 0, 0));
		pOldPen = memDC.SelectObject(&pen1);
		CBrush brush1(RGB(255, 100, 100));
		pOldBrush = memDC.SelectObject(&brush1);
		for (int i = 0; i < stars.size(); i++)
		{
			double x = scale_dim_x(stars[i].x);
			double y = scale_dim_y(stars[i].y);
			memDC.Ellipse(x - 3, y - 3, x + 3, y + 3);
		}
		memDC.SelectObject(pOldPen);
		memDC.SelectObject(pOldBrush);

		dc.BitBlt(r.left, r.top, r.right, r.bottom, &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pOldBmp);
		memDC.DeleteDC();
		memBitmap.DeleteObject();
		DeleteDC(memDC);
	}
}

double CStarsViewerDlg::scale_dim_x(double v)
{
	return transform.x + 135 - 250 * (scale - 1.0) + 500 * scale * (v - dim_min) / (dim_max - dim_min);
}

double CStarsViewerDlg::scale_dim_y(double v)
{
	return transform.y + 20 - 250 * (scale - 1.0) + 500 * scale * (v - dim_min) / (dim_max - dim_min);
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CStarsViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CStarsViewerDlg::OnBnClickedButtonOpenStarsQueries()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(TRUE, //TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		stars.clear();
		segments.clear();
		queries_result.clear();
		queries_list_box.ResetContent();
		std::ifstream input(dlg.GetPathName(), std::ifstream::in);
		std::string line;
		double max = -9999, min = 9999;
		std::getline(input, line);
		int n_stars, n_queries;
		std::istringstream iss(line);
		iss >> n_stars >> n_queries;
		for (int i = 0; i < n_stars; i++)
		{
			std::getline(input, line);
			std::istringstream iss1(line);
			double x, y;
			iss1 >> x >> y;
			stars.push_back(point(x, y));
			max = MAX2(x, y) > max ? MAX2(x, y) : max;
			min = MIN2(x, y) < min ? MIN2(x, y) : min;
		}
		for (int i = 0; i < n_queries; i++)
		{
			std::getline(input, line);
			std::istringstream iss2(line);
			double x1, y1, x2, y2;
			iss2 >> x1 >> y1 >> x2 >> y2;
			segments.push_back(query(x1, y1, x2, y2));
			max = MAX4(x1, y1, x2, y2) > max ? MAX4(x1, y1, x2, y2) : max;
			min = MIN4(x1, y1, x2, y2) < min ? MIN4(x1, y1, x2, y2) : min;
			TCHAR label[100];
			swprintf(label, _T("[%2d] (%d, %d), (%d, %d)"), i + 1, (int)x1, (int)y1, (int)x2, (int)y2);
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


void CStarsViewerDlg::OnBnClickedButtonOpenResults()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CFileDialog dlg(TRUE, //TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
	}
	else
		return;
}


void CStarsViewerDlg::OnBnClickedCheckShowResults()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (show_results_btn.GetCheck() == 0)
		show_results = false;
	else
		show_results = true;
	redraw();
}


void CStarsViewerDlg::OnBnClickedButtonResetView()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	scale = 1.0;
	transform = point();
	drag_start_transform = point();
	redraw();
}


BOOL CStarsViewerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
}


void CStarsViewerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	dragging = true;
	drag_start_pos = point;
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CStarsViewerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	dragging = false;
	drag_start_transform = transform;
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CStarsViewerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
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


BOOL CStarsViewerDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	scale *= 1 + zDelta / 2000.0;
	if (scale <= 0) scale = 0;
	redraw();
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CStarsViewerDlg::redraw()
{
	CRect r;
	GetClientRect(&r);
	r.bottom -= 50;
	r.right -= 250;
	InvalidateRect(r);
}


void CStarsViewerDlg::OnLbnSelchangeListQueries()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	redraw();
}

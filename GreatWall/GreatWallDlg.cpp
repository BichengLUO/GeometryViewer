
// GreatWallDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GreatWall.h"
#include "GreatWallDlg.h"
#include "afxdialogex.h"
#include <stack>
#include <fstream>
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define RANGE 100000


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


// CGreatWallDlg 对话框



CGreatWallDlg::CGreatWallDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGreatWallDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGreatWallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGreatWallDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK_SHOW_GUARDIANS, &CGreatWallDlg::OnBnClickedCheckShowGuardians)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CGreatWallDlg::OnBnClickedButtonClear)
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_CHECK_SHOW_UPPER_HULL, &CGreatWallDlg::OnBnClickedCheckShowUpperHull)
	ON_BN_CLICKED(IDC_BUTTON_UNDO, &CGreatWallDlg::OnBnClickedButtonUndo)
	ON_BN_CLICKED(IDC_CHECK_SHOW_COORDINATES, &CGreatWallDlg::OnBnClickedCheckShowCoordinates)
	ON_BN_CLICKED(IDC_BUTTON_GENERATE, &CGreatWallDlg::OnBnClickedButtonGenerate)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, &CGreatWallDlg::OnBnClickedButtonImport)
END_MESSAGE_MAP()


// CGreatWallDlg 消息处理程序

BOOL CGreatWallDlg::OnInitDialog()
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
	srand((unsigned)time(NULL));
	show_guardians = FALSE;
	show_upper_hull = FALSE;
	show_coordinates = FALSE;
	first_run = TRUE;
	((CButton*)GetDlgItem(IDC_RADIO_RANDOM_MODE))->SetCheck(TRUE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGreatWallDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGreatWallDlg::OnPaint()
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
		Pen dash_pen_red(Color::Red, 3);
		dash_pen_red.SetDashStyle(DashStyleDash);
		Pen dash_pen_orange(Color::Orange, 3);
		dash_pen_orange.SetDashStyle(DashStyleDash);
		SolidBrush brush_red(Color::Red);
		SolidBrush brush_black(Color::Black);
		LinearGradientBrush brush_aqua(Rect(0, 0, rect.Width(), rect.Height()), Color::Aqua, Color::Blue, LinearGradientModeHorizontal);
		SolidBrush brush_white(Color::White);
		SolidBrush brush_white_alpha(Color::MakeARGB(200, 255, 255, 255));

		Bitmap pMemBitmap(rect.Width() - 300, rect.Height() - 50);
		Graphics* pMemGraphics = Graphics::FromImage(&pMemBitmap);
		pMemGraphics->SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);
		pMemGraphics->FillRectangle(&brush_white, 0, 0, rect.Width() - 300, rect.Height() - 50);

		CPaintDC dc(this);
		Graphics graphics(dc.m_hDC);

		if (first_run)
		{
			first_run = FALSE;
			return;
		}
		WCHAR count_title[128];
		wsprintf(count_title, L"Size of Points = %d", pts.size());
		draw_string(pMemGraphics, count_title, 5, 5, 500, 20, &brush_black);
		if (pts.size() > 0)
		{
			if (pts.size() > 1)
			{
				Point *array = new Point[pts.size() + 2];
				for (int i = 0; i < pts.size(); i++)
					array[i] = Point(pts[i].x, rect.Height() - pts[i].y);
				array[pts.size()] = Point(array[pts.size() - 1].X, 800);
				array[pts.size() + 1] = Point(array[0].X, 800);
				pMemGraphics->FillPolygon(&brush_aqua, array, pts.size() + 2);
				delete[] array;
			}
			if (show_coordinates)
			{
				for (int i = 0; i < pts.size(); i++)
				{
					WCHAR pt_title[128];
					wsprintf(pt_title, L"(%d,%d)", pts[i].x, pts[i].y);
					pMemGraphics->DrawRectangle(&pen, pts[i].x - 25, rect.Height() - pts[i].y + 50, 70, 20);
					pMemGraphics->FillRectangle(&brush_white_alpha, pts[i].x - 25, rect.Height() - pts[i].y + 50, 70, 20);
					draw_string(pMemGraphics, pt_title, pts[i].x - 20, rect.Height() - pts[i].y + 50, 70, 20, &brush_black);
				}
			}
			if (show_upper_hull)
			{
				for (int i = 0; i < hull.size() - 1; i++)
				{
					int x = pts[hull[i]].x;
					int y = pts[hull[i]].y;
					int nx = pts[hull[i + 1]].x;
					int ny = pts[hull[i + 1]].y;

					pMemGraphics->DrawLine(&dash_pen_red, x, rect.Height() - y, nx, rect.Height() - ny);
				}
				if (hull.size() > 1)
				{
					int lx = pts[hull.end()[-2]].x;
					int ly = pts[hull.end()[-2]].y;
					int x = pts[hull.end()[-1]].x;
					int y = pts[hull.end()[-1]].y;
					double len = sqrt((x - lx)*(x - lx) + (y - ly)*(y - ly));
					int nx = x + 60 * (x - lx) / len;
					int ny = y + 60 * (y - ly) / len;

					pMemGraphics->DrawLine(&dash_pen_orange, x, rect.Height() - y, nx, rect.Height() - ny);
				}
			}

			double x = pts[0].x;
			double y = pts[0].y;
			pMemGraphics->FillEllipse(&brush_red, x - 5, rect.Height() - y - 5, 10, 10);
			for (int i = 1; i < pts.size(); i++)
			{
				int x = pts[i].x;
				int y = pts[i].y;

				int nx = pts[i - 1].x;
				int ny = pts[i - 1].y;

				pMemGraphics->FillEllipse(&brush_black, x - 3, rect.Height() - y - 3, 6, 6);
				pMemGraphics->DrawLine(&pen, x, rect.Height() - y, nx, rect.Height() - ny);
			}

			if (show_guardians)
			{
				for (int i = 0; i < gds.size(); i++)
				{
					int x = pts[gds[i]].x;
					int y = pts[gds[i]].y;

					pMemGraphics->FillEllipse(&brush_white, x - 5, rect.Height() - y - 5, 10, 10);
					pMemGraphics->DrawEllipse(&pen, x - 5, rect.Height() - y - 5, 10, 10);
				}
				WCHAR gds_title[128];
				wsprintf(gds_title, L"Minimium Size of Guardians = %d", gds.size());
				draw_string(pMemGraphics, gds_title, 5, 25, 500, 20, &brush_black);
			}
		}
		delete pMemGraphics;
		graphics.DrawImage(&pMemBitmap, 0, 0);
	}
}

void CGreatWallDlg::draw_string(Graphics* pMemGraphics, TCHAR *str, int x, int y, int width, int height, Brush *brush)
{
	// Initialize arguments.
	Gdiplus::Font myFont(L"Arial", 10);
	StringFormat format;

	// Draw string.
	pMemGraphics->DrawString(
		str,
		wcslen(str),
		&myFont,
		RectF(x, y, width, height),
		&format,
		brush);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGreatWallDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGreatWallDlg::redraw()
{
	CRect rect;
	GetClientRect(&rect);
	rect.right -= 300;
	rect.bottom -= 50;
	InvalidateRect(rect);
}

void CGreatWallDlg::OnBnClickedCheckShowGuardians()
{
	// TODO:  在此添加控件通知处理程序代码
	if (((CButton*)GetDlgItem(IDC_CHECK_SHOW_GUARDIANS))->GetCheck())
		show_guardians = TRUE;
	else
		show_guardians = FALSE;
	redraw();
}


void CGreatWallDlg::OnBnClickedButtonClear()
{
	// TODO:  在此添加控件通知处理程序代码
	pts.clear();
	gds.clear();

	redraw();
}


void CGreatWallDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetClientRect(&rect);

	CRect rect2 = rect;
	rect2.right -= 300;
	rect2.bottom -= 50;

	if (rect2.PtInRect(point) && (pts.size() == 0 || point.x < pts[pts.size() - 1].x))
	{
		pts.push_back(point2d(point.x, rect.Height() - point.y));
		generate_guardians();

		redraw();
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


BOOL CGreatWallDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	return TRUE;
}

void CGreatWallDlg::generate_guardians()
{
	hull.clear();
	gds.clear();
	if (pts.size() > 0)
	{
		hull.push_back(0);
		gds.push_back(0);
		for (int i = 1; i < pts.size() - 1; i++)
		{
			while (hull.size() > 1 && !to_left_on(pts[hull.end()[-2]], pts[hull.end()[-1]], pts[i]))
				hull.pop_back();
			hull.push_back(i);
			if (to_left(pts[hull.end()[-2]], pts[hull.end()[-1]], pts[i + 1]))
				gds.push_back(i);
		}
	}
}

bool CGreatWallDlg::to_left(point2d p1, point2d p2, point2d p3)
{
	long long a = p3.x - p1.x;
	long long b = p2.y - p1.y;
	long long c = p2.x - p1.x;
	long long d = p3.y - p1.y;
	return a * b - c * d < 0;
}

bool CGreatWallDlg::to_left_on(point2d p1, point2d p2, point2d p3)
{
	long long a = p3.x - p1.x;
	long long b = p2.y - p1.y;
	long long c = p2.x - p1.x;
	long long d = p3.y - p1.y;
	return a * b - c * d <= 0;
}


void CGreatWallDlg::OnBnClickedCheckShowUpperHull()
{
	// TODO:  在此添加控件通知处理程序代码
	if (((CButton*)GetDlgItem(IDC_CHECK_SHOW_UPPER_HULL))->GetCheck())
		show_upper_hull = TRUE;
	else
		show_upper_hull = FALSE;
	redraw();
}


void CGreatWallDlg::OnBnClickedButtonUndo()
{
	// TODO:  在此添加控件通知处理程序代码
	if (pts.size() > 0)
		pts.pop_back();
	generate_guardians();

	redraw();
}


void CGreatWallDlg::OnBnClickedCheckShowCoordinates()
{
	// TODO:  在此添加控件通知处理程序代码
	if (((CButton*)GetDlgItem(IDC_CHECK_SHOW_COORDINATES))->GetCheck())
		show_coordinates = TRUE;
	else
		show_coordinates = FALSE;
	redraw();
}


void CGreatWallDlg::OnBnClickedButtonGenerate()
{
	// TODO:  在此添加控件通知处理程序代码
	double i;
	CString number_str;
	GetDlgItem(IDC_EDIT_NUMBER)->GetWindowText(number_str);
	int number = _ttoi(number_str);

	CString count_formula;
	GetDlgItem(IDC_EDIT_COUNT_FORMULA)->GetWindowText(count_formula);
	CString name_formula;
	GetDlgItem(IDC_EDIT_NAME_FORMULA)->GetWindowText(name_formula);

	symbol_table_t symbol_table;
	symbol_table.add_variable("i", i);
	symbol_table.add_constants();

	expression_t exp_count;
	exp_count.register_symbol_table(symbol_table);

	expression_t exp_name;
	exp_name.register_symbol_table(symbol_table);

	parser_t parser;
	parser.compile(std::string(CT2CA(count_formula)), exp_count);
	parser.compile(std::string(CT2CA(name_formula)), exp_name);

	std::ostringstream oss;
	oss << "Generation done!\n";
	oss << "---------------------\n";
	for (i = 0; i < number; i += 1)
	{
		int count = exp_count.value();
		int name = exp_name.value();

		char input_name_str[128];
		sprintf(input_name_str, "%d.in", name);
		char output_name_str[128];
		sprintf(output_name_str, "%d.out", name);

		points input;
		if (((CButton*)GetDlgItem(IDC_RADIO_RANDOM_MODE))->GetCheck())
			input = random_input(count);
		else if (((CButton*)GetDlgItem(IDC_RADIO_CONVEX_MODE))->GetCheck())
			input = convex_input(count);
		else if (((CButton*)GetDlgItem(IDC_RADIO_CONCAVE_MODE))->GetCheck())
			input = concave_input(count);

		LARGE_INTEGER BeginTime;
		LARGE_INTEGER EndTime;
		LARGE_INTEGER Frequency;

		QueryPerformanceFrequency(&Frequency);
		QueryPerformanceCounter(&BeginTime);
		int output = generate_guardians_pts(input);
		QueryPerformanceCounter(&EndTime);
		float tm = (float)(EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart;
		oss << "(" << i + 1 << ") " << name << ".in [size: " << count << "] --> " << name << ".out [" << output << "] Time: " << tm << "s\n";

		std::ofstream input_file(input_name_str, std::ofstream::out);
		std::ofstream output_file(output_name_str, std::ofstream::out);

		input_file << count << std::endl;
		for (int j = count - 1; j >= 0; j--)
			input_file << (int)input[j].x << " " << (int)input[j].y << std::endl;
		output_file << output;
	}

	MessageBox(CA2CT(oss.str().c_str()), _T("Complete"));
}

int CGreatWallDlg::random_int(std::mt19937 &rng, int min, int max)
{
	std::uniform_int_distribution<int> uni(min + 1, max - 1); // guaranteed unbiased
	return uni(rng);
}

points CGreatWallDlg::random_input(int count)
{
	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	points input;
	int slot = 2 * RANGE / count;
	int x = random_int(rng, RANGE - slot, RANGE);
	int y = random_int(rng, -RANGE, RANGE);
	input.push_back(point2d(x, y));
	for (int i = 1; i < count; i++)
	{
		x = random_int(rng, x - slot, x);
		y = random_int(rng, -RANGE, RANGE);
		input.push_back(point2d(x, y));
	}
	return input;
}


points CGreatWallDlg::convex_input(int count)
{
	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	points input;
	int slot = 2 * RANGE / count;
	int x = random_int(rng, RANGE - slot, RANGE);
	int y = random_int(rng, -RANGE, RANGE);
	input.push_back(point2d(x, y));
	for (int i = 1; i < count; i++)
	{
		int nx = random_int(rng, x - slot, x);
		if (i > 1)
		{
			int lx = input.end()[-2].x;
			int ly = input.end()[-2].y;
			int ny_max = y - (x - nx) * ((ly - y) / (double)(lx - x));
			y = random_int(rng, ny_max - slot, ny_max);
		}
		else
			y = random_int(rng, y, y + slot);
		x = nx;
		input.push_back(point2d(x, y));
	}
	return input;
}


points CGreatWallDlg::concave_input(int count)
{
	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	points input;
	int slot = 2 * RANGE / count;
	int x = random_int(rng, RANGE - slot, RANGE);
	int y = random_int(rng, -RANGE, RANGE);
	input.push_back(point2d(x, y));
	for (int i = 1; i < count; i++)
	{
		int nx = random_int(rng, x - slot, x);
		if (i > 1)
		{
			int lx = input.end()[-2].x;
			int ly = input.end()[-2].y;
			int ny_min = y - (x - nx) * ((ly - y) / (double)(lx - x));
			y = random_int(rng, ny_min, ny_min + slot);
		}
		else
			y = random_int(rng, y - slot, y);
		x = nx;
		input.push_back(point2d(x, y));
	}
	return input;
}


int CGreatWallDlg::generate_guardians_pts(const points &ptsp)
{
	std::vector<int> hullp;
	guardians gdsp;
	if (ptsp.size() > 0)
	{
		hullp.push_back(0);
		gdsp.push_back(0);
		for (int i = 1; i < ptsp.size() - 1; i++)
		{
			while (hullp.size() > 1 && !to_left_on(ptsp[hullp.end()[-2]], ptsp[hullp.end()[-1]], ptsp[i]))
				hullp.pop_back();
			hullp.push_back(i);
			if (to_left(ptsp[hullp.end()[-2]], ptsp[hullp.end()[-1]], ptsp[i + 1]))
				gdsp.push_back(i);
		}
	}
	return gdsp.size() - 1;
}


void CGreatWallDlg::OnBnClickedButtonImport()
{
	// TODO:  在此添加控件通知处理程序代码
	CRect rect;
	GetClientRect(&rect);
	CFileDialog dlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"),
		NULL);
	if (dlg.DoModal() == IDOK)
	{
		
		pts.clear();
		gds.clear();
		std::ifstream input(dlg.GetPathName(), std::ifstream::in);
		int count;
		input >> count;
		int min_x = 9999999, max_x = -9999999;
		int min_y = 9999999, max_y = -9999999;
		for (int i = 0; i < count; i++)
		{
			int x, y;
			input >> x >> y;
			pts.push_back(point2d(x, y));

			min_x = min(x, min_x);
			max_x = max(x, max_x);
			min_y = min(y, min_y);
			max_y = max(y, max_y);
		}
		std::reverse(std::begin(pts), std::end(pts));
		generate_guardians();
		for (int i = 0; i < count; i++)
		{
			pts[i].x = ((pts[i].x - min_x) / (double)(max_x - min_x)) * (rect.Width() - 340) + 20;
			pts[i].y = ((pts[i].y - min_y) / (double)(max_y - min_y)) * (rect.Height() - 90) + 70;
		}
		redraw();
	}
	else return;
}

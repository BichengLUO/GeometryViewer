
// GeometryViewerDlg.h : 头文件
//

#pragma once

#include <vector>

typedef struct _point
{
	double x;
	double y;
	_point() : x(0.0), y(0.0) {}
	_point(double xp, double yp) : x(xp), y(yp){}
} point;

typedef std::vector<point> points;

typedef struct _triangle
{
	int i;
	int j;
	int k;
	_triangle() : i(-1), j(-1), k(-1) {}
	_triangle(int ip, int jp, int kp) : i(ip), j(jp), k(kp) {}
} triangle;

typedef std::vector<triangle> triangles;

// CGeometryViewerDlg 对话框
class CGeometryViewerDlg : public CDialogEx
{
// 构造
public:
	CGeometryViewerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GEOMETRYVIEWER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	points polygon_points;
	double polygon_dim_min;
	double polygon_dim_max;
	triangles mesh_triangles;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	double scale_dim_x(double v);
	double scale_dim_y(double v);
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpenPolygon();
	afx_msg void OnBnClickedButtonOpenMesh();
};
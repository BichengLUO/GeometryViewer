
// GreatWallDlg.h : 头文件
//

#pragma once
#include <vector>

typedef struct _point2d
{
	double x;
	double y;
	_point2d(double xp, double yp) : x(xp), y(yp){}
} point2d;

typedef std::vector<point2d> points;


// CGreatWallDlg 对话框
class CGreatWallDlg : public CDialogEx
{
// 构造
public:
	CGreatWallDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GREATWALL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	points pts;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckShowGuardians();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


// GreatWallDlg.h : ͷ�ļ�
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


// CGreatWallDlg �Ի���
class CGreatWallDlg : public CDialogEx
{
// ����
public:
	CGreatWallDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GREATWALL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	points pts;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

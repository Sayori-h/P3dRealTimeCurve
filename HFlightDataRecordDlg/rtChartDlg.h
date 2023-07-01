
// rtChartDlg.h: 头文件
//
#pragma once

#include "Pegrpapi.h"
// CrtChartDlg 对话框
class CrtChartDlg : public CDialogEx
{
// 构造
public:
	CrtChartDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RTCHART_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


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
	afx_msg void OnBnClickedOk();
	int secPointNum;
	CString strStateInfo;
	
	CRect speedRect;//图片控件区域
	CRect altitudeRect;
	CRect pitchRect;
	HWND m_hPE1,m_hPE2, m_hPE3;//绘制曲线的控件句柄

	afx_msg LRESULT OnUserMsgHdl(WPARAM wParam, LPARAM lParam);
	
	CStatic m_speedChart;
	CStatic m_altitudeChart;
	CStatic m_pitchChart;
};


// rtChartDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "rtChart.h"
#include "rtChartDlg.h"
#include "afxdialogex.h"

#include "rtData.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CrtChartDlg 对话框



CrtChartDlg::CrtChartDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RTCHART_DIALOG, pParent)
	, secPointNum(10)
	, strStateInfo(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CrtChartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PERSECNUM, secPointNum);
	DDX_Text(pDX, IDC_STATEINFO, strStateInfo);
	DDX_Control(pDX, IDC_SPEEDCHART, m_speedChart);
	DDX_Control(pDX, IDC_ALTITUDCHART, m_altitudeChart);
	DDX_Control(pDX, IDC_PITCHCHART, m_pitchChart);
}

BEGIN_MESSAGE_MAP(CrtChartDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_USER_MSG_HDL, OnUserMsgHdl)
	ON_BN_CLICKED(IDOK, &CrtChartDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CrtChartDlg 消息处理程序

BOOL CrtChartDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// TODO: 在此添加额外的初始化代码
	if (m_hPE1||m_hPE2|| m_hPE3)
	{
		if(m_hPE1)
			PEdestroy(m_hPE1);
		if (m_hPE2)
			PEdestroy(m_hPE2);
		if(m_hPE3)
			PEdestroy(m_hPE3);
		Invalidate();
	}
	m_speedChart.GetClientRect(&speedRect);
	m_altitudeChart.GetClientRect(&altitudeRect);
	m_pitchChart.GetClientRect(&pitchRect);
	m_hPE1 = PEcreate(PECONTROL_GRAPH, WS_VISIBLE, &speedRect, m_speedChart.m_hWnd, 1001);//PECONTROL_SGRAPH 是科学图，PECONTROL_GRAPH是简单图
	m_hPE2 = PEcreate(PECONTROL_GRAPH, WS_VISIBLE, &altitudeRect, m_altitudeChart.m_hWnd, 1001);
	m_hPE3 = PEcreate(PECONTROL_GRAPH, WS_VISIBLE, &pitchRect, m_pitchChart.m_hWnd, 1001);
	
	 // No Shadows //
	PEnset(m_hPE1, PEP_nDATASHADOWS, PEDS_NONE);
	PEnset(m_hPE2, PEP_nDATASHADOWS, PEDS_NONE);
	PEnset(m_hPE3, PEP_nDATASHADOWS, PEDS_NONE);
		// No Flicker //
	PEnset(m_hPE1, PEP_bPREPAREIMAGES, TRUE);
	PEnset(m_hPE1, PEP_bCACHEBMP, TRUE);
	PEnset(m_hPE2, PEP_bPREPAREIMAGES, TRUE);
	PEnset(m_hPE2, PEP_bCACHEBMP, TRUE);
	PEnset(m_hPE3, PEP_bPREPAREIMAGES, TRUE);
	PEnset(m_hPE3, PEP_bCACHEBMP, TRUE);

	PEnset(m_hPE1, PEP_nSUBSETS, 1);	//设置曲线条数
	PEnset(m_hPE1, PEP_nPOINTS, 15);
	PEnset(m_hPE2, PEP_nSUBSETS, 1);	//设置曲线条数
	PEnset(m_hPE2, PEP_nPOINTS, 15);
	PEnset(m_hPE3, PEP_nSUBSETS, 1);	//设置曲线条数
	PEnset(m_hPE3, PEP_nPOINTS, 15);

	PEnset(m_hPE1, PEP_nPOINTSTOGRAPH, 1);  // set to lower number to show earliest 
	PEnset(m_hPE1, PEP_nPOINTSTOGRAPHINIT, PEPTGI_LASTPOINTS);
	PEnset(m_hPE1, PEP_nPLOTTINGMETHOD, PEGPM_POINTSPLUSLINE);

	PEnset(m_hPE2, PEP_nPOINTSTOGRAPH, 1);  // set to lower number to show earliest 
	PEnset(m_hPE2, PEP_nPOINTSTOGRAPHINIT, PEPTGI_LASTPOINTS);
	PEnset(m_hPE2, PEP_nPLOTTINGMETHOD, PEGPM_POINTSPLUSLINE);

	PEnset(m_hPE3, PEP_nPOINTSTOGRAPH, 1);  // set to lower number to show earliest 
	PEnset(m_hPE3, PEP_nPOINTSTOGRAPHINIT, PEPTGI_LASTPOINTS);
	PEnset(m_hPE3, PEP_nPLOTTINGMETHOD, PEGPM_POINTSPLUSLINE);

	int nPointTypes[] = { PEPT_DOTSOLID, PEPT_PLUS };
	PEvset(m_hPE1, PEP_naSUBSETPOINTTYPES, nPointTypes, 1);
	PEvset(m_hPE2, PEP_naSUBSETPOINTTYPES, nPointTypes, 1);
	PEvset(m_hPE3, PEP_naSUBSETPOINTTYPES, nPointTypes, 1);

	int nTmpStyles[1];
	nTmpStyles[0] = PELT_MEDIUMTHINSOLID;
	 //设置颜色
	DWORD dwSubsetColors[1];
	dwSubsetColors[0] = RGB(255, 0, 0); // First Subset	
	PEvset(m_hPE2, PEP_dwaSUBSETCOLORS, dwSubsetColors, 1);


	PEvset(m_hPE1, PEP_naSUBSETLINETYPES, nTmpStyles, 1);//
	PEnset(m_hPE1, PEP_nPOINTSIZE, PEPS_SMALL);

	PEvset(m_hPE2, PEP_naSUBSETLINETYPES, nTmpStyles, 1);//
	PEnset(m_hPE2, PEP_nPOINTSIZE, PEPS_SMALL);

	PEvset(m_hPE3, PEP_naSUBSETLINETYPES, nTmpStyles, 1);//
	PEnset(m_hPE3, PEP_nPOINTSIZE, PEPS_SMALL);
	// Manually configure scales //
	PEnset(m_hPE1, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
	PEnset(m_hPE2, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);
	PEnset(m_hPE3, PEP_nMANUALSCALECONTROLY, PEMSC_MINMAX);

	double manminY = 1.0F;
	PEvset(m_hPE1, PEP_fMANUALMINY, &manminY, 1);
	double manmaxY = 360.0F;
	PEvset(m_hPE1, PEP_fMANUALMAXY, &manmaxY, 1);

	manminY = 1.0F;
	PEvset(m_hPE2, PEP_fMANUALMINY, &manminY, 1);
	manmaxY = 15000.0F;
	PEvset(m_hPE2, PEP_fMANUALMAXY, &manmaxY, 1);

	manminY = -2.5F;
	PEvset(m_hPE3, PEP_fMANUALMINY, &manminY, 1);
	manmaxY = 2.5F;
	PEvset(m_hPE3, PEP_fMANUALMAXY, &manmaxY, 1);
	// Needed to allocate point labels so append logic works //
	// Set last point label, Points - 1 //
	PEszset(m_hPE1, PEP_szaPOINTLABELS, TEXT(""));
	PEszset(m_hPE2, PEP_szaPOINTLABELS, TEXT(""));
	PEszset(m_hPE3, PEP_szaPOINTLABELS, TEXT(""));

	float f1 = 0.0F;
	PEvsetcellEx(m_hPE1, PEP_faYDATA, 0, 0, &f1);
	PEvsetcellEx(m_hPE1, PEP_faYDATA, 0, 1, &f1);
	PEvsetcellEx(m_hPE1, PEP_faYDATA, 0, 2, &f1);
	PEvsetcellEx(m_hPE1, PEP_faYDATA, 0, 3, &f1);

	PEvsetcellEx(m_hPE2, PEP_faYDATA, 0, 0, &f1);
	PEvsetcellEx(m_hPE2, PEP_faYDATA, 0, 1, &f1);
	PEvsetcellEx(m_hPE2, PEP_faYDATA, 0, 2, &f1);
	PEvsetcellEx(m_hPE2, PEP_faYDATA, 0, 3, &f1);

	PEvsetcellEx(m_hPE3, PEP_faYDATA, 0, 0, &f1);
	PEvsetcellEx(m_hPE3, PEP_faYDATA, 0, 1, &f1);
	PEvsetcellEx(m_hPE3, PEP_faYDATA, 0, 2, &f1);
	PEvsetcellEx(m_hPE3, PEP_faYDATA, 0, 3, &f1);
	/*PEnset(m_hPE1, PEP_bFIXEDFONTS, TRUE);
	PEnset(m_hPE2, PEP_bFIXEDFONTS, TRUE);*/
	PEnset(m_hPE1, PEP_nFONTSIZE, PEFS_SMALL);
	PEnset(m_hPE2, PEP_nFONTSIZE, PEFS_SMALL);
	PEnset(m_hPE3, PEP_nFONTSIZE, PEFS_SMALL);

	PEszset(m_hPE1, PEP_szMAINTITLE, _T(" AIRSPEED TRUE—SIM TIME"));
	PEszset(m_hPE2, PEP_szMAINTITLE, _T(" PLANE ALTITUDE—SIM TIME"));
	PEszset(m_hPE3, PEP_szMAINTITLE, _T(" PLANE PITCH DEGREES—SIM TIME"));
	PEszset(m_hPE1, PEP_szSUBTITLE, _T(" "));
	PEszset(m_hPE2, PEP_szSUBTITLE, _T(" "));
	PEszset(m_hPE3, PEP_szSUBTITLE, _T(" "));

	PEszset(m_hPE1, PEP_szYAXISLABEL, _T(" AIRSPEED TRUE(Knots)"));
	PEszset(m_hPE1, PEP_szXAXISLABEL, _T("SIM TIME(seconds)"));

	
	PEszset(m_hPE2, PEP_szYAXISLABEL, _T(" PLANE ALTITUDE (Feet)"));
	PEszset(m_hPE2, PEP_szXAXISLABEL, _T("SIM TIME(seconds)"));

	PEszset(m_hPE3, PEP_szYAXISLABEL, _T(" PLANE PITCH DEGREES (Radians)"));
	PEszset(m_hPE3, PEP_szXAXISLABEL, _T("SIM TIME(seconds)"));
	/*SetTimer(1, 50, NULL);*/

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CrtChartDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CrtChartDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CrtChartDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT RunDataHarvester(LPVOID Param)
{

	CrtChartDlg* pDlg = (CrtChartDlg*)Param;


	HRESULT hr;
	bool bConnected = false;
	CString str;
	// Attempt to connect to Prepar3D.

	str = _T("Attempting to connect to Prepar3D...");
	pDlg->GetDlgItem(IDC_STATEINFO)->SetWindowText(str);
	for (unsigned int i = 0; i < 60; ++i)
	{
		if (SUCCEEDED(SimConnect_Open(&g_hSimConnect, TITLE_STRING, NULL, 0, 0, 0)))
		{
			bConnected = true;
			break;
		}
		str.Format(_T("Attempt %d"), i + 1);
		pDlg->GetDlgItem(IDC_STATEINFO)->SetWindowText(str);
		Sleep(1000);
	}

	if (bConnected)
	{
		setColour(4);
		str = _T("Connected to Prepar3D!");
		pDlg->GetDlgItem(IDC_STATEINFO)->SetWindowText(str);

		// Set up the data definition.
		for (unsigned int i = 0; i < ARRAYSIZE(g_aVariables); ++i)
		{
			const PropertyDefinition& prop = g_aVariables[i];
			hr = SimConnect_AddToDataDefinition(g_hSimConnect, DEFINITION_ID_USER_OBJECT, prop.pszName, prop.pszUnits, prop.eDataType);
		}

		// Add the menu item that will start the harvest.
		hr = SimConnect_MenuAddItem(g_hSimConnect, START_STRING, EVENT_ID_START_HARVEST, 0);

		// Application main loop.
		while (0 == g_bQuit)
		{
			SimConnect_CallDispatch(g_hSimConnect, MyDispatchProcRD, NULL);
			Sleep(1);
		}
		// Clean up.
		if (g_bIsHarvesting)
		{
			hr = SimConnect_RequestDataOnSimObject(g_hSimConnect, REQUEST_ID_USER_OBJECT_DATA, DEFINITION_ID_USER_OBJECT, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_NEVER);
			hr = SimConnect_MenuDeleteItem(g_hSimConnect, EVENT_ID_STOP_HARVEST);

			if (g_pFile)
			{
				fclose(g_pFile);
				g_pFile = NULL;
			}

		}
		else
		{
			hr = SimConnect_MenuDeleteItem(g_hSimConnect, EVENT_ID_START_HARVEST);
		}

		// Close.
		hr = SimConnect_Close(g_hSimConnect);
	}
	else
	{
		str = _T("Connection timeout!");
		pDlg->GetDlgItem(IDC_STATEINFO)->SetWindowText(str);
	}

	return 0;
}

void CrtChartDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	nCount1Sec = secPointNum;
	if (0 >= nCount1Sec)
	{
		return;
	}
	if (nCount1Sec > 6)
	{
		nCount1Sec += 3;
	}
	nTimeOutMS = (int)(1000. / nCount1Sec);
	HANDLE hMultiThread = AfxBeginThread(RunDataHarvester, /*(LPVOID)*/this, THREAD_PRIORITY_IDLE);

	CWnd* cSeBkBtnMy = GetDlgItem(IDOK);
	cSeBkBtnMy->EnableWindow(false);
}
LRESULT CrtChartDlg::OnUserMsgHdl(WPARAM wParam, LPARAM lParam)
{
	CString ttext;
	float fNew1,fNew2, fNew3;

	double* dataArray = (double*)lParam;

	PEnset(m_hPE1, PEP_bANTIALIASGRAPHICS, FALSE); // best for real time  
	PEnset(m_hPE2, PEP_bANTIALIASGRAPHICS, FALSE);
	PEnset(m_hPE3, PEP_bANTIALIASGRAPHICS, FALSE);
	
	ttext.Format(_T("%.1f"), dataArray[0]);
	PEvset(m_hPE1, PEP_szaAPPENDPOINTLABELDATA, (void*)(LPCTSTR)ttext, 1);

	fNew1 =dataArray[1];
	PEvset(m_hPE1, PEP_faAPPENDYDATA, &fNew1, 1);
	fNew2 = dataArray[2];

	PEvset(m_hPE2, PEP_szaAPPENDPOINTLABELDATA, (void*)(LPCTSTR)ttext, 1);
	PEvset(m_hPE2, PEP_faAPPENDYDATA, &fNew2, 1);
	fNew3 = dataArray[3];

	PEvset(m_hPE3, PEP_szaAPPENDPOINTLABELDATA, (void*)(LPCTSTR)ttext, 1);
	PEvset(m_hPE3, PEP_faAPPENDYDATA, &fNew3, 1);
	return 0;
}


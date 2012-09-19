
// ClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"

#include "DataMgr.h"
#include "DlgModify.h"
#include "DlgLoad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "../../include/UI/ImageHelpers.h"

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


// CClientDlg 对话框



CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: ui::frame::CNonFrameDlg(this, CClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	ui::frame::CNonFrameDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_MAIN_MIN, wndBtnMin_);
	DDX_Control(pDX, IDC_BUTTON_MAIN_CLOSE, wndBtnClose_);

	DDX_Control(pDX, IDC_BUTTON_MAIN, wndBtnMain_);
	DDX_Control(pDX, IDC_BUTTON_NEI, wndBtnNei_);
	DDX_Control(pDX, IDC_BUTTON_VIDEO, wndBtnVideo_);
	DDX_Control(pDX, IDC_BUTTON_PUSH, wndBtnPush_);
	DDX_Control(pDX, IDC_BUTTON_GOLDEN, wndBtnGolden_);
	DDX_Control(pDX, IDC_BUTTON_HIGH, wndBtnDiamond_);
	DDX_Control(pDX, IDC_BUTTON_JIGOU, wndBtnInstitute_);
}

BEGIN_MESSAGE_MAP(CClientDlg, ui::frame::CNonFrameDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_MAIN, &CClientDlg::OnBnClickedButtonMain)
	ON_BN_CLICKED(IDC_BUTTON_NEI, &CClientDlg::OnBnClickedButtonNei)
	ON_BN_CLICKED(IDC_BUTTON_VIDEO, &CClientDlg::OnBnClickedButtonVideo)
	ON_BN_CLICKED(IDC_BUTTON_PUSH, &CClientDlg::OnBnClickedButtonPush)
	ON_BN_CLICKED(IDC_BUTTON_GOLDEN, &CClientDlg::OnBnClickedButtonGolden)
	ON_BN_CLICKED(IDC_BUTTON_HIGH, &CClientDlg::OnBnClickedButtonHigh)
	ON_BN_CLICKED(IDC_BUTTON_JIGOU, &CClientDlg::OnBnClickedButtonJigou)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_MIN, &CClientDlg::OnBnClickedButtonMainMin)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_CLOSE, &CClientDlg::OnBnClickedButtonMainClose)

	ON_COMMAND(0x01, &CClientDlg::OnModifyPsw)
END_MESSAGE_MAP()


// CClientDlg 消息处理程序

BOOL CClientDlg::OnInitDialog()
{
	__super::OnInitDialog();

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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	bkImg_.Attach(ui::draw::AtlLoadGdiplusImage(IDB_PNG_MAIN_BK, _T("PNG")));

	HBITMAP index0[] = 
	{
		ui::draw::AtlLoadGdiplusImage(IDB_PNG_INDEX0_NORMAL, _T("PNG")),
		ui::draw::AtlLoadGdiplusImage(IDB_PNG_INDEX0_HOVER, _T("PNG")),
		ui::draw::AtlLoadGdiplusImage(IDB_PNG_INDEX0_CLICK, _T("PNG"))
	};
	wndBtnMain_.SetImages(index0[0], index0[1], index0[2]);

	HBITMAP index1[] = 
	{
		ui::draw::AtlLoadGdiplusImage(IDB_PNG_INDEX1_NORMAL, _T("PNG")),
		ui::draw::AtlLoadGdiplusImage(IDB_PNG_INDEX1_HOVER, _T("PNG")),
		ui::draw::AtlLoadGdiplusImage(IDB_PNG_INDEX1_CLICK, _T("PNG"))
	};
	wndBtnNei_.SetImages(index1[0], index1[1], index1[2]);

	HBITMAP index2[] = 
	{
		ui::draw::AtlLoadGdiplusImage(IDB_PNG_INDEX2_NORMAL, _T("PNG")),
		ui::draw::AtlLoadGdiplusImage(IDB_PNG_INDEX2_HOVER, _T("PNG")),
		ui::draw::AtlLoadGdiplusImage(IDB_PNG_INDEX2_CLICK, _T("PNG"))
	};
	wndBtnVideo_.SetImages(index2[0], index2[1], index2[2]);

	HBITMAP index3[] = 
	{
		ui::draw::AtlLoadGdiplusImage(IDB_PNG_INDEX3_NORMAL, _T("PNG")),
		ui::draw::AtlLoadGdiplusImage(IDB_PNG_INDEX3_HOVER, _T("PNG")),
		ui::draw::AtlLoadGdiplusImage(IDB_PNG_INDEX3_CLICK, _T("PNG"))
	};
	wndBtnPush_.SetImages(index3[0], index3[1], index3[2]);

	HBITMAP index4[] = 
	{
		ui::draw::AtlLoadGdiplusImage(IDB_PNG_INDEX4_NORMAL, _T("PNG")),
		ui::draw::AtlLoadGdiplusImage(IDB_PNG_INDEX4_HOVER, _T("PNG")),
		ui::draw::AtlLoadGdiplusImage(IDB_PNG_INDEX4_CLICK, _T("PNG"))
	};
	wndBtnGolden_.SetImages(index4[0], index4[1], index4[2]);

	HBITMAP index5[] = 
	{
		ui::draw::AtlLoadGdiplusImage(IDB_PNG_INDEX5_NORMAL, _T("PNG")),
		ui::draw::AtlLoadGdiplusImage(IDB_PNG_INDEX5_HOVER, _T("PNG")),
		ui::draw::AtlLoadGdiplusImage(IDB_PNG_INDEX5_CLICK, _T("PNG"))
	};
	wndBtnDiamond_.SetImages(index5[0], index5[1], index5[2]);

	HBITMAP index6[] = 
	{
		ui::draw::AtlLoadGdiplusImage(IDB_PNG_INDEX6_NORMAL, _T("PNG")),
		ui::draw::AtlLoadGdiplusImage(IDB_PNG_INDEX6_HOVER, _T("PNG")),
		ui::draw::AtlLoadGdiplusImage(IDB_PNG_INDEX6_CLICK, _T("PNG"))
	};
	wndBtnInstitute_.SetImages(index6[0], index6[1], index6[2]);


	HBITMAP min[] = 
	{
		ui::draw::AtlLoadGdiplusImage(IDB_PNG_MIN_NORMAL, _T("PNG")),
		ui::draw::AtlLoadGdiplusImage(IDB_PNG_MIN_HOVER, _T("PNG")),
		ui::draw::AtlLoadGdiplusImage(IDB_PNG_MIN_CLICK, _T("PNG"))
	};
	wndBtnMin_.SetImages(min[0], min[1], min[2]);

	HBITMAP close[] = 
	{
		ui::draw::AtlLoadGdiplusImage(IDB_PNG_CLOSE_NORMAL, _T("PNG")),
		ui::draw::AtlLoadGdiplusImage(IDB_PNG_CLOSE_HOVER, _T("PNG")),
		ui::draw::AtlLoadGdiplusImage(IDB_PNG_CLOSE_CLICK, _T("PNG"))
	};
	wndBtnClose_.SetImages(close[0], close[1], close[2]);

	htmlCtrl_.CreateFromStatic(IDC_STATIC_HTML, this);

	htmlCtrl_.SetHideContextMenu(TRUE);	
	htmlCtrl_.RegisterCmdMap(0x01, _T("修改密码"));

	htmlCtrl_.Navigate(data::DataMgrInstance().GetUrl(2).c_str());	

	SetChangeSize(false);
	SetWindowText(_T("红马甲用户服务终端"));

	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		__super::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CClientDlg::OnPaint()
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
		__super::OnPaint();
	}
}

BOOL CClientDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(rcClient);

	WTL::CMemoryDC memDC(*pDC, rcClient);

	bkImg_.Draw(memDC, rcClient);

	return TRUE;
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CClientDlg::OnBnClickedButtonMain()
{
	if( !data::DataMgrInstance().HasPermission(2) )
	{
		AfxMessageBox(_T("抱歉！您的权限无法查看该内容。详情致电红马甲VIP 客服热线：400-699-0897。"));
		return;
	}
	htmlCtrl_.Navigate(data::DataMgrInstance().GetUrl(2).c_str());			
}

void CClientDlg::OnBnClickedButtonNei()
{
	if( !data::DataMgrInstance().HasPermission(3) )
	{
		AfxMessageBox(_T("抱歉！您的权限无法查看该内容。详情致电红马甲VIP 客服热线：400-699-0897。"));
		return;
	}
	htmlCtrl_.Navigate(data::DataMgrInstance().GetUrl(3).c_str());			
}


void CClientDlg::OnBnClickedButtonVideo()
{
	if( !data::DataMgrInstance().HasPermission(4) )
	{
		AfxMessageBox(_T("抱歉！您的权限无法查看该内容。详情致电红马甲VIP 客服热线：400-699-0897。"));
		return;
	}
	htmlCtrl_.Navigate(data::DataMgrInstance().GetUrl(4).c_str());
}


void CClientDlg::OnBnClickedButtonPush()
{
	if( !data::DataMgrInstance().HasPermission(5) )
		htmlCtrl_.Navigate(data::DataMgrInstance().GetUrl(9).c_str());	
	else
		htmlCtrl_.Navigate(data::DataMgrInstance().GetUrl(5).c_str());	
}


void CClientDlg::OnBnClickedButtonGolden()
{
	if( !data::DataMgrInstance().HasPermission(6) )
		htmlCtrl_.Navigate(data::DataMgrInstance().GetUrl(10).c_str());
	else
		htmlCtrl_.Navigate(data::DataMgrInstance().GetUrl(6).c_str());	
}


void CClientDlg::OnBnClickedButtonHigh()
{
	if( !data::DataMgrInstance().HasPermission(7) )
		htmlCtrl_.Navigate(data::DataMgrInstance().GetUrl(11).c_str());	
	else
		htmlCtrl_.Navigate(data::DataMgrInstance().GetUrl(7).c_str());	
}


void CClientDlg::OnBnClickedButtonJigou()
{
	htmlCtrl_.Navigate(data::DataMgrInstance().GetUrl(8).c_str());	
}


void CClientDlg::OnBnClickedButtonMainMin()
{
	ShowWindow(SW_MINIMIZE);
}


void CClientDlg::OnBnClickedButtonMainClose()
{
	if( MessageBox(_T("确定需要退出吗?"), _T("提示"), MB_YESNO) == IDYES )
		PostQuitMessage(0);
}


void CClientDlg::OnModifyPsw()
{
	CDlgModify dlg;
	if( IDOK == dlg.DoModal() )
	{
		ShowWindow(SW_HIDE);

		CDlgLoad load;
		if( load.DoModal() != IDOK )
			PostQuitMessage(0);
		else
			ShowWindow(SW_SHOW);

	}
	
}
// MainDlg.cpp: 实现文件
//

#include "pch.h"
#include "iClick.h"
#include "afxdialogex.h"
#include "MainDlg.h"
#include "iClickDlg.h"

// MainDlg 对话框

IMPLEMENT_DYNAMIC(MainDlg, CDialogEx)

MainDlg::MainDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAIN_DIALOG, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	EnableAutomation();

}

MainDlg::~MainDlg()
{
}


BOOL MainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	// 添加Tab标题
	m_tabCtrl.InsertItem(0, _T("后台点击"));
	m_tabCtrl.InsertItem(1, _T("前台点击"));
	m_tabCtrl.InsertItem(2, _T("软件说明"));

	// 创建后台点击页
	iClickDlg = new CiClickDlg();
	iClickDlg->Create(IDD_ICLICK_DIALOG, &m_tabCtrl);

	CRect rect;
	GetClientRect(rect);
	m_tabCtrl.MoveWindow(rect);
	m_tabCtrl.AdjustRect(FALSE, rect); // FALSE 时在传入的矩形上剪切掉标签页区域
	iClickDlg->MoveWindow(rect);

	// tap页设置第一页，
	m_tabCtrl.SetCurSel(0);
	// 显示第一个tab页内容
	iClickDlg->ShowWindow(SW_SHOW);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void MainDlg::OnFinalRelease()
{
	CDialogEx::OnFinalRelease();
}

void MainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabCtrl);
}


BEGIN_MESSAGE_MAP(MainDlg, CDialogEx)
END_MESSAGE_MAP()



static const IID IID_IMainDlg =
{0x5fcc7b92,0xf7d4,0x4d96,{0xa0,0xeb,0x6c,0xce,0xb5,0x17,0x26,0x7c}};





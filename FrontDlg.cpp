// FrontDlg.cpp: 实现文件
//

#include "pch.h"
#include "iClick.h"
#include "afxdialogex.h"
#include "FrontDlg.h"


// FrontDlg 对话框

IMPLEMENT_DYNAMIC(FrontDlg, CDialogEx)

FrontDlg::FrontDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FRONTDLG, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

	EnableAutomation();

}

FrontDlg::~FrontDlg()
{
}

void FrontDlg::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。  基类将自动
	// 删除该对象。  在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

	CDialogEx::OnFinalRelease();
}

void FrontDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HOTKEY1, start_hotkey_ipt);
	DDX_Control(pDX, IDC_EDIT1, gap_ipt);
}


BEGIN_MESSAGE_MAP(FrontDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL FrontDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	start_hotkey_ipt.SetHotKey(start_hotkey,NULL);

	gap_ipt.SetWindowTextW(_T("20"));

	return TRUE;
}

static const IID IID_IFrontDlg =
{0x19d11a9e,0x9882,0x4e17,{0x82,0xfc,0x81,0x1d,0xdd,0x5f,0xf3,0xd8}};




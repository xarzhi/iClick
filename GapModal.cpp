// GapModal.cpp: 实现文件
//

#include "pch.h"
#include "iClick.h"
#include "afxdialogex.h"
#include "GapModal.h"


// GapModal 对话框

IMPLEMENT_DYNAMIC(GapModal, CDialogEx)

GapModal::GapModal(CWnd* pParent /*=nullptr*/,  UINT gap)
	: CDialogEx(IDD_GAP_DIALOG, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
	this->gap = gap;
	EnableAutomation();

}

GapModal::~GapModal()
{
}

void GapModal::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。  基类将自动
	// 删除该对象。  在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

	CDialogEx::OnFinalRelease();
}

void GapModal::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, gap_ipt);
}


BEGIN_MESSAGE_MAP(GapModal, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &GapModal::OnEnChangeEdit1)
END_MESSAGE_MAP()



static const IID IID_IGapModal =
{0xa5a4b0df,0xb2c0,0x4ffb,{0x95,0x2c,0x71,0xe3,0x2f,0x0a,0x54,0x09}};


BOOL GapModal::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString str;
	str.Format(_T("%d"), this->gap);
	gap_ipt.SetWindowTextW(str);
	return FALSE;
}


void GapModal::OnEnChangeEdit1()
{
	CString num;
	gap_ipt.GetWindowText(num);
	gap = (UINT)_ttoi(num);
}

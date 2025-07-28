// TimesDlg.cpp: 实现文件
//

#include "pch.h"
#include "iClick.h"
#include "afxdialogex.h"
#include "TimesDlg.h"


// TimesDlg 对话框

IMPLEMENT_DYNAMIC(TimesDlg, CDialogEx)

TimesDlg::TimesDlg(CWnd* pParent /*=nullptr*/,UINT times)
	: CDialogEx(IDD_TIMES_DLG, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

	this->times = times;
	EnableAutomation();

}

TimesDlg::~TimesDlg()
{
}

void TimesDlg::OnFinalRelease()
{
	CDialogEx::OnFinalRelease();
}

void TimesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, times_ipt);
}
BOOL TimesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString str;
	str.Format(_T("%d"), this->times);
	times_ipt.SetWindowTextW(str);

	return FALSE;
}

BEGIN_MESSAGE_MAP(TimesDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &TimesDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()




static const IID IID_ITimesDlg =
{0x90717404,0xdad1,0x4417,{0x99,0x47,0x32,0xf5,0x34,0xb2,0x49,0x79}};




void TimesDlg::OnEnChangeEdit1()
{
	CString num;
	times_ipt.GetWindowText(num);
	times = (UINT)_ttoi(num);
}

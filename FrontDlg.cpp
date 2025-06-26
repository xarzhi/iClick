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
	CDialogEx::OnFinalRelease();
}

void FrontDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HOTKEY1, start_hotkey_ipt);
	DDX_Control(pDX, IDC_EDIT1, gap_ipt);
}


BEGIN_MESSAGE_MAP(FrontDlg, CDialogEx)
	ON_WM_HOTKEY()
	ON_EN_CHANGE(IDC_HOTKEY1, &FrontDlg::OnHotKeyChanged)
	ON_EN_CHANGE(IDC_EDIT1, &FrontDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()

BOOL FrontDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	start_hotkey_ipt.SetHotKey(start_hotkey,NULL);
	RegisterHotKey(m_hWnd, 0x125, NULL, VK_F8);

	gap_ipt.SetWindowTextW(_T("20"));

	return TRUE;
}


// 多线程点击
UINT FrontClickThread(LPVOID pParam)
{
	FrontDlg* Wnd = (FrontDlg*)pParam;

	INPUT inputs[2] = {};

	inputs[0].type = INPUT_MOUSE;
	inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;  // 左键按下

	inputs[1].type = INPUT_MOUSE;
	inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;    // 左键释放

	while (Wnd->isClick) {
		SendInput(2, inputs, sizeof(INPUT));
		Sleep(Wnd->gap);
	}
	return 0;
}



// 接受快捷键消息
void FrontDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	if (main_tab_index != 1) return;
	if (nHotKeyId == 0x125) {
		if (isClick == TRUE) {
			isClick = FALSE;
		}
		else {
			isClick = TRUE;
			AfxBeginThread(FrontClickThread, this);
		}
	}
	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}

// 快捷键内容改变
void FrontDlg::OnHotKeyChanged()
{
	WORD wVirtualKeyCode;
	WORD wModifiers;
	start_hotkey_ipt.GetHotKey(wVirtualKeyCode, wModifiers);
	UnregisterHotKey(m_hWnd, 0x125);
	RegisterHotKey(m_hWnd, 0x125, wModifiers, wVirtualKeyCode);
}



void FrontDlg::OnEnChangeEdit1()
{
	CString num;
	gap_ipt.GetWindowText(num);
	UINT gapText= (UINT)_ttoi(num);
	if (gapText<1) {
		gap_ipt.SetWindowTextW(L"1");
		gapText = 1;
	}
	gap = gapText;
}





static const IID IID_IFrontDlg =
{0x19d11a9e,0x9882,0x4e17,{0x82,0xfc,0x81,0x1d,0xdd,0x5f,0xf3,0xd8}};






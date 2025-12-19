// FrontDlg.cpp: 实现文件
//

#include "pch.h"
#include "iClick.h"
#include "afxdialogex.h"
#include "FrontDlg.h"
#include <imm.h>
#pragma comment(lib, "imm32.lib")

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
	DDX_Control(pDX, IDC_EDIT2, loop_ipt);
}


BEGIN_MESSAGE_MAP(FrontDlg, CDialogEx)
	ON_WM_HOTKEY()
	ON_EN_CHANGE(IDC_HOTKEY1, &FrontDlg::OnHotKeyChanged)
	ON_EN_CHANGE(IDC_EDIT1, &FrontDlg::OnEnChangeEdit1)

	ON_EN_CHANGE(IDC_EDIT2, &FrontDlg::OnEnChangeEdit2)
END_MESSAGE_MAP()

BOOL FrontDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	start_hotkey_ipt.SetHotKey(start_hotkey,NULL);
	RegisterHotKey(m_hWnd, 0x125, NULL, VK_F9);

	gap_ipt.SetWindowTextW(_T("20"));
	loop_ipt.SetWindowTextW(_T("0"));


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
	UINT loop_times = Wnd->loop_times;

	while (Wnd->isClick) {
		SendInput(2, inputs, sizeof(INPUT));
		Sleep(Wnd->gap);
		if (Wnd->loop_times != 0) {
			loop_times--;
			if (loop_times == 0) {
				Wnd->isClick = false;
				return 0;
			};
		}
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
	UINT mod = 0;

	start_hotkey_ipt.GetHotKey(wVirtualKeyCode, wModifiers);

	if (wModifiers & HOTKEYF_CONTROL) mod |= MOD_CONTROL;
	if (wModifiers & HOTKEYF_SHIFT)   mod |= MOD_SHIFT;
	if (wModifiers & HOTKEYF_ALT)     mod |= MOD_ALT;

	UnregisterHotKey(m_hWnd, 0x125);
	RegisterHotKey(m_hWnd, 0x125, mod, wVirtualKeyCode);
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

void SetEnglishIME(HWND hWnd) {
	HIMC hImc = ImmGetContext(hWnd); // 获取输入法上下文
	if (hImc) {
		DWORD dwConv, dwSent;
		// 获取当前状态
		ImmGetConversionStatus(hImc, &dwConv, &dwSent);
		// 设置标志位：英文 + 半角
		dwConv |= IME_CMODE_ALPHANUMERIC; // 英文模式
		dwConv &= ~(IME_CMODE_FULLSHAPE | IME_CMODE_NATIVE); // 禁用全角/本地语言
		// 应用新状态
		ImmSetConversionStatus(hImc, dwConv, dwSent);
		ImmReleaseContext(hWnd, hImc); // 释放上下文
	}
}



static const IID IID_IFrontDlg =
{0x19d11a9e,0x9882,0x4e17,{0x82,0xfc,0x81,0x1d,0xdd,0x5f,0xf3,0xd8}};






void FrontDlg::OnEnChangeEdit2()
{
	CString num;
	loop_ipt.GetWindowText(num);
	loop_times = (UINT)_ttoi(num);
}

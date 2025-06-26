#pragma once
#include "afxdialogex.h"


// FrontDlg 对话框

class FrontDlg : public CDialogEx
{
	DECLARE_DYNAMIC(FrontDlg)

public:
	FrontDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~FrontDlg();

	virtual void OnFinalRelease();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FRONTDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg) {
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) {
			return TRUE;
		}
		return CDialogEx::PreTranslateMessage(pMsg);
	}
	virtual BOOL OnInitDialog();

public:
	UINT gap = 20;
	UINT start_hotkey = VK_F8;
	CHotKeyCtrl start_hotkey_ipt;
	CEdit gap_ipt;
	int main_tab_index;
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	afx_msg void OnHotKeyChanged();
	BOOL isClick = FALSE;
	afx_msg void OnEnChangeEdit1();
};

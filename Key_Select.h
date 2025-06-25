#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "pch.h"
#include "iClickDlg.h"

// Key_Select 对话框

class Key_Select : public CDialogEx
{
	DECLARE_DYNAMIC(Key_Select)

public:
	Key_Select(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Key_Select();
	DWORD m_dwHotkey = 0;
	virtual void OnFinalRelease();

	HotKeyInfo m_hotKeyInfo;

	virtual BOOL PreTranslateMessage(MSG* pMsg) {
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) {
			return TRUE;
		}
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) {
			m_hotkey.SetHotKey(VK_RETURN,NULL);
			return TRUE;
		}
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_BACK) {
			m_hotkey.SetHotKey(VK_BACK,NULL);
			return TRUE;
		}
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE) {
			m_hotkey.SetHotKey(VK_SPACE,NULL);
			return TRUE;				
		}
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB) {
			m_hotkey.SetHotKey(VK_TAB,NULL);
			return TRUE;
		}

		return CDialogEx::PreTranslateMessage(pMsg);
	}
	virtual void OnOK();
	CString ConvertToDisplayString();

	
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KEYSELECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CHotKeyCtrl m_hotkey;
private:
};

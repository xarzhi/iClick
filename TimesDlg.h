#pragma once
#include "afxdialogex.h"


// TimesDlg 对话框

class TimesDlg : public CDialogEx
{
	DECLARE_DYNAMIC(TimesDlg)

public:
	TimesDlg(CWnd* pParent = nullptr, UINT times=1);   // 标准构造函数
	virtual ~TimesDlg();
	UINT times = 1;

	virtual void OnFinalRelease();
	virtual BOOL PreTranslateMessage(MSG* pMsg) {
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) {
			return TRUE;
		}
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) {
			return TRUE;
		}
		return CDialogEx::PreTranslateMessage(pMsg);
	}
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TIMES_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	CEdit times_ipt;
	afx_msg void OnEnChangeEdit1();
};

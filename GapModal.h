#pragma once
#include "afxdialogex.h"


// GapModal 对话框

class GapModal : public CDialogEx
{
	DECLARE_DYNAMIC(GapModal)

public:
	GapModal(CWnd* pParent = nullptr,UINT gap=0);   // 标准构造函数
	virtual ~GapModal();

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
	enum { IDD = IDD_GAP_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:

	afx_msg void OnEnChangeEdit1();
	CEdit gap_ipt;
	UINT gap = 0;

};

#pragma once
#include "afxdialogex.h"
#include "iClickDlg.h"
#include "FrontDlg.h"

// MainDlg 对话框

class MainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MainDlg)

public:
	MainDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~MainDlg();

	virtual void OnFinalRelease();

	BOOL OnInitDialog();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAIN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg) {
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) {
			return TRUE;
		}
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) {
			return TRUE;
		}
		return CDialogEx::PreTranslateMessage(pMsg);
	}
	DECLARE_MESSAGE_MAP()

	CiClickDlg* iClickDlg;
	FrontDlg* frontDlg;
public:
	HICON m_hIcon;
	CTabCtrl m_tabCtrl;
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	int tabIndex = 0;

};

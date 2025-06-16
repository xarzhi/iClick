
// iClickDlg.h: 头文件
//

#pragma once
#define WM_MOUSEBUTTONDOWN WM_USER+777

// CiClickDlg 对话框
class CiClickDlg : public CDialogEx
{
// 构造
public:
	CiClickDlg(CWnd* pParent = nullptr);	// 标准构造函数
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ICLICK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMOutofmemoryHotkey1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnStnClickedStaticPic();
	CListCtrl list;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	CButton setOnTop_Check;
	CEdit hwnd_ipt;
	CHotKeyCtrl hotkey1;
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	CEdit wnd_title_ipt;
	CButton start_Watch_Check;
	afx_msg void OnNMThemeChangedHotkey3(NMHDR* pNMHDR, LRESULT* pResult);
	BOOL isClick=FALSE;
	UINT gap = 20;
	UINT loop = 0;
	UINT Random_Radius = 0;
	CButton start_btn;
	CEdit gap_ipt;
	UINT watch_hotkey = VK_F1;
	afx_msg void OnEnChangeEdit5();
	CEdit loop_ipt;
	CEdit blurry_ipt;
	afx_msg void OnHotKeyChanged(); // 声明处理函数
	CHotKeyCtrl start_hotkey;
	afx_msg void OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult); 
	CMenu m_Menu;
	afx_msg void OnMenuRClick();
	int select_row;
	BOOL isRandomClick = FALSE;
	afx_msg void OnDeleteAll();
	afx_msg void OnBnClickedCheck1();
	CButton random_check;
	afx_msg void OnEnChangeEdit1();
};


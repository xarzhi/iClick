
// iClickDlg.h: 头文件
//

#pragma once


#define WM_MOUSEBUTTONDOWN WM_USER+777
struct HotKeyInfo {
	WORD wVirtualKey='/0';      // 主键虚拟码（如VK_A）
	WORD wModifiers= '/0';       // 修饰键（如HOTKEYF_CONTROL）
	CString strDisplay=L"";    // 显示文本（如"Ctrl+A"）
};

typedef struct PointInfo {
	int x = 0;
	int y = 0;
	HWND hwnd = NULL;
	int event_type = 1;			// 事件类型        1：鼠标事件   2：键盘事件
	int moust_key = 1;			// 鼠标点击类型    1:单机    2：双击
	UINT gap = 20;
	HotKeyInfo hotKeyInfo;
}PointInfo;

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
	virtual BOOL PreTranslateMessage(MSG* pMsg) {
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) {
			return TRUE;
		}
		return CDialogEx::PreTranslateMessage(pMsg);
	}


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
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
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
	UINT watch_hotkey = VK_F6;
	afx_msg void OnEnChangeEdit5();
	CEdit loop_ipt;
	CEdit blurry_ipt;
	afx_msg void OnHotKeyChanged(); // 声明处理函数
	afx_msg void OnStartHotKeyChanged(); // 声明处理函数
	CHotKeyCtrl start_hotkey;
	afx_msg void OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult); 
	CMenu m_Menu;
	afx_msg void OnMenuRClick();
	INT select_row=-1;
	BOOL isRandomClick = FALSE;
	afx_msg void OnDeleteAll();
	afx_msg void OnBnClickedCheck1();
	CButton random_check;
	BOOL isDown = FALSE;
	BOOL start_watch = FALSE;
	INT Event_Type = 1;  // 1：鼠标事件   2：键盘事件
	afx_msg void OnEnChangeEdit1();
	CStatic pic_box;
	BOOL need_hide = FALSE;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedCheck4();
	CButton hide_check;
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void ChangeToSingleClick();
	afx_msg void ChangeToDoubleClick();
	afx_msg void DeleteSingleRow();
	afx_msg void DeleteAllRow();
	afx_msg void OpenKeySelectDlg();
	CEdit loop_edit;
	UINT loop_times = 0;
	afx_msg void OpenGapModal();
	afx_msg void OpenGapDialog1();
};


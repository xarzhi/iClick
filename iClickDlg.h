
// iClickDlg.h: 头文件
//

#pragma once


#define WM_MOUSEBUTTONDOWN WM_USER+777
#include <vector>
using namespace std;

// 键盘事件热键结构体
 struct HotKeyInfo {
	WORD wVirtualKey='/0';      // 主键虚拟码（如VK_A）
	WORD wModifiers= '/0';       // 修饰键（如HOTKEYF_CONTROL）
	CString strDisplay=L"";    // 显示文本（如"Ctrl+A"）
};

// 列表中单行数据结构体
typedef struct PointInfo {
	int x = 0;			// 相对于 控件 窗口的 x 坐标
	int y = 0;			// 相对于 控件 窗口的 y 坐标
	int screenX = 0;	// 相对于 电脑 窗口的 x 坐标y
	int screenY = 0;	// 相对于 电脑 窗口的 x 坐标
	HWND hwnd = NULL;	// 窗口句柄
	CString className;  // 窗口类名
	int event_type = 1;	// 事件类型			1：鼠标事件   2：键盘事件
	int moust_key = 1;	// 鼠标点击类型		说明在下方 
	int keybd_key = 1;	// 键盘类型			1：组合按下松开	1：指定按键按下  2：指定按键松开 
	DWORD keyCode = 0;
	UINT gap = 20;		// 单次操作前的延迟
	UINT times = 1;		// 单次操作执行次数
	int scrollDistance = 0;   // 滚轮滚动距离
	CString title;		// 窗口标题
	HotKeyInfo hotKeyInfo;	// 键盘信息
}PointInfo;



/*  moust_key
	1: 左键单机   
	2：左键双击  
	3：滚轮上滚  
	4：滚轮下滚 
	5：滚轮单击 
	6：右键单击 
	7：右键双击 
	8：鼠标移动 
	9：左键按下 
	10：左键松开
	11：中键按下 
	12：中键松开
	13：右键按下 
	14：右键松开
*/





// 总体配置结构体
typedef struct Config {
	UINT loop_times=0;  // 循环次数
	BOOL isFrontOpt=FALSE;	// 是否前台操作
	BOOL need_hide= FALSE; // 是否选择时隐藏主窗口
	BOOL isRandomClick= FALSE;  // 是否模糊点击
	BOOL setOnTop=TRUE;  // 是否窗口置顶
	UINT Random_Radius=0; // 模糊点击半径
	UINT gap=20; // 总操作间隔
	UINT loop=0; // 循环间隔
	INT Event_Type = 1;  // 1：鼠标事件   2：键盘事件
	HotKeyInfo start_hotkey = { VK_F8, 0, L"F8" };		//	开始快捷键
	HotKeyInfo mouse_hotkey = {};		//	鼠标事件快捷键
	HotKeyInfo keyboard_hotkey = {};		//	键盘事件快捷键
	vector<PointInfo> List;
	
	// 新增迁移变量
	BOOL isClick = FALSE;
	BOOL isScript = FALSE;
	BOOL isRecording = FALSE;
	BOOL start_watch = FALSE;
	UINT watch_hotkey = VK_F6;
	UINT keybd_hotkey = VK_F7;
	UINT record_hotkey = VK_F10;
}Config;



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
		if (pMsg->message == WM_MOUSEMOVE || pMsg->message == WM_LBUTTONDOWN) {
			loop_time_tip.RelayEvent(pMsg);
			front_tip.RelayEvent(pMsg);
			random_tip.RelayEvent(pMsg);
			random_ipt_tip.RelayEvent(pMsg);
			gap_tip.RelayEvent(pMsg);
			loop_gap_tip.RelayEvent(pMsg);
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
	Config config;
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedCheck2();
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
	CButton start_btn;
	CEdit gap_ipt;
	afx_msg void OnEnChangeEdit5();
	CEdit loop_ipt;
	CEdit blurry_ipt;
	afx_msg void OnHotKeyChanged(); // 声明处理函数
	afx_msg void OnStartHotKeyChanged(); // 声明处理函数
	afx_msg void OnRecordHotKeyChanged(); // 声明处理函数
	CHotKeyCtrl start_hotkey;
	afx_msg void OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult); 
	CMenu m_Menu;
	afx_msg void OnMenuRClick();
	INT select_row=-1;
	afx_msg void OnDeleteAll();
	afx_msg void OnBnClickedCheck1();
	CButton random_check;
	BOOL isDown = FALSE;
	afx_msg void OnEnChangeEdit1();
	CStatic pic_box;
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
	afx_msg void OpenGapModal();
	afx_msg void OpenGapDialog1();
	int main_tab_index=0;
	BOOL m_bTracking = FALSE;
	CButton isfront_check;
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnEnChangeEdit6();
	void CreateTip(CToolTipCtrl &tipCtrl,WORD ID, CString text);
	CToolTipCtrl loop_time_tip;
	CToolTipCtrl front_tip;
	CToolTipCtrl random_tip;
	CToolTipCtrl random_ipt_tip;
	CToolTipCtrl gap_tip;
	CToolTipCtrl loop_gap_tip;

	CHotKeyCtrl keybd_hotkey_ipt;
	afx_msg void ChangeToKeyBd();
	afx_msg void ChangeToMouse();
	afx_msg void ChangeToMidUp();
	afx_msg void ChangeToMidDown();
	afx_msg void ChangeToMidClick();
	afx_msg void ChangeToRightClick();
	afx_msg void ChangeToRightDbClick();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	void SaveInitConfig(CString Section, CString Key, CString Value, CString filePath);
	void SaveHotKey(CHotKeyCtrl& hotkey, CString Section, CString filePath);
	void ReadHotKey(CHotKeyCtrl& hotkey, CString Section, CString filePath);
	CString ReadSection(CString path, CString Section, CString Key);
	vector<CString> GetPointSections(CString iniPath);
	afx_msg void SetTimes2();
	afx_msg void SetTimes1();
	CButton read_btn;
	CButton save_btn;
	HHOOK g_hMouseHook = NULL;
	HHOOK g_hKeyboardHook = NULL;
	CHotKeyCtrl record_ipt;
	CStatic record_text;
};


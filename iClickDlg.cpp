
// iClickDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "iClick.h"
#include "iClickDlg.h"
#include "afxdialogex.h"
#include "Key_Select.h"
#include "MainDlg.h"
#include "TimesDlg.h"
#include <shlobj.h> // 头文件
#include "GapModal.h"

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cctype> // 用于字符串处理
using namespace std;



LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);



HHOOK g_hMouseHook = NULL;
HHOOK g_hKeyboardHook = NULL;
CString VirtualKeyCodeToCString(DWORD vkCode);
CiClickDlg* g_pThis = nullptr;
INT64 timeTamp = 0;



#ifdef _DEBUG
#define new DEBUG_NEW
#endif





CiClickDlg::CiClickDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ICLICK_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CiClickDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, list);
	DDX_Control(pDX, IDC_CHECK3, setOnTop_Check);
	DDX_Control(pDX, IDC_EDIT3, hwnd_ipt);
	DDX_Control(pDX, IDC_HOTKEY3, hotkey1);
	DDX_Control(pDX, IDC_EDIT4, wnd_title_ipt);
	DDX_Control(pDX, IDC_CHECK2, start_Watch_Check);
	DDX_Control(pDX, IDC_BUTTON1, start_btn);
	DDX_Control(pDX, IDC_EDIT2, gap_ipt);
	DDX_Control(pDX, IDC_EDIT5, loop_ipt);
	DDX_Control(pDX, IDC_EDIT1, blurry_ipt);
	DDX_Control(pDX, IDC_HOTKEY1, start_hotkey);
	DDX_Control(pDX, IDC_CHECK1, random_check);
	DDX_Control(pDX, IDC_STATIC_PIC, pic_box);
	DDX_Control(pDX, IDC_CHECK4, hide_check);
	DDX_Control(pDX, IDC_EDIT6, loop_edit);
	DDX_Control(pDX, IDC_CHECK5, isfront_check);
	DDX_Control(pDX, IDC_HOTKEY2, keybd_hotkey_ipt);
	DDX_Control(pDX, IDC_BUTTON2, read_btn);
	DDX_Control(pDX, IDC_BUTTON3, save_btn);
	//DDX_Control(pDX, IDC_BUTTON4, record_btn);
	DDX_Control(pDX, IDC_HOTKEY4, record_ipt);
	DDX_Control(pDX, IDC_STATIC_RECORD, record_text);
}

BEGIN_MESSAGE_MAP(CiClickDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT2, &CiClickDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_CHECK2, &CiClickDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON1, &CiClickDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK3, &CiClickDlg::OnBnClickedCheck3)
//	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_HOTKEY() //添加消息宏
	ON_WM_HOTKEY()
	ON_EN_CHANGE(IDC_EDIT5, &CiClickDlg::OnEnChangeEdit5)
	ON_EN_CHANGE(IDC_HOTKEY3, &CiClickDlg::OnHotKeyChanged)
	ON_EN_CHANGE(IDC_HOTKEY1, &CiClickDlg::OnStartHotKeyChanged)
	ON_EN_CHANGE(IDC_HOTKEY4, &CiClickDlg::OnRecordHotKeyChanged)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CiClickDlg::OnNMRClickList1)
	ON_COMMAND(ID_32773, &CiClickDlg::OnMenuRClick)
	ON_COMMAND(ID_32774, &CiClickDlg::OnDeleteAll)
	ON_BN_CLICKED(IDC_CHECK1, &CiClickDlg::OnBnClickedCheck1)
	ON_EN_CHANGE(IDC_EDIT1, &CiClickDlg::OnEnChangeEdit1)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_CHECK4, &CiClickDlg::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_RADIO3, &CiClickDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CiClickDlg::OnBnClickedRadio4)
	ON_COMMAND(ID_32776, &CiClickDlg::ChangeToSingleClick)
	ON_COMMAND(ID_32777, &CiClickDlg::ChangeToDoubleClick)
	ON_COMMAND(ID_32778, &CiClickDlg::DeleteSingleRow)
	ON_COMMAND(ID_32779, &CiClickDlg::DeleteAllRow)
	ON_COMMAND(ID_32781, &CiClickDlg::OpenKeySelectDlg)

	ON_COMMAND(ID_32785, &CiClickDlg::OpenKeySelectDlg)
	ON_COMMAND(ID_32787, &CiClickDlg::OpenGapModal)
	ON_COMMAND(ID_32786, &CiClickDlg::OpenGapDialog1)
	ON_BN_CLICKED(IDC_CHECK5, &CiClickDlg::OnBnClickedCheck5)
	ON_EN_CHANGE(IDC_EDIT6, &CiClickDlg::OnEnChangeEdit6)
	ON_COMMAND(ID_32788, &CiClickDlg::ChangeToKeyBd)
	ON_COMMAND(ID_32794, &CiClickDlg::ChangeToMouse)
	ON_COMMAND(ID_32789, &CiClickDlg::ChangeToMidUp)
	ON_COMMAND(ID_32790, &CiClickDlg::ChangeToMidDown)
	ON_COMMAND(ID_32793, &CiClickDlg::ChangeToMidClick)
	ON_COMMAND(ID_32791, &CiClickDlg::ChangeToRightClick)
	ON_COMMAND(ID_32792, &CiClickDlg::ChangeToRightDbClick)
	ON_BN_CLICKED(IDC_BUTTON3, &CiClickDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CiClickDlg::OnBnClickedButton2)
	ON_COMMAND(ID_32795, &CiClickDlg::SetTimes2)
	ON_COMMAND(ID_32796, &CiClickDlg::SetTimes1)
END_MESSAGE_MAP()


// CiClickDlg 消息处理程序

BOOL CiClickDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	/*CRect rect;
	list.GetClientRect(&rect);
	int width = rect.Width() / 4;*/
	list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);	// 整行选择、网格线
	list.InsertColumn(0, _T("X坐标"), LVCFMT_LEFT, 50);
	list.InsertColumn(1, _T("Y坐标"), LVCFMT_LEFT, 50);	
	list.InsertColumn(2, _T("操作类型"), LVCFMT_LEFT,70);	
	list.InsertColumn(3, _T("操作方式"), LVCFMT_LEFT, 70);	
	list.InsertColumn(4, _T("窗口标题"), LVCFMT_LEFT, 120);
	list.InsertColumn(5, _T("延迟"), LVCFMT_LEFT, 60);
	list.InsertColumn(6, _T("执行次数"), LVCFMT_LEFT, 60);


	// 为列表视图控件添加全行选中和栅格风格   
	list.SetExtendedStyle(list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);


	// 点击间隔edit初始化值
	CString strGap, strLoop, strLoopTimes;
	strGap.Format(_T("%d"), config.gap);
	strLoop.Format(_T("%d"), config.loop);
	strLoopTimes.Format(_T("%d"), config.loop_times);

	gap_ipt.SetWindowTextW(strGap);
	loop_ipt.SetWindowTextW(strLoop);
	loop_edit.SetWindowTextW(strLoopTimes);

	CString Random_Radius_Str;
	Random_Radius_Str.Format(_T("%d"), config.Random_Radius);
	blurry_ipt.SetWindowTextW(Random_Radius_Str);

	setOnTop_Check.SetCheck(TRUE);
	CWnd* mainWnd = AfxGetMainWnd();
	mainWnd->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);



	//注册热键 F6
	hotkey1.SetHotKey(config.watch_hotkey, NULL);
	keybd_hotkey_ipt.SetHotKey(config.keybd_hotkey, NULL);
	
	start_hotkey.SetHotKey(config.start_hotkey.wVirtualKey, config.start_hotkey.wModifiers);
	RegisterHotKey(m_hWnd, 0x124, config.start_hotkey.wModifiers, config.start_hotkey.wVirtualKey);


	random_check.SetCheck(config.isRandomClick);

	// 默认禁用模糊点击范围设置
	blurry_ipt.EnableWindow(config.isRandomClick);

	// 表格选中行默认为-1，未选择任何行
	select_row = -1;

	// 初始化单选按钮
	((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(config.Event_Type == 1); 
	((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(config.Event_Type == 2);

	// 给图片空间设置位图
	CBitmap m_bmp;
	m_bmp.LoadBitmap(IDB_BITMAP1);
	CStatic* pPic = (CStatic*)GetDlgItem(IDC_STATIC_PIC);
	pPic->SetWindowPos(NULL, 0, 0, 32, 32, SWP_NOMOVE | SWP_NOZORDER);
	pPic->SetBitmap(m_bmp);
	m_bmp.Detach(); // 关键！防止bmp析构时删除位图

	// 初始化是否前台点击，默认后台点击
	isfront_check.SetCheck(config.isFrontOpt);

	EnableToolTips(TRUE);

	// 添加提示框
	CreateTip(loop_time_tip, IDC_EDIT6, _T("整个列表中的操作会重复相应的次数，自动停止，0为无限重复"));
	CreateTip(front_tip, IDC_CHECK5, _T("前台操作会占用鼠标键盘"));
	CreateTip(random_tip, IDC_CHECK1, _T("是否开始模糊点击"));
	CreateTip(random_ipt_tip, IDC_EDIT1, _T("以目标点为中心，半径为此值的范围内随机点击"));
	CreateTip(gap_tip, IDC_EDIT2, _T("每次操作后的间隔"));
	CreateTip(loop_gap_tip, IDC_EDIT5, _T("整个列表的操作执行一边后的间隔"));

	save_btn.EnableWindow(TRUE);
	read_btn.EnableWindow(TRUE);

	record_ipt.SetHotKey(config.record_hotkey, NULL);
	RegisterHotKey(m_hWnd, 0x128, NULL, config.record_hotkey);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}



void CiClickDlg::CreateTip(CToolTipCtrl &tipCtrl, WORD ID, CString text) {
	tipCtrl.Create(this);
	tipCtrl.AddTool(GetDlgItem(ID), text); // 关联控件及提示文本
	tipCtrl.Activate(TRUE);
}



void CiClickDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CiClickDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CiClickDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CiClickDlg::OnEnChangeEdit2()
{
	CString num;
	gap_ipt.GetWindowText(num);
	config.gap = (UINT)_ttoi(num);
}

// 注册获取开始坐标快捷键
void CiClickDlg::OnBnClickedCheck2()
{
	if (start_Watch_Check.GetCheck() == TRUE) {
		WORD wVirtualKeyCode;
		WORD wModifiers;
		hotkey1.GetHotKey(wVirtualKeyCode, wModifiers);
		RegisterHotKey(m_hWnd, 0x123, wModifiers, wVirtualKeyCode);

		WORD wVirtualKeyCode1;
		WORD wModifiers1;
		keybd_hotkey_ipt.GetHotKey(wVirtualKeyCode1, wModifiers1);
		RegisterHotKey(m_hWnd, 0x126, wModifiers1, wVirtualKeyCode1);
	}
	else {
		 UnregisterHotKey(m_hWnd, 0x123);
		 UnregisterHotKey(m_hWnd, 0x126);
	}
	config.start_watch = start_Watch_Check.GetCheck();
}



int GetRand(int MIN, int MAX)//产生随机数
{
	    int max;
	    max = RAND_MAX;//rand()函数随机数的最大值
	    return (int)(rand() * (MAX - MIN) / max + MIN);
}

void SendLeftClick() {
	INPUT inputs[2] = {};
	inputs[0].type = INPUT_MOUSE;
	inputs[0].mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;

	inputs[1].type = INPUT_MOUSE;
	inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

	SendInput(2, inputs, sizeof(INPUT));
}

void RightClick() {
	INPUT inputs[2] = { 0 };
	// 右键按下
	inputs[0].type = INPUT_MOUSE;
	inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	// 右键释放
	inputs[1].type = INPUT_MOUSE;
	inputs[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	SendInput(2, inputs, sizeof(INPUT));
}


// 发送单个键盘事件
enum class KeyEventType {
	KEY_DOWN,
	KEY_UP
};
void SendKeyEvent(UINT vkCode, KeyEventType eventType) {
	INPUT input = { 0 };
	input.type = INPUT_KEYBOARD;

	// 设置键盘事件
	input.ki.wVk = vkCode;         // 虚拟键码
	input.ki.dwFlags = 0;          // 默认按键按下
	input.ki.dwFlags = eventType == KeyEventType::KEY_UP? KEYEVENTF_KEYUP:0; 

	// 特殊键处理（需要设置扩展键标志）
	if (vkCode == VK_RCONTROL || vkCode == VK_RMENU || vkCode == VK_RSHIFT ||
		vkCode == VK_NUMLOCK || vkCode == VK_RWIN ||
		vkCode == VK_INSERT || vkCode == VK_DELETE ||
		vkCode == VK_HOME || vkCode == VK_END ||
		vkCode == VK_PRIOR || vkCode == VK_NEXT ||
		vkCode == VK_LEFT || vkCode == VK_RIGHT ||
		vkCode == VK_UP || vkCode == VK_DOWN ||
		vkCode == VK_DIVIDE) {
		input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY; // 扩展键标志
	}

	// 发送事件
	SendInput(1, &input, sizeof(INPUT));
}

// 前台事件多线程
UINT FrontThreadOption(LPVOID pParam) {
	CiClickDlg* Wnd = (CiClickDlg*)pParam;
	UINT loop_times = Wnd->config.loop_times;

	while (Wnd->config.isClick) {
		for (const auto& point : Wnd->config.List) {
			if (!Wnd->config.isClick) return 0;

			point.gap > 0 ? Sleep(point.gap) : NULL;		// 延迟

			CPoint ptCursor = { point.x,point.y };

			if (::IsWindow(point.hwnd)) {
				CWnd* pTempWnd = CWnd::FromHandle(point.hwnd);  // 临时对象，仅当前作用域有效
				pTempWnd->SetForegroundWindow();
				pTempWnd->ClientToScreen(&ptCursor);
			}
			else {
				ptCursor.x = point.screenX;
				ptCursor.y = point.screenY;
			}

	
			// 设置模糊点击
			UINT Radius = Wnd->config.Random_Radius;
			int x, y;
			if (Wnd->config.isRandomClick) {
				x = GetRand(ptCursor.x - Radius, ptCursor.x + Radius);
				y = GetRand(ptCursor.y - Radius, ptCursor.y + Radius);
			}
			else {
				x = ptCursor.x;
				y = ptCursor.y;
			}
			// 设置鼠标位置
			SetCursorPos(x, y);

			for (int times = point.times; times > 0; times--) {
				if (!Wnd->config.isClick) return 0;

				if (point.event_type == 1) {		// 鼠标事件
					if (point.moust_key == 1) {// 单击
						SendLeftClick();
					}
					else if (point.moust_key == 2) {	// 双击
						SendLeftClick();
						Sleep(100);					// 模拟双击间隔
						SendLeftClick();
					}
					else if (point.moust_key == 3) {	// 滚轮上滑
						INPUT input = { 0 };
						input.type = INPUT_MOUSE;
						input.mi.dwFlags = MOUSEEVENTF_WHEEL;
						input.mi.mouseData = point.scrollDistance ? point.scrollDistance : 120; // 正值表示向上滚动
						
						SendInput(1, &input, sizeof(INPUT));
					}
					else if (point.moust_key == 4) {// 滚轮下滑
						INPUT input = { 0 };
						input.type = INPUT_MOUSE;
						input.mi.dwFlags = MOUSEEVENTF_WHEEL;

						input.mi.mouseData = point.scrollDistance ? -point.scrollDistance : -120; // 负值表示向下滚动
						SendInput(1, &input, sizeof(INPUT));
					}
					else if (point.moust_key == 5) {// 滚轮点击
						INPUT inputs[2] = { 0 };
						// 中键按下
						inputs[0].type = INPUT_MOUSE;
						inputs[0].mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
						// 中键释放
						inputs[1].type = INPUT_MOUSE;
						inputs[1].mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
						SendInput(2, inputs, sizeof(INPUT));
					}
					else if (point.moust_key == 6) {// 右键单击
						RightClick();
					}
					else if (point.moust_key == 7) {// 右键双击
						RightClick();       // 第一次单击
						Sleep(100);    // 间隔 100ms（系统默认双击间隔为 500ms）
						RightClick();       // 第二次单击
					}
					else if (point.moust_key == 8) {// 鼠标移动
						SetCursorPos(point.screenX, point.screenY);
					}
					else if (point.moust_key == 9) {// 左键按下
						INPUT inputs[1] = {};
						inputs[0].type = INPUT_MOUSE;
						inputs[0].mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;

						SendInput(1, inputs, sizeof(INPUT));
					}
					else if (point.moust_key == 10) {// 左键松开
						INPUT inputs[1] = {};
					
						inputs[0].type = INPUT_MOUSE;
						inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;

						SendInput(1, inputs, sizeof(INPUT));
					}
					else if (point.moust_key == 11) {	// 中键按下
						INPUT inputs[1] = { 0 };

						inputs[0].type = INPUT_MOUSE;
						inputs[0].mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;

						SendInput(1, inputs, sizeof(INPUT));
					}
					else if (point.moust_key == 12) { 	// 中键释放
						INPUT inputs[1] = { 0 };

						inputs[0].type = INPUT_MOUSE;
						inputs[0].mi.dwFlags = MOUSEEVENTF_MIDDLEUP;

						SendInput(1, inputs, sizeof(INPUT));
					}
					else if (point.moust_key == 13) {   // 右键按下
						INPUT inputs[1] = { 0 };

						inputs[0].type = INPUT_MOUSE;
						inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;  

						SendInput(1, inputs, sizeof(INPUT));
					}
					else if (point.moust_key == 14) {	 // 右键释放
						INPUT inputs[1] = { 0 };

						inputs[0].type = INPUT_MOUSE;
						inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTUP;    

						SendInput(1, inputs, sizeof(INPUT));
					}
				
				}
				else if (point.event_type == 2) {			// 键盘事件

					if (point.keybd_key ==  1) {
						std::vector<INPUT> inputs1;
						std::vector<INPUT> inputs2;

						DWORD modifiers = point.hotKeyInfo.wModifiers;

						// 1. 按下修饰键（如果存在）
						if (modifiers & HOTKEYF_CONTROL) inputs1.push_back({ INPUT_KEYBOARD, { VK_CONTROL } });
						if (modifiers & HOTKEYF_SHIFT) inputs1.push_back({ INPUT_KEYBOARD, { VK_SHIFT } });
						if (modifiers & HOTKEYF_ALT) inputs1.push_back({ INPUT_KEYBOARD, { VK_MENU } });
						// 2. 按下主键
						INPUT down;
						down.type = INPUT_KEYBOARD;
						down.ki.wVk = point.hotKeyInfo.wVirtualKey;
						down.ki.wScan = MapVirtualKey(point.hotKeyInfo.wVirtualKey, MAPVK_VK_TO_VSC);
						down.ki.dwFlags = KEYEVENTF_SCANCODE;  // 按键按下
						inputs2.push_back(down);


						INPUT up;
						up.type = INPUT_KEYBOARD;
						up.ki.wVk = point.hotKeyInfo.wVirtualKey;
						up.ki.wScan = MapVirtualKey(point.hotKeyInfo.wVirtualKey, MAPVK_VK_TO_VSC);
						up.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;  // 按键按下
						inputs2.push_back(up);

						if (modifiers & HOTKEYF_ALT) inputs2.push_back({ INPUT_KEYBOARD, { VK_MENU ,KEYEVENTF_KEYUP} });
						if (modifiers & HOTKEYF_SHIFT) inputs2.push_back({ INPUT_KEYBOARD, { VK_SHIFT ,KEYEVENTF_KEYUP} });
						if (modifiers & HOTKEYF_CONTROL) inputs2.push_back({ INPUT_KEYBOARD, { VK_CONTROL ,KEYEVENTF_KEYUP} });

						SendInput(static_cast<UINT>(inputs1.size()), inputs1.data(), sizeof(INPUT));
						Sleep(8);
						SendInput(static_cast<UINT>(inputs2.size()), inputs2.data(), sizeof(INPUT));
					}
					else if(point.keybd_key == 2){
						SendKeyEvent(point.keyCode, KeyEventType::KEY_DOWN);
					}
					else if (point.keybd_key == 3) {
						SendKeyEvent(point.keyCode, KeyEventType::KEY_UP);
					}
				}
				Wnd->config.gap > 0 ? Sleep(Wnd->config.gap) : NULL;		// 单次操作间隔

			}


		}
		Wnd->config.loop > 0 ? Sleep(Wnd->config.loop) : NULL;		// 每一轮间隔
		if (Wnd->config.loop_times != 0) {
			loop_times--;
			if (loop_times == 0) {
				Wnd->config.isClick = false;
				Wnd->start_btn.SetWindowTextW(_T("开始"));
				return 0;
			}
		}

	}

	return 0;
}

// 后台事件多线程
UINT BackThreadOption(LPVOID pParam)
{
	CiClickDlg* Wnd = (CiClickDlg*)pParam;
	UINT loop_times= Wnd->config.loop_times;
	
	while (Wnd->config.isClick) {
		for (const auto& point : Wnd->config.List) {
			if (!Wnd->config.isClick) return 0;
			
			HWND targetHwnd = point.hwnd;
			// 如果句柄失效（例如重新读取的脚本），尝试通过标题和类名找回
			if (!::IsWindow(targetHwnd)) {
				if (!point.title.IsEmpty()) {
					targetHwnd = ::FindWindow(point.className.IsEmpty() ? NULL : point.className, point.title);
				}
			}

			if (!::IsWindow(targetHwnd)) continue;

			point.gap > 0 ? Sleep(point.gap) : NULL;		// 延迟
			// 处理鼠标事件
			UINT Radius = Wnd->config.Random_Radius;
			CWnd* pTempWnd = CWnd::FromHandle(targetHwnd);  // 临时对象，仅当前作用域有效

			int x, y;
			if (Wnd->config.isRandomClick) {
				x = GetRand(point.x - Radius, point.x + Radius);
				y = GetRand(point.y - Radius, point.y + Radius);
			}
			else {
				x = point.x;
				y = point.y;
			}

			for (int times = point.times; times > 0; times--) {
				if (!Wnd->config.isClick) return 0;
				switch (point.event_type) {
				case 1:    // 鼠标事件
				{
					switch (point.moust_key) {
					case 1:			// 单击
					{
						pTempWnd->PostMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x, y));
						pTempWnd->PostMessage(WM_LBUTTONUP, 0, MAKELPARAM(x, y));
						break;
					}
					case 2:			// 双击
					{
						// 第一次点击
						pTempWnd->PostMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x, y));
						pTempWnd->PostMessage(WM_LBUTTONUP, 0, MAKELPARAM(x, y));
						// 第二次点击（双击）
						pTempWnd->PostMessage(WM_LBUTTONDBLCLK, MK_LBUTTON, MAKELPARAM(x, y));
						pTempWnd->PostMessage(WM_LBUTTONUP, 0, MAKELPARAM(x, y));
						break;
					}
					case 3:		// 滚轮上滚
					{
						int distance = point.scrollDistance ? point.scrollDistance : 120;
						pTempWnd->PostMessage(WM_MOUSEWHEEL, MAKEWPARAM(0, distance), MAKELPARAM(x, y));
						break;
					}
					case 4:// 滚轮下滚

					{
						int distance = point.scrollDistance ? -point.scrollDistance : -120;
						pTempWnd->PostMessage(WM_MOUSEWHEEL, MAKEWPARAM(0, distance), MAKELPARAM(x, y));
						break;
					}
					case 5: // 滚轮点击
					{
						pTempWnd->PostMessage(WM_MBUTTONDOWN, 0, MAKELPARAM(x, y));
						pTempWnd->PostMessage(WM_MBUTTONUP, 0, MAKELPARAM(x, y));
						break;
					}

					case 6:		// 右键单击
					{
						pTempWnd->PostMessage(WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(x, y));
						pTempWnd->PostMessage(WM_RBUTTONUP, 0, MAKELPARAM(x, y));
						break;
					}
					case 7:		// 右键双击
					{
						pTempWnd->PostMessage(WM_RBUTTONDOWN, 0, MAKELPARAM(x, y));
						pTempWnd->PostMessage(WM_RBUTTONUP, 0, MAKELPARAM(x, y));
						Sleep(200); // 推荐200-500ms
						pTempWnd->PostMessage(WM_RBUTTONDOWN, 0, MAKELPARAM(x, y));
						pTempWnd->PostMessage(WM_RBUTTONUP, 0, MAKELPARAM(x, y));
						break;
					}
					default:
						break;
					}
					break;
				}
				case 2:  // 键盘事件
				{
					pTempWnd->SetForegroundWindow();  // 确保目标窗口在前台
					DWORD modifiers = point.hotKeyInfo.wModifiers;
					DWORD virtualKey = point.hotKeyInfo.wVirtualKey;

					// 按下修饰符键（如果有）
					if (modifiers & HOTKEYF_CONTROL) keybd_event(VK_CONTROL, 0, 0, 0);		// 按下 Ctrl
					if (modifiers & HOTKEYF_SHIFT) keybd_event(VK_SHIFT, 0, 0, 0);        // 按下 Shift
					if (modifiers & HOTKEYF_ALT) keybd_event(VK_MENU, 0, 0, 0);        // 按下 ALT

					keybd_event((BYTE)virtualKey, (BYTE)MapVirtualKey(virtualKey, 0), 0, 0);
					Sleep(8);
					keybd_event((BYTE)virtualKey, (BYTE)MapVirtualKey(virtualKey, 0), KEYEVENTF_KEYUP, 0);

					if (modifiers & HOTKEYF_ALT) keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);   // 松开 ALT
					if (modifiers & HOTKEYF_SHIFT) keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);  // 松开 SHIFT
					if (modifiers & HOTKEYF_CONTROL) keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);  // 松开 Ctrl
					break;
				}
				default:
					break;
				}

				Wnd->config.gap > 0 ? Sleep(Wnd->config.gap) : NULL;		// 单次操作间隔
			}

			
		}
		Wnd->config.loop > 0 ? Sleep(Wnd->config.loop) : NULL;		// 每一轮间隔
		if (Wnd->config.loop_times != 0) {
			loop_times--;
			if (loop_times == 0) {
				Wnd->config.isClick = false;
				Wnd->start_btn.SetWindowTextW(_T("开始"));
				return 0;
			}
		}
	}
	return 0; // 线程退出码
}


void CiClickDlg::OnBnClickedButton1()
{
	if (config.List.empty()) {
		MessageBox(_T("请添加坐标信息"));
		return;
	}
	
	if (config.isClick == TRUE) { 
		config.isClick = FALSE;
		start_btn.SetWindowTextW(_T("开始"));
	} 
	else {
				config.isClick = TRUE;
				if (config.isFrontOpt == TRUE) {
					AfxBeginThread(FrontThreadOption, this);
				}
				else {
					AfxBeginThread(BackThreadOption, this);
				}
				start_btn.SetWindowTextW(_T("停止"));

			}
	save_btn.EnableWindow(!config.isClick);
	read_btn.EnableWindow(!config.isClick);

}


// 设置窗口是否置顶
void CiClickDlg::OnBnClickedCheck3()
{
	CWnd* mainWnd = AfxGetMainWnd();
	
	if (setOnTop_Check.GetCheck() ==  TRUE) {
		mainWnd->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		config.setOnTop = TRUE;
	}
	else {
		mainWnd->SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		config.setOnTop = FALSE;
	}
}




void CiClickDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	CPoint ptCursor;
	GetCursorPos(&ptCursor);//获取鼠标位置
	CString str;
	str.Format(_T("X: %d, Y: %d"), ptCursor.x, ptCursor.y);
	hwnd_ipt.SetWindowTextW(str);
}


// 获取窗口句柄的十进制字符串
CString GetWindowHandleDecimal(CWnd* pWnd) {
	if (pWnd == nullptr || !pWnd->GetSafeHwnd())
		return _T("无效句柄");

	// 将 HWND 转为无符号整数
	UINT_PTR hDecimal = reinterpret_cast<UINT_PTR>(pWnd->GetSafeHwnd());

	// 格式化为十进制字符串
	CString strDecimal;
	strDecimal.Format(_T("%llu"), hDecimal); // 64位系统用 %llu, 32位用 %lu
	return strDecimal;
}


void CiClickDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	if (main_tab_index != 0) return;
	if (nHotKeyId == 0x123) {
		if (config.start_watch == FALSE) return;
		CPoint ptCursor;
		GetCursorPos(&ptCursor);//获取鼠标位置
		int ptScreenX = ptCursor.x;
		int ptScreenY = ptCursor.y;
		CWnd* hWnd = WindowFromPoint(ptCursor); // 获取窗口句柄

		::ScreenToClient(hWnd->m_hWnd, &ptCursor);
		// 窗口标题
		CString str;
		hWnd->GetWindowTextW(str);
		wnd_title_ipt.SetWindowTextW(str);

	

		CString x, y;
		x.Format(_T("%d"), ptCursor.x);
		y.Format(_T("%d"), ptCursor.y);
		int iRow = list.GetItemCount(); //获取行数
		list.InsertItem(iRow, x);
		list.SetItemText(iRow, 1, y);
		list.SetItemText(iRow, 2, L"鼠标");
		list.SetItemText(iRow, 3, L"左键单击");
		list.SetItemText(iRow, 4, str);
		list.SetItemText(iRow, 5, _T("0"));
		list.SetItemText(iRow, 6, _T("1"));

		// 同步到数组
		PointInfo pI;
		pI.x = ptCursor.x;
		pI.y = ptCursor.y;
		pI.screenX = ptScreenX;
		pI.screenY = ptScreenY;
		pI.hwnd = hWnd->m_hWnd;
		pI.event_type = 1;
		pI.gap = 0;
		pI.moust_key = 1;
		pI.times = 1;
		
		config.List.push_back(pI);
	
	}
	else if (nHotKeyId == 0x124) {
		if (config.List.empty()) {
			MessageBox(_T("请添加坐标信息"));
			return;
		}

		if (config.isClick == TRUE) {
			start_btn.SetWindowTextW(_T("开始"));
			config.isClick = FALSE;
		}
		else {
			config.isClick = TRUE;
			if (config.isFrontOpt == TRUE) {
				AfxBeginThread(FrontThreadOption, this);
			}
			else {
				AfxBeginThread(BackThreadOption, this);
			}
			start_btn.SetWindowTextW(_T("停止"));

		}
		save_btn.EnableWindow(!config.isClick);
		read_btn.EnableWindow(!config.isClick);
	}
	else if (nHotKeyId == 0x126) {
		if (config.start_watch == FALSE) return;
		CPoint ptCursor;
		GetCursorPos(&ptCursor);//获取鼠标位置
		int ptScreenX = ptCursor.x;
		int ptScreenY = ptCursor.y;

		CWnd* hWnd = WindowFromPoint(ptCursor); // 获取窗口句柄
		
		::ScreenToClient(hWnd->m_hWnd, &ptCursor);
		// 窗口标题
		CString str;
		hWnd->GetWindowTextW(str);
		wnd_title_ipt.SetWindowTextW(str);



		CString x, y;
		x.Format(_T("%d"), ptCursor.x);
		y.Format(_T("%d"), ptCursor.y);
		int iRow = list.GetItemCount(); //获取行数
		list.InsertItem(iRow, x);
		list.SetItemText(iRow, 1, y);
	
		list.SetItemText(iRow, 2, L"键盘");
		list.SetItemText(iRow, 3, L"");
		list.SetItemText(iRow, 4, str);
		list.SetItemText(iRow, 5, _T("0"));
		list.SetItemText(iRow, 6, _T("1"));

		// 同步到数组
		PointInfo pI;
		pI.x = ptCursor.x;
		pI.y = ptCursor.y;
		pI.screenX = ptScreenX;
		pI.screenY = ptScreenY;
		pI.hwnd = hWnd->m_hWnd;
		pI.event_type = 2;
		pI.gap = 0;
		pI.times = 1;

		TCHAR szClassName[256] = { 0 };
		int len = ::GetClassName(hWnd->m_hWnd, szClassName, 256);
		pI.className = CString(szClassName, len);

		config.List.push_back(pI);
	}
	else if (nHotKeyId == 0x128) {			// 录制脚本功能

		if (!config.isFrontOpt) {
			MessageBox(L"只可录制前台操作");
			return;
		};
		if (config.isClick) return;
		if (!config.isRecording) {
			Sleep(500);
			g_pThis = this;
			g_hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, GetModuleHandle(NULL), 0);
			g_hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);
			if (!g_hMouseHook || !g_hKeyboardHook) {
				AfxMessageBox(_T("钩子安装失败！"));
			}
			record_text.SetWindowTextW(_T("停止录制"));
			config.isScript = TRUE;
		}
		else {
			g_pThis = nullptr;
			if (g_hMouseHook) UnhookWindowsHookEx(g_hMouseHook);
			if (g_hKeyboardHook) UnhookWindowsHookEx(g_hKeyboardHook);
			g_hMouseHook = g_hKeyboardHook = NULL;
			timeTamp = 0;
			record_text.SetWindowTextW(_T("开始录制"));

		}
		config.isRecording = !config.isRecording;
	}
	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}


void CiClickDlg::OnEnChangeEdit5()
{
	CString num;
	loop_ipt.GetWindowText(num);
	config.loop = (UINT)_ttoi(num);
}

// 获取坐标快捷键控件值改变，先取消之前的快捷键，再注册新的
void CiClickDlg::OnHotKeyChanged()
{
	WORD wVirtualKeyCode;
	WORD wModifiers;
	hotkey1.GetHotKey(wVirtualKeyCode, wModifiers);
	UnregisterHotKey(m_hWnd, 0x123);
	RegisterHotKey(m_hWnd, 0x123, wModifiers, wVirtualKeyCode);
}

// 开始结束点击快捷键值改变
void CiClickDlg::OnStartHotKeyChanged()
{
	WORD wVirtualKeyCode;
	WORD wModifiers;
	start_hotkey.GetHotKey(wVirtualKeyCode, wModifiers);
	config.start_hotkey.wVirtualKey = wVirtualKeyCode;
	config.start_hotkey.wModifiers = wModifiers;
	UnregisterHotKey(m_hWnd, 0x124); 
	RegisterHotKey(m_hWnd, 0x124, wModifiers, wVirtualKeyCode);
}
// 开始结束点击快捷键值改变
void CiClickDlg::OnRecordHotKeyChanged()
{
	WORD wVirtualKeyCode;
	WORD wModifiers;
	record_ipt.GetHotKey(wVirtualKeyCode, wModifiers);
	config.record_hotkey = wVirtualKeyCode;
	if (wModifiers) {
		MessageBox(L"录制快捷键带有修饰符会影响录制\n只可设置单个快捷键");
		return;
	}
	else {
		UnregisterHotKey(m_hWnd, 0x128);
		RegisterHotKey(m_hWnd, 0x128, NULL, wVirtualKeyCode);
	}
}


// 列表右键点击事件

void CiClickDlg::OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int row = pNMListView->iItem;

	if (row != -1)
	{
		select_row = row;
		CPoint pt;
		GetCursorPos(&pt);
		// 加载鼠标点击方式的菜单
		if (config.List[row].event_type == 1) {
			CMenu menu;
			menu.LoadMenu(IDR_MENU1);

			CMenu* popmenu;
			popmenu = menu.GetSubMenu(0);
			popmenu->TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, this);
		}
		// 加载设置键位的菜单
		else if (config.List[row].event_type == 2) {
			CMenu menu;
			menu.LoadMenu(IDR_MENU2);

			CMenu* popmenu;
			popmenu = menu.GetSubMenu(0);
			popmenu->TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, this);
		}


	}

	*pResult = 0;
}


void CiClickDlg::OnMenuRClick()
{
	// TODO: 在此添加命令处理程序代码
	if (select_row >= 0 && select_row < (int)config.List.size()) {
		config.List.erase(config.List.begin() + select_row); // 删除第select_row个元素
	}
	list.DeleteItem(select_row);
}



void CiClickDlg::OnBnClickedCheck1()
{
		if (random_check.GetCheck() == TRUE) {
			blurry_ipt.EnableWindow(TRUE);
			config.isRandomClick = TRUE;
		}
		else {
			blurry_ipt.EnableWindow(FALSE);
			config.isRandomClick = FALSE;
		}
}

void CiClickDlg::OnEnChangeEdit1()
{
	CString str;
	blurry_ipt.GetWindowTextW(str);
	UINT num = (UINT)_ttoi(str);
	config.Random_Radius = num;
}

void CiClickDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(&rect);
	ScreenToClient(rect);

	if (point.x > rect.left && point.x<rect.right && point.y>rect.top && point.y < rect.bottom) {
		if (isDown==FALSE) {
			isDown = TRUE;
			if (GetCapture() != NULL) {
				// 可尝试释放其他窗口的捕获
				ReleaseCapture();
			}
			SetCursor(LoadCursor(NULL, IDC_CROSS));
			if (config.need_hide==TRUE) {
				CWnd* mainWnd = AfxGetMainWnd();
				mainWnd->ShowWindow(SW_MINIMIZE);
			}
			SetCapture();

		}
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CiClickDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (isDown==TRUE) {
		if (GetCapture() == this) {
			ReleaseCapture();
		}
		
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		isDown = FALSE;
		CPoint ptCursor;
		GetCursorPos(&ptCursor);//获取鼠标位置
		CWnd* hWnd = WindowFromPoint(ptCursor); // 获取窗口句柄
		int ptScreenX = ptCursor.x;
		int ptScreenY = ptCursor.y;

		// 窗口标题
		CString str;
		hWnd->GetWindowTextW(str);
		wnd_title_ipt.SetWindowTextW(str);

		// 窗口句柄
		CString strHandle = GetWindowHandleDecimal(hWnd);
		hwnd_ipt.SetWindowTextW(strHandle);

		::ScreenToClient(hWnd->m_hWnd, &ptCursor);

		CString x, y;
		x.Format(_T("%d"), ptCursor.x);
		y.Format(_T("%d"), ptCursor.y);
		int iRow = list.GetItemCount(); //获取行数
		PointInfo pI;


		list.InsertItem(iRow, x);
		list.SetItemText(iRow, 1, y);
		if (config.Event_Type == 1) {
			list.SetItemText(iRow, 2, L"鼠标");
			list.SetItemText(iRow, 3, L"左键单击");
		}
		else if (config.Event_Type == 2) {
			list.SetItemText(iRow, 2, L"键盘");
			list.SetItemText(iRow, 3, L"");
			pI.keybd_key = 1;

		}
		list.SetItemText(iRow, 4, str);
		list.SetItemText(iRow, 5, _T("0"));
		list.SetItemText(iRow, 6, _T("1"));

		pI.x = ptCursor.x;
		pI.y = ptCursor.y;
		pI.screenX = ptScreenX;
		pI.screenY = ptScreenY;
		pI.hwnd = hWnd->m_hWnd;
		pI.event_type = config.Event_Type;
		pI.title = str;
		pI.gap = 0;
		pI.times = 1;
		if (config.Event_Type==1) {
			pI.moust_key = 1;
		}

		TCHAR szClassName[256] = { 0 };
		int len = ::GetClassName(hWnd->m_hWnd, szClassName, 256);
		pI.className = CString(szClassName, len);

		config.List.push_back(pI);


		// 恢复显示主窗口
		if (config.need_hide == TRUE) {
			CWnd* mainWnd = AfxGetMainWnd();
			mainWnd->ShowWindow(SW_RESTORE);
		}
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CiClickDlg::OnMouseMove(UINT nFlags, CPoint point)
{

	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (isDown == TRUE) {
		//MessageBox(_T("132"));

		CPoint ptCursor;
		GetCursorPos(&ptCursor);//获取鼠标位置
		CWnd* hWnd = WindowFromPoint(ptCursor); // 获取窗口句柄

		// 窗口标题
		CString str;
		hWnd->GetWindowTextW(str);
		wnd_title_ipt.SetWindowTextW(str);

		// 窗口句柄
		CString str2;
		CString strHandle = GetWindowHandleDecimal(hWnd);
		hwnd_ipt.SetWindowTextW(strHandle);
		
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

void CiClickDlg::OnBnClickedCheck4()
{
	BOOL hide = hide_check.GetCheck();
	config.need_hide = hide;
}

// 修改事件类型为鼠标事件
void CiClickDlg::OnBnClickedRadio3()
{
	if (IsDlgButtonChecked(IDC_RADIO3) == BST_CHECKED) {
		config.Event_Type = 1;
	}
}
// 修改事件类型为键盘事件
void CiClickDlg::OnBnClickedRadio4()
{
	if (IsDlgButtonChecked(IDC_RADIO4) == BST_CHECKED) {
		config.Event_Type = 2;
	}
}

void CiClickDlg::ChangeToSingleClick()
{
	// TODO: 修改点击方式为单击
	config.List[select_row].moust_key = 1;
	list.SetItemText(select_row, 3, L"左键单击");

}

void CiClickDlg::ChangeToDoubleClick()
{
	// TODO: 修改点击方式为双击
	config.List[select_row].moust_key = 2;
	list.SetItemText(select_row, 3, L"左键双击");
}


void CiClickDlg::DeleteSingleRow()
{
	// 删除单行
	if (select_row >= 0 && select_row < (int)config.List.size()) {
		config.List.erase(config.List.begin() + select_row); // 删除第select_row个元素
	}
	list.DeleteItem(select_row);
}

void CiClickDlg::DeleteAllRow()
{
	// 删除所有元素
	config.List.clear(); // 删除所有元素
	list.DeleteAllItems(); // 删除所有行
	config.isScript = FALSE;
}

void CiClickDlg::OnDeleteAll()
{
	// TODO: 在此添加命令处理程序代码
	config.List.clear(); // 删除所有元素
	list.DeleteAllItems(); // 删除所有行
	config.isScript = FALSE;
}

void CiClickDlg::OpenKeySelectDlg()
{
		// 打开键位选择模态框
	Key_Select keySelect;
	if (keySelect.DoModal() == IDOK) { // 阻塞直到模态框关闭[7](@ref)
		config.List[select_row].hotKeyInfo = keySelect.m_hotKeyInfo;
		list.SetItemText(select_row, 3, keySelect.m_hotKeyInfo.strDisplay);
	}
}

void CiClickDlg::OpenGapModal()
{
	GapModal gapModal(NULL, config.List[select_row].gap);

	if (gapModal.DoModal() == IDOK) { // 阻塞直到模态框关闭[7](@ref)
		config.List[select_row].gap = gapModal.gap;
		CString str;
		str.Format(_T("%d"), gapModal.gap);
		list.SetItemText(select_row, 5, str);
	}
}


// 打开单条数据延迟设置弹窗
void CiClickDlg::OpenGapDialog1()
{
	GapModal gapModal(NULL, config.List[select_row].gap);
	if (gapModal.DoModal() == IDOK) { // 阻塞直到模态框关闭[7](@ref)
		config.List[select_row].gap = gapModal.gap;
		CString str;
		str.Format(_T("%d"), gapModal.gap);
		list.SetItemText(select_row, 5, str);
	}
}

void CiClickDlg::OnBnClickedCheck5()
{
	// TODO: 在此添加控件通知处理程序代码
	config.isFrontOpt = isfront_check.GetCheck();
}

void CiClickDlg::OnEnChangeEdit6()
{
	config.loop_times = GetDlgItemInt(IDC_EDIT6, NULL, FALSE);
}

// 修改为键盘事件
void CiClickDlg::ChangeToKeyBd()
{
	config.List[select_row].event_type = 2;
	list.SetItemText(select_row, 2, L"键盘");
	list.SetItemText(select_row, 3, L"");
}

// 修改为鼠标事件
void CiClickDlg::ChangeToMouse()
{
	config.List[select_row].event_type = 1;
	config.List[select_row].moust_key = 1;
	list.SetItemText(select_row, 2, L"鼠标");
	list.SetItemText(select_row, 3, L"左键单击");
}

// 修改为滚轮上滚
void CiClickDlg::ChangeToMidUp()
{
	config.List[select_row].moust_key = 3;
	list.SetItemText(select_row, 3, L"滚轮上滚");
}

// 修改为滚轮下滚
void CiClickDlg::ChangeToMidDown()
{
	config.List[select_row].moust_key = 4;
	list.SetItemText(select_row, 3, L"滚轮下滚");
}


// 修改为滚轮单击
void CiClickDlg::ChangeToMidClick()
{
	config.List[select_row].moust_key = 5;
	list.SetItemText(select_row, 3, L"滚轮单击");
}

// 修改为右键单击
void CiClickDlg::ChangeToRightClick()
{
	config.List[select_row].moust_key = 6;
	list.SetItemText(select_row, 3, L"右键单击");
}

// 修改为右键双击
void CiClickDlg::ChangeToRightDbClick()
{
	config.List[select_row].moust_key = 7;
	list.SetItemText(select_row, 3, L"右键双击");
}



// 获取桌面路径
void GetDesktopPath(CString& strPath) {
	TCHAR szPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(
		NULL,                   // 无父窗口
		CSIDL_DESKTOP,          // 桌面标识符
		NULL,                   // 当前用户令牌
		SHGFP_TYPE_CURRENT,     // 获取当前路径（非默认）
		szPath)))               // 输出缓冲区
	{
		strPath = szPath; // 成功时赋值
	}
	else {
		strPath.Empty();  // 失败时清空
	}
}



void CiClickDlg::SaveInitConfig(CString Section, CString Key, CString Value, CString filePath) {
	if (!filePath.IsEmpty()) {
		::WritePrivateProfileString(Section, Key, Value, filePath);
	}
	else {
		MessageBox(_T("保存路径不能为空"));
	}
}

void CiClickDlg::SaveHotKey(CHotKeyCtrl& hotkey, CString Section, CString filePath) {
	WORD startVirtualKeyCode;
	WORD startModifiers;
	hotkey.GetHotKey(startVirtualKeyCode, startModifiers);

	SaveInitConfig(Section, _T("Ctrl"), startModifiers & HOTKEYF_CONTROL ? _T("1") : _T("0"), filePath);
	SaveInitConfig(Section, _T("Shift"), startModifiers & HOTKEYF_SHIFT ? _T("1") : _T("0"), filePath);
	SaveInitConfig(Section, _T("Alt"), startModifiers & HOTKEYF_ALT ? _T("1") : _T("0"), filePath);

	CString strResult;
	strResult.Format(_T("%d"), startVirtualKeyCode);
	SaveInitConfig(Section, _T("Hotkey"), strResult, filePath);
}

void CiClickDlg::ReadHotKey(CHotKeyCtrl& hotkey, CString Section, CString filePath) {
	CString Ctrl = ReadSection(filePath, Section, _T("Ctrl"));
	CString Shift = ReadSection(filePath, Section, _T("Shift"));
	CString Alt = ReadSection(filePath, Section, _T("Alt"));
	CString Hotkey = ReadSection(filePath, Section, _T("Hotkey"));

	WORD wVirtualKeyCode = (WORD)_ttoi(Hotkey);
	WORD wModifiers = 0;
	if (Ctrl == _T("1")) wModifiers |= HOTKEYF_CONTROL;
	if (Shift == _T("1")) wModifiers |= HOTKEYF_SHIFT;
	if (Alt == _T("1")) wModifiers |= HOTKEYF_ALT;

	hotkey.SetHotKey(wVirtualKeyCode, wModifiers);
}


// 保存配置
void CiClickDlg::OnBnClickedButton3()
{
	CFileDialog dlg(FALSE, _T("ini"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("配置文件 (*.ini)|*.ini|所有文件 (*.*)|*.*||"), this);

	if (dlg.DoModal() == IDOK)
	{
		CString filePath = dlg.GetPathName();

		// 删除旧文件以防干扰
		DeleteFile(filePath);


		// 其他配置
		CString str;
		str.Format(_T("%d"), config.loop_times);
		SaveInitConfig(_T("Global"), _T("loop_times"), str, filePath);

		str.Format(_T("%d"), config.Random_Radius);
		SaveInitConfig(_T("Global"), _T("Random_Radius"), str, filePath);

		str.Format(_T("%d"), config.gap);
		SaveInitConfig(_T("Global"), _T("gap"), str, filePath);

		str.Format(_T("%d"), config.loop);
		SaveInitConfig(_T("Global"), _T("loop"), str, filePath);

		SaveInitConfig(_T("Global"), _T("IsFrontOpt"), config.isFrontOpt ? _T("1") : _T("0"), filePath);
		SaveInitConfig(_T("Global"), _T("need_hide"), config.need_hide ? _T("1") : _T("0"), filePath);
		SaveInitConfig(_T("Global"), _T("isRandomClick"), config.isRandomClick ? _T("1") : _T("0"), filePath);
		SaveInitConfig(_T("Global"), _T("setOnTop"), config.setOnTop ? _T("1") : _T("0"), filePath);

		// start热键
		SaveHotKey(start_hotkey, _T("Start_Hotkey"), filePath);
		SaveHotKey(hotkey1, _T("Mouse_Hotkey"), filePath);
		SaveHotKey(keybd_hotkey_ipt, _T("Keybd_Hotkey"), filePath);
		SaveHotKey(record_ipt, _T("Record_Hotkey"), filePath);

		// 表格配置
		for (int i = 0; i < (int)config.List.size(); i++) {
			CString strSection;
			strSection.Format(_T("Point_%d"), i); // 动态节名
			const PointInfo& point = config.List[i];

			str.Format(_T("%d"), point.x);
			SaveInitConfig(strSection, _T("X"), str, filePath);

			str.Format(_T("%d"), point.y);
			SaveInitConfig(strSection, _T("Y"), str, filePath);

			str.Format(_T("%p"), point.hwnd);
			SaveInitConfig(strSection, _T("Hwnd"), str, filePath);

			SaveInitConfig(strSection, _T("Class_Name"), point.className, filePath);

			str.Format(_T("%d"), point.event_type);
			SaveInitConfig(strSection, _T("Event_Type"), str, filePath);

			str.Format(_T("%d"), point.moust_key);
			SaveInitConfig(strSection, _T("Moust_Key"), str, filePath);

			str.Format(_T("%d"), point.gap);
			SaveInitConfig(strSection, _T("Gap"), str, filePath);

			str.Format(_T("%d"), point.times);
			SaveInitConfig(strSection, _T("Times"), str, filePath);

			str.Format(_T("%d"), point.scrollDistance);
			SaveInitConfig(strSection, _T("ScrollDistance"), str, filePath);

			str.Format(_T("%d"), point.keybd_key);
			SaveInitConfig(strSection, _T("Keybd_Key"), str, filePath);

			SaveInitConfig(strSection, _T("Title"), point.title, filePath);

			if (point.event_type == 2) {
				DWORD modifiers = point.hotKeyInfo.wModifiers;
				SaveInitConfig(strSection, _T("Ctrl"), modifiers & HOTKEYF_CONTROL ? _T("1") : _T("0"), filePath);
				SaveInitConfig(strSection, _T("Shift"), modifiers & HOTKEYF_SHIFT ? _T("1") : _T("0"), filePath);
				SaveInitConfig(strSection, _T("Alt"), modifiers & HOTKEYF_ALT ? _T("1") : _T("0"), filePath);

				CString strResult;
				strResult.Format(_T("%d"), point.hotKeyInfo.wVirtualKey);
				SaveInitConfig(strSection, _T("Hotkey"), strResult, filePath);
			}
		}
		MessageBox(_T("保存成功"));
	}
}


CString CiClickDlg::ReadSection(CString path,CString Section,CString Key) {

	TCHAR szBuffer[256];
	DWORD dwRet = ::GetPrivateProfileString(
		Section,
		Key,
		_T(""),
		szBuffer,
		sizeof(szBuffer) / sizeof(TCHAR),
		path
	);
	CString strValue = szBuffer;  // 获取的值

	return strValue;
}

vector<CString> CiClickDlg::GetPointSections(CString iniPath) {
	TCHAR buffer[65535] = { 0 };  // 缓冲区需足够大
	DWORD len = GetPrivateProfileSectionNames(buffer, 65535, iniPath);

	vector<CString> sections;
	TCHAR* p = buffer;
	while (*p != _T('\0')) {     // 节名以双空字符结尾
		CString section(p);
		if (section.Find(_T("Point_")) == 0) {  // 筛选前缀
			sections.push_back(section);
		}
		p += section.GetLength() + 1;  // 移动到下一个节名
	}
	return sections;
}

// 读取配置
void CiClickDlg::OnBnClickedButton2()
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		_T("配置文件 (*.ini)|*.ini|所有文件 (*.*)|*.*||"),
		this
	);

	if (dlg.DoModal() == IDOK) {        // 显示对话框
		CString filePath = dlg.GetPathName(); // 获取完整文件路径

		// 清理旧数据
		list.DeleteAllItems();
		config.List.clear();

		CString Vloop_times = ReadSection(filePath, _T("Global"), _T("loop_times"));
		loop_edit.SetWindowTextW(Vloop_times);
		config.loop_times = (UINT)_ttoi(Vloop_times);

		CString VRandom_Radius = ReadSection(filePath, _T("Global"), _T("Random_Radius"));
		blurry_ipt.SetWindowTextW(VRandom_Radius);
		config.Random_Radius = (UINT)_ttoi(VRandom_Radius);

		CString Vgap = ReadSection(filePath, _T("Global"), _T("gap"));
		gap_ipt.SetWindowTextW(Vgap);
		config.gap = (UINT)_ttoi(Vgap);

		CString Vloop = ReadSection(filePath, _T("Global"), _T("loop"));
		loop_ipt.SetWindowTextW(Vloop);
		config.loop = (UINT)_ttoi(Vloop);

		CString VIsFrontOpt = ReadSection(filePath, _T("Global"), _T("IsFrontOpt"));
		BOOL BIsFrontOpt = VIsFrontOpt == _T("1") ? TRUE : FALSE;
		config.isFrontOpt = BIsFrontOpt;
		isfront_check.SetCheck(BIsFrontOpt);

		CString Vneed_hide = ReadSection(filePath, _T("Global"), _T("need_hide"));
		BOOL Bneed_hide = Vneed_hide == _T("1") ? TRUE : FALSE;
		config.need_hide = Bneed_hide;
		hide_check.SetCheck(Bneed_hide);

		CString VisRandomClick = ReadSection(filePath, _T("Global"), _T("isRandomClick"));
		BOOL BisRandomClick = VisRandomClick == _T("1") ? TRUE : FALSE;
		config.isRandomClick = BisRandomClick;
		random_check.SetCheck(BisRandomClick);

		CString VsetOnTop = ReadSection(filePath, _T("Global"), _T("setOnTop"));
		BOOL BsetOnTop = VsetOnTop == _T("1") ? TRUE : FALSE;
		config.setOnTop = BsetOnTop;
		setOnTop_Check.SetCheck(BsetOnTop);

		// 读取热键
		ReadHotKey(start_hotkey, _T("Start_Hotkey"), filePath);
		ReadHotKey(hotkey1, _T("Mouse_Hotkey"), filePath);
		ReadHotKey(keybd_hotkey_ipt, _T("Keybd_Hotkey"), filePath);
		ReadHotKey(record_ipt, _T("Record_Hotkey"), filePath);

		// 重新注册热键
		OnStartHotKeyChanged();
		OnHotKeyChanged();
		OnRecordHotKeyChanged();

		vector<CString> pointSections = GetPointSections(filePath);
		int index = 0;
		for (const auto& Section : pointSections) {
			PointInfo point;

			CString X = ReadSection(filePath, Section, _T("X"));
			CString Y = ReadSection(filePath, Section, _T("Y"));
			CString Class_Name = ReadSection(filePath, Section, _T("Class_Name"));
			CString Event_Type = ReadSection(filePath, Section, _T("Event_Type"));
			CString Moust_Key = ReadSection(filePath, Section, _T("Moust_Key"));
			CString Gap = ReadSection(filePath, Section, _T("Gap"));
			CString Times = ReadSection(filePath, Section, _T("Times"));
			CString ScrollDistance = ReadSection(filePath, Section, _T("ScrollDistance"));
			CString Keybd_Key = ReadSection(filePath, Section, _T("Keybd_Key"));
			CString Title = ReadSection(filePath, Section, _T("Title"));
			CString Hwnd = ReadSection(filePath, Section, _T("Hwnd"));

			point.x = (UINT)_ttoi(X);
			point.y = (UINT)_ttoi(Y);
			point.className = Class_Name;
			point.event_type = (UINT)_ttoi(Event_Type);
			point.moust_key = (UINT)_ttoi(Moust_Key);
			point.gap = (UINT)_ttoi(Gap);
			point.times = (UINT)_ttoi(Times);
			if (point.times == 0) point.times = 1;
			point.scrollDistance = _ttoi(ScrollDistance);
			point.keybd_key = _ttoi(Keybd_Key);
			if (point.keybd_key == 0) point.keybd_key = 1;
			point.title = Title;
			point.hwnd = reinterpret_cast<HWND>(_tcstoul(Hwnd, nullptr, 16));

			if (point.event_type == 2) {
				CString Ctrl = ReadSection(filePath, Section, _T("Ctrl"));
				CString Shift = ReadSection(filePath, Section, _T("Shift"));
				CString Alt = ReadSection(filePath, Section, _T("Alt"));
				CString Hotkey = ReadSection(filePath, Section, _T("Hotkey"));

				point.hotKeyInfo.wVirtualKey = (WORD)_ttoi(Hotkey);
				point.hotKeyInfo.wModifiers = 0;
				if (Ctrl == _T("1")) point.hotKeyInfo.wModifiers |= HOTKEYF_CONTROL;
				if (Shift == _T("1")) point.hotKeyInfo.wModifiers |= HOTKEYF_SHIFT;
				if (Alt == _T("1")) point.hotKeyInfo.wModifiers |= HOTKEYF_ALT;
			}

			list.InsertItem(index, X);
			list.SetItemText(index, 1, Y);

			CString opTypeStr = (point.event_type == 1) ? L"鼠标" : L"键盘";
			list.SetItemText(index, 2, opTypeStr);

			CString opMethodStr = L"";
			if (point.event_type == 1) {
				switch (point.moust_key) {
				case 1: opMethodStr = L"左键单击"; break;
				case 2: opMethodStr = L"左键双击"; break;
				case 3: opMethodStr = L"滚轮上滚"; break;
				case 4: opMethodStr = L"滚轮下滚"; break;
				case 5: opMethodStr = L"滚轮单击"; break;
				case 6: opMethodStr = L"右键单击"; break;
				case 7: opMethodStr = L"右键双击"; break;
				case 8: opMethodStr = L"鼠标移动"; break;
				case 9: opMethodStr = L"左键按下"; break;
				case 10: opMethodStr = L"左键松开"; break;
				case 11: opMethodStr = L"中键按下"; break;
				case 12: opMethodStr = L"中键松开"; break;
				case 13: opMethodStr = L"右键按下"; break;
				case 14: opMethodStr = L"右键松开"; break;
				}
			}
			else {
				opMethodStr = VirtualKeyCodeToCString(point.hotKeyInfo.wVirtualKey);
				if (point.hotKeyInfo.wModifiers & HOTKEYF_CONTROL) opMethodStr = L"Ctrl+" + opMethodStr;
				if (point.hotKeyInfo.wModifiers & HOTKEYF_SHIFT) opMethodStr = L"Shift+" + opMethodStr;
				if (point.hotKeyInfo.wModifiers & HOTKEYF_ALT) opMethodStr = L"Alt+" + opMethodStr;
			}
			list.SetItemText(index, 3, opMethodStr);
			list.SetItemText(index, 4, Title);
			list.SetItemText(index, 5, Gap);

			CString strTimes;
			strTimes.Format(_T("%d"), point.times);
			list.SetItemText(index, 6, strTimes);

			config.List.push_back(point);
			index++;
		}

		config.isScript = TRUE;
		MessageBox(_T("读取成功"));
	}
}


void CiClickDlg::SetTimes2()
{
	TimesDlg timesModal(NULL, config.List[select_row].times);
	if (timesModal.DoModal() == IDOK) {
		config.List[select_row].times = timesModal.times;
		CString str;
		str.Format(_T("%d"), timesModal.times);
		list.SetItemText(select_row, 6, str);
	}
}

void CiClickDlg::SetTimes1()
{
	TimesDlg timesModal(NULL, config.List[select_row].times);
	if (timesModal.DoModal() == IDOK) {
		config.List[select_row].times = timesModal.times;
		CString str;
		str.Format(_T("%d"), timesModal.times);
		list.SetItemText(select_row, 6, str);
	}
}



// 获取时间戳
__int64 GetMillisecondTimestamp() {
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime); // 获取本地时间（含毫秒）
	CTime curTime(sysTime);   // 转换为CTime对象
	__int64 seconds = curTime.GetTime(); // 秒级部分
	return seconds * 1000 + sysTime.wMilliseconds; // 组合为毫秒级时间戳
}

// 捕获鼠标事件
void ListAndVectorInstert(int x, int y, int mouse_key, int scrollDistance = 0) {
	int iRow = g_pThis->list.GetItemCount(); //获取行数

	CPoint pt(x, y);
	HWND hWnd = ::WindowFromPoint(pt);
	CString strTitle;
	if (hWnd) {
		::GetWindowText(hWnd, strTitle.GetBuffer(256), 256);
		strTitle.ReleaseBuffer();
		// 转换屏幕坐标到客户端坐标
		::ScreenToClient(hWnd, &pt);
	}

	CString strX, strY;
	strX.Format(_T("%d"), pt.x);
	strY.Format(_T("%d"), pt.y);

	CString eventTypeStr;

	if (mouse_key == 1) {
		eventTypeStr = L"左键单击";
	}
	else if (mouse_key == 2) {
		eventTypeStr = L"左键双击";
	}
	else if (mouse_key == 3) {
		eventTypeStr = L"滚轮上滚";
	}
	else if (mouse_key == 4) {
		eventTypeStr = L"滚轮下滚";
	}
	else if (mouse_key == 5) {
		eventTypeStr = L"滚轮单击";
	}
	else if (mouse_key == 6) {
		eventTypeStr = L"右键单击";
	}
	else if (mouse_key == 7) {
		eventTypeStr = L"右键双击";
	}
	else if (mouse_key == 8) {
		eventTypeStr = L"鼠标移动";
	}
	else if (mouse_key == 9) {
		eventTypeStr = L"左键按下";
	}
	else if (mouse_key == 10) {
		eventTypeStr = L"左键松开";
	}


	PointInfo pI;
	INT64 nowTime = GetMillisecondTimestamp();
	if (timeTamp) {
		pI.gap = (UINT)(nowTime - timeTamp);
	}
	else {
		pI.gap = 0;
	}

	g_pThis->list.InsertItem(iRow, strX);
	g_pThis->list.SetItemText(iRow, 1, strY);
	g_pThis->list.SetItemText(iRow, 2, L"鼠标");
	g_pThis->list.SetItemText(iRow, 3, eventTypeStr);
	g_pThis->list.SetItemText(iRow, 4, strTitle);

	CString timeStr;
	timeStr.Format(_T("%d"), pI.gap);
	g_pThis->list.SetItemText(iRow, 5, timeStr);
	g_pThis->list.SetItemText(iRow, 6, _T("1"));


	pI.x = pt.x;
	pI.y = pt.y;
	pI.screenX = x;
	pI.screenY = y;
	pI.hwnd = hWnd;
	if (hWnd) {
		TCHAR szClassName[256] = { 0 };
		::GetClassName(hWnd, szClassName, 256);
		pI.className = szClassName;
	}
	pI.event_type = 1;
	pI.title = strTitle;
	pI.times = 1;
	pI.moust_key = mouse_key;
	pI.scrollDistance = scrollDistance;

	timeTamp = nowTime;
	g_pThis->config.List.push_back(pI);
}

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= HC_ACTION) { 
		MSLLHOOKSTRUCT* pMouse = (MSLLHOOKSTRUCT*)lParam;   //pMouse->pt为屏幕坐标
		
		int x = pMouse->pt.x;
		int y = pMouse->pt.y;
		switch (wParam) {
		case WM_MOUSEMOVE: {			// 鼠标移动
			ListAndVectorInstert(x,y,8);
			break;
		}
		case WM_LBUTTONDOWN: {			// 左键按下
			ListAndVectorInstert(x, y,  9);
			break;
		}
		case WM_RBUTTONDOWN:{			// 右键按下
			ListAndVectorInstert(x, y, 13);
			break;
		}
		case WM_MBUTTONDOWN:{			// 中键按下
			ListAndVectorInstert(x, y,  11);
			break;
		}
		case WM_LBUTTONUP: {			// 左键松开
			ListAndVectorInstert(x, y, 10);
			break;
		}
		case WM_RBUTTONUP: {			// 右键松开
			ListAndVectorInstert(x, y, 14);
			break;
		}
		case WM_MBUTTONUP: {			// 中键松开
			ListAndVectorInstert(x, y,  12);
			break;
		}
		case WM_MOUSEWHEEL:				// 滚轮滚动
			// 处理滚轮：HIWORD(pMouse->mouseData)为滚动值
			int scrollDelta = GET_WHEEL_DELTA_WPARAM(pMouse->mouseData);
			
			if (scrollDelta > 0) {   // 上划
				ListAndVectorInstert(x, y,  3, scrollDelta);
			}
			else {							// 下划
				ListAndVectorInstert(x, y,  4, scrollDelta);
			}
			break;
		}


	}
	return CallNextHookEx(g_hMouseHook, nCode, wParam, lParam);
}



CString VirtualKeyCodeToCString(DWORD vkCode) {
	// 完整键码映射表（包括所有修饰键的左右版本）
	static const std::vector<std::pair<UINT, CString>> nonCharKeys = {
		{VK_LBUTTON, L"Left Mouse"}, {VK_RBUTTON, L"Right Mouse"},
		{VK_CANCEL, L"Break"}, {VK_MBUTTON, L"Middle Mouse"},
		{VK_XBUTTON1, L"XButton1"}, {VK_XBUTTON2, L"XButton2"},
		{VK_BACK, L"Backspace"}, {VK_TAB, L"Tab"},
		{VK_CLEAR, L"Clear"}, {VK_RETURN, L"Enter"},
		{VK_SHIFT, L"Shift"}, {VK_CONTROL, L"Ctrl"},
		{VK_MENU, L"Alt"}, {VK_PAUSE, L"Pause"},
		{VK_CAPITAL, L"Caps Lock"}, {VK_ESCAPE, L"Esc"},
		{VK_SPACE, L"Space"}, {VK_PRIOR, L"Page Up"},
		{VK_NEXT, L"Page Down"}, {VK_END, L"End"},
		{VK_HOME, L"Home"}, {VK_LEFT, L"Left"},
		{VK_UP, L"Up"}, {VK_RIGHT, L"Right"},
		{VK_DOWN, L"Down"}, {VK_SELECT, L"Select"},
		{VK_PRINT, L"Print"}, {VK_EXECUTE, L"Execute"},
		{VK_SNAPSHOT, L"Print Screen"}, {VK_INSERT, L"Insert"},
		{VK_DELETE, L"Delete"}, {VK_HELP, L"Help"},
		{VK_LWIN, L"Left Win"}, {VK_RWIN, L"Right Win"},
		{VK_APPS, L"Apps"}, {VK_SLEEP, L"Sleep"},
		{VK_NUMPAD0, L"Num 0"}, {VK_NUMPAD1, L"Num 1"},
		{VK_NUMPAD2, L"Num 2"}, {VK_NUMPAD3, L"Num 3"},
		{VK_NUMPAD4, L"Num 4"}, {VK_NUMPAD5, L"Num 5"},
		{VK_NUMPAD6, L"Num 6"}, {VK_NUMPAD7, L"Num 7"},
		{VK_NUMPAD8, L"Num 8"}, {VK_NUMPAD9, L"Num 9"},
		{VK_MULTIPLY, L"*"}, {VK_ADD, L"+"},
		{VK_SEPARATOR, L"Separator"}, {VK_SUBTRACT, L"-"},
		{VK_DECIMAL, L"."}, {VK_DIVIDE, L"/"},
		{VK_F1, L"F1"}, {VK_F2, L"F2"}, {VK_F3, L"F3"},
		{VK_F4, L"F4"}, {VK_F5, L"F5"}, {VK_F6, L"F6"},
		{VK_F7, L"F7"}, {VK_F8, L"F8"}, {VK_F9, L"F9"},
		{VK_F10, L"F10"}, {VK_F11, L"F11"}, {VK_F12, L"F12"},
		{VK_F13, L"F13"}, {VK_F14, L"F14"}, {VK_F15, L"F15"},
		{VK_F16, L"F16"}, {VK_F17, L"F17"}, {VK_F18, L"F18"},
		{VK_F19, L"F19"}, {VK_F20, L"F20"}, {VK_F21, L"F21"},
		{VK_F22, L"F22"}, {VK_F23, L"F23"}, {VK_F24, L"F24"},
		{VK_NUMLOCK, L"Num Lock"}, {VK_SCROLL, L"Scroll Lock"},
		{VK_LSHIFT, L"Left Shift"}, {VK_RSHIFT, L"Right Shift"},
		{VK_LCONTROL, L"Left Ctrl"}, {VK_RCONTROL, L"Right Ctrl"},
		{VK_LMENU, L"Left Alt"}, {VK_RMENU, L"Right Alt"},
		{VK_BROWSER_BACK, L"Browser Back"}, {VK_BROWSER_FORWARD, L"Browser Forward"},
		{VK_BROWSER_REFRESH, L"Browser Refresh"}, {VK_BROWSER_STOP, L"Browser Stop"},
		{VK_BROWSER_SEARCH, L"Browser Search"}, {VK_BROWSER_FAVORITES, L"Browser Favorites"},
		{VK_BROWSER_HOME, L"Browser Home"}, {VK_VOLUME_MUTE, L"Volume Mute"},
		{VK_VOLUME_DOWN, L"Volume Down"}, {VK_VOLUME_UP, L"Volume Up"},
		{VK_MEDIA_NEXT_TRACK, L"Next Track"}, {VK_MEDIA_PREV_TRACK, L"Prev Track"},
		{VK_MEDIA_STOP, L"Stop Media"}, {VK_MEDIA_PLAY_PAUSE, L"Play/Pause"},
		{VK_LAUNCH_MAIL, L"Launch Mail"}, {VK_LAUNCH_MEDIA_SELECT, L"Media Select"},
		{VK_LAUNCH_APP1, L"App1"}, {VK_LAUNCH_APP2, L"App2"},
		{VK_OEM_1, L";"}, {VK_OEM_PLUS, L"="}, {VK_OEM_COMMA, L","},
		{VK_OEM_MINUS, L"-"}, {VK_OEM_PERIOD, L"."}, {VK_OEM_2, L"/"},
		{VK_OEM_3, L"`"}, {VK_OEM_4, L"["}, {VK_OEM_5, L"\\"},
		{VK_OEM_6, L"]"}, {VK_OEM_7, L"'"}, {VK_OEM_8, L"OEM8"},
		{VK_OEM_102, L"OEM102"}, {VK_PROCESSKEY, L"Process Key"},
		{VK_ATTN, L"Attn"}, {VK_CRSEL, L"CrSel"}, {VK_EXSEL, L"ExSel"},
		{VK_EREOF, L"Erase EOF"}, {VK_PLAY, L"Play"}, {VK_ZOOM, L"Zoom"},
		{VK_NONAME, L"NoName"}, {VK_PA1, L"PA1"}, {VK_OEM_CLEAR, L"Clear"}
	};

	// 在非字符键映射表中查找
	for (const auto& key : nonCharKeys) {
		if (vkCode == key.first) {
			return key.second;
		}
	}

	// 处理字符键
	BYTE keyboardState[256] = { 0 };
	GetKeyboardState(keyboardState);

	wchar_t charBuffer[5] = { 0 };
	int result = ToUnicode(vkCode, 0, keyboardState, charBuffer, 4, 0);

	if (result > 0) {
		return CString(charBuffer);
	}

	// 未知键码：返回十六进制格式
	CString hexStr;
	hexStr.Format(L"0x%02X", vkCode);
	return hexStr;
}

// 捕获键盘事件
void ListAndVectorInstertKeyBd(int x, int y, DWORD keyCode, BOOL isDown) {
	int iRow = g_pThis->list.GetItemCount(); //获取行数

	CPoint pt(x, y);
	HWND hWnd = ::WindowFromPoint(pt);
	CString strTitle;
	if (hWnd) {
		::GetWindowText(hWnd, strTitle.GetBuffer(256), 256);
		strTitle.ReleaseBuffer();
		// 转换屏幕坐标到客户端坐标
		::ScreenToClient(hWnd, &pt);
	}

	CString strX, strY;
	strX.Format(_T("%d"), pt.x);
	strY.Format(_T("%d"), pt.y);


	if (g_pThis->config.record_hotkey == keyCode) {
		g_pThis = nullptr;
		if (g_hMouseHook) UnhookWindowsHookEx(g_hMouseHook);
		if (g_hKeyboardHook) UnhookWindowsHookEx(g_hKeyboardHook);
		g_hMouseHook = g_hKeyboardHook = NULL;
		timeTamp = 0;
		return;
	}

	PointInfo pI;
	INT64 nowTime = GetMillisecondTimestamp();
	if (timeTamp) {
		pI.gap = (UINT)(nowTime - timeTamp);
	}
	else {
		pI.gap = 0;
	}

	CString keyStr = VirtualKeyCodeToCString(keyCode);

	OutputDebugString(keyStr);
	OutputDebugString(L"\n");

	CString downStr = keyStr + _T("按下");
	CString upStr = keyStr + _T("松开");

	g_pThis->list.InsertItem(iRow, strX);
	g_pThis->list.SetItemText(iRow, 1, strY);
	g_pThis->list.SetItemText(iRow, 2, L"键盘");
	g_pThis->list.SetItemText(iRow, 3, isDown ? downStr : upStr);
	g_pThis->list.SetItemText(iRow, 4, strTitle);

	CString timeStr;
	timeStr.Format(_T("%d"), pI.gap);
	g_pThis->list.SetItemText(iRow, 5, timeStr);
	g_pThis->list.SetItemText(iRow, 6, _T("1"));

	pI.x = pt.x;
	pI.y = pt.y;
	pI.screenX = x;
	pI.screenY = y;
	pI.keybd_key = isDown ? 2 : 3;
	pI.hwnd = hWnd;
	if (hWnd) {
		TCHAR szClassName[256] = { 0 };
		::GetClassName(hWnd, szClassName, 256);
		pI.className = szClassName;
	}
	pI.keyCode = keyCode;
	pI.event_type = 2;
	pI.title = strTitle;
	pI.times = 1;
	timeTamp = nowTime;
	g_pThis->config.List.push_back(pI);
}

// 键盘钩子处理函数（捕获按键）
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= HC_ACTION) {
		CPoint ptCursor;
		GetCursorPos(&ptCursor);//获取鼠标位置
		KBDLLHOOKSTRUCT* pKey = (KBDLLHOOKSTRUCT*)lParam;
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {				// 处理按键按下：pKey->vkCode为虚拟键码
			ListAndVectorInstertKeyBd(ptCursor.x, ptCursor.y,pKey->vkCode,TRUE);
		}
		else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {  // 处理按键释放
			ListAndVectorInstertKeyBd(ptCursor.x, ptCursor.y, pKey->vkCode, FALSE);
		}
	}
	return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
}



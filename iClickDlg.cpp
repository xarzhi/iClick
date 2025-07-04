﻿
// iClickDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "iClick.h"
#include "iClickDlg.h"
#include "afxdialogex.h"
#include "Key_Select.h"
#include <vector>
#include "MainDlg.h"
#include "GapModal.h"
using namespace std;



vector<PointInfo> pointInfo;

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
}

BEGIN_MESSAGE_MAP(CiClickDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT2, &CiClickDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_CHECK2, &CiClickDlg::OnBnClickedCheck2)
	ON_STN_CLICKED(IDC_STATIC_PIC, &CiClickDlg::OnStnClickedStaticPic)
	ON_BN_CLICKED(IDC_BUTTON1, &CiClickDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK3, &CiClickDlg::OnBnClickedCheck3)
//	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_HOTKEY() //添加消息宏
	ON_WM_HOTKEY()
	ON_NOTIFY(NM_THEMECHANGED, IDC_HOTKEY3, &CiClickDlg::OnNMThemeChangedHotkey3)
	ON_EN_CHANGE(IDC_EDIT5, &CiClickDlg::OnEnChangeEdit5)
	ON_EN_CHANGE(IDC_HOTKEY3, &CiClickDlg::OnHotKeyChanged)
	ON_EN_CHANGE(IDC_HOTKEY1, &CiClickDlg::OnStartHotKeyChanged)
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
	list.InsertColumn(2, _T("操作类型"), LVCFMT_LEFT,100);	
	list.InsertColumn(3, _T("操作方式"), LVCFMT_LEFT, 100);	
	list.InsertColumn(4, _T("窗口标题"), LVCFMT_LEFT, 120);
	list.InsertColumn(5, _T("延迟"), LVCFMT_LEFT, 60);


	// 为列表视图控件添加全行选中和栅格风格   
	list.SetExtendedStyle(list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);


	// 点击间隔edit初始化值
	gap_ipt.SetWindowTextW(_T("20"));
	loop_ipt.SetWindowTextW(_T("0"));

	CString Random_Radius_Str;
	Random_Radius_Str.Format(_T("%d"), Random_Radius);
	blurry_ipt.SetWindowTextW(Random_Radius_Str);

	setOnTop_Check.SetCheck(TRUE);
	CWnd* mainWnd = AfxGetMainWnd();
	mainWnd->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);



	//注册热键 F6
	hotkey1.SetHotKey(watch_hotkey, NULL);
	keybd_hotkey_ipt.SetHotKey(keybd_hotkey, NULL);
	
	start_hotkey.SetHotKey(VK_F8,NULL);
	RegisterHotKey(m_hWnd, 0x124, NULL, VK_F8);


	random_check.SetCheck(isRandomClick);

	// 默认禁用模糊点击范围设置
	blurry_ipt.EnableWindow(FALSE);

	// 表格选中行默认为-1，未选择任何行
	select_row = -1;

	// 初始化单选按钮
	((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(TRUE); //选上
	((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(FALSE);//不选上

	// 给图片空间设置位图
	CBitmap m_bmp;
	m_bmp.LoadBitmap(IDB_BITMAP1);
	CStatic* pPic = (CStatic*)GetDlgItem(IDC_STATIC_PIC);
	pPic->SetWindowPos(NULL, 0, 0, 32,32, SWP_NOMOVE | SWP_NOZORDER);
	pPic->SetBitmap(m_bmp);
	m_bmp.Detach(); // 关键！防止bmp析构时删除位图

	// 初始化是否前台点击，默认后台点击
	isfront_check.SetCheck(isFrontOpt);

	// 初始化循环次数，0代表无限循环
	loop_edit.SetWindowTextW(_T("0"));

	EnableToolTips(TRUE);

	// 添加提示框
	CreateTip(loop_time_tip, IDC_EDIT6, _T("整个列表中的操作会重复相应的次数，自动停止，0为无限重复"));
	CreateTip(front_tip, IDC_CHECK5, _T("前台操作会占用鼠标键盘"));
	CreateTip(random_tip, IDC_CHECK1, _T("是否开始模糊点击"));
	CreateTip(random_ipt_tip, IDC_EDIT1, _T("以目标点为中心，半径为此值的范围内随机点击"));
	CreateTip(gap_tip, IDC_EDIT2, _T("每次操作后的间隔"));
	CreateTip(loop_gap_tip, IDC_EDIT5, _T("整个列表的操作执行一边后的间隔"));


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


void CiClickDlg::OnNMOutofmemoryHotkey1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CiClickDlg::OnEnChangeEdit2()
{
	CString num;
	gap_ipt.GetWindowText(num);
	gap = (UINT)_ttoi(num);
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
	start_watch = start_Watch_Check.GetCheck();
}

void CiClickDlg::OnStnClickedStaticPic()
{
	//MessageBox(_T("asdasd"));
}

int GetRand(int MIN, int MAX)//产生随机数
{
	    int max;
	    max = RAND_MAX;//rand()函数随机数的最大值
	    return (int)(rand() * (MAX - MIN) / max + MIN);
}

long PixelToAbsolute(int pos, bool isX) {
	int screenRes = isX ? GetSystemMetrics(SM_CXSCREEN) : GetSystemMetrics(SM_CYSCREEN);
	return static_cast<long>((pos * 65535) / (screenRes - 1));
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


// 前台事件多线程
UINT FrontThreadOption(LPVOID pParam) {
	CiClickDlg* Wnd = (CiClickDlg*)pParam;
	UINT loop_times = Wnd->loop_times;

	while (Wnd->isClick) {
		for (const auto& point : pointInfo) {
			if (!::IsWindow(point.hwnd)) continue;
			::SetForegroundWindow(point.hwnd);  // 确保目标窗口在前台

			Sleep(point.gap);			// 延迟

			CPoint ptCursor = {point.x,point.y};
			::ClientToScreen(point.hwnd, &ptCursor);

			// 设置模糊点击
			UINT Radius = Wnd->Random_Radius;
			int x, y;
			if (Wnd->isRandomClick) {
				x = GetRand(ptCursor.x - Radius, ptCursor.x + Radius);
				y = GetRand(ptCursor.y - Radius, ptCursor.y + Radius);
			}
			else {
				x = ptCursor.x;
				y = ptCursor.y;
			}
			// 设置鼠标位置
			SetCursorPos(x, y);

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
					input.mi.mouseData = 120; // 正值表示向上滚动
					SendInput(1, &input, sizeof(INPUT));
				}
				else if (point.moust_key == 4) {// 滚轮下滑
					INPUT input = { 0 };
					input.type = INPUT_MOUSE;
					input.mi.dwFlags = MOUSEEVENTF_WHEEL;
					input.mi.mouseData = -120; // 正值表示向上滚动
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
			}
			else if (point.event_type == 2) {			// 键盘事件
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
			Sleep(Wnd->gap);			// 单次操作间隔
		}
		Sleep(Wnd->loop);				// 每一轮间隔
		if (Wnd->loop_times != 0) {
			loop_times--;
			if (loop_times == 0) {
				Wnd->isClick = false;
				Wnd->start_btn.SetWindowTextW(_T("开始"));
				return 0;
			};
		}

	}

	return 0;
}

// 后台事件多线程
UINT BackThreadOption(LPVOID pParam)
{
	CiClickDlg* Wnd = (CiClickDlg*)pParam;
	UINT loop_times= Wnd->loop_times;
	
	while (Wnd->isClick) {
		for (const auto& point : pointInfo) {
			if (!::IsWindow(point.hwnd)) continue;
			Sleep(point.gap);			// 延迟
			if (point.event_type == 1) {
				// 处理鼠标事件
				UINT Radius = Wnd->Random_Radius;
				int x, y;
				if (Wnd->isRandomClick) {
					x = GetRand(point.x - Radius, point.x + Radius);
					y = GetRand(point.y - Radius, point.y + Radius);
				}
				else {
					x = point.x;
					y = point.y;
				}
				if (point.moust_key == 1) {
					// 单击
					::SendMessage(point.hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x, y));
					::SendMessage(point.hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(x, y));
				}
				else if (point.moust_key == 2) {
					// 双击
					 // 第一次点击
					::SendMessage(point.hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x, y));
					::SendMessage(point.hwnd, WM_LBUTTONUP, 0, MAKELPARAM(x, y));
					// 第二次点击（双击）
					::SendMessage(point.hwnd, WM_LBUTTONDBLCLK, MK_LBUTTON, MAKELPARAM(x, y));
					::SendMessage(point.hwnd, WM_LBUTTONUP, 0, MAKELPARAM(x, y));
				}
				else if (point.moust_key == 3) {
					// 滚轮上滚
					UINT distance = 120;		// 滚动距离
					::SendMessage(point.hwnd, WM_MOUSEWHEEL, MAKEWPARAM(0, distance), MAKELPARAM(x, y));
				}
				else if (point.moust_key == 4) {
					// 滚轮下滚
					UINT distance = 120;		// 滚动距离
					::SendMessage(point.hwnd, WM_MOUSEWHEEL, MAKEWPARAM(0, distance * -1), MAKELPARAM(x, y));
				}
				else if (point.moust_key == 5) {
					// 滚轮点击
					::SendMessage(point.hwnd, WM_MBUTTONDOWN, 0, MAKELPARAM(x, y));
					::SendMessage(point.hwnd, WM_MBUTTONUP, 0, MAKELPARAM(x, y));
				}
				else if (point.moust_key == 6) {
					// 右键单击
					::SendMessage(point.hwnd, WM_RBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x, y));
					::SendMessage(point.hwnd, WM_RBUTTONUP, MK_LBUTTON, MAKELPARAM(x, y));
				}
				else if (point.moust_key == 7) {
					// 右键双击
					::SendMessage(point.hwnd, WM_RBUTTONDOWN, 0, MAKELPARAM(x, y));
					::SendMessage(point.hwnd, WM_RBUTTONUP, 0, MAKELPARAM(x, y));
					Sleep(200); // 推荐200-500ms
					::SendMessage(point.hwnd, WM_RBUTTONDOWN, 0, MAKELPARAM(x, y));
					::SendMessage(point.hwnd, WM_RBUTTONUP, 0, MAKELPARAM(x, y));
				}
				
			}
			else if (point.event_type == 2) {
				// 处理键盘事件
				::SetForegroundWindow(point.hwnd);  // 确保目标窗口在前台
				DWORD modifiers = point.hotKeyInfo.wModifiers;
				DWORD virtualKey = point.hotKeyInfo.wVirtualKey;

				// 按下修饰符键（如果有）
				if (modifiers & HOTKEYF_CONTROL) keybd_event(VK_CONTROL, 0, 0, 0);		// 按下 Ctrl
				if (modifiers & HOTKEYF_SHIFT) keybd_event(VK_SHIFT, 0, 0, 0);        // 按下 Shift
				if (modifiers & HOTKEYF_ALT) keybd_event(VK_MENU, 0, 0, 0);        // 按下 ALT

				keybd_event(virtualKey, MapVirtualKey(virtualKey, 0), 0, 0);
				Sleep(8);
				keybd_event(virtualKey, MapVirtualKey(virtualKey, 0), KEYEVENTF_KEYUP, 0);

				if (modifiers & HOTKEYF_ALT) keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);   // 松开 ALT
				if (modifiers & HOTKEYF_SHIFT) keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);  // 松开 SHIFT
				if (modifiers & HOTKEYF_CONTROL) keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);  // 松开 Ctrl
			}
			Sleep(Wnd->gap);			// 单次操作间隔
		}
		Sleep(Wnd->loop);				// 每一轮间隔
		if (Wnd->loop_times != 0) {
			loop_times--;
			if (loop_times == 0) {
				Wnd->isClick = false;
				Wnd->start_btn.SetWindowTextW(_T("开始"));
				return 0;
			};
		}
	}
	return 0; // 线程退出码
}


void CiClickDlg::OnBnClickedButton1()
{
	if (pointInfo.empty()) {
		MessageBox(_T("请添加坐标信息"));
		return;
	}
	
	if (isClick == TRUE) { 
		start_btn.SetWindowTextW(_T("开始"));
		isClick = FALSE;
	} 
	else {
		start_btn.SetWindowTextW(_T("停止"));
		isClick = TRUE;
		if (isFrontOpt ==  TRUE) {
			AfxBeginThread(FrontThreadOption, this);
		}
		else {
			AfxBeginThread(BackThreadOption, this);
		}
	}
}


// 设置窗口是否置顶
void CiClickDlg::OnBnClickedCheck3()
{
	CWnd* mainWnd = AfxGetMainWnd();
	mainWnd->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	
	if (setOnTop_Check.GetCheck() ==  TRUE) {
		mainWnd->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}
	else {
		mainWnd->SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
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
		if (start_watch == FALSE) return;
		CPoint ptCursor;
		GetCursorPos(&ptCursor);//获取鼠标位置

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

		// 同步到数组
		PointInfo pI;
		pI.x = ptCursor.x;
		pI.y = ptCursor.y;
		pI.hwnd = hWnd->m_hWnd;
		pI.event_type = 1;
		pI.gap = 0;
		pI.moust_key = 1;
		
		pointInfo.push_back(pI);
	
	}
	else if (nHotKeyId == 0x124) {
		if (pointInfo.empty()) {
			MessageBox(_T("请添加坐标信息"));
			return;
		}

		if (isClick == TRUE) {
			start_btn.SetWindowTextW(_T("开始"));
			isClick = FALSE;
		}
		else {
			start_btn.SetWindowTextW(_T("停止"));
			isClick = TRUE;
			if (isFrontOpt == TRUE) {
				AfxBeginThread(FrontThreadOption, this);
			}
			else {
				AfxBeginThread(BackThreadOption, this);
			}
		}
	}
	else if (nHotKeyId == 0x126) {
		if (start_watch == FALSE) return;
		CPoint ptCursor;
		GetCursorPos(&ptCursor);//获取鼠标位置

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

		// 同步到数组
		PointInfo pI;
		pI.x = ptCursor.x;
		pI.y = ptCursor.y;
		pI.hwnd = hWnd->m_hWnd;
		pI.event_type = 2;
		pI.gap = 0;

		pointInfo.push_back(pI);
	}
	
	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}

void CiClickDlg::OnNMThemeChangedHotkey3(NMHDR* pNMHDR, LRESULT* pResult)
{
	// 该功能要求使用 Windows XP 或更高版本。
	// 符号 _WIN32_WINNT 必须 >= 0x0501。
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CiClickDlg::OnEnChangeEdit5()
{
	CString num;
	loop_ipt.GetWindowText(num);
	loop = (UINT)_ttoi(num);
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
	UnregisterHotKey(m_hWnd, 0x124); 
	RegisterHotKey(m_hWnd, 0x124, wModifiers, wVirtualKeyCode);
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
		if (pointInfo[row].event_type == 1) {
			CMenu menu;
			menu.LoadMenu(IDR_MENU1);

			CMenu* popmenu;
			popmenu = menu.GetSubMenu(0);
			popmenu->TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, this);
		}
		// 加载设置键位的菜单
		else if (pointInfo[row].event_type == 2) {
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
	if (select_row >= 0 && select_row < pointInfo.size()) {
		pointInfo.erase(pointInfo.begin() + select_row); // 删除第select_row个元素
	}
	list.DeleteItem(select_row);
}

void CiClickDlg::OnDeleteAll()
{
	// TODO: 在此添加命令处理程序代码
	pointInfo.clear(); // 删除所有元素
	list.DeleteAllItems(); // 删除所有行
}

void CiClickDlg::OnBnClickedCheck1()
{
		if (random_check.GetCheck() == TRUE) {
			blurry_ipt.EnableWindow(TRUE);
			isRandomClick = TRUE;
		}
		else {
			blurry_ipt.EnableWindow(FALSE);
			isRandomClick = FALSE;
		}
}

void CiClickDlg::OnEnChangeEdit1()
{
	CString str;
	blurry_ipt.GetWindowTextW(str);
	UINT num = (UINT)_ttoi(str);
	Random_Radius = num;
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
			if (need_hide==TRUE) {
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

		list.InsertItem(iRow, x);
		list.SetItemText(iRow, 1, y);
		if (Event_Type == 1) {
			list.SetItemText(iRow, 2, L"鼠标");
			list.SetItemText(iRow, 3, L"左键单击");
		}
		else if (Event_Type == 2) {
			list.SetItemText(iRow, 2, L"键盘");
			list.SetItemText(iRow, 3, L"");
		}
		list.SetItemText(iRow, 4, str);
		list.SetItemText(iRow, 5, _T("0"));

		PointInfo pI;
		pI.x = ptCursor.x;
		pI.y = ptCursor.y;
		pI.hwnd = hWnd->m_hWnd;
		pI.event_type = Event_Type;
		pI.gap = 0;
		if (Event_Type==1) {
			pI.moust_key = 1;
		}
		else if(Event_Type == 2) {
			//pI.keybord_key = "A";
		}
		pointInfo.push_back(pI);


		// 恢复显示主窗口
		if (need_hide == TRUE) {
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
	need_hide = hide;
}

// 修改事件类型为鼠标事件
void CiClickDlg::OnBnClickedRadio3()
{
	if (IsDlgButtonChecked(IDC_RADIO3) == BST_CHECKED) {
		Event_Type = 1;
	}
}
// 修改事件类型为键盘事件
void CiClickDlg::OnBnClickedRadio4()
{
	if (IsDlgButtonChecked(IDC_RADIO4) == BST_CHECKED) {
		Event_Type = 2;
	}
}

void CiClickDlg::ChangeToSingleClick()
{
	// TODO: 修改点击方式为单击
	pointInfo[select_row].moust_key = 1;
	list.SetItemText(select_row, 3, L"左键单击");

}

void CiClickDlg::ChangeToDoubleClick()
{
	// TODO: 修改点击方式为双击
	pointInfo[select_row].moust_key = 2;
	list.SetItemText(select_row, 3, L"左键双击");
}


void CiClickDlg::DeleteSingleRow()
{
	// 删除单行
	if (select_row >= 0 && select_row < pointInfo.size()) {
		pointInfo.erase(pointInfo.begin() + select_row); // 删除第select_row个元素
	}
	list.DeleteItem(select_row);
}

void CiClickDlg::DeleteAllRow()
{
	// 删除所有元素
	pointInfo.clear(); // 删除所有元素
	list.DeleteAllItems(); // 删除所有行
}


void CiClickDlg::OpenKeySelectDlg()
{
		// 打开键位选择模态框
	Key_Select keySelect;
	if (keySelect.DoModal() == IDOK) { // 阻塞直到模态框关闭[7](@ref)
		pointInfo[select_row].hotKeyInfo = keySelect.m_hotKeyInfo;
		list.SetItemText(select_row, 3, keySelect.m_hotKeyInfo.strDisplay);
	}
}

void CiClickDlg::OpenGapModal()
{
	// TODO: 在此添加命令处理程序代码
	GapModal gapModal(NULL, pointInfo[select_row].gap);

	if (gapModal.DoModal() == IDOK) { // 阻塞直到模态框关闭[7](@ref)
		pointInfo[select_row].gap = gapModal.gap;
		CString str;
		str.Format(_T("%d"), gapModal.gap);
		list.SetItemText(select_row, 5, str);
	}
}

void CiClickDlg::OpenGapDialog1()
{
	// TODO: 在此添加命令处理程序代码
	GapModal gapModal(NULL, pointInfo[select_row].gap);
	if (gapModal.DoModal() == IDOK) { // 阻塞直到模态框关闭[7](@ref)
		pointInfo[select_row].gap = gapModal.gap;
		CString str;
		str.Format(_T("%d"), gapModal.gap);
		list.SetItemText(select_row, 5, str);
	}
}

void CiClickDlg::OnBnClickedCheck5()
{
	// TODO: 在此添加控件通知处理程序代码
	isFrontOpt = isfront_check.GetCheck();
}

void CiClickDlg::OnEnChangeEdit6()
{
	loop_times = GetDlgItemInt(IDC_EDIT6, NULL, FALSE);
}

// 修改为键盘事件
void CiClickDlg::ChangeToKeyBd()
{
	pointInfo[select_row].event_type = 2;
	list.SetItemText(select_row, 2, L"键盘");
	list.SetItemText(select_row, 3, L"");
}

// 修改为鼠标事件
void CiClickDlg::ChangeToMouse()
{
	pointInfo[select_row].event_type = 1;
	pointInfo[select_row].moust_key = 1;
	list.SetItemText(select_row, 2, L"鼠标");
	list.SetItemText(select_row, 3, L"左键单击");
}

// 修改为滚轮上滚
void CiClickDlg::ChangeToMidUp()
{
	pointInfo[select_row].moust_key = 3;
	list.SetItemText(select_row, 3, L"滚轮上滚");
}

// 修改为滚轮下滚
void CiClickDlg::ChangeToMidDown()
{
	pointInfo[select_row].moust_key = 4;
	list.SetItemText(select_row, 3, L"滚轮下滚");
}


// 修改为滚轮单击
void CiClickDlg::ChangeToMidClick()
{
	pointInfo[select_row].moust_key = 5;
	list.SetItemText(select_row, 3, L"滚轮单击");
}

// 修改为右键单击
void CiClickDlg::ChangeToRightClick()
{
	pointInfo[select_row].moust_key = 6;
	list.SetItemText(select_row, 3, L"右键单击");
}

// 修改为右键双击
void CiClickDlg::ChangeToRightDbClick()
{
	pointInfo[select_row].moust_key = 7;
	list.SetItemText(select_row, 3, L"右键双击");
}

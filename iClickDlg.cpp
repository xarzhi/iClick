
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CiClickDlg 对话框



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
	start_hotkey.SetHotKey(VK_F7,NULL);
	RegisterHotKey(m_hWnd, 0x124, NULL, VK_F7);


	random_check.SetCheck(isRandomClick);

	// 默认禁用模糊点击范围设置
	blurry_ipt.EnableWindow(FALSE);

	// 表格选中行默认为-1，未选择任何行
	select_row = -1;

	// 初始化单选按钮
	((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(TRUE); //选上
	((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(FALSE);//不选上

	//// 给图片空间设置位图
	CBitmap m_bmp;
	m_bmp.LoadBitmap(IDB_BITMAP1);
	CStatic* pPic = (CStatic*)GetDlgItem(IDC_STATIC_PIC);
	pPic->SetWindowPos(NULL, 0, 0, 32,32, SWP_NOMOVE | SWP_NOZORDER);
	pPic->SetBitmap(m_bmp);
	m_bmp.Detach(); // 关键！防止bmp析构时删除位图



	loop_edit.SetWindowTextW(_T("0"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}





void CiClickDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
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
	}
	else {
		 UnregisterHotKey(m_hWnd, 0x123);
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

// 多线程触发事件
UINT MyThreadFunction(LPVOID pParam)
{
	CiClickDlg* Wnd = (CiClickDlg*)pParam;
	UINT loop_times= Wnd->loop_times;
	
	while (Wnd->isClick) {
		for (const auto& point : pointInfo) {
			Sleep(point.gap);			// 延迟

			if (point.event_type == 1) {
				// 处理鼠标事件
				UINT num = Wnd->Random_Radius;
				int x, y;
				if (Wnd->isRandomClick) {
					x = GetRand(point.x - num, point.x + num);
					y = GetRand(point.y - num, point.y + num);
				}
				else {
					x = point.x;
					y = point.y;
				}
				if (point.moust_key == 1) {
					// 单机
					::SendMessage(point.hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x, y));
					::SendMessage(point.hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(x, y));
				}
				else if (point.moust_key == 2) {
					// 双击
					 // 第一次点击
					::SendMessage(point.hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x, y));
					::SendMessage(point.hwnd, WM_LBUTTONUP, 0, MAKELPARAM(x, y));
					// 短暂延迟（模拟用户双击速度）
					Sleep(GetDoubleClickTime() / 2);
					// 第二次点击（双击）
					::SendMessage(point.hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x, y));
					::SendMessage(point.hwnd, WM_LBUTTONDBLCLK, MK_LBUTTON, MAKELPARAM(x, y));
					::SendMessage(point.hwnd, WM_LBUTTONUP, 0, MAKELPARAM(x, y));
				}
				
			}
			else if (point.event_type == 2) {
				// 处理键盘事件
				::SetForegroundWindow(point.hwnd);  // 确保目标窗口在前台

				// ******************* keybd_event *****************
				// 按下修饰符键（如果有）
				if (point.hotKeyInfo.wModifiers & HOTKEYF_CONTROL) {
					keybd_event(VK_CONTROL, 0, 0, 0);        // 按下 Ctrl
				}
				if (point.hotKeyInfo.wModifiers & HOTKEYF_SHIFT) {
					keybd_event(VK_SHIFT, 0, 0, 0);        // 按下 Shift
				}
				if (point.hotKeyInfo.wModifiers & HOTKEYF_ALT) {
					keybd_event(VK_MENU, 0, 0, 0);        // 按下 ALT
				}
				keybd_event(point.hotKeyInfo.wVirtualKey, 0, 0, 0);             
				keybd_event(point.hotKeyInfo.wVirtualKey, 0, KEYEVENTF_KEYUP, 0); 

				if (point.hotKeyInfo.wModifiers & HOTKEYF_ALT) {
					keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);   // 松开 ALT
				}
				if (point.hotKeyInfo.wModifiers & HOTKEYF_SHIFT) {
					keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);  // 松开 SHIFT
				}
				if (point.hotKeyInfo.wModifiers & HOTKEYF_CONTROL) {
					keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);  // 松开 Ctrl
				}

			
				// ******************* SendMessage *****************
			/*	if (point.hotKeyInfo.wModifiers & HOTKEYF_CONTROL) {
					::SendMessage(point.hwnd, WM_KEYDOWN, VK_CONTROL, lParamDown);
				}
				if (point.hotKeyInfo.wModifiers & HOTKEYF_SHIFT) {
					::SendMessage(point.hwnd, WM_KEYDOWN, VK_SHIFT, lParamDown);
				}
				if (point.hotKeyInfo.wModifiers & HOTKEYF_ALT) {
					::SendMessage(point.hwnd, WM_KEYDOWN, VK_MENU, lParamDown);
				}
				::SendMessage(point.hwnd, WM_KEYDOWN, point.hotKeyInfo.wVirtualKey, lParamDown);
				::SendMessage(point.hwnd, WM_KEYUP, point.hotKeyInfo.wVirtualKey, lParamUp);
				if (point.hotKeyInfo.wModifiers & HOTKEYF_ALT) {
					::SendMessage(point.hwnd, WM_KEYUP, VK_MENU, lParamUp);
				}
				if (point.hotKeyInfo.wModifiers & HOTKEYF_SHIFT) {
					::SendMessage(point.hwnd, WM_KEYUP, VK_SHIFT, lParamUp);
				}
				if (point.hotKeyInfo.wModifiers & HOTKEYF_CONTROL) {
					::SendMessage(point.hwnd, WM_KEYUP, VK_CONTROL, lParamUp);
				}*/
			}

			Sleep(Wnd->gap);			// 单次操作间隔
		}
		Sleep(Wnd->loop);				// 每一轮间隔
		if (Wnd->loop_times != 0) {
			loop_times--;
			if (loop_times == 0) {
				Wnd->isClick = false;
				Wnd->start_btn.SetWindowTextW(_T("开始点击"));
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
	loop_times = GetDlgItemInt(IDC_EDIT6, NULL, FALSE);
	
	if (isClick == TRUE) { 
		start_btn.SetWindowTextW(_T("开始点击"));
		isClick = FALSE;
	} 
	else {
		start_btn.SetWindowTextW(_T("停止点击"));
		isClick = TRUE;
		AfxBeginThread(MyThreadFunction, this);
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
	if (nHotKeyId == 0x123) {
		// TODO: 在此添加消息处理程序代码和/或调用默认值
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

		// 同步到数组
		PointInfo pI;
		pI.x = ptCursor.x;
		pI.y = ptCursor.y;
		pI.hwnd = hWnd->m_hWnd;
		pI.event_type = Event_Type;
		pI.gap = 0;
		if (Event_Type == 1) {
			pI.moust_key = 1;
		}
		
		pointInfo.push_back(pI);
	
	}
	else if (nHotKeyId == 0x124) {
		if (pointInfo.empty()) {
			MessageBox(_T("请添加坐标信息"));
			return;
		}

		if (isClick == TRUE) {
			start_btn.SetWindowTextW(_T("开始点击"));
			isClick = FALSE;
		}
		else {
			start_btn.SetWindowTextW(_T("停止点击"));
			isClick = TRUE;
			AfxBeginThread(MyThreadFunction, this);
		}
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

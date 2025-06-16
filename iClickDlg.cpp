
// iClickDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "iClick.h"
#include "iClickDlg.h"
#include "afxdialogex.h"
#include <vector>
using namespace std;

typedef struct PointInfo {
	int x;
	int y;
	HWND hwnd;

}PointInfo;


vector<PointInfo> pointInfo;
//PointInfo point1 = { 10, 20, nullptr };
//pointInfo.push_back(point1);

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
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_HOTKEY() //添加消息宏
	ON_WM_HOTKEY()
	ON_NOTIFY(NM_THEMECHANGED, IDC_HOTKEY3, &CiClickDlg::OnNMThemeChangedHotkey3)
	ON_EN_CHANGE(IDC_EDIT5, &CiClickDlg::OnEnChangeEdit5)
	ON_EN_CHANGE(IDC_HOTKEY3, &CiClickDlg::OnHotKeyChanged)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CiClickDlg::OnNMRClickList1)
	ON_COMMAND(ID_32773, &CiClickDlg::OnMenuRClick)
	ON_COMMAND(ID_32774, &CiClickDlg::OnDeleteAll)
	ON_BN_CLICKED(IDC_CHECK1, &CiClickDlg::OnBnClickedCheck1)
	ON_EN_CHANGE(IDC_EDIT1, &CiClickDlg::OnEnChangeEdit1)
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

	CRect rect;
	list.GetClientRect(&rect);
	int width = rect.Width() / 5;
	list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);	// 整行选择、网格线
	list.InsertColumn(0, _T("X坐标"), LVCFMT_LEFT, width);	// 插入第2列的列名
	list.InsertColumn(1, _T("Y坐标"), LVCFMT_LEFT, width);	// 插入第3列的列名
	list.InsertColumn(2, _T("鼠标按键"), LVCFMT_LEFT, width);	// 插入第4列的列名
	list.InsertColumn(3, _T("点击方式"), LVCFMT_LEFT, width);	// 插入第5列的列名
	list.InsertColumn(4, _T("操作"), LVCFMT_LEFT, width-20);	// 插入第6列的列名

	// 为列表视图控件添加全行选中和栅格风格   
	list.SetExtendedStyle(list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	//注册热键 F6
	hotkey1.SetHotKey(watch_hotkey, NULL);

	// 点击间隔edit初始化值
	gap_ipt.SetWindowTextW(_T("20"));
	loop_ipt.SetWindowTextW(_T("0"));

	CString Random_Radius_Str;
	Random_Radius_Str.Format(_T("%d"), Random_Radius);
	blurry_ipt.SetWindowTextW(Random_Radius_Str);

	setOnTop_Check.SetCheck(TRUE);
	this->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	start_hotkey.SetHotKey(VK_F3,NULL);
	RegisterHotKey(m_hWnd, 0x124, NULL, VK_F3);

	random_check.SetCheck(isRandomClick);

	blurry_ipt.EnableWindow(FALSE);
	select_row = -1;



	//CImage cim;
	//cim.Load(_T(""));
	//HBITMAP hbmp = cim.Detach();
	//pic_box.SetBitmap(hbmp);


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

void CiClickDlg::OnBnClickedCheck2()
{
	if (start_Watch_Check.GetCheck() == TRUE) {
		RegisterHotKey(m_hWnd, 0x123, NULL, watch_hotkey);
	}
	else {
		 UnregisterHotKey(m_hWnd, 0x123);
	}
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

UINT MyThreadFunction(LPVOID pParam)
{
	CiClickDlg* Wnd = (CiClickDlg*)pParam;
	while (Wnd->isClick) {
		for (const auto& point : pointInfo) {
			UINT num=Wnd->Random_Radius;
			int x, y;
			if (Wnd->isRandomClick) {
				 x = GetRand(point.x - num, point.x + num);
				 y = GetRand(point.y - num, point.y + num);
			}
			else {
				x = point.x;
				y = point.y;
			}
			::SendMessage(point.hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x, y));
			::SendMessage(point.hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(x, y));
			Sleep(Wnd->gap);
		}
		Sleep(Wnd->loop);
	}
	return 0; // 线程退出码
}


void CiClickDlg::OnBnClickedButton1()
{
	if (pointInfo.empty()) return;

	if (isClick == TRUE) { 
		start_btn.SetWindowTextW(_T("开始点击"));
		isClick = FALSE;
	} 
	else {
		start_btn.SetWindowTextW(_T("停止点击"));
		isClick = TRUE;
	}
	AfxBeginThread(MyThreadFunction, this);
}


// 设置窗口是否置顶
void CiClickDlg::OnBnClickedCheck3()
{
	if (setOnTop_Check.GetCheck() ==  TRUE) {
		this->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}
	else {
		this->SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}
}


void CiClickDlg::OnMouseMove(UINT nFlags, CPoint point) {
	if (nFlags == MK_LBUTTON) {
		//MessageBox(_T("ASD"));
	}
	CString str;
	str.Format(_T("X: %d, Y: %d"), point.x, point.y);

	CDialogEx::OnMouseMove(nFlags, point);
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
		CPoint ptCursor;
		GetCursorPos(&ptCursor);//获取鼠标位置

		CWnd* hWnd = WindowFromPoint(ptCursor); // 获取窗口句柄

		::ScreenToClient(hWnd->m_hWnd, &ptCursor);
		CString x, y;
		x.Format(_T("%d"), ptCursor.x);
		y.Format(_T("%d"), ptCursor.y);
		int iRow = list.GetItemCount(); //获取行数
		list.InsertItem(iRow, x);
		list.SetItemText(iRow, 1, y);
		pointInfo.push_back({ ptCursor.x,ptCursor.y,hWnd->m_hWnd });
		// 窗口标题
		CString str;
		hWnd->GetWindowTextW(str);
		wnd_title_ipt.SetWindowTextW(str);

		// 窗口句柄
		CString str2;
		CString strHandle = GetWindowHandleDecimal(hWnd);
		hwnd_ipt.SetWindowTextW(strHandle);
	}
	else if (nHotKeyId == 0x124) {
		if (pointInfo.empty()) return;

		if (isClick == TRUE) {
			start_btn.SetWindowTextW(_T("开始点击"));
			isClick = FALSE;
		}
		else {
			start_btn.SetWindowTextW(_T("停止点击"));
			isClick = TRUE;
		}
		AfxBeginThread(MyThreadFunction, this);
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

void CiClickDlg::OnHotKeyChanged()
{
	WORD wVirtualKeyCode;
	WORD wModifiers;
	hotkey1.GetHotKey(wVirtualKeyCode, wModifiers); // 获取键码和修饰符


	UnregisterHotKey(m_hWnd, 0x123);
	RegisterHotKey(m_hWnd, 0x123, NULL, wVirtualKeyCode);

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
		CMenu menu; 
		menu.LoadMenu(IDR_MENU1);

		CMenu* popmenu;
		popmenu = menu.GetSubMenu(0);
		popmenu->TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, this);
	}

	*pResult = 0;
}


void CiClickDlg::OnMenuRClick()
{
	// TODO: 在此添加命令处理程序代码
	if (select_row >= 0 && select_row < pointInfo.size()) {
		pointInfo.erase(pointInfo.begin() + select_row); // 删除第3个元素（30）
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

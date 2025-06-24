// Key_Select.cpp: 实现文件
//
#include "pch.h"

#include "iClick.h"
#include "afxdialogex.h"
#include "Key_Select.h"

// Key_Select 对话框

IMPLEMENT_DYNAMIC(Key_Select, CDialogEx)



Key_Select::Key_Select(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KEYSELECT, pParent)
{

#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif
	m_hotKeyInfo = { 0, 0, _T("") };
	EnableAutomation();

}

Key_Select::~Key_Select()
{
}


BOOL Key_Select::OnInitDialog()
{
    CDialogEx::OnInitDialog();
 
    m_hotKeyInfo = {};
    m_hotkey.SetFocus();
    return FALSE;
}








void Key_Select::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。  基类将自动
	// 删除该对象。  在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

	CDialogEx::OnFinalRelease();
}

void Key_Select::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HOTKEY2, m_hotkey);
}


BEGIN_MESSAGE_MAP(Key_Select, CDialogEx)
END_MESSAGE_MAP()


static const IID IID_IKey_Select =
{0x8c97c43c,0x6ee5,0x49aa,{0xb7,0x3b,0xbe,0x44,0xf9,0x0d,0x39,0xdd}};



void Key_Select::OnOK() {
	CHotKeyCtrl* pHotKey = (CHotKeyCtrl*)GetDlgItem(IDC_HOTKEY2);
	pHotKey->GetHotKey(m_hotKeyInfo.wVirtualKey, m_hotKeyInfo.wModifiers);

	m_hotKeyInfo.strDisplay = ConvertToDisplayString();

	EndDialog(IDOK); // 关闭对话框并返回IDOK
}

CString Key_Select::ConvertToDisplayString() {
    CString strModifiers;
    // 处理修饰键（Ctrl/Alt/Shift）
    if (m_hotKeyInfo.wModifiers & HOTKEYF_CONTROL) strModifiers += _T("Ctrl+");
    if (m_hotKeyInfo.wModifiers & HOTKEYF_SHIFT)   strModifiers += _T("Shift+");
    if (m_hotKeyInfo.wModifiers & HOTKEYF_ALT)     strModifiers += _T("Alt+");

    UINT vk = m_hotKeyInfo.wVirtualKey;
    CString strKey;
    if (vk == VK_SPACE) {
        strKey = _T("Space"); // 空格键显示为 "Space"
    }
    else if (vk == VK_RETURN) {
        strKey = _T("Enter"); // 回车键显示为 "Enter"
    }
    // 1. 功能键 F1-F12（范围 0x70-0x7B）
    else if (vk >= VK_F1 && vk <= VK_F24) {
        strKey.Format(_T("F%d"), vk - VK_F1 + 1);
    }
    // 2. 小键盘数字键（范围 0x60-0x69）
    else if (vk >= VK_NUMPAD0 && vk <= VK_NUMPAD9) {
        strKey.Format(_T("Num %d"), vk - VK_NUMPAD0); // 显示为 "Num 1"
    }
    // 3. 主键盘数字键（范围 0x30-0x39）
    else if (vk >= '0' && vk <= '9') {
        strKey = (TCHAR)vk; // 直接显示数字
    }
    // 4. 字母键（范围 0x41-0x5A）
    else if (vk >= 'A' && vk <= 'Z') {
        strKey = (TCHAR)vk; // 直接显示字母
    }
    // 5. 特殊按键（预定义映射）
    else {
        switch (vk) {
            // 方向键
        case VK_UP:     strKey = _T("↑"); break;
        case VK_DOWN:   strKey = _T("↓"); break;
        case VK_LEFT:   strKey = _T("←"); break;
        case VK_RIGHT:  strKey = _T("→"); break;
            // 功能键
        case VK_ESCAPE: strKey = _T("Esc"); break;
        case VK_RETURN: strKey = _T("Enter"); break;
        case VK_SPACE:  strKey = _T("Space"); break;
        case VK_TAB:    strKey = _T("Tab"); break;
        case VK_BACK:   strKey = _T("Backspace"); break;
        case VK_DELETE: strKey = _T("Delete"); break;
        case VK_HOME:   strKey = _T("Home"); break;
        case VK_END:    strKey = _T("End"); break;
        case VK_PRIOR:  strKey = _T("PageUp"); break;
        case VK_NEXT:   strKey = _T("PageDown"); break;
            // 其他未定义键：用系统API获取名称
        default:
            TCHAR szKeyName[32];
            UINT scanCode = MapVirtualKey(vk, MAPVK_VK_TO_VSC) << 16;
            if (GetKeyNameText(scanCode, szKeyName, 32) > 0) {
                strKey = szKeyName;
            }
            else {
                strKey.Format(_T("0x%X"), vk); // 显示原始键码
            }
        }
    }
    return strModifiers + strKey;
}


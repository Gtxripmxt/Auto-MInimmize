#include <Geode/Geode.hpp>
#include <Windows.h>

using namespace geode::prelude;

HHOOK g_keyboardHook = nullptr;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* pKeyInfo = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        
        if (wParam == WM_KEYDOWN && (pKeyInfo->vkCode == VK_LWIN || pKeyInfo->vkCode == VK_RWIN)) {
            HWND hWnd = FindWindow(L"CocosDenshion", nullptr);
            if (!hWnd) {
                hWnd = FindWindow(L"GLFW30", nullptr);
            }
            
            if (hWnd) {
                if (GetForegroundWindow() == hWnd) {
                    ShowWindow(hWnd, SW_MINIMIZE);
                    return 1;
                }
            }
        }
    }
    
    return CallNextHookEx(g_keyboardHook, nCode, wParam, lParam);
}

$on_mod(Loaded) {
    g_keyboardHook = SetWindowsHookEx(
        WH_KEYBOARD_LL,
        LowLevelKeyboardProc,
        GetModuleHandle(NULL),
        0
    );
    
    if (g_keyboardHook) {
        log::info("Yippie sussces");
    } else {
        log::error("Error idk", GetLastError());
    }
    
    return true;
}

// Clean up hook when the mod is unloaded
$on_mod(Unloaded) {
    if (g_keyboardHook) {
        UnhookWindowsHookEx(g_keyboardHook);
        g_keyboardHook = nullptr;
        log::info("Win Minimizer mod: Keyboard hook removed");
    }
    
    return true;
}

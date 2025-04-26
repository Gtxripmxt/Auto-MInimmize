#include <Geode/Geode.hpp>
#include <Windows.h>

using namespace geode::prelude;

HHOOK g_keyboardHook = nullptr;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* pKeyInfo = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        
        if (wParam == WM_KEYDOWN && (pKeyInfo->vkCode == VK_LWIN || pKeyInfo->vkCode == VK_RWIN)) {
            HWND hWnd = FindWindowA("CocosDenshion", nullptr);
            if (!hWnd) {
                hWnd = FindWindowA("GLFW30", nullptr);
            }
            if (!hWnd) {
                hWnd = FindWindowA("GDWindowClass", nullptr);
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

class AutoMinimizeMod : public geode::Mod {
    void onEnable(geode::ModLoadEvent*) override {
        g_keyboardHook = SetWindowsHookEx(
            WH_KEYBOARD_LL,
            LowLevelKeyboardProc,
            GetModuleHandle(NULL),
            0
        );
        
        if (g_keyboardHook) {
            log::info("Auto Minimizer mod: Keyboard hook installed successfully");
        } else {
            log::error("Auto Minimizer mod: Failed to install keyboard hook. Error code: {}", GetLastError());
        }
    }
    
    void onDisable() override {
        if (g_keyboardHook) {
            UnhookWindowsHookEx(g_keyboardHook);
            g_keyboardHook = nullptr;
            log::info("Auto Minimizer mod: Keyboard hook removed");
        }
    }
};

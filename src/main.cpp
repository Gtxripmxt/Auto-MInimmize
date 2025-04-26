#include <Geode/Geode.hpp>
#include <Geode/modify/CCEGLView.hpp>
#include <Windows.h>

using namespace geode::prelude;

WNDPROC g_originalWndProc = nullptr;

LRESULT CALLBACK CustomWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_KEYDOWN) {
        if (wParam == VK_LWIN || wParam == VK_RWIN) {
            ShowWindow(hWnd, SW_MINIMIZE);
            return 0;
        }
    }
    
    return CallWindowProc(g_originalWndProc, hWnd, message, wParam, lParam);
}

class $modify(CCEGLView) {
    bool $modify(initWithRect)(RECT rect, float scaleFactor, bool isFullscreen) {
        auto result = this->$original(rect, scaleFactor, isFullscreen);
        
        if (result) {
            HWND hWnd = nullptr;
            
            hWnd = static_cast<HWND>(CCDirector::sharedDirector()->getOpenGLView()->getWindow());
            
            if (hWnd) {
                g_originalWndProc = reinterpret_cast<WNDPROC>(
                    SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(CustomWndProc))
                );
                
                log::info("Win Minimizer mod initialized: Window procedure hook established");
            } else {
                log::error("Win Minimizer mod: Failed to get window handle");
            }
        }
        
        return result;
    }
};

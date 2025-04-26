#include <Geode/Geode.hpp>
#include <Geode/modify/CCEGLView.hpp>
#include <Windows.h>

using namespace geode::prelude;

class $modify(CCEGLViewWin) CCEGLView {
private:
    WNDPROC m_oldWndProc;
    
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        auto self = reinterpret_cast<CCEGLViewWin*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        
        if (message == WM_KEYDOWN) {
            if (wParam == VK_LWIN || wParam == VK_RWIN) {
                ShowWindow(hWnd, SW_MINIMIZE);
                return 0;
            }
        }
        

        return CallWindowProc(self->m_oldWndProc, hWnd, message, wParam, lParam);
    }

public:
    bool $modify(initWithRect) initWithRect(RECT rect, float, bool isFullscreen) {
        if (!$original(rect, 1.0f, isFullscreen)) {
            return false;
        }
        
        HWND hWnd = this->getWin32Window();
        
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
        
        m_oldWndProc = reinterpret_cast<WNDPROC>(
            SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc))
        );
        
        log::info("Win Minimizer mod initialized");
        return true;
    }
};


$execute {
    log::info("Win Minimizer mod loaded");
}

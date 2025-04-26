#include <Geode/Geode.hpp>
#include <Windows.h>

using namespace geode::prelude;

namespace {
    void checkForWindowsKey(float) {
        if (isFullscreenBorderless()) {
            if (GetAsyncKeyState(VK_LWIN) & 0x8000 || GetAsyncKeyState(VK_RWIN) & 0x8000) {
                minimizeWindow();
            }
        }
    }

    bool isFullscreenBorderless() {
        auto settings = GameManager::sharedState();
        bool isFullscreen = settings->getGameVariable("0010"); // fullscreen
        bool isBorderless = settings->getGameVariable("0040"); // borderless
        return isFullscreen && isBorderless;
    }

    void minimizeWindow() {
        HWND hwnd = GetActiveWindow();
        if (hwnd) {
            ShowWindow(hwnd, SW_MINIMIZE);
        }
    }
}

$on_mod(Loaded) {
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(
        schedule_selector(checkForWindowsKey),
        CCDirector::sharedDirector(),
        0.1f,
        false
    );
}

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class WindowMinimizerMod : public GeodeMod {
public:
    WindowMinimizerMod() : GeodeMod("WindowMinimizerMod") {}

    void onEnable() override {
        Hook::create("OnWindowsKeyPress", &WindowMinimizerMod::onWindowsKeyPress, this);
    }

    static void onWindowsKeyPress() {
        if (isFullscreenBorderless()) {
            minimizeWindow();
        }
    }

    static bool isFullscreenBorderless() {
        return true;
    }

    static void minimizeWindow() {
        HWND hwnd = GetActiveWindow();
        ShowWindow(hwnd, SW_MINIMIZE);
    }
};

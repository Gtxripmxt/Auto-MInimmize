#define UNICODE
#define _UNICODE

#include <Geode/Geode.hpp>
#include <Windows.h>

using namespace geode::prelude;

// Helper: check if Windows Start Menu is open
bool isStartMenuOpen() {
    HWND startMenu = FindWindowW(L"Windows.UI.Core.CoreWindow", nullptr);
    return startMenu != nullptr;
}

class WinKeyMinimizer : public CCNode {
public:
    static WinKeyMinimizer* create() {
        auto ret = new WinKeyMinimizer();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    bool init() override {
        if (!CCNode::init())
            return false;

        this->scheduleUpdate();
        m_minimized = false;
        return true;
    }

    void update(float) override {
        auto settings = GameManager::sharedState();
        bool isFullscreen = settings->getGameVariable("0123");
        bool isBorderless = settings->getGameVariable("0124");

        if (isFullscreen && isBorderless) {
            if (isStartMenuOpen() && !m_minimized) {
                HWND window = GetActiveWindow();
                if (window) {
                    ShowWindow(window, SW_MINIMIZE);
                    m_minimized = true;
                }
            } else if (!isStartMenuOpen()) {
                m_minimized = false;
            }
        }
    }

private:
    bool m_minimized;
};

$on_mod(Loaded) {
    CCDirector::sharedDirector()->getRunningScene()->addChild(WinKeyMinimizer::create(), INT_MAX);
}

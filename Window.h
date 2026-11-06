#pragma once
#include <Windows.h>

class Window {
public:
    static const int kWindowWidth = 1280;
    static const int kWindowHeight = 720;
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    void Initialize();
    bool ProcessMessage();
    void Finalize();
    HWND GetHwnd() const { return hwnd_; }
    HINSTANCE GetHInstance() const { return hInstance_; }
private:
    HWND hwnd_ = nullptr;
    WNDCLASSEX w_{};
    HINSTANCE hInstance_ = nullptr;
};

#include "Window.h"

// ウィンドウプロシージャの定義
LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
    case WM_DESTROY:
        // ウィンドウが閉じられたらアプリ終了を通知
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Window::Initialize() {
    hInstance_ = GetModuleHandle(nullptr);

    // 1. ウィンドウクラスの設定
    w_.cbSize = sizeof(WNDCLASSEX);
    w_.lpfnWndProc = WindowProc;
    w_.lpszClassName = L"CG2_WindowClass";
    w_.hInstance = hInstance_;
    w_.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassEx(&w_);

    // 2. ウィンドウサイズの調整
    RECT wrc = { 0, 0, kWindowWidth, kWindowHeight };
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, FALSE);

    // 3. ウィンドウの生成
    hwnd_ = CreateWindow(
        w_.lpszClassName,
        L"CG2_Master",                           // タイトルバー名
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        wrc.right - wrc.left,
        wrc.bottom - wrc.top,
        nullptr,
        nullptr,
        w_.hInstance,
        nullptr
    );

    // 4. ウィンドウ表示
    ShowWindow(hwnd_, SW_SHOW);
    UpdateWindow(hwnd_);
}

bool Window::ProcessMessage() {
    MSG msg{};
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // アプリ終了要求が来たら true
    if (msg.message == WM_QUIT) {
        return true;
    }
    return false;
}

void Window::Finalize() {
    UnregisterClass(w_.lpszClassName, w_.hInstance);
}
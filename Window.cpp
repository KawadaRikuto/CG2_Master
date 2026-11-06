#include "Window.h"
#ifdef USE_IMGUI
#include "externals/imgui/imgui_impl_win32.h"
#endif

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
#ifdef USE_IMGUI
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
        return 1;
    }
#endif
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Window::Initialize() {
    hInstance_ = GetModuleHandle(nullptr);
    w_.cbSize = sizeof(WNDCLASSEX);
    w_.lpfnWndProc = WindowProc;
    w_.lpszClassName = L"CG2_WindowClass";
    w_.hInstance = hInstance_;
    w_.hCursor = LoadCursor(nullptr, IDC_ARROW);
    RegisterClassEx(&w_);
    RECT wrc = { 0, 0, kWindowWidth, kWindowHeight };
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, FALSE);
    hwnd_ = CreateWindow(w_.lpszClassName, L"CG2_Master", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, wrc.right - wrc.left, wrc.bottom - wrc.top,
        nullptr, nullptr, w_.hInstance, nullptr);
    ShowWindow(hwnd_, SW_SHOW);
    UpdateWindow(hwnd_);
}

bool Window::ProcessMessage() {
    MSG msg{};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) return true;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return false;
}

void Window::Finalize() {
    if (hwnd_) { DestroyWindow(hwnd_); hwnd_ = nullptr; }
    UnregisterClass(w_.lpszClassName, w_.hInstance);
}

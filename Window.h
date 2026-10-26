#pragma once
#include <Windows.h>

class Window {
public:
    // ウィンドウサイズ定数
    static const int kWindowWidth = 1280;
    static const int kWindowHeight = 720;

    // ウィンドウプロシージャ（静的メンバ関数）
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    // 初期化（ウィンドウ生成・表示）
    void Initialize();

    // メッセージ処理（WM_QUITを受け取った場合 true を返す）
    bool ProcessMessage();

    // 終了・解放処理
    void Finalize();

    // ゲッター（DirectX初期化等で使用）
    HWND GetHwnd() const { return hwnd_; }
    HINSTANCE GetHInstance() const { return hInstance_; }

private:
    HWND hwnd_ = nullptr;
    WNDCLASSEX w_{};
    HINSTANCE hInstance_ = nullptr;
};
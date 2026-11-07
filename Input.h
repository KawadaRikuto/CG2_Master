#pragma once
#include <Windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class Window;

class Input {
public:
    template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

    // 初期化
    void Initialize(Window* window);
    void Update();
    bool PushKey(BYTE keyNumber);
    bool TriggerKey(BYTE keyNumber);

private:
    BYTE key[256] = {};
    BYTE prevKey[256] = {};
    ComPtr<IDirectInput8> directInput;
    ComPtr<IDirectInputDevice8> keyboard;
};

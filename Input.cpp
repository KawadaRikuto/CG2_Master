#include "Input.h"
#include "Window.h"
#include <cassert>
#include <cstring>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

void Input::Initialize(Window* window) {
    assert(window != nullptr);
    HINSTANCE hInstance = window->GetHInstance();
    HWND hwnd = window->GetHwnd();
    assert(hInstance != nullptr && hwnd != nullptr);

    HRESULT result;
    result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)directInput.GetAddressOf(), nullptr);
    assert(SUCCEEDED(result));
    result = directInput->CreateDevice(GUID_SysKeyboard, keyboard.GetAddressOf(), nullptr);
    assert(SUCCEEDED(result));
    result = keyboard->SetDataFormat(&c_dfDIKeyboard);
    assert(SUCCEEDED(result));
    result = keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    assert(SUCCEEDED(result));
}

void Input::Update() {
    memcpy(prevKey, key, sizeof(key));
    keyboard->Acquire();
    keyboard->GetDeviceState(sizeof(key), key);
}

bool Input::PushKey(BYTE keyNumber) {
    return key[keyNumber] != 0;
}

bool Input::TriggerKey(BYTE keyNumber) {
    return key[keyNumber] && !prevKey[keyNumber];
}

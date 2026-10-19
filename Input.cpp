#include "Input.h"
#include<cassert>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

void Input::Initialize(HINSTANCE hInstance, HWND hwnd) {

	HRESULT result;

	//DirectInputの初期化
	ComPtr<IDirectInput8> directInput = nullptr;
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, nullptr);
	assert(SUCCEEDED(result));

	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));

	result = keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));

}

void Input::Update() {

	// キーボードの状態を取得する
	keyboard->Acquire();

	// キーの状態を取得する
	BYTE key[256] = {};
	keyboard->GetDeviceState(sizeof(key), key);

}
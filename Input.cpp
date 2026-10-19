#include "Input.h"
#include<cassert>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

void Input::Initialize(HINSTANCE hInstance, HWND hwnd) {

	HRESULT result;

	//DirectInputの初期化
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

	// 前のフレームのキー状態を保存
	memcpy(prevKey, key, sizeof(key));

	// キーボードの状態を取得する
	keyboard->Acquire();

	// キーの状態を取得する
	keyboard->GetDeviceState(sizeof(key), key);

}

bool Input::PushKey(BYTE keyNumber) {

	// キーの状態を取得する
	if (key[keyNumber]) {
		return true;
	}
	
	// キーが押されていない場合はfalseを返す
	return false;
}

bool Input::TriggerKey(BYTE keyNumber) {
	
	// キーが押された瞬間のみtrueを返す
	if (key[keyNumber] && !prevKey[keyNumber]) {
		return true;
	}
	
	// キーが押されていない場合はfalseを返す
	return false;
}
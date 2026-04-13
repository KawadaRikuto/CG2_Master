#include<windows.h>
#include<cstdint>
#include<string>
#include<format>


// ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
	WPARAM wparam, LPARAM lparam) {

	// メッセージに応じてゲーム固有の処理を行う
	switch (msg) {
		// ウィンドウが破棄された
	case WM_DESTROY:
		// OSに対して、アプリケーションの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Log(const std::string& message) {
	OutputDebugStringA(message.c_str());
}

// string->wstring
//std::wstring ConvertString(const std::string& str);

// wstring->string
std::string ConvertString(const std::wstring& str) {
	if (str.empty()) {
		return {};
	}

	int size = WideCharToMultiByte(
		CP_UTF8,
		0,
		str.data(),
		-1,
		nullptr,
		0,
		nullptr,
		nullptr
	);

	std::string result(size, 0);

	WideCharToMultiByte(
		CP_UTF8,
		0,
		str.data(),
		-1,
		result.data(),
		size,
		nullptr,
		nullptr
	);

	return result;
}



// windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int){

	WNDCLASS wc{};

	// ウィンドウプロシージャ
	wc.lpfnWndProc = WndProc;

	// ウィンドウクラス名
	wc.lpszClassName = L"CG2WindowClass";

	// インスタンスハンドル
	wc.hInstance = GetModuleHandle(nullptr);

	// カーソル
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	// ウィンドウクラスを登録する
	RegisterClass(&wc);

	// クライアント領域のサイズ
	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

	// ウィンドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc = { 0, 0, kClientWidth, kClientHeight };

	// クライアント領域を元に実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	HWND hwnd = CreateWindow(
		wc.lpszClassName,	  // 利用するクラス名
		L"CG2",               // タイトルバーの文字
		WS_OVERLAPPEDWINDOW,  // ウィンドウスタイル
		CW_USEDEFAULT,        // ウィンドウx座標
		CW_USEDEFAULT,        // ウィンドウy座標
		wrc.right - wrc.left, // ウィンドウ横幅
		wrc.bottom - wrc.top, // ウィンドウ縦幅
		nullptr,			  // 親ウィンドウハンドル
		nullptr,			  // メニューハンドル
		wc.hInstance,		  // インスタンスハンドル
		nullptr			      // オプション
	);

	// ウィンドウを表示する
	ShowWindow(hwnd, SW_SHOW);

	// 出力ウィンドウへの文字出力
	OutputDebugStringA("Hello, DirectX!\n");

	MSG msg{};

	// 文字列を格納する
	std::string str0{ "STRING!!!" };

	// 整数を文字列にする
	std::string str1{ std::to_string(10) };

	// 変数から型を推論してくれる
	//Log(std::format("enemyHp:{}, texturePath:{}\n", enemyHp, texturePath));

	std::wstring wstringValue = L"TEST";

	// wstring->string
	Log(ConvertString(std::format(L"WSTRING{}\n",wstringValue)));

	// ウィンドウの×ボタンが押されるまでループ
	while (msg.message != WM_QUIT) {
		// windowにメッセージが来てたら最優先で処理させる
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			// ゲームの処理
		}
	}

	return 0;
}


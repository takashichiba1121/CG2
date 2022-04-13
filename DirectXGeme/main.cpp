#include<Windows.h>

//ウィンドウプロージャー
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wapram, LPARAM lparam) {
	//メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{
	//ウインドウが破棄された
	case WM_DESTROY:
		//OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wapram, lparam);
}
//windowsアプリのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//コンソールへの文字出力
	OutputDebugStringA("Hello,DirectX!!!\n");
	const int window_width = 1200;//横
	const int window_heigit = 720;//縦
	//ウインドウクラスの設定
	WNDCLASSEX w{};
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;//ウインドウプロ―ジャーの設定
	w.lpszClassName = L"DirectXGame";//ウインドウクラス名
	w.hInstance = GetModuleHandle(nullptr);//ウインドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW);//カーソル指定

	//ウインドウクラスをOSに登録する
	RegisterClassEx(&w);
	//ウインドウサイズ｛X座標　Y座標　横幅　縦幅｝
	RECT wrc = { 0,0,window_width,window_heigit };
	//自動サイズを補正する
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);
	//ウインドウオブジェクトの生成
	HWND hwnd = CreateWindow(w.lpszClassName,//クラス名
		L"DirectXGame",//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,//標準的なウインドウスタイル
		CW_USEDEFAULT,//表示X座標（OSに任せる）
		CW_USEDEFAULT,//表示Y座標（OSに任せる）
		wrc.right - wrc.left,//ウインドウ横幅
		wrc.bottom - wrc.top,//ウインドウ縦幅
		nullptr,//親ウインドウハンドル
		nullptr,//メニューハンドル
		w.hInstance,//呼び出しアプリケーションハンドル
		nullptr);//オプション

	//ウインドウを表示状態にする
	ShowWindow(hwnd, SW_SHOW);

	MSG msg{};

	//DirectX初期化処理　ここから
	// 
	//DirectX初期化処理　ここまで

	//ゲームループ
	while (true) {
		//メッセージがある？
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);//キー入力メッセージの処理
			DispatchMessage(&msg);//プロシージャにメッセージが残る
		}

		//×ボタンで終了メッセージがきたらゲームループを抜ける
		if (msg.message == WM_QUIT) {
			break;
		}
		//DirectX毎フレーム処理　ここから
		
		//DirectX毎フレーム処理　ここまで
	}

	//ウインドウクラスを登録解除
	UnregisterClass(w.lpszClassName, w.hInstance);

	return 0;
}
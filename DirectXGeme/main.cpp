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
	return 0;
}
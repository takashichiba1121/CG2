#include<Windows.h>

//�E�B���h�E�v���[�W���[
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wapram, LPARAM lparam) {
	//���b�Z�[�W�ɉ����ăQ�[���ŗL�̏������s��
	switch (msg)
	{
	//�E�C���h�E���j�����ꂽ
	case WM_DESTROY:
		//OS�ɑ΂��āA�A�v���̏I����`����
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wapram, lparam);
}
//windows�A�v���̃G���g���[�|�C���g�imain�֐��j
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//�R���\�[���ւ̕����o��
	OutputDebugStringA("Hello,DirectX!!!\n");
	const int window_width = 1200;//��
	const int window_heigit = 720;//�c
	//�E�C���h�E�N���X�̐ݒ�
	WNDCLASSEX w{};
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;//�E�C���h�E�v���\�W���[�̐ݒ�
	w.lpszClassName = L"DirectXGame";//�E�C���h�E�N���X��
	w.hInstance = GetModuleHandle(nullptr);//�E�C���h�E�n���h��
	w.hCursor = LoadCursor(NULL, IDC_ARROW);//�J�[�\���w��

	//�E�C���h�E�N���X��OS�ɓo�^����
	RegisterClassEx(&w);
	//�E�C���h�E�T�C�Y�oX���W�@Y���W�@�����@�c���p
	RECT wrc = { 0,0,window_width,window_heigit };
	//�����T�C�Y��␳����
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);
	return 0;
}
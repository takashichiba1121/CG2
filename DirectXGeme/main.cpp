#include<Windows.h>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<vector>
#include<string>
#include<DirectXMath.h>
using namespace DirectX;
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
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
	//�E�C���h�E�I�u�W�F�N�g�̐���
	HWND hwnd = CreateWindow(w.lpszClassName,//�N���X��
		L"DirectXGame",//�^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,//�W���I�ȃE�C���h�E�X�^�C��
		CW_USEDEFAULT,//�\��X���W�iOS�ɔC����j
		CW_USEDEFAULT,//�\��Y���W�iOS�ɔC����j
		wrc.right - wrc.left,//�E�C���h�E����
		wrc.bottom - wrc.top,//�E�C���h�E�c��
		nullptr,//�e�E�C���h�E�n���h��
		nullptr,//���j���[�n���h��
		w.hInstance,//�Ăяo���A�v���P�[�V�����n���h��
		nullptr);//�I�v�V����

	//�E�C���h�E��\����Ԃɂ���
	ShowWindow(hwnd, SW_SHOW);

	MSG msg{};

	//DirectX�����������@��������
 #ifdef _DEBUG
	//�f�o�b�O���C���[���I����
	ID3D12Debug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
	}
#endif
	HRESULT result;
	ID3D12Device* device = nullptr;
	IDXGIFactory7* dxgiFactory = nullptr;
	IDXGISwapChain4* swapChain = nullptr;
	ID3D12CommandAllocator* comdAllocator = nullptr;
	ID3D12GraphicsCommandList* commandList = nullptr;
	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12DescriptorHeap* rtvHeap = nullptr;
	//�Ή����x���̔z��
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	//DXGI�t�@�N�g���[�̐���
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));

	//�A�_�v�^�[�̗񋓗p
	std::vector<IDXGIAdapter4*>adapters;
	//�����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	IDXGIAdapter4* tmpAdapter = nullptr;

	//�p�t�H�[�}���X���������̂��珇�ɁA���ׂẴA�_�v�^�[��񋓂���
	for (UINT i = 0;
		dxgiFactory->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++) {
		//���I�z��ɒǉ�����
		adapters.push_back(tmpAdapter);
	}
	//�Ó��ȃA�_�v�^��I�ʂ���
	for (size_t i = 0; i < adapters.size(); i++) {
		DXGI_ADAPTER_DESC3 adapterDesc;
		//�A�_�v�^�[�̏����擾����
		adapters[i]->GetDesc3(&adapterDesc);

		//�\�t�g�E�F�A�f�o�C�X�����
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			//�f�o�C�X���̗p���ă��[�v�𔲂���
			tmpAdapter = adapters[i];
			break;
		}
	}
	D3D_FEATURE_LEVEL featureLevel;

	for (size_t i = 0; i < _countof(levels); i++) {
		//�̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(tmpAdapter, levels[i],
			IID_PPV_ARGS(&device));
		if (result == S_OK) {
			//�f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
		}
	}
	//�R�}���h�A���P�[�^�𐶐�
	result = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&comdAllocator));
	assert(SUCCEEDED(result));
	//�R�}���h���X�g�̐���
	result = device->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		comdAllocator, nullptr,
		IID_PPV_ARGS(&commandList));
	//�R�}���h�L���[�̐ݒ�
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	//�R�}���h�L���[�𐶐�
	result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(result));
	//�X���b�v�`�F�[���̐ݒ�
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = 1280;
	swapChainDesc.Height = 720;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//�F���̏���
	swapChainDesc.SampleDesc.Count = 1;//�}���`�T���v�����Ȃ�
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;//�o�b�N�o�b�t�@�p
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//�t���b�v��͔j��
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	//�X���b�v�`�F�[���̐���
	result = dxgiFactory->CreateSwapChainForHwnd(
		commandQueue, hwnd, &swapChainDesc, nullptr, nullptr,
		(IDXGISwapChain1**)&swapChain);
	assert(SUCCEEDED(result));

	//�f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;//�����_�[�^�[�Q�b�g�r���[
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount;//���\�̓��

	//�f�X�N���v�^�q�[�v�̐���
	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));
	//�o�b�N�o�b�t�@
	std::vector<ID3D12Resource*>backBuffers;
	backBuffers.resize(swapChainDesc.BufferCount);

	//�X���b�v�`�F�[���̂��ׂẴo�b�t�@�ɂ����ꏈ������
	for (size_t i = 0; i < backBuffers.size(); i++) {
		//�X���b�v�`�F�[������o�b�t�@���擾
		swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
		//�f�X�N���v�^�q�[�v�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		//�����\���ŃA�h���X�������
		rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		//�����_�[�^�[�Q�b�g�r���[���ݒ�
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
		//�V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		//�����_�[�^�[�Q�b�g�r���[�̐���
		device->CreateRenderTargetView(backBuffers[i], &rtvDesc, rtvHandle);
	}

	//�t�F���X�̐���
	ID3D12Fence* fence = nullptr;
	UINT64 fenceVal = 0;

	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	//DirectX����������
	
	//�`�揉��������

	//���_�f�[�^
	XMFLOAT3 vertices[] = {
		{-0.5f,-0.5f,0.0f},
		{-0.5f,+0.5f,0.0f},
		{+0.5f,-0.5f,0.0f},
	};
	//���_�f�[�^�S�̂̃T�C�Y=���_�f�[�^����̃T�C�Y*���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * _countof(vertices));
	//���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};  //�q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPU�ւ̓]���p
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; //���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//���_�o�b�t�@�̐���
	ID3D12Resource* vertBuff = nullptr;
	result = device->CreateCommittedResource(
		&heapProp, //�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, //���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));
	//GPU��̃o�b�t�@�ɑΉ��������z�������i���C����������j���擾
	XMFLOAT3* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//�O���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i]; //���W�R�s�[
	}
	//�q�������
	vertBuff->Unmap(0, nullptr);

	//���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeof(XMFLOAT3);
	
	//�Q�[�����[�v
	while (true) {
		//���b�Z�[�W������H
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);//�L�[���̓��b�Z�[�W�̏���
			DispatchMessage(&msg);//�v���V�[�W���Ƀ��b�Z�[�W���c��
		}

		//�~�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
		if (msg.message == WM_QUIT) {
			break;
		}
		//DirectX���t���[�������@��������
		//�o�b�N�o�b�t�@�̔ԍ����擾�i�Q�Ȃ̂łO�Ԃ��P�ԁj
		UINT bbIndex = swapChain->GetCurrentBackBufferIndex();

		//1.���\�[�X�o���A�ŏ������݉\�ɕύX
		D3D12_RESOURCE_BARRIER barrierDesc{};
		barrierDesc.Transition.pResource = backBuffers[bbIndex];//�o�b�N�o�b�t�@���w��
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;//�\����Ԃ���
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		commandList->ResourceBarrier(1,&barrierDesc);
		//2.�`���̕ύX
		// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIndex * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

		//3.��ʃN���A
		FLOAT clearColor[] = { 0.1f,0.25f,0.5f,0.0f };//���ۂ��F
		commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		//�`��R�}���h��������
		// 
		//�`��R�}���h�����܂�
		
		// 5.���\�[�X�o���A��߂�
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;//�`���Ԃ���
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		commandList->ResourceBarrier(1, &barrierDesc);

		//���߃N���[�Y
		result = commandList->Close();
		assert(SUCCEEDED(result));
		//�R�}���h���X�g�̎��s
		ID3D12CommandList* commandLists[] = { commandList };
		commandQueue->ExecuteCommandLists(1, commandLists);

		//��ʂɕ\������o�b�t�@���t���b�v�i���\�̓���ւ��j
		result = swapChain->Present(1, 0);
		assert(SUCCEEDED(result));
		//�R�}���h�̎��s������҂�
		commandQueue->Signal(fence, ++fenceVal);
		if (fence->GetCompletedValue() != fenceVal) {
			HANDLE event = CreateEvent(nullptr, false, false, nullptr);
			fence->SetEventOnCompletion(fenceVal, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
		//�L���[�̃N���A
		result = comdAllocator->Reset();
		assert(SUCCEEDED(result));
		//�ĂуR�}���h���X�g�����߂鏀��
		result = commandList->Reset(comdAllocator, nullptr);
		assert(SUCCEEDED(result));

		//DirectX���t���[�������@�����܂�
	}

	//�E�C���h�E�N���X��o�^����
	UnregisterClass(w.lpszClassName, w.hInstance);

	return 0;
}
#pragma once
#define DIRECTINPUT_VERSION 0x0800 //DirectInput�̃o�[�W�����w��
#include<dinput.h>
#include<d3dcompiler.h>
#include<Windows.h>
#include <tchar.h>
#include<iostream>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include<vector>
#include<string>
#include<DirectXMath.h>
#include<DirectXTex.h>
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

class Triangle
{
public:
	ID3D12PipelineState* pipelineState;
	ID3D12RootSignature* rootSignature;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	D3D12_INDEX_BUFFER_VIEW ibView{};
	//�C���f�b�N�X�f�[�^
	unsigned short indices[3] =
	{
		0,1,2,//�O�p�`�P
	};
	//�R���X�g���N�^
	Triangle();
	//�f�X�g���N�^
	~Triangle();
	//�`�揉��������
	void Initialize(HRESULT result,ID3D12Device* device = nullptr);
	void Draw(ID3D12GraphicsCommandList* commandList, int window_width, int window_heigit);
};

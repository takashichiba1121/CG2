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

using namespace DirectX;
using namespace std;


class Triangle
{
public:
	struct Vertex
	{
		XMFLOAT3 pos; //xyz���W
		XMFLOAT2 uv;  //uv���W
	};
	ID3D12PipelineState* pipelineState=nullptr;
	ID3D12RootSignature* rootSignature=nullptr;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	D3D12_INDEX_BUFFER_VIEW ibView;
	HRESULT result;
	Vertex vertex1;
	Vertex vertex2;
	Vertex vertex3;
	//�C���f�b�N�X�f�[�^
	unsigned short indices[3] =
	{
		0,1,2,//�O�p�`�P
	};
	//�R���X�g���N�^
	Triangle();
	Triangle(Vertex vertex1, Vertex vertex2, Vertex vertex3);
	//�f�X�g���N�^
	~Triangle();
	//�`�揉��������
	void Initialize(ID3D12Device* device);
	void Draw(ID3D12CommandQueue* commandQueue, D3D12_RESOURCE_BARRIER barrierDesc,ID3D12GraphicsCommandList* commandList, int window_width, int window_heigit, D3D12_VIEWPORT viewport);
};
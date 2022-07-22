#pragma once
#include<wrl.h>
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
#include"Triangle.h"
#include"object3d.h"

using namespace DirectX;
using namespace std;
using namespace Microsoft::WRL;

struct Vertex
{
	XMFLOAT3 pos;    //xyz���W
};

class Line
{
public:
	void Initialize(ID3D12Device* device, HRESULT result);

	void Draw(ID3D12GraphicsCommandList* commandList, XMMATRIX matView, XMMATRIX matProjection);
private:
	//�C���f�b�N�X�f�[�^
	unsigned short indices[2] =
	{
		//�O
		0,1,
	};
	//�萔�o�b�t�@�i�s��p�j
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform;
	//�萔�o�b�t�@�}�b�v�i�s��p�j
	ConstBufferDataTransform* constMapTransform;
	ComPtr<ID3D12PipelineState> pipelineState = nullptr;
	ComPtr<ID3D12RootSignature> rootSignature;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
	//�萔�o�b�t�@�̃}�b�s���O
	ConstBufferDataMaterial* constMapMaterial = nullptr;
};
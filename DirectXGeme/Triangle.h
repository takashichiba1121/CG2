#pragma once
#define DIRECTINPUT_VERSION 0x0800 //DirectInputのバージョン指定
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
using namespace DirectX;
using namespace std;


class Triangle
{
public:
	struct Vertex
	{
		XMFLOAT3 pos; //xyz座標
		XMFLOAT2 uv;  //uv座標
	};
	ID3D12PipelineState* pipelineState=nullptr;
	ID3D12RootSignature* rootSignature=nullptr;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	D3D12_INDEX_BUFFER_VIEW ibView;
	HRESULT result;
	Vertex vertex1;
	Vertex vertex2;
	Vertex vertex3;
	//インデックスデータ
	unsigned short indices[3] =
	{
		0,1,2,//三角形１
	};
public:
	//コンストラクタ
	Triangle();
	Triangle(Vertex vertex1, Vertex vertex2, Vertex vertex3);
	//デストラクタ
	~Triangle();
	//描画初期化処理
	void Initialize(ID3D12Device* device);
	void Draw(ID3D12GraphicsCommandList* commandList);
};

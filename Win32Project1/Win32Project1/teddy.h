#pragma once

#include "myDirectX.h"
#include <DirectXMath.h>
#include <DirectXColors.h>
//#include <Windows.h>
//#include "D3D11.h"
#include "FBX.h"
#include "pixelShader.csh"
#include "vertexShader.csh"
#include "..\DLL\DLL.h"

using namespace std;
using namespace DirectX;

namespace ShaderStruct
{
	struct MVPConstBuffer
	{
		DirectX::XMFLOAT4X4 model;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;

	};

	struct VertPosColor
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 color;

	};

	struct VertPosUVNorm
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 uv;
		DirectX::XMFLOAT3 norm;

	};

	struct VertexBuffObj
	{
		ID3D11InputLayout* inputLayout;
		ID3D11Buffer* vertexBuffer;
		ID3D11VertexShader* vertexShader;
		ID3D11PixelShader* pixelShader;
		ID3D11Buffer* constantBuffer;
		ShaderStruct::MVPConstBuffer constBuffData;
		unsigned int vertCount;

		void cleanMem()
		{
			inputLayout->Release();
			vertexBuffer->Release();
			vertexShader->Release();
			pixelShader->Release();
			constantBuffer->Release();
		}

	};


}



class teddy : public myDirectX
{
private:
	XMFLOAT4X4 xCamera;
	
	ID3D11RasterizerState* xWire;
	ID3D11RasterizerState* xFill;
	ID3D11SamplerState* xSampler;
	ID3D11ShaderResourceView* view;
	ShaderStruct::VertexBuffObj teddyMesh;
	ShaderStruct::VertexBuffObj teddyJoints;
	ShaderStruct::VertexBuffObj plane;
	fbx fLoader;
	std::vector<pos> mesh;
	std::vector<Joint> joints;
	Clip fAnimation;
	std::vector<ShaderStruct::VertPosColor> jointVertBuff;


public:
	teddy(HINSTANCE hInst);
	~teddy();
	bool init() override;
	void update(float u) override;
	void render(float r) override;

private:
	void createResources(void);
	void setMPos(void);
	void updateCam(float dt);


};
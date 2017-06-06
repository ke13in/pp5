//#pragma once
//
//#include <d3d11.h>
//#include <D3DX11.h>
//#include <DirectXMath.h>
//#include <DirectXColors.h>
//#include "pixelShader.csh"
//#include "vertexShader.csh"
//#include "..\DLL\DLL.h"
//
////
////float Speed = 0.05f;
////
////POINT CurrMousePos;
////POINT PrevMousePos;
////
////bool RMouseClick = false;
//
//
//
//class myDirectX
//{
//private:
//	fbx fbxLoader;
//	std::vector<pos> mesh;
//
//
//public:
//	myDirectX();
//	~myDirectX();
//
//
//	DirectX::XMFLOAT4X4 dCamera;
//
//	struct matrix
//	{
//		DirectX::XMFLOAT4X4 proj;
//		DirectX::XMFLOAT4X4 view;
//		DirectX::XMFLOAT4X4 world;
//
//	};
//
//	struct pipeline_stage_t
//	{
//		ID3D11InputLayout *input_layout;
//		ID3D11VertexShader *vertex_shader;
//		ID3D11PixelShader * pixel_shader;
//		ID3D11RenderTargetView *render_target;
//		ID3D11Texture2D * depthStenciBuffer;
//		ID3D11DepthStencilState * depthlStencilState;
//		ID3D11DepthStencilView * depthStencilView;
//		ID3D11RasterizerState *rasterState;
//		ID3D11Buffer * buffer;
//		ID3D11Buffer * constantBuffer;
//
//		matrix modelViewProj;
//		unsigned int vertCount;
//
//		void cleanMem()
//		{
//			//Setup Mem
//		}
//
//	}default_pipeline;
//
//	struct directX_attributes
//	{
//
//		ID3D11Device * device;
//		ID3D11DeviceContext * deviceContext;
//		IDXGISwapChain * swapChain;
//		ID3D11RenderTargetView * renderTarget;
//		D3D_DRIVER_TYPE driverType;
//		D3D_FEATURE_LEVEL featureLevel;
//		D3D11_VIEWPORT viewport;
//	};
//
//	struct vertex
//	{
//		DirectX::XMFLOAT4 pos;
//		DirectX::XMFLOAT4 color;
//	};
//	
//	
//
//	pipeline_stage_t pipelineStage;
//	pipeline_stage_t meshStage;
//	directX_attributes attributes;
//	matrix matrix;
//
//	ID3D11Buffer *matrixBuffer;
//	ID3D11Buffer *xBuffer;
//	ID3D11Buffer *yBuffer;
//	ID3D11Buffer *zBuffer;
//	
//	void init(HWND wndw, int w, int h);
//	void render();
//	void setShader();
//	void triangle();
//	void camera();
//	void draw();
//	
//
//
//};
//

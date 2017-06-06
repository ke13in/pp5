#include "stdafx.h"
//#include "DirectX.h"
//
//using namespace DirectX;
//
//myDirectX::myDirectX()
//{
//
//	/*pipelineStage.depthStencilBuffer = nullptr;
//	pipelineStage.depthStencilState = nullptr;*/
//	pipelineStage.depthStencilView = nullptr;
//	pipelineStage.input_layout = nullptr;
//	pipelineStage.pixel_shader = nullptr;
//	pipelineStage.rasterState = nullptr;
//	pipelineStage.vertex_shader = nullptr;
//	
//
//	attributes.device = nullptr;
//	attributes.deviceContext = nullptr;
//	attributes.renderTarget = nullptr;
//	attributes.swapChain = nullptr;
//
//}
//
//
//myDirectX::~myDirectX()
//{
//	/*if (pipelineStage.depthStencilBuffer != nullptr)
//		pipelineStage.depthStencilBuffer->Release();
//
//	if (pipelineStage.depthStencilState != nullptr)
//		pipelineStage.depthStencilState->Release();*/
//
//	if (pipelineStage.depthStencilView != nullptr)
//		pipelineStage.depthStencilView->Release();
//
//	if (pipelineStage.input_layout != nullptr)
//		pipelineStage.input_layout->Release();
//
//	if (pipelineStage.pixel_shader != nullptr)
//		pipelineStage.pixel_shader->Release();
//
//	if (pipelineStage.rasterState != nullptr)
//		pipelineStage.rasterState->Release();
//
//	if (pipelineStage.vertex_shader != nullptr)
//		pipelineStage.vertex_shader->Release();
//
//	if (attributes.device != nullptr)
//		attributes.device->Release();
//
//	if (attributes.deviceContext != nullptr)
//		attributes.deviceContext->Release();
//
//	if (attributes.renderTarget != nullptr)
//		attributes.renderTarget->Release();
//
//	if (attributes.swapChain != nullptr)
//		attributes.swapChain->Release();
//}
//
//
//
//
//
//
//
//
//void myDirectX::init(HWND wndw, int w, int h)
//{
//	fbxLoader.loadScene();
//	fbxLoader.loadMesh();
//	mesh = fbxLoader.getMesh();
//
//	/*pipelineStage.depthStencilBuffer = nullptr;
//	pipelineStage.depthStencilState = nullptr;*/
//	pipelineStage.depthStencilView = nullptr;
//	pipelineStage.input_layout = nullptr;
//	pipelineStage.pixel_shader = nullptr;
//	pipelineStage.rasterState = nullptr;
//	pipelineStage.vertex_shader = nullptr;
//
//
//	attributes.device = nullptr;
//	attributes.deviceContext = nullptr;
//	attributes.renderTarget = nullptr;
//	attributes.swapChain = nullptr;
//
//	UINT flags = 0;
//
//#ifdef _DEBUG
//	flags |= D3D11_CREATE_DEVICE_DEBUG;
//#endif
//
//	D3D_FEATURE_LEVEL features[] = {
//		D3D_FEATURE_LEVEL_10_0,
//		D3D_FEATURE_LEVEL_10_1,
//		D3D_FEATURE_LEVEL_11_0,
//		D3D_FEATURE_LEVEL_9_3
//	};
//
//	UINT featurelvls = ARRAYSIZE(features);
//
//	DXGI_SWAP_CHAIN_DESC _swapChain;
//	ZeroMemory(&_swapChain, sizeof(DXGI_SWAP_CHAIN_DESC));
//
//	_swapChain.BufferCount = 1;
//	_swapChain.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	_swapChain.BufferDesc.Height = h;
//	_swapChain.BufferDesc.Width = w;
//	_swapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//	_swapChain.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
//	_swapChain.OutputWindow = wndw;
//	_swapChain.SampleDesc.Count = 1;
//	_swapChain.Windowed = true;
//
//	HRESULT result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, features, featurelvls, D3D11_SDK_VERSION, &_swapChain, &attributes.swapChain, &attributes.device, &attributes.featureLevel, &attributes.deviceContext);
//
//	ID3D11Texture2D * texture2d = 0;
//	attributes.swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&texture2d);
//	attributes.device->CreateRenderTargetView(texture2d, nullptr, &attributes.renderTarget);
//	texture2d->Release();
//
//	attributes.deviceContext->OMSetRenderTargets(1, &attributes.renderTarget, nullptr);
//
//	ZeroMemory(&attributes.viewport, sizeof(D3D11_VIEWPORT));
//
//	attributes.viewport.Height = h;
//	attributes.viewport.MaxDepth = 1.0f;
//	attributes.viewport.MinDepth = 0.0f;
//	attributes.viewport.TopLeftX = 0;
//	attributes.viewport.TopLeftY = 0;
//	attributes.viewport.Width = w;
//
//	attributes.deviceContext->RSSetViewports(1, &attributes.viewport);
//
//
//
//	float aspect = 800 / 600;
//	float fov = 70.0f * 3.14f / 180.0f;
//
//	if (aspect < 1.0f) 
//		fov *= 2.0f;
//
//	DirectX::XMMATRIX perspec = DirectX::XMMatrixPerspectiveFovLH(fov, aspect, 0.01f, 10000000.0f);
//	DirectX::XMStoreFloat4x4(&meshStage.modelViewProj.proj, DirectX::XMMatrixTranspose(perspec));
//	DirectX::XMStoreFloat4x4(&meshStage.modelViewProj.world, DirectX::XMMatrixIdentity());
//
//	DirectX::XMVECTORF32 eye = { 100.5f, 100.7f, -100.5f, 0.0f };
//	DirectX::XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
//	DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };
//
//	DirectX::XMStoreFloat4x4(&dCamera, DirectX::XMMatrixInverse(nullptr, DirectX::XMMatrixLookAtLH(eye, at, up)));
//	DirectX::XMStoreFloat4x4(&meshStage.modelViewProj.view, DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&dCamera))));
//
//
//	setShader();
//	triangle();
//
//	XMStoreFloat4x4(&matrix.world, XMMatrixTranspose(DirectX::XMMatrixIdentity()));
//
//	camera();
//
//	//setup depthstencilbuffer
//}
//
//void myDirectX::render()
//{
//	attributes.deviceContext->UpdateSubresource(matrixBuffer, 0, NULL, &matrix, 0, 0);
//	attributes.deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);
//	attributes.deviceContext->ClearRenderTargetView(attributes.renderTarget, DirectX::Colors::SeaGreen);
//
//	attributes.deviceContext->UpdateSubresource(meshStage.constantBuffer, 0, NULL, &meshStage.modelViewProj, 0, 0);
//	attributes.deviceContext->VSSetConstantBuffers(0, 1, &meshStage.constantBuffer);
//
//
//	draw();
//
//	attributes.deviceContext->PSSetShader(pipelineStage.pixel_shader, NULL, 0);
//	attributes.deviceContext->VSSetShader(pipelineStage.vertex_shader, NULL, 0);
//	//// select which vertex buffer to display
//	UINT stride = sizeof(vertex);
//	UINT offset = 0;
//	//// select which primtive type we are using
//	attributes.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
//	//// draw the vertex buffer to the back buffer
//	attributes.deviceContext->IASetVertexBuffers(0, 1, &xBuffer, &stride, &offset);
//	attributes.deviceContext->Draw(2, 0);
//
//	attributes.deviceContext->IASetVertexBuffers(0, 1, &yBuffer, &stride, &offset);
//	attributes.deviceContext->Draw(2, 0);
//
//	attributes.deviceContext->IASetVertexBuffers(0, 1, &zBuffer, &stride, &offset);
//	attributes.deviceContext->Draw(2, 0);
//	attributes.swapChain->Present(0, 0);
//}
//
//void myDirectX::setShader()
//{
//	attributes.device->CreatePixelShader(pixelShader, sizeof(pixelShader), NULL, &pipelineStage.pixel_shader);
//	attributes.device->CreateVertexShader(vertexShader, sizeof(vertexShader), NULL, &pipelineStage.vertex_shader);
//
//	D3D11_INPUT_ELEMENT_DESC ied[] =
//	{
//		{ "SV_POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//	};
//
//	attributes.device->CreateInputLayout(ied, 2, vertexShader, sizeof(vertexShader), &pipelineStage.input_layout);
//	attributes.deviceContext->IASetInputLayout(pipelineStage.input_layout);
//
//	CD3D11_BUFFER_DESC constBuffDesc(sizeof(matrix), D3D11_BIND_CONSTANT_BUFFER);
//	attributes.device->CreateBuffer(&constBuffDesc, nullptr, &meshStage.constantBuffer);
//
//	attributes.device->CreatePixelShader(pixelShader, sizeof(pixelShader), NULL, &meshStage.pixel_shader);
//	attributes.device->CreateVertexShader(vertexShader, sizeof(vertexShader), NULL, &meshStage.vertex_shader);
//
//	attributes.device->CreateInputLayout(ied, 2, vertexShader, sizeof(vertexShader), &meshStage.input_layout);
//	attributes.deviceContext->IASetInputLayout(meshStage.input_layout);
//
//
//}
//
//void myDirectX::triangle()
//{
//	vertex verts[] = {
//		{ DirectX::XMFLOAT4(0,.5f, 0, 1), DirectX::XMFLOAT4(1, 0, 0, 1) },
//		{ DirectX::XMFLOAT4(0.5f, -0.5f, 0, 1), DirectX::XMFLOAT4(0, 1, 0, 1) },
//		{ DirectX::XMFLOAT4(-0.5f, -0.5f, 0, 1), DirectX::XMFLOAT4(0, 0, 1, 1) },
//	};
//
//	D3D11_BUFFER_DESC bufferDesc;
//	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
//
//	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
//	bufferDesc.ByteWidth = sizeof(vertex) * 3;
//	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//
//	attributes.device->CreateBuffer(&bufferDesc, NULL, &pipelineStage.buffer);
//
//	D3D11_MAPPED_SUBRESOURCE mappedSub;
//	attributes.deviceContext->Map(pipelineStage.buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedSub);
//	memcpy(mappedSub.pData, verts, sizeof(verts));
//	attributes.deviceContext->Unmap(pipelineStage.buffer, NULL);
//
//	vertex xVert[] =
//	{
//		{ DirectX::XMFLOAT4(0.0f,0.0f,0.0f,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
//		{ DirectX::XMFLOAT4(0.5f,0.0f,0.0f,1.0f),DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) }
//	};
//
//	vertex yVert[] =
//	{
//		{ DirectX::XMFLOAT4(0.0f,0.0f,0.0f,1.0f),DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
//		{ DirectX::XMFLOAT4(0.0f,0.5f,0.0f,1.0f),DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }
//	};
//
//	vertex zVert[] =
//	{
//		{ DirectX::XMFLOAT4(0.0f,0.0f,0.0f,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
//		{ DirectX::XMFLOAT4(0.0f,0.0f,-0.5f,1.0f),DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }
//	};
//
//	D3D11_BUFFER_DESC bufferDesc2;
//	ZeroMemory(&bufferDesc2, sizeof(bufferDesc2));
//
//	bufferDesc2.Usage = D3D11_USAGE_DYNAMIC;
//	bufferDesc2.ByteWidth = sizeof(vertex) * 2;
//	bufferDesc2.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bufferDesc2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//
//	D3D11_SUBRESOURCE_DATA subDataX;
//	subDataX.pSysMem = xVert;
//	subDataX.SysMemPitch = 0;
//	subDataX.SysMemSlicePitch = 0;
//
//	D3D11_SUBRESOURCE_DATA subDataY;
//	subDataY.pSysMem = yVert;
//	subDataY.SysMemPitch = 0;
//	subDataY.SysMemSlicePitch = 0;
//
//	D3D11_SUBRESOURCE_DATA subDataZ;
//	subDataZ.pSysMem = zVert;
//	subDataZ.SysMemPitch = 0;
//	subDataZ.SysMemSlicePitch = 0;
//
//	attributes.device->CreateBuffer(&bufferDesc2, &subDataX, &xBuffer);
//	attributes.device->CreateBuffer(&bufferDesc2, &subDataY, &yBuffer);
//	attributes.device->CreateBuffer(&bufferDesc2, &subDataZ, &zBuffer);
//
//	CD3D11_BUFFER_DESC desc(sizeof(matrix), D3D11_BIND_CONSTANT_BUFFER);
//	attributes.device->CreateBuffer(&desc, NULL, &matrixBuffer);
//
//	std::vector<vertex> meshVert;
//
//	for (unsigned int i = 0; i < mesh.size(); i++)
//	{
//		vertex tmp;
//
//		tmp.pos.x = mesh[i].xyzw[0];
//		tmp.pos.y = mesh[i].xyzw[1];
//		tmp.pos.z = mesh[i].xyzw[2];
//		tmp.pos.w = mesh[i].xyzw[3];
//
//		DirectX::XMFLOAT4 color = { 1 ,0 , 0, 1 };
//
//		tmp.color = color;
//
//		meshVert.push_back(tmp);
//
//	}
//	
//	meshStage.vertCount = mesh.size();
//
//	D3D11_SUBRESOURCE_DATA vertData = { 0 };
//
//	vertData.pSysMem = meshVert.data();
//	vertData.SysMemPitch = 0;
//	vertData.SysMemSlicePitch = 0;
//
//	CD3D11_BUFFER_DESC vertDesc(sizeof(vertex) * meshVert.size(), D3D11_BIND_VERTEX_BUFFER);
//	attributes.device->CreateBuffer(&vertDesc, &vertData, &meshStage.buffer);
//
//
//
//}
//
//void myDirectX::camera()
//{
//	DirectX::XMVECTORF32 eye = { 0.5f, 0.7f, -1.5f, 0.0f };
//	DirectX::XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
//	DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };
//
//	XMStoreFloat4x4(&matrix.view, XMMatrixTranspose(XMMatrixLookAtLH(eye, at, up)));
//
//	float aspectRatio = 500 / 500;
//	float fovAngleY = 70.0f * 3.14f / 180.0f;
//
//	DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, 0.01f, 100.0f);
//	XMStoreFloat4x4(&matrix.proj, XMMatrixTranspose(projection));
//}
//
//void myDirectX::draw()
//{
//	attributes.deviceContext->PSSetShader(pipelineStage.pixel_shader, NULL, 0);
//	attributes.deviceContext->VSSetShader(pipelineStage.vertex_shader, NULL, 0);
//
//	UINT stride = sizeof(vertex);
//	UINT offset = 0;
//	attributes.deviceContext->IASetVertexBuffers(0, 1, &pipelineStage.buffer, &stride, &offset);
//
//	attributes.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	attributes.deviceContext->Draw(3, 0);
//
//
//	stride = sizeof(vertex);
//	offset = 0;
//	attributes.deviceContext->IASetVertexBuffers(0, 1, &meshStage.buffer, &stride, &offset);
//	attributes.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	attributes.deviceContext->IASetInputLayout(meshStage.input_layout);
//	attributes.deviceContext->VSSetShader(meshStage.vertex_shader, nullptr, 0);
//	attributes.deviceContext->PSSetShader(meshStage.pixel_shader, nullptr, 0);
//	attributes.deviceContext->Draw(meshStage.vertCount, 0);
//	
//	//change vertex buffer between the two pipelines
//	//disable backface
//	//check proj and view matrix
//	//get camera movement ^
//
//
//
//
//
//}
//

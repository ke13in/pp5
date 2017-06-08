#include "teddy.h"
#include "stdafx.h"

teddy::teddy(HINSTANCE hInst) : myDirectX(hInst)
{
}

teddy::~teddy()
{
	xWire->Release();
	xFill->Release();
	

}

bool teddy::init()
{
	if (!myDirectX::init())
		return false;

	fLoader.loadScene();
	fLoader.loadMesh();
	fLoader.loadJoints();
	joints = fLoader.getJoints();
	mesh = fLoader.getMesh();



	float aspect = clientW / clientH;
	float fov = 70.0f * XM_PI / 180.0f;

	if (aspect < 1.0f)
		fov *= 2.0f;

	XMMATRIX perspec = XMMatrixPerspectiveFovLH(fov, aspect, 0.01f, 10000.0f);

	XMStoreFloat4x4(&teddyMesh.constBuffData.projection, XMMatrixTranspose(perspec));
	XMStoreFloat4x4(&teddyMesh.constBuffData.model, XMMatrixIdentity());
	XMStoreFloat4x4(&teddyJoints.constBuffData.projection, XMMatrixTranspose(perspec));
	XMStoreFloat4x4(&teddyJoints.constBuffData.model, XMMatrixIdentity());
	XMStoreFloat4x4(&plane.constBuffData.projection, XMMatrixTranspose(perspec));
	XMStoreFloat4x4(&plane.constBuffData.model, XMMatrixIdentity());

	static const XMVECTORF32 eye = { 0.0f, 5.7f, -5.5f, 0.0f };
	static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	DirectX::XMStoreFloat4x4(&xCamera, XMMatrixInverse(nullptr, XMMatrixLookAtLH(eye, at, up)));
	DirectX::XMStoreFloat4x4(&teddyMesh.constBuffData.view, XMMatrixTranspose(XMMatrixInverse(nullptr, XMLoadFloat4x4(&xCamera))));
	DirectX::XMStoreFloat4x4(&teddyJoints.constBuffData.view, XMMatrixTranspose(XMMatrixInverse(nullptr, XMLoadFloat4x4(&xCamera))));
	DirectX::XMStoreFloat4x4(&plane.constBuffData.view, XMMatrixTranspose(XMMatrixInverse(nullptr, XMLoadFloat4x4(&xCamera))));


	createResources();

	return true;

}

void teddy::update(float u)
{
	updateCam(u);
	DirectX::XMStoreFloat4x4(&teddyMesh.constBuffData.view, XMMatrixTranspose(XMMatrixInverse(nullptr, XMLoadFloat4x4(&xCamera))));
	DirectX::XMStoreFloat4x4(&teddyJoints.constBuffData.view, XMMatrixTranspose(XMMatrixInverse(nullptr, XMLoadFloat4x4(&xCamera))));
	DirectX::XMStoreFloat4x4(&plane.constBuffData.view, XMMatrixTranspose(XMMatrixInverse(nullptr, XMLoadFloat4x4(&xCamera))));

}

void teddy::render(float r)
{
	deviceContext->ClearRenderTargetView(renderTarget, DirectX::Colors::SeaGreen);



	deviceContext->UpdateSubresource(plane.constantBuffer, 0, NULL, &plane.constBuffData, 0, 0);
	UINT Stride = sizeof(ShaderStruct::VertPosColor);
	UINT Offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &plane.vertexBuffer, &Stride, &Offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetInputLayout(plane.inputLayout);
	deviceContext->VSSetShader(plane.vertexShader, nullptr, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &plane.constantBuffer);
	deviceContext->PSSetShader(plane.pixelShader, nullptr, 0);
	deviceContext->Draw(plane.vertCount, 0);


#if _DEBUG
	deviceContext->UpdateSubresource(teddyJoints.constantBuffer, 0, NULL, &teddyJoints.constBuffData, 0, 0);
	Stride = sizeof(ShaderStruct::VertPosColor);
	Offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &teddyJoints.vertexBuffer, &Stride, &Offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	deviceContext->IASetInputLayout(teddyJoints.inputLayout);
	deviceContext->VSSetShader(teddyJoints.vertexShader, nullptr, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &teddyJoints.constantBuffer);
	deviceContext->PSSetShader(teddyJoints.pixelShader, nullptr, 0);
	deviceContext->Draw(teddyJoints.vertCount, 0);
#endif // _DEBUG

	deviceContext->UpdateSubresource(teddyMesh.constantBuffer, 0, NULL, &teddyMesh.constBuffData, 0, 0);
	Stride = sizeof(ShaderStruct::VertPosColor);
	Offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &teddyMesh.vertexBuffer, &Stride, &Offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->IASetInputLayout(teddyMesh.inputLayout);
	deviceContext->VSSetShader(teddyMesh.vertexShader, nullptr, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &teddyMesh.constantBuffer);
	deviceContext->PSSetShader(teddyMesh.pixelShader, nullptr, 0);
	deviceContext->RSSetState(xWire);
	deviceContext->Draw(teddyMesh.vertCount, 0);
	deviceContext->RSSetState(xFill);






	swapChain->Present(0, 0);

}

void teddy::setMPos(void)
{
	prevMPos = currMPos;
	GetCursorPos(&currMPos);
	ScreenToClient(hAppwnd, &currMPos);
}

void teddy::updateCam(float dt)
{
	if (GetAsyncKeyState('W'))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, 0.005f);
		XMMATRIX tmp = XMLoadFloat4x4(&xCamera);
		XMMATRIX res = XMMatrixMultiply(translation, tmp);
		XMStoreFloat4x4(&xCamera, res);
	}
	else if (GetAsyncKeyState('S'))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, -0.005f);
		XMMATRIX tmp = XMLoadFloat4x4(&xCamera);
		XMMATRIX res = XMMatrixMultiply(translation, tmp);
		XMStoreFloat4x4(&xCamera, res);
	}
	else if (GetAsyncKeyState('A'))
	{
		XMMATRIX translation = XMMatrixTranslation(-0.005f, 0.0f, 0.0f);
		XMMATRIX tmp = XMLoadFloat4x4(&xCamera);
		XMMATRIX res = XMMatrixMultiply(translation, tmp);
		XMStoreFloat4x4(&xCamera, res);
	}
	else if (GetAsyncKeyState('D'))
	{
		XMMATRIX translation = XMMatrixTranslation(0.005f, 0.0f, 0.0f);
		XMMATRIX tmp = XMLoadFloat4x4(&xCamera);
		XMMATRIX res = XMMatrixMultiply(translation, tmp);
		XMStoreFloat4x4(&xCamera, res);
	}

	if (GetAsyncKeyState(VK_UP))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, 0.005f, 0.0f);
		XMMATRIX tmp = XMLoadFloat4x4(&xCamera);
		XMMATRIX res = XMMatrixMultiply(translation, tmp);
		XMStoreFloat4x4(&xCamera, res);
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, -0.005f, 0.0f);
		XMMATRIX tmp = XMLoadFloat4x4(&xCamera);
		XMMATRIX res = XMMatrixMultiply(translation, tmp);
		XMStoreFloat4x4(&xCamera, res);
	}

	if (rMClick)
	{
		float x = currMPos.x - prevMPos.x;
		float y = currMPos.y - prevMPos.y;

		XMFLOAT4 Pos = XMFLOAT4(xCamera._41, xCamera._42, xCamera._43, xCamera._44);

		xCamera._41 = 0.0f;
		xCamera._42 = 0.0f;
		xCamera._43 = 0.0f;

		XMMATRIX RotationX = XMMatrixRotationX(y * 0.01f);
		XMMATRIX RotationY = XMMatrixRotationY(x * 0.01f);

		XMMATRIX Temp = XMLoadFloat4x4(&xCamera);
		Temp = XMMatrixMultiply(RotationX, Temp);
		Temp = XMMatrixMultiply(Temp, RotationY);

		XMStoreFloat4x4(&xCamera, Temp);

		xCamera._41 = Pos.x;
		xCamera._42 = Pos.y;
		xCamera._43 = Pos.z;

		setMPos();
	}



}

void teddy::createResources(void)
{
	D3D11_RASTERIZER_DESC rastDesc;
	ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC));
	rastDesc.FillMode = D3D11_FILL_WIREFRAME;
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.DepthClipEnable = true;

	device->CreateRasterizerState(&rastDesc, &xWire);

	D3D11_RASTERIZER_DESC rastDesc2;
	ZeroMemory(&rastDesc2, sizeof(D3D11_RASTERIZER_DESC));
	rastDesc2.FillMode = D3D11_FILL_SOLID;
	rastDesc2.CullMode = D3D11_CULL_NONE;
	rastDesc2.DepthClipEnable = true;

	HRESULT Return;

	Return = device->CreateRasterizerState(&rastDesc2, &xFill);

	device->CreatePixelShader(pixelShader, sizeof(pixelShader), NULL, &teddyMesh.pixelShader);
	device->CreateVertexShader(vertexShader, sizeof(vertexShader), NULL, &teddyMesh.vertexShader);

	device->CreatePixelShader(pixelShader, sizeof(pixelShader), NULL, &teddyJoints.pixelShader);
	device->CreateVertexShader(vertexShader, sizeof(vertexShader), NULL, &teddyJoints.vertexShader);

	device->CreatePixelShader(pixelShader, sizeof(pixelShader), NULL, &plane.pixelShader);
	device->CreateVertexShader(vertexShader, sizeof(vertexShader), NULL, &plane.vertexShader);

	D3D11_INPUT_ELEMENT_DESC layout[] =

	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};

	device->CreateInputLayout(layout, 2, vertexShader, sizeof(vertexShader), &teddyMesh.inputLayout);
	device->CreateInputLayout(layout, 2, vertexShader, sizeof(vertexShader), &teddyJoints.inputLayout);
	device->CreateInputLayout(layout, 2, vertexShader, sizeof(vertexShader), &plane.inputLayout);


	CD3D11_BUFFER_DESC constBuffDesc(sizeof(ShaderStruct::MVPConstBuffer), D3D11_BIND_CONSTANT_BUFFER);
	device->CreateBuffer(&constBuffDesc, nullptr, &teddyMesh.constantBuffer);
	device->CreateBuffer(&constBuffDesc, nullptr, &teddyJoints.constantBuffer);
	device->CreateBuffer(&constBuffDesc, nullptr, &plane.constantBuffer);

	ShaderStruct::VertPosColor planeVerts[] = {
		{XMFLOAT3{-40,-2,40}, XMFLOAT3{0,1,0}},
		{XMFLOAT3{-40,-2,-40}, XMFLOAT3{0,1,0}},
		{XMFLOAT3{40,-2,-40}, XMFLOAT3{0,1,0}},
		{XMFLOAT3{ 40,-2,-40 }, XMFLOAT3{ 0,1,0 }},
		{XMFLOAT3{ 40,-2,40 }, XMFLOAT3{ 0,1,0 }},
		{XMFLOAT3{ -40,-2,40 }, XMFLOAT3{ 0,1,0 }},
		
	};

	plane.vertCount = ARRAYSIZE(planeVerts);

	D3D11_SUBRESOURCE_DATA planeBuffData = { 0 };
	planeBuffData.pSysMem = planeVerts;
	planeBuffData.SysMemPitch = 0;
	planeBuffData.SysMemSlicePitch = 0;

	CD3D11_BUFFER_DESC planeBuffDesc(sizeof(ShaderStruct::VertPosColor) * ARRAYSIZE(planeVerts), D3D11_BIND_VERTEX_BUFFER);
	device->CreateBuffer(&planeBuffDesc, &planeBuffData, &plane.vertexBuffer);


	std::vector<ShaderStruct::VertPosColor> meshVerts;

	for (unsigned int i = 0; i < mesh.size(); i++)
	{
		ShaderStruct::VertPosColor tmp;

		tmp.pos.x = mesh[i].xyzw[0];
		tmp.pos.y = mesh[i].xyzw[1];
		tmp.pos.z = mesh[i].xyzw[2];

		tmp.color = XMFLOAT3{ 0, 0, 1 };

		meshVerts.push_back(tmp);
		
	}


	teddyMesh.vertCount = meshVerts.size();

	D3D11_SUBRESOURCE_DATA vertBuffData = { 0 };
	vertBuffData.pSysMem = meshVerts.data();
	vertBuffData.SysMemPitch = 0;
	vertBuffData.SysMemSlicePitch = 0;

	CD3D11_BUFFER_DESC vertBuffDesc(sizeof(ShaderStruct::VertPosColor) * meshVerts.size(), D3D11_BIND_VERTEX_BUFFER);
	device->CreateBuffer(&vertBuffDesc, &vertBuffData, &teddyMesh.vertexBuffer);

#if _DEBUG
	for each(Joint j in joints)
	{
		if (j.parentInd != -1)
		{
			ShaderStruct::VertPosColor tmp;
			tmp.pos = XMFLOAT3(j.transform[12], j.transform[13], j.transform[14]);
			tmp.color = XMFLOAT3(1.0f, 1.0f, 1.0f);
			jointVertBuff.push_back(tmp);
			tmp.pos = XMFLOAT3(joints[j.parentInd].transform[12], joints[j.parentInd].transform[13], joints[j.parentInd].transform[14]);
			jointVertBuff.push_back(tmp);
		}
	}

	teddyJoints.vertCount = jointVertBuff.size();

	D3D11_SUBRESOURCE_DATA jointsBuffData = { 0 };
	jointsBuffData.pSysMem = jointVertBuff.data();
	jointsBuffData.SysMemPitch = 0;
	jointsBuffData.SysMemSlicePitch = 0;

	CD3D11_BUFFER_DESC jointsBuffDesc(sizeof(ShaderStruct::VertPosColor) * jointVertBuff.size(), D3D11_BIND_VERTEX_BUFFER);
	device->CreateBuffer(&jointsBuffDesc, &jointsBuffData, &teddyJoints.vertexBuffer);
#endif // _DEBUG



}
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
	fLoader.loadAnim();
	joints = fLoader.getJoints();
	mesh = fLoader.getMesh();
	fAnimation = fLoader.getAnimation();

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

	float prevTime = fAnimation.frames[fAnimation.frames.size() - 1].elapsed;
	float tTime = fmod(totalTime, prevTime);

	unsigned int frame = 1;

	for (unsigned int i = 0; i < fAnimation.frames.size(); i++)
	{
		if (fAnimation.frames[i].elapsed > tTime)
		{
			frame = i;
			break;
		}
	}

	int prevFrame = frame - 1;

	if (prevFrame < 1)
	{
		prevFrame = fAnimation.frames.size() - 1;
	}

	float dt = fAnimation.frames[frame].elapsed - fAnimation.frames[prevFrame].elapsed;
	float modDt = tTime - fAnimation.frames[prevFrame].elapsed;

	if (frame == 1)
	{
		dt = fAnimation.frames[frame].elapsed;
		modDt = tTime;
	}

	float lerp = modDt / dt;

	std::vector<Joint> animJoints;

	for (unsigned int i = 0; i < fAnimation.frames[frame].j.size(); i++)
	{
		Joint prev;
		Joint curr;
		Joint animJoint;

		prev = fAnimation.frames[prevFrame].j[i];
		curr = fAnimation.frames[frame].j[i];

		animJoint = curr;

		animJoint.transform[12] = (curr.transform[12] - prev.transform[12]) * lerp + prev.transform[12];
		animJoint.transform[13] = (curr.transform[13] - prev.transform[13]) * lerp + prev.transform[13];
		animJoint.transform[14] = (curr.transform[14] - prev.transform[14]) * lerp + prev.transform[14];

		animJoints.push_back(animJoint);

	}

	std::vector<ShaderStruct::VertPosColor> newJoints;

	for each (Joint j in animJoints)
	{
		if (j.parentInd != -1)
		{
			ShaderStruct::VertPosColor tmp;

			tmp.pos = XMFLOAT3(j.transform[12], j.transform[13], j.transform[14]);
			tmp.color = XMFLOAT3(1, 1, 1);

			newJoints.push_back(tmp);

			tmp.pos = XMFLOAT3(animJoints[j.parentInd].transform[12], animJoints[j.parentInd].transform[13], animJoints[j.parentInd].transform[14]);

			newJoints.push_back(tmp);


		}
	}

	teddyJoints.vertCount = newJoints.size();

	D3D11_SUBRESOURCE_DATA newBuffer = { 0 };
	newBuffer.pSysMem = &newJoints[0];
	newBuffer.SysMemPitch = 0;
	newBuffer.SysMemSlicePitch = 0;

	CD3D11_BUFFER_DESC newDesc;
	newDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	newDesc.ByteWidth = sizeof(ShaderStruct::VertPosColor) * newJoints.size();
	newDesc.CPUAccessFlags = NULL;
	newDesc.MiscFlags = 0;
	newDesc.Usage = D3D11_USAGE_IMMUTABLE;

	HRESULT result = device->CreateBuffer(&newDesc, &newBuffer, &teddyJoints.vertexBuffer);



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
		XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, 0.05f);
		XMMATRIX tmp = XMLoadFloat4x4(&xCamera);
		XMMATRIX res = XMMatrixMultiply(translation, tmp);
		XMStoreFloat4x4(&xCamera, res);
	}
	else if (GetAsyncKeyState('S'))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, -0.05f);
		XMMATRIX tmp = XMLoadFloat4x4(&xCamera);
		XMMATRIX res = XMMatrixMultiply(translation, tmp);
		XMStoreFloat4x4(&xCamera, res);
	}
	else if (GetAsyncKeyState('A'))
	{
		XMMATRIX translation = XMMatrixTranslation(-0.05f, 0.0f, 0.0f);
		XMMATRIX tmp = XMLoadFloat4x4(&xCamera);
		XMMATRIX res = XMMatrixMultiply(translation, tmp);
		XMStoreFloat4x4(&xCamera, res);
	}
	else if (GetAsyncKeyState('D'))
	{
		XMMATRIX translation = XMMatrixTranslation(0.05f, 0.0f, 0.0f);
		XMMATRIX tmp = XMLoadFloat4x4(&xCamera);
		XMMATRIX res = XMMatrixMultiply(translation, tmp);
		XMStoreFloat4x4(&xCamera, res);
	}

	if (GetAsyncKeyState(VK_UP))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, 0.05f, 0.0f);
		XMMATRIX tmp = XMLoadFloat4x4(&xCamera);
		XMMATRIX res = XMMatrixMultiply(translation, tmp);
		XMStoreFloat4x4(&xCamera, res);
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, -0.05f, 0.0f);
		XMMATRIX tmp = XMLoadFloat4x4(&xCamera);
		XMMATRIX res = XMMatrixMultiply(translation, tmp);
		XMStoreFloat4x4(&xCamera, res);
	}

	if (rMClick)
	{
		float x = currMPos.x - prevMPos.x;
		float y = currMPos.y - prevMPos.y;

		XMFLOAT4 pos = XMFLOAT4(xCamera._41, xCamera._42, xCamera._43, xCamera._44);

		xCamera._41 = 0.0f;
		xCamera._42 = 0.0f;
		xCamera._43 = 0.0f;

		XMMATRIX xRotate = XMMatrixRotationX(y * 0.01f);
		XMMATRIX yRotate = XMMatrixRotationY(x * 0.01f);

		XMMATRIX tmp = XMLoadFloat4x4(&xCamera);
		tmp = XMMatrixMultiply(xRotate, tmp);
		tmp = XMMatrixMultiply(tmp, yRotate);

		XMStoreFloat4x4(&xCamera, tmp);

		xCamera._41 = pos.x;
		xCamera._42 = pos.y;
		xCamera._43 = pos.z;

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
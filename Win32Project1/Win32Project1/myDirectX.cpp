#include "myDirectX.h"
#include "stdafx.h"

namespace
{
	myDirectX* app = nullptr;
}

LRESULT CALLBACK mainWndwProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (app)
		return app->msgProc(hWnd, msg, wParam, lParam);
	else
		return DefWindowProc(hWnd, msg, wParam, lParam);
}

myDirectX::myDirectX(HINSTANCE hInst)
{
	hAppInst = hInst;
	hAppwnd = NULL;
	clientH = 600;
	clientW = 800;
	appTitle = "PP5 Project";
	wndwStyle = WS_OVERLAPPEDWINDOW;
	app = this;

	device = nullptr;
	deviceContext = nullptr;
	renderTarget = nullptr;
	swapChain = nullptr;

}

myDirectX::~myDirectX()
{
	if (deviceContext)
		deviceContext->ClearState();

	renderTarget->Release();
	swapChain->Release();
	deviceContext->Release();
	device->Release();
}

int myDirectX::run()
{
	MSG m = { 0 };
	while (WM_QUIT != m.message)
	{
		if (PeekMessage(&m, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&m);
			DispatchMessage(&m);
		}
		else
		{
			update(0);
			render(0);
		}
	}
	return static_cast<int>(m.wParam);
}

bool myDirectX::init()
{
	if (!initWndw())
		return false;
	if (!initD3D())
		return false;
}

bool myDirectX::initWndw()
{
	WNDCLASSEX w;
	ZeroMemory(&w, sizeof(WNDCLASSEX));
	w.cbClsExtra = 0;
	w.cbWndExtra = 0;
	w.cbSize = sizeof(WNDCLASSEX);
	w.style = CS_HREDRAW | CS_VREDRAW;
	w.hInstance = hAppInst;
	w.lpfnWndProc = mainWndwProc;
	w.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	w.hCursor = LoadCursor(NULL, IDC_ARROW);
	w.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	w.lpszMenuName = NULL;
	w.lpszClassName = L"DXAPPWNDCLASS";
	w.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&w))
	{
		OutputDebugString(L"WINDOW CLASS CREATION FAILED");
		return false;
	}

	RECT r = { 0,0, clientW, clientH };
	AdjustWindowRect(&r, wndwStyle, FALSE);
	UINT _w = r.right - r.left;
	UINT _h = r.bottom - r.top;

	UINT x = GetSystemMetrics(SM_CXSCREEN) / 2 - _w / 2;
	UINT y = GetSystemMetrics(SM_CYSCREEN) / 2 - _h / 2;

	hAppwnd = CreateWindow(L"DXAPPWNDCLASS", L"PP5 PROJECT - KEVIN SZANTO", wndwStyle, x, y, _w, _h, NULL, NULL, hAppInst, NULL);

	if (!hAppwnd)
	{
		OutputDebugString(L"WINDOW CREATION FAILED");
		return false;
	}

	ShowWindow(hAppwnd, SW_SHOW);

	return true;


}

bool myDirectX::initD3D()
{
	UINT flags = 0;

#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	
	D3D_DRIVER_TYPE drivers[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};

	UINT driverLvls = ARRAYSIZE(drivers);

	D3D_FEATURE_LEVEL features[] = {
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_9_3
	};

	UINT featurelvls = ARRAYSIZE(features);

	DXGI_SWAP_CHAIN_DESC _swapChain;
	ZeroMemory(&_swapChain, sizeof(DXGI_SWAP_CHAIN_DESC));

	_swapChain.BufferCount = 1;
	_swapChain.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	_swapChain.BufferDesc.Height = clientH;
	_swapChain.BufferDesc.Width = clientW;
	_swapChain.BufferDesc.RefreshRate.Numerator = 60;
	_swapChain.BufferDesc.RefreshRate.Denominator = 1;
	_swapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	_swapChain.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	_swapChain.OutputWindow = hAppwnd;
	_swapChain.SampleDesc.Count = 1;
	_swapChain.SampleDesc.Quality = 0;
	_swapChain.Windowed = true;

	HRESULT result;

	for (unsigned int i = 0; i < featurelvls; i++)
	{
		result = D3D11CreateDeviceAndSwapChain(NULL, drivers[i], NULL, flags, features, featurelvls, D3D11_SDK_VERSION, &_swapChain, &swapChain, &device, &featureLevel, &deviceContext);

		if (SUCCEEDED(result))
		{
			driverType = drivers[i];
			break;
		}

	}

	if (FAILED(result))
	{
		OutputDebugString(L"DEVICE AND SWAPCHAIN FAILURE");
		return false;
	}

	ID3D11Texture2D* texture2d = 0;
	swapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&texture2d));
	device->CreateRenderTargetView(texture2d, nullptr, &renderTarget);

	deviceContext->OMSetRenderTargets(1, &renderTarget, nullptr);

	viewport.Width = static_cast<float>(clientW);
	viewport.Height = static_cast<float>(clientH);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	deviceContext->RSSetViewports(1, &viewport);

	return true;

}

void myDirectX::setNewMClick() 
{
	GetCursorPos(&currMPos);
	ScreenToClient(hAppwnd, &currMPos);
	prevMPos = currMPos;
}

LRESULT myDirectX::msgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_RBUTTONDOWN)
	{
		rMClick = true;
		setNewMClick();
	}
	else if (msg == WM_RBUTTONUP)
		rMClick = false;

	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}
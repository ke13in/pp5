#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <string>
#include <chrono>



class myDirectX
{
public:
	myDirectX(HINSTANCE myHinst);
	~myDirectX();

	int run();

	virtual bool init();
	virtual void update(float u) = 0;
	virtual void render(float r) = 0;

	virtual LRESULT msgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	bool rMClick = false;
	POINT currMPos;
	POINT prevMPos;

	HWND hAppWnd;
	HINSTANCE hAppInst;
	UINT clientWidth;
	UINT clientHeight;
	std::string appTitle;
	DWORD wndStyle;

	ID3D11Device * device;
	ID3D11DeviceContext * deviceContext;
	IDXGISwapChain * swapChain;
	ID3D11RenderTargetView * renderTarget;
	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;
	D3D11_VIEWPORT viewport;



private:

};

myDirectX::myDirectX()
{
}

myDirectX::~myDirectX()
{
}
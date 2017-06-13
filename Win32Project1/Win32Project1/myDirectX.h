#pragma once
#define WIN32_LEAN_AND_MEA

#include <Windows.h>
#include <string>
#include <stdio.h>
#include <chrono>
#include <D3D11.h>


class myDirectX
{
public:
	myDirectX(HINSTANCE hInstace);

	~myDirectX();

	int run();

	virtual bool init();
	virtual void update(float u) = 0;
	virtual void render(float r) = 0;
	virtual LRESULT msgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:

	bool rMClick;
	POINT currMPos;
	POINT prevMPos;
	float totalTime;
	float currTime;

	HWND hAppwnd;
	HINSTANCE hAppInst;
	UINT clientW;
	UINT clientH;
	std::string appTitle;
	DWORD wndwStyle;

	ID3D11Device * device;
	ID3D11DeviceContext * deviceContext;
	IDXGISwapChain * swapChain;
	ID3D11RenderTargetView * renderTarget;
	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;
	D3D11_VIEWPORT viewport;

	bool initWndw();

	bool initD3D();

	void setNewMClick();



};
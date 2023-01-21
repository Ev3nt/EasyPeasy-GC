#pragma once

#include "ImGUI/imgui.h"
#include <d3d11.h>
#include <string>
#include <map>

#define SPLIT_AFTER(str, sym) std::string(str).substr(std::string(str).find_last_of(sym) + 1)
#define MODULE_CONSTRUCTOR(m, n) m(): n(__FUNCTION__)
//#define MODULE_CONSTRUCTOR(m, n) m(): n(SPLIT_AFTER(__FUNCTION__, ":"))

class IOverlayModule {
public:
	virtual void Update(bool isDisplay) = 0;
	virtual const std::string& GetName() = 0;
};

class Overlay {
	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_deviceContext = nullptr;
	IDXGISwapChain* m_swapChain = nullptr;

	std::map<std::string, std::unique_ptr<IOverlayModule>> m_modules;

	bool m_display = false;
public:
	Overlay(HWND wnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext, IDXGISwapChain* swapChain);
	~Overlay();

	template <typename Module>
	void AddModule();

	void Render();
	bool WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void Show(bool state);

	bool IsShown();
};
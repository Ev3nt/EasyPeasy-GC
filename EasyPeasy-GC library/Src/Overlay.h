#pragma once

#include <d3d11.h>
#include <map>

#define SPLIT_AFTER(str, sym) std::string(str).substr(std::string(str).find_last_of(sym) + 1)
#define MODULE_CONSTRUCTOR(m, n) m(): n(__FUNCTION__)
//#define MODULE_CONSTRUCTOR(m, n) m(): n(SPLIT_AFTER(__FUNCTION__, ":"))

class Overlay {
	inline static ID3D11Device* m_device = nullptr;
	inline static ID3D11DeviceContext* m_deviceContext = nullptr;
	inline static IDXGISwapChain* m_swapChain = nullptr;

	inline static bool m_display = false;
	inline static bool m_created = false;
public:
	static void Create(HWND wnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext, IDXGISwapChain* swapChain);
	static void Destroy();

	static void Render();
	static bool WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static void Show(bool state);

	static bool IsShown();
	static bool IsCreated();
};
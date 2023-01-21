#include "pch.h"

#include "kiero.h"
#include "D3D11 Hooks.h"
#include "Window Hooks.h"
#include "ImGUI/imgui_impl_dx11.h"
#include "ModuleManager.h"

namespace D3D11 {
	inline static ID3D11Device* n_device = nullptr;
	inline static ID3D11RenderTargetView* n_renderTargetView = nullptr;
	inline static ID3D11DeviceContext* n_deviceContext = nullptr;

	void CreateRenderTarget(IDXGISwapChain* swapChain) {
		DXGI_SWAP_CHAIN_DESC sd;
		swapChain->GetDesc(&sd);
		ID3D11Texture2D* backBuffer;
		D3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc;
		ZeroMemory(&render_target_view_desc, sizeof(render_target_view_desc));
		render_target_view_desc.Format = sd.BufferDesc.Format;
		render_target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
		n_device->CreateRenderTargetView(backBuffer, &render_target_view_desc, &n_renderTargetView);
		backBuffer->Release();
	}

	void CleanupRenderTarget() {
		if (n_renderTargetView) {
			n_renderTargetView->Release();
			n_renderTargetView = nullptr;
		}
	}

	HRESULT WINAPI Present(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) {
		if (!Overlay::IsCreated()) {
			LOG_INFO("Initializing ImGUI...");

			if (SUCCEEDED(swapChain->GetDevice(__uuidof(n_device), (PVOID*)&n_device))) {
				n_device->GetImmediateContext(&n_deviceContext);

				DXGI_SWAP_CHAIN_DESC desc;
				swapChain->GetDesc(&desc);
				HWND wnd = desc.OutputWindow;

				CreateRenderTarget(swapChain);
				Overlay::Create(wnd, n_device, n_deviceContext, swapChain);
				Window::SetProcessWindow(wnd);
				Window::SetWindowHooksState(true);
			}
		} else {
			n_deviceContext->OMSetRenderTargets(1, &n_renderTargetView, NULL);
			Overlay::Render();
		}

		return HookManager::Invoke(Present, swapChain, syncInterval, flags);
	}

	HRESULT WINAPI ResizeBuffers(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags) {
		bool isCreated = Overlay::IsCreated();
		if (isCreated) {
			ImGui_ImplDX11_InvalidateDeviceObjects();
			CleanupRenderTarget();
		}
		HRESULT result = HookManager::Invoke(ResizeBuffers, swapChain, bufferCount, width, height, newFormat, swapChainFlags);
		if (isCreated) {
			CreateRenderTarget(swapChain);
			ImGui_ImplDX11_CreateDeviceObjects();
		}

		return result;
	}

	void SetD3D11HooksState(bool state) {
		if (state) {
			kiero::Status::Enum status = kiero::init(kiero::RenderType::D3D11);

			if (status == kiero::Status::Success) {
				HookManager::Attach(kiero::getMethodsTable()[8], Present, "D3D11::Present");
				HookManager::Attach(kiero::getMethodsTable()[13], ResizeBuffers, "D3D11::ResizeBuffers");
			}
			else {
				LOG_ERROR(Utils::format("Kiero D3D11 init error: %d", status));
			}
		}
		else {
			HookManager::Detach(Present);
		}
	}
}
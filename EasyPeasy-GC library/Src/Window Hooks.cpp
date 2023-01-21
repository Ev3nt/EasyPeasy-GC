#include "pch.h"

#include "Window Hooks.h"

namespace Window {
	HWND n_wnd = nullptr;
	WNDPROC n_wndProc = nullptr;

	LRESULT WINAPI WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		bool status = false;

		Overlay::IsCreated() ? status = Overlay::WndProc(wnd, msg, wParam, lParam) : false;

		return status ? DefWindowProc(wnd, msg, wParam, lParam) : CallWindowProc(n_wndProc, wnd, msg, wParam, lParam);
	}

	BOOL WINAPI ClipCursor(const RECT* lpRect) {
		return HookManager::Invoke(ClipCursor, Overlay::IsShown() ? NULL : lpRect);
	}

	void SetProcessWindow(HWND wnd) {
		n_wnd = wnd;
	}

	void SetWindowHooksState(bool state) {
		if (state) {
			n_wndProc = (WNDPROC)SetWindowLongPtr(n_wnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
			HookManager::Attach(::ClipCursor, ClipCursor, "Window::ClipCursor");
		}
		else if (n_wndProc) {
			SetWindowLongPtr(n_wnd, GWLP_WNDPROC, (LONG_PTR)n_wndProc);
			HookManager::Detach(ClipCursor);
		}
	}
}
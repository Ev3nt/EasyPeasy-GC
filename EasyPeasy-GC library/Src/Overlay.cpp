#include "pch.h"

#include "Overlay.h"
#include "ImGUI/imgui_impl_dx11.h"
#include "ImGUI/imgui_impl_win32.h"
#include "ImGUI/imgui_memory_editor.h"

#include "FPS.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Overlay* overlay = nullptr;

Overlay::Overlay(HWND wnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext, IDXGISwapChain* swapChain): m_device(device), m_deviceContext(deviceContext), m_swapChain(swapChain) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    ImGui_ImplWin32_Init(wnd);
    ImGui_ImplDX11_Init(m_device, m_deviceContext);

    AddModule<FPS>();
}

Overlay::~Overlay() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

template <typename Module>
void Overlay::AddModule() {
    auto module = std::unique_ptr<IOverlayModule>(new Module);
    const std::string& name = module.get()->GetName();

    if (!m_modules.count(name)) {
        m_modules[name] = std::move(module);

        return;
    }

    LOG_WARNING(Utils::format(R"(Attempt to add existing module "%s")", name.data()));
}

void Overlay::Render() {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGuiIO& io = ImGui::GetIO();
    io.MouseDrawCursor = m_display || (io.WantCaptureKeyboard || io.WantCaptureMouse || io.WantTextInput);

    // Overlay

    for (const auto& module : m_modules) {
        module.second.get()->Update(m_display);
    }

    if (m_display) {
        if (ImGui::Begin("Example", nullptr, ImGuiWindowFlags_NoScrollbar)) {
            ImGuiListClipper clipper;
            clipper.Begin(10, ImGui::GetTextLineHeight());
            
            while (clipper.Step()) {
                ImGui::Text("Salam");
            }
        }

        ImGui::End();
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Insert)) {
        static RECT rect{};
        if (!m_display) {
            GetClipCursor(&rect);
        }

        Show(!m_display);

        ClipCursor(&rect);
    }

    // Draw overlay

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

bool Overlay::WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplWin32_WndProcHandler(wnd, msg, wParam, lParam);

    return (io.WantCaptureKeyboard || io.WantCaptureMouse || io.WantTextInput);
}

void Overlay::Show(bool state) {
    m_display = state;
}

bool Overlay::IsShown() {
    return m_display;
}
#include "pch.h"

#include "Overlay.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_dx11.h"
#include "ImGUI/imgui_impl_win32.h"
#include "ImGUI/imgui_memory_editor.h"
#include <fstream>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void Overlay::Create(HWND wnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext, IDXGISwapChain* swapChain) {
    if (m_device) {
        LOG_ERROR(Utils::format(R"(Attempt to create existing "%s")", __FUNCTION__));

        return;
    }

    m_device = device, m_deviceContext = deviceContext, m_swapChain = swapChain, m_created = true;
    std::ifstream json(GC_CONFIG);
    config = nlohmann::json::parse(json, nullptr, false, false);
    json.close();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    ImGui_ImplWin32_Init(wnd);
    ImGui_ImplDX11_Init(m_device, m_deviceContext);

    ModuleManager::Init();
}

void Overlay::Destroy() {
    if (!m_device) {
        return;
    }

    m_device = nullptr, m_deviceContext = nullptr, m_swapChain = nullptr, m_created = false;

    ModuleManager::Destroy();

    Overlay::SaveConfig();
    config.clear();

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void Overlay::SaveConfig() {
    if (config.empty()) {
        return;
    }

    std::ofstream json(GC_CONFIG);
    json << config.dump(4);
    json.close();
}

UINT Overlay::GetID() {
    return 0;
}

//template <typename Module>
//void Overlay::AddModule() {
//    auto module = std::unique_ptr<IOverlayModule>(new Module);
//    const std::string& name = module.get()->GetName();
//
//    if (!m_modules.count(name)) {
//        m_modules[name] = std::move(module);
//
//        return;
//    }
//
//    LOG_WARNING(Utils::format(R"(Attempt to add existing module "%s")", name.data()));
//}

void Overlay::Render() {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGuiIO& io = ImGui::GetIO();
    io.MouseDrawCursor = m_display || (io.WantCaptureKeyboard || io.WantCaptureMouse || io.WantTextInput);

    // Overlay

    ModuleManager::Update();

    if (ImGui::IsKeyPressed(ImGuiKey_Insert)) {
        static RECT rect{};
        if (!m_display) {
            GetClipCursor(&rect);
        }

        m_display = !m_display;

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

bool Overlay::IsCreated() {
    return m_created;
}
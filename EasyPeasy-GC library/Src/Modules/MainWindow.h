#pragma once

#include "../IModule.h"

#include "FPS.h"

class MainWindow : public IModule {
	UINT m_id = GetID();
public:
	MainWindow() {
		ModuleManager::Add<FPS>(m_id);
	}

	void Update(UINT ownerID) {
		if (!Overlay::IsShown()) {
			return;
		}

		ImGui::Begin(GetName().data(), NULL, ImGuiWindowFlags_NoSavedSettings);
		ImGui::Columns(2);
		ImGui::Checkbox("Show FPS", &FPS::show);
		ImGui::NextColumn();
		ModuleManager::Update(m_id);
		ImGui::End();
	}
};
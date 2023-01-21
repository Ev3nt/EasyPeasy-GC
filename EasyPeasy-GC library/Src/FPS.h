#pragma once

#include "Overlay.h"

class FPS : public IOverlayModule {
	std::string m_name;
public:
	MODULE_CONSTRUCTOR(FPS, m_name) {}

	void Update(bool isDisplay) {
		ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);
		ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
		ImGui::End();
	}

	const std::string& GetName() {
		return m_name;
	}
};
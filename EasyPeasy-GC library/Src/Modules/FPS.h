#pragma once

#include "../IModule.h"

class FPS : public IModule {
	std::string m_name = GetName();
public:
	inline static bool show = false;

	FPS() {
		show = Overlay::config[m_name]["Show"].is_boolean() ? Overlay::config[m_name]["Show"].get<bool>() : false;
	}

	void Destroy() {
		Overlay::config[m_name]["Show"] = show;
	}

	void Update(UINT ownerID) {
		if (!show) {
			return;
		}

		bool isOverlay = ownerID == Overlay::GetID();
		if (isOverlay) {
			ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar);
			ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
			ImGui::End();
		}
		else {
			ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
		}

		//Overlay::config["Show FPS"] = showFPS;
	}
};
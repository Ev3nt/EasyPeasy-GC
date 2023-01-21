#pragma once

#include "../IModule.h"

class FPS : public IModule {
public:
	FPS() {}

	void Update() {
		ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar);
		ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
		ImGui::End();
	}
};

class StatusDisplayFPS : public IModule {
public:
	StatusDisplayFPS() {}

	void Update() {
		ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
	}
};

class StatusDisplay : public IModule {
public:
	StatusDisplay() { }

	void Update() {
		ImGui::Begin(GetName().data(), nullptr, ImGuiWindowFlags_NoSavedSettings);
		
		ImGui::Text(__MODULE_METHOD__);

		ModuleManager::Update("StatusDisplay");

		ImGui::End();
	}
};
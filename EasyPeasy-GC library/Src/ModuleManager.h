#pragma once

#include "IModule.h"
#include "ImGUI/imgui.h"

#include "Modules/FPS.h"

#define SPLIT_AFTER(str, sym) std::string(str).substr(std::string(str).find_last_of(sym) + 1)
#define MODULE_CONSTRUCTOR(m, n) m(): n(__FUNCTION__)
//#define MODULE_CONSTRUCTOR(m, n) m(): n(SPLIT_AFTER(__FUNCTION__, ":"))

class ModuleManager {
	inline static std::map<std::string, std::unique_ptr<IModule>> m_modules;
public:
	static void Init() {
		Add<FPS>();
	}

	template <typename Module>
	static void Add() {
		auto module = std::unique_ptr<IModule>(new Module);
		const std::string& name = module.get()->GetName();

		if (!m_modules.count(name)) {
			m_modules[name] = std::move(module);

			//return name;
		}

		LOG_WARNING(Utils::format(R"(Attempt to add existing module "%s")", name.data()));
	}

	static void Update() {
		for (const auto& module : m_modules) {
			module.second.get()->Update();
		}
	}
};
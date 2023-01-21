#pragma once

#include "IModule.h"
#include "ImGUI/imgui.h"

//#define SPLIT_AFTER(str, sym) std::string(str).substr(std::string(str).find_last_of(sym) + 1)
//#define MODULE_CONSTRUCTOR(m, n) m(): n(__FUNCTION__)
//#define MODULE_CONSTRUCTOR(m, n) m(): n(SPLIT_AFTER(__FUNCTION__, ":"))

class ModuleManager {
	inline static std::map<std::string, std::vector<std::unique_ptr<IModule>>> m_modules;
public:
	static void Init();

	template <typename Module>
	static void Add(const std::string& category) {
		auto module = std::unique_ptr<IModule>(new Module);
		m_modules[category].push_back(std::move(module));
	}

	static void Update(const std::string& category) {
		auto modules = m_modules.find(category);
		if (modules != m_modules.end()) {
			for (const auto& module : modules->second) {
				module.get()->Update();
			}
		}
	}
};
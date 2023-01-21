#pragma once

#include "IModule.h"
#include "ImGUI/imgui.h"

//#define SPLIT_AFTER(str, sym) std::string(str).substr(std::string(str).find_last_of(sym) + 1)
//#define MODULE_CONSTRUCTOR(m, n) m(): n(__FUNCTION__)
//#define MODULE_CONSTRUCTOR(m, n) m(): n(SPLIT_AFTER(__FUNCTION__, ":"))

class ModuleManager {
	inline static std::map<UINT, std::vector<std::unique_ptr<IModule>>> m_modules;
public:
	static void Init();

	template <typename Module>
	static void Add(UINT ownerID = 0) {
		auto module = std::unique_ptr<IModule>(new Module);
		m_modules[ownerID].push_back(std::move(module));
	}

	static void Update(UINT ownerID = 0) {
		auto modules = m_modules.find(ownerID);
		if (modules != m_modules.end()) {
			for (const auto& module : modules->second) {
				module.get()->Update(ownerID);
			}
		}
	}

	static void Destroy() {
		for (auto& owner : m_modules) {
			for (auto& module : owner.second) {
				module.get()->Destroy();
			}
		}

		m_modules.clear();
	}
};
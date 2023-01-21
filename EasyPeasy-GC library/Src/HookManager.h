#pragma once

#include <map>
#include "Detours.h"

class HookManager {
	// Detour, Function
	inline static std::map<LPVOID, std::pair<LPVOID, std::string>> m_detours;
public:
	template <typename Function, typename Detour>
	static BOOL Attach(Function function, Detour detour, const std::string& name = nullptr) {
		m_detours[(LPVOID)detour] = { (LPVOID)function, name };

		BOOL status;
		if (status = Detours::Install(&m_detours[(LPVOID)detour].first, (LPVOID)detour)) {
			name.empty() ? LOG_ERROR(Utils::format("Failed to attach, error: %d", status)) : LOG_ERROR(Utils::format(R"(Failed to attach "%s", error: %d)", name.data(), status));
			m_detours.erase((LPVOID)detour);
		}

		return status;
	}

	template <typename Detour>
	static BOOL Detach(Detour detour) {
		std::pair<LPVOID, std::string>& function = m_detours[(LPVOID)detour];
		std::string name = function.second;

		BOOL status;
		if (status = Detours::Uninstall(&function.first, (LPVOID)detour)) {
			name.empty() ? LOG_ERROR(Utils::format("Failed to detach, error: %d", status)) : LOG_ERROR(Utils::format(R"(Failed to detach "%s", error: %d)", name.data(), status));
		}
		else {
			m_detours.erase((LPVOID)detour);
		}

		return status;
	}

	template <typename Detour>
	static Detour GetOrigin(Detour detour) {
		return (Detour)m_detours[(LPVOID)detour].first;
	}

	template <typename Return, typename ...Args>
	static Return Invoke(Return(*detour)(Args...), Args ... args) {
		auto origin = GetOrigin(detour);

		return origin ? origin(args...) : Return();
	}
};
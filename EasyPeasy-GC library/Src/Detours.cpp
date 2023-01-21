#include "pch.h"

#include "Detours.h"

namespace Detours {
	std::vector<PVOID> Hooked;

	DWORD Install(PVOID* pointer_ptr, PVOID detour) {
		DWORD error = NO_ERROR;
		auto item = std::find(Hooked.begin(), Hooked.end(), pointer_ptr); if (item != Hooked.end()) { LOG_ERROR("Detour::Install aborted, function already detoured!\n"); return error; }

		if ((error = DetourTransactionBegin()) != NO_ERROR) { LOG_ERROR(Utils::format("DetourTransactionBegin failed with error: %d.\n", error)); }
		if (!error && (error = DetourUpdateThread(::GetCurrentThread())) != NO_ERROR) { LOG_ERROR(Utils::format("DetourUpdateThread failed with error: %d.\n", error)); }
		if (!error && (error = DetourAttach(pointer_ptr, detour)) != NO_ERROR) { LOG_ERROR(Utils::format("DetourAttach failed with error: %d.\n", error)); }
		if (!error && (error = DetourTransactionCommit()) != NO_ERROR) { LOG_ERROR(Utils::format("DetourTransactionCommit failed with error: %d.\n", error)); }

		if (error == NO_ERROR) { Hooked.push_back(pointer_ptr); } else { DetourTransactionAbort(); }
		
		return error;
	}

	DWORD Uninstall(PVOID* pointer_ptr, PVOID detour) {
		DWORD error = NO_ERROR;
		auto item = std::find(Hooked.begin(), Hooked.end(), pointer_ptr); if (item == Hooked.end()) { LOG_ERROR("Detour::Uninstall aborted, function was not detoured!\n"); return error; }

		if ((error = DetourTransactionBegin()) != NO_ERROR) { LOG_ERROR(Utils::format("DetourTransactionBegin failed with error: %d.\n", error)); }
		if (!error && (error = DetourUpdateThread(::GetCurrentThread())) != NO_ERROR) { LOG_ERROR(Utils::format("DetourUpdateThread failed with error: %d.\n", error)); }
		if (!error && (error = DetourDetach(pointer_ptr, detour)) != NO_ERROR) { LOG_ERROR(Utils::format("DetourAttach failed with error: %d.\n", error)); }
		if (!error && (error = DetourTransactionCommit()) != NO_ERROR) { LOG_ERROR(Utils::format("DetourTransactionCommit failed with error: %d.\n", error)); }

		if (error == NO_ERROR) { Hooked.erase(item); } else { DetourTransactionAbort(); }
		
		return error;
	}

	DWORD InstallEx(bool flag, PVOID* pointer_ptr, PVOID detour) {
		if (pointer_ptr && *pointer_ptr) {
			return flag ? Install(pointer_ptr, detour) : Uninstall(pointer_ptr, detour);
		}

		LOG_ERROR("Detour::InstallEx failure!\n");

		return -1;
	}

	DWORD SetState(bool flag, PVOID* pointer_ptr, PVOID detour) {
		return flag ? Install(pointer_ptr, detour) : Uninstall(pointer_ptr, detour);
	}
}
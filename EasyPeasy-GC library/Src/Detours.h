#pragma once

#include <detours.h>
#include <vector>

namespace Detours {
	DWORD Install(PVOID* pointer_ptr, PVOID detour);
	DWORD Uninstall(PVOID* pointer_ptr, PVOID detour);
	DWORD InstallEx(bool flag, PVOID* pointer_ptr, PVOID detour);
	DWORD SetState(bool flag, PVOID* pointer_ptr, PVOID detour);
}
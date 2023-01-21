#include "pch.h"

#include "D3D11 Hooks.h"

void MainThread() {
	Logger::OpenConsole(GC_NAME);
	
	LOG_INFO("Waiting for the game to load...");
	while (!GetModuleHandle("d3d11.dll"));
	LOG_INFO("Game found");
	
	D3D11::SetD3D11HooksState(true);
}

BOOL APIENTRY DllMain(HMODULE module, UINT reason, LPVOID reserved) {
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MainThread, NULL, NULL, NULL));

		break;
	case DLL_PROCESS_DETACH:

		break;
	}

	return TRUE;
}
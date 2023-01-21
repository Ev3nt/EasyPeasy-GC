#pragma once

#pragma comment(lib,"winmm.lib")

namespace SpeedHack {
	typedef DWORD(WINAPI* GetProcessTickCount)();
	extern GetProcessTickCount GetTickCount;

	typedef ULONGLONG(WINAPI* GetProcessTickCount64)();
	extern GetProcessTickCount64 GetTickCount64;

	typedef BOOL(WINAPI* QueryProcessPerformanceCounter)(LARGE_INTEGER* lpPerformanceCounter);
	extern QueryProcessPerformanceCounter QueryPerformanceCounter;

	typedef DWORD(WINAPI* timeProcessGetTime)();
	extern timeProcessGetTime timeGetTime;

	void SetSpeed(float speed);

	namespace Hooks {
		DWORD WINAPI GetTickCount();
		ULONGLONG WINAPI GetTickCount64();
		BOOL WINAPI QueryPerformanceCounter(LARGE_INTEGER* lpPerformanceCounter);
		DWORD WINAPI timeGetTime();
	};
}
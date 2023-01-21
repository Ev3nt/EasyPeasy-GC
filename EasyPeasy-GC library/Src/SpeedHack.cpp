#include "pch.h"

#include "SpeedHack.h"

namespace SpeedHack {
	DWORD tickCount;
	ULONGLONG tickCount64;
	LARGE_INTEGER performanceCounter;
	DWORD time;
	float speed = 1.f;

	GetProcessTickCount GetTickCount = ::GetTickCount;
	GetProcessTickCount64 GetTickCount64 = ::GetTickCount64;
	QueryProcessPerformanceCounter QueryPerformanceCounter = ::QueryPerformanceCounter;
	timeProcessGetTime timeGetTime = ::timeGetTime;

	void SetSpeed(float _speed) {
		tickCount = GetTickCount();
		tickCount64 = GetTickCount64();
		QueryPerformanceCounter(&performanceCounter);
		time = timeGetTime();
		speed = _speed;
	}

	namespace Hooks {
		DWORD WINAPI GetTickCount() {
			DWORD current = SpeedHack::GetTickCount();
			return (DWORD)((current - tickCount) * speed) + current;
		}

		ULONGLONG WINAPI GetTickCount64() {
			ULONGLONG current = SpeedHack::GetTickCount64();
			return (ULONGLONG)((current - tickCount64) * speed) + current;
		}

		BOOL WINAPI QueryPerformanceCounter(LARGE_INTEGER* lpPerformanceCounter) {
			LARGE_INTEGER current;
			BOOL status = SpeedHack::QueryPerformanceCounter(&current);
			lpPerformanceCounter->QuadPart = (LONGLONG)((current.QuadPart - performanceCounter.QuadPart) * speed) + current.QuadPart;

			return status;
		}

		DWORD WINAPI timeGetTime() {
			DWORD current = SpeedHack::timeGetTime();
			return (DWORD)((current - time) * speed) + current;
		}
	}
}
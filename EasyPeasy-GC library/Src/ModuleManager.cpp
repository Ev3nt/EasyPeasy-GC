#include "pch.h"

#include "ModuleManager.h"

#include "Modules/FPS.h"

void ModuleManager::Init() {
	Add<FPS>("overlay");
	Add<StatusDisplay>("overlay");
	Add<StatusDisplayFPS>("StatusDisplay");
}
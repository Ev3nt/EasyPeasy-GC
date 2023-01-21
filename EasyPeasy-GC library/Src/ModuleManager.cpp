#include "pch.h"

#include "ModuleManager.h"

#include "Modules/FPS.h"
#include "Modules/MainWindow.h"

void ModuleManager::Init() {
	Add<FPS>();
	Add<MainWindow>();
}
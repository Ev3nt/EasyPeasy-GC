#pragma once

#include <string>

class IModule {
public:
	virtual void Update() = 0;
	virtual const std::string& GetName() = 0;
};
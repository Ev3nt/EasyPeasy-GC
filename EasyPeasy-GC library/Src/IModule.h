#pragma once

#include <string>

#define __MODULE_METHOD__ __func__

class IModule {
public:
	virtual void Update() = 0;
	std::string GetName() const {
		return &(typeid(*this).name()[6]);
	}
};
#pragma once

#include <string>

#define __MODULE_METHOD__ __func__

template <typename T>
std::string GetModuleName() {
	return &(typeid(T).name()[6]);
}

class IModule {
	inline static UINT counter = 1;
	const UINT id = counter++;
public:
	virtual void Destroy() {};
	virtual void Update(UINT ownerID = 0) {};

	UINT GetID() const {
		return id;
	}

	std::string GetName() const {
		return &(typeid(*this).name()[6]);
	}
};
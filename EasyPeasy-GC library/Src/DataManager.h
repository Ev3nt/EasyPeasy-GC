#pragma once

#include <Windows.h>
#include <string>
#include <map>

class Container {
	LPVOID m_data;
	LPVOID type;
public:
	template<typename T>
	void Set(T data) {
		m_data = (LPVOID)(new T);
		*(T*)m_data = data;

		type = typeid(T)
	}

	template<typename T>
	T& Get() {
		return *(T*)m_data;
	}

	auto GetType() {
		return type;
	}
};

class DataManager {
	std::map <std::string, Container> m_data;
public:
	Container& operator[](const std::string& key) {

		return m_data[key];
	}
};
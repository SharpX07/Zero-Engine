#pragma once
#include <iostream>

class IComponent {
public:
	std::string Name;
	inline std::string GetComponentName() const {
		return Name;
	}
};

class ComponenteColor: public IComponent
{

};
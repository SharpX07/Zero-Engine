#pragma once

#include <iostream>
#include <glm.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


class BaseApp
{
public:
	BaseApp() {};
	virtual void setup() const = 0;
	virtual void run() const = 0;
	virtual void stop() const = 0;
};
#pragma once

#include <glad/glad.h>

#include <string>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "Mesh.h"

namespace Zero
{
	class ModelReader {
	public:
		ModelReader(const char* texturePath);
		std::vector<mesh::MeshVertex> vertices;
		std::vector<unsigned int> indices;
	};
}

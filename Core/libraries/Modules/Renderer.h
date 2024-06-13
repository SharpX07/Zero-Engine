#pragma once
#include <glad/glad.h>
#include <ResourceManagement/Mesh.h>
#include <ResourceManagement/Model.h>
#include <Scene/Scene.h>

namespace Zero
{
	class Renderer
	{
	public:
		struct Color
		{
			GLfloat r, g, b, a;
		};
		// Constructor
		Renderer() = default;

		~Renderer() = default;

		inline static const void SetViewport(GLint x, GLint y, GLsizei w, GLsizei h)
		{
			glViewport(x, y, w, h);
		};

		inline static const void Clear(Color c)
		{
			glClearColor(c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, c.a / 255.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		inline static const void EnableCapability(GLenum capability) { glEnable(capability); }
		static void Render(Model& model, Shader& shader);
		static void Render(Scene& scene);
		static void InitializeRenderer();
		GLTexture noTextureSample = GLTexture("Assets/Core/Textures/missing_texture.jpg");
	};


}

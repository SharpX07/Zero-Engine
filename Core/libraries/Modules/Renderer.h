#pragma once
#include <glad/glad.h>
#include <ResourceManagement/Mesh.h>
#include <ResourceManagement/Model.h>
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

		inline const void SetViewport(GLint x, GLint y, GLsizei w, GLsizei h) const
		{
			glViewport(x, y, w, h);
		};

		inline const void Clear(Color c) const
		{
			glClearColor(c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, c.a / 255.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		inline const void EnableCapability(GLenum capability) const { glEnable(capability); }
		void Render(Model& model, Shader& shader);
		std::unique_ptr<GLTexture> noTextureSample;
		void InitializeRenderer();
	};


}

#include "OpenGLRenderer.h"

#include <glad/glad.h>

namespace ShaderSystem
{
	OpenGLRenderer::OpenGLRenderer()
	{
	}

	OpenGLRenderer::~OpenGLRenderer()
	{
	}
	
	void OpenGLRenderer::BeginFrame(uint32_t inWindowWidth, uint32_t inWindowHeight, const glm::vec4 &inClearColor)
	{
		glClearColor(inClearColor.r, inClearColor.g, inClearColor.b, inClearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, inWindowWidth, inWindowHeight);
	}

	void OpenGLRenderer::EndFrame(uint32_t inIndexCount)
	{
		glDrawElements(GL_TRIANGLES, inIndexCount, GL_UNSIGNED_INT, nullptr);
	}
}

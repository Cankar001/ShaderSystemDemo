#include <iostream>

#include "Core/Logger.h"
#include "Core/FileSystem.h"
#include "Window/Window.h"
#include "Renderer/Renderer.h"
#include "Renderer/FPSCamera.h"

#include "Graphics/Shader.h"
#include "Graphics/ShaderUniformBufferSet.h"
#include "Graphics/BufferLayout.h"

#include "Events/WindowEvent.h"

// TODO: remove include (TMP)
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace ShaderSystem;

static bool sRunning = false;
static Ref<FPSCamera> fpsCamera;
static bool onEvent(Event& e);
static bool onWindowResize(WindowResizeEvent& e);
static bool onWindowClose(WindowCloseEvent& e);

// TODO: Move to renderer
static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:    return GL_FLOAT;
	case ShaderDataType::Float2:   return GL_FLOAT;
	case ShaderDataType::Float3:   return GL_FLOAT;
	case ShaderDataType::Float4:   return GL_FLOAT;
	case ShaderDataType::Mat3:     return GL_FLOAT;
	case ShaderDataType::Mat4:     return GL_FLOAT;
	case ShaderDataType::Int:      return GL_INT;
	case ShaderDataType::Int2:     return GL_INT;
	case ShaderDataType::Int3:     return GL_INT;

		// OpenGL has problems with uploading Int4s on the GPU unless
		// they are defined as GL_FLOAT.
	case ShaderDataType::Int4:     return GL_FLOAT;

	case ShaderDataType::Bool:     return GL_BOOL;
	}

	return 0;
}

// USER DATA BEGIN

struct Vertex
{
	glm::vec3 Position;
	glm::vec2 TexCoord;
	glm::vec4 Color;
};

struct GlobalUBO
{
	glm::mat4 Projection;
	glm::mat4 View;
};

struct InstanceUBO
{
	glm::mat4 Model;
};

// USER DATA END

int main(int argc, char* argv[])
{
	Logger::Init();

	// TODO: TEMP until ready to run in solo app (outside vs, then the assets folder has to be in the same directory)
	//FileSystem::SetCurrentWorkingDirectory("../../../");

	WindowData properties;
	properties.Title = "ShaderSystem Demo";
	properties.Width = 1280;
	properties.Height = 720;
	properties.Maximized = true;

	Ref<Window> mainWindow = Window::Create(properties);
	mainWindow->SetEventCallback(SHADER_SYSTEM_BIND_EVENT_FN(onEvent));

	Renderer::Init();

	Ref<UniformBufferSet> uniformBufferSet = UniformBufferSet::Create(1);
	Ref<Shader> flatColorShader = Renderer::GetShaderLibrary()->Get("FlatColorShader");
	fpsCamera = MakeRef<FPSCamera>(CameraProjection::Orthographic, 1280, 720);

	glm::vec4 quadColor(1.0f, 0.0f, 0.0f, 1.0f);

	Vertex* vertices = new Vertex[4];
	vertices[0].Position = glm::vec3(-0.5f, -0.5f, 0.0f);
	vertices[0].TexCoord = glm::vec2(0.0f, 0.0f);
	vertices[0].Color = quadColor;

	vertices[1].Position = glm::vec3(-0.5f, 0.5f, 0.0f);
	vertices[1].TexCoord = glm::vec2(0.0f, 0.0f);
	vertices[1].Color = quadColor;

	vertices[2].Position = glm::vec3(0.5f, 0.5f, 0.0f);
	vertices[2].TexCoord = glm::vec2(0.0f, 0.0f);
	vertices[2].Color = quadColor;

	vertices[3].Position = glm::vec3(0.5f, -0.5f, 0.0f);
	vertices[3].TexCoord = glm::vec2(0.0f, 0.0f);
	vertices[3].Color = quadColor;

	uint32_t indices[] = {
		0, 1, 2, 2, 3, 0
	};

	GlobalUBO cameraUBO;
	cameraUBO.Projection = fpsCamera->GetProjection();
	cameraUBO.View = fpsCamera->GetViewMatrix();

	InstanceUBO instanceUBO;
	instanceUBO.Model = glm::mat4(1.0f);

	// NOTE: only one global vao
	GLuint vao;
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// vertex buffer
	GLuint vertexBufferId;
	glCreateBuffers(1, &vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glNamedBufferData(vertexBufferId, sizeof(Vertex) * 4, vertices, GL_STATIC_DRAW);

	// Index buffer
	GLuint indexBufferId;
	glCreateBuffers(1, &indexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
	glNamedBufferData(indexBufferId, sizeof(uint32_t) * 6, indices, GL_STATIC_DRAW);

	// Global ubo uniform buffer
	std::vector<UniformVariable> globalUboLayout = {
		UniformVariable("projection", UniformLayoutDataType::Mat4, 1, 0, sizeof(glm::mat4)),
		UniformVariable("view", UniformLayoutDataType::Mat4, 1, 0, sizeof(glm::mat4))
	};
	uniformBufferSet->CreateUniform(sizeof(GlobalUBO), 0, globalUboLayout);
	 
	// Instance ubo uniform buffer
	std::vector<UniformVariable> instanceUboLayout = {
		UniformVariable("model", UniformLayoutDataType::Mat4, 1, 0, sizeof(glm::mat4)),
	};
	uniformBufferSet->CreateUniform(sizeof(InstanceUBO), 1, instanceUboLayout);

	// Vertex bindings
	BufferLayout layout = {
		BufferElement("a_Position", ShaderDataType::Float3, false),
		BufferElement("a_TexCoord", ShaderDataType::Float2, false),
		BufferElement("a_Color", ShaderDataType::Float4, false)
	};

	uint32_t attribIndex = 0;
	for (const auto& element : layout)
	{
		GLenum glBaseType = ShaderDataTypeToOpenGLBaseType(element.Type);
		glEnableVertexAttribArray(attribIndex);

		if (glBaseType == GL_INT)
		{
			glVertexAttribIPointer(attribIndex,
				element.GetComponentCount(),
				glBaseType,
				layout.GetStride(),
				(const void*)(intptr_t)element.Offset);
		}
		else
		{
			glVertexAttribPointer(attribIndex,
				element.GetComponentCount(),
				glBaseType,
				element.Normalized,
				layout.GetStride(),
				(const void*)(intptr_t)element.Offset);
		}
		++attribIndex;
	}

	sRunning = true;
	while (sRunning)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, mainWindow->GetWidth(), mainWindow->GetHeight());

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
		flatColorShader->Bind();

		// TODO: Update camera

		// Update camera properties
		cameraUBO.Projection = fpsCamera->GetProjection();
		cameraUBO.View = fpsCamera->GetViewMatrix();
		uniformBufferSet->GetUniform(0)->SetData(&cameraUBO, sizeof(GlobalUBO), 0);

		// Update Model data
		instanceUBO.Model = glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f}) * glm::rotate(glm::mat4(1.0f), 90.0f, {0.0f, 1.0f, 0.0f}) * glm::scale(glm::mat4(1.0f), {2.0f, 2.0f, 2.0f});
		uniformBufferSet->GetUniform(1)->SetData(&instanceUBO, sizeof(InstanceUBO), 0);
		
		// Bind all uniforms
		uniformBufferSet->ForEach([&](const Ref<UniformBuffer> &buffer)
		{
			buffer->Bind();
		});

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		mainWindow->Update();
	}

	SHADER_SYSTEM_INFO("Shutting down application.");

	// NOTE: Not really necessary, not cleaning global memory might also be faster than to clean it, because delete[]/delete results in a free() call, which is a OS function.
	//       And OS functions are slow.
	delete[] vertices;

	Renderer::Shutdown();
	Logger::Shutdown();
	return 0;
}

static bool onEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(SHADER_SYSTEM_BIND_EVENT_FN(onWindowClose));
	dispatcher.Dispatch<WindowResizeEvent>(SHADER_SYSTEM_BIND_EVENT_FN(onWindowResize));
	
	fpsCamera->OnEvent(e);
	return false;
}

static bool onWindowResize(ShaderSystem::WindowResizeEvent& e)
{
	SHADER_SYSTEM_INFO("Received application resize event to size {0}x{1}.", e.GetWidth(), e.GetHeight());
	return false;
}

static bool onWindowClose(ShaderSystem::WindowCloseEvent& e)
{
	SHADER_SYSTEM_INFO("Received application quit event.");
	sRunning = false;
	return true;
}

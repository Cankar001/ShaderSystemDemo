#include <iostream>

#include "Core/Logger.h"
#include "Core/FileSystem.h"
#include "Core/Input.h"
#include "Window/Window.h"
#include "Renderer/Renderer.h"
#include "Renderer/FPSCamera.h"

#include "Graphics/Shader.h"
#include "Graphics/ShaderUniformBufferSet.h"
#include "Graphics/BufferLayout.h"

#include "Interfaces/VertexArray.h"
#include "Interfaces/VertexBuffer.h"
#include "Interfaces/IndexBuffer.h"

#include "Events/WindowEvent.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace ShaderSystem;

static bool sRunning = false;
static Ref<FPSCamera> sFpsCamera;
static bool onEvent(Event& e);
static bool onWindowResize(WindowResizeEvent& e);
static bool onWindowClose(WindowCloseEvent& e);

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

int main(int argc, char* argv[])
{
	Logger::Init();

	WindowData properties;
	properties.Title = "ShaderSystem Demo";
	properties.Width = 1280;
	properties.Height = 720;
	properties.Maximized = true;

	Renderer::SelectRenderingBackend(RenderingAPIType::DirectX11);

	Ref<Window> mainWindow = Window::Create(properties);
	mainWindow->SetEventCallback(SHADER_SYSTEM_BIND_EVENT_FN(onEvent));

	Renderer::Init();

	// NOTE: In this prototype we will just render one frame per frame, 
	// in a more realistic example you would have up to 3 frames per frame, 
	// that could roughly be rendered at the same time.
	Ref<UniformBufferSet> uniformBufferSet = UniformBufferSet::Create(1);
	
	Ref<VertexArray> vao = VertexArray::Create();
	Ref<Shader> flatColorShader = Renderer::GetShaderLibrary()->Get("FlatColorShader");
	sFpsCamera = MakeRef<FPSCamera>(CameraProjection::Orthographic, properties.Width, properties.Height);

	glm::vec4 quadColorRed(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 quadColorGreen(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 quadColorBlue(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 quadColorYellow(1.0f, 1.0f, 0.0f, 1.0f);

	Vertex* vertices = new Vertex[4];
	vertices[0].Position = glm::vec3(-0.5f, -0.5f, 0.0f);
	vertices[0].TexCoord = glm::vec2(0.0f, 0.0f);
	vertices[0].Color = quadColorRed;

	vertices[1].Position = glm::vec3(0.5f, -0.5f, 0.0f);
	vertices[1].TexCoord = glm::vec2(1.0f, 0.0f);
	vertices[1].Color = quadColorGreen;

	vertices[2].Position = glm::vec3(0.5f, 0.5f, 0.0f);
	vertices[2].TexCoord = glm::vec2(1.0f, 1.0f);
	vertices[2].Color = quadColorBlue;

	vertices[3].Position = glm::vec3(-0.5f, 0.5f, 0.0f);
	vertices[3].TexCoord = glm::vec2(0.0f, 1.0f);
	vertices[3].Color = quadColorYellow;

	int32_t indices[] = {
		0, 1, 2, 2, 3, 0
	};

	GlobalUBO cameraUBO;
	cameraUBO.Projection = sFpsCamera->GetProjection();
	cameraUBO.View = sFpsCamera->GetViewMatrix();

	InstanceUBO instanceUBO;
	instanceUBO.Model = glm::mat4(1.0f);

	// vertex buffer
	Ref<VertexBuffer> vbo = VertexBuffer::Create(vertices, sizeof(Vertex) * 4);
	// NOTE: As soon as the buffer object is created, the memory can be freed, because the impl does make a copy and the data is uploaded to GPU right away.
	delete[] vertices;

	// Index buffer
	Ref<IndexBuffer> ibo = IndexBuffer::Create(indices, sizeof(int32_t) * 6);

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
	BufferLayout layout = BufferLayout::GetFlatColorShaderLayout();
	vao->Bind(layout);

	float frameTime = 0.0f;
	float lastFrameTime = 0.0f;
	float deltaTime = 1.0f;
	sRunning = true;
	while (sRunning)
	{
		mainWindow->SetTitle("ShaderSystem Demo [Current Renderer: " + Renderer::GetCurrentRenderingAPI()->ToString() + "]");

		Input::Update(); // NOTE: Does nothing for keyboards, would do actions for controllers.

		if (Input::IsKeyPressed(256)) // Escape
		{
			break;
		}

		sFpsCamera->UpdateControls(deltaTime);

		Renderer::BeginFrame(mainWindow->GetWidth(), mainWindow->GetHeight(), glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

		vbo->Bind();
		ibo->Bind();
		flatColorShader->Bind();

		// Update camera properties
		cameraUBO.Projection = sFpsCamera->GetProjection();
		cameraUBO.View = sFpsCamera->GetViewMatrix();

		// NOTE: DX11 uses transposed matrices.
		if (Renderer::GetCurrentRenderingAPI()->GetType() == RenderingAPIType::DirectX11)
		{
			cameraUBO.Projection[1][1] *= -1;
			cameraUBO.Projection = glm::transpose(cameraUBO.Projection);
			cameraUBO.View = glm::transpose(cameraUBO.View);
		}

		uniformBufferSet->GetUniform(0)->SetData(&cameraUBO, sizeof(GlobalUBO), 0);

		// Update Model data
		instanceUBO.Model = glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f}) * glm::rotate(glm::mat4(1.0f), 90.0f, {0.0f, 1.0f, 0.0f}) * glm::scale(glm::mat4(1.0f), {2.0f, 2.0f, 2.0f});
		if (Renderer::GetCurrentRenderingAPI()->GetType() == RenderingAPIType::DirectX11)
		{
			instanceUBO.Model = glm::transpose(instanceUBO.Model);
		}

		uniformBufferSet->GetUniform(1)->SetData(&instanceUBO, sizeof(InstanceUBO), 0);
		
		// Bind all uniforms
		uniformBufferSet->ForEach([&](const Ref<UniformBuffer> &buffer)
		{
			buffer->Bind();
		});

		Renderer::EndFrame(ibo->GetCount());

		float time = mainWindow->GetTime();
		frameTime = time - lastFrameTime;
		lastFrameTime = time;
		deltaTime = glm::min<float>(frameTime, 0.0333f);

		mainWindow->Update();
	}

	SHADER_SYSTEM_INFO("Shutting down application.");

	Renderer::Shutdown();
	Logger::Shutdown();
	return 0;
}

static bool onEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(SHADER_SYSTEM_BIND_EVENT_FN(onWindowClose));
	dispatcher.Dispatch<WindowResizeEvent>(SHADER_SYSTEM_BIND_EVENT_FN(onWindowResize));
	
	sFpsCamera->OnEvent(e);
	return false;
}

static bool onWindowResize(WindowResizeEvent& e)
{
	SHADER_SYSTEM_INFO("Received application resize event to size {0}x{1}.", e.GetWidth(), e.GetHeight());
	return false;
}

static bool onWindowClose(WindowCloseEvent& e)
{
	SHADER_SYSTEM_INFO("Received application quit event.");
	sRunning = false;
	return true;
}

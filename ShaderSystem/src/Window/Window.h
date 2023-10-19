#pragma once

#include <string>
#include <functional>
#include <memory>
#include <ostream>

#include "Core/Core.h"
#include "Events/Event.h"

#define SHADER_SYSTEM_USE_NATIVE_WIN_API 0

namespace ShaderSystem
{
	class RenderingContext;

	using EventCallbackFn = std::function<void(Event&)>;

	struct WindowData
	{
		uint32_t Width;
		uint32_t Height;
		std::string Title;
		bool Fullscreen = false;
		bool Maximized = false;
		bool Focused = false;
		bool CursorVisible = true;
		bool Centered = false;
		bool VSync = true;
		bool Visible = true;
		EventCallbackFn EventCallback = nullptr;

		WindowData(bool fullscreen = false, uint32_t width = 1270, uint32_t height = 720, const std::string& title = "ShaderSystem")
			: Fullscreen(fullscreen), Width(width), Height(height), Title(title) {}
	};

	class Window
	{
	public:

		virtual ~Window() {}

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual const EventCallbackFn& GetEventCallback() const = 0;

		virtual void Update() = 0;

		virtual void SetWindowIcon(const std::string& path, bool flip = false) = 0;
		virtual std::pair<int32_t, int32_t> GetWindowDimensions() = 0;
		virtual std::pair<int32_t, int32_t> GetWindowPosition() = 0;
		virtual void CloseWindow() = 0;

		virtual void SetVSync(bool bEnabled) = 0;
		virtual void SetVisible(bool bVisible) = 0;
		virtual void SetFocus(bool bEnabled) = 0;
		virtual void SetFullscreen(bool bEnabled) = 0;
		virtual void SetResizable(bool bEnabled) = 0;
		virtual void ShowCursor() = 0;
		virtual void HideCursor() = 0;
		virtual void Maximize() = 0;
		virtual void CenterWindow() = 0;
		virtual void SetTitle(const std::string& title) = 0;

		virtual bool IsVisible() = 0;
		virtual bool IsCursorHidden() = 0;
		virtual bool IsMaximized() = 0;
		virtual bool IsFullscreen() = 0;
		virtual bool IsCentered() = 0;
		virtual bool IsFocused() = 0;
		virtual const std::string& GetTitle() = 0;

		virtual uint32_t GetWidth() = 0;
		virtual uint32_t GetHeight() = 0;
		virtual void* GetNativeHandle() = 0;
		virtual void* GetNativeContext() = 0;
		virtual void* GetNativeCursor() = 0;

		virtual Ref<RenderingContext>& GetContext() = 0;
		virtual const Ref<RenderingContext>& GetContext() const = 0;
		virtual const WindowData& GetProperties() const = 0;

		static Window& Get();
		static UniqueRef<Window> Create(const WindowData& properties = WindowData());

	protected:

		static Window* s_WindowInstance;
	};
}

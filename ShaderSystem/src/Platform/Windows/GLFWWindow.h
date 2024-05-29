#pragma once

#include "Window/Window.h"

#include "Interfaces/RenderingContext.h"

struct GLFWwindow;
struct GLFWcursor;

namespace ShaderSystem
{
	class GLFWWindow : public Window
	{
	public:

		GLFWWindow(const WindowData& properties);
		~GLFWWindow();

		virtual void SetEventCallback(const EventCallbackFn& callback) override;
		virtual const EventCallbackFn& GetEventCallback() const override;

		virtual void Update() override;

		virtual void SetWindowIcon(const std::string& path, bool flip = false) override;
		virtual std::pair<int32_t, int32_t> GetWindowDimensions() override;
		virtual std::pair<int32_t, int32_t> GetWindowPosition() override;
		virtual void CloseWindow() override;
		virtual void RecreateContext() override;

		virtual void SetVSync(bool bEnabled) override;
		virtual void SetVisible(bool bVisible) override;
		virtual void SetFocus(bool bEnabled) override;
		virtual void SetFullscreen(bool bEnabled) override;
		virtual void SetResizable(bool bEnabled) override;
		virtual void ShowCursor() override;
		virtual void HideCursor() override;
		virtual void Maximize() override;
		virtual void CenterWindow() override;
		virtual void SetTitle(const std::string& title) override;

		virtual bool IsVisible() override;
		virtual bool IsCursorHidden() override;
		virtual bool IsMaximized() override;
		virtual bool IsFullscreen() override;
		virtual bool IsCentered() override;
		virtual bool IsFocused() override;
		virtual const std::string& GetTitle() override;

		virtual uint32_t GetWidth() override;
		virtual uint32_t GetHeight() override;
		virtual void* GetNativeHandle() override;
		virtual void* GetNativeContext() override;
		virtual void* GetNativeCursor() override;

		virtual float GetTime() const override;

		virtual Ref<RenderingContext>& GetContext() override { return mContext; }
		virtual const Ref<RenderingContext>& GetContext() const override { return mContext; }
		virtual const WindowData& GetProperties() const override { return mWindowData; }

	private:

		void Init();
		void SetCallbacks();

	private:

		struct WNDPlacement
		{
			int32_t WindowWidth = 0, WindowHeight = 0;
			int32_t WindowPosX = 0, WindowPosY = 0;
		};

		WindowData mWindowData;
		WNDPlacement mPlacement;
		Ref<RenderingContext> mContext;

		GLFWwindow* mNativeHandle = nullptr;
		GLFWcursor* mNativeCursor = nullptr;
		bool mCursorLocked = false;
	};
}

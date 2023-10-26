#pragma once

#ifdef SHADER_SYSTEM_PLATFORM_MACOS

#include "Window/Window.h"

namespace ShaderSystem
{
	class MacOSWindow : public Window
	{
	public:

		MacOSWindow(const WindowData &properties = WindowData());
		virtual ~MacOSWindow();

		virtual void SetEventCallback(const EventCallbackFn &callback) override;
		virtual const EventCallbackFn &GetEventCallback() const override { return mWindowData.EventCallback; }

		virtual void Update() override;

		virtual void SetWindowIcon(const std::string &path, bool flip = false) override;
		virtual std::pair<int32_t, int32_t> GetWindowDimensions() override;
		virtual std::pair<int32_t, int32_t> GetWindowPosition() override;
		virtual void CloseWindow() override;

		virtual void SetVSync(bool bEnabled) override;
		virtual void SetVisible(bool bVisible) override;
		virtual void SetFocus(bool bEnabled) override;
		virtual void SetFullscreen(bool bEnabled) override;
		virtual void SetResizable(bool bEnabled) override;
		virtual void ShowCursor() override;
		virtual void HideCursor() override;
		virtual void Maximize() override;
		virtual void CenterWindow() override;
		virtual void SetTitle(const std::string &title) override;

		virtual bool IsVisible() override { return mWindowData.Visible; }
		virtual bool IsCursorHidden() override { return !mWindowData.CursorVisible; }
		virtual bool IsMaximized() override { return mWindowData.Maximized; }
		virtual bool IsFullscreen() override { return mWindowData.Fullscreen; }
		virtual bool IsCentered() override { return mWindowData.Centered; }
		virtual bool IsFocused() override { return mWindowData.Focused; }
		virtual const std::string &GetTitle() override { return mWindowData.Title; }

		virtual uint32_t GetWidth() override { return mWindowData.Width; }
		virtual uint32_t GetHeight() override { return mWindowData.Height; }
		virtual void *GetNativeHandle() override;
		virtual void *GetNativeContext() override;
		virtual void *GetNativeCursor() override;

		virtual Ref<RenderingContext> &GetContext() override { return mContext; }
		virtual const Ref<RenderingContext> &GetContext() const override { return mContext; }
		virtual const WindowData &GetProperties() const override { return mWindowData; }

	private:

		struct WNDPlacement
		{
			int32_t WindowWidth = 0, WindowHeight = 0;
			int32_t WindowPosX = 0, WindowPosY = 0;
		};

		WindowData mWindowData;
		WNDPlacement mPlacement;
		Ref<RenderingContext> mContext;

		bool mCursorLocked = false;
	};
}

#endif // SHADER_SYSTEM_PLATFORM_MACOS

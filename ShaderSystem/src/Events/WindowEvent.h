#pragma once

#include "Event.h"

#include <sstream>

namespace ShaderSystem
{
	class WindowCloseEvent : public Event
	{
	public:

		WindowCloseEvent() {}

		std::string ToString() const override
		{
			return std::string("WindowCloseEvent");
		}

		REGISTER_EVENT_CLASS_TYPE(WindowClose)
		REGISTER_EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowResizeEvent : public Event
	{
	public:

		WindowResizeEvent(uint32_t width, uint32_t height)
			: mWidth(width), mHeight(height) {}

		inline uint32_t GetWidth() const { return mWidth; }
		inline uint32_t GetHeight() const { return mHeight; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << mWidth << ", " << mHeight;
			return ss.str();
		}

		REGISTER_EVENT_CLASS_TYPE(WindowResize)
		REGISTER_EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:

		uint32_t mWidth, mHeight;
	};
}

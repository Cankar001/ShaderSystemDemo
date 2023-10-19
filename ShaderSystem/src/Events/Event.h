#pragma once

#include "Core/Core.h"

#include <string>

namespace ShaderSystem
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoves, FileMenu, FileMenuChanged, FileSystemChanged,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
		ThreadStarting, ThreadEnding
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = SHADER_SYSTEM_BIT(0),
		EventCategoryInput = SHADER_SYSTEM_BIT(1),
		EventCategoryKeyboard = SHADER_SYSTEM_BIT(2),
		EventCategoryMouse = SHADER_SYSTEM_BIT(3),
		EventCategoryMouseButton = SHADER_SYSTEM_BIT(4),
	};

	enum class FileSystemAction
	{
		None = 0,
		Added, Renamed, Modified, Deleted
	};

	template<typename T>
	using EventFn = std::function<bool(T&)>;

#define REGISTER_EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
	virtual EventType GetEventType() const override { return GetStaticType(); }\
	virtual const char *GetName() const override { return #type; }

#define REGISTER_EVENT_CLASS_CATEGORY(type) virtual int GetCategoryFlags() const override { return type; }

	class Event
	{
	public:

		bool mHandled = false; /**< Indicates whether the Event has been handled or not. */

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;

		virtual std::string ToString() const
		{
			return GetName();
		}

#pragma warning(push)
#pragma warning(disable: 26812)
		bool IsInCategory(EventCategory c)
		{
			return GetCategoryFlags() & c;
		}
#pragma warning(pop)

		friend class EventDispatcher;
	};

	class EventDispatcher
	{
	public:

		EventDispatcher(Event& e) : mEvent(e) {}

		template<typename EventType, typename FunctionType>
		bool Dispatch(const FunctionType& func)
		{
			if (mEvent.GetEventType() == EventType::GetStaticType())
			{
				mEvent.mHandled = func(static_cast<EventType&>(mEvent));
				return true;
			}

			return false;
		}

	private:

		Event& mEvent;
	};

	inline std::ostream& operator<<(std::ostream& stream, const Event& e)
	{
		return stream << e.ToString();
	}
}

#pragma once

#include <memory>
#include <functional>

#define SHADER_SYSTEM_BIT(X)	 (1 << X)
#define SHADER_SYSTEM_BIND_EVENT_FN(fn) std::bind(&fn, std::placeholders::_1)
#define SHADER_SYSTEM_BIND_CLASS_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace ShaderSystem
{
	using Byte = unsigned char;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> MakeRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using UniqueRef = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr UniqueRef<T> MakeUnique(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using WeakRef = std::weak_ptr<T>;

	template<typename T>
	constexpr WeakRef<T> MakeWeak(const Ref<T> &ref)
	{
		return std::weak_ptr<T>(ref);
	}
}

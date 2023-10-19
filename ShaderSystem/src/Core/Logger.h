#pragma once

#include <memory>

#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

namespace ShaderSystem
{
	class Logger
	{
	public:

		static void Init();
		static void Shutdown();

		static std::shared_ptr<spdlog::logger>& GetLogger();
	};

	template<typename T>
	T& operator<<(T& os, const glm::vec2& vec)
	{
		return os << "(" << vec.x << ", " << vec.y << ")";
	}

	template<typename T>
	T& operator<<(T& os, const glm::vec3& vec)
	{
		return os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	}

	template<typename T>
	T& operator<<(T& os, const glm::vec4& vec)
	{
		return os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
	}
}

#define SHADER_SYSTEM_ERROR(...)   ::ShaderSystem::Logger::GetLogger()->error(__VA_ARGS__)
#define SHADER_SYSTEM_WARN(...)    ::ShaderSystem::Logger::GetLogger()->warn(__VA_ARGS__)
#define SHADER_SYSTEM_INFO(...)    ::ShaderSystem::Logger::GetLogger()->info(__VA_ARGS__)
#define SHADER_SYSTEM_TRACE(...)   ::ShaderSystem::Logger::GetLogger()->trace(__VA_ARGS__)
#define SHADER_SYSTEM_FATAL(...)   ::ShaderSystem::Logger::GetLogger()->critical(__VA_ARGS__)

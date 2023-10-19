#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace ShaderSystem
{
	static std::shared_ptr<spdlog::logger> s_Logger;

	void Logger::Init()
	{
		bool shouldLogToFile = true;

		std::vector<spdlog::sink_ptr> appSinks =
		{
			std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
		};

		appSinks[0]->set_pattern("%^[%T] %n: %v%$");

		if (shouldLogToFile)
		{
			appSinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/app.log", true));
			appSinks[appSinks.size() - 1]->set_pattern("%^[%T] %n: %v%$");
		}

		s_Logger = std::make_shared<spdlog::logger>("ShaderSystem", appSinks.begin(), appSinks.end());
		s_Logger->set_level(spdlog::level::trace);
	}

	void Logger::Shutdown()
	{
		s_Logger.reset();

		spdlog::drop_all();
	}

	std::shared_ptr<spdlog::logger>& Logger::GetLogger()
	{
		return s_Logger;
	}
}

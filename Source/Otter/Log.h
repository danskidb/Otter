#pragma once

#include <memory>
#include "Core.h"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#pragma warning(pop) 

namespace Otter {

	class Log
	{
	public:
		static void Init();

		static void CoreAssert(bool condition, std::string errorMsg);

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core log macros
#define OT_CORE_TRACE(...)    ::Otter::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define OT_CORE_INFO(...)     ::Otter::Log::GetCoreLogger()->info(__VA_ARGS__)
#define OT_CORE_WARN(...)     ::Otter::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define OT_CORE_ERROR(...)    ::Otter::Log::GetCoreLogger()->error(__VA_ARGS__)
#define OT_CORE_FATAL(...)    ::Otter::Log::GetCoreLogger()->fatal(__VA_ARGS__)
#define OT_ASSERT(condition, errorMsg) ::Otter::Log::CoreAssert(condition, errorMsg)

// Client log macros
#define OT_TRACE(...)	      ::Otter::Log::GetClientLogger()->trace(__VA_ARGS__)
#define OT_INFO(...)	      ::Otter::Log::GetClientLogger()->info(__VA_ARGS__)
#define OT_WARN(...)	      ::Otter::Log::GetClientLogger()->warn(__VA_ARGS__)
#define OT_ERROR(...)	      ::Otter::Log::GetClientLogger()->error(__VA_ARGS__)
#define OT_FATAL(...)	      ::Otter::Log::GetClientLogger()->fatal(__VA_ARGS__)

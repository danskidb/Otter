#pragma once

#include <memory>

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#pragma warning(pop) 
#include <string>

namespace Otter {

	class Log
	{
	public:
		static void Init();
		static void CoreAssert(bool condition, std::string errorMsg);
		static std::string GetFileName(const char filePath[]);

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core log macros
#define OT_CORE_TRACE(...)    ::Otter::Log::GetCoreLogger()->trace("[{}] {}", __FUNCTION__, __VA_ARGS__)
#define OT_CORE_INFO(...)     ::Otter::Log::GetCoreLogger()->info("[{}] {}", __FUNCTION__, __VA_ARGS__)
#define OT_CORE_WARN(...)     ::Otter::Log::GetCoreLogger()->warn("[{}] {}", __FUNCTION__, __VA_ARGS__)
#define OT_CORE_ERROR(...)    ::Otter::Log::GetCoreLogger()->error("[{}] {}", __FUNCTION__, __VA_ARGS__)
#define OT_CORE_FATAL(...)    ::Otter::Log::GetCoreLogger()->fatal("[{}] {}", __FUNCTION__, __VA_ARGS__)
#define OT_ASSERT(condition, errorMsg) ::Otter::Log::CoreAssert(condition, errorMsg)

// Client log macros
#define OT_TRACE(...)	      ::Otter::Log::GetClientLogger()->trace("[{}] {}", __FUNCTION__, __VA_ARGS__)
#define OT_INFO(...)	      ::Otter::Log::GetClientLogger()->info("[{}] {}", __FUNCTION__, __VA_ARGS__)
#define OT_WARN(...)	      ::Otter::Log::GetClientLogger()->warn("[{}] {}", __FUNCTION__, __VA_ARGS__)
#define OT_ERROR(...)	      ::Otter::Log::GetClientLogger()->error("[{}] {}", __FUNCTION__, __VA_ARGS__)
#define OT_FATAL(...)	      ::Otter::Log::GetClientLogger()->fatal("[{}] {}", __FUNCTION__, __VA_ARGS__)

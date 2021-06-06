#pragma once
#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Enum.h"
#include "Singleton.h"

//#if defined(LOGGER_RELEASE_ASSERT) || defined(_DEBUG) || defined(DEBUG)
//#define NGN_ASSERT(exp) if (exp) ngenius::Logger::Log<std::string>(LogLevel::ERROR_LOG, #exp);
//#else
//#define NGN_ASSERT(exp) exp;
//#endif

namespace ngenius
{
	class Logger : public Singleton<Logger>
	{
	public:
		~Logger() override = default;
		Logger(const Logger& other) = delete;
		Logger(Logger&& other) = delete;
		Logger& operator=(const Logger& other) = delete;
		Logger& operator=(Logger&& other) = delete;

		template<typename MESSAGE_TYPE>
		static void Log(LogLevel level, const MESSAGE_TYPE& message);

	private:
		friend class Singleton<Logger>;
		explicit Logger() = default;
	};
	
	template<typename MESSAGE_TYPE>
	void Logger::Log(LogLevel level, const MESSAGE_TYPE& message)
	{
		HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		std::string strHeader{};
		
		switch (level)
		{
		case LogLevel::INFO_LOG:
			strHeader = "[INFO] >>> ";
			SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			break;
		case LogLevel::WARNING_LOG:
			strHeader = "[WARNING] >>> ";
			SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN);
			break;
		case LogLevel::ERROR_LOG:
			strHeader = "[ERROR] >>> ";
			SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
			break;
		}
		std::cout << strHeader << message << std::endl;

		SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

		if (level == LogLevel::ERROR_LOG)
			__debugbreak();
	}
}

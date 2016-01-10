#pragma once

#include <ctime>
#include <iostream>
#include <sstream>

void log_message(const std::string& log_level, const std::string& message) {
	time_t current_time = time(0);

#if defined(__GNUG__)
	struct tm* local_time = localtime(&current_time);
#elif defined(_MSC_VER)
	struct tm local_time;
	localtime_s(&local_time, &current_time);
#endif
	
	std::ostringstream stream;
	stream << "[";
#if defined(__GNUG__)
	stream << (local_time->tm_hour) << ":"
		<< (local_time->tm_min) << ":"
		<< (local_time->tm_sec);
#elif defined(_MSC_VER)
	stream << (local_time.tm_hour) << ":"
		<< (local_time.tm_min) << ":"
		<< (local_time.tm_sec);
#endif
	stream << "] " << log_level << ": " << message << std::endl;
	if (log_level == "INFO" || log_level == "DEBUG") {
		std::cout << stream.str();
	}
	else {
		std::cerr << stream.str();
	}
}

#define LOG_ERROR(message) (log_message("ERROR", message))
#define LOG_INFO(message) (log_message("INFO", message))
#ifdef NDEBUG
	#define LOG_DEBUG(message)
#else
	#define LOG_DEBUG(message) (log_message("DEBUG", message))
#endif

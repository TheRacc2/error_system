#pragma once
#include <iostream>
#include <windows.h>
#include <Psapi.h>
#pragma region FMT
#pragma warning(push, 0)    // Push warning settings, and set them to 0.

#define FMT_HEADER_ONLY
#include "fmt/format.h"

#pragma warning(pop) // Pop warning settings.
#pragma endregion

constexpr const char* name = "Error Handler";

// Author: https://github.com/TheRacc2
// Repository: https://github.com/TheRacc2/error_handler/
// 
// Libraries:
// FMT at https://fmt.dev
// 
// Other Help:
// https://github.com/dgoguerra/, for the bytes_to_size function (heavliy modified from the "humanSize" function found in https://gist.github.com/dgoguerra/7194777),
// https://codereview.stackexchange.com/users/129343/g-sliepen/ & https://codereview.stackexchange.com/users/24084/uli, for the feedback.
// 
// Report issues here: https://github.com/TheRacc2/error_handler/issues

namespace helpers {
	constexpr int unit_size = 1000;

	// Convert bytes into readable sizes, E.G: 1000B -> 1ki, 1000000B -> 1Mi
	//
	// call as such:
	// const std::string sign = bytes_to_size(1024);
	std::string bytes_to_size(int bytes) {
		const char* units[] = { "Bytes", "ki", "Mi", "Gi", "Ti", "Pi" };
		const int size = sizeof(units) / sizeof(const char*);

		int i = 0;
		int outside_bytes = bytes;

		if (bytes > unit_size)
			for (; (bytes / unit_size) > 0 && i < size; i++, bytes /= unit_size)
				outside_bytes = bytes / unit_size;

		return fmt::format("{}{}", outside_bytes, units[i]);
	}
}

#ifndef CATCH
// Use this in a "try, catch" situation, the send to developers function doesn't need to be provided.
// 
// call as such:
// try {
//     your_function_or_code();
// } CATCH(send_log_to_developer(ex))
#define CATCH(send_to_developers_function) \
catch (const std::exception& ex) { \
	if (MessageBoxA(nullptr, fmt::format("{}\n\nWould you like to report this to the developers? Should your report be seen, this crash may be prevented.", ex.what()).c_str(), name, MB_YESNO) == IDYES) { \
		send_to_developers_function; \
		MessageBoxA(nullptr, "Thank you for reporting this!", name, MB_OK); \
	} \
}
#endif

#ifndef THROW
// Call this when something needs to go wrong, formatted like "fmt::format". The second argument doesn't need to be provided.
// The first argument must be provided, it is the error type.
// 
// call as such:
// THROW(std::runtime_exception, "You didn't use this right! You used this like: {}", "throw(std::exception)")
// 
// output:
// "You didnt use this right! You used this like: throw(std::exception)"
#define THROW(type, text, ...) \
error_handler::throw_ex<type>(text, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__);
#endif

namespace error_handler {
	// You shouldn't manually call this function, as you have to provide the file, function and line.
	// Use the "THROW" macro instead.
	template <typename Type, typename... Args>
	void throw_ex(const char* text, const char* file, const char* function, const int& line, Args... args) {
		PROCESS_MEMORY_COUNTERS_EX pmc;
		K32GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc));

		const std::string json = // use {{ or }} to escape the brace.
			fmt::format(
				"{{\n"
				"    File: \"{}\",\n"
				"    Function: \"{}\",\n"
				"    Line: {},\n"
				"    Memory: {}\n"
				"}}",
				file, function, line, helpers::bytes_to_size(pmc.PrivateUsage)
			);
		
		throw Type(
			fmt::format(
				"{}\n\n{}",
				fmt::format(text, args...), json
			)
		);
	}
}
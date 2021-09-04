#include "core.hpp"
#include <Psapi.h>
#include <Shlwapi.h>

// Author: https://github.com/TheRacc2
// Repository: https://github.com/TheRacc2/error_handler/
// 
// Libraries:
// FMT @ https://fmt.dev
// 
// Other Help:
// **
// 
// Report issues here: https://github.com/TheRacc2/error_handler/issues

#define FMT_HEADER_ONLY // We only need 3 files, use the smaller FMT version.
#include "fmt/format.h"

#pragma comment(lib, "Shlwapi.lib") // Fix for "unresolved external symbol __impl_StrFormatByteSizeA@12", keep this lib commented.

namespace error_handler {
	std::string bytes_to_readable_size(const unsigned long& bytes) {
		char buffer[10]; // max size is "1234 bytes", or 10 characters
		StrFormatByteSizeA(bytes, buffer, sizeof buffer); // Found in the deepest darkest depths of MSDN, https://docs.microsoft.com/en-us/windows/win32/api/shlwapi/nf-shlwapi-strformatbytesizea

		return buffer; // std::string automatically constructs when (compatible) improper type is returned, no need to call constructor.
	}

	std::string get_error_text(const char* text, const char* file, const char* function, const int& line) {
		PROCESS_MEMORY_COUNTERS_EX pmc; // https://docs.microsoft.com/en-us/windows/win32/api/psapi/ns-psapi-process_memory_counters_ex
		K32GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof pmc); // Use C++ style casting.

		return fmt::format( // Use {{ to escape, same with }}.
			"{}\n\n"
			"{{\n"
			"    File: {},\n"
			"    Function: {},\n"
			"    Line: {},\n"
			"    Memory: {}\n"
			"}}",
			text,
			file,
			function,
			line,
			bytes_to_readable_size(pmc.PrivateUsage) // Virtal Memory used by process. Memory usage that shows in task manager, essentially. 
		);
	}
}
#pragma once
#ifndef ERROR_SYSTEM_HEADER
#define ERROR_SYSTEM_HEADER

#include <Windows.h>
#include <iostream>
#include <format>
#include <Psapi.h>
#include <Shlwapi.h>

#pragma comment(lib, "Shlwapi.lib") // StrToBytesizeA

// Author: https://github.com/TheRacc2
// Repository: https://github.com/TheRacc2/error_system/
// 
// Libraries:
// **
// 
// Other Help:
// **
// 
// Report issues here: https://github.com/TheRacc2/error_system/issues

#pragma region macros

#define THROW(type, x, ...) \
throw type(errsys::get_error_text(__FILE__, __FUNCTION__, __LINE__, x, __VA_ARGS__).c_str());

#define ASSERT(x) \
if (!(x)) { THROW(errsys::failed_assert, "statement: '{}', failed to assert!", #x) }

#define SHOW(x, mb_title, mb_flags) \
MessageBoxA(nullptr, x.what(), mb_title, mb_flags);

#pragma endregion

namespace errsys {
    #pragma region customerrorclases

    // new exception type, extend on any classes this library can throw. from file "stdexcept" line 89.
    // CLASS custom_error
    class failed_assert : public std::logic_error { // base of all exceptions
        public:
            using _Mybase = logic_error;

            explicit failed_assert(const std::string& _Message) : _Mybase(_Message.c_str()) {}

            explicit failed_assert(const char* _Message) : _Mybase(_Message) {}

        #if !_HAS_EXCEPTIONS
        protected:
            virtual void _Doraise() const override { // perform class-specific exception handling
                _RAISE(*this);
            }
        #endif // !_HAS_EXCEPTIONS
    };

    #pragma endregion
	
    inline std::string format_bytesize(const unsigned long& bytes) {
        char buffer[10]; // max size is "1234 bytes", or 10 characters
        StrFormatByteSizeA(bytes, buffer, sizeof buffer); // Found in the deepest darkest depths of MSDN, https://docs.microsoft.com/en-us/windows/win32/api/shlwapi/nf-shlwapi-strformatbytesizea

        return buffer; // std::string automatically constructs when improper type is returned, no need to call constructor.
    }

    inline PROCESS_MEMORY_COUNTERS_EX get_pmc_instance() {
        PROCESS_MEMORY_COUNTERS_EX pmc;
        K32GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc));

        return pmc;
    }

    // Because we're using std::format, your message will be formatted using "{}" as the wildcard(%) character.
    template<typename ...Args>
    inline std::string get_error_text(const std::string& file, const std::string& function, const int& line, const std::string& message, const Args... arguments) {
        const std::string message_formatted = std::format(message, arguments...);
        const std::string json = std::format(
            "{}\n"
            "    file: \"{}\",\n"
            "    function: \"{}\",\n"
            "    line: {},\n"
            "    memory_usage: \"{}\",\n"
            "{}",
            "{", file, function, line, format_bytesize(get_pmc_instance().WorkingSetSize), "}"
        );

        return std::format("{}\n\n{}", message_formatted, json);
    }
}

#endif
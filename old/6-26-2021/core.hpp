#pragma once
#include <Windows.h>
#include <iostream>

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

#define THROW(type, text) \
throw type(error_handler::get_error_text(text, __FILE__, __FUNCTION__, __LINE__).c_str());

#define CATCH(mb_title, mb_flags) \
catch (const std::exception& ex) { \
    MessageBoxA(nullptr, ex.what(), mb_title, mb_flags); \
}

#define ASSERT(value) \
if (!(value)) \
    THROW(error_handler::failed_assert, "(" #value ") was false when asserted")

namespace error_handler {
	const std::string bytes_to_readable_size(const unsigned long& bytes);

	const std::string get_error_text(const char* text, const char* file, const char* function, const int& line);

    // new exception type, for failed asserts. from file "stdexcept" line 89.
    // CLASS failed_assert
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
}

#include "current/errsys.hpp"

// ---- CREDITS ----
// Author: https://github.com/TheRacc2
// Repository: https://github.com/TheRacc2/error_system/
// 
// Report issues here: https://github.com/TheRacc2/error_system/issues
// ---- END CREDITS ----

int main() {
	printf("Continue to see a custom exception.\n");
	system("pause");

	try {
		THROW(std::exception, "This is a custom handled exception, thrown with the type {}", "std::exception")
	}
	catch (const std::exception& ex) {
		SHOW(ex, "example exception", MB_ICONERROR | MB_OK)
	}

	printf("\nContinue to see a custom assert exception.\n");
	system("pause");

	try {
		ASSERT(false)
	}
	catch (const std::exception& ex) {
		SHOW(ex, "example assert", MB_ICONERROR | MB_OK)
	}
}
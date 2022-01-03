#include "current/errsys.hpp"

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
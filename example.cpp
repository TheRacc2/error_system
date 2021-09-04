#include "current/core.hpp"

int main() {
	printf("Enter 0 for a custom assert error, enter 1 for a custom handled exception.\n> ");

	char i;
	std::cin >> i;

	try {
		if (i)
			THROW(std::exception, "This is a custom handled exception, thrown with the type \'std::exception\'")
		else
			ASSERT(false)
	} CATCH("Exception Tests", MB_ICONERROR | MB_OK);
}
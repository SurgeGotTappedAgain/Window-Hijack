#include "includes.hpp"

int main()
{
	Overlay overlay;

	if (!overlay.MessageLoop())
		printf("[!] error occurred..\n");

	return 0;
}
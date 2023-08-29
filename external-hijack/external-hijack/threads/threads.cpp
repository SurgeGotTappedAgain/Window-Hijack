#include "includes.hpp"

/* install overwolf here https://download.overwolf.com/install/Download?Channel=web_dl_btn&utm_content=web_app_store&guid=03f373c7-2ae2-4af3-9491-14865eed9189 */
/* navigate to settings and enable overlay for desired game */

/* todo list:
- resize buffer (scale menu based on overlay size)
- fix menu flicker when alt tabbed 
*/

int main()
{
	try
	{
		HWND target_window = FindWindowA("OOPO_WINDOWS_CLASS", "ow overlay");
		if (!target_window) {
			ExceptionManger::Error("Failed To Find Target Window");
		}

		Overlay overlay;

		if (!overlay.MessageLoop()) {
			ExceptionManger::Error("Failed To Init Render");
		}
	}
	catch (const ExceptionManger& exception)
	{
		printf("[!] %s\n", exception.what());
		std::cin.get();
		return 0;
	}
}
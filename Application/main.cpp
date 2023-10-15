#include <exception>

#include <Windows.h>

#include "Window.hpp"

#define ErrorBox(msg) MessageBoxA(NULL, msg, "Error", MB_ICONERROR | MB_OK)


int CALLBACK
WinMain(HINSTANCE hinstance, HINSTANCE prev_hinstance, LPSTR cmdline, int cmdshow)
{
#ifdef NDEBUG
	try
	{
		Window window;
		window.Run();
	}
	catch (const std::exception& e)
	{
		ErrorBox(e.what());
	}
	catch (...)
	{
		ErrorBox("An unknown error has occurred.");
	}
#else
	Window window;
	window.Run();
#endif
}

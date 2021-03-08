#include "Engine.h"
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"DirectXTK.lib")

/* Entry point to the Windows application. Alias for WINAPI */
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, // handle to an instance of the app
	_In_opt_ HINSTANCE hPrevInstance, // handle to the previous instance
	_In_ LPWSTR lpCmdLine, // pointer to command line
	_In_ int nCmdShow) // command show arguement value
{
	Engine engine;
	engine.Initialize(hInstance, "Title", "MyWindowClass", 800, 600);
	while (engine.ProcessMessages() == true)
	{
		engine.Update();
	}

	return 0;
}
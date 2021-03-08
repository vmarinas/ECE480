#include "WindowContainer.h"

bool RenderWindow::Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, std::string window_title, std::string window_class,
	int width, int height)
{
	this->hInstance = hInstance;
	this->width = width;
	this->height = height;
	this->window_title = window_title;
	this->window_title_wide = StringConverter::StringToWide(this->window_title);
	this->window_class = window_class;
	this->window_class_wide = StringConverter::StringToWide(this->window_class);

	this->RegisterWindowClass();

	this->handle = CreateWindowEx(0, // Extended Windows style
		this->window_class_wide.c_str(), // window class name
		this->window_title_wide.c_str(), // window title
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, // windows style
		0, // window X-positon
		0, // window Y-position
		this->width, // window width
		this->height, // window height
		NULL, // handle to parent of this window
		NULL, // handle to menu or child window identifier
		this->hInstance, // handle to the instance of module to be used with this window
		pWindowContainer); // param to create window

	if (this->handle == NULL)
	{
		ErrorLogger::Log(GetLastError(), "Create WindowEX Failed for window: " + this->window_title);
		return false;
	}

	// bring the window up on the screen and set it as main focus
	ShowWindow(this->handle, SW_SHOW);
	SetForegroundWindow(this->handle);
	SetFocus(this->handle);

	return true;
}

bool RenderWindow::ProcessMessages()
{
	// handle the window messages
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG)); // initialize the message structure

	if (PeekMessage(&msg, // where to store message(if one exists)
		this->handle, // handle to window we are checking messages for
		0, // minimum filter msg value
		0, // maximum filter msg value
		PM_REMOVE)) // remove message after capturing it via PeekMessage
	{
		TranslateMessage(&msg); // translate message from virtual key messages into character messages
		DispatchMessage(&msg); // dispatch message to our Window Proc for this window
	}

	// check if the window was closed
	if (msg.message == WM_NULL)
	{
		if (!IsWindow(this->handle))
		{
			this->handle = NULL; // message processing loop takes care of destroying this window
			UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
			return false;
		}
	}

	return true;
}

RenderWindow::~RenderWindow()
{
	if (this->handle != NULL)
	{
		UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
		DestroyWindow(handle);
	}
}

LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	// all other messages
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;

	default:
	{
		// retrieve ptr to window class
		WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		// forward message to window class handler
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
}

LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_NCCREATE:
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		WindowContainer* pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);
		if (pWindow == nullptr) // sanity check
		{
			ErrorLogger::Log("Critical Error: Pointer to window container is null duuring WM_CREATE.");
			exit(-1);
		}
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
		
	}
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

void RenderWindow::RegisterWindowClass()
{
	WNDCLASSEX wc; // our Window Class
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // flags [Redraw of width/height change from resize/movement]
	wc.lpfnWndProc = HandleMessageSetup; // pointer to Window Proc function for handling messages from this window
	wc.cbClsExtra = 0; // number of extra bytes to allocate following the window-class structure
	wc.cbWndExtra = 0; // number of extra bytes to allocate following the window instance
	wc.hInstance = this->hInstance; // handle to the instance that contains the Window Procedure
	wc.hIcon = NULL; // handle to the class icon. Must be a handle to an icon resource
	wc.hIconSm = NULL; // handle to small icon for this class
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // default cursor
	wc.hbrBackground = NULL; // handle to the class background brush for the window's background color
	wc.lpszMenuName = NULL; // pointer to a null terminated character string for the menu
	wc.lpszClassName = this->window_class_wide.c_str(); // pointer to null terminated string of our class name
	wc.cbSize = sizeof(WNDCLASSEX); // need to fill in the size of our struct for cbSize
	RegisterClassEx(&wc); // register the class so that it is usable
}
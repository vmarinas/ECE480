#include "ErrorLogger.h"
#include <comdef.h>

void ErrorLogger::Log(std::string message)
{
	std::string error_message = "Error: " + message;
	// Message Box with ASCII (short string)
	MessageBoxA(NULL, // handle to parent window 
		error_message.c_str(), // text inside the message box
		"Error", // message box title
		MB_ICONERROR); // message box type
}

void ErrorLogger::Log(HRESULT hr, std::string message)
{
	_com_error error(hr); // returns a wide string
	std::wstring error_message = L"Error: " + StringConverter::StringToWide(message) + L"\n" + error.ErrorMessage();
	// Message Box with wide strings
	MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}
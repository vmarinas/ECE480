#pragma once
#include "StringConverter.h"
#include <Windows.h>
class ErrorLogger
{
public:
	/*Logs error messages, param: string message*/
	static void Log(std::string message);
	/*Logs error messages, param: HRESULT hr, string message.
	HRESULT is a data type common to Windows and DirectX API's 
	that is used for error conditions.*/
	static void Log(HRESULT hr, std::string message);
};
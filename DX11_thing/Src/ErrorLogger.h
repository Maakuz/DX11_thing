#pragma once
#include "StringConverter.h"
#include <Windows.h>
#include <comdef.h>

class ErrorLogger
{
public: 
	static void log(std::string message) 
	{
		std::string l_message = "Error: " + message;
		MessageBoxA(NULL, l_message.c_str(), "Error", MB_ICONERROR);
	}

	static void log(HRESULT hr, std::string message)
	{
		_com_error l_error(hr);
		std::wstring l_message = L"Error: " + StringConverter::stringToWide(message) + L"\n" + l_error.ErrorMessage();
		MessageBoxW(NULL, l_message.c_str(), L"Error", MB_ICONERROR);
	}

	static void log(HRESULT hr, std::wstring message)
	{
		_com_error l_error(hr);
		std::wstring l_message = L"Error: " + message + L"\n" + l_error.ErrorMessage();
		MessageBoxW(NULL, l_message.c_str(), L"Error", MB_ICONERROR);
	}
};


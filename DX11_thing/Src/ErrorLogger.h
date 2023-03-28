#pragma once
#include "COMException.h"
#include <Windows.h>

#define LOG_ERROR(msg) ErrorLogger::log(msg, __FILE__, __FUNCTION__, __LINE__)

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

	static void log(COMException& exception)
	{
		std::wstring message = exception.what();
		MessageBoxW(NULL, message.c_str(), L"Error", MB_ICONERROR);
	}

	static void log(const std::string& msg, const std::string& file, const std::string& function, int line)
	{
		std:: wstring l_msg = L"Msg: " + StringConverter::stringToWide(msg) + L"\n";
		l_msg += L"File: " + StringConverter::stringToWide(file) + L"\n";
		l_msg += L"Function: " + StringConverter::stringToWide(function) + L"\n";
		l_msg += L"Line: " + StringConverter::stringToWide(std::to_string(line)) + L"\n";
		MessageBoxW(NULL, l_msg.c_str(), L"Error", MB_ICONERROR);
	}
};


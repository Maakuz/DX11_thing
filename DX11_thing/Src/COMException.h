#pragma once
#include <comdef.h>
#include "StringConverter.h"

#define COM_ERROR_IF_FAILED(hr, msg) if(FAILED(hr)) throw COMException(hr, msg, __FILE__, __FUNCTION__, __LINE__)

class COMException 
{
public:
    COMException(HRESULT hr, const std::string& msg, const std::string& file, const std::string& function, int line)
    {
        _com_error error(hr);

        m_msg = L"Msg: " + StringConverter::stringToWide(msg) + L"\n";
        m_msg += error.ErrorMessage();
        m_msg += L"File: " + StringConverter::stringToWide(file) + L"\n";
        m_msg += L"Function: " + StringConverter::stringToWide(function) + L"\n";
        m_msg += L"Line: " + StringConverter::stringToWide(std::to_string(line)) + L"\n";
    }

    const wchar_t* what() const { return m_msg.c_str(); }

private:
    std::wstring m_msg;
};
#pragma once
#include "ErrorLogger.h"

class WindowContainer;

class RenderWindow
{
public:
	bool initialize(WindowContainer* pWinContainer, HINSTANCE hInstance, std::string title, std::string windowClass, int width, int height);
	bool processMessage();
	~RenderWindow();
private:
	void registerWindowClass();

	HWND m_handle = NULL;
	HINSTANCE m_hInstance = NULL;
	std::string m_winTitle = "";
	std::wstring m_winTitleW = L"";
	std::string m_windowClass = "";
	std::wstring m_windowClassW = L"";

	int m_width = 0;
	int m_height = 0;

};
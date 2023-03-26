#pragma once
#include "WindowContainer.h"
#include "Timer.h"
class Engine : WindowContainer
{
public: 
	bool initialize(HINSTANCE hInstance, std::string title, std::string windowClass, int width, int height);
	bool processMessage();

	bool update();
	void render();

private:
	Timer m_timer;
};
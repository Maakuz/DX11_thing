#include "Engine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow) 
{
	/*HRESULT hr = E_INVALIDARG;

	if (FAILED(hr))
		ErrorLogger::log(hr, "test");*/

	HRESULT l_hr = CoInitialize(NULL);
	if (FAILED(l_hr))
	{
		ErrorLogger::log(l_hr, "Error coinitializing");
		return -1;
	}

	Engine l_engine;
	if (l_engine.initialize(hInstance, "Best engie", "WindowClass", 1920, 1080))
	{
		while (l_engine.processMessage() == true)
		{
			l_engine.update();
			l_engine.render();
		}
	}
	return 0;
}
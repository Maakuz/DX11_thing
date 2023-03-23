#include "WindowContainer.h"
#include <memory>

WindowContainer::WindowContainer()
{
	static bool s_rawInputInitialized = false;
	if (s_rawInputInitialized == false)
	{
		RAWINPUTDEVICE l_rid;
		l_rid.usUsagePage = 0x01;
		l_rid.usUsage = 0x02;
		l_rid.dwFlags = 0;
		l_rid.hwndTarget = NULL;

		if (RegisterRawInputDevices(&l_rid, 1, sizeof(l_rid)) == false)
		{
			ErrorLogger::log(GetLastError(), "Failed to register RAW input devices");
			exit(-1);
		}

		s_rawInputInitialized = true;
	}
}

LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	//Keyboars Messages
	case WM_KEYDOWN:
	{
		unsigned char l_keycode = static_cast<unsigned char>(wParam);
		if (m_keyboard.isKeysAutoRepeat())
		{
			m_keyboard.onKeyPressed(l_keycode);
		}

		else
		{
			const bool l_wasPressed = lParam & 0x40000000;
			if (!l_wasPressed)
				m_keyboard.onKeyPressed(l_keycode);
		}
		return 0;
	}

	case WM_KEYUP:
	{
		unsigned char l_keycode = static_cast<unsigned char>(wParam);
		m_keyboard.onKeyReleased(l_keycode);
		return 0;
	}

	case WM_CHAR:
	{
		unsigned char l_c = static_cast<unsigned char>(wParam);
		if (m_keyboard.isCharsAutoRepeat())
		{
			m_keyboard.onChar(l_c);
		}

		else
		{
			const bool l_wasPressed = lParam & 0x40000000;
			if (!l_wasPressed)
				m_keyboard.onChar(l_c);
		}

		return 0;
	}

	//Mouse Messages
	case WM_INPUT:
	{
		UINT l_dataSize = 0;

		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &l_dataSize, sizeof(RAWINPUTHEADER));

		if (l_dataSize > 0)
		{
			std::unique_ptr<BYTE[]> l_rawData = std::make_unique<BYTE[]>(l_dataSize);
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, l_rawData.get(), &l_dataSize, sizeof(RAWINPUTHEADER)) == l_dataSize)
			{
				RAWINPUT* l_raw = reinterpret_cast<RAWINPUT*>(l_rawData.get());
				if (l_raw->header.dwType == RIM_TYPEMOUSE)
					m_mouse.onMouseMoveRaw(l_raw->data.mouse.lLastX, l_raw->data.mouse.lLastY);
			}
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	case WM_MOUSEMOVE:
	{
		int l_x = LOWORD(lParam);
		int l_y = HIWORD(lParam);
		m_mouse.onMouseMove(l_x, l_y);
		return 0;
	}

	case WM_LBUTTONDOWN:
	{
		int l_x = LOWORD(lParam);
		int l_y = HIWORD(lParam);
		m_mouse.onLeftPressed(l_x, l_y);
		return 0;
	}

	case WM_RBUTTONDOWN:
	{
		int l_x = LOWORD(lParam);
		int l_y = HIWORD(lParam);
		m_mouse.onRightPressed(l_x, l_y);
		return 0;
	}

	case WM_MBUTTONDOWN:
	{
		int l_x = LOWORD(lParam);
		int l_y = HIWORD(lParam);
		m_mouse.onMiddlePressed(l_x, l_y);
		return 0;
	}

	case WM_LBUTTONUP:
	{
		int l_x = LOWORD(lParam);
		int l_y = HIWORD(lParam);
		m_mouse.onLeftReleased(l_x, l_y);
		return 0;
	}

	case WM_RBUTTONUP:
	{
		int l_x = LOWORD(lParam);
		int l_y = HIWORD(lParam);
		m_mouse.onRightReleased(l_x, l_y);
		return 0;
	}

	case WM_MBUTTONUP:
	{
		int l_x = LOWORD(lParam);
		int l_y = HIWORD(lParam);
		m_mouse.onMiddleReleased(l_x, l_y);
		return 0;
	}

	case WM_MOUSEWHEEL:
	{
		int l_x = LOWORD(lParam);
		int l_y = HIWORD(lParam);

		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			m_mouse.onWheelUp(l_x, l_y);

		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
			m_mouse.onWheelDown(l_x, l_y);

		return 0;
	}

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

#include "WindowContainer.h"
#include <memory>

WindowContainer::WindowContainer()
{
	static bool raw_input_initialized = false;
	if (raw_input_initialized == false)
	{
		RAWINPUTDEVICE rid;

		rid.usUsagePage = 0x01; //Mouse
		rid.usUsage = 0x02;
		rid.dwFlags = 0;
		rid.hwndTarget = NULL;

		if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
		{
			ErrorLogger::Log(GetLastError(), "Failed to register raw input devices.");
			exit(-1);
		}

		raw_input_initialized = true;
	}
}

LRESULT WindowContainer::WindowProc(const HWND hwnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam)
{
	switch (uMsg)
	{

		// Keyboard Messages
		case WM_KEYDOWN:
		{
			const auto key_code = static_cast<unsigned char>(wParam);

			if (keyboard_.IsKeyAutoRepeat())
			{
				keyboard_.OnKeyPressed(key_code);
			}
			else
			{
				const bool was_pressed = lParam & 0x40000000;
				if (!was_pressed)
				{
					keyboard_.OnKeyPressed(key_code);
				}
			}
			return 0;
		}
		case WM_KEYUP:
		{
			const auto key_code = static_cast<unsigned char>(wParam);
			keyboard_.OnKeyReleased(key_code);
			return 0;
		}
		case WM_CHAR:
		{
			const auto ch = static_cast<unsigned char>(wParam);
			if (keyboard_.IsCharsAutoRepeat())
			{
				keyboard_.OnChar(ch);
			}
			else
			{
				const bool was_pressed = lParam & 0x40000000;
				if(!was_pressed)
				{
					keyboard_.OnChar(ch);
				}
			}
			return 0;
		}
		// Mouse Messages
		case WM_MOUSEMOVE:
		{
			const int x = LOWORD(lParam);
			const int y = HIWORD(lParam);
			mouse_.OnMouseMove(x, y);
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			const int x = LOWORD(lParam);
			const int y = HIWORD(lParam);
			mouse_.OnLeftPressed(x, y);
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			const int x = LOWORD(lParam);
			const int y = HIWORD(lParam);
			mouse_.OnRightPressed(x, y);
			return 0;
		}
		case WM_MBUTTONDOWN:
		{
			const int x = LOWORD(lParam);
			const int y = HIWORD(lParam);
			mouse_.OnMiddlePressed(x, y);
			return 0;
		}
		case WM_LBUTTONUP:
		{
			const int x = LOWORD(lParam);
			const int y = HIWORD(lParam);
			mouse_.OnLeftReleased(x, y);
			return 0;
		}
		case WM_RBUTTONUP:
		{
			const int x = LOWORD(lParam);
			const int y = HIWORD(lParam);
			mouse_.OnRightReleased(x, y);
			return 0;
		}
		case WM_MBUTTONUP:
		{
			const int x = LOWORD(lParam);
			const int y = HIWORD(lParam);
			mouse_.OnMiddleReleased(x, y);
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			const int x = LOWORD(lParam);
			const int y = HIWORD(lParam);
			if(GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			{
				mouse_.OnWheelUp(x, y);
			}
			else if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			{
				mouse_.OnWheelDown(x, y);
			}
			return 0;
		}
		case WM_INPUT:
		{
			UINT dataSize;
			GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER)); //Need to populate data size first

			if (dataSize > 0)
			{
				const auto raw_data = std::make_unique<BYTE[]>(dataSize);
				if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, raw_data.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
				{
					const RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(raw_data.get());
					if (raw->header.dwType == RIM_TYPEMOUSE)
					{
						mouse_.OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
					}
				}
			}

			return DefWindowProc(hwnd, uMsg, wParam, lParam); //Need to call DefWindowProc for WM_INPUT messages
		}
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

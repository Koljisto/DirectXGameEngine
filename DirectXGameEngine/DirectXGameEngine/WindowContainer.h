﻿#pragma once
#include "RenderWindow.h"
#include "Keyboard/KeyboardClass.h"
#include "Mouse/MouseClass.h"
#include "Graphics/Graphics.h"

class WindowContainer
{
public:
	WindowContainer();
	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	//WindowProc
	//Mouse
	//Keyboard
protected:
	RenderWindow render_window_;
	KeyboardClass keyboard_;
	MouseClass mouse_;
	Graphics gfx_;

};

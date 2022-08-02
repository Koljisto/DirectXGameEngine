#pragma once
#include "WindowContainer.h"
class Engine : WindowContainer
{
public:
	bool Initialize(HINSTANCE h_instance, const std::string& window_title, const std::string& window_class, int width, int height);
	bool ProcessMessages();
	void Update();
	void RenderFrame();
};

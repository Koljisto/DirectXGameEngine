#pragma once
#include "ErrorLogger.h"

class WindowContainer;

class RenderWindow
{
public:
	bool Initialize(WindowContainer * p_window_container, HINSTANCE h_instance, const std::string& window_title, const std::string& window_class, int width, int height);
	bool ProcessMessages();
	HWND GetHWND() const;
	~RenderWindow();
private:
	void RegisterWindowClass() const;
	HWND handle_ = nullptr;
	HINSTANCE h_instance_ = nullptr;
	std::string window_title_;
	std::wstring window_title_wide_;
	std::string window_class_;
	std::wstring window_class_wide_;
	int width_ = 0;
	int height_ = 0;
};

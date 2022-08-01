#include "Engine.h"

bool Engine::Initialize(const HINSTANCE h_instance, const std::string& window_title, const std::string& window_class,
                        const int width, const int height)
{
	return this->render_window_.Initialize(this, h_instance, window_title, window_class, width, height);
}

bool Engine::ProcessMessages()
{
	return this->render_window_.ProcessMessages();
}

void Engine::Update()
{
	while(!keyboard_.CharBufferIsEmpty())
	{
		const unsigned char ch = keyboard_.ReadChar();
	}
	while (!keyboard_.KeyBufferIsEmpty())
	{
		KeyboardEvent keyboard_event = keyboard_.ReadKey();
		const unsigned char key_code = keyboard_event.GetKeyCode();
	}
	while(!mouse_.EventBufferIsEmpty())
	{
		MouseEvent em = mouse_.ReadEvent();
	}
}

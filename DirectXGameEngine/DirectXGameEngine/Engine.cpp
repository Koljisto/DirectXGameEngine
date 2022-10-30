#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	timer.Start();

	if (!this->render_window.Initialize(this, hInstance, window_title, window_class, width, height))
		return false;

	if (!gfx.Initialize(this->render_window.GetHWND(), width, height))
		return false;

	return true;
}

bool Engine::ProcessMessages()
{
	return this->render_window.ProcessMessages();
}

void Engine::Update()
{
	float dt = timer.GetMillisecondsElapsed();
	timer.Restart();

	while (!keyboard.CharBufferIsEmpty())
	{
		unsigned char ch = keyboard.ReadChar();
	}

	while (!keyboard.KeyBufferIsEmpty())
	{
		KeyboardEvent kbe = keyboard.ReadKey();
		unsigned char keycode = kbe.GetKeyCode();
	}

	while (!mouse.EventBufferIsEmpty())
	{
		MouseEvent me = mouse.ReadEvent();
		if (mouse.IsRightDown())
		{
			if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
			{
				this->gfx.camera3D.AdjustRotation(static_cast<float>(me.GetPosY()) * 0.01f, static_cast<float>(me.GetPosX()) * 0.01f, 0);
			}
		}
	}

	this->gfx.gameObject.AdjustRotation(0.0f, 0.001f * dt, 0.0f);

	float cameraSpeed = 0.006f;
	if(keyboard.KeyIsPressed(VK_SHIFT))
	{
		cameraSpeed = 0.3f;
	}
	if (keyboard.KeyIsPressed('W'))
	{
		this->gfx.camera3D.AdjustPosition(this->gfx.camera3D.GetForwardVector() * cameraSpeed * dt);
	}
	if (keyboard.KeyIsPressed('S'))
	{
		this->gfx.camera3D.AdjustPosition(this->gfx.camera3D.GetBackwardVector() * cameraSpeed * dt);
	}
	if (keyboard.KeyIsPressed('A'))
	{
		this->gfx.camera3D.AdjustPosition(this->gfx.camera3D.GetLeftVector() * cameraSpeed * dt);
	}
	if (keyboard.KeyIsPressed('D'))
	{
		this->gfx.camera3D.AdjustPosition(this->gfx.camera3D.GetRightVector() * cameraSpeed * dt);
	}
	if (keyboard.KeyIsPressed(VK_SPACE))
	{
		this->gfx.camera3D.AdjustPosition(0.0f, cameraSpeed * dt, 0.0f);
	}
	if (keyboard.KeyIsPressed('C'))
	{
		this->gfx.camera3D.AdjustPosition(0.0f, -cameraSpeed * dt, 0.0f);
	}

	if (keyboard.KeyIsPressed('Z'))
	{
		XMVECTOR lightPosition = this->gfx.camera3D.GetPositionVector();
		lightPosition += this->gfx.camera3D.GetForwardVector();
		this->gfx.light.SetPosition(lightPosition);
		this->gfx.light.SetRotation(this->gfx.camera3D.GetRotationFloat3());
	}
	
}

void Engine::RenderFrame()
{
	this->gfx.RenderFrame();
}

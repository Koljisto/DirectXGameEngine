#include "Engine.h"

int APIENTRY wWinMain(_In_ const HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nShowCmd)
{
	Engine engine;
	if(engine.Initialize(hInstance, "Title", "MyWindowClass", 800, 600))
	{
		while (engine.ProcessMessages() == true)
		{
			engine.Update();
			engine.RenderFrame();
		}
	}
	return 0;
}

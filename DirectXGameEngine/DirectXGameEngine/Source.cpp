#include "Engine.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")

int APIENTRY wWinMain(_In_ const HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nShowCmd)
{
	Engine engine;
	engine.Initialize(hInstance, "Title", "MyWindowClass", 800, 600);
	while (engine.ProcessMessages() == true)
	{
		engine.Update();
	}
	return 0;
}

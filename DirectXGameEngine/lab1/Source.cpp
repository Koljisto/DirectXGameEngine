#include "Game.h"

int APIENTRY wWinMain(_In_ const HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nShowCmd)
{
	const HRESULT hr = CoInitialize(NULL);

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to call CoInitialize.");
		return -1;
	}

	Game game;

	if (game.Initialize(hInstance, "Title", "MyWindowClass", 1600, 900))
	{
		while (game.ProcessMessages() == true)
		{
			game.Update();
			game.RenderFrame();
		}
	}
	return 0;
}

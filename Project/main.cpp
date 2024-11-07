#include <Windows.h>
#include "Engine/Framework/MyFramework.h"
#include "Engine/Framework/MyGame.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	MyFramework* game = new MyGame();

	game->Run();

	delete game;

	return 0;
}

#include "DxLib.h"
#include "GameApp/GameApp.h"

// [EN] Entry Point [JP] プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	GameApp gameApp{};

	// [EN] Initialize GameApp. Exit if failed. [JP] GameAppの初期化をする。失敗したら終了する
	if (!gameApp.Initialize())
	{
		return -1;
	}

	// [EN] Start Game Loop. [JP] ゲームループの開始
	gameApp.Run();

	return 0; 
}
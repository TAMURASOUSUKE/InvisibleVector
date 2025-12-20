#include "DxLib.h"
#include "GameApp/GameApp.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	GameApp gameApp{}; // instance of gameapp class.

	// 初期化処理に失敗したら-1を返す If failed initialize return -1.
	if (!gameApp.Initialize())
	{
		return -1;
	}

	// gameLoop
	gameApp.Run();

	return 0;				// ソフトの終了 
}
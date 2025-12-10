#include "DxLib.h"


/*
	エントリーポイントを実装する
*/

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetGraphMode(1280, 720, 32); // 画面サイズの調整
	ChangeWindowMode(true); // ウィンドウモードに設定
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	SetDrawScreen(DX_SCREEN_BACK); // 裏画面(バックバッファの設定)

	// 3D設定 -----------------------------------------------------------------

	SetUseBackCulling(TRUE); // 裏画面の設定
	SetUseZBuffer3D(true); // zバッファの有効化
	SetWriteZBuffer3D(true); // zバッファへの書き込み許可
	SetUseLighting(true); // ライトの有効化
	SetCameraNearFar(0.1f, 1000.0f); // カメラのクリップ距離
	SetCameraPositionAndTarget_UpVecY(VGet(320.0f, 240.0f, -10.0f), VGet(320.0f, 240.0f, 1.0f)); // カメラの位置と注視点の設定(位置途中視点はデフォルトです)


	// ------------------------------------------------------------------------

	// ゲームループの実装
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{


		DrawFormatString(20, 20, GetColor(255, 255, 255), "Hello_World_Debug");
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}
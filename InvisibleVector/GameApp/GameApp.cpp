#include "DxLib.h"
#include <timeapi.h>
#include "../Constant/GameConstant.h"
#include "GameApp.h"

#pragma comment(lib, "winmm.lib")

// [EN] Constructor: Initialze variables [JP] コンストラクタ: 変数の初期化などを行う
GameApp::GameApp()
{
	timeBeginPeriod(1);
}


// [EN] Finalize Dxlib resource. [JP] Dxlibの終了処理を行う
GameApp::~GameApp()
{
	timeEndPeriod(1);

	// [EN] Ensure Dxlib is shut down correctly.
	DxLib_End();
}

// [EN] Initialize system settings. [JP] Dxlibなどのシステムを設定する
bool GameApp::Initialize()
{
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, COLOR_DEPTH);
	ChangeWindowMode(true);

	// [EN] Initialize Dxli. Returns -1 if failed. [JP] Dxlibの初期化をする-1が返ればfalseを返す
	if (DxLib_Init() == -1)	
	{
		return false;
	}

	// [EN] Initialize color handles.
	// [JP] Dxlibの機能を使って初期化するためコンストラクタではなくInitializeで初期化
	white = GetColor(255, 255, 255);
	red = GetColor(255, 0, 0);

	// [EN] Set draw target to the back buffer(Double Buffering) [JP] 描画するバッファをバックバッファに設定する
	SetDrawScreen(DX_SCREEN_BACK);
	// [EN] 3D Configration [JP] 3D設定 -----------------------------------------------------------------

	SetUseBackCulling(true); // [EN] Enable back-face culling [JP] 背面のカリングを有効化
	SetUseZBuffer3D(true); // [EN] Enable Z-buffer [JP] Zバッファを有効化
	SetWriteZBuffer3D(true); // [EN] Allow writing to Z-buffer [JP] Zバッファへの書き込みを許可
	SetUseLighting(true); // [EN] Enable lighting [JP] ライトの有効化
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR); // [EN] Set clip distance [JP] クリップ距離の設定

	// TODO:
	// [EN] Move camera settings to Player or Camera class later. Currently using magic numbers for testing purporse.
	// [JP] カメラに関してはプレイヤーで使うのであくまでこれはテスト用。プレイヤー実装時にカメラの設定をプレイヤーに移すのでいったんはマジックナンバーを許容する
	SetCameraPositionAndTarget_UpVecY(VGet(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, -100.0f), VGet(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 1.0f)); // カメラの位置と注視点の設定(位置途中視点はデフォルトです)
	// ------------------------------------------------------------------------

	return true;
}


// [EN] Main Game Loop [JP] メインループを実装
void GameApp::Run()
{
	// [EN] Loop until ProcessMessage fails or Escape key is pressed. [JP] プロセスメッセージが失敗するかESCキーが押されるまでループする 
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		frameController.BeginFrame();

		Update();
		Draw();

		frameController.EndFrame();
	}
}

// [EN] Update Game Logic. [JP] ゲームロジックの更新
void GameApp::Update()
{

}

// [EN] Render Frame
void GameApp::Draw()
{

	// [EN] Draw 3D area [JP] 3D領域の描画

	SetUseZBuffer3D(true); // [EN] Enable Z-buffer for 3D drawing. [JP] 3D描画のためZバッファを有効化する
	SetWriteZBuffer3D(true); // [EN] Enable writing to Z-buffer for 3D drawing. [JP] 3D描画のためZバッファへの書き込みを有効化する

	ClearDrawScreen();

	// [EN] Draw debug text [JP] デバッグ用のテキスト
	DrawFormatString(20, 20, white, "Hello_World_Debug");

	// [EN] Draw debug sphere [JP] デバッグ用の球
	DrawSphere3D(VGet(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 32, 32, red, red, true);


	// [EN] Draw 2D area FPS. [JP] 2D領域にFPSを描画する

	SetUseZBuffer3D(false); // [EN] Disable Z-buffer for 2D drawing. [JP] 2D描画のためZバッファを無効化する
	SetWriteZBuffer3D(false); // [EN] Disable writing to Z-buffer for 2D drawing. [JP] 2D描画のためZバッファへの書き込みを無効化する

#ifdef _DEBUG

	DrawFormatString(DEBUG_FPS_POSITION_X, DEBUG_FPS_POSITION_Y, GetColor(255, 255, 255), "%.1f", frameController.GetCurrentFPS());

#endif // _DEBUG


	ScreenFlip();
}
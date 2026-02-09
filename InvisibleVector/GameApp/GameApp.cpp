#include "DxLib.h"
#include <timeapi.h>
#include "GameConstant.h"
#include "../Math/Vector_Dxlib.h"
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

	// [EN] Initialize ObjectFactory [JP] 生成クラスを初期化するためにマネージャーを渡す
	objectFactory = std::make_unique<ObjectFactory>(&objectManager);

	return true;
}


// [EN] Main Game Loop [JP] メインループを実装
void GameApp::Run()
{
	// [EN] Loop until ProcessMessage fails or Escape key is pressed. [JP] プロセスメッセージが失敗するかESCキーが押されるまでループする 
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		frameController.BeginFrame();
		inputManager.Update(); // 入力更新(システムの更新なのでUpdate関数の外側)

		Update();
		Draw();

		frameController.EndFrame();
	}
}

// [EN] Update Game Logic. [JP] ゲームロジックの更新
void GameApp::Update()
{

	// FixedUpdate更新
	while (frameController.IsFixedUpdateRequired())
	{
		objectManager.FixedUpdate();

		frameController.ConsumeFixedTime();
	}

	// オブジェクト更新
	objectManager.Update();

	// 使わなくなったデータ等を消す
	objectManager.Refresh();
}

// [EN] Render Frame
void GameApp::Draw()
{

// ======================================================== [EN] Draw 3D area [JP] 3D領域の描画 ================================================================

	SetUseZBuffer3D(true); // [EN] Enable Z-buffer for 3D drawing. [JP] 3D描画のためZバッファを有効化する
	SetWriteZBuffer3D(true); // [EN] Enable writing to Z-buffer for 3D drawing. [JP] 3D描画のためZバッファへの書き込みを有効化する

	ClearDrawScreen();




#ifdef _DEBUG


// ======================================================== [EN] Draw 2D area FPS. [JP] 2D領域にFPSを描画する ==================================================

	SetUseZBuffer3D(false); // [EN] Disable Z-buffer for 2D drawing. [JP] 2D描画のためZバッファを無効化する
	SetWriteZBuffer3D(false); // [EN] Disable writing to Z-buffer for 2D drawing. [JP] 2D描画のためZバッファへの書き込みを無効化する


	DrawFormatString(DEBUG_FPS_POSITION_X, DEBUG_FPS_POSITION_Y, GetColor(255, 255, 255), "現在のFPS : %.1f", frameController.GetCurrentFPS());
	DrawFormatString(DEBUG_DELTA_TIME_POSITION_X, DEBUG_DELTA_TIME_POSITION_Y, GetColor(255, 255, 255), "Delta Time : %.6f", frameController.GetDeltaTime());

	Vector2 inputDebugDrawPos{ 20.0f, 100.0f };
	float lineHight{ 20.0f };

	DrawString(inputDebugDrawPos.x, inputDebugDrawPos.y, "--- Input Debug ---", white);
	inputDebugDrawPos.y += lineHight;

	// 軸入力の確認
	Vector2 axis{ inputManager.GetAxis() };
	DrawFormatString(inputDebugDrawPos.x, inputDebugDrawPos.y, white, "Axis: (%.2f, %.2f)", axis.x, axis.y);

	inputDebugDrawPos.y += lineHight;

	// ボタンの確認
	// 押されていれば文字が赤くなるようにする
	auto DrawButtonState = [&](const char* name, GameKey key)
		{
			unsigned int color = inputManager.GetButtonStay(key) ? red : white; // 押し続けていれば赤

			const char* status = ""; // ステータスを文字列にする
			if (inputManager.GetButtonDown(key)) status = "DWN";
			if (inputManager.GetButtonUp(key)) status = "UP";

			DrawFormatString(inputDebugDrawPos.x, inputDebugDrawPos.y, color, "[%s] %s %s", status, name, inputManager.GetButtonStay(key) ? "ON" : "off");

			inputDebugDrawPos.y += lineHight;

		};

	// 主要なキーを表示
	DrawButtonState("Jump", GameKey::Jump);
	DrawButtonState("Dash", GameKey::Dash);
	DrawButtonState("Crouch", GameKey::Crouch);
	DrawButtonState("Up", GameKey::Up);
	DrawButtonState("Down", GameKey::Down);
	DrawButtonState("Left", GameKey::Left);
	DrawButtonState("Right", GameKey::Right);

	// JSON保存のテスト用
	if (inputManager.GetButtonDown(GameKey::Zoom))
	{
		DrawString(200, 100, "Save Config Triggered!", red);
		inputManager.SaveConfig();
	}


	// キーコンフィグを変更できるか確認する
	static bool isRebindingJump{ false }; // 新しくキーを設定するかどうかを判定するフラグ(簡易的なテストなのでstatic)

	if (isRebindingJump)
	{
		// 変更モード中を表示
		DrawString(inputDebugDrawPos.x, inputDebugDrawPos.y, ">> Press Any Key for [JUMP] <<", red);

		int newKey{ inputManager.GetAnyPressedKey() }; // 入力されたキーを判別し保存する

		// キーが押されているかつ変更モードを起動するキー以外が押されたら登録する
		if (newKey != -1 && newKey != KEY_INPUT_C)
		{
			// 入力されたキーに変更
			inputManager.SetBinding(GameKey::Jump, newKey, PAD_INPUT_A);

			// 保存する
			inputManager.SaveConfig();

			// 変更モード終了
			isRebindingJump = false;
		}
	}
	else
	{
		// 通常時の文字列表示
		DrawString(inputDebugDrawPos.x, inputDebugDrawPos.y, "[C] key : Change 'Jump' Binding", white);

		// Cキーで変更モードへ
		if (CheckHitKey(KEY_INPUT_C))
		{
			isRebindingJump = true;
		}
	}

#endif // _DEBUG


	ScreenFlip();
}
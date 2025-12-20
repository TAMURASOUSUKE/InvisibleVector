#include "DxLib.h"
#include "../Constant/GameConstant.h"
#include "GameApp.h"


/// <summary>
/// 必要な変数の初期化などを担当する
/// </summary>
GameApp::GameApp()
{

}


/// <summary>
/// インスタンスが破棄される時にDxlibも終了するようにする
/// </summary>
GameApp::~GameApp()
{
	DxLib_End(); // DXライブラリ使用の終了処理 Dxlib finish process.
}

/// <summary>
/// 画面サイズの設定とウィンドウモードの設定、バッファ、3D関係の設定を行い
/// Dxlibの初期化を行う。
/// Set the screen size and window mode and buffer and 3D related and initialize Dxlib.
/// </summary>
/// <returns>初期化成功ならture successful return true、失敗ならfalse fails return false</returns>
bool GameApp::Initialize()
{
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, COLOR_DEPTH);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理 Dxlib initialize process.
	{
		return false;
	}


	// Dxlibの機能を使って初期化するためコンストラクタではなくInitializeで初期化
	white = GetColor(255, 255, 255);
	red = GetColor(255, 0, 0);

	SetDrawScreen(DX_SCREEN_BACK); // 裏画面(バックバッファの設定) Setting of back buffer.
	// 3D設定 -----------------------------------------------------------------

	SetUseBackCulling(true); // 裏画面の設定
	SetUseZBuffer3D(true); // zバッファの有効化
	SetWriteZBuffer3D(true); // zバッファへの書き込み許可
	SetUseLighting(true); // ライトの有効化
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR); // カメラのクリップ距離

	// カメラに関してはプレイヤーで使うのであくまでこれはテスト用。プレイヤー実装時にカメラの設定をプレイヤーに移すのでいったんはマジックナンバーを許容する
	SetCameraPositionAndTarget_UpVecY(VGet(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, -100.0f), VGet(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 1.0f)); // カメラの位置と注視点の設定(位置途中視点はデフォルトです)
	// ------------------------------------------------------------------------

	return true;
}


/// <summary>
/// ゲームループを実装する
/// 入力の更新、オブジェクトの更新、画面への表示を担当する
/// ESCキーによってループを終了することができるようにする
/// </summary>
void GameApp::Run()
{
	// ゲームループの実装 implement of the gameloop.
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		Update();
		Draw();
	}
}

/// <summary>
/// 入力やゲームワールドの更新を行う
/// ほかクラスの更新は直接ゲームループに書くのではなく
/// ここに書くことでデバッグの効率を上げる
/// </summary>
void GameApp::Update()
{

}

/// <summary>
/// 描画や画面クリア、画面フリップを担当する
/// ほかクラスの描画は直接ゲームループに書くのではなく
/// ここに書くことでデバッグの効率を上げる
/// </summary>
void GameApp::Draw()
{

	ClearDrawScreen();

	// 今後汎用的なカラークラスを作りそこから呼び出すことで計算量軽減を図る
	DrawFormatString(20, 20, white, "Hello_World_Debug");

	DrawSphere3D(VGet(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 32, 32, red, red, true);

	ScreenFlip();
}
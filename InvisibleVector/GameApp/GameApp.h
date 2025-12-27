#pragma once
#include "SystemConstant.h"
#include "../System/FPS.h"


/// <summary>
/// [EN] Manages the application entry point and tha main game loop.
/// Separates logic from WinMain to improve readbility.
/// 
/// [JP] エントリーポイントとゲームループを分離し可読性を向上させるためのクラス。
/// インスタンスがグローバル変数や静的変数として呼ばれた場合にも対応できるように、
/// 明示的に初期化関数を作成しゲームループ開始前に呼ぶようにする
/// </summary>
class GameApp
{
public:
	GameApp(); // [EN] Constructor: Initialze variables [JP] コンストラクタ: 変数の初期化などを行う
	~GameApp(); // [EN] Finalize Dxlib resource. [JP] Dxlibの終了処理を行う

	// [EN] Delete copy constructor and assignment to prevent copying.
	// The instance should be unique and owned by Main.cpp.
	// [JP] コピーコンストラクタとコピー代入を禁止する
	// このクラスのインスタンスはMain.cppが唯一の所有者とする
	GameApp(const GameApp& other) = delete;
	GameApp& operator=(const GameApp&) = delete;

	/// <summary>
	/// [EN] Initialize Dxlib, Window settings, and 3D configration.
	/// [JP] ウィンドウの設定や3D関連の設定、Dxlibの初期化を行う
	/// </summary>
	/// <returns>[EN] Returns true if initialization succeeds, false otherwise. [JP] 初期化が成功すればtrueを返す そうでないならfalse</returns>
	[[nodiscard]]bool Initialize();
	/// <summary>
	/// [EN] Start the main game loop.
	/// [JP] メインループを始める
	/// </summary>
	void Run();

private:
	// [EN] Update input and game objects. [JP] 入力やオブジェクトを更新する
	void Update();
	// [EN] Clear screen, draw objects, and flip screen. [JP] 画面のクリアやオブジェクトの描画、画面フリップを行う
	void Draw();

private:
	FPS fps{DEBUG_FPS_POSITION_X, DEBUG_FPS_POSITION_Y };
	unsigned int white{0};
	unsigned int red{0};
};
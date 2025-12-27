#pragma once
#include "SystemConstant.h"

/// <summary>
/// [EN] Managed FPS and controls wait time. [JP] FPSの管理と待機時間の制御を行う
/// </summary>
class FPS
{
public:

	/// <summary>
	/// [EN] Constructor : Initialize position to draw FPS. [JP] コンストラクタ : FPSを描画する位置を初期化する
	/// </summary>
	/// <param name="x"> [EN] PositionX coordinate [JP] X座標 </param>
	/// <param name="y"> [EN] PositionY coordinate [JP] Y座標 </param>
	FPS(const float x, const float y) : positionX{ x }, positionY{y} {}

	/// <summary>
	/// [EN] Prohibit copying and limit ownership [JP] コピー禁止し所有者を限定する
	/// </summary>
	/// <param name="other"></param>
	FPS(const FPS& other) = delete;
	FPS operator=(const FPS& other) = delete;

	/// <summary>
	/// [EN] Update FPS counter and control wait time to maintain target FPS. Called once per frame. [JP] FPSカウンタの更新と目標FPSを維持するための待機時間の制御を行う。1フレームに1回呼ばれる
	/// </summary>
	void Update();

	/// <summary>
	/// [EN] Draw FPS on the screen. Called once per frame after Update. 
	///	This function is only for debugging purposes. 
	/// [JP] 画面にFPSを描画する。Updateの後に1フレームに1回呼ばれる
	///	この関数はデバッグ目的で使用する
	/// </summary>
	void Draw();

	/// <summary>
	/// [EN] Control wait time to maintain target FPS. [JP] 目標FPSを維持するための待機時間の制御を行う
	/// </summary>
	void Wait();
private :

	LONGLONG startTime{ 0 }; // [EN] Start measurement time. [JP] FPSの測定開始時刻

	int frameCounter{ 0 }; // [EN] Frame counter. [JP] フレームカウンタ

    float currentFPS{ 0 }; // [EN] Current FPS value. [JP] 現在のFPS値
	float positionX{ 0.0f }; // [EN] PositionX coordinate [JP] X座標
	float positionY{ 0.0f }; // [EN] PositionY coordinate [JP] Y座標
};
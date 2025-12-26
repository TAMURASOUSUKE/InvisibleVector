#pragma once
#include <cmath>
#include "DxLib.h"
#include "../Base/ObjectBase.h"


/// <summary>
/// [EN] Managed FPS and controls wait time. [JP] FPSの管理と待機時間の制御を行う
/// </summary>
class FPS : public ObjectBase
{
public :

	/// <summary>
	/// [EN] Update FPS counter and control wait time to maintain target FPS. Called once per frame. [JP] FPSカウンタの更新と目標FPSを維持するための待機時間の制御を行う。1フレームに1回呼ばれる
	/// </summary>
	void Update() override;

	/// <summary>
	/// [EN] Draw FPS on the screen. Called once per frame after Update. 
	///	　　 This function is only for debugging purposes. 
	/// [JP] 画面にFPSを描画する。Updateの後に1フレームに1回呼ばれる
	///		 この関数はデバッグ目的で使用する
	/// </summary>
	void Draw() override;
private :
	int startTime{ 0 }; // [EN] Start measurement time. [JP] FPSの測定開始時刻

};
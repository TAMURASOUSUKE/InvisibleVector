#pragma once
#include <chrono>
#include "SystemConstant.h"
#include "FrameRateCounter.h"
#include "FrameRateLimiter.h"


/// <summary>
/// [EN] Manage the FPS and  delta time. [JP] FPSとdeltaタイムの管理を行う
/// </summary>
class FrameController
{	
public:
	/// <summary>
	/// [EN] Constructor specifying FPS. [JP] FPSを指定するコンストラクタ
	/// </summary>
	/// <param name="FPS"> [EN] specifying FPS. [JP] FPSの指定</param>
	FrameController(int FPS) : limiter{FPS} {}
	~FrameController() = default;

	// [EN] Prohibited copy of this class. [JP] このクラスのコピーを禁止する
	FrameController(const FrameController& other) = delete;
	FrameController operator= (FrameController other) = delete;

	/// <summary>
	/// [EN] First processing of the frame [JP] 最初のフレーム処理
	/// </summary>
	void BeginFrame();
 
	/// <summary>
	/// [EN] End of frame processing. [JP] フレームの最後の処理
	/// </summary>
	void EndFrame();

	/// <summary>
	/// [EN] Get current FPS [JP] 現在のFPSを取得する
	/// </summary>
	/// <returns>current FPS</returns>
	float GetCurrentFPS() const { return counter.GetCurrentFPS(); }

	/// <summary>
	/// [EN] Get delta time. [JP] 前フレームからの経過時間(秒)を取得 
	/// </summary>
	/// <returns> [EN] deltaTime. [JP] デルタタイム </returns>
	float GetDeltaTime() const { return deltaTime; }

private:
	time_point<steady_clock> startTime{}; // [EN] For limiter wait start. [JP] 待機関数用の開始フレーム 
	time_point<steady_clock> prevFrameStartTime{}; // [EN] For delta time calculation. [JP] デルタタイム計算用

	FrameRateCounter counter{};
	FrameRateLimiter limiter{0};

	float deltaTime{ 0.0f }; // [EN] Delat time.(seconds) [JP] デルタタイム(秒)

};
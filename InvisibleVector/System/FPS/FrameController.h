#pragma once
#include <chrono>
#include "FrameRateCounter.h"
#include "FrameRateLimiter.h"


/// <summary>
/// [EN] Manage the FPS. [JP] FPSの管理を行う
/// </summary>
class FrameRateController
{	
public:
	/// <summary>
	/// [EN] Constructor specifying FPS. [JP] FPSを指定するコンストラクタ
	/// </summary>
	/// <param name="FPS"> [EN] specifying FPS. [JP] FPSの指定</param>
	FrameRateController(int FPS) : limiter{FPS} {}
	~FrameRateController() = default;

	// [EN] Prohibited copy of this class. [JP] このクラスのコピーを禁止する
	FrameRateController(const FrameRateController& other) = delete;
	FrameRateController operator= (FrameRateController other) = delete;

	/// <summary>
	/// [EN] First processing of the frame [JP] 最初のフレーム処理
	/// </summary>
	void BeginFrame() { startTime = steady_clock::now(); }

	/// <summary>
	/// [EN] End of frame processing. [JP] フレームの最後の処理
	/// </summary>
	void EndFrame()
	{
		limiter.Wait(startTime);

		counter.Update(startTime);
	}

	/// <summary>
	/// [EN] Get current FPS [JP] 現在のFPSを取得する
	/// </summary>
	/// <returns>current FPS</returns>
	float GetCurrentFPS() const { return counter.GetCurrentFPS(); }

private:
	time_point<steady_clock> startTime{};
	FrameRateCounter counter{};
	FrameRateLimiter limiter{0};

};
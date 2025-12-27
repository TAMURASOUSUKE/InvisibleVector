#pragma once
#include <chrono>
#include "SystemConstant.h"


using namespace std::chrono;

/// <summary>
/// [EN] This class takes the current time and adjust it to the specified FPS. [JP] 現在の時間を受け取り指定されたFPSになるよう調整するクラスです
/// </summary>
class FrameRateLimiter
{
public:
	/// <summary>
	/// [EN] Constructor : receive of specify FPS. [JP] 引数としてFPSを受け取る
	/// </summary>
	/// <param name="targetFPS"> [EN] FPS to adjust [JP] 調整したいFPSを入れる </param>
	FrameRateLimiter(int targetFPS)
	{
		targetDuration = microseconds(static_cast<long long>(ONE_SEC_MICRO / targetFPS));
	}
	~FrameRateLimiter() = default;


	// [EN] Prohibited copy on this class. [JP] コピーを禁止する
	FrameRateLimiter(const FrameRateLimiter& other) = delete;
	FrameRateLimiter operator=(FrameRateLimiter other) = delete;

	/// <summary>
	///	[EN] Calculate ideal time and duration of start time to now time, then wait difference times.
	/// [JP] 理想の時間とかかった時間を求めて、それらの差の分だけ待機する
	/// </summary>
	/// <param name="startTime"> [EN] Start time. [JP] 最初の時間 </param>
	void Wait(time_point<steady_clock> startTime);
private:
	microseconds targetDuration{}; // [EN] Converts the given value to a time. [JP] 指定されたFPSを時間に変換する
};
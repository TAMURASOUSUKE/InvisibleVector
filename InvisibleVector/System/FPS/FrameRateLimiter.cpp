#include <intrin.h>
#include "DxLib.h"
#include "FrameRateLimiter.h"

void FrameRateLimiter::Wait(time_point<steady_clock> startTime)
{
	// [EN] Calculate target time. [JP] 目標となる時間を計算
	auto targetTime = startTime + targetDuration;

	// [EN] Get the now time. [JP] 現在の時間を取得
	auto nowTime = steady_clock::now();

	// [EN] Calculate remaining time. [JP] 残り時間を計算する
	auto remainingTime = targetTime - nowTime;

	// [EN] Until has times. [JP] まだ時間があるかチェック
	if (remainingTime.count() <= 0)
	{
		// [EN] Processing for slowdowns. [JP] ここでは処理落ち用の処理を書く
		return;
	}

	auto sleepMargin = milliseconds(1);

	if (remainingTime > sleepMargin)
	{
		auto sleepTime = duration_cast<milliseconds>(remainingTime - sleepMargin);

		Sleep(static_cast<DWORD>(sleepTime.count()));
	}

	// [EN] Do SpinWait [JP] 目標時刻になるまでの間空回しさせる
	while (steady_clock::now() < targetTime)
	{
		_mm_pause(); // [EN] Optimization process. [JP] 最適化処理
	}
}
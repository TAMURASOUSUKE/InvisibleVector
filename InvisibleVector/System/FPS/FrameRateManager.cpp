#include <algorithm>
#include "SystemConstant.h"
#include "../Time/TimeManager.h"
#include "FrameRateManager.h"


FrameRateManager::FrameRateManager(int FPS) : limiter{ FPS }
{
	fixedDeltaTime = FIXED_DELTA_TIME;
	TimeManager::Bind(&deltaTime, &fixedDeltaTime, &currentFPS, &alpha);
}

// [EN] Zero clear of Bind function [JP] 結合用関数を終了時に0でクリアする
FrameRateManager::~FrameRateManager()
{
	TimeManager::Bind(&SEAF_ZERO, &SEAF_ZERO, &SEAF_ZERO, &SEAF_ZERO);
}

void FrameRateManager::BeginFrame()
{
	// [EN] Get now time. [JP] 現在の時間を取得
	auto now = steady_clock::now();

	// [EN] Calculate deleta time.(Skip first frame)
	// [JP] デルタタイムの計算を行う(2フレーム目以降)
	if (prevFrameStartTime.time_since_epoch().count() != 0)
	{
		duration<float> sec = now - prevFrameStartTime;

		float rawDeltaTime = sec.count();

		// [EN] Limit delta time to prevent slowdowns. [JP] 処理落ち対策のためデルタタイムを制限する
		deltaTime = std::min(rawDeltaTime, LIMIT_DELTA_TIME);
	}

	accumulator += deltaTime;

	// [EN] Prevent overflow when processing slows down [JP] 処理落ちした際にあふれるのを防止する
	if (accumulator > LIMIT_ACCUMULATOR) accumulator = LIMIT_ACCUMULATOR;

	CalculateAlpha();

	prevFrameStartTime = now;
	startTime = now;
	currentFPS = counter.GetCurrentFPS();


}

/// <summary>
/// [EN] A function to check the percentage of the remaining time in a fixed step
/// [JP] 残り時間が固定ステップの何割かを調べる関数
/// </summary>
void FrameRateManager::CalculateAlpha()
{
	alpha = accumulator / fixedDeltaTime;
}


void FrameRateManager::EndFrame()
{
	limiter.Wait(startTime);

	counter.Update(startTime);
}

void FrameRateManager::ConsumeFixedTime()
{
	accumulator -= FIXED_DELTA_TIME;
	CalculateAlpha();
}

bool FrameRateManager::IsFixedUpdateRequired()
{
	return accumulator >= FIXED_DELTA_TIME;
}
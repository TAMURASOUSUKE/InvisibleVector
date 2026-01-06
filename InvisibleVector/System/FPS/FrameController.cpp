#include <algorithm>
#include "SystemConstant.h"
#include "FrameController.h"

void FrameController::BeginFrame()
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

	prevFrameStartTime = now;
	startTime = now;


}


void FrameController::EndFrame()
{
	limiter.Wait(startTime);

	counter.Update(startTime);
}

void FrameController::ConsumeFixedTime()
{
	accumulator -= FIXED_DELTA_TIME;
}

bool FrameController::IsFixedUpdateRequired()
{
	return accumulator >= FIXED_DELTA_TIME;
}
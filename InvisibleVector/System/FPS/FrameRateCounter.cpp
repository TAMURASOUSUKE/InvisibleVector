#include "SystemConstant.h"
#include "FrameRateCounter.h"

void FrameRateCounter::Update(const time_point<steady_clock>& currentTime)
{
	// [EN] Initialize process. If first call assignment startTime to currentTime. [JP] 初期化処理。最初に呼び出されたときにstartTimeにcurrentTimeを代入する
	if (frameCounter == 0 && startTime.time_since_epoch().count() == 0)
	{
		startTime = currentTime;
	}

	// [EN] Calculate elapsed time then cast microseconds to it. [JP] 開始時刻から現在の時間までの経過を計算しそれをマイクロ秒に変換する
	auto elapsedTime = duration_cast<microseconds>(currentTime - startTime).count();

	// [EN] Calculate currentFPS. [JP] 現在のFPSを計算する処理
	if (frameCounter == SAMPLE_FRAME_COUNT)
	{
		if (elapsedTime > 0)
		{
			currentFPS = static_cast<float>(frameCounter) * ONE_SEC_MICRO / static_cast<float>(elapsedTime);
		}
		frameCounter = 0;
		startTime = currentTime;
	}

	frameCounter++;
}
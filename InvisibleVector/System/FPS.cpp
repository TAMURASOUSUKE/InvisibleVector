#include "DxLib.h"
#include "FPS.h"


/*
	[EN] Average Calculate at the per 60 frame. [JP] 60フレームごとに平均を計算する
*/
void FPS::Update()
{
	// [EN] If frameCounter is 0, record start time. [JP] frameCounterが0なら開始時間を記録する
	if (frameCounter == 0)
	{
		// [EN] Get high precision current time. [JP] 高精度の現在時刻を取得する
		startTime = GetNowHiPerformanceCount();
	}

	// [EN] If 60 frames have passed, calculate current FPS. [JP] 60フレーム経過したら現在のFPSを計算する
	if (frameCounter == SAMPLE_FRAME_COUNT)
	{
		LONGLONG t = GetNowHiPerformanceCount();
		currentFPS = static_cast<float>(SAMPLE_FRAME_COUNT * ONE_SEC_MICRO) / static_cast<float>(t - startTime); // [EN] Calculate current FPS. [JP] 現在のFPSを計算する
		frameCounter = 0; // [EN] Reset frame counter. [JP] フレームカウンタをリセットする
		startTime = t; // [EN] Update start time for next calculation. [JP] 次の計算のために開始時間を更新する
	}

	frameCounter++; // [EN] Increment frame counter. [JP] フレームカウンタをインクリメントする
}

void FPS::Draw()
{
#ifdef _DEBUG

	// [EN] Draw current FPS on the screen for debugging purposes. [JP] デバッグ目的で画面に現在のFPSを描画する
	DrawFormatString(static_cast<int>(positionX), static_cast<int>(positionY), GetColor(255, 255, 255), "FPS: %.1f", currentFPS);

#endif // _DEBUG
}

void FPS::Wait()
{
	// [EN] Calculate taken time. [JP] かかった時間の計測
	LONGLONG tookTime = GetNowHiPerformanceCount() - startTime;
	// [EN] Substraction : Ideal FPS - Taken time. [JP] 理想のかかるべき時間から実際にかかった時間を引く
 	LONGLONG waitTime = frameCounter * ONE_SEC_MICRO / USE_FPS - tookTime;
	// [EN] If wait time is positive,it is considered too early and waits for that amount of time.
	// [JP] もしwaitTimeが正なら速すぎるとみなしその分待機する
	if (waitTime > 0)
	{
		Sleep(static_cast<DWORD>(waitTime / ONE_SEC_MILLI));
	}
}
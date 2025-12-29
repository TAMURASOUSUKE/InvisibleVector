#pragma once
#include <chrono>


/// <summary>
/// [EN] Counts frame rate (FPS). [JP] フレームレート(FPS)の計測を行う
/// </summary>

using namespace std::chrono; // [EN] For high precision time measurement. [JP] 高精度な時間計測のため

class FrameRateCounter
{
public :

	FrameRateCounter() = default;
	~FrameRateCounter() = default;

	// [EN] Prohibited copy of this class. [JP] このクラスのコピーを禁止する
	FrameRateCounter(const FrameRateCounter& other) = delete;
	FrameRateCounter operator=(FrameRateCounter other) = delete;

	/// <summary>
	/// [EN] Count of frame rate. [JP] フレームレートの計測を行う
	/// </summary>
	/// <param name="currentTime"> [EN] Current time point. [JP] 現在の時間点 </param>
	void Update(const time_point<steady_clock>& currentTime);

	/// <summary>
	/// [EN] Getter of currentFPS. [JP] CurrentFPSを取得する用のGetter
	/// </summary>
	/// <returns> [EN] CurrentFPS [JP] 現在のFPS</returns>
	float GetCurrentFPS() const { return currentFPS; }
private:
	time_point<steady_clock> startTime{}; // [EN] Start measurement time. [JP] FPSの測定開始時刻
	int frameCounter{ 0 }; // [EN] Frame counter. [JP] フレームカウンタ
	float currentFPS{ 0.0f }; // [EN] Current FPS value. [JP] 現在のFPS値

};
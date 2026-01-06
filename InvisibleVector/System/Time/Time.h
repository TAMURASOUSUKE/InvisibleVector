#pragma once

/// <summary>
/// [EN] Manage of delta time and fixed delta time. [JP] DeltaTimeとFixedDeltaTimeを管理する
/// </summary>
class Time
{
public:
	/// <summary>
	/// [EN] Prohibited of instance [JP] インスタンスを作ることを禁止する
	/// </summary>
	Time() = delete;

	// [EN] Prohibited copy of this class. [JP] このクラスのコピーを禁止する
	Time(const Time& other) = delete;
	Time operator=(Time other) = delete;

	/// <summary>
	/// [EN] Get delta time in seconds [JP] 秒単位のdeltatime
	/// </summary>
	/// <returns>[EN] delta time [JP] デルタタイム</returns>
	static float DeltaTime() { return *deltaTimePtr; }

	/// <summary>
	/// [EN] Get fixed delta time in seconds [JP] 固定更新用のデルタタイム
	/// </summary>
	/// <returns>[EN] fixed delta time [JP] 固定更新用のデルタタイム</returns>
	static float FixedDeltaTime() { return *fixedDeltaTimePtr; }

	/// <summary>
	/// [EN] Get current fps [JP] 現在のFPSを取得
	/// </summary>
	/// <returns>[EN] Current FPS [JP] 現在のFPS</returns>
	static float FPS() { return *fpsPtr; }

	/// <summary>
	/// [EN] Register the FPS and deltaTime calculated by the FrameCounter side
	/// [JP] FrameCounter側で計算したFPSやDeltaTime等を登録する
	/// </summary>
	/// <param name="dt">deltaTime</param>
	/// <param name="fixDt">fixedDeltaTime</param>
	/// <param name="fps">FPS</param>
	static void Bind(const float* dt, const float* fixDt, const float* fps)
	{
		deltaTimePtr = dt;
		fixedDeltaTimePtr = fixDt;
		fpsPtr = fps;
	}

	
private:
	static const float* deltaTimePtr;
	static const float* fixedDeltaTimePtr;
	static const float* fpsPtr;

};
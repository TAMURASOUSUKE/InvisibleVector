#pragma once

class FrameController;

/// <summary>
/// [EN] Manage of delta time and fixed delta time. [JP] DeltaTimeとFixedDeltaTimeを管理する
/// </summary>
class TimeManager
{
public:
	/// <summary>
	/// [EN] Prohibited of instance [JP] インスタンスを作ることを禁止する
	/// </summary>
	TimeManager() = delete;

	// [EN] Prohibited copy of this class. [JP] このクラスのコピーを禁止する
	TimeManager(const TimeManager& other) = delete;
	TimeManager operator=(TimeManager other) = delete;

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
	/// [EN] A function that returns the number of steps remaining in the physics fixed time
	/// [JP] 物理固定時間が残り何ステップかを返す関数
	/// </summary>
	/// <returns>[EN] How many steps left [JP] 残り何ステップか</returns>
	static float Alpha() { return *alphaPtr; }

private:
	/// <summary>
/// [EN] Register the FPS and deltaTime calculated by the FrameCounter side
/// [JP] FrameCounter側で計算したFPSやDeltaTime等を登録する
/// </summary>
/// <param name="dt">deltaTime</param>
/// <param name="fixDt">fixedDeltaTime</param>
/// <param name="fps">FPS</param>
/// <param name="alpha">[EN] Linear Interpolation [JP] 線形補完</param>
	static void Bind(const float* dt, const float* fixDt, const float* fps, const float* alpha)
	{
		deltaTimePtr = dt;
		fixedDeltaTimePtr = fixDt;
		fpsPtr = fps;
		alphaPtr = alpha;
	}

	
private:
	static const float* deltaTimePtr;
	static const float* fixedDeltaTimePtr;
	static const float* fpsPtr;
	static const float* alphaPtr;

	friend class FrameController;

};
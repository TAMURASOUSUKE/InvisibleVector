#pragma once
#include "../Math/Vector_Dxlib.h"

/// <summary>
/// [EN] The base class for all objects [JP] すべてのオブジェクトの基底クラスとなる
/// </summary>
class ObjectBase
{
public:
	ObjectBase() = default;
	~ObjectBase() = default;


	/// <summary>
	/// [EN] Virtual function. Update every frame. [JP] 毎フレームの更新を行う純粋仮想関数
	/// </summary>
	/// <param name="deltaTime">[EN] delta times [JP] デルタタイム</param>
	virtual void Update(float deltaTime) = 0;

	/// <summary>
	/// [EN] Physics update functions [JP] 物理更新用関数
	/// </summary>
	/// /// <param name="deltaTime">[EN] fixed delta times [JP] 物理用の更新時間</param>
	virtual void FixedUpdate(float fixedDeltaTime) {}

	/// <summary>
	/// [EN] Virtual function. Do the drawing. [JP] 描画を行う純粋仮想関数
	/// </summary>
	virtual void Draw() = 0;


protected:
	// [EN] Active flag. [JP] 生存フラグ
	bool isActive{ true };

};
#pragma once
#include "Vector_Dxlib.h"
#include "ObjectBase.h"


class WorldObjectBase : public ObjectBase
{
public:
	/// <summary>
	/// [EN] Default constructor [JP] デフォルトコンストラクタ
	/// </summary>
	WorldObjectBase() = default;
	/// <summary>
	/// [EN] Default destructor [JP] デフォルトデストラクタ
	/// </summary>
	virtual ~WorldObjectBase() = default;

	/// <summary>
	/// [EN] Initialize position constructor [JP] 位置を初期化するコンストラクタ
	/// </summary>
	/// <param name="position">[EN] Initialize position [JP 初期化したい位置</param>
	WorldObjectBase(const Vector3& position) : position{position} {}
	
	/// <summary>
	/// Physical Update. [JP] 物理的な更新を行う
	/// </summary>
	/// <param name="deltaTime">[EN] delta time [JP] 毎フレームごとに計算されるdelta time</param>
	virtual void UpdatePhysics(float deltaTime);

	/// <summary>
	/// Get position [JP] 位置のGetter
	/// </summary>
	/// <returns>[EN] Now position [JP] 現在の位置</returns>
	[[nodiscard]] const Vector3& GetPosition() const noexcept { return position; }
	/// <summary>
	/// [EN] Get rotation [JP] 回転のGetter
	/// </summary>
	/// <returns>[EN] Now rotation [JP] 現在の回転状況</returns>
	[[nodiscard]] const Vector3& GetRotation() const noexcept { return rotation; }
	/// <summary>
	/// [EN] Get scale [JP] 大きさ(スケール)のGetter
	/// </summary>
	/// <returns>[EN] Now scale [JP] 現在の大きさ(スケール)</returns>
	[[nodiscard]] const Vector3& GetScale() const noexcept { return scale; }

protected:
	//--------------------------[EN] Required variable [JP] 必須変数-----------------------------------
	Vector3 position{ Vector3::Zero() };
	Vector3 rotation{ Vector3::Zero() };
	Vector3 scale{ 1.0f, 1.0f, 1.0f };

	//--------------------------[EN] Only move objects [JP] 動くオブジェクトのみ必要な変数-------------
	Vector3 velocity{ Vector3::Zero() };
	Vector3 acceleration{ Vector3::Zero() };

};
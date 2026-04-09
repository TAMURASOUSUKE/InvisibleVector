#pragma once
#include "Base/WorldObjectBase.h"
#include "Vector_Dxlib.h"
#include "Collider.h"
#include "Physics.h"

// デバッグ用のオブジェクトを実装する

class DebugPhysicsObject : public WorldObjectBase
{
public :
	// コンストラクタ群
	DebugPhysicsObject();
	DebugPhysicsObject(const Vector3& position, const Vector3& rotation, const Vector3& scale, const bool& isActive); // 位置、回転、スケールを、生存フラグを初期化
	// デストラクタ
	virtual ~DebugPhysicsObject() = default;

	void Update() override; // 通常更新関数
	void Draw() override; // 描画関数

private:
	void OnHit(HitResult& result); // 受け取り用関数

private:
	CapsuleCollider collider{};
	PhysicsBody body; // 物理挙動を補助するメンバ
};
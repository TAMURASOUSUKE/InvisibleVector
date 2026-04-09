#pragma once
#include "Base/WorldObjectBase.h"
#include "Vector_Dxlib.h"
#include "Collider.h"

class DebugGround : public WorldObjectBase
{
public:
	// コンストラクタ群
	DebugGround() = default;
	DebugGround(const Vector3& position, const Vector3& rotation, const Vector3& scale, const bool& isActive, const Vector3& boxSize);
	DebugGround(const Vector3& position, const Vector3& rotation, const Vector3& scale, const bool& isActive);
	// デストラクタ
	virtual ~DebugGround() = default;

	void Update() override;
	void Draw() override;

private:
	BoxCollider collider;
	Vector3 boxSize{ 300.0f, 20.0f, 300.0f };
};
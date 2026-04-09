#include "Dxlib.h"
#include "../CollisionManagement/CollisionManager.h"
#include "DebugGround.h"

DebugGround::DebugGround(const Vector3& position, const Vector3& rotation, const Vector3& scale, const bool& isActive, const Vector3& boxSize) : WorldObjectBase(position, rotation, scale, isActive), boxSize{boxSize}
{

}

DebugGround::DebugGround(const Vector3& position, const Vector3& rotation, const Vector3& scale, const bool& isActive) : WorldObjectBase(position, rotation, scale, isActive)
{

}



void DebugGround::Update()
{
	collider.pos = position;
	collider.size = boxSize;
	collider.rotate = rotation;
	collider.tag = CollisionTag::Ground;
	collider.ReciveFunc = nullptr; // コールバックの必要がないためnullptr

	CollisionManager::Instance().Register(collider.tag, &collider);
}

void DebugGround::Draw()
{
	DrawCube3D(ToDxLib(position), ToDxLib(position + boxSize), GetColor(255, 255, 255), GetColor(255, 255, 255), true); // モデル
	DrawCube3D(ToDxLib(collider.pos), ToDxLib(collider.pos + collider.size), GetColor(255, 0, 0), GetColor(255, 0, 0), false); // コライダー
}
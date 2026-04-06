#include "Dxlib.h"
#include "DebugPhysicsObject.h"
#include "../CollisionManagement/CollisionManager.h"

DebugPhysicsObject::DebugPhysicsObject(const Vector3& position, const Vector3& rotation, const Vector3& scale, const bool& isActive) : WorldObjectBase(position, rotation, scale, isActive)
{
	acceleration.y = -9.8f; // 重力値
}


void DebugPhysicsObject::Update()
{
	collider.startPos = position;
	collider.endPos = Vector3(position.x, position.y + 50.0f, position.z);
	collider.rotate = rotation;
	collider.radius =  10.0f;
	collider.tag = CollisionTag::Player;
    collider.ReciveFunc = std::bind(&DebugPhysicsObject::OnHit, this, std::placeholders::_1); // コールバック

	// CollisionManagerに登録
	CollisionManager::Instance().Register(collider.tag, &collider);
}


void DebugPhysicsObject::Draw()
{
	DrawCapsule3D(ToDxLib(position), ToDxLib(Vector3(position.x, position.y + 50.0f, position.z)), 10.0f, 32, GetColor(255, 255, 255), GetColor(255, 255, 255), true); // テストモデル
	DrawCapsule3D(ToDxLib(collider.startPos), ToDxLib(collider.endPos), collider.radius, 32, GetColor(255, 0, 0), GetColor(255, 0, 0), false); // コライダー
}

void DebugPhysicsObject::OnHit(HitResult& result)
{
	position += result.pushVec;
}
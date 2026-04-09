#include "Dxlib.h"
#include "DebugPhysicsObject.h"
#include "../CollisionManagement/CollisionManager.h"
#include "../PhysicsManagement/PhysicsManager.h"

DebugPhysicsObject::DebugPhysicsObject() : WorldObjectBase(position, rotation, scale, isActive)
{
	body.position = &this->position;
	body.velocity = &this->velocity;
	body.acceleration = &this->acceleration;


	// 物理挙動管理クラスに登録
	PhysicsManager::Instance().Register(&body);
}

DebugPhysicsObject::DebugPhysicsObject(const Vector3& position, const Vector3& rotation, const Vector3& scale, const bool& isActive) : WorldObjectBase(position, rotation, scale, isActive)
{
	body.position = &this->position;
	body.velocity = &this->velocity;
	body.acceleration = &this->acceleration;


	velocity.z = 800.0f;

	// 物理挙動管理クラスに登録
	PhysicsManager::Instance().Register(&body);
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
	Vector3 endPos{ position.x, position.y + 50.0f, position.z };
	DrawCapsule3D(ToDxLib(position), ToDxLib(Vector3(position.x, position.y + 50.0f, position.z)), 10.0f, 32, GetColor(255, 255, 255), GetColor(255, 255, 255), true); // テストモデル
	DrawCapsule3D(ToDxLib(position), ToDxLib(endPos), collider.radius, 32, GetColor(255, 0, 0), GetColor(255, 0, 0), false); // コライダー

#ifdef _DEBUG
	int y{ 40 };
	unsigned int white{ GetColor(255, 255, 255) };
	unsigned int green{ GetColor(100, 255, 100) };
	DrawFormatString(10, y, white, "Pos: (%.4f, %.4f, %.4f)", position.x, position.y, position.z); y += 18;
	DrawFormatString(10, y, white, "Vel: (%.4f, %.4f, %.4f)", velocity.x, velocity.y, velocity.z); y += 18;
	DrawFormatString(10, y, white, "Acc: (%.4f, %.4f, %.4f)", acceleration.x, acceleration.y, acceleration.z); y += 18;
	DrawFormatString(10, y, body.isGround ? green : white, "isGround: %s", body.isGround ? "TRUE" : "FALSE"); y += 18;
	DrawFormatString(10, y, white, "LenSqr: %.6f", velocity.LengthNoSqr());
#endif
}

void DebugPhysicsObject::OnHit(HitResult& result)
{
	PhysicsManager::Instance().ResolveCollision(body, result.pushVec);
}
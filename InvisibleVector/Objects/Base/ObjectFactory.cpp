#include <memory>
#include "GameConstant.h"
#include "ObjectFactory.h"

// 具体をインクルード
#include "../DebugGround.h"
#include "../DebugPhysicsObject.h"

// static変数の実態を作る
ObjectManager* ObjectFactory::targetManager{ nullptr };


void ObjectFactory::BindManager(ObjectManager& _targetManager)
{
	targetManager = &_targetManager; // 引数で受け取ったObjectManagerのインスタンスをFactoryに登録
}

void ObjectFactory::CreateDebugObject(const Vector3& position, const Vector3& rotation, const Vector3& scale, const bool& isActive)
{
	std::unique_ptr<WorldObjectBase> obj{ std::make_unique<DebugPhysicsObject>(position, rotation, scale, isActive) };
	targetManager->RegisterWorld(std::move(obj));
}

void ObjectFactory::CreateDebugGround(const Vector3& position, const Vector3& rotation, const Vector3& scale, const bool& isActive)
{
	std::unique_ptr<WorldObjectBase> obj{ std::make_unique<DebugGround>(position, rotation, scale, isActive) };
	targetManager->RegisterWorld(std::move(obj));
}

void ObjectFactory::CreateDebugGround(const Vector3& position, const Vector3& rotation, const Vector3& scale, const bool& isActive, const Vector3& boxSize)
{
	std::unique_ptr<WorldObjectBase> obj{ std::make_unique<DebugGround>(position, rotation, scale, isActive, boxSize) };
	targetManager->RegisterWorld(std::move(obj));
}



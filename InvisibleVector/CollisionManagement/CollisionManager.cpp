#include "CollisionManager.h"


// 球と球のあたり判定
bool CollisionManager::IsOverlapping(const SphereCollider& collider01, const SphereCollider& collider02)
{
	Vector3 vec{ collider01.pos - collider02.pos }; // 2点間のベクトルを作る

	// 直線の長さと2つの半径の和を比較する
	if (((collider01.radius * collider01.radius) + (collider02.radius * collider02.radius)) >= vec.LengthNoSqr())
	{
		return true; // 重なっている
	}

	return false; // 重なっていない
}

// 更新
void CollisionManager::Update()
{

}

void CollisionManager::DecideCollisionCombination(ColliderBase* collider01, ColliderBase* collider02)
{
	if (collider01 == nullptr || collider02 == nullptr) return;

	// 型ごとに仕分け
	std::unordered_map<ColliderType, std::vector<ColliderBase*>> colliders{};
	colliders[collider01->GetType()].push_back(collider01);
	colliders[collider02->GetType()].push_back(collider02);

	// 球と球
	if (colliders.count(ColliderType::Sphere) > 0 && colliders.size() == 2)
	{
		ProcessSphereSphereCollision(colliders[ColliderType::Sphere][0], colliders[ColliderType::Sphere][1]);
	}
}

// 作り直しと判定の実行
void CollisionManager::ProcessSphereSphereCollision(ColliderBase* sphere01, ColliderBase* sphere02)
{
	if (sphere01->GetType() != ColliderType::Sphere || sphere02->GetType() != ColliderType::Sphere) return;

	// 一時的な判定の入れ物
	SphereCollider rebuildSphere01{};
	SphereCollider rebuildSphere02{};

	// 抽出して3D図形として復元
	if (!rebuilder.Rebuild(sphere01, rebuildSphere01) || !rebuilder.Rebuild(sphere02, rebuildSphere02))
	{
		return; // 復元失敗
	}

	// 実際のあたり判定ロジックを実行
	if (IsOverlapping(rebuildSphere01, rebuildSphere02))
	{
		// 当たっていたらお互いに通知
		NotifyResults(*sphere01, *sphere02);
		NotifyResults(*sphere02, *sphere01);

	}
}

void CollisionManager::NotifyResults(ColliderBase& from, ColliderBase& to)
{
	if (to.ReciveFunc != nullptr)
	{
		HitResult result{from.tag, from.subTag};
		to.ReciveFunc(result);
	}
}

// 登録関数
void CollisionManager::Register(CollisionTag tag, ColliderBase* collider)
{
	if (collider != nullptr)
	{
		colliderByTag[tag].push_back(collider);
	}
}
#pragma once
#include <vector>
#include <unordered_map>
#include "Collider.h"
#include "ColliderRebuilder.h"

class CollisionManager
{
public:
	// インスタンスのゲッター
	static CollisionManager& Instance()
	{
		static CollisionManager instance;
		return instance;
	}

	// あたり判定ロジック
	static bool IsOverlapping(const SphereCollider& collider01, const SphereCollider& collider02); // 球と球

	// 判定の登録
	void Register(CollisionTag tag, ColliderBase* collider);

	// 毎フレーム呼ばれる更新関数
	void Update();

private:
	CollisionManager() = default;
	// コピーと代入の禁止
	CollisionManager(const CollisionManager&) = delete;
	CollisionManager& operator=(const CollisionManager&) = delete;

	// 組み合わせ決定
	void DecideCollisionCombination(ColliderBase* collider01, ColliderBase* collider02);

	// 判定を実行
	void ProcessSphereSphereCollision(ColliderBase* sphere01, ColliderBase* sphere02);

	// 結果通知
	void NotifyResults(ColliderBase& from, ColliderBase& to);

private:
	// CollisionTag別にまとめたColliderBaseの配列
	std::unordered_map<CollisionTag&, std::vector<ColliderBase*>> colliderByTag{};
	ColliderRebuilder rebuilder{};
};
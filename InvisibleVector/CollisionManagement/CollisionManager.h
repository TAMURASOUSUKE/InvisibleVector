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
	static bool IsOverlapping(const SphereCollider& collider01, const SphereCollider& collider02, Vector3& outPushVec); // 球と球
	static bool IsOverlapping(const SphereCollider& sphere, const BoxCollider& box, Vector3& pushVec); // 球と箱
	static bool IsOverlapping(const SphereCollider& sphere, const CapsuleCollider& capsule, Vector3& pushVec); // 球とカプセル
	static bool IsOverlapping(const BoxCollider& box01, const BoxCollider& box02, Vector3& pushVec); // 箱と箱
	static bool IsOverlappingOBB(const SphereCollider& sphere, const BoxCollider& box, Vector3& pushVec); // 回転箱と球
	static bool IsOverlappingOBB(const BoxCollider& box01, const BoxCollider& box02, Vector3& pushVec); // 回転箱と回転箱

	// 判定の登録
	void Register(CollisionTag tag, ColliderBase* collider);

	// 毎フレーム呼ばれる更新関数
	void Update();

	void Clear(); // 破棄したいときにこれを呼ぶ

private:
	CollisionManager() = default;
	// コピーと代入の禁止
	CollisionManager(const CollisionManager&) = delete;
	CollisionManager& operator=(const CollisionManager&) = delete;

	// Y -> X -> Zの順で回転させる
	static Vector3 RotateVector(const Vector3& vec, const Vector3& rot);

	// 分離軸テスト用ヘルパー関数
	static bool TestSeparatingAxis(
		const Vector3& axis,                  // テストする分離軸
		const Vector3& centerToCenter,        // 箱1から箱2へのベクトル
		const Vector3& aAxisX, const Vector3& aAxisY, const Vector3& aAxisZ, const Vector3& aHalfSize, // 箱1の情報
		const Vector3& bAxisX, const Vector3& bAxisY, const Vector3& bAxisZ, const Vector3& bHalfSize,  // 箱2の情報
		float& outPenetration
	);

	// 組み合わせ決定
	void DecideCollisionCombination(ColliderBase* collider01, ColliderBase* collider02);

	// 判定を実行
	void ProcessSphereSphereCollision(ColliderBase* sphere01, ColliderBase* sphere02);
	void ProcessSphereBoxCollision(ColliderBase* sphere, ColliderBase* box);
	void ProcessBoxBoxCollision(ColliderBase* box01, ColliderBase* box02);
	void ProcessSphereCapsuleCollision(ColliderBase* sphere, ColliderBase* capsule);

	// 結果通知
	void NotifyResults(ColliderBase& from, ColliderBase& to, Vector3 pushVec);

private:
	// CollisionTag別にまとめたColliderBaseの配列
	std::unordered_map<CollisionTag, std::vector<ColliderBase*>> colliderByTag{};
	ColliderRebuilder rebuilder{};
};
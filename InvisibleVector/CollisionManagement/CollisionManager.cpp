#include <algorithm>
#include "CollisionManager.h"


// 球と球のあたり判定
bool CollisionManager::IsOverlapping(const SphereCollider& collider01, const SphereCollider& collider02, Vector3& outPushVec)
{
	Vector3 vec{ collider01.pos - collider02.pos }; // 2点間のベクトルを作る

	// 二点間のベクトルの長さ
	float distance{ vec.Length() };
	float radiusSum{ collider01.radius + collider02.radius }; // 二つの円の半径の合計

	// めり込みチェック
	if (distance < radiusSum)
	{
		// めり込み量
		float penetration{ radiusSum - distance };

		// 押し戻す方向
		Vector3 normal;
		if (distance == 0.0f)
		{
			// 座標が完全に同じ場合は上に押し戻す
			normal = Vector3{ 0.0f, 1.0f, 0.0f };
		}
		else
		{
			vec.Normalize();
			normal = vec; // 押し戻す方向を決定
		}
		
		outPushVec = normal * penetration; // 押し出す方向 * めり込み量
		return true;

	}

	return false; // 当たっていない

}

// 球と箱(AABB)
bool CollisionManager::IsOverlapping(const SphereCollider& sphere, const BoxCollider& box, Vector3 pushVec)
{
	// AABBように最大と最小座標を取得
	Vector3 minPos{ box.GetMinPos() };
	Vector3 maxPos{ box.GetMaxPos() };

	// 球の中心座標を箱の範囲内にclampし最近点を求める
	Vector3 closestPoint
	{
		std::clamp(sphere.pos.x, minPos.x, maxPos.x),
		std::clamp(sphere.pos.y, minPos.y, maxPos.y),
		std::clamp(sphere.pos.z, minPos.z, maxPos.z),
	};

	// 最近点から球の中心へ向かうベクトルを作成する
	Vector3 vec{ sphere.pos - closestPoint };
	float distance{ vec.Length() };

	// 距離が半径より小さいか
	if (distance < sphere.radius)
	{
		// めり込み量
		float penetration{ sphere.radius - distance };

		// 押し戻す方向
		Vector3 normal;
		if (distance == 0.0f)
		{
			/*
				球の中心が箱の中に完全に押し込まれている場合は
				簡易的に中心から外側に向けて押し出す
			*/

			Vector3 centerToSphere{ sphere.pos - box.GetCenter() }; // 箱の中心から球の座標へ向けてのべkる取る
			if (centerToSphere.Length() == 0.0f)
			{
				normal = Vector3{ 0.0f, 1.0f, 0.0f }; // 完全に中心が同じなら真上へ
			}
			else
			{
				centerToSphere.Normalize();
				normal = centerToSphere; // 正規化したベクトルを押し戻す方向として決定する
			}
		}
		else
		{
			vec.Normalize();
			normal = vec;
		}

		pushVec = normal * penetration;
		return true; // 当たっている
	}

	return false; // 当たっていない
}

// 更新
void CollisionManager::Update()
{
	// 判定したいタグを組み合わせる
	std::vector<std::pair<CollisionTag, CollisionTag>> combinationList
	{
		{CollisionTag::Player, CollisionTag::Enemy},
		{CollisionTag::Player, CollisionTag::Ground},
		{CollisionTag::Enemy, CollisionTag::Ground},
	};

	// 総当たり
	for (auto& pair : combinationList)
	{
		std::vector<ColliderBase*> colliders01{ colliderByTag[pair.first] };
		std::vector<ColliderBase*> colliders02{ colliderByTag[pair.second] };

		for (auto& collider01 : colliders01)
		{
			for (auto& collider02 : colliders02)
			{
				DecideCollisionCombination(collider01, collider02);
			}
		}
	}

	colliderByTag.clear(); // 1フレーム分の判定が終わったのでクリア
}

void CollisionManager::DecideCollisionCombination(ColliderBase* collider01, ColliderBase* collider02)
{
	if (collider01 == nullptr || collider02 == nullptr) return;

	// 型ごとに仕分け
	std::unordered_map<ColliderType, std::vector<ColliderBase*>> colliders{};
	colliders[collider01->GetType()].push_back(collider01);
	colliders[collider02->GetType()].push_back(collider02);

	// 球と球
	if (colliders.count(ColliderType::Sphere) > 0 && colliders[ColliderType::Sphere].size() == 2)
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

	// 押し戻しを受け取る変数
	Vector3 pushVec{};

	// 実際のあたり判定ロジックを実行
	if (IsOverlapping(rebuildSphere01, rebuildSphere02, pushVec))
	{
		// 01はpushVecの方向へ押し出す
		NotifyResults(*sphere02, *sphere01, pushVec);
		// 02は逆の方向へ押し出す
		NotifyResults(*sphere01, *sphere02, -pushVec);

	}
}

// 球と箱の作り直しと判定の実行
void CollisionManager::ProcessSphereBoxCollision(ColliderBase* sphere, ColliderBase* box)
{

}

void CollisionManager::NotifyResults(ColliderBase& from, ColliderBase& to, Vector3 pushVec)
{
	if (to.ReciveFunc != nullptr)
	{
		HitResult result{from.tag, from.subTag, pushVec};
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
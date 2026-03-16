#include <algorithm>
#include <cmath>
#include "DxLib.h"
#include "SystemConstant.h"
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
bool CollisionManager::IsOverlapping(const SphereCollider& sphere, const BoxCollider& box, Vector3& pushVec)
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

bool CollisionManager::IsOverlapping(const BoxCollider& box01, const BoxCollider& box02, Vector3& pushVec)
{
	// それぞれの最大座標と最小座標を取得
	Vector3 min01{ box01.GetMinPos() };
	Vector3 max01{ box01.GetMaxPos() };
	Vector3 min02{ box02.GetMinPos() };
	Vector3 max02{ box02.GetMaxPos() };

	// 判定を行う どれか一つでも重なっていなければ当たっていない
	if (max01.x < min02.x || min01.x > max02.x) return false;
	if (max01.y < min02.y || min01.y > max02.y) return false;
	if (max01.z < min02.z || min01.z > max02.z) return false;

	// 押し出しベクトルの計算
	float overlapX1{ max01.x - min02.x }; // Box01が左に押し出される量
	float overlapX2{ max02.x - min01.x }; // Box01が右に押し出される量
	float overlapY1{ max01.y - min02.y }; // Box01が下に押し出される量
	float overlapY2{ max02.y - min01.y }; // Box01が上に押し出される量
	float overlapZ1{ max01.z - min02.z }; // Box01が奥に押し出される量
	float overlapZ2{ max02.z - min01.z }; // Box01が手前に押し出される量

	// めり込みが浅い方向を探す
	float minOverlap{ overlapX1 };
	Vector3 normal{ -1.0f, 0.0f, 0.0f }; // 初期は左

	if (overlapX2 < minOverlap) { minOverlap = overlapX2; normal = Vector3(1.0f, 0.0f, 0.0f); }
	if (overlapY1 < minOverlap) { minOverlap = overlapY1; normal = Vector3(0.0f, -1.0f, 0.0f); }
	if (overlapY2 < minOverlap) { minOverlap = overlapY2; normal = Vector3(0.0f, 1.0f, 0.0f); }
	if (overlapZ1 < minOverlap) { minOverlap = overlapZ1; normal = Vector3(1.0f, 0.0f, -1.0f); }
	if (overlapZ2 < minOverlap) { minOverlap = overlapZ2; normal = Vector3(1.0f, 0.0f, 1.0f); }

	// 押し戻しベクトルを決定する
	pushVec = normal * minOverlap;

	return true;
}

bool CollisionManager::IsOverlappingOBB(const SphereCollider& sphere, const BoxCollider& box, Vector3& pushVec)
{
	// 中心を基準に計算するようにするためのキャッシュ
	Vector3 center{ box.GetCenter() };
	Vector3 halfSize{ box.GetHalfSize() };

	// 3つのローカル軸を回転させ生成する
	Vector3 localAxisX{ RotateVector(Vector3(1.0f, 0.0f, 0.0f), box.rotate) };
	Vector3 localAxisY{ RotateVector(Vector3(0.0f, 1.0f, 0.0f), box.rotate) };
	Vector3 localAxisZ{ RotateVector(Vector3(0.0f, 0.0f, 1.0f), box.rotate) };

	// OBBの中心から球のベクトル
	Vector3 centerToSphere{ sphere.pos - center };

	// 内積を使い球の中心がローカル軸のどの位置にあるのかを求める
	// 球の位置座標をローカル軸へ閉じ込めるイメージ
	float distX{ Vector3::Dot(centerToSphere, localAxisX) };
	float distY{ Vector3::Dot(centerToSphere, localAxisY) };
	float distZ{ Vector3::Dot(centerToSphere, localAxisZ) };

	// ハーフサイズでclampをかけてOBB上の最も近い点のローカル座標を出す
	float clampX{ std::clamp(distX, -halfSize.x, halfSize.x) };
	float clampY{ std::clamp(distY, -halfSize.y, halfSize.y) };
	float clampZ{ std::clamp(distZ, -halfSize.z, halfSize.z) };

	// ローカル距離をワールド座標の最近点に復元する
	Vector3 closestPoint
	{	center +
		(localAxisX * clampX) +
		(localAxisY * clampY) +
		(localAxisZ * clampZ)
	};

	// 最近点から球の中心へのベクトルを作り距離を出す
	Vector3 vec{ sphere.pos - closestPoint };
	float distance{ vec.Length() };

	// 距離が半径より近ければ当たっているとする
	if (distance < sphere.radius)
	{
		float penetration{ sphere.radius - distance }; // 押し戻し量
		Vector3 normal; // 法線

		// 球と中心が完全に一致した場合は真上(ローカル座標)に戻す
		if (distance == 0.0f)
		{
			normal = localAxisY;
		}
		else
		{
			vec.Normalize();
			normal = vec;
		}

		pushVec = normal * penetration; // 押し戻す方向 * 押し戻す量
		return true;
	}

	return false;


}

bool CollisionManager::IsOverlappingOBB(const BoxCollider& box01, const BoxCollider& box02, Vector3& pushVec)
{
	// 各箱の中心座標とハーフサイズを取得
	Vector3 center01{ box01.GetCenter() };
	Vector3 halfSize01{ box01.GetHalfSize() };
	Vector3 center02{ box02.GetCenter() };
	Vector3 halfSize02{ box02.GetHalfSize() };

	// 各軸のローカル軸を生成する
	Vector3 aX{ RotateVector(Vector3(1.0f, 0.0f, 0.0f), box01.rotate) };
	Vector3 aY{ RotateVector(Vector3(0.0f, 1.0f, 0.0f), box01.rotate) };
	Vector3 aZ{ RotateVector(Vector3(0.0f, 0.0f, 1.0f), box01.rotate) };

	Vector3 bX{ RotateVector(Vector3(1.0f, 0.0f, 0.0f), box02.rotate) };
	Vector3 bY{ RotateVector(Vector3(0.0f, 1.0f, 0.0f), box02.rotate) };
	Vector3 bZ{ RotateVector(Vector3(0.0f, 0.0f, 1.0f), box02.rotate) };

	// 箱から箱への中心間ベクトル
	Vector3 centerToCenter{ center02 - center01 };

	// 分離軸テスト
	Vector3 centerToCenter{ center02 - center01 };

	// 15本の分離軸を配列にまとめる
	Vector3 axes[15]
	{
		aX, aY, aZ, // A箱の軸
		bX, bY, bZ, // B箱の軸

		// 各軸の外積
		Vector3::Cross(aX, bX), Vector3::Cross(aX, bY), Vector3::Cross(aX, bZ),
		Vector3::Cross(aY, bX), Vector3::Cross(aY, bY), Vector3::Cross(aY, bZ),
		Vector3::Cross(aZ, bX), Vector3::Cross(aZ, bY), Vector3::Cross(aZ, bZ),
	};

	// 最小のめりこみを記録する用の変数
	float minPenetration{ FLT_MAX };
	Vector3 minAxis{}; // 押し出す方向

	// 15本の軸をループでテストする
	for (int i = 0; i < 15; i++)
	{
		float penetration{ 0.0f }; // この軸でのめり込み量を受け取る

		// 一つでも隙間があるなら当たっていないとする
		if (!TestSeparatingAxis(axes[i], centerToCenter, aX, aY, aZ, halfSize01, bX, bY, bZ, halfSize02, penetration))
		{
			return false;
		}

		// もし当たっていたら今までで一番めり込みが浅いかをチェックする
		if (penetration < minPenetration)
		{
			minPenetration = penetration; // 最小めり込み量を更新
			minAxis = axes[i]; // その時の方向を保持
		}
	}

	// 押し出す向きの確認
	// 02->01へ向いている時は反転させる
	if (Vector3::Dot(minAxis, centerToCenter) < 0.0f)
	{
		minAxis = -minAxis;
	}

	// 軸を正規化する
	minAxis.Normalize();

	// 押し戻しベクトルの計算
	pushVec = minAxis * minPenetration;

	return true;
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
	// 球と箱
	else if (colliders.count(ColliderType::Box) > 0 && colliders[ColliderType::Sphere].size() == 1 && colliders[ColliderType::Box].size() == 1)
	{
		ProcessSphereBoxCollision(colliders[ColliderType::Sphere][0], colliders[ColliderType::Box][0]);
	}
	// 箱と箱
	else if (colliders.count(ColliderType::Box) > 0 && colliders[ColliderType::Box].size() == 2)
	{
		ProcessBoxBoxCollision(colliders[ColliderType::Box][0], colliders[ColliderType::Box][1]);
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
	// 一時的な判定の入れ物
	SphereCollider rebuildSphere{};
	BoxCollider rebuildBox{};

	// 抽出して3D図形として復元
	if (!rebuilder.Rebuild(sphere, rebuildSphere) || !rebuilder.Rebuild(box, rebuildBox))
	{
		return; // 復元失敗とする
	}

	Vector3 pushVec{};

	// 回転していな場合の処理
	/*
		回転している時としていない時では計算量に雲泥の差があるので分ける
	*/
	if (rebuildBox.rotate == Vector3::Zero())
	{
		// 回転していないと判断しAABBを実行する
		if (IsOverlapping(rebuildSphere, rebuildBox, pushVec))
		{
			// 球はpushVec方向へ
			NotifyResults(*box, *sphere, pushVec);
			// 箱は-pushVec方向へ
			NotifyResults(*sphere, *box, -pushVec);
		}
	}
	else
	{
		// 回転しているとみなしOBB処理
		if (IsOverlappingOBB(rebuildSphere, rebuildBox, pushVec))
		{
			// 球はpushVec方向へ
			NotifyResults(*box, *sphere, pushVec);
			// 箱は-pushVec方向へ
			NotifyResults(*sphere, *box, -pushVec);
		}
	}
}

void CollisionManager::ProcessBoxBoxCollision(ColliderBase* box01, ColliderBase* box02)
{
	// 一時的な入れ物
	BoxCollider rebuildBox01{};
	BoxCollider rebuildBox02{};

	// 抽出して3D図形として復元
	if (!rebuilder.Rebuild(box01, rebuildBox01) || !rebuilder.Rebuild(box02, rebuildBox02))
	{
		return; // 復元失敗
	}

	Vector3 pushVec{};

	// どちらも回転していない場合にのみAABBを行う
	if (rebuildBox01.rotate == Vector3::Zero() && rebuildBox02.rotate == Vector3::Zero())
	{
		// AABB
		if (IsOverlapping(rebuildBox01, rebuildBox02, pushVec))
		{
			NotifyResults(*box02, *box01, pushVec);
			NotifyResults(*box01, *box02, -pushVec);
		}
	}
	else
	{
		// OBB
		if (IsOverlappingOBB(rebuildBox01, rebuildBox02, pushVec))
		{
			NotifyResults(*box02, *box01, pushVec);
			NotifyResults(*box01, *box02, -pushVec);
		}

	}
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


// 回転補助関数
Vector3 CollisionManager::RotateVector(const Vector3& vec, const Vector3& rot)
{
	Vector3 result{ vec };
	// 回転を保存
	float cx{ std::cos(rot.x) }, sx{ std::sin(rot.x) };
	float cy{ std::cos(rot.y) }, sy{ std::sin(rot.y) };
	float cz{ std::cos(rot.z) }, sz{ std::sin(rot.z) };

	// Y軸回転
	float tmpX{ result.x * cy + result.z * sy };
	float tmpZ{ -result.x * sy + result.z * cy };
	result.x = tmpX;
	result.z = tmpZ;

	// X軸回転
	float tmpY{ result.y * cx - result.z * sx };
	tmpZ = result.y * sx + result.z * cx ;
	result.y = tmpY;
	result.z = tmpZ;

	// Z軸回転
	tmpX = result.x * cz - result.y * sz;
	tmpY = result.x * sz + result.y * cz;
	result.x = tmpX;
	result.y = tmpY;

	return result;
}

 bool CollisionManager::TestSeparatingAxis(const Vector3& axis,
	const Vector3& centerToCenter,
	const Vector3& aAxisX, const Vector3& aAxisY, const Vector3& aAxisZ, const Vector3& aHalfSize,
	const Vector3& bAxisX, const Vector3& bAxisY, const Vector3& bAxisZ, const Vector3& bHalfSize,
	float& outPenetration)
{
	// ゼロベクトルのチェック
	if (axis.LengthNoSqr() < GAME_EPSILON<float>) return true;

	// 分離軸の正規化
	Vector3 L{ axis };
	L.Normalize();

	// 二つの箱の中心間の距離を分離軸Lに投影する
	float distance{ std::abs(Vector3::Dot(centerToCenter, L)) };

	// 箱Aの影の長さを計算する
	float dotX{ Vector3::Dot(aAxisX, L) };
	float dotY{ Vector3::Dot(aAxisY, L) };
	float dotZ{ Vector3::Dot(aAxisZ, L) };
	float rA{ std::abs(dotX) * aHalfSize.x + std::abs(dotY) * aHalfSize.y + std::abs(dotZ) * aHalfSize.z };

	dotX = Vector3::Dot(bAxisX, L);
	dotY = Vector3::Dot(bAxisY, L);
	dotZ = Vector3::Dot(bAxisZ, L);
	float rB{ std::abs(dotX) * bHalfSize.x + std::abs(dotY) * bHalfSize.y + std::abs(dotZ) * bHalfSize.z };

	// distanceと各箱の内積の結果を合わせたものを比較し結果を出す
	float totalLength{ rA + rB };

	// めり込み量の計算
	float penetration{totalLength - distance};

	if (penetration <= 0.0f)
	{
		return false; // 中心間の距離が影の合計の長さよりも大きければ当たっていない
	}

	outPenetration = penetration;

	// ここまでくれば当たっているとする
	return true;
}
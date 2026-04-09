#define NOMINMAX // Windowsのmin,maxマクロに邪魔されないようにする
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

bool CollisionManager::IsOverlapping(const SphereCollider& sphere, const CapsuleCollider& capsule, Vector3& pushVec)
{
	Vector3 A{ capsule.startPos }; // 始点
	Vector3 B{ capsule.endPos }; // 終点
	Vector3 P{ sphere.pos }; // 球の中心座標

	Vector3 AB{ B - A }; // 高さ
	Vector3 AP{ P - A }; // 始点から球の中心座標へのベクトル

	// 内積を使って球の中心が線分ABのどのあたりにあるのかを出す
	float lenSp{ AB.LengthNoSqr() }; // 長さの二乗

	float t{ 0.0f };
	if (lenSp != 0.0f) // 0割り防止
	{
		t = Vector3::Dot(AP, AB);
	}

	t = std::clamp(t, 0.0f, 1.0f); // 割合を0.0f - 1.0fの値に収める

	// 最近点をを求める
	Vector3 closestPoint{ A + (AB * t) }; // 始点 + (線分の長さ * 割合)

	// 最近点と球の中心との判定
	Vector3 vec{ P - closestPoint }; // 最近点から球の中心座標へのベクトル
	float distance{ vec.Length() }; // 長さ
	float radiusSum{ sphere.radius + capsule.radius }; // 半径の合計

	if (radiusSum < distance)
	{
		// ここに入ると当たっていないと判断
		return false;
	}

	float penetration{ sphere.radius - distance }; // 押し出す量
	Vector3 normal{}; // 押し出す方向

	// 例外処理(球とカプセルが完全に重なっている場合)
	if (distance == 0.0f)
	{
		normal = Vector3(0.0f, 1.0f, 0.0f);
	}
	else
	{
		vec.Normalize();
		normal = vec;
	}

	pushVec = normal * penetration;
	return true;
}

bool CollisionManager::IsOverlapping(const CapsuleCollider& capsule01, const CapsuleCollider& capsule02, Vector3& pushVec)
{
	Vector3 closestPoint01{}; // 線分01の最接近点
	Vector3 closestPoint02{}; // 線分02の最接近点

	// 最接近点をそれぞれの線分で求める　　
	ClosestPointSegmentSegment(capsule01.startPos, capsule01.endPos, capsule02.startPos, capsule02.endPos, closestPoint01, closestPoint02);

	Vector3 distance{ closestPoint01 - closestPoint02 }; // 最接近点間のベクトル
	float distLen{ distance.Length() }; // 最接近点間の距離
	float totalRadius{ capsule01.radius + capsule02.radius }; // 半径の合計

	if (totalRadius < distLen)
	{
		return false; // 半径を足し合わせたものより二点間の距離が大きいのなら当たっていないとする
	}

	float penetration{ totalRadius - distLen }; // 押し出し量
	Vector3 normal{}; // 押し出す方向

	// 例外処理 : 完全に重なっている場合
	if (distLen == 0.0f)
	{
		// 上に押し出す
		normal = Vector3(0.0f, 1.0f, 0.0f);
	}
	else
	{
		distance.Normalize();
		normal = distance;
	}

	pushVec = normal * penetration;
	return true;
}

bool CollisionManager::IsOverlapping(const BoxCollider& box, const CapsuleCollider& capsule, Vector3& pushVec)
{
	Vector3 boxCenter{ box.GetCenter() }; // 箱の中心座標
	Vector3 boxMin{ box.GetMinPos() }; // 箱の最小座標
	Vector3 boxMax{ box.GetMaxPos() }; // 箱の最大座標

	// 箱の中心からカプセルの線分上で一番近い点を探す
	Vector3 p1{ ClosestPointOnSegment(capsule.startPos, capsule.endPos, box.GetCenter()) };

	// AABBのクランプを行いP1から箱の表面上で一番近い点を探す
	Vector3 q1
	{
		Vector3
		(
			std::clamp(p1.x, boxMin.x, boxMax.x),
			std::clamp(p1.y, boxMin.y, boxMax.y),
			std::clamp(p1.z, boxMin.z, boxMax.z)
		)
	};

	// q1(箱の表面)から再度線分に対して最近点を求める
	Vector3 p2{ ClosestPointOnSegment(capsule.startPos, capsule.endPos, q1) };

	Vector3 vec{ p2 - q1 };
	float len{ vec.Length() };

	if (len > capsule.radius)
	{
		return false; // 二点間の距離がカプセルの半径より大きかったら当たっていないとする
	}

	float penetration{ capsule.radius - len }; // めり込み量
	Vector3 normal{}; // 押し戻し方向

	if (len == 0.0f)
	{
		// 完全に埋まっている(芯が箱の中にある)際の対処
		Vector3 centerToCalsule{ p2 - boxCenter };
		if (centerToCalsule.LengthNoSqr() == 0.0f)
		{
			// 完全に中心が同じ場合
			normal = Vector3(0.0f, 1.0f, 0.0f);
		}
		else
		{
			centerToCalsule.Normalize();
			normal = centerToCalsule; // 入ってきた方向へ押し出す
		}
	}
	else
	{
		// 浅いめり込みの場合
		vec.Normalize();
		normal = vec;
	}

	pushVec = normal * penetration;
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
	{ center +
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

bool CollisionManager::IsOverlappingOBB(const BoxCollider& box, const CapsuleCollider& capsule, Vector3& pushVec)
{
	Vector3 center{ box.GetCenter() };
	Vector3 halfSize{ box.GetHalfSize() };

	// OBB用のローカル軸を作る
	Vector3 localAxisX{ RotateVector(Vector3(1.0f, 0.0f, 0.0f), box.rotate) };
	Vector3 localAxisY{ RotateVector(Vector3(0.0f, 1.0f, 0.0f), box.rotate) };
	Vector3 localAxisZ{ RotateVector(Vector3(0.0f, 0.0f, 1.0f), box.rotate) };

	// カプセルの線分をOBBのローカル空間に合わせる

	// 中心からのベクトルを作る
	Vector3 centerToStart{ capsule.startPos - center }; // 箱の中心から始点に向かうベクトル
	Vector3 centerToEnd{ capsule.endPos - center }; // 箱の中心から終点に向かうベクトル

	// 内積を使ってローカル空間のXYZ座標に変換する
	Vector3 localStart
	{
		Vector3::Dot(centerToStart, localAxisX),
		Vector3::Dot(centerToStart, localAxisY),
		Vector3::Dot(centerToStart, localAxisZ),
	};

	Vector3 localEnd
	{
		Vector3::Dot(centerToEnd, localAxisX),
		Vector3::Dot(centerToEnd, localAxisY),
		Vector3::Dot(centerToEnd, localAxisZ),
	};

	// 原点(0, 0, 0)からローカル線分上で一番近い点を探す
	Vector3 localP1{ ClosestPointOnSegment(localStart, localEnd, Vector3(0.0f, 0.0f, 0.0f)) };

	// localP1を-halfSizeからhalfSizeでクランプして箱に閉じる
	Vector3 localQ1{ Vector3(std::clamp(localP1.x, -halfSize.x, halfSize.x), std::clamp(localP1.y, -halfSize.y, halfSize.y), std::clamp(localP1.z, -halfSize.z, halfSize.z)) };

	// 箱の表面からローカル線分上で最近点を探す
	Vector3 localP2{ ClosestPointOnSegment(localStart, localEnd, localQ1) };

	// めり込み判定とローカル法線の計算
	Vector3 distanceVec{ localP2 - localQ1 };
	float distance{ distanceVec.Length() };

	if (distance > capsule.radius)
	{
		return false;
	}


	float penetration{ capsule.radius - distance };
	Vector3 localNormal{};

	if (distance == 0.0f)
	{
		// 深く刺さった場合は原点からlocalP2へ向けて押し出す
		if (localP2.LengthNoSqr() == 0.0f)
		{
			localNormal = Vector3(0.0f, 1.0f, 0.0f);
		}
		else
		{
			localNormal = localP2;
			localNormal.Normalize();
		}
	}
	else
	{
		// 浅い場合はそのまま正規化
		distanceVec.Normalize();
		localNormal = distanceVec;
	}

	// ローカルの法線をワールド空間に戻す
	Vector3 worldNormal
	{
		(localNormal.x * localAxisX) +
		(localNormal.y * localAxisY) +
		(localNormal.z * localAxisZ)
	};

	pushVec = worldNormal * penetration;
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
	// 球とカプセル
	else if (colliders.count(ColliderType::Sphere) > 0 && colliders.count(ColliderType::Capsule) > 0 && colliders[ColliderType::Sphere].size() == 1 && colliders[ColliderType::Capsule].size() == 1)
	{
		ProcessSphereCapsuleCollision(colliders[ColliderType::Sphere][0], colliders[ColliderType::Capsule][0]);
	}
	// カプセルとカプセル
	else if (colliders.count(ColliderType::Capsule) > 0 && colliders[ColliderType::Capsule].size() == 2)
	{
		ProcessCapsuleCapsuleCollision(colliders[ColliderType::Capsule][0], colliders[ColliderType::Capsule][1]);
	}
	// 箱とカプセル
	else if (colliders.count(ColliderType::Box) > 0 && colliders.count(ColliderType::Capsule) > 0 && colliders[ColliderType::Box].size() == 1 && colliders[ColliderType::Capsule].size() == 1)
	{
		ProcessBoxCapsuleCollision(colliders[ColliderType::Box][0], colliders[ColliderType::Capsule][0]);
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

void CollisionManager::ProcessSphereCapsuleCollision(ColliderBase* sphere, ColliderBase* capsule)
{
	// 一時的な入れ物
	SphereCollider rebuildShere{};
	CapsuleCollider rebuildCapsule{};

	// 抽出して3D図形として復元
	if (!rebuilder.Rebuild(sphere, rebuildShere) || !rebuilder.Rebuild(capsule, rebuildCapsule))
	{
		return; // 復元失敗
	}

	Vector3 pushVec{};

	if (IsOverlapping(rebuildShere, rebuildCapsule, pushVec))
	{
		NotifyResults(*sphere, *capsule, pushVec);
		NotifyResults(*capsule, *sphere, -pushVec);
	}
}

void CollisionManager::ProcessCapsuleCapsuleCollision(ColliderBase* capsule01, ColliderBase* capsule02)
{
	// 一時的な入れ物
	CapsuleCollider rebuildCapsule01{};
	CapsuleCollider rebuildCapsule02{};

	// 抽出して3D図形として復元
	if (!rebuilder.Rebuild(capsule01, rebuildCapsule01) || !rebuilder.Rebuild(capsule02, rebuildCapsule02))
	{
		return; // 復元失敗
	}

	Vector3 pushVec{};

	if (IsOverlapping(rebuildCapsule01, rebuildCapsule02, pushVec))
	{
		NotifyResults(*capsule02, *capsule01, pushVec);
		NotifyResults(*capsule01, *capsule02, -pushVec);
	}

}

void CollisionManager::ProcessBoxCapsuleCollision(ColliderBase* box, ColliderBase* capsule)
{
	// 一時的な入れ物
	CapsuleCollider rebuildCapsule{};
	BoxCollider rebuildBox{};

	// 抽出して3D図形として復元
	if (!rebuilder.Rebuild(box, rebuildBox) || !rebuilder.Rebuild(capsule, rebuildCapsule))
	{
		return; // 復元失敗
	}

	Vector3 pushVec{};

	// 箱が回転していない場合
	if (rebuildBox.rotate == Vector3::Zero())
	{
		if (IsOverlapping(rebuildBox, rebuildCapsule, pushVec))
		{
			NotifyResults(*box, *capsule, pushVec);
			NotifyResults(*capsule, *box, -pushVec);
		}
	}
	else
	{
		if (IsOverlappingOBB(rebuildBox, rebuildCapsule, pushVec))
		{
			NotifyResults(*box, *capsule, pushVec);
			NotifyResults(*capsule, *box, -pushVec);
		}
	}
}

void CollisionManager::NotifyResults(ColliderBase& from, ColliderBase& to, Vector3 pushVec)
{
	if (to.ReciveFunc != nullptr)
	{
		HitResult result{ from.tag, from.subTag, pushVec };
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
	tmpZ = result.y * sx + result.z * cx;
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
	float penetration{ totalLength - distance };

	if (penetration <= 0.0f)
	{
		return false; // 中心間の距離が影の合計の長さよりも大きければ当たっていない
	}

	outPenetration = penetration;

	// ここまでくれば当たっているとする
	return true;
}

void CollisionManager::ClosestPointSegmentSegment(
	const Vector3& p1, const Vector3& q1, // 線分1の始点と終点
	const Vector3& p2, const Vector3& q2, // 線分2の始点と終点
	Vector3& closest1, Vector3& closest2  // 結果として出力される2つの最近点
)
{
	Vector3 d1{ q1 - p1 }; // 一つ目の線分の方向
	Vector3 d2{ q2 - p2 }; // 二つ目の線分の方向
	Vector3 r{ p1 - p2 }; // 始点から始点へのベクトル

	float a{ d1.LengthNoSqr() }; // 線分1の長さの二乗
	float e{ d2.LengthNoSqr() }; // 線分2の長さの二乗
	float f{ Vector3::Dot(d2, r) }; // 線分2上への始点どうしのベクトルの投影

	float s{ 0.0f };
	float t{ 0.0f };

	// 双方の線分が点になってしまっている場合のエラー回避
	if (a <= GAME_EPSILON<float> && e <= GAME_EPSILON<float>)
	{
		closest1 = p1;
		closest2 = p2;
		return;
	}

	// ここからは片方の線分が点になっている時の処理
	if (a <= GAME_EPSILON<float>)
	{
		// 線分1が点の時
		s = 0.0f;
		t = std::clamp(f / e, 0.0f, 1.0f);
	}
	else
	{
		float c{ Vector3::Dot(d1, r) }; // 線分1上への始点同士ベクトルの投影
		if (e <= GAME_EPSILON<float>)
		{
			// 線分2が点の時
			t = 0.0f;
			s = std::clamp(-c / a, 0.0f, 1.0f);
		}
		// 両方の線分が長さを持っている場合
		else
		{
			float b{ Vector3::Dot(d1, d2) };
			float denom{ a * e - b * b }; // 平行かどうかの判定分母

			// 線分が平行ではない場合
			if (denom != 0.0f)
			{
				s = std::clamp((b * f - c * e) / denom, 0.0f, 1.0f);
			}
			else
			{
				// 平行な場合は始点を基準にする
				s = 0.0f;
			}

			// 線分1の割合sに基づいて線分2の割合tを計算しクランプする
			float tNom{ b * s + f };
			if (tNom < 0.0f)
			{
				t = 0.0f;
				s = std::clamp(-c / a, 0.0f, 1.0f);
			}
			else if (tNom > e)
			{
				t = 1.0f;
				s = std::clamp((b - c) / a, 0.0f, 1.0f);
			}
			else
			{
				t = tNom / e;
			}
		}
	}

	// 求まった割合を使って実際の座標を出す
	closest1 = p1 + d1 * s;
	closest2 = p2 + d2 * t;
}

Vector3 CollisionManager::ClosestPointOnSegment(const Vector3& start, const Vector3& end, const Vector3& point)
{
	Vector3 AB{ end - start }; // 始点から終点
	Vector3 AP{ point - start }; // 始点からある点

	float lenSq{ AB.LengthNoSqr() }; // ABの長さの二乗
	if (lenSq == 0.0f) return start; // 線分が点の時のエラー回避

	// 内積を使って割合を出す
	float t{ std::clamp(Vector3::Dot(AB, AP) / lenSq, 0.0f, 1.0f) };
	return	start + (AB * t); // 始点 + 線分の長さ * 割合

}

bool CollisionManager::RayCast(const Ray& ray, RayCastHit& outHit, CollisionTag targetTag)
{
	bool isHitAny{ false }; // オブジェクトに当たったかどうか
	float closestDistance{ FLT_MAX }; // 一番近かった距離(最初はfloat型の最大値)

	// 指定されたタグのコライダー群を取得
	auto it{ colliderByTag.find(targetTag) };
	if (it == colliderByTag.end()) return false; // そのコライダーが一つも登録されていない場合の処理

	std::vector<ColliderBase*>& targets{ it->second };

	for (ColliderBase* collider : targets)
	{
		if (collider == nullptr) continue;

		float hitDistance{ 0.0f }; // 当たったオブジェクトの距離
		bool hitThis{ false }; // 検証中のコライダーに対して当たっているかのフラグ

		// 相手の型に合わせた専用のRay関数を呼ぶ
		switch (collider->GetType())
		{
		case ColliderType::Sphere:
			// RayとSphereの判定
			hitThis = IntersectRaySphere(ray, *static_cast<SphereCollider*>(collider) , hitDistance);
			break;
		case ColliderType::Box:
		{
			// RayとBoxの判定
			BoxCollider* box{ static_cast<BoxCollider*>(collider) };
			// 回転しているかどうか
			if (box->rotate != Vector3::Zero())
			{
				// 回転していない->AABB
				hitThis = IntersectRayBox(ray, *box, hitDistance);
			}
			else
			{
				// 回転している->OBB
				hitThis = IntersectRayOBB(ray, *box, hitDistance);
			}

			break;
		}
		case ColliderType::Capsule:
			// Rayとカプセルの判定
			hitThis = IntersectRayCapsule(ray, *static_cast<CapsuleCollider*>(collider), hitDistance);
			break;
		default:
			break;
		}

		// もし当たっていてかつ今まで一番近かった場合は結果を更新する
		if (hitThis && hitDistance < closestDistance)
		{
			closestDistance = hitDistance;
			isHitAny = true;

			// outHitに情報を詰める
			outHit.distance = hitDistance;
			outHit.hitCollider = collider;
		}
	}

	return isHitAny;
}

bool CollisionManager::IntersectRaySphere(const Ray& ray, const SphereCollider& sphere, float& outDistance)
{
	Vector3 m{ ray.origin - sphere.pos }; // 球の中心からRayのスタート地点へのベクトル
	float b{ Vector3::Dot(m, ray.direction) }; // Rayの進行方向と始点と中心間のベクトル中心間
	float c{ m.LengthNoSqr() - (sphere.radius * sphere.radius) }; // 中心間ベクトルの二乗ノルム - 球の半径二乗 (表面からスタート地点までの長さの二乗)

	// 枝刈り処理　レイの始点が球の外側にありかつレイが球から遠ざかっている場合は当たらない
	if (c > 0.0f && b > 0.0f) return false;

	float discriminant{ (b * b) - c }; // 判別式

	if (discriminant < 0.0f) return false; // 判別式の結果が負の値なら解なしとし、当たっていないと判断

	outDistance = -b - std::sqrt(discriminant); // 当たっているなら一番近い交点を求める

	// もしoutDistanceがマイナスならレイを発射した地点が球の中にあったということなのでその場合は0.0fにしておく
	if (outDistance < 0.0f)
	{
		outDistance = 0.0f;
	}

	return true;
}


bool CollisionManager::IntersectRayBox(const Ray& ray, const BoxCollider& box, float& outDistance)
{
	Vector3 minPos{ box.GetMinPos() };
	Vector3 maxPos{ box.GetMaxPos() };

	// あらかじめRay方向の逆数を計算しておく
	Vector3 invDir{
		1.0f / ray.direction.x,
		1.0f / ray.direction.y,
		1.0f / ray.direction.z,
	};

	// X軸のスラブに入った時間と出た時間
	float tx1{ (minPos.x - ray.origin.x) * invDir.x };
	float tx2{ (maxPos.x - ray.origin.x) * invDir.x };
	float tminX{ std::min(tx1, tx2) }; // どちらから入るのかわからないので小さいほうを入った時間にする
	float tmaxX{ std::min(tx1, tx2) }; // 出たほうを大きい時間にする

	// Y軸のスラブに入った時間と出た時間
	float ty1{ (minPos.y - ray.origin.y) * invDir.y };
	float ty2{ (maxPos.y - ray.origin.y) * invDir.y };
	float tminY{ std::min(ty1, ty2) }; // どちらから入るのかわからないので小さいほうを入った時間にする
	float tmaxY{ std::max(ty1, ty2) }; // 出たほうを大きい時間にする

	// XとYの時間がかぶっているかチェック
	// Xの部屋を出る時間よりYの部屋に入る時間のほうが遅いか
	// Yの部屋を出る時間よりXの部屋に入る時間の方が遅いならfalseを返す
	if (tmaxX < tminY || tmaxY < tminX) return false;

	// かぶっているなら部屋の中にいる時間を更新する
	// 入った時間はより遅いほう、出た時間はより速いほうが両方の部屋に共通している時間になる
	float tMin{ std::max(tminX, tminY) };
	float tMax{ std::min(tmaxX, tmaxY) };

	// Z軸スラブの計算
	float tz1{ (minPos.z - ray.origin.z) * invDir.z };
	float tz2{ (maxPos.z - ray.origin.z) * invDir.z };
	float tminZ{ std::min(tz1, tz2) }; // どちらから入るのかわからないので小さいほうを入った時間にする
	float tmaxZ{ std::max(tz1, tz2) }; // 出たほうを大きい時間にする


	// tMinとtMaxを使ってz軸で計算した値がかぶっているかのチェック
	if (tMax < tminZ || tmaxZ < tMin) return false;

	tMin = std::max(tminZ, tMin);
	tMax = std::min(tmaxZ, tMax);

	if (tMax < 0.0f) return false; //　箱がRayの発射地点より後ろに合ったら当たっていない

	// 全てのスラブに当たっていて先に存在するなら命中しているとする
	// 箱の中から撃った場合はtMinがマイナスになるので距離を0にする
	outDistance = (tMin < 0.0f) ? 0.0f : tMin;
	return true;
}

bool CollisionManager::IntersectRayOBB(const Ray& ray, const BoxCollider& box, float& outDistance)
{
	Vector3 center{ box.GetCenter() };
	Vector3 halfSize{ box.GetHalfSize() };

	// OBB用の各ローカル軸を作る
	Vector3 localAxisX{ RotateVector(Vector3(1.0f, 0.0f, 0.0f), box.rotate) };
	Vector3 localAxisY{ RotateVector(Vector3(0.0f, 1.0f, 0.0f), box.rotate) };
	Vector3 localAxisZ{ RotateVector(Vector3(0.0f, 0.0f, 1.0f), box.rotate) };

	// Rayをローカル軸へ持ってくる
	Vector3 centerToOrigin{ ray.origin - center };
	// 発射地点をローカルへ
	Vector3 localOrigin{
		Vector3::Dot(centerToOrigin, localAxisX),
		Vector3::Dot(centerToOrigin, localAxisY),
		Vector3::Dot(centerToOrigin, localAxisZ),
	};

	// 発射方向をローカルへ
	Vector3 localDir{
		Vector3::Dot(ray.direction, localAxisX),
		Vector3::Dot(ray.direction, localAxisY),
		Vector3::Dot(ray.direction, localAxisZ),
	};

	// ローカル空間の仮のRayとローカル空間の箱AABBで判定をする
	Ray localRay{ localOrigin, localDir };

	// ローカル空間では箱は原点にあり最小、最大は±halfSizeになるためそれをAABBとして使う
	BoxCollider localAABB;
	localAABB.pos = -halfSize;
	localAABB.size = halfSize * 2.0f;

	// 仮想Rayと仮想AABBで判定を行う
	return IntersectRayBox(localRay, localAABB, outDistance);
}

bool CollisionManager::IntersectRayCapsule(const Ray& ray, const CapsuleCollider& capsule, float& outDistance)
{
	// 始点の球と終点の球と円柱にRayを飛ばし、最初に当たった距離を採用する
	bool isHitAny{ false };
	float minT{ FLT_MAX }; // 一番近い距離を保持する変数

	// 始点球との判定
	SphereCollider startSphere{};
	startSphere.pos = capsule.startPos;
	startSphere.radius = capsule.radius;
	float tStart{ 0.0f };
	// 当たっていてかつminTより近ければminTを更新する
	if (IntersectRaySphere(ray, startSphere, tStart))
	{
		minT = std::min(minT, tStart);
		isHitAny = true;
	}

	// 終点球の判定
	SphereCollider endSphere{};
	endSphere.pos = capsule.endPos;
	endSphere.radius = capsule.radius;
	float tEnd{ 0.0f };
	// 当たっていてかつminTより近ければminTを更新する
	if (IntersectRaySphere(ray, endSphere, tEnd))
	{
		minT = std::min(minT, tEnd);
		isHitAny = true;
	}

	// 円柱との判定
	Vector3 AB{ capsule.endPos - capsule.startPos };
	float capsuleLength{ AB.Length() };

	if (capsuleLength > 0.0f)
	{
		Vector3 u{ AB }; // 方向だけを取り出すためのキャッシュ
		u.Normalize(); // カプセルの軸方向の正規化ベクトル

		// レイとカプセルの始点の差分
		Vector3 m = ray.origin - capsule.startPos;
		// カプセル始点からRay始点までのベクトルとカプセルの軸ベクトルとの内積を取りその長さ分軸ベクトル方向に伸ばすことで投影分の長さの軸ベクトルを得る
		// その後mから引くことで成分を一つ消し2D上に落とし込む
		Vector3 m2D{ m - (u * Vector3::Dot(m, u)) };
		Vector3 d2D{ ray.direction - (u * Vector3::Dot(ray.direction, u)) }; // 上記と同様Rayの方向も2D化する

		// 球で行っている二次方程式を使う
		// d2Dは投影で長さが変わっている(a ≠ 1)ため再計算する
		float a{ d2D.LengthNoSqr() };
		float b{ Vector3::Dot(m2D, d2D) };
		float c{ m2D.LengthNoSqr() - (capsule.radius * capsule.radius) };

		// d2Dがゼロ(Rayがカプセルの軸と完全に平行)でなければ計算
		if (a > GAME_EPSILON<float>)
		{
			float d{ (b * b) - (a * c) };

			if (d > 0)
			{
				float tCyl{ std::max((-b - std::sqrt(d)) / a, 0.0f) }; // 解の公式結果が負なら0.0fにする
				// 円柱に当たった場所がカプセル長さのうちにあるか
				Vector3 P{ ray.origin + (ray.direction * tCyl) }; // 実際に当たった3D座標
				float dotP{ Vector3::Dot(P - capsule.startPos, u) }; // 始点からどのくらい進んだ位置か
			
				if (dotP > 0.0f && capsuleLength >= dotP)
				{
					minT = tCyl;
					isHitAny = true;
				}
			}
		}
	}

	if (isHitAny)
	{
		outDistance = minT;
	}

	return isHitAny;

}

void CollisionManager::Clear()
{
	colliderByTag.clear();
}
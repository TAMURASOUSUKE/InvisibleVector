#pragma once
#include <vector>
#include "Physics.h"

// 物理挙動を管理するクラス
class PhysicsManager
{
public:
	// シングルトン化
	static PhysicsManager& Instance()
	{
		static PhysicsManager instance;
		return instance;
	}

	~PhysicsManager() = default;

	void Register(PhysicsBody* body); // 毎フレームPhysicsBody
	void FixedUpdate(); // 物理更新
	void ResolveCollision(PhysicsBody& body, Vector3& pushVec); // 衝突処理時にこの関数を呼ぶだけで欲案る
	void Clear(); // vectorクリア用(毎フレーム末に呼ぶ)

private:
	PhysicsManager() = default;

private:
	std::vector<PhysicsBody*> bodys; // 登録されたPhysicsBodyを格納する変数
};
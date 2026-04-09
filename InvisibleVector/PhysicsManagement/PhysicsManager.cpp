#include "TimeManager.h"
#include "PhysicsManager.h"

void PhysicsManager::Register(PhysicsBody* body)
{
	if (body != nullptr)
	{
		bodys.push_back(body);
	}
}

void PhysicsManager::FixedUpdate()
{
	for (auto& body : bodys)
	{

		// 設置中は重力を切る
		if (body->isGround)
		{

			if (body->velocity->y > UNGROUND_VELOCITY)
			{
				body->isGround = false;
			}
			else
			{
				body->acceleration->y = 0.0f;

				if (body->velocity->y < 0.0f)
				{
					body->velocity->y = 0.0f;
				}

				if (std::abs(body->velocity->x) < VELOCITY_SLEEP_THRESHOLD) body->velocity->x = 0.0f;
				if (std::abs(body->velocity->z) < VELOCITY_SLEEP_THRESHOLD) body->velocity->z = 0.0f;

				// 完全に止まっていたら積分をスキップ
				if (body->velocity->LengthNoSqr() < VELOCITY_SLEEP_THRESHOLD * VELOCITY_SLEEP_THRESHOLD)
				{
					continue;  // このbodyの積分を飛ばす
				}
			}
		}
		else if (body->useGravity)
		{
			// 重力有効処理
			body->acceleration->y = -GRAVITY;
		}

		(*body->velocity) += (*body->acceleration) * TimeManager::FixedDeltaTime();

		(*body->position) += (*body->velocity) * TimeManager::FixedDeltaTime();

		// 抵抗計算
		(*body->velocity) *= (1.0f - body->drag);

	}
}

void PhysicsManager::ResolveCollision(PhysicsBody& body, Vector3& pushVec)
{
	// 既に接地して停止しているなら位置補正だけして終了
	if (body.isGround && body.velocity->LengthNoSqr() < VELOCITY_SLEEP_THRESHOLD * VELOCITY_SLEEP_THRESHOLD)
	{
		(*body.velocity) = Vector3::Zero();
		(*body.acceleration) = Vector3::Zero();
		return;
	}

	(*body.position) += pushVec;

	Vector3 normal{ Vector3::Normalized(pushVec) };

	// 押し戻し方向のY成分が設定値以上なら地面と判断しフラグをtrueにする
	if (normal.y > GROUND_NORMAL_THRESHOLD)
	{
		body.isGround = true;
	}

	// 内積を使って速度の法線方向成分を取り出す
	float velocityDot{ Vector3::Dot((*body.velocity), normal) };

	// 成分が負(接触面に向かっている)なら反射する
	if (velocityDot < 0.0f)
	{
		(*body.velocity) = (*body.velocity) - normal * ((1.0f + body.bounciness) * velocityDot);

		// 反射が微小なら0にする
		if (std::abs(body.velocity->y) < VELOCITY_SLEEP_THRESHOLD)
		{
			body.velocity->y = 0.0f;
		}
		// 反射で上向き速度が出たなら接地解除
		else if (body.velocity->y > UNGROUND_VELOCITY)
		{
			body.isGround = false;
		}
	}

	if (body.isGround)
	{

		if (body.velocity->y < 0.0f)
		{
			body.velocity->y = 0.0f;
		}

		body.acceleration->y = 0.0f;

		// 速度が規定値以下なら0にする
		if ((*body.velocity).LengthNoSqr() <= VELOCITY_SLEEP_THRESHOLD * VELOCITY_SLEEP_THRESHOLD)
		{
			(*body.velocity) = Vector3::Zero();
			(*body.acceleration) = Vector3::Zero();
	
		}


		// 水平速度に規定値を掛ける
		// 速度のx,zにfrictionをかける
		Vector3 currentVel{ (*body.velocity) }; // 現在の速度
		// 水平速度ベクトル
		Vector3 horizontalVel{ currentVel.x, 0.0f, currentVel.z };

		// 摩擦を掛ける
		// friction = 0.6なら減算されて40％になる
		horizontalVel *= (1.0f - body.friction);

		body.velocity->x = horizontalVel.x;
		body.velocity->z = horizontalVel.z;
	}

}

// 配列のクリア
void PhysicsManager::Clear()
{

	bodys.clear();
}
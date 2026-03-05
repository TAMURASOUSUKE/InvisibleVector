#pragma once
#include <variant>
#include <unordered_map>
#include <functional>
#include "Vector3.h"
#include "CollisionConstant.h"

// 結果を通知する際に使う判定情報体
struct HitResult
{
	// コンストラクタ
	HitResult() = default; // デフォルト
	HitResult(CollisionTag tag, SubTag subTag, Vector3 pushVec) : tag{ tag }, subTag{ subTag }, pushVec{ pushVec } {}

	// 当たった相手
	CollisionTag tag{ CollisionTag::None };
	// 詳細情報 Enemy等とあたった場合どの相手と当たった化などを取得したい場合に使う
	SubTag subTag{ 0 };

	// 衝突判定に使う押し戻し用
	Vector3 pushVec{ Vector3::Zero() };

};

// 判定の共通情報をまとめた構造体
struct ColliderBase
{
	ColliderBase() = default; // デフォルトコンストラクタ
	// 各タグと当たった時に呼び出したい関数を引数に入れる
	ColliderBase(CollisionTag tag, SubTag subTag, std::function<void(HitResult&)> recive_func)
		: tag{ tag }, subTag{ subTag }, ReciveFunc{ recive_func }
	{
	}

	virtual ~ColliderBase() = default; // デストラクタ

	virtual void ExtractParameter(std::unordered_map<ColliderParamKey, float>& out_parames_) const = 0;

	virtual ColliderType GetType() const = 0;

	// Collider所持オブジェクト識別用
	CollisionTag tag{ CollisionTag::None };

	// 追加の情報識別用
	SubTag subTag{ 0 };

	// 判定結果を受信するための器
	std::function<void(HitResult&)> ReciveFunc{ nullptr };
};

struct SphereCollider : public ColliderBase
{
	SphereCollider() = default;
	SphereCollider(Vector3 pos, float radius, CollisionTag tag, SubTag subTag, std::function<void(HitResult&)> func) :
		ColliderBase{ tag, subTag, func },
		pos{ pos },
		radius{ radius }
	{
	}

	SphereCollider(float x, float y, float z, float radius, CollisionTag tag, SubTag subTag, std::function<void(HitResult&)> func) :
		ColliderBase{ tag, subTag, func },
		pos{ x, y, z },
		radius{ radius }
	{
	}

	// コライダーの情報をoutPramsにつめる
	void ExtractParameter(std::unordered_map<ColliderParamKey, float>& outParams) const override
	{
		outParams[ColliderParamKey::PosX] = pos.x;
		outParams[ColliderParamKey::PosY] = pos.y;
		outParams[ColliderParamKey::PosZ] = pos.z;
		outParams[ColliderParamKey::Radius] = radius;
	}

	// タイプのゲッター
	ColliderType GetType() const override 
	{
		return ColliderType::Sphere;
	}

	// 中心座標
	Vector3 pos{ Vector3::Zero() };
	// 半径
	float radius{ 0.0f };
};

struct BoxCollider : public ColliderBase
{
	BoxCollider() = default;
	BoxCollider(Vector3 pos, Vector3 size, Vector3 rota, CollisionTag tag, SubTag subTag, std::function<void(HitResult&)> func) :
		ColliderBase(tag, subTag, func), pos{ pos }, size{ size }, rotate{ rota }
	{
	}

	BoxCollider(float x, float y, float z, float width, float height, float depth, float rotaX, float rotaY, float rotaZ, CollisionTag tag, SubTag subTag, std::function<void(HitResult&)> func) :
		ColliderBase{tag, subTag, func},
		pos {x, y, z},
		size{width, height, depth},
		rotate{rotaX, rotaY, rotaZ} { }

	void ExtractParameter(std::unordered_map<ColliderParamKey, float>& outParam) const override
	{
		outParam[ColliderParamKey::PosX] = pos.x;
		outParam[ColliderParamKey::PosY] = pos.y;
		outParam[ColliderParamKey::PosZ] = pos.z;
		outParam[ColliderParamKey::RotaX] = rotate.x;
		outParam[ColliderParamKey::RotaY] = rotate.y;
		outParam[ColliderParamKey::RotaZ] = rotate.z;
		outParam[ColliderParamKey::Width] = size.x;
		outParam[ColliderParamKey::Height] = size.y;
		outParam[ColliderParamKey::Depth] = size.z;
	}

	// ゲッター類 (最大と最小サイズは回転していない状態を前提としたGetterなので注意)
	Vector3 GetCenter() const
	{
		return pos + (size * 0.5f);
	}

	Vector3 GetHalfSize() const
	{
		return size * 0.5f;
	}

	Vector3 GetMinPos() const
	{
		return pos;
	}

	Vector3 GetMaxPos() const
	{
		return pos + size;
	}

	// 判定の種類
	ColliderType GetType() const override
	{
		return ColliderType::Box;
	}


	// 左上座標
	Vector3 pos{ Vector3::Zero() };
	// サイズ
	Vector3 size{ Vector3::Zero() };
	// 回転
	Vector3 rotate{ Vector3::Zero() };

};

struct CapsuleCollider : public ColliderBase
{
	CapsuleCollider() = default;
	CapsuleCollider(Vector3 startPos, Vector3 endPos, Vector3 rotate, float radius, CollisionTag tag, SubTag subTag, std::function<void(HitResult&)> func) :
		ColliderBase{tag, subTag, func}, startPos{startPos}, endPos{endPos}, rotate{rotate}, radius{radius} { }

	CapsuleCollider(float startX, float startY, float startZ, float endX, float endY, float endZ, float rotaX, float rotaY, float rotaZ, float radius, CollisionTag tag, SubTag subTag, std::function<void(HitResult&)> func) :
		ColliderBase{tag, subTag, func}, startPos{startX, startY, startZ}, endPos{endX, endY, endZ}, rotate{rotaX, rotaY, rotaZ}, radius{radius} { }

	// outParamにデータを詰め込む
	void ExtractParameter(std::unordered_map<ColliderParamKey, float>& outParam) const override
	{
		outParam[ColliderParamKey::PosX] = startPos.x;
		outParam[ColliderParamKey::PosY] = startPos.y;
		outParam[ColliderParamKey::PosZ] = startPos.z;
		outParam[ColliderParamKey::EndPosX] = endPos.x;
		outParam[ColliderParamKey::EndPosY] = endPos.y;
		outParam[ColliderParamKey::EndPosZ] = endPos.z;
		outParam[ColliderParamKey::RotaX] = rotate.x;
		outParam[ColliderParamKey::RotaY] = rotate.y;
		outParam[ColliderParamKey::RotaZ] = rotate.z;
		outParam[ColliderParamKey::Radius] = radius;
 	}

	// 高さ取得
	Vector3 GetHeight() const
	{
		return endPos - startPos;
	}

	ColliderType GetType() const override
	{
		return ColliderType::Capsule;
	}



	Vector3 startPos{ Vector3::Zero() };
	Vector3 endPos{ Vector3::Zero() };
	Vector3 rotate{ Vector3::Zero() };
	float radius{ 0.0f };
};
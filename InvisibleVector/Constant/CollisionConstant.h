#pragma once
#include <variant>

// 種類を識別するためのもの
enum class CollisionTag
{
	None = -1,
	Player,
	Enemy,
	Ground,
};

// Enemyがどのタイプを持っているか(SubTag)
enum class CollisionEnemyTag
{
	None = -1,
	Boss,
};

// 判定形状識別用
enum class ColliderType
{
	Sphere,
	Box,
	Capsule,
	Mesh,
};

// 判定の情報リスト
enum class ColliderParamKey
{
	PosX, // X位置
	PosY, // Y位置
	PosZ, // Z位置

	RotaX, // 回転X
	RotaY, // 回転Y
	RotaZ, // 回転Z

	Radius, // 半径

	Width, // 横幅
	Height, // 縦幅
	Depth, // 奥行

	EndPosX, // カプセルに使う終点座標X
	EndPosY, // カプセルに使う終点座標Y
	EndPosZ, // カプセルに使う終点座標Z
};

struct SubTag
{
	// EnemyTagのGetter
	CollisionEnemyTag GetEnemyTag()
	{
		// 他を確認する
		if (std::holds_alternative<CollisionEnemyTag>(tag))
		{
			return std::get<CollisionEnemyTag>(tag);
		}

		return CollisionEnemyTag::None; // 型が異なる場合は取得不可としてNoneを返す
	}

	// 識別情報 (variant : 指定した複数の型に対応できる)
	std::variant<int, CollisionEnemyTag> tag{ 0 };
};


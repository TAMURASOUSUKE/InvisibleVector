#pragma once
#include "Vector_Dxlib.h"
#include "PhysicsConstant.h"
// 物理挙動をするオブジェクトが持つ構造体を定義するファイル

// これを物理挙動を持つオブジェクトが保持する
struct PhysicsBody
{
	PhysicsBody() = default;
	PhysicsBody(Vector3* position, Vector3* velocity, Vector3* acceleration) : position{position}, velocity{velocity}, acceleration{acceleration}
	{
	}

	// ポインタ類(書き換えが必要なもの)
	Vector3* position;
	Vector3* velocity;
	Vector3* acceleration;

	// パラメータ類
	float mass{DEFAULT_MASS};
	float bounciness{DEFAULT_BAUNCINESS};
	float drag{DEFAULT_DRAG};
	float friction{DEFAULT_FRICTION};

	bool useGravity{ true };
	bool isGround{ false };
};
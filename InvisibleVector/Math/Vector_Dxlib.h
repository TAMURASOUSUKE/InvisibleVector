#pragma once
#include "Dxlib.h"
#include "Vector3.h"
#include "Vector2.h"


/// <summary>
/// [EN] Convert VECTOR to Vector3 [JP] Vector3からVECTORへの変換関数
/// </summary>
/// <param name="other">[EN] Other vector [JP] ほかのベクトル</param>
/// <returns>[EN] Convert after VECTOR [JP] 変換後のVECTOR</returns>
inline VECTOR ToDxLib(const Vector3& other)
{
	return { other.x, other.y, other.z };
}


/// <summary>
/// [EN] Convert VECTOR to Vector2 [JP] Vector2からVECTORへの変換関数
/// </summary>
/// <param name="other">[EN] Other vector [JP] ほかのベクトル</param>
/// <returns>[EN] Convert after VECTOR [JP] 変換後のVECTOR</returns>
inline VECTOR ToDxLib(const Vector2& other)
{
	return { other.x, other.y, 0.0f };
}

/// <summary>
/// [EN] Convert VECTOR to Vector3(Ver.Int) [JP] Vector3からVECTORへの変換関数(Int版)
/// </summary>
/// <param name="other">[EN] Other vector [JP] ほかのベクトル</param>
/// <returns>[EN] Convert after VECTOR [JP] 変換後のVECTOR</returns>
inline VECTOR ToDxLib(const Vector3Int& other)
{
	return { static_cast<float>(other.x), static_cast<float>(other.y), static_cast<float>(other.z) };
}


/// <summary>
/// [EN] Convert VECTOR to Vector2(Ver.Int) [JP] Vector2からVECTORへの変換関数(Int版)
/// </summary>
/// <param name="other">[EN] Other vector [JP] ほかのベクトル</param>
/// <returns>[EN] Convert after VECTOR [JP] 変換後のVECTOR</returns>
inline VECTOR ToDxLib(const Vector2Int& other)
{
	return { static_cast<float>(other.x), static_cast<float>(other.y), 0.0f };
}

/// <summary>
/// [EN] Convert Vector3 to VECTOR [JP] VECTORからVector3への変換関数
/// </summary>
/// <param name="other">[EN] Other vector [JP] ほかのベクトル</param>
/// <returns>[EN] Convert after Vector3 [JP] 変換後のVector3</returns>
inline Vector3 ToVector3(const VECTOR& other)
{
	return Vector3{ other.x, other.y, other.z };
}

/// <summary>
/// [EN] Convert VECTOR to Vector2 [JP] VECTORからVector2への変換関数
/// </summary>
/// <param name="other">[EN] Other vector [JP] ほかのベクトル</param>
/// <returns>[EN] Convert after Vector2 [JP] 変換後のVector2</returns>
inline Vector2 ToVector2(const VECTOR& other)
{
	return Vector2{ other.x, other.y };
}

/// <summary>
/// [EN] Convert Vector3 to VECTOR(Ver.Int) [JP] VECTORからVector3への変換関数(Int版)
/// </summary>
/// <param name="other">[EN] Other vector [JP] ほかのベクトル</param>
/// <returns>[EN] Convert after Vector3 [JP] 変換後のVector3</returns>
inline Vector3Int ToVector3Int(const VECTOR& other)
{
	return Vector3Int{ static_cast<int>(other.x), static_cast<int>(other.y), static_cast<int>(other.z) };
}

/// <summary>
/// [EN] Convert Vector2 to VECTOR(Ver.Int) [JP] VECTORからVector2への変換関数(Int版)
/// </summary>
/// <param name="other">[EN] Other vector [JP] ほかのベクトル</param>
/// <returns>[EN] Convert after Vector2 [JP] 変換後のVector2</returns>
inline Vector2Int ToVector2Int(const VECTOR& other)
{
	return Vector2Int{ static_cast<int>(other.x), static_cast<int>(other.y) };
}
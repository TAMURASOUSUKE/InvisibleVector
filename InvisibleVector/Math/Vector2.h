#pragma once
#include <type_traits>
#include <cmath>
#include "SystemConstant.h"

/// <summary>
/// [EN] This struct is contained coordinate "x, y". [JP] x,yの座標を内包した構造体です。
/// </summary>
template<class ValueType>
struct Vector2Base
{
	/// <summary>
	/// [EN] Constructor : Default. Setting zero to all coordinates.
	/// [JP] コンストラクタ : デフォルト, すべての座標を0に設定する
	/// </summary>
	Vector2Base() : x{ 0 }, y{ 0 }{}

	/// <summary>
	/// [EN] Constructor : Initialize the coordinates with the values entered as arguments.
	/// [JP] コンストラクタ : 引数に入れた値で座標を初期化する
	/// </summary>
	/// <param name="x">[EN] Coordinate x. [JP] X座標</param>
	/// <param name="y">[EN] Coordinate y. [JP] Y座標</param>
	Vector2Base(ValueType x, ValueType y) : x{ x }, y{ y } {}

	/// <summary>
	/// [EN] Calculate Dot. 
	/// [JP] 内積計算を行う
	/// </summary>
	/// <param name="vec01">[EN] Vector2 [JP] Vector2クラスを入れる</param>
	/// <param name="vec02">[EN] Vector2 [JP] Vector2クラスを入れる</param>
	/// <returns></returns>
	static ValueType Dot(const Vector2Base& vec01, const Vector2Base& vec02)
	{
		return vec01.x * vec02.x + vec01.y * vec02.y;
	}

	/// <summary>
	/// [EN] Calculate Cross. 
	/// [JP] 外積計算を行う
	/// </summary>
	/// <param name="vec01">[EN] Vector2 [JP] Vector2クラスを入れる</param>
	/// <param name="vec02">[EN] Vector2 [JP] Vector2クラスを入れる</param>
	/// <returns>[EN] If positive, counterclockwise; if negative, clockwise
	///	         [JP] 正なら反時計回り負なら時計回りになる</returns>
	static ValueType Cross(const Vector2Base& vec01, const Vector2Base& vec02)
	{
		return vec01.x * vec02.y - vec01.y * vec02.x;
	}

	/// <summary>
	/// [EN] A function for initializing a vector to 0. [JP] ベクトルを0で初期化する用の関数
	/// </summary>
	/// <returns>[EN] Return of zero vector. [JP] 0ベクトルを返す</returns>
	static Vector2Base Zero()
	{
		return Vector2Base{ 0, 0};
	}

	/// <summary>
	/// [EN] Calculate Length of vector. [JP] ベクトルの長さを作る
	/// </summary>
	/// <returns>[EN] Return Length of vector
	///			 [JP] ベクトルの長さをfloatで返す</returns>
	float Length() const
	{
		return std::sqrtf(static_cast<float>(x * x + y * y));
	}

	/// <summary>
	/// [EN] Calculate Length of Vector. Not use square root.
	/// [JP] ベクトルの長さを計算する。平方根を使わない
	/// </summary>
	/// <returns>[EN] Return Length of square vector
	///			 [JP] ベクトルの距離の二乗を返す</returns>
	float LengthNoSqr() const
	{
		return static_cast<float>(x * x + y * y);
	}

	/// <summary>
	/// [EN] Normalize a vector.
	/// [JP] ベクトルを正規化する
	/// </summary>
	void Normalize()
	{
		float len{ Length() }; // [EN] Length of vector. [JP] ベクトルの長さ

		// [EN] Preventing division by 0 [JP] 0除算の防止
		if (len > 0)
		{
			x = static_cast<ValueType>(static_cast<float>(x) / len);
			y = static_cast<ValueType>(static_cast<float>(y) / len);
		}
	}

	// ====================================== [EN] Calculattions [JP] 演算

	/// <summary>
	/// [EN] Addiotn of vectors 
	/// [JP] ベクター同士の加算
	/// </summary>
	/// <param name="other">[EN] Other vector [JP] ほかのベクトル</param>
	/// <returns>[EN] Vector after addition [JP] 加算後のベクトル</returns>
	Vector2Base operator +(const Vector2Base& other) const
	{
		return Vector2Base{
			x + other.x,
			y + other.y,
		};
	}

	/// <summary>
/// [EN] Subtract of vectors 
/// [JP] ベクター同士の減算
/// </summary>
/// <param name="other">[EN] Other vector [JP] ほかのベクトル</param>
/// <returns>[EN] Vector after subtract [JP] 減算後のベクトル</returns>
	Vector2Base operator -(const Vector2Base& other) const
	{
		return Vector2Base{
			x - other.x,
			y - other.y,
		};
	}

	/// <summary>
	/// [EN] Vector and scalar multiplication 
	/// [JP] ベクトルとスカラーの乗算
	/// </summary>
	/// <param name="value">[EN] scalar [JP] スカラー値</param>
	/// <returns>[EN] Vector after scalar multiplication [JP] スカラー乗算後のベクトル</returns>
	Vector2Base operator *(const ValueType& value) const
	{
		return Vector2Base{
			x * value,
			y * value,
		};
	}


	/// <summary>
	/// [EN] Inverse multiplication of a vector and a scalar
	/// [JP] ベクトルとスカラーの逆乗算
	/// </summary>
	/// <param name="value">[EN] scalar [JP] スカラー値</param>
	/// <returns>[EN] Vector after scalar multiplication [JP] スカラー乗算後のベクトル</returns>
	friend Vector2Base operator *(const ValueType value, const Vector2Base& other)
	{
		return other * value;
	}

	/// <summary>
	/// [EN] Vector and scalar division
	/// [JP] ベクトルとスカラーの除算
	/// </summary>
	/// <param name="value">[EN] scalar [JP] スカラー値</param>
	/// <returns>[EN] Vector after scalar division [JP] スカラー値除算後のベクトル</returns>
	Vector2Base operator /(const ValueType& value) const
	{

		// 0に近い値で割ろうとしたら0ベクトルを返す
		if (std::abs(value) < GAME_EPSILON<ValueType>)
		{
			return Vector2Base{ 0, 0};
		}

		return Vector2Base{
			x / value,
			y / value,
		};
	}

	/// <summary>
	/// [EN] Inverse division of a vector and a scalar
	/// [JP] ベクトルとスカラーの逆除算
	/// </summary>
	/// <param name="value">[EN] scalar [JP] スカラー値</param>
	/// <returns>[EN] Vector after scalar division [JP] スカラー除算後のベクトル</returns>
	friend Vector2Base operator /(const ValueType value, const Vector2Base& other)
	{
		return other / value;
	}


	/// <summary>
	/// [EN] Addition and assignment between vectors.
	/// [JP] ベクトル同士の加算代入
	/// </summary>
	/// <param name="other">[EN] Other vector [JP] ほかのベクトル</param>
	/// <returns>[EN] Self after addition [JP] 加算後の自身</returns>
	Vector2Base& operator +=(const Vector2Base& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}


	/// <summary>
	/// [EN] Substract and assignment between vectors.
	/// [JP] ベクトル同士の減算代入
	/// </summary>
	/// <param name="other">[EN] Other vector [JP] ほかのベクトル</param>
	/// <returns>[EN] Self after substact [JP] 減算後の自身</returns>
	Vector2Base& operator -=(const Vector2Base& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	/// <summary>
	/// [EN] Scalar and vector multiplication assignment.
	/// [JP] スカラーとベクトルの乗算代入
	/// </summary>
	/// <param name="value">[EN] Scalar. [JP] スカラー値</param>
	/// <returns>[EN] Self after multiplication [JP] 乗算後の自身</returns>
	Vector2Base& operator *=(const ValueType& value)
	{
		x *= value;
		y *= value;
		return *this;
	}

	/// <summary>
	/// [EN] Scalar and vector division assignment.
	/// [JP] スカラーとベクトルの除算代入
	/// </summary>
	/// <param name="value">[EN] Scalar. [JP] スカラー値</param>
	/// <returns>[EN] Self after division [JP] 除算後の自身</returns>
	Vector2Base& operator /=(const ValueType& value)
	{
		// 0に近い値で割ろうとしたら0ベクトルを返す
		if (std::abs(value) < GAME_EPSILON<ValueType>)
		{
			x = 0;
			y = 0;
		}
		else
		{
			x /= value;
			y /= value;
		}

		return *this;
	}

	/// <summary>
	/// [EN] Equality comparison between vector classes
	/// [JP] ベクター　クラス同士の等価比較
	/// </summary>
	/// <param name="other">[EN] Other vector [JP] ほかのベクトル </param>
	/// <returns>[EN] If they are equal, it is true. Otherwise, it is false.
	///			 [JP] 等価ならtrue　それ以外はfalse </returns>
	bool operator ==(const Vector2Base& other) const
	{
		// ValueTypeが浮動小数点か整数かによって処理を分ける
		// if constexprはコンパイル時に分岐を確定させるため実行時のif文コストが0となる
		// is_floating_point = テンプレート型がfloatかを調べる
		if constexpr (std::is_floating_point_v<ValueType>)
		{
			// 許容誤差を取得
			constexpr ValueType epsilon = GAME_EPSILON<ValueType>;

			// 差の絶対値がイプシロン以下なら同じとみなす
			return std::abs(x - other.x) <= epsilon &&
				std::abs(y - other.y) <= epsilon;
		}
		// 整数値などの場合は普通の比較を行う
		else
		{
			return x == other.x && y == other.y;
		}
	}

	/// <summary>
	/// [EN] Non-Equality comparison between vector classes
	/// [JP] ベクター　クラス同士の非等価比較
	/// </summary>
	/// <param name="other">[EN] Other vector [JP] ほかのベクトル </param>
	/// <returns>[EN] If they are not equal, it is true. Otherwise, it is false.
	///			 [JP] 非等価ならtrue　それ以外はfalse </returns>
	bool operator !=(const Vector2Base& other)const
	{
		return !(*this == other);
	}

	// [EN] Define coordinates. [JP] 座標を定義
	ValueType x;
	ValueType y;

};

// [EN] Alias of int & float [JP] int型とfloat型のエイリアスを定義する
using Vector2 = Vector2Base<float>;
using Vector2Int = Vector2Base<int>;
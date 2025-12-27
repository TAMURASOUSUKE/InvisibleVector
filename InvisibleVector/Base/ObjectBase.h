#pragma once


/// <summary>
/// [EN] Base class for all obejects in the game. [JP] すべてのオブジェクトの基底クラス
/// </summary>
class ObjectBase
{
public :

	/// <summary> [EN] Constructor : default. [JP] デフォルトコンストラクタ </summary>
	ObjectBase() = default;

	/// <summary>
	/// [EN] Constructor : Initialize position of 2D object. [JP] コンストラクタ : 2Dオブジェクトの位置を初期化する
	/// </summary>
	/// <param name="x"> [EN] PositionX coordinate [JP] X座標 </param>
	/// <param name="y"> [EN] PositionY coordinate [JP] Y座標 </param>
	ObjectBase(float x, float y) : position_x{ x }, position_y{ y } {}

	/// <summary>
	/// [EN] Constructor : Initialize position of 3D object. [JP] コンストラクタ : 3Dオブジェクトの位置を初期化する
 	/// </summary>
	/// <param name="x"> [EN] PositionX coordinate [JP] X座標 </param>
	/// <param name="y"> [EN] PositionY coordinate [JP] Y座標 </param>
	/// <param name="z"> [EN] PositionZ coordinate [JP] Z座標 </param>
	ObjectBase(float x, float y, float z) : position_x(x), position_y(y), position_z(z) {}

	/// <summary>
	/// [EN] Virtual destructor. [JP] 仮想デストラクタ
	/// </summary>
	virtual ~ObjectBase() = default;

	/// <summary>
	/// [EN] Update object state. Called once per frame. [JP] オブジェクトの状態を更新する。1フレームに1回呼ばれる
 	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// [EN] Draw object on the screen. Called once per frame after Update. [JP] 画面にオブジェクトを描画する。Updateの後に1フレームに1回呼ばれる
	/// </summary>
	virtual void Draw() = 0;

protected:
	float position_x{ 0.0f }; // [EN] Position X coordinate. [JP] X座標
	float position_y{ 0.0f }; // [EN] Position Y coordinate. [JP] Y座標
	float position_z{ 0.0f }; // [EN] Position Z coordinate. [JP] Z座標
};
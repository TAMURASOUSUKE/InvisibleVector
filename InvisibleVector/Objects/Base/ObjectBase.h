#pragma once

// [EN] Draw layer [JP] 描画レイヤー
enum class RenderLayer
{
	None,
	World, // 3D
	UI // 2D
};


/// <summary>
/// [EN] The base class for all objects [JP] すべてのオブジェクトの基底クラスとなる
/// </summary>
class ObjectBase
{
public:
	~ObjectBase() = default;


	/// <summary>
	/// [EN] Virtual function. Update every frame. [JP] 毎フレームの更新を行う純粋仮想関数
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// [EN] Physics update functions [JP] 物理更新用関数
	/// </summary>
	virtual void FixedUpdate() {}

	/// <summary>
	/// [EN] Virtual function. Do the drawing. [JP] 描画を行う純粋仮想関数
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// [EN] Get current isActive [JP] 現在の生存状況を取得
	/// </summary>
	/// <returns>[EN] Current isActive [JP] 現在の生存状況</returns>
	bool GetIsActive() const { return isActive; }

	/// <summary>
	/// [EN] Turn off the object live flag [JP] オブジェクトの生存フラグをOffにする
	/// </summary>
	void Destroy() { isActive = false; }

protected:
	ObjectBase(RenderLayer layer) : layer{layer}{}

protected:
	RenderLayer layer{RenderLayer::None};
	// [EN] Active flag. [JP] 生存フラグ
	bool isActive{ true };

};
#pragma once
#include "Vector_Dxlib.h"
#include "ObjectBase.h"
#include "DxLib.h"

class UIObjectBase : public ObjectBase
{
public:
	UIObjectBase() = default;
	~UIObjectBase() = default;

	UIObjectBase(const Vector2 pos) : position{pos}, ObjectBase(RenderLayer::UI) {}

	/// <summary>
	/// [EN] Setting color(RGB) [JP] カラーの設定(RGB)
	/// </summary>
	/// <param name="c">[EN] color [JP] 色/param>
	void SetColor(unsigned int c) { color = c; }

	/// <summary>
	/// [EN] Setting alpha [JP] 透明度設定
	/// </summary>
	/// <param name="a">[EN] alpha value [JP] 透明度</param>
	void SetAlpha(float a) { alpha = a; }

	/// <summary>
	/// [EN] Get visible flag [JP] 表示フラグを取得する
	/// </summary>
	/// <returns>[EN] Visble flag [JP] 表示フラグ</returns>
	bool GetIsVisible() { return isVisible; }

	/// <summary>
	/// [EN] Setting Visible flag [JP] 表示フラグを設定する
	/// </summary>
	/// <param name="visible">[EN] Visible flag [JP] 表示フラグ</param>
	void SetVisble(bool visible) { isVisible = visible; }
	
	/// <summary>
	/// [EN] Get priority value [JP] 優先度を取得する
	/// </summary>
	/// <returns>[EN] priority [JP] 優先度</returns>
	int GetPriority() { return priority; }

	/// <summary>
	/// [EN] Setting priority value [JP] 優先度を設定する
	/// </summary>
	/// <param name="p">[EN] priority [JP] 優先度</param>
	void SetPriority(int p) { priority = p; }

protected:
	/// <summary>
	/// [EN] Combines and returns Dxlib color and transparency
	/// [JP] Dxlibの色と透明度を合成して返す
	/// </summary>
	/// <returns>[EN] Composite color and transparency
	///			 [JP] 合成語の色と透明度</returns>
	unsigned int GetDrawColor() const { return color; }
	/// <summary>
	/// [EN] Get Dxlib alpha values [JP] Dxlibのアルファ値の取得
	/// </summary>
	/// <returns>[EN] Dxlib alpha values [JP] Dxlibのアルファ値</returns>
	int GetDxLibAlpha() const { return static_cast<int>(alpha * 255.0f); }

protected:
	Vector2 position{ Vector2::Zero() }; // [EN] coordinate [JP] 座標
	unsigned int color{ GetColor(255, 255, 255) }; // [EN] color [JP] 色(デフォルトは白)
	int priority{ 0 }; // [EN] Draw priority [JP] 描画優先度
	float alpha{ 0.0f }; // [EN] alpha value [JP] 透明度
	bool isVisible{ true }; // [EN] Display flag [JP] 描画フラグ
};
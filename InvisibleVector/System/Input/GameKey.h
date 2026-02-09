#pragma once
#include <vector>
// ゲームの入力に関する情報をまとめたヘッダ

// 入力名を抽象化しわかりやすくするための名前空間
namespace PadCode {
	// Xboxコントローラーの配置に合わせた定義
	constexpr int South = PAD_INPUT_A; // Aボタン (下)
	constexpr int East = PAD_INPUT_B; // Bボタン (右)
	constexpr int West = PAD_INPUT_C; // Xボタン (左)
	constexpr int North = PAD_INPUT_X; // Yボタン (上)

	constexpr int ShoulderL = PAD_INPUT_Y; // LB (左バンパー)
	constexpr int ShoulderR = PAD_INPUT_Z; // RB (右バンパー)

	constexpr int TriggerL = PAD_INPUT_L;
	constexpr int TriggerR = PAD_INPUT_R;

	constexpr int DPadUp = PAD_INPUT_UP;
	constexpr int DPadDown = PAD_INPUT_DOWN;
	constexpr int DPadLeft = PAD_INPUT_LEFT;
	constexpr int DPadRight = PAD_INPUT_RIGHT;

	constexpr int StickClickL = PAD_INPUT_9;
	constexpr int StickClickR = PAD_INPUT_10;
}

/*
	ゲームで使う入力をまとめた列挙体
	入力デバイスを抽象化して使うため起こすアクションの名前で設定する
*/
enum class GameKey
{
	// MoveMent(GetAxisで使う)
	Up,
	Right,
	Left,
	Down,

	// Actions
	Jump, // Space / B button
	Dash, // 
	Crouch,
	Zoom,

	// meta
	Count, // 要素数
};

/*
	[EN] Structure managing input assignment for one action
	[JP] 1つのアクションに対する入力割り当てを管理する構造体
*/
struct InputBinding
{
	// [EN] List of assigned keyboard keys(Variable length)
	// [JP] 割り当てられたキーボードのキーリスト(可変長)
	std::vector<int> keyboardKeys;
	
	// [EN] Gamepad button mask (bit flag)
	// [JP] ゲームパッドのボタンマスク(ビットフラグ)
	int padButtonMask{ 0 };
};
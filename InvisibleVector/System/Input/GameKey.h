#pragma once
#include <vector>
// ゲームの入力に関する情報をまとめたヘッダ

constexpr int DEFAULT_TRIGGER_DEAD_ZONE = 128;

// 入力名を抽象化しわかりやすくするための名前空間
namespace PadCode {
	// Xboxコントローラーの配置に合わせた定義
	constexpr int SOUTH = PAD_INPUT_A; // Aボタン (下)
	constexpr int EAST = PAD_INPUT_B; // Bボタン (右)
	constexpr int WEST = PAD_INPUT_C; // Xボタン (左)
	constexpr int NORTH = PAD_INPUT_X; // Yボタン (上)

	constexpr int SHOULDER_L = PAD_INPUT_Y; // LB (左バンパー)
	constexpr int SHOULDER_R = PAD_INPUT_Z; // RB (右バンパー)

	constexpr int TRIGGER_L = PAD_INPUT_L;
	constexpr int TRIGGER_R = PAD_INPUT_R;

	constexpr int UP = PAD_INPUT_UP;
	constexpr int DOWN = PAD_INPUT_DOWN;
	constexpr int LEFT = PAD_INPUT_LEFT;
	constexpr int RIGHT = PAD_INPUT_RIGHT;

	constexpr int STICK_CLICK_L = PAD_INPUT_9;
	constexpr int STICK_CLICK_R = PAD_INPUT_10;
}

/*
	ゲームで使う入力をまとめた列挙体
	入力デバイスを抽象化して使うため起こすアクションの名前で設定する
*/
enum class GameAction
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
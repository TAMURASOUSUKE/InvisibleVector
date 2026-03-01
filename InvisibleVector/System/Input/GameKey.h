#pragma once
#include <vector>
// ゲームの入力に関する情報をまとめたヘッダ

constexpr int DEFAULT_TRIGGER_DEAD_ZONE = 128;
constexpr int DEFAULT_STICK_DEAD_ZONE = 5000; // 右スティックのデッドゾーン
constexpr int BLOCK_CONTINUOUS_INPUT = 30; // 連続入力防止入力用フレーム数
constexpr int MAX_STICK_DEAD_ZONE = 30000; // 右スティックの最大デッドゾーン
constexpr int MIN_STICK_DEAD_ZONE = 0; // 右スティックの最小デッドゾーン
constexpr int MAX_TRIGGER_DEAD_ZONE = 250; // トリガーの最大デッドゾーン
constexpr int MIN_TRIGGER_DEAD_ZONE = 0; // トリガーの最小デッドゾーン
constexpr int ADJ_STICK_DEAD_ZONE = 200; // スティックのデッドゾーン調整値
constexpr int ADJ_TRIGGER_DEAD_ZONE = 20; // トリガーのデッドゾーンを調整する値

constexpr float DEFAULT_MOUSE_SENSIBILITY = 0.005f; // マウス感度
constexpr float DEFAULT_STICK_SENSIBILITY = 1.0f; // スティック感度
constexpr float MAX_STICK_SENSIBILITY = 5.0f;
constexpr float MIN_STICK_SENSIBILITY = 0.1f;
constexpr float ADJ_STICK_SENSIBILITY = 0.3f;
constexpr float MAX_XINPUT_VALUE = 32767.0f;
constexpr float ADJ_MOUSE_SENSIBILITY = 0.0005f; // マウス感度を調整するときに使う値

constexpr float MAX_MOUSE_SENSIBILITY = 0.5f; // 最大マウス感度
constexpr float MIN_MOUSE_SEBSIBILITY = 0.001; // 最小マウス感度

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

	constexpr int START = PAD_INPUT_10; // スタートボタン
}

/*
	ゲームで使う入力をまとめた列挙体
	入力デバイスを抽象化して使うため起こすアクションの名前で設定する
*/

namespace ActionID {
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
		Shoot,

		// meta
		Count, // 要素数
	};

	// システム用のアクション
	enum class UI
	{
		/*
			上下左右はカーソル移動などに使う
		*/
		Up,
		Down,
		Left,
		Right,
		Decide, // 決定
		Cancel, // キャンセル
		Pause, // ポーズ
		Count, // 要素数
	};
};

// 現在の入力状態
enum class InputMode
{
	Game, // ゲーム中
	Menu, // メニュー状態 
	Config, // キーコンフィグ設定状態
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
	std::vector<int>padButtonMasks;
};
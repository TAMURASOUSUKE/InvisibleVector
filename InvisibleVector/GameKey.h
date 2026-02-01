#pragma once

// ゲームの入力に関する情報をまとめたヘッダ


/*
	ゲームで使う入力をまとめた列挙体
	入力デバイスを抽象化して使うため起こすアクションの名前で設定する
*/
enum class GameKey
{
	Idle,
	Walk,
	Dash,
	Crounch,
	Jump,
	Zoom,
};
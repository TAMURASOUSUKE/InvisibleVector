#pragma once

/*
	ゲームシステムに関する定数や汎用的な構造体等を定義する
*/

const int SCREEN_WIDTH = 1280; // 横幅
const int SCREEN_HEIGHT = 720; // 縦幅
const int COLOR_DEPTH = 32; // 色の深度
const float CAMERA_NEAR = 1.0f; // 視錐台の上面までの距離
const float CAMERA_FAR = 10000.0f; // 視錐台の底面までの距離


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
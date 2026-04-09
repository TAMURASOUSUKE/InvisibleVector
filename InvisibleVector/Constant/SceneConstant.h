#pragma once
#pragma once

// シーンに関する汎用構造体等を定義する

enum class SceneType
{
	None,
	Debug,
	Title,
	Game,
	GameOver,
	GameClear,
};

enum class SceneStep
{
	None,
	Initialize,
	Execute,
	Terminate,
};
#pragma once

// 画像やモデルの種類などの定義

enum class ResourceUseScene
{
	Common, // 汎用
	Debug, // デバッグシーン
	Title, // タイトル
	Game, // ゲームシーン
	GameOver, // ゲームオーバー
	GameClaer, // ゲームクリア
};

enum class SingleTextureKind
{
	None = -1,
	TitleLogo, // タイトルロゴ
	TitleBackGround, // タイトル背景
	TitleChangeSceneLogo, // タイトル遷移ロゴ
	GameOverLogo, // ゲームオーバーロゴ
	GameOverBackGround, // ゲームオーバー背景
	GameOverChangeSceneLogo,
	GameClearLogo, // ゲームクリアロゴ
	GameClearBackGround, // ゲームクリア背景
	GameClearChangeSceneLogo, // ゲームクリア遷移ロゴ
	Reticle, // レティクル
	FloatingBox, // パーティクルの役割を持たせる箱
	CrackRuleMake, // ひび割れのマスク画像 
	RankA, // スコア
	RankB,
	RankC,
	DebugTestRnak,
};

enum class Model3DKind
{
	None = -1,
	Gun, // 銃
	Ground, // 背景
	Obstacles, // 壁などのステージ
	Target, // 的
};

enum class PSShaderKind
{
	None = -1,
	Crack, // ひび割れ
};

enum class VSShaderKind
{
	None = -1,

};

enum class FontKind
{
	None = -1,
	Meiryo // メイリオ
};


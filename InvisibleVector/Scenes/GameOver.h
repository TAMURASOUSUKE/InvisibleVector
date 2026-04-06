#pragma once
#include "../Objects/Base/ObjectManager.h"
#include "../Scenes/Base/SceneBase.h"

class GameOver : public SceneBase
{
public:
	GameOver() = default;
	~GameOver();

	void Draw() override; // 描画
	SceneType GetType()override { return SceneType::GameOver; } // TypeのGetter

private:
	void Initialize() override; // 最初に行う処理
	void Execute() override; // 実行部分
	void FixedExecute() override; // 毎フレーム行う物理処理
	SceneType Terminate() override; // 抜ける時の処理

private:
	ObjectManager objManager;
};
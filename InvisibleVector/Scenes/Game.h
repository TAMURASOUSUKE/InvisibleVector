#pragma once
#include "../Objects/Base/ObjectManager.h"
#include "../Scenes/Base/SceneBase.h"

class Game : public SceneBase
{
public:
	Game() = default;
	~Game();

	void Draw() override; // 描画
	SceneType GetType() override { return SceneType::Game; } // TypeのGetter

private:
	void Initialize() override; // 最初に行う処理
	void Execute() override; // 実行処理
	void FixedExecute() override; // 毎フレーム行う物理処理
	SceneType Terminate() override; // 抜ける処理

private:
	ObjectManager objManager;
};
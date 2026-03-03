#pragma once
#include "../Objects/World/Player/Player.h" // 本来はNG
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
	SceneType Terminate() override; // 抜ける処理

private:
	ObjectManager objManager;
	Player* player{ nullptr }; // 状態監視のためにポインタを保持(本来はNG)
	GameEndFlagList endResult{ GameEndFlagList::None }; // クリアかオーバーかを保持
};
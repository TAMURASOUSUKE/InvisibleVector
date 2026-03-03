#pragma once
#include "../Objects/Base/ObjectManager.h"
#include "../Scenes/Base/SceneBase.h"

// タイトルシーンを実装する
class Title : public SceneBase
{
public:
	Title() = default;
	~Title();

	void Draw() override; // 描画
	SceneType GetType() override { return SceneType::Title; } // TypeのGetter

private:
	void Initialize() override; // 最初の処理
	void Execute() override; // 毎フレーム行う処理(更新)
	SceneType Terminate() override; // シーンを抜ける処理
private:
	ObjectManager objManager;
};

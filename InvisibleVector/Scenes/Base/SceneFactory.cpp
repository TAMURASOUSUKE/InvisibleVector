#include <memory>
#include "SceneFactory.h"

// 具体をインクルード
#include "../Scenes/Title.h"
#include "../Scenes/Game.h"
#include "../Scenes/GameClear.h"
#include "../Scenes/GameOver.h"


std::map<SceneType, std::function<std::unique_ptr<SceneBase>()>> SceneFactory::creationMap; // 実体


void SceneFactory::Initlialize()
{
	// この関数を呼び出すことでmapに作る予定のシーンを登録していく
	creationMap[SceneType::Title] = []() { return std::make_unique<Title>(); }; // タイトルを作る
	creationMap[SceneType::Game] = []() { return std::make_unique<Game>(); }; // ゲームシーンを作る
	creationMap[SceneType::GameOver] = []() { return std::make_unique<GameOver>(); }; // ゲームオーバーシーンを作る
	creationMap[SceneType::GameClear] = []() { return std::make_unique<GameClear>(); }; // ゲームクリアシーンを作る

}


std::unique_ptr<SceneBase> SceneFactory::CreateScene(SceneType _type)
{
	// 引数に入れたタイプがkeyとして存在するかを探す
	auto it{ creationMap.find(_type) };
	if (it != creationMap.end())
	{
		return it->second(); // Initlializeで設定した関数の結果を返す
	}
	return nullptr;
}

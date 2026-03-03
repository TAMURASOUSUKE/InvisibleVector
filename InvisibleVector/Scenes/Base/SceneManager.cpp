#include <memory>
#include "../Objects/Base/ObjectManager.h"
#include "SceneManager.h"

void SceneManager::SetFirstScene(SceneType _type)
{
	currentScene = SceneFactory::CreateScene(_type);
}


void SceneManager::Main()
{
	if (currentScene == nullptr) return;

	// 通常時の更新
	SceneType type{ currentScene->Main() }; // 各処理を実行しつつそのフレームのSceneTypeを取得
	if (type != currentScene->GetType())
	{
		currentScene = SceneFactory::CreateScene(type); // 新しいシーンを作る
	}
}

void SceneManager::Draw()
{
	// 中身が入っているかを確認して
	if (currentScene == nullptr) return;

	currentScene->Draw();
}
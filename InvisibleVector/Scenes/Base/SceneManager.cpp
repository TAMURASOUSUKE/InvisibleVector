#include <memory>
#include "../Objects/Base/ObjectManager.h"
#include "SceneManager.h"

void SceneManager::SetFirstScene(SceneType _type)
{
	currentScene = SceneFactory::CreateScene(_type);
}


void SceneManager::Update()
{
	if (currentScene == nullptr) return;

	// 通常時の更新
	SceneType type{ currentScene->Update() }; // 各処理を実行しつつそのフレームのSceneTypeを取得


	if (type != currentScene->GetType())
	{
		currentScene = SceneFactory::CreateScene(type); // 新しいシーンを作る
	}
}

void SceneManager::FixedUpdate()
{
	// 物理更新
	currentScene->FixedUpdate();
}

void SceneManager::Draw()
{
	// 中身が入っているかを確認して
	if (currentScene == nullptr) return;

	currentScene->Draw();
}
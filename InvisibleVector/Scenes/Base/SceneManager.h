#pragma once
#include <memory>
#include <vector>
#include <map>
#include "GameConstant.h"
#include "../ResourceManagement/ResourceManager.h"
#include "SceneFactory.h"
#include "SceneBase.h"
// シーンを管理するクラス
class SceneManager
{
public:
	// シングルトン化
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	~SceneManager()
	{
		ResourceManager::Instance().Delete(ResourceUseScene::Common); // 汎用画像を削除する

	}

	void SetFirstScene(SceneType _type); // 最初のシーンを設定する補助関数

	void Update();
	void FixedUpdate();
	void Draw();
	
private:
	SceneManager()
	{
		SceneFactory::Initlialize(); // 使用するシーンの登録
		ResourceManager::Instance().Load(ResourceUseScene::Common); // 汎用画像を先にLoadしておく

	}



private:
	std::unique_ptr<SceneBase> currentScene;
	SceneType newSceneType{ SceneType::None };

};
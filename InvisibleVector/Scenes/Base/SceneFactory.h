#pragma once
#include <memory>
#include <map>
#include <functional>
#include "SceneBase.h"

// シーンを作成するためのクラス
class SceneFactory
{
public:
	static void Initlialize(); // 作るシーンを設定する初期化関数
	static std::unique_ptr<SceneBase> CreateScene(SceneType _type); // シーンを呼ぶだけで生成できるようにする抽象関数

private:
	static std::map<SceneType, std::function<std::unique_ptr<SceneBase>()>> creationMap; // key = SceneType value = 具体シーンのインスタンスを返す関数のmap
};
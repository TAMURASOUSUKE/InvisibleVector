#include "DxLib.h"
#include "../CollisionManagement/CollisionManager.h"
#include "../ResourceManagement/ResourceManager.h"
#include "../Objects/Base/ObjectFactory.h"
#include "Game.h"

Game::~Game()
{
	ResourceManager::Instance().Delete(ResourceUseScene::Game);
	CollisionManager::Instance().Clear(); // 破棄
}

void Game::Initialize()
{

	ResourceManager::Instance().Load(ResourceUseScene::Game);

	ObjectFactory::BindManager(objManager); // シーンにバインド
	objManager.SortUI(); // UIのソート

	currentStep = SceneStep::Execute;
}

void Game::Execute()
{

	objManager.Update();

	CollisionManager::Instance().Update();

	currentStep = SceneStep::Terminate;
}

void Game::FixedExecute()
{
	objManager.FixedUpdate();
}

SceneType Game::Terminate()
{



	// if (endResult == GameEndFlagList::GameOver)
	//{
	//	return SceneType::GameOver;
	//}
	//else if (endResult == GameEndFlagList::GameClaer)
	//{
	//	return SceneType::GameClear;
	//}

#ifdef _DEBUG


	// デバッグ時は1ならClearへ
	if (CheckHitKey(KEY_INPUT_1))
	{
		return SceneType::GameClear;
	}
	// 2ならOverへ
	else if (CheckHitKey(KEY_INPUT_2))
	{
		return SceneType::GameOver;
	}

#endif _DEBUG

	return SceneType::Game;
}

void Game::Draw()
{

	objManager.Draw();
#ifdef _DEBUG
	Vector2 debugPos{ 10.0f, 20.0f }; // デバッグ用描画ポジション
	float lineHigh{ 20.0f }; // 行間
	// デバッグ用描画
	DrawString(debugPos.x, debugPos.y, "GameSceneです。", white);
	debugPos.y += lineHigh;
	DrawFormatString(debugPos.x, debugPos.y, white, "現在のシーンは%s", currentStep == SceneStep::Terminate ? "Terminateです。" : "Terminate以外です");

#endif // _DEBUG
}


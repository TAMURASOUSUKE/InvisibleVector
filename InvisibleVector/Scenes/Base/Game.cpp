#include "DxLib.h"
#include "../ScoreManagement/ScoreManager.h"
#include "../Collision/CollisionManager.h"
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
	ScoreManager::Instance().Reset(); // スコアのリセット

	ResourceManager::Instance().Load(ResourceUseScene::Game);

	ObjectFactory::BindManager(objManager); // シーンにバインド

	player = ObjectFactory::CreatePlayer(Vector3(0.0f, 60.0f, -180.0f), PLAYER_SIZE, ResourceManager::Instance().GetResourceHandle(Model3DKind::Gun)); // Playerの生成(カメラに渡すので変数化)
	ObjectFactory::CreateCamera(Vector3(0.0f, 60.0f, -180.0f), player);
	ObjectFactory::CreateReticle(1000, ResourceManager::Instance().GetResourceHandle(SingleTextureKind::Reticle)); // レティクルの生成

	ObjectFactory::CreateStage(); // ステージはまとめたクラスを実体化しそこから更新、描画を行う

	objManager.SortUI(); // UIのソート

	currentStep = SceneStep::Execute;
}

void Game::Execute()
{

	objManager.Update();

	CollisionManager::Instance().Update();

	if (player->GetGameEndFlag())
	{
		endResult = GameEndFlagList::GameOver;
		currentStep = SceneStep::Terminate; // 終了処理へ移行
	}
	else if (player->GetPosition().z >= 810)
	{
		endResult = GameEndFlagList::GameClaer;
		currentStep = SceneStep::Terminate; // 終了処理へ移行
	}

}

SceneType Game::Terminate()
{



	if (endResult == GameEndFlagList::GameOver)
	{
		return SceneType::GameOver;
	}
	else if (endResult == GameEndFlagList::GameClaer)
	{
		return SceneType::GameClear;
	}

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


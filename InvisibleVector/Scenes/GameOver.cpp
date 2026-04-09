#include "DxLib.h"
#include "GameConstant.h"
#include "../ResourceManagement/ResourceManager.h"
#include "../Objects/Base/ObjectFactory.h"
#include "GameOver.h"

GameOver::~GameOver()
{
	ResourceManager::Instance().Delete(ResourceUseScene::GameOver); // リソースの削除
}

void GameOver::Initialize()
{
	ResourceManager::Instance().Load(ResourceUseScene::GameOver); // リソースのロード

	ObjectFactory::BindManager(objManager); // シーンにバインド


	objManager.SortUI(); // UIのソート

	currentStep = SceneStep::Execute;
}

void GameOver::Execute()
{
	objManager.Update();

#ifdef _DEBUG

	if (CheckHitKey(KEY_INPUT_O))
	{
		currentStep = SceneStep::Terminate;
	}

#endif // _DEBUG
}

void GameOver::FixedExecute()
{
	objManager.FixedUpdate();
}

SceneType GameOver::Terminate()
{


	return SceneType::Title; // タイトルシーンへ
}

void GameOver::Draw()
{
	objManager.Draw();

#ifdef _DEBUG


	Vector2 debugPos{ 10.0f, 20.0f }; // デバッグ用描画ポジション
	// デバッグ用描画
	DrawString(debugPos.x, debugPos.y, "GameOverSceneです。", white);

#endif // _DEBUG
}
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

	ObjectFactory::CreateBackGround(Vector2(Vector2::Zero()), 10, ResourceManager::Instance().GetResourceHandle(SingleTextureKind::GameOverBackGround));
	ObjectFactory::CreateChangeSceneLogo(Vector2(GAMEOVER_CHANGE_SCENE_LOGO_X, GAMEOVER_CHANGE_SCENE_LOGO_Y), 100, ResourceManager::Instance().GetResourceHandle(SingleTextureKind::GameOverChangeSceneLogo));
	ObjectFactory::CreateLogo(Vector2(0.0f, -310.0f), 150, ResourceManager::Instance().GetResourceHandle(SingleTextureKind::GameOverLogo));
	ObjectFactory::CreateFloatingBox(Vector2(300.0f, 100.0f), Vector2(1600.0f, 400.0f), 120, ResourceManager::Instance().GetResourceHandle(SingleTextureKind::FloatingBox));

	ObjectFactory::CreateTestUIB(Vector2(40.0f, 50.0f), GetColor(255, 0, 255)); // TestB : UIを生成
	ObjectFactory::CreateTestWorldB(Vector3(80.0f, 80.0f, 0.0f)); // TestB : Worldを作成

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
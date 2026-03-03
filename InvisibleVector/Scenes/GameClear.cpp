#include "DxLib.h"
#include "GameConstant.h"
#include "../ResourceManagement/ResourceManager.h"
#include "../ScoreManagement/ScoreManager.h"
#include "../Objects/Base/ObjectFactory.h"
#include "../Input/InputManager.h"
#include "GameClear.h"

GameClear::~GameClear()
{
	ResourceManager::Instance().Delete(ResourceUseScene::GameClaer); // リソースの削除
}

void GameClear::Initialize()
{
	ResourceManager::Instance().Load(ResourceUseScene::GameClaer); // リソースのロード

	ScoreManager::Instance().AddClearBonus(); // ゲームクリアなのでボーナスを付ける

	ObjectFactory::BindManager(objManager);

	ObjectFactory::CreateBackGround(Vector2(Vector2::Zero()), 10, ResourceManager::Instance().GetResourceHandle(SingleTextureKind::GameClearBackGround));
	ObjectFactory::CreateChangeSceneLogo(Vector2(GAMECLEAR_CHANGE_SCENE_LOGO_X, GAMECLEAR_CHANGE_SCENE_LOGO_Y), 100, ResourceManager::Instance().GetResourceHandle(SingleTextureKind::GameClearChangeSceneLogo));
	ObjectFactory::CreateLogo(Vector2(0.0f, -310.0f), 150, ResourceManager::Instance().GetResourceHandle(SingleTextureKind::GameClearLogo));
	ObjectFactory::CreateFloatingBox(Vector2(300.0f, 100.0f), Vector2(1600.0f, 400.0f), 120, ResourceManager::Instance().GetResourceHandle(SingleTextureKind::FloatingBox));


	ObjectFactory::CreateScore(
		Vector2(0.0f, 650.0f),
		Vector2(200.0f, 400.0f),
		400,
		ResourceManager::Instance().GetResourceHandle(SingleTextureKind::RankA),
		ResourceManager::Instance().GetResourceHandle(SingleTextureKind::RankB),
		ResourceManager::Instance().GetResourceHandle(SingleTextureKind::RankC),
		ResourceManager::Instance().GetResourceHandle(FontKind::Meiryo)
	);

	objManager.SortUI(); // UIのソート

	currentStep = SceneStep::Execute;
}

void GameClear::Execute()
{
	objManager.Update();


	if (GetMouseInput() & MOUSE_INPUT_LEFT || GetMouseInput() & MOUSE_INPUT_RIGHT || InputManager::Instance().GetButtonDown(ActionID::GameAction::Jump))
	{
		currentStep = SceneStep::Terminate;
	}
}

SceneType GameClear::Terminate()
{


	return SceneType::Title; // タイトルシーンへ
}

void GameClear::Draw()
{
	objManager.Draw();

#ifdef _DEBUG


	Vector2 debugPos{ 10.0f, 20.0f }; // デバッグ用描画ポジション
	// デバッグ用描画
	DrawString(debugPos.x, debugPos.y, "GameClearSceneです。", white);

#endif // _DEBUG
}
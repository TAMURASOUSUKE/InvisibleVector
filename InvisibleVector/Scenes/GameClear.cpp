#include "DxLib.h"
#include "GameConstant.h"
#include "../ResourceManagement/ResourceManager.h"
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

	ObjectFactory::BindManager(objManager);


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

void GameClear::FixedExecute()
{
	objManager.FixedUpdate();
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
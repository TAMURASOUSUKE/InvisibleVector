#include "Dxlib.h"
#include "../ResourceManagement/ResourceManager.h"
#include "../Objects/Base/ObjectFactory.h"
#include "../Input/InputManager.h"
#include "Title.h"

Title::~Title()
{
	ResourceManager::Instance().Delete(ResourceUseScene::Title); // タイトル用にロードしたリソースの削除
}

void Title::Initialize()
{
	ResourceManager::Instance().Load(ResourceUseScene::Title); // 必要なリソースのロード

	ObjectFactory::BindManager(objManager); // シーンのObjManagerをバインド


	objManager.SortUI(); // UIをソート

	currentStep = SceneStep::Execute;
}

void Title::Execute()
{
	objManager.Update();

	if (GetMouseInput() & MOUSE_INPUT_LEFT || GetMouseInput() & MOUSE_INPUT_RIGHT || InputManager::Instance().GetButtonDown(ActionID::GameAction::Jump))
	{
		currentStep = SceneStep::Terminate;
	}
}


void Title::FixedExecute()
{
	objManager.FixedUpdate();
}


SceneType Title::Terminate()
{


	return SceneType::Game; // 次はGameSceneへ
}

void Title::Draw()
{
	objManager.Draw();


#ifdef _DEBUG

	Vector2 debugPos{ 10.0f, 20.0f }; // デバッグ用描画ポジション
	// デバッグ用描画
	DrawString(debugPos.x, debugPos.y, "TitleSceneです。", white);

#endif // _DEBUG

}
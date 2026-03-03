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

	// タイトルロゴの位置からタイトルロゴのサイズ端までのランダム制限にしてハンドルを取得している(優先度100)
	ObjectFactory::CreateFloatingBox(Vector2(TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y), Vector2(TITLE_FLOATING_WIDTH, TITLE_FLOATING_HEIGHT), 100, ResourceManager::Instance().GetResourceHandle(SingleTextureKind::FloatingBox));
	ObjectFactory::CreateLogo(Vector2(TITLE_LOGO_POS_X, TITLE_LOGO_POS_Y), 150, ResourceManager::Instance().GetResourceHandle(SingleTextureKind::TitleLogo));
	ObjectFactory::CreateBackGround(Vector2::Zero(), 10, ResourceManager::Instance().GetResourceHandle(SingleTextureKind::TitleBackGround));
	ObjectFactory::CreateChangeSceneLogo(Vector2(TITLE_CHANGE_SCENE_LOGO_X, TITLE_CHANGE_SCENE_LOGO_Y), 160, ResourceManager::Instance().GetResourceHandle(SingleTextureKind::TitleChangeSceneLogo));



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
#include "DxLib.h"
#include "../CollisionManagement/CollisionManager.h"
#include "../ResourceManagement/ResourceManager.h"
#include "../Objects/Base/ObjectFactory.h"
#include "Debug.h"

Debug::~Debug()
{
	ResourceManager::Instance().Delete(ResourceUseScene::Debug);
	CollisionManager::Instance().Clear(); // 破棄
}

void Debug::Initialize()
{

	ResourceManager::Instance().Load(ResourceUseScene::Debug);

	ObjectFactory::BindManager(objManager); // シーンにバインド
	objManager.SortUI(); // UIのソート

	ObjectFactory::CreateDebugGround(Vector3(-180.0f, 0.0f, 40.0f), Vector3::Zero(), Vector3(1.0f, 1.0f, 1.0f), true);
	ObjectFactory::CreateDebugObject(Vector3(-80.0f, 90.0f, 100.0f), Vector3::Zero(), Vector3(1.0f, 1.0f, 1.0f), true);

	currentStep = SceneStep::Execute;
}

void Debug::Execute()
{
	objManager.Update();

	CollisionManager::Instance().Update();

	if (CheckHitKey(KEY_INPUT_F6))
	{
		currentStep = SceneStep::Terminate;
	}
}

void Debug::FixedExecute()
{
	objManager.FixedUpdate();
}

SceneType Debug::Terminate()
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
	if (CheckHitKey(KEY_INPUT_F1))
	{
		return SceneType::Game;
	}

#endif _DEBUG

	return SceneType::Debug;
}

void Debug::Draw()
{

	objManager.Draw();

	Vector2 debugStringPos{ 10.0f, 10.0f };
	DrawString(debugStringPos.x, debugStringPos.y, "ここはDebugSceneです",GetColor(255, 255, 255), GetColor(255, 255, 255));

}


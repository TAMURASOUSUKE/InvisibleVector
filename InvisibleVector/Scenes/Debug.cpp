#include "DxLib.h"
#include <cmath>
#include "../CollisionManagement/CollisionManager.h"
#include "../ResourceManagement/ResourceManager.h"
#include "../PhysicsManagement/PhysicsManager.h"
#include "../Objects/Base/ObjectFactory.h"
#include "../System/Input/InputManager.h"
#include "Debug.h"

Debug::~Debug()
{
	ResourceManager::Instance().Delete(ResourceUseScene::Debug);
	CollisionManager::Instance().Clear(); // 破棄
    PhysicsManager::Instance().Clear();
}

void Debug::Initialize()
{

	ResourceManager::Instance().Load(ResourceUseScene::Debug);

	ObjectFactory::BindManager(objManager); // シーンにバインド
	objManager.SortUI(); // UIのソート

	ObjectFactory::CreateDebugGround(Vector3(-180.0f, 0.0f, 40.0f), Vector3::Zero(), Vector3(1.0f, 1.0f, 1.0f), true, Vector3(300.0f, 20.0f, 300.0f)); // 床
	ObjectFactory::CreateDebugGround(Vector3(-180.0f, 20.0f, 340.0f), Vector3::Zero(), Vector3(1.0f, 1.0f, 1.0f), true, Vector3(300.0f, 100.0f, 10.0f)); // 壁
	ObjectFactory::CreateDebugObject(Vector3(-80.0f, 90.0f, 100.0f), Vector3::Zero(), Vector3(1.0f, 1.0f, 1.0f), true);

	currentStep = SceneStep::Execute;
}

void Debug::Execute()
{
     // 回転（マウス / 右スティック）
    Vector2 camInput{ InputManager::Instance().GetCameraAxis() };
    cameraYaw += camInput.x * cameraRotateSpeed;
    cameraPitch += camInput.y * cameraRotateSpeed;

    // 上下の回転を制限（真上・真下を超えないようにする）
    if (cameraPitch > 1.2f)  cameraPitch = 1.2f;
    if (cameraPitch < -1.2f) cameraPitch = -1.2f;

    // カメラの向いている方向を計算
    Vector3 forward
    {
        std::cos(cameraPitch) * std::sin(cameraYaw),
        std::sin(cameraPitch),
        std::cos(cameraPitch) * std::cos(cameraYaw)
    };

    // 右方向（forwardとY軸の外積）
    Vector3 right
    {
        std::cos(cameraYaw),
        0.0f,
        -std::sin(cameraYaw)
    };

    // 移動（WASD / 左スティック）
    Vector2 moveInput{ InputManager::Instance().GetAxis() };
    cameraPos += forward * (moveInput.y * cameraMoveSpeed);
    cameraPos += right * (moveInput.x * cameraMoveSpeed);

    // 上下移動（ジャンプ/しゃがみキーで代用）
    if (InputManager::Instance().GetButtonStay(ActionID::GameAction::Jump))
    {
        cameraPos.y += cameraMoveSpeed;
    }
    if (InputManager::Instance().GetButtonStay(ActionID::GameAction::Crouch))
    {
        cameraPos.y -= cameraMoveSpeed;
    }

    // 注視点を更新
    cameraTarget = cameraPos + forward;

    // ===== 既存の処理 =====
    objManager.Update();

    CollisionManager::Instance().Update();

    if (CheckHitKey(KEY_INPUT_F6))
    {
        currentStep = SceneStep::Terminate;
    }
}

void Debug::FixedExecute()
{
	PhysicsManager::Instance().FixedUpdate(); // 物理計算
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
    // カメラ適用
    SetCameraPositionAndTarget_UpVecY(ToDxLib(cameraPos), ToDxLib(cameraTarget));

	objManager.Draw();

	Vector2 debugStringPos{ 10.0f, 10.0f };
	DrawString(debugStringPos.x, debugStringPos.y, "ここはDebugSceneです",GetColor(255, 255, 255), GetColor(255, 255, 255));

}


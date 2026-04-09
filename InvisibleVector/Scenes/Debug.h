#pragma once
#include "../Objects/Base/ObjectManager.h"
#include "../Scenes/Base/SceneBase.h"

class Debug : public SceneBase
{
public:
	Debug() = default;
	~Debug();

	void Draw() override; // 描画
	SceneType GetType() override { return SceneType::Debug; } // TypeのGetter

private:
	void Initialize() override; // 最初に行う処理
	void Execute() override; // 実行処理
	void FixedExecute() override; // 毎フレーム行う物理処理
	SceneType Terminate() override; // 抜ける処理

private:
	ObjectManager objManager;

	// デバッグ用
	// デバッグカメラ
	Vector3 cameraPos{ -80.0f, 150.0f, -200.0f };  // カメラの位置
	Vector3 cameraTarget{ -80.0f, 50.0f, 100.0f };  // 注視点
	float cameraYaw{ 0.0f };    // 水平回転（ラジアン）
	float cameraPitch{ 0.0f };  // 垂直回転（ラジアン）
	float cameraMoveSpeed{ 3.0f };
	float cameraRotateSpeed{ 0.05f };
};
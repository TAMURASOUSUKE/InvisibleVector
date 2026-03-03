#pragma once
#include "../Objects/Base/ObjectManager.h"
#include "../Scenes/Base/SceneBase.h"

class GameClear : public SceneBase
{
public:
	GameClear() = default;
	~GameClear();

	void Draw() override;
	SceneType GetType()override { return SceneType::GameClear; }

private:
	void Initialize() override;
	void Execute()override;
	SceneType Terminate() override;

private:
	ObjectManager objManager;
};
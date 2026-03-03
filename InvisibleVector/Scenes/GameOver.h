#pragma once
#include "../Objects/Base/ObjectManager.h"
#include "../Scenes/Base/SceneBase.h"

class GameOver : public SceneBase
{
public:
	GameOver() = default;
	~GameOver();

	void Draw() override; // •`‰æ
	SceneType GetType()override { return SceneType::GameOver; } // Type‚ÌGetter

private:
	void Initialize() override; // Å‰‚És‚¤ˆ—
	void Execute() override; // Às•”•ª
	SceneType Terminate() override; // ”²‚¯‚é‚Ìˆ—

private:
	ObjectManager objManager;
};
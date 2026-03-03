#pragma once
#include "../Objects/Base/ObjectManager.h"
#include "../Scenes/Base/SceneBase.h"

class Game : public SceneBase
{
public:
	Game() = default;
	~Game();

	void Draw() override; // •`‰æ
	SceneType GetType() override { return SceneType::Game; } // Type‚ÌGetter

private:
	void Initialize() override; // Å‰‚És‚¤ˆ—
	void Execute() override; // Àsˆ—
	SceneType Terminate() override; // ”²‚¯‚éˆ—

private:
	ObjectManager objManager;
};
#include "GameConstant.h"
#include "ObjectFactory.h"

// 具体をインクルード
#include "../Objects/World/Player/Player.h"
#include "../Objects//World/Camera/Camera.h"
#include "../Objects/World/Stage/Stage.h"
#include "../Objects/UI/Game/Reticle.h"
#include "../Objects/UI/FloatingBox.h"
#include "../Objects/UI/ChangeSceneLogo.h"
#include "../Objects/UI/Logo.h"
#include "../Objects/UI/BackGround.h"
#include "../Objects/UI/Score.h"
#include "../Objects/World/Bullet/Bullet.h"


// static変数の実態を作る
ObjectManager* ObjectFactory::targetManager{ nullptr };


void ObjectFactory::BindManager(ObjectManager& _targetManager)
{
	targetManager = &_targetManager; // 引数で受け取ったObjectManagerのインスタンスをFactoryに登録
}


void ObjectFactory::CreateFloatingBox(Vector2 _randomStartPos, Vector2 _randomEndPos, unsigned int _priority, int _handle)
{
	// 設定した数分生成する
	for (int i = 0; i < FLOATING_BOX_DRAW_VALUE; i++)
	{
		std::unique_ptr<UIObjectBase> obj{ std::make_unique<FloatingBox>(_randomStartPos, _randomEndPos, _priority, _handle) };
		targetManager->RegisterUI(std::move(obj));
	}
}

void ObjectFactory::CreateCamera(Vector3 _position, Player* _player)
{
	std::unique_ptr<WorldObjectBase> obj{ std::make_unique<Camera>(_position, _player) }; // 実態を作る
	targetManager->RegisterWorld(std::move(obj)); // 所有権を移動
}

Player* ObjectFactory::CreatePlayer(Vector3 _position, float _radius, int _handle)
{
	std::unique_ptr<Player> obj = std::make_unique<Player>(_position, _radius, _handle); // 実態を作る
	Player* ptr = obj.get();
	targetManager->RegisterWorld(std::move(obj)); // 所有権を移動
	return ptr;
}

void ObjectFactory::CreateStage()
{
	std::unique_ptr<WorldObjectBase> obj{ std::make_unique<Stage>() }; // 実体を作る
	targetManager->RegisterWorld(std::move(obj));
}

void ObjectFactory::CreateBullet(Vector3 _pos, Vector3 _start_pos, Vector3 _direction_vec, float _radius)
{
	std::unique_ptr<WorldObjectBase> obj{ std::make_unique<Bullet>(_pos, _start_pos, _direction_vec, _radius) }; // 実態を作る
	targetManager->TemporaryRegister(std::move(obj));
}

void ObjectFactory::CreateChangeSceneLogo(Vector2 _pos, unsigned int _priority, int _handle)
{
	std::unique_ptr<UIObjectBase> obj{ std::make_unique<ChangeSceneLogo>(_pos, _priority, _handle) };
	targetManager->RegisterUI(std::move(obj));
}

void ObjectFactory::CreateLogo(Vector2 _pos, unsigned int _priority, int _handle)
{
	std::unique_ptr<UIObjectBase> obj{ std::make_unique<Logo>(_pos, _priority, _handle) };
	targetManager->RegisterUI(std::move(obj));
}

void ObjectFactory::CreateBackGround(Vector2 _pos, unsigned int _priority, int _handle)
{
	std::unique_ptr<UIObjectBase> obj{ std::make_unique<BackGround>(_pos, _priority, _handle) };
	targetManager->RegisterUI(std::move(obj));
}

void ObjectFactory::CreateReticle(unsigned int _priority, int _handle)
{
	std::unique_ptr<UIObjectBase> obj{ std::make_unique<Reticle>(_priority, _handle) };
	targetManager->RegisterUI(std::move(obj));
}


void ObjectFactory::CreateScore(Vector2 _graphPos, Vector2 _fontPos, unsigned int _priority, int _rankAHandle, int _rankBHandle, int _rankCHandle, int _fontHandle)
{
	std::unique_ptr<UIObjectBase> obj{ std::make_unique<Score>(_graphPos, _fontPos, _priority, _rankAHandle, _rankBHandle, _rankCHandle, _fontHandle) };
	targetManager->RegisterUI(std::move(obj));
}
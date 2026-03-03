#include "GameConstant.h"
#include "ObjectFactory.h"

// 具体をインクルード


// static変数の実態を作る
ObjectManager* ObjectFactory::targetManager{ nullptr };


void ObjectFactory::BindManager(ObjectManager& _targetManager)
{
	targetManager = &_targetManager; // 引数で受け取ったObjectManagerのインスタンスをFactoryに登録
}



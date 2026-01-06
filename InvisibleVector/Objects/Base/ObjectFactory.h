#pragma once
#include "ObjectManager.h"

/// <summary>
/// [EN] Create the Object [JP] オブジェクトの生成を行うクラス
/// </summary>
class ObjectFactory
{
public:
	ObjectFactory(ObjectManager* objectManager) : objectManager{objectManager} {}
private:
	ObjectManager* objectManager;
};
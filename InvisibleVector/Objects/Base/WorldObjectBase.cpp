#include "WorldObjectBase.h"

void WorldObjectBase::UpdatePhysics(float deltaTime)
{
	velocity += acceleration * deltaTime; // [EN] Calculate velocity [JP] 速度計算
	position += velocity * deltaTime; // [EN] Calculate position [JP] 位置計算
}
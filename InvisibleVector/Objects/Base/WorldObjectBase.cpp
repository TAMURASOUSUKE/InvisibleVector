#include "WorldObjectBase.h"

void WorldObjectBase::FixedUpdate(float fixedDeltaTime)
{
	velocity += acceleration * fixedDeltaTime; // [EN] Calculate velocity [JP] 速度計算
	position += velocity * fixedDeltaTime; // [EN] Calculate position [JP] 位置計算
}
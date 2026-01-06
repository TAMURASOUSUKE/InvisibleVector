#include "Time.h"
#include "WorldObjectBase.h"

void WorldObjectBase::FixedUpdate()
{
	velocity += acceleration * Time::FixedDeltaTime(); // [EN] Calculate velocity [JP] 速度計算
	position += velocity * Time::FixedDeltaTime(); // [EN] Calculate position [JP] 位置計算
}
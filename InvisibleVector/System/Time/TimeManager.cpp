#include "SystemConstant.h"
#include "TimeManager.h"

const float* TimeManager::deltaTimePtr = &SEAF_ZERO;
const float* TimeManager::fixedDeltaTimePtr = &SEAF_ZERO;
const float* TimeManager::fpsPtr = &SEAF_ZERO;
const float* TimeManager::alphaPtr = &SEAF_ZERO;
#include "SystemConstant.h"
#include "Time.h"

const float* Time::deltaTimePtr = &DUMMY_ZERO;
const float* Time::fixedDeltaTimePtr = &DUMMY_ZERO;
const float* Time::fpsPtr = &DUMMY_ZERO;
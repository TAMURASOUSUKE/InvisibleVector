#pragma once
#include "Vector_Dxlib.h"
#include "ObjectBase.h"

class UIObjectBase : public ObjectBase
{
public:
	UIObjectBase() : ObjectBase(RenderLayer::UI) {}
};
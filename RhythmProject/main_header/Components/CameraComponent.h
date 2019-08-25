#pragma once
#include "../../main_header/Components/Component.h"

class CameraComponent : public Component
{
public:
	CameraComponent(Actor* owner, int updateOrder = 200);
};
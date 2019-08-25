#pragma once
#include "../Actors/Actor.h"

class ObjectSampler : public Actor
{
public:
	ObjectSampler(class Game* game);
	~ObjectSampler();

	void SetCollision() { MV1SetupCollInfo(GetModelHandle(), 0, 16, 16, 16); }
	void RefreshCollision() { MV1RefreshCollInfo(GetModelHandle(), 0); }
};
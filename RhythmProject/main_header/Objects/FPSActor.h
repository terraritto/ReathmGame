#pragma once
#include "../Actors/Actor.h"
#include "DxLib.h"
#include <memory>

class FPSActor:public Actor
{
public:
	FPSActor(class Game* game);
	void UpdateActor(float deltaTime) override;
	void ActorInput(const struct InputState& keys) override;
private:
	class MoveComponent* mMoveComp;
	class FPSCamera* mCameraComp;
	class Actor* mFPSModel;
	int mouseX, mouseY;
};
#pragma once
#include "../../main_header/Actors/Actor.h"
#include "define.h"
#include <array>

class Player : public Actor
{
public:
	Player(class Game* game, class MainScreen* main);
	~Player();
	void UpdateActor(float deltaTime) override;
	void ActorInput(const struct InputState& keyState) override;
private:
	const int mSpeed = 10;
public:
	//notes—p
	std::array<LONGLONG,3> mInputTime;
	std::array<bool,3> mIsInput;
	//LongNotes—p
	std::array<LONGLONG, 3> mInputLongTime;
	std::array<bool, 3> mIsLongInput;
};
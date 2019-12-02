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
	std::array<LONGLONG,3> mInputTimeRight;
	std::array<bool,3> mIsInputRight;
	std::array<LONGLONG, 3> mInputTimeLeft;
	std::array<bool, 3> mIsInputLeft;
	//LongNotes—p
	std::array<LONGLONG, 9> mInputLongTimeLeft;
	std::array<bool, 9> mIsLongInputLeft;
	std::array<LONGLONG, 9> mInputLongTimeRight;
	std::array<bool, 9> mIsLongInputRight;
};
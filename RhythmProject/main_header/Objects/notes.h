#pragma once
#include "../../main_header/Actors/Actor.h"
#include "define.h"

class Notes : public Actor
{
public:
	Notes(class Game* game);
	~Notes();
	void UpdateActor(float deltaTime) override;
	void ActorInput(const struct InputState& keyState) override;

	void SetArrivalTime(LONGLONG time) { ArrivalTime = time; }
	void SetLane(int lane) { mLane = lane; }
	void SetColor(char color);
private:
	int mLane; 
	LONGLONG ArrivalTime; 
	EColor mColor;
};
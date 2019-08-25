#pragma once
#include "../Actors/Actor.h"
#include <array>
#include "define.h"

class LongNotes : public Actor
{
public:
	LongNotes(class Game* game);
	~LongNotes();
	void UpdateActor(float deltaTime) override;

	void SetArrivalTime(std::array<LONGLONG,2> time) { mArrivalTime = time; }
	void SetLane(int lane) { mLane = lane; }
	void SetColor(char color);
	void SetScale();
private:
	int mSpeed;
	int mLane;
	float mScale;
	std::array<LONGLONG,2> mArrivalTime;
	EColor mColor;

	bool mIsUpdate; //object‚ÌXV‚ªI‚í‚Á‚Ä‚é‚©?
};
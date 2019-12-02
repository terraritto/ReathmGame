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

	//input
	//keyboard is left or right?
	EKeyboardDirection mDir;
	// is push?
	bool mIsPush;
	bool mIsRelease;
	
	//effect
	bool mIseffect; //effectÇÕäÑÇËìñÇƒÇΩÇ©ÅH
	int mEffectIndex;
};
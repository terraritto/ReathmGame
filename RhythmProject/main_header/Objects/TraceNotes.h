#pragma once
#include "../Actors/Actor.h"

class TraceNotes : public Actor
{
public:
	TraceNotes(class Game* game);
	~TraceNotes();
	void UpdateActor(float deltaTime) override;
	void Draw() override;
	
	void SetArrivalTime(std::array<LONGLONG, 2> time) { mArrivalTime = time; }
	void SetLane(std::array<int, 2> lane) { mLane = lane; }
	void SetScale();
private:
	int mSpeed;
	std::array<int, 2> mLane;
	float mScale;
	float mRad;
	std::array<LONGLONG, 2> mArrivalTime;
};
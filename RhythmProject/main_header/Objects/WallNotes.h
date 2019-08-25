#pragma once
#include "../Actors/Actor.h"
#include <array>

class WallNotes : public Actor
{
public:
	WallNotes(class Game* game);
	~WallNotes();
	void UpdateActor(float deltaTime) override;
	void Draw() override;

	void SetArrivalTime(std::array<LONGLONG, 2> time) { mArrivalTime = time; }
	void SetLane(int lane) { mLane = lane; }
	void SetScale();
	void SetHeight(float height) { mHeight = height; }
private:
	int mSpeed;
	int mLane; 
	float mScale;
	std::array<LONGLONG, 2> mArrivalTime;

	//wall specific
	float mHeight; //ãƒŒ[ƒ“‚©‰ºƒŒ[ƒ“‚©‚ğŠm’è
};
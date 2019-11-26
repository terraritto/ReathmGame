#pragma once
#include "UIScreen.h"

class OptionSelectBox : public UIScreen
{
public:
	enum class OptionState
	{
		EStart,
		ESetting,
		EBack
	};

	OptionSelectBox(class Game* game);
	~OptionSelectBox();
	void Draw() override;

	void SetBackGround(std::string fileName);
	void SetDraw(bool ans) { mIsDraw = ans; }
	void SetSize(float size) { mSize = size; }
	void SetSpaceX(int x) { mBGPos.x = x; }
	void SetSpaceY(int y) { mBGPos.y += y; }
	int GetPosX() { return mBGPos.x; }
	int GetPosY() { return mBGPos.y; }
	float GetSize() { return mSize; }
	int GetBGSizeX() { return mBGSize.x; }
	void SetState(OptionState state) { mState = state; }
	OptionState GetState() { return mState; }
private:
	Pos mBGSize;
	float mSize;
	bool mIsDraw;
	OptionState mState;
};
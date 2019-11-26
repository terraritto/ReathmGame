#pragma once
#include "UIScreen.h"

class SettingBox : public UIScreen
{
public:
	SettingBox(class Game* game);
	~SettingBox();
	void Draw() override;

	void SetSize(float size) {
		mSize = size;
		mValuePosNow = mValuePos * size;
		mTextPosNow = mTextPos * size;
		mMinusPosNow = mMinusPos * size;
		mPlusPosNow = mPlusPos * size;
	}
	void SetSpaceX(int x) { mBGPos.x = x; }
	void SetSpaceY(int y) { mBGPos.y += y; }
	int GetPosX() { return mBGPos.x; }
	int GetPosY() { return mBGPos.y; }
	float GetSize() { return mSize; }
	int GetBGSizeX() { return mBGSize.x; }
	void SetDraw(bool ans) { mIsDraw = ans; }
	Pos GetBackGroundPos() { return mBGPos; }

	//text
	float GetSettingValue() { return mSettingValue; }
	void SetBackGroundText(std::string text) { mBackGroundText.textInformation = text; }
	void SetBackGroundTextPosX(int x) { mBackGroundText.x = x; }
	void SetSettingValue(float value) { mSettingValue = value; }
	void SetSettingLimitMin(float limit) { mLimitMin = limit; }
	void SetSettingLimitMax(float limit) { mLimitMax = limit; }
	void MoveValue(float value) { 
		float temp = value + mSettingValue; 
		mSettingValue = (temp >= mLimitMin && temp <= mLimitMax) ? temp : mSettingValue; 
	}
	void SetExplainText(std::string text) { mExplainText.textInformation = text; }
private:
	int mValue; Pos mValuePos; Pos mValuePosNow; Pos mValueSize;
	int mText; Pos mTextPos; Pos mTextPosNow; Pos mTextSize;
	int mMinus; Pos mMinusPos; Pos mMinusPosNow; Pos mMinusSize;
	int mPlus; Pos mPlusPos; Pos mPlusPosNow; Pos mPlusSize;
	Pos mBGSize; //background
	float mSize;
	bool mIsDraw;

	TextInf mBackGroundText;
	TextInf mValueText; float mSettingValue; float mLimitMin; float mLimitMax;
	TextInf mExplainText;
};
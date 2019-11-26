#pragma once
#include "UIScreen.h"

class MusicSelectBox : public UIScreen
{
public:
	MusicSelectBox(class Game* game);
	~MusicSelectBox();
	void Draw() override;

	void SetSize(float size) { mSize = size; 
	mArtworkPosNow = mArtworkPos * size; 
	mMusicLevelPosNow = mMusicLevelPos * size; 
	mMusicScorePosNow = mMusicScorePos * size; 
	}
	void SetSpaceX(int x) { mBGPos.x = x; }
	void SetSpaceY(int y) { mBGPos.y += y; }
	int GetPosX() { return mBGPos.x; }
	int GetPosY() { return mBGPos.y; }
	float GetSize() { return mSize; }
	int GetBGSizeX() { return mBGSize.x; }
	void SetDraw(bool ans) { mIsDraw = ans; }

	//inf set
	void SetLvInf(int lv) { mLvInf.textInformation = std::to_string(lv); }
	void SetSoundInf(std::string text) { mSoundInf.textInformation = text; }
	void SetNotesFile(std::string text) { mNotesFile = text; }
	void SetSoundFile(std::string text) { mSoundFile = text; }
	void SetImage(std::string text) { mImage = LoadGraph(text.c_str()); }
	std::string GetNotesFile() { return mNotesFile; }
private:
	int mArtwork; Pos mArtworkPos; Pos mArtworkPosNow; Pos mArtworkSize;
	int mMusicLevel; Pos mMusicLevelPos; Pos mMusicLevelPosNow; Pos mMusicLevelSize;
	int mMusicScore; Pos mMusicScorePos; Pos mMusicScorePosNow; Pos mMusicScoreSize;
	Pos mBGSize; //background
	float mSize;
	bool mIsDraw;
	//add information
	TextInf mLvInf; //use text
	TextInf mSoundInf; //use text
	std::string mNotesFile; //carry game 
	std::string mSoundFile; // carry game
	int mImage; //use image
};
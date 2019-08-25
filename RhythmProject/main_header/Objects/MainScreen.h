#pragma once
#include "DxLib.h"
#include "TraceNotes.h"
#include <string>

class MainScreen {
public:
	MainScreen(class Game* game);
	~MainScreen();
	void Start();
	void Update();

	//Notes
	void AddNotes(class Notes* note);
	void RemoveNotes(class Notes* note);
	//LongNotes
	void AddLongNotes(class LongNotes* note);
	void RemoveLongNotes(class LongNotes* note);
	//TraceNotes
	void AddTraceNotes(class TraceNotes* note);
	void RemoveTraceNotes(class TraceNotes* note);
	std::pair<float, float> GetPosOffset() { return mPosOffset; }
	//WallNotes
	void AddWallNotes(class WallNotes* note);
	void RemoveWallNotes(class WallNotes* note);
	//music
	void SetMusicFile(std::string fileName) { mMusicMemory = LoadSoundMem(fileName.c_str()); ChangeVolumeSoundMem(50, mMusicMemory); }
	auto GetNoteMusic() const { return mNortsSound; }
	void StartNoteMusic();
public:
	int mBpm;
	int mLpb;
	int mMusicMemory;
	LONGLONG mFirstTime;
	LONGLONG mNowTime;

	//ÉmÅ[Écëfçﬁ
	std::vector<class ObjectSampler*> mObjectSampler;
	
	//player
	class Player* mPlayer;

	//score
	int mMaxCombo;
	int mCombo;
	int mTotalCombo;
private:
	// is start?
	bool mIsStart;

	//stage
	class Actor* mStage;
	float mStageRotation;

	//ÉmÅ[Écä÷òA
	std::vector<class Notes*> mNorts;
	std::vector<class LongNotes*> mLongNotes;
	std::vector<TraceNotes*> mTraceNotes;
	std::vector<class WallNotes*> mWallNotes;
	std::pair<float, float> mPosOffset;//margin
	
	//ÉmÅ[Écâπê∫
	std::vector<int> mNortsSound;
	int mNortsSoundCount;

	class Game* mGame;

	//camera
	class FPSActor* mFPSActor;

	//UI
	class MainUI* mMainUI;
};
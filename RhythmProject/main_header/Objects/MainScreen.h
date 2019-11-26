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

	enum class MainScene
	{
		BlurScene,
		StartScene,
		EndScene
	};

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
	//notes effect
	void StartNoteEffect(VECTOR pos);
	void StartLongEffect(VECTOR pos);
	void UpdateLongEffect(VECTOR pos,int& handle);
	void EndLongEffect(int index);
	//judge effect
	void StartJudgeEffect();
	void EndJudgeEffect();
	//notes music
	void SetMusicFile(std::string fileName);
	auto GetNoteMusic() const { return mNortsSound; }
	void StartNoteMusic();
	//Scene
	MainScene GetScene() { return mScene; }
	//Text
	void DrawStartText();
public:
	int mBpm;
	int mLpb;
	int mMusicMemory;
	LONGLONG mFirstTime;
	LONGLONG mNowTime;

	//ノーツ素材
	std::vector<class ObjectSampler*> mObjectSampler;
	
	//notes effect
	std::vector<std::pair<bool,int>> mNortsPlayingEffect;
	std::vector<int> mNortsEffectCount;

	//player
	class Player* mPlayer;

	//score
	int mMaxCombo;
	int mCombo;
	int mTotalCombo;
private:
	//scene
	MainScene mScene;

	//stage
	class Actor* mStage;
	float mStageRotation;

	//ノーツ関連
	std::vector<class Notes*> mNorts;
	std::vector<class LongNotes*> mLongNotes;
	std::vector<TraceNotes*> mTraceNotes;
	std::vector<class WallNotes*> mWallNotes;
	std::pair<float, float> mPosOffset;//margin
	
	//ノーツエフェクト
	std::vector<int> mNortsEffect;

	//judge line effect
	int mJudgeEffect; int mJudgeEffectStartHandle;

	//text draw
	bool mIsDrawText;

	//ノーツ音声
	std::vector<int> mNortsSound;
	int mNortsSoundCount;

	class Game* mGame;

	//camera
	class FPSActor* mFPSActor;

	//UI
	class MainUI* mMainUI;

	//blur Scene specific
	LONGLONG mBrightTime;
};
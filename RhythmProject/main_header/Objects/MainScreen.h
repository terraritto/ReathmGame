#pragma once
#include "DxLib.h"
#include "TraceNotes.h"
#include <string>
#include <chrono>
#include "define.h"

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

	enum class ScreenState
	{
		ETutorial,
		EMain
	};

	/*********************************norts*****************************************/
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
	/*******************************************************************************/


	/********************************effect*****************************************/
	//notes effect
	void StartNoteEffect(VECTOR pos);
	void StartLongEffect(VECTOR pos);
	void EndLongEffect(VECTOR pos,int index);
	//trace effect
	void StartTraceEffect(VECTOR pos);
	void EndTraceEffect();
	//wall effect
	void StartWallEffect(VECTOR pos);
	void EndWallEffect();
	// judge Leteral
	void StartJudgeLiteralEffect(VECTOR pos, ETiming status);
	//judge effect
	void StartJudgeEffect();
	// chacter area effct
	void StartCharacterAreaEffect();
	// character area line
	void StartCharacterLineEffect();
	void UpdateCharacterLineEffect(VECTOR pos);
	/*******************************************************************************/


	//notes music
	void SetMusicFile(std::string fileName);
	auto GetNoteMusic() const { return mNortsSound; }
	void StartNoteMusic();
	//Scene
	MainScene GetScene() { return mScene; }
	//State
	ScreenState GetState() { return mState; }
	void SetState(ScreenState state) { mState = state; }
	//Text
	void DrawStartText();
	void TextRead();
public:
	int mBpm;
	int mLpb;
	int mMusicMemory;
	LONGLONG mFirstTime;
	LONGLONG mNowTime;
	std::chrono::system_clock::time_point mFirst, mNow;

	//ノーツ素材
	std::vector<class ObjectSampler*> mObjectSampler;
	
	//notes effect
	std::vector<std::pair<bool,int>> mNortsPlayingEffect;
	std::vector<int> mNortsEffectCount;

	//single notes effect
	int mTraceNotesEffect; int mTraceNotesEffectStart;
	int mWallNotesEffect; int mWallNotesEffectStart;

	//player
	class Player* mPlayer;

	//score
	int mMaxCombo;
	int mCombo;
	int mTotalCombo;
private:
	//scene
	MainScene mScene;

	//state
	ScreenState mState;
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
	int mJudgeEffect; int mJudgeEffectStartHandle; bool mIsTraceEffect;

	//character area effect
	int mCharaAreaEffect; int mCharaAreaEffectStartHandle; bool mIsWallEffect;

	// character line effect
	int mCharaLineEffect; int mCharaLineEffectStartHandle;

	//text draw
	bool mIsDrawText;
	//text count
	int mTutorialTextCount;

	//ノーツ音声
	std::vector<int> mNortsSound;
	int mNortsSoundCount;

	class Game* mGame;

	//camera
	class FPSActor* mFPSActor;

	//UI
	class MainUI* mMainUI;

	// tutorial Specific UI
	class tutorialUI* mTutorialUI;

	//blur Scene specific
	LONGLONG mBrightTime;
};
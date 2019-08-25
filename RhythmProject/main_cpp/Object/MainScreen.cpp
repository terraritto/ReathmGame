#include "../../Game.h"
#include "../../main_header/Actors/Actor.h"
#include "../../main_header/Objects/notes.h"
#include "../../main_header/Objects/LongNotes.h"
#include "../../main_header/Objects/TraceNotes.h"
#include "../../main_header/Objects/WallNotes.h"
#include "../../main_header/Objects/MainScreen.h"
#include "../../main_header/Objects/ObjectSampler.h"
#include "../../main_header/Objects/Player.h"
#include "../../main_header/Objects/FPSActor.h"
#include "../../main_header/Objects/MainUI.h"

MainScreen::MainScreen(Game* game)
	:mGame(game),
	mPosOffset(std::make_pair(-300,-1140)),
	mNortsSoundCount(0),
	mStageRotation(0.0f),
	mCombo(0),
	mMaxCombo(0),
	mTotalCombo(0),
	mIsStart(false)
{
	mStage = new Actor(game);
	mStage->SetPosition(VGet(0.0f, 0.0f, 0.0f));
	mStage->SetModelHandle<std::string>("object/Stage.mv1");

	//notesの音を用意
	int notesSound = LoadSoundMem("Music/touchNotes.wav");
	mNortsSound.emplace_back(notesSound);
	for (int i = 0; i < 29; i++)
	{
		mNortsSound.emplace_back(LoadSoundMem("Music/touchNotes.wav"));
	}
	
	//playerの用意
	mPlayer = new Player(game, this);

	//cameraの用意
	mFPSActor = new FPSActor(game);
	mFPSActor->SetPosition(VGet(0.0f, -1800.0f, 550.0f));

	//UIの用意
	mMainUI = new MainUI(game);

	//objectSamplerにObjectを読み込む
	ObjectSampler* object = new ObjectSampler(game);
	object->SetModelHandle<std::string>("object/trace_notes_pipe.mv1");
	object->SetIsDrawModel(false);
	object->SetCollision();
	MV1SetOpacityRate(object->GetModelHandle(), 0.3f);

	mObjectSampler.emplace_back(object);
	
	object = new ObjectSampler(game);
	object->SetModelHandle<std::string>("object/wall.mv1");
	object->SetIsDrawModel(false);
	object->SetCollision();
	MV1SetOpacityRate(object->GetModelHandle(), 0.3f);

	mObjectSampler.emplace_back(object);
}

MainScreen::~MainScreen()
{
	//delete sound
	for (auto sound : mNortsSound)
	{
		if (sound != -1)
		{
			DeleteSoundMem(sound);
		}
	}

	if (mMusicMemory != -1)
	{
		DeleteSoundMem(mMusicMemory);
	}

	//delete actor
	for (auto object : mObjectSampler)
	{
		object->SetState(Actor::State::EDead);
		MV1DeleteModel(object->GetModelHandle());
	}

	mPlayer->SetState(Actor::State::EDead);
	MV1DeleteModel(mPlayer->GetModelHandle());

	mStage->SetState(Actor::State::EDead);
	MV1DeleteModel(mStage->GetModelHandle());

	mFPSActor->SetState(Actor::State::EDead); // don't add 3D model

	//delete UI
	mMainUI->Close();
}

void MainScreen::Start()
{
	mMaxCombo = mNorts.size() + mLongNotes.size();

	PlaySoundMem(mMusicMemory, DX_PLAYTYPE_BACK, TRUE);
	mFirstTime = GetNowHiPerformanceCount();
	mIsStart = true;
}

void MainScreen::Update()
{
	//球体の回転
	mStageRotation += 0.002;
	MATRIX rot = MGetRotY(mStageRotation);
	MV1SetFrameUserLocalMatrix(mStage->GetModelHandle(), 0, rot);
	if (mStageRotation > DX_TWO_PI) { mStageRotation = 0.0f; }


	if (mIsStart) {
		mNowTime = GetNowHiPerformanceCount() - mFirstTime;

		if (
			!CheckSoundMem(mMusicMemory) && //stop music
			mNorts.size() == 0 && // don't exist norts
			mLongNotes.size() == 0 &&
			mWallNotes.size() == 0 &&
			mTraceNotes.size() == 0
			)
		{
			mGame->DeleteMainScreen(Game::RhythmGame::EStartScene);
		}
	}
}

//Notes
void MainScreen::AddNotes(Notes* note)
{
	mNorts.emplace_back(note);
}

void MainScreen::RemoveNotes(Notes* note)
{
	auto iter = std::find(mNorts.begin(), mNorts.end(), note);
	if (iter != mNorts.end())
	{
		mNorts.erase(iter);
	}
}

//LongNotes
void MainScreen::AddLongNotes(LongNotes* note)
{
	mLongNotes.emplace_back(note);
}

void MainScreen::RemoveLongNotes(LongNotes* note)
{
	auto iter = std::find(mLongNotes.begin(), mLongNotes.end(), note);
	if (iter != mLongNotes.end())
	{
		mLongNotes.erase(iter);
	}
}

//TraceNotes
void MainScreen::AddTraceNotes(TraceNotes* note)
{
	mTraceNotes.emplace_back(note);
}

void MainScreen::RemoveTraceNotes(TraceNotes* note)
{
	auto iter = std::find(mTraceNotes.begin(), mTraceNotes.end(), note);
	if (iter != mTraceNotes.end())
	{
		mTraceNotes.erase(iter);
	}
}

//WallNotes
void MainScreen::AddWallNotes(WallNotes* note)
{
	mWallNotes.emplace_back(note);
}

void MainScreen::RemoveWallNotes(WallNotes* note)
{
	auto iter = std::find(mWallNotes.begin(), mWallNotes.end(), note);
	if (iter != mWallNotes.end())
	{
		mWallNotes.erase(iter);
	}
}

//Music
void MainScreen::StartNoteMusic()
{
	PlaySoundMem(mNortsSound[mNortsSoundCount],DX_PLAYTYPE_BACK);
	mNortsSoundCount++;
	if (mNortsSound.size() == mNortsSoundCount) { mNortsSoundCount = 0; }
}
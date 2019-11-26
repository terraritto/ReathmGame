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

#include "EffekseerForDXLib.h"
#include <algorithm>

MainScreen::MainScreen(Game* game)
	:mGame(game),
	mPosOffset(std::make_pair(-300,-1160)),
	mNortsSoundCount(0),
	mNortsEffectCount(),
	mStageRotation(0.0f),
	mCombo(0),
	mMaxCombo(0),
	mTotalCombo(0),
	mScene(MainScene::BlurScene),
	mIsDrawText(false)
{
	mStage = new Actor(game);
	mStage->SetPosition(VGet(0.0f, 0.0f, 0.0f));
	mStage->SetModelHandle<std::string>("object/Stage.mv1");

	//notesのeffectを用意
	mNortsEffect.emplace_back(LoadEffekseerEffect("object/tap_effect.efk",1.5f));
	mNortsEffectCount.emplace_back(0);
	mNortsEffect.emplace_back(LoadEffekseerEffect("object/long_effect.efk", 1.5f));
	mNortsEffectCount.emplace_back(0);

	// set effect of judge
	mJudgeEffect = LoadEffekseerEffect("object/trace_judge_line.efk");
	ErrorLogFmtAdd("load effect number: %d", mJudgeEffect);

	for (int i = 0; i < 30; i++)
	{
		mNortsPlayingEffect.emplace_back(std::make_pair(false, -1));
	}

	//notesの音を用意
	for (int i = 0; i < 30; i++)
	{
		mNortsSound.emplace_back(LoadSoundMem("Music/touchNotes.wav"));
		ChangeVolumeSoundMem(255, mNortsSound[i]);
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
	
	//bright specific
	mBrightTime = GetNowHiPerformanceCount();
}

MainScreen::~MainScreen()
{
	//delete sound
	for (auto sound : mNortsSound)
	{
		if (sound != -1)
		{
			sound = DeleteSoundMem(sound);
			ErrorLogFmtAdd("delete sound: %d", sound);
		}
	}

	if (mMusicMemory != -1)
	{
		ErrorLogFmtAdd("delete main sound: %d",DeleteSoundMem(mMusicMemory));
	}

	//delete actor
	for (auto object : mObjectSampler)
	{
		object->SetState(Actor::State::EDead);
	}

	mPlayer->SetState(Actor::State::EDead);

	mStage->SetState(Actor::State::EDead);

	mFPSActor->SetState(Actor::State::EDead); // don't add 3D model

	//delete UI
	mMainUI->Close();

	//delete effect
	for (auto object : mNortsEffect)
	{
		ErrorLogFmtAdd("delete effect number: %d", object);
		ErrorLogFmtAdd("effect: %d", DeleteEffekseerEffect(object));
	}
	DeleteEffekseerEffect(mJudgeEffect);
}

void MainScreen::Start()
{
	mMaxCombo = mNorts.size() + mLongNotes.size();

	PlaySoundMem(mMusicMemory, DX_PLAYTYPE_BACK, TRUE);
	mFirstTime = GetNowHiPerformanceCount();
	StartJudgeEffect();//judge effect start
}

void MainScreen::Update()
{
	//rotate of sphere
	mStageRotation += 0.002;
	MATRIX rot = MGetRotY(mStageRotation);
	MV1SetFrameUserLocalMatrix(mStage->GetModelHandle(), 0, rot);
	if (mStageRotation > DX_TWO_PI) { mStageRotation = 0.0f; }
	
	if (mScene == MainScene::BlurScene)
	{
		LONGLONG nowTime = GetNowHiPerformanceCount() - mBrightTime;
		
		//---text output---
		if (nowTime < ONE_TIME / 2)
		{
			mIsDrawText = true;
		}
		//-----------------
		if (nowTime < ONE_TIME)
		{
			SetLightDifColor(GetColorF(0.1f, 0.1f, 0.1f, 0.0f));
		}
		else if (nowTime < ONE_TIME * 9 / 6)
		{
			double col = 1 -(ONE_TIME * 9 / 6 - nowTime)/(ONE_TIME/2);
			SetLightDifColor(GetColorF(1.0f * col, 1.0f * col, 1.0f * col, 0.0f));
			mIsDrawText = false;
		}
		else
		{
			SetLightDifColor(GetColorF(1.0f, 1.0f, 1.0f, 0.0f));
			Start();
			mScene = MainScene::StartScene;
		}
		return;
	}

	if (mScene == MainScene::StartScene)
	{

		mNowTime = GetNowHiPerformanceCount() - mFirstTime;

		if (
			!CheckSoundMem(mMusicMemory) && //stop music
			mNorts.size() == 0 && // don't exist norts
			mLongNotes.size() == 0 &&
			mWallNotes.size() == 0 &&
			mTraceNotes.size() == 0
			)
		{
			mScene = MainScene::EndScene;
		}
		
		return;
	}

	if (mScene == MainScene::EndScene)
	{
		mGame->DeleteMainScreen(Game::RhythmGame::EStartScene);
		return;
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

void MainScreen::DrawStartText()
{
	if (mIsDrawText)
	{
		DrawString(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 200, "Are You Ready?", GetColor(255, 255, 255));
	}
}

//notes effect
void MainScreen::StartNoteEffect(VECTOR pos)
{
	VECTOR setPos = VGet(pos.x, 20.0, mPosOffset.second - 5); //prepare pos
	int handle = PlayEffekseer3DEffect(mNortsEffect[0]); //play effect
	SetPosPlayingEffekseer3DEffect(handle, setPos.x, setPos.y, setPos.z);
	SetRotationPlayingEffekseer3DEffect(handle, 0,0,0); //set effect pos and rotation
}


//long notes effect
void MainScreen::StartLongEffect(VECTOR pos)
{
	VECTOR setPos = VGet(pos.x, 20.0, mPosOffset.second - 5); //prepare pos
	mNortsPlayingEffect[mNortsEffectCount[1]] = std::make_pair(true,PlayEffekseer3DEffect(mNortsEffect[1])); //play effect
	SetPosPlayingEffekseer3DEffect(mNortsPlayingEffect[mNortsEffectCount[1]].second, setPos.x, setPos.y, setPos.z);
	SetRotationPlayingEffekseer3DEffect(mNortsPlayingEffect[mNortsEffectCount[1]].second, 0, 0, 0); //set effect pos and rotation

	mNortsEffectCount[1]++; //plus count

	if (mNortsPlayingEffect.size() == mNortsEffectCount[1]) { mNortsEffectCount[1] = 0; } // update index
}

void MainScreen::UpdateLongEffect(VECTOR pos,int& handle)
{
	VECTOR setPos = VGet(pos.x, 20.0, mPosOffset.second - 5); //prepare pos
	handle = PlayEffekseer3DEffect(mNortsEffect[1]); //play effect
	SetPosPlayingEffekseer3DEffect(handle, setPos.x, setPos.y, setPos.z);
	SetRotationPlayingEffekseer3DEffect(handle, 0, 0, 0); //set effect pos and rotation
}

void MainScreen::EndLongEffect(int index)
{
	int handle = StopEffekseer3DEffect(mNortsPlayingEffect[index].second);
	mNortsPlayingEffect[index] = std::pair(false, 0);
}

void MainScreen::StartJudgeEffect()
{
	mJudgeEffectStartHandle = PlayEffekseer3DEffect(mJudgeEffect);
	SetPosPlayingEffekseer3DEffect(mJudgeEffectStartHandle, 0, 240, mPosOffset.second + 15);
}

void MainScreen::EndJudgeEffect()
{
	StopEffekseer3DEffect(mJudgeEffect);
}

void MainScreen::SetMusicFile(std::string fileName)
{
	mMusicMemory = LoadSoundMem(fileName.c_str()); 
	ChangeVolumeSoundMem(255 * mGame->GetGameVolume() / 10, mMusicMemory);
}

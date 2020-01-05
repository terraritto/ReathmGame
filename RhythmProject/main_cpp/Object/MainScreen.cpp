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
#include "../../main_header/Objects/tutorialUI.h"

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
	mIsDrawText(false),
	mIsTraceEffect(false),
	mIsWallEffect(false)
{
	mStage = new Actor(game);
	mStage->SetPosition(VGet(0.0f, 0.0f, 0.0f));
	mStage->SetModelHandle<std::string>("object/Stage.mv1");

	//notesのeffectを用意
	mNortsEffect.emplace_back(LoadEffekseerEffect("object/tap_effect.efk",1.5f));
	mNortsEffectCount.emplace_back(0);
	mNortsEffect.emplace_back(LoadEffekseerEffect("object/long_effect.efk", 1.5f));
	mNortsEffectCount.emplace_back(0);
	mNortsEffect.emplace_back(LoadEffekseerEffect("object/long_end_effect.efk", 1.5f));
	mNortsEffectCount.emplace_back(0);
	//judge literalのeffectを用意
	mNortsEffect.emplace_back(LoadEffekseerEffect("object/timing_critical_effect.efk"));
	mNortsEffectCount.emplace_back(0);
	mNortsEffect.emplace_back(LoadEffekseerEffect("object/timing_blast_effect.efk"));
	mNortsEffectCount.emplace_back(0);
	mNortsEffect.emplace_back(LoadEffekseerEffect("object/timing_hit_effect.efk"));
	mNortsEffectCount.emplace_back(0);
	mNortsEffect.emplace_back(LoadEffekseerEffect("object/timing_miss_effect.efk"));
	mNortsEffectCount.emplace_back(0);

	// set effect of trace
	mTraceNotesEffect = LoadEffekseerEffect("object/trace_effect.efk");

	// set effect of wall
	mWallNotesEffect = LoadEffekseerEffect("object/damage_effect.efk");
	
	// set effect of judge
	mJudgeEffect = LoadEffekseerEffect("object/trace_judge_line.efk");

	// set effect of character area
	mCharaAreaEffect = LoadEffekseerEffect("object/character_area.efk");

	//set effect of character line area
	mCharaLineEffect = LoadEffekseerEffect("object/character_line.efk");

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

	if (mState == ScreenState::ETutorial)
	{
		mTutorialUI->Close();
	}
	//delete effect
	for (auto object : mNortsEffect)
	{
		ErrorLogFmtAdd("delete effect number: %d", object);
		ErrorLogFmtAdd("effect: %d", DeleteEffekseerEffect(object));
	}
	StopEffekseer3DEffect(mJudgeEffectStartHandle);
	StopEffekseer3DEffect(mCharaAreaEffectStartHandle);
	StopEffekseer3DEffect(mCharaLineEffectStartHandle);
	DeleteEffekseerEffect(mJudgeEffect);
	DeleteEffekseerEffect(mCharaAreaEffect);
	DeleteEffekseerEffect(mCharaLineEffect);
}

void MainScreen::Start()
{
	mMaxCombo = mNorts.size() + mLongNotes.size();

	if (mState == ScreenState::EMain) {
		PlaySoundMem(mMusicMemory, DX_PLAYTYPE_BACK, TRUE);
	}
	
	if (mState == ScreenState::ETutorial)
	{
		PlaySoundMem(mMusicMemory, DX_PLAYTYPE_LOOP);

		mTutorialUI = new tutorialUI(mGame);
		mTutorialUI->SetTitleText("チュートリアル");
		std::vector<std::string> text;
		text.push_back("これからチュートリアルを開始するよ！！");
		text.push_back("準備はいい？");
		mTutorialUI->SetExplainText(text);
		mTutorialTextCount = 0;
	}

	mFirstTime = GetNowHiPerformanceCount();
	mFirst = std::chrono::system_clock::now();
	StartJudgeEffect();//judge effect start
	StartCharacterAreaEffect(); // character area effect start
	StartCharacterLineEffect(); // character line effect start
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
		mNow = std::chrono::system_clock::now();

		//effect for player line
		VECTOR pos = mPlayer->GetPosition<VECTOR>();
		pos = Math::VectorTransAxis(pos);
		UpdateCharacterLineEffect(pos);


		if (mState == ScreenState::EMain) {
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

		if (mState == ScreenState::ETutorial)
		{
			if (
				mNorts.size() == 0 && // don't exist norts
				mLongNotes.size() == 0 &&
				mWallNotes.size() == 0 &&
				mTraceNotes.size() == 0
				)
			{
				mScene = MainScene::EndScene;
			}
			TextRead();
		}
		
		return;
	}

	if (mScene == MainScene::EndScene)
	{
		if (mState == ScreenState::EMain) {
			mGame->DeleteMainScreen(Game::RhythmGame::EStartScene);
		}

		if (mState == ScreenState::ETutorial)
		{
			mNowTime = GetNowHiPerformanceCount() - mFirstTime;

			if (mNowTime >= 120000000)
			{
				StopSoundMem(mMusicMemory);
				mGame->DeleteTurorialScreen(Game::RhythmGame::ESelectScene);
			}
		}
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

void MainScreen::TextRead()
{
	std::vector<std::string> text;
	switch (mTutorialTextCount)
	{
	case 0:
		if (mNowTime >= 10000000)
		{
			text.push_back("タイミングよくボタンを");
			text.push_back("叩いてね！！");
			text.push_back("キーボードの場合");
			text.push_back("赤:S,Jキー");
			text.push_back("青:D,Kキー");
			text.push_back("緑:F,Lキー");
			mTutorialTextCount++;
			mTutorialUI->SetExplainText(text);
			mTutorialUI->SetTitleText("タップノーツ");
		}
		break;
	case 1:
		if (mNowTime >= 32000000)
		{
			text.push_back("OK！！");
			text.push_back("次に行くよ！！");
			mTutorialTextCount++;
			mTutorialUI->SetExplainText(text);
			mTutorialUI->SetTitleText("チュートリアル");

		}
		break;
	case 2:
		if (mNowTime >= 36500000)
		{
			text.push_back("ボタンを長押ししてね！！");
			text.push_back("最後は離しても");
			text.push_back("押しっぱなしでも");
			text.push_back("大丈夫だよ！！");
			mTutorialTextCount++;
			mTutorialUI->SetExplainText(text);
			mTutorialUI->SetTitleText("ロングノーツ");

		}
		break;
	case 3:
		if (mNowTime >= 66500000)
		{
			text.push_back("OK！！");
			text.push_back("次に行くよ！！");
			mTutorialTextCount++;
			mTutorialUI->SetExplainText(text);
			mTutorialUI->SetTitleText("チュートリアル");

		}
		break;
	case 4:
		if (mNowTime >= 71000000)
		{
			text.push_back("壁が向かってくるよ");
			text.push_back("コントローラを使って避けてね！！");
			text.push_back("キーボードの場合");
			text.push_back("矢印キー");
			mTutorialTextCount++;
			mTutorialUI->SetExplainText(text);
			mTutorialUI->SetTitleText("ウォールノーツ");

		}
		break;
	case 5:
		if (mNowTime >= 92000000)
		{
			text.push_back("OK！！");
			text.push_back("次に行くよ！！");
			mTutorialTextCount++;
			mTutorialUI->SetExplainText(text);
			mTutorialUI->SetTitleText("チュートリアル");

		}
		break;
	case 6:
		if (mNowTime >= 96500000)
		{
			text.push_back("ノーツに沿って");
			text.push_back("キャラクターを動かそう！！");
			text.push_back("キーボードの場合");
			text.push_back("矢印キー");
			mTutorialTextCount++;
			mTutorialUI->SetExplainText(text);
			mTutorialUI->SetTitleText("トレースノーツ");

		}
		break;
	case 7:
		if (mNowTime >= 116500000)
		{
			text.push_back("OK！！");
			text.push_back("これでチュートリアルは終わりだよ！！");
			text.push_back("これで君も一人前だ！！");
			text.push_back("一緒に宇宙に飛び立とう！！");
			mTutorialTextCount++;
			mTutorialUI->SetExplainText(text);
			mTutorialUI->SetTitleText("チュートリアル");
		}
	default:
		break;
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

	auto temp = GetEffekseer3DManager();
	temp->SetTargetLocation(mNortsPlayingEffect[mNortsEffectCount[1]].second, setPos.x, setPos.y, setPos.z);

	mNortsEffectCount[1]++; //plus count

	if (mNortsPlayingEffect.size() == mNortsEffectCount[1]) { mNortsEffectCount[1] = 0; } // update index
}

void MainScreen::EndLongEffect(VECTOR pos,int index)
{
	int handle = StopEffekseer3DEffect(mNortsPlayingEffect[index].second);
	mNortsPlayingEffect[index] = std::pair(false, 0);

	VECTOR setPos = VGet(pos.x, 20.0, mPosOffset.second - 5); //prepare pos
	handle = PlayEffekseer3DEffect(mNortsEffect[2]); //play effect
	SetPosPlayingEffekseer3DEffect(handle, setPos.x, setPos.y, setPos.z);
	SetRotationPlayingEffekseer3DEffect(handle, 0, 0, 0); //set effect pos and rotation
}

void MainScreen::StartTraceEffect(VECTOR pos)
{
	if (!mIsTraceEffect) {
		VECTOR setPos = VGet(pos.x, pos.y, pos.z); //prepare pos
		mTraceNotesEffectStart = PlayEffekseer3DEffect(mTraceNotesEffect); //play effect
		SetPosPlayingEffekseer3DEffect(mTraceNotesEffectStart, setPos.x, setPos.y, setPos.z);
		SetRotationPlayingEffekseer3DEffect(mTraceNotesEffectStart, 0, 0, 0); //set effect pos and rotation
		mIsTraceEffect = true;
	}
}

void MainScreen::EndTraceEffect()
{
	if (mIsTraceEffect) {
		StopEffekseer3DEffect(mTraceNotesEffectStart);
		mIsTraceEffect = false;
	}
}

void MainScreen::StartWallEffect(VECTOR pos)
{
	if (!mIsWallEffect) {
		VECTOR setPos = VGet(pos.x, pos.y, pos.z); //prepare pos
		mWallNotesEffectStart = PlayEffekseer3DEffect(mWallNotesEffect); //play effect
		SetPosPlayingEffekseer3DEffect(mWallNotesEffectStart, setPos.x, setPos.y, setPos.z);
		SetRotationPlayingEffekseer3DEffect(mWallNotesEffectStart, 0, 0, 0); //set effect pos and rotation
		mIsWallEffect = true;
	}
}

void MainScreen::EndWallEffect()
{
	if (mIsWallEffect) {
		StopEffekseer3DEffect(mWallNotesEffectStart);
		mIsWallEffect = false;
	}
}

void MainScreen::StartJudgeLiteralEffect(VECTOR pos, ETiming status)
{
	int handle;
	VECTOR setPos;

	setPos = VGet(pos.x, 20.0 + 50, mPosOffset.second - 5); //prepare pos
	handle = PlayEffekseer3DEffect(mNortsEffect[static_cast<int>(status) + 3]); //play effect
	SetPosPlayingEffekseer3DEffect(handle, setPos.x, setPos.y, setPos.z);
}

// judge Effect
void MainScreen::StartJudgeEffect()
{
	mJudgeEffectStartHandle = PlayEffekseer3DEffect(mJudgeEffect);
	SetPosPlayingEffekseer3DEffect(mJudgeEffectStartHandle, 0, 240, mPosOffset.second + 15);
}

// start effect
void MainScreen::StartCharacterAreaEffect()
{
	mCharaAreaEffectStartHandle = PlayEffekseer3DEffect(mCharaAreaEffect);
	SetPosPlayingEffekseer3DEffect(mCharaAreaEffectStartHandle, 0, 10, mPosOffset.second + 15);
}

// character line effect
void MainScreen::StartCharacterLineEffect()
{
	mCharaLineEffectStartHandle = PlayEffekseer3DEffect(mCharaLineEffect);
}

void MainScreen::UpdateCharacterLineEffect(VECTOR pos)
{
	StopEffekseer3DEffect(mCharaLineEffectStartHandle);
	mCharaLineEffectStartHandle = PlayEffekseer3DEffect(mCharaLineEffect);
	SetPosPlayingEffekseer3DEffect(mCharaLineEffectStartHandle, 0.0f, pos.y - 50.0f, pos.z);
}

// set music
void MainScreen::SetMusicFile(std::string fileName)
{
	mMusicMemory = LoadSoundMem(fileName.c_str()); 
	ChangeVolumeSoundMem(255 * mGame->GetGameVolume() / 10, mMusicMemory);
}


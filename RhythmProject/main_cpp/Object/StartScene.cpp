#include "../../main_header/Objects/StartScene.h"
#include "../../Game.h"
#include "../../main_header/Actors/Actor.h"
#include "../../main_header/Objects/InputSystem.h"
#include "../../main_header/Objects/FPSActor.h"
#include "../../main_header/Objects/define.h"

StartScene::StartScene(Game* game)
	: mGame(game)
	, mStageRotation(0.0f)
	, mAlpha(1.0f)
	, mStrState(FlashStr::Down)
	//, mIsCardRead(false)
	, mIsCoinIn(false)
{
	mStage = new Actor(game);
	mStage->SetPosition(VGet(0.0f, 0.0f, 0.0f));
	mStage->SetModelHandle<std::string>("object/Stage.mv1");
	//non-visible because they are unnecessary
	MV1SetFrameVisible(mStage->GetModelHandle(), 1, FALSE);
	MV1SetFrameVisible(mStage->GetModelHandle(), 2, FALSE);
	MV1SetFrameVisible(mStage->GetModelHandle(), 3, FALSE);
	MV1SetFrameVisible(mStage->GetModelHandle(), 4, FALSE);
	MV1SetFrameVisible(mStage->GetModelHandle(), 5, FALSE);

	mFPSActor = new FPSActor(game);
	mFPSActor->SetPosition(VGet(0.0f, -1500.0f, 0.0f));
	//mFPSActor->SetPosition(VGet(0.0f, 0.0f, 0.0f));

	//setting card reader
	//mCardReader = LazyPCSCFelicaLite::PCSCFelicaLite();
}

StartScene::~StartScene()
{
	mStage->SetState(Actor::State::EDead);
	mFPSActor->SetState(Actor::State::EDead);
}

void StartScene::ProcessInput(const InputState& state)
{
	if (state.Keyboard.GetKeyValue(KEY_INPUT_ESCAPE))
	{
		mGame->SetState(Game::GameState::EQuit);
	}

	if (state.Keyboard.GetKeyValue(KEY_INPUT_RETURN))
	{
		mIsCoinIn = true;
	}
	/*
	if (mCardReader.autoConnectToFelica())
	{
		mIsCardRead = true;
	}
	*/
}

void StartScene::Update()
{
	// rotate sphere
	mStageRotation += 0.002f;
	MATRIX rot = MGetRotY(mStageRotation);
	MV1SetFrameUserLocalMatrix(mStage->GetModelHandle(), 0, rot);
	if (mStageRotation > DX_TWO_PI) { mStageRotation = 0.0f; }

	if (/*mIsCardRead &&*/ mIsCoinIn)
	{
		mGame->DeleteStartScreen(Game::RhythmGame::ETutorialSelect);
	}
}

void StartScene::DrawStr()
{
	//draw string
	SetFontSize(70);
	DrawString(175, 100, "Rhythm Space", GetColor(255, 255, 255));

	SetFontSize(30);
	if (mStrState == FlashStr::Down)
	{
		mAlpha -= 0.02f;
		if (mAlpha <= 0.0f) {
			mAlpha = 0;
			mStrState = FlashStr::Up;
		}
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * mAlpha);
		DrawString(384 - 150, 512, "Press Enter To Start", GetColor(255, 0, 255));
		DrawString(384 - 115, 612, "Press Esc To End", GetColor(255, 0, 255));
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else
	{
		mAlpha += 0.02f;
		if (mAlpha >= 1) { mAlpha = 1; mStrState = FlashStr::Down; }
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * mAlpha);
		DrawString(384 - 150, 512, "Press Enter To Start", GetColor(255, 0, 255));
		DrawString(384 - 115, 612, "Press Esc To End", GetColor(255, 0, 255));
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	std::string card = "Card:";
	//card += mIsCardRead ? "True" : "False";
	std::string coin = "Coin:";
	coin += mIsCoinIn ? "True" : "False";

	int drawWidth = GetDrawStringWidth(card.c_str(), card.size());
	DrawString((WINDOW_WIDTH - drawWidth) / 2,WINDOW_HEIGHT-30, card.c_str(), GetColor(255, 0, 255));

	drawWidth = GetDrawStringWidth(coin.c_str(), coin.size());
	DrawString((WINDOW_WIDTH - drawWidth) / 2, WINDOW_HEIGHT-60, coin.c_str(), GetColor(255, 0, 255));
}
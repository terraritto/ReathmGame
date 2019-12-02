#include "../../main_header/Objects/notes.h"
#include "../../main_header/Objects/Player.h"
#include "../../Game.h"

Notes::Notes(Game* game)
	:Actor(game)
{
	SetPosition<VECTOR>(VGet(game->GetMainScreen()->GetPosOffset().first + 100.0f * mLane + 50,  
		game->GetMainScreen()->GetPosOffset().second + (+2500.0f * (ArrivalTime / (ONE_TIME * (2.1 - GetGame<Game>()->GetGameSpeed() / 5)))),
		10.0f));

	game->GetMainScreen()->AddNotes(this);
}

Notes::~Notes()
{
	GetGame<Game>()->GetMainScreen()->RemoveNotes(this);
}

void Notes::UpdateActor(float deltaTime)
{
	VECTOR pos = GetPosition<VECTOR>();
	float value = +2500.0f * (static_cast<float>((ArrivalTime - GetGame<Game>()->GetMainScreen()->mNowTime)) / static_cast<float>((ONE_TIME *(2.1 - GetGame<Game>()->GetGameSpeed()/5))));
	pos.x = GetGame<Game>()->GetMainScreen()->GetPosOffset().first + 100.0f * mLane + 50;
	pos.y = GetGame<Game>()->GetMainScreen()->GetPosOffset().second
		+ (value);//+2500.0f * ((ArrivalTime - GetGame<Game>()->GetMainScreen()->mNowTime) / 6000000.0f));
	SetPosition<VECTOR>(pos);


	// input
	bool isInput = true;
	LONGLONG plTime;
	if (GetGame<Game>()->GetMainScreen()->mPlayer->mIsInputLeft[static_cast<int>(mColor)]) //left side
	{
		plTime = GetGame<Game>()->GetMainScreen()->mPlayer->mInputTimeLeft[static_cast<int>(mColor)]
			- GetGame<Game>()->GetMainScreen()->mFirstTime;
		mDir = EKeyboardDirection::ELeft;
	}
	else if (GetGame<Game>()->GetMainScreen()->mPlayer->mIsInputRight[static_cast<int>(mColor)]) //right side
	{
		plTime = GetGame<Game>()->GetMainScreen()->mPlayer->mInputTimeRight[static_cast<int>(mColor)]
			- GetGame<Game>()->GetMainScreen()->mFirstTime;
		mDir = EKeyboardDirection::ERight;
	}
	else
	{
		isInput = false; //not exist input
	}

	if (isInput)
	{
		if (
			(plTime - ArrivalTime + (-1) * GetGame<Game>()->GetGameTiming()/10 * JUDGE_OFFSET) <= JUDGE_TIME * 4
			&& (plTime - ArrivalTime + (-1) * GetGame<Game>()->GetGameTiming()/10 * JUDGE_OFFSET) >= -JUDGE_TIME * 4
			)
		{
			//update combo
			GetGame<Game>()->GetMainScreen()->mTotalCombo += 1;
			GetGame<Game>()->GetMainScreen()->mCombo += 1;

			SetState(EDead);
			GetGame<Game>()->GetMainScreen()->StartNoteMusic();
			GetGame<Game>()->GetMainScreen()->StartNoteEffect(GetPosition<VECTOR>());

			if (mDir == EKeyboardDirection::ELeft)
			{
				GetGame<Game>()->GetMainScreen()->mPlayer->mIsInputLeft[static_cast<int>(mColor)] = false;
			}
			
			if(mDir == EKeyboardDirection::ERight)
			{
				GetGame<Game>()->GetMainScreen()->mPlayer->mIsInputRight[static_cast<int>(mColor)] = false;
			}
		}
	}

	if (pos.y + 100.0f < GetGame<Game>()->GetMainScreen()->GetPosOffset().second)
	{
		//update combo
		GetGame<Game>()->GetMainScreen()->mCombo = 0;

		SetState(EDead);
	}
}

void Notes::ActorInput(const InputState & input)
{

}

void Notes::SetColor(char color)
{
	switch (color)
	{
	case 'r':
		SetModelHandle<std::string>("object/Notes_red.mv1");
		mColor = EColor::ERed;
		break;
	case 'g':
		SetModelHandle<std::string>("object/Notes_green.mv1");
		mColor = EColor::EGreen;
		break;
	case 'b':
		SetModelHandle<std::string>("object/Notes_blue.mv1");
		mColor = EColor::EBlue;
		break;
	default:
		break;
	}
}
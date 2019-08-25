#include "../../main_header/Objects/WallNotes.h"
#include "../../main_header/Objects/ObjectSampler.h"
#include "../../main_header/Objects/Player.h"
#include "../../Game.h"
#include "../../main_header/Objects/define.h"

WallNotes::WallNotes(Game* game)
	:Actor(game)
{
	game->GetMainScreen()->AddWallNotes(this);
}

WallNotes::~WallNotes()
{
	GetGame<Game>()->GetMainScreen()->RemoveWallNotes(this);
}

void WallNotes::UpdateActor(float deltaTime)
{
	float startValue = GetGame<Game>()->GetMainScreen()->GetPosOffset().second
		+ 2500.0f * ((mArrivalTime[0] - GetGame<Game>()->GetMainScreen()->mNowTime) / 6000000.0f);
	float endValue = +2500.0f * ((mArrivalTime[1] - GetGame<Game>()->GetMainScreen()->mNowTime) / 6000000.0f);


	if (startValue < GetGame<Game>()->GetMainScreen()->GetPosOffset().second)
	{
		if (endValue < -500.0f) // 0.0Ç™íöìxèIÇÌÇËÇÃïîï™Ç™ìÕÇ¢ÇΩèÍèäÅAÇªÇ±Ç©ÇÁè≠ÇµÇ∏ÇÁÇ∑
		{
			SetState(EDead);
		}

		VECTOR pos = GetPosition<VECTOR>();
		pos.x = GetGame<Game>()->GetMainScreen()->GetPosOffset().first + 100.0f * mLane + 50;//Ç±Ç±Ç≈offsetÇÃâeãø
		pos.y = GetGame<Game>()->GetMainScreen()->GetPosOffset().second
			+ (endValue);
		SetPosition<VECTOR>(pos);
		
		if (endValue < -100.0f) { return; }//Ç±ÇÍà»ç~Ç…ìñÇΩÇÈÇ±Ç∆ÇÕÇ»Ç¢ÇÃÇ≈èdÇ¢èàóùÇÕè¡Ç∑
		else //ìñÇΩÇËîªíË
		{
			int objectHandle = GetGame<Game>()->GetMainScreen()->mObjectSampler[1]->GetModelHandle();

			//à íuí≤êÆ
			MV1SetScale(objectHandle, VGet(1.0f, 1.0f, mScale));//scaleÇîΩâf
			MV1SetPosition(objectHandle, Math::VectorTransAxis(pos));

			//collisionÇÃçXêV
			GetGame<Game>()->GetMainScreen()->mObjectSampler[1]->RefreshCollision();

			//player capsuleóp
			int plHandle = GetGame<Game>()->GetMainScreen()->mPlayer->GetModelHandle();
			VECTOR posDown = Math::VectorTransAxis(GetGame<Game>()->GetMainScreen()->mPlayer->GetPosition<VECTOR>());
			posDown = VAdd(posDown, VGet(0.0f, PLAYER_RAD_OFFSET, 0.0f));
			VECTOR posUp = VAdd(posDown, VGet(0.0f, PLAYER_RAD_OFFSET, 0.0f));
			
			//ìñÇΩÇËîªíË
			MV1_COLL_RESULT_POLY_DIM  result = MV1CollCheck_Capsule(objectHandle, 0, posDown, posUp, PLAYER_RAD_OFFSET);

			if (result.HitNum >= 1) //hit
			{
				MV1SetMaterialDrawBlendMode(plHandle, 0, DX_BLENDMODE_ADD);
				MV1SetMaterialDrawBlendParam(plHandle, 0, 50);
			}
		}
	}
	else
	{
		VECTOR pos = GetPosition<VECTOR>();
		pos.x = GetGame<Game>()->GetMainScreen()->GetPosOffset().first + 100.0f * mLane + 50;//Ç±Ç±Ç≈offsetÇÃâeãø
		pos.y = GetGame<Game>()->GetMainScreen()->GetPosOffset().second
			+ (endValue);//+2500.0f * ((ArrivalTime - GetGame<Game>()->GetMainScreen()->mNowTime) / 6000000.0f));
		pos.z = mHeight; //çÇÇ≥ÇÃâeãø
		SetPosition<VECTOR>(pos);
	}
}

void WallNotes::Draw()
{
	int objectHandle = GetGame<Game>()->GetMainScreen()->mObjectSampler[1]->GetModelHandle();
	MV1SetScale(objectHandle, VGet(1.0f, 1.0f, mScale));//scaleÇîΩâf
	VECTOR pos = GetPosition<VECTOR>();
	MV1SetPosition(objectHandle, Math::VectorTransAxis(pos));
	MV1DrawModel(objectHandle);
}

void WallNotes::SetScale()
{
	float start = GetGame<Game>()->GetMainScreen()->GetPosOffset().second
		+ (+2500.0f * (mArrivalTime[0] / 6000000.0f));
	float end = GetGame<Game>()->GetMainScreen()->GetPosOffset().second
		+ (+2500.0f * (mArrivalTime[1] / 6000000.0f));
	float size = (end - start) / 10.0f;
	mScale = size;
}
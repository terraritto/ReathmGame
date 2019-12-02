#include "../../main_header/Objects/TraceNotes.h"
#include "../../main_header/Objects/ObjectSampler.h"
#include "../../main_header/Objects/Player.h"
#include "../../Game.h"

TraceNotes::TraceNotes(Game* game)
	:Actor(game)
{
	game->GetMainScreen()->AddTraceNotes(this);
}

TraceNotes::~TraceNotes()
{
	GetGame<Game>()->GetMainScreen()->RemoveTraceNotes(this);
}

void TraceNotes::UpdateActor(float deltaTime)
{
	float startValue = GetGame<Game>()->GetMainScreen()->GetPosOffset().second + mScale * 10
		+ 2500.0f * ((mArrivalTime[0] - GetGame<Game>()->GetMainScreen()->mNowTime) / (ONE_TIME * (2.1 - GetGame<Game>()->GetGameSpeed() / 5.0f)));
	float endValue = +2500.0f * ((mArrivalTime[1] - GetGame<Game>()->GetMainScreen()->mNowTime) / (ONE_TIME * (2.1 - GetGame<Game>()->GetGameSpeed() / 5.0f)));

	if (startValue < GetGame<Game>()->GetMainScreen()->GetPosOffset().second + mScale * 10)
	{
		if (endValue < -500.0f) // 0.0Ç™íöìxèIÇÌÇËÇÃïîï™Ç™ìÕÇ¢ÇΩèÍèäÅAÇªÇ±Ç©ÇÁè≠ÇµÇ∏ÇÁÇ∑
		{
			SetState(EDead);
			GetGame<Game>()->GetMainScreen()->EndTraceEffect(); //end effect
		}

		VECTOR pos = GetPosition<VECTOR>();
		pos.x = GetGame<Game>()->GetMainScreen()->GetPosOffset().first + 50.0f * mLane[1] + 50;//Ç±Ç±Ç≈offsetÇÃâeãø
		pos.y = GetGame<Game>()->GetMainScreen()->GetPosOffset().second  +
			endValue;
		SetPosition<VECTOR>(pos);

		if (endValue < -100.0f) { return; }//Ç±ÇÍà»ç~Ç…ìñÇΩÇÈÇ±Ç∆ÇÕÇ»Ç¢ÇÃÇ≈èdÇ¢èàóùÇÕè¡Ç∑
		else //ìñÇΩÇËîªíË
		{
			int objectHandle = GetGame<Game>()->GetMainScreen()->mObjectSampler[0]->GetModelHandle();

			//à íuí≤êÆ
			MV1SetScale(objectHandle, VGet(1.0f, 1.0f, mScale));//scaleÇîΩâf
			MV1SetPosition(objectHandle, Math::VectorTransAxis(pos));

			//collisionÇÃçXêV
			GetGame<Game>()->GetMainScreen()->mObjectSampler[0]->RefreshCollision();

			//player capsuleóp
			int plHandle = GetGame<Game>()->GetMainScreen()->mPlayer->GetModelHandle();
			VECTOR posDown = Math::VectorTransAxis(GetGame<Game>()->GetMainScreen()->mPlayer->GetPosition<VECTOR>());
			posDown = VAdd(posDown, VGet(0.0f, PLAYER_RAD_OFFSET, 0.0f));
			VECTOR posUp = VAdd(posDown, VGet(0.0f, PLAYER_RAD_OFFSET, 0.0f));

			//ìñÇΩÇËîªíË
			MV1_COLL_RESULT_POLY_DIM  result = MV1CollCheck_Capsule(objectHandle, 0, posDown, posUp, PLAYER_RAD_OFFSET+50);

			if (result.HitNum >= 1) //hit
			{
				MV1SetMaterialDrawBlendMode(plHandle, 0, DX_BLENDMODE_MUL);
				MV1SetMaterialDrawBlendParam(plHandle, 0, 255);
				GetGame<Game>()->GetMainScreen()->StartTraceEffect(posDown); //start effect
			}
		}
	}
	else
	{
		VECTOR pos = GetPosition<VECTOR>();
		pos.x = GetGame<Game>()->GetMainScreen()->GetPosOffset().first + 50.0f * mLane[1] + 50;//Ç±Ç±Ç≈offsetÇÃâeãø
		pos.y = GetGame<Game>()->GetMainScreen()->GetPosOffset().second
			+endValue; //((ArrivalTime - GetGame<Game>()->GetMainScreen()->mNowTime) / 6000000.0f));
		pos.z = 240.0f + 60.0f; //add characteroffset 
		
		SetPosition<VECTOR>(pos);

		GetGame<Game>()->GetMainScreen()->EndTraceEffect(); //end effect
	}
}

void TraceNotes::Draw()
{
	int objectHandle = GetGame<Game>()->GetMainScreen()->mObjectSampler[0]->GetModelHandle();
	MV1SetScale(objectHandle, VGet(1.0f, 1.0f, mScale));//scaleÇîΩâf
	MV1SetRotationXYZ(objectHandle, VGet(0.0f, mRad, 0.0f));
	VECTOR pos = GetPosition<VECTOR>();
	MV1SetPosition(objectHandle, Math::VectorTransAxis(pos));
	MV1DrawModel(objectHandle);
}

void TraceNotes::SetScale()
{
	float start = GetGame<Game>()->GetMainScreen()->GetPosOffset().second
		+ (+2500.0f * (mArrivalTime[0] / (ONE_TIME * (2.1 - GetGame<Game>()->GetGameSpeed() / 5))));
	float startLane = GetGame<Game>()->GetMainScreen()->GetPosOffset().first + 50.0f * mLane[0] + 50;
	
	float end = GetGame<Game>()->GetMainScreen()->GetPosOffset().second
		+ (+2500.0f * (mArrivalTime[1] / (ONE_TIME * (2.1 - GetGame<Game>()->GetGameSpeed() / 5))));
	float endLane = GetGame<Game>()->GetMainScreen()->GetPosOffset().first + 50.0f * mLane[1] + 50;

	VECTOR startVec = VGet(startLane, start, 0.0f);
	VECTOR endVec = VGet(endLane, end, 0.0f);

	//ãóó£ÇéÊìæÇµÅAägëÂ
	float size = Math::Distance2D(startVec, endVec)/10;
	mScale = size;
	//äpìxÇéÊìæÇµÅAâÒì]
	float rad = Math::Atan2(endVec.x - startVec.x, endVec.y - startVec.y);
	//float rad = Math::Atan2(-startVec.y + endVec.y,-startVec.x + endVec.x);
	mRad = rad;
}
#include "../../main_header/Objects/MusicSelectBox.h"
#include "../../Game.h"
#include "../../main_header/Objects/define.h"
#include "../../main_header/Objects/Font.h"

MusicSelectBox::MusicSelectBox(Game* game)
	:UIScreen(game)
	, mSize(1.0)
	, mIsDraw(true)
{
	mBackground = LoadGraph("object/Select_Music_Background.png");
	GetGraphSize(mBackground, &mBGSize.x, &mBGSize.y);
	mBGPos = Pos(WINDOW_WIDTH/2 - mBGSize.x/2, WINDOW_HEIGHT / 2 - mBGSize.x / 2 + 25);

	mArtwork = LoadGraph("object/Select_Music_Artwork.png");
	GetGraphSize(mArtwork, &mArtworkSize.x, &mArtworkSize.y);
	mArtworkPos = Pos(mBGSize.x / 2 - mArtworkSize.x / 2, mBGSize.y / 2 - mArtworkSize.y / 2);
	mArtworkPosNow = mArtworkPos;

	mMusicLevel = LoadGraph("object/Select_Music_Level.png");
	GetGraphSize(mMusicLevel, &mMusicLevelSize.x, &mMusicLevelSize.y);
	mMusicLevelPos = Pos(mArtworkPos.x - mMusicLevelSize.x / 2, mArtworkPos.y + mArtworkSize.y - mMusicLevelSize.y/2);
	mMusicLevelPosNow = mMusicLevelPos;

	mMusicScore = LoadGraph("object/Select_Music_Score.png");
	GetGraphSize(mMusicScore, &mMusicScoreSize.x, &mMusicScoreSize.y);
	mMusicScorePos = Pos(mMusicLevelPos.x + mMusicLevelSize.x + 10, mArtworkPos.y + mArtworkSize.y + 10);
	mMusicScorePosNow = mMusicScorePos;

	//lv text
	mLvInf.x = LV_TEXT_X_OFFSET;
	mLvInf.y = LV_TEXT_Y_OFFSET;
	mLvInf.color = GetColor(0, 0, 0);
	mLvInf.size = 40;

	//sound information
	mSoundInf.x = SOUND_TEXT_X_OFFSET;
	mSoundInf.y = SOUND_TEXT_Y_OFFSET;
	mSoundInf.color = GetColor(255, 255, 255);
	mSoundInf.size = 30;
}

MusicSelectBox::~MusicSelectBox()
{
	DeleteGraph(mArtwork);
	DeleteGraph(mMusicLevel);
	DeleteGraph(mMusicScore);
}

void MusicSelectBox::Draw()
{
	if (mIsDraw) {
		if (mBackground)
		{
			DrawExtendGraph(mBGPos.x, mBGPos.y,
				mBGPos.x + mBGSize.x * mSize, mBGPos.y + mBGSize.y * mSize,
				mBackground, FALSE);
		}

		DrawExtendGraph(mBGPos.x + mArtworkPosNow.x, mBGPos.y + mArtworkPosNow.y,
			mBGPos.x + mArtworkPosNow.x + mArtworkSize.x * mSize, mBGPos.y + mArtworkPosNow.y + mArtworkSize.y * mSize,
			mArtwork, FALSE);

		DrawExtendGraph(mBGPos.x + mArtworkPosNow.x, mBGPos.y + mArtworkPosNow.y,
			mBGPos.x + mArtworkPosNow.x + mArtworkSize.x * mSize, mBGPos.y + mArtworkPosNow.y + mArtworkSize.y * mSize,
			mImage, FALSE);

		DrawExtendGraph(mBGPos.x + mMusicLevelPosNow.x, mBGPos.y + mMusicLevelPosNow.y,
			mBGPos.x + mMusicLevelPosNow.x + mMusicLevelSize.x * mSize, mBGPos.y + mMusicLevelPosNow.y + mMusicLevelSize.y * mSize,
			mMusicLevel, FALSE);

		DrawExtendGraph(mBGPos.x + mMusicScorePosNow.x, mBGPos.y + mMusicScorePosNow.y,
			mBGPos.x + mMusicScorePosNow.x + mMusicScoreSize.x * mSize, mBGPos.y + mMusicScorePosNow.y + mMusicScoreSize.y * mSize,
			mMusicScore, FALSE);

		//text
		int fontHandle = mFont->GetFont(mLvInf.size * mSize);
		if (fontHandle != -1)
		{
			DrawStringFToHandle(mBGPos.x + mMusicLevelPosNow.x + mLvInf.x * mSize, mBGPos.y + mMusicLevelPosNow.y + mLvInf.y * mSize,
				mLvInf.textInformation.c_str(), mLvInf.color, fontHandle);
		}

		fontHandle = mFont->GetFont(mSoundInf.size * mSize);
		if (fontHandle != -1)
		{
			DrawStringToHandle(mBGPos.x + mSoundInf.x * mSize, mBGPos.y + mSoundInf.y * mSize,
				mSoundInf.textInformation.c_str(), mSoundInf.color, fontHandle);
		}
	}
}
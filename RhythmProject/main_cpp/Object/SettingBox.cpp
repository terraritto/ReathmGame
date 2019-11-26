#include "..\..\main_header\Objects\SettingBox.h"
#include "../../main_header/Objects/define.h"
#include "../../main_header/Objects/Font.h"
#include "../../Game.h"

SettingBox::SettingBox(Game* game)
	: UIScreen(game)
	, mSize(1.0)
	, mIsDraw(false)
{
	mBackground = LoadGraph("object/Select_Setting_Background.png");
	GetGraphSize(mBackground, &mBGSize.x, &mBGSize.y);
	mBGPos = Pos(WINDOW_WIDTH / 2 - mBGSize.x / 2, WINDOW_HEIGHT / 2 - mBGSize.x / 2 + 25);

	mValue = LoadGraph("object/Select_Setting_Value.png");
	GetGraphSize(mValue, &mValueSize.x, &mValueSize.y);
	mValuePos = Pos(0, 80);
	mValuePosNow = mValuePos;

	mText = LoadGraph("object/Select_Setting_Text.png");
	GetGraphSize(mText, &mTextSize.x, &mTextSize.y);
	mTextPos = Pos(0, mValuePos.y + mValueSize.y + 10);
	mTextPosNow = mTextPos;

	mMinus = LoadGraph("object/Select_Setting_Minus.png");
	GetGraphSize(mMinus, &mMinusSize.x, &mMinusSize.y);
	mMinusPos = Pos(3, mValuePos.y + mValueSize.y / 2 - mMinusSize.y / 2);
	mMinusPosNow = mMinusPos;

	mPlus = LoadGraph("object/Select_Setting_Plus.png");
	GetGraphSize(mPlus, &mPlusSize.x, &mPlusSize.y);
	mPlusPos = Pos(mValueSize.x - mPlusSize.x - 3, mValuePos.y + mValueSize.y / 2 - mPlusSize.y / 2);
	mPlusPosNow = mPlusPos;

	//BackGround text
	mBackGroundText.color = GetColor(255, 255, 255);
	mBackGroundText.size = 42;
	mBackGroundText.y = BACKGROUND_TEXT_Y_OFFSET;

	//Value text
	mValueText.color = GetColor(255, 255, 255);
	mValueText.size = 42;
	mValueText.x = VALUE_TEXT_X_OFFSET;
	mValueText.y = VALUE_TEXT_Y_OFFSET;

	//Explain Text
	mExplainText.color = GetColor(0, 0, 0);
	mExplainText.size = 20;
	mExplainText.x = EXPLAIN_TEXT_X_OFFSET;
	mExplainText.y = EXPLAIN_TEXT_Y_OFFSET;
}

SettingBox::~SettingBox()
{
	DeleteGraph(mValue);
	DeleteGraph(mText);
	DeleteGraph(mPlus);
	DeleteGraph(mMinus);
}

void SettingBox::Draw()
{
	if (mIsDraw) {
		if (mBackground)
		{
			DrawExtendGraph(mBGPos.x, mBGPos.y,
				mBGPos.x + mBGSize.x * mSize, mBGPos.y + mBGSize.y * mSize,
				mBackground, FALSE);
		}

		DrawExtendGraph(mBGPos.x + mValuePosNow.x, mBGPos.y + mValuePosNow.y,
			mBGPos.x + mValuePosNow.x + mValueSize.x * mSize, mBGPos.y + mValuePosNow.y + mValueSize.y * mSize,
			mValue, FALSE);

		DrawExtendGraph(mBGPos.x + mTextPosNow.x, mBGPos.y + mTextPosNow.y,
			mBGPos.x + mTextPosNow.x + mTextSize.x * mSize, mBGPos.y + mTextPosNow.y + mTextSize.y * mSize,
			mText, FALSE);

		DrawExtendGraph(mBGPos.x + mMinusPosNow.x, mBGPos.y + mMinusPosNow.y,
			mBGPos.x + mMinusPosNow.x + mMinusSize.x * mSize, mBGPos.y + mMinusPosNow.y + mMinusSize.y * mSize,
			mMinus, TRUE);

		DrawExtendGraph(mBGPos.x + mPlusPosNow.x, mBGPos.y + mPlusPosNow.y,
			mBGPos.x + mPlusPosNow.x + mPlusSize.x * mSize, mBGPos.y + mPlusPosNow.y + mPlusSize.y * mSize,
			mPlus, TRUE);

		//•¶Žšo—Í
		//background
		int fontHandle = mFont->GetFont(mBackGroundText.size * mSize);

		if (fontHandle != -1)
		{
			DrawStringFToHandle(mBGPos.x + mBackGroundText.x * mSize, mBGPos.y + mBackGroundText.y * mSize,
				mBackGroundText.textInformation.c_str(), mBackGroundText.color, fontHandle);
		}

		//value
		fontHandle = mFont->GetFont(mBackGroundText.size * mSize);
		
		if (fontHandle != -1)
		{
			DrawFormatStringToHandle(mBGPos.x + mValuePosNow.x + mValueText.x * mSize, mBGPos.y + mValuePosNow.y + mValueText.y * mSize,
			 mBackGroundText.color, fontHandle,"%.1f",mSettingValue);
		}

		//explain
		fontHandle = mFont->GetFont(mExplainText.size * mSize);

		if (fontHandle != -1)
		{
			DrawStringFToHandle(mBGPos.x + mTextPosNow.x + mExplainText.x * mSize, mBGPos.y + mTextPosNow.y + mExplainText.y * mSize,
				mExplainText.textInformation.c_str(), mExplainText.color, fontHandle);
		}
	}
}
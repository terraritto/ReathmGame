#include "..\..\main_header\Objects\tutorialUI.h"
#include "..\..\main_header\Objects\define.h"
#include "..\..\main_header\Objects\Font.h"

tutorialUI::tutorialUI(Game* game)
	:UIScreen(game)
{
	mBackground = LoadGraph("object/Tutorial_Title.png");
	GetGraphSize(mBackground, &mBGSize.x, &mBGSize.y);

	mBGPos = Pos((WINDOW_WIDTH-mBGSize.x)/2, 0);

	mTitleBg = LoadGraph("object/Tutorial_Textbox.png");
	GetGraphSize(mTitleBg, &mTitleSize.x, &mTitleSize.y);
	mTitlePos = Pos((WINDOW_WIDTH-mTitleSize.x)/2, mBGSize.y);
}

tutorialUI::~tutorialUI()
{
	DeleteGraph(mTitleBg);
}

void tutorialUI::Draw()
{
	UIScreen::Draw();

	DrawGraph(mTitlePos.x, mTitlePos.y, mTitleBg, FALSE);

	//title
	int fontHandle = mFont->GetFont(40);
	if (fontHandle != -1) //if font exist, draw text
	{
		int drawWidth = GetDrawStringWidthToHandle(mTitleText.c_str(),mTitleText.size(),fontHandle);
		DrawStringToHandle((mBGSize.x-drawWidth)/2, mBGSize.y/2, mTitleText.c_str(), GetColor(0,0,0), fontHandle);
	}

	//textbox
	int n = 0;
	fontHandle = mFont->GetFont(14);
	for (auto text : mExplainText)
	{
		int drawWidth = GetDrawStringWidthToHandle(text.c_str(), text.size(), fontHandle);
		DrawStringToHandle(mTitlePos.x + (mTitleSize.x - drawWidth) / 2, 
			mTitlePos.y + n*14 + TUTORIAL_TEXT_OFFSET , text.c_str(), GetColor(0, 0, 0), fontHandle);
		n++;
	}
}

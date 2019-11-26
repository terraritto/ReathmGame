#include "../../main_header/Objects/MainUI.h"
#include "../../main_header/Objects/Font.h"
#include "../../Game.h"
#include "../../main_header/Objects/define.h"
#include "../../main_header/Objects/JacketUI.h"
#include <fstream>
#include <iostream>

MainUI::MainUI(Game* game)
	: UIScreen(game)
	, mJacket(nullptr)
{
	//mBGPos = Pos(0,0);
	//mBackground = LoadGraph("object/HUD_Background.png");
	
	//mJacket = new JacketUI(game);
}

MainUI::~MainUI()
{
	//mJacket->Close();
}

void MainUI::Update(float deltaTime)
{
	UIScreen::Update(deltaTime);

}

void MainUI::Draw()
{
	if (mBackground)
	{
		DrawExtendGraph(mBGPos.x, mBGPos.y, WINDOW_WIDTH, 100, mBackground, TRUE);

	}

	for (auto it : mTitleText)
	{
		int fontHandle = mFont->GetFont(it.size);
		if (fontHandle != -1) //if font exist, draw text
		{
			DrawStringToHandle(it.x, it.y, it.textInformation.c_str(), it.color, fontHandle);
		}
	}

	//update combo
	int fontHandle = mFont->GetFont(30);
	int total = mGame->GetMainScreen()->mTotalCombo;
	int combo = mGame->GetMainScreen()->mCombo;
	int maxCombo = mGame->GetMainScreen()->mMaxCombo;
	DrawFormatStringToHandle(10, 200, GetColor(255, 255, 255), fontHandle, "total: %d / %d", total, maxCombo);
	DrawFormatStringToHandle(10, 235, GetColor(255, 255, 255), fontHandle, "combo: %d", combo);

}
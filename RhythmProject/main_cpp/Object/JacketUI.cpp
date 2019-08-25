#include "../../main_header/Objects/JacketUI.h"
#include "../../main_header/Objects/Font.h"
#include "../../Game.h"
#include "../../main_header/Objects/define.h"
#include <fstream>
#include <iostream>

JacketUI::JacketUI(Game* game)
	: UIScreen(game)
{
	mBGPos = Pos(WINDOW_WIDTH - 100, 0);
	mBackground = LoadGraph("object/soreha.PNG");
}

JacketUI::~JacketUI()
{
}

void JacketUI::Update(float deltaTime)
{
	UIScreen::Update(deltaTime);

}

void JacketUI::Draw()
{
	UIScreen::Draw();
}
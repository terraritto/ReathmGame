#include "../../main_header/Objects/DialogBox.h"
#include "../../Game.h"

DialogBoxClass::DialogBoxClass(Game* game, TextInf& text,
	std::function<void()> onOK)
	: UIScreen(game)
{
	mBGPos = Pos(400, 400);
	mNextButtonPos = Pos(400, 400);

	mBackground = LoadGraph("object/DialogBG.png");
	SetTitleTextAndPos(text);

	TextInf ButtonTemp1("OKButton", mNextButtonPos.x, mNextButtonPos.y, GetColor(0, 0, 255), 20);
	AddButton(ButtonTemp1, [onOK]() {onOK(); });
	TextInf ButtonTemp2("CancelButton", mNextButtonPos.x, mNextButtonPos.y, GetColor(0, 0, 255), 20);
	AddButton(ButtonTemp2, [this]() {Close(); });
}

DialogBoxClass::~DialogBoxClass()
{

}
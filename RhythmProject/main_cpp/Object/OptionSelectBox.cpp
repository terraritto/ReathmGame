#include "..\..\main_header\Objects\OptionSelectBox.h"
#include "../../main_header/Objects/define.h"

OptionSelectBox::OptionSelectBox(Game* game)
	:UIScreen(game)
	,mSize(1.0f)
	,mIsDraw(false)
{

}

OptionSelectBox::~OptionSelectBox()
{
	DeleteGraph(mBackground);
}

void OptionSelectBox::Draw()
{
	if (mIsDraw)
	{
		DrawExtendGraph(mBGPos.x, mBGPos.y,
			mBGPos.x + mBGSize.x * mSize, mBGPos.y + mBGSize.y * mSize,
			mBackground, FALSE);
	}
}

void OptionSelectBox::SetBackGround(std::string fileName)
{
	mBackground = LoadGraph(fileName.c_str());
	GetGraphSize(mBackground, &mBGSize.x, &mBGSize.y);
	mBGPos = Pos(WINDOW_WIDTH / 2 - mBGSize.x / 2, WINDOW_HEIGHT / 2 - mBGSize.x / 2 + 25);
}


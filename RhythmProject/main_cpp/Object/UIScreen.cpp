#include "../../main_header/Objects/UIScreen.h"
#include "../../main_header/Objects/Font.h"
#include "../../main_header/Actors/Actor.h"
#include "../../main_header/Objects/InputSystem.h"
#include "../../Game.h"

UIScreen::UIScreen(Game* game)
	: mGame(game)
	, mState(EActive)
	, mNextButtonPos(Pos(200, 500))
	, mButtonOff(-1)
	, mButtonOn(-1)
	, mBackground(-1)
{
	mGame->PushUI(this);
	mFont = mGame->GetFont("MSƒSƒVƒbƒN");

	mButtonOn = LoadGraph("object/Button_Push.png");
	mButtonOff = LoadGraph("object/Button.png");
}

UIScreen::~UIScreen()
{
	if (mButtonOff != -1) { DeleteGraph(mButtonOff); }
	if (mButtonOn != -1) { DeleteGraph(mButtonOn); }
	if (mBackground != -1) { DeleteGraph(mBackground); }
}

void UIScreen::Update(float deltaTime)
{

}

void UIScreen::Draw()
{
	if (mBackground)
	{
		DrawGraph(mBGPos.x, mBGPos.y, mBackground, TRUE);
	}

	for (auto it : mTitleText)
	{
		int fontHandle = mFont->GetFont(it.size);
		if (fontHandle != -1) //if font exist, draw text
		{
			DrawStringToHandle(it.x, it.y, it.textInformation.c_str(), it.color, fontHandle);
		}
	}

	for (auto b : mButtons)
	{
		int tex = b->GetHighlighted() ? mButtonOn : mButtonOff;
		DrawGraph(b->GetPosition().x, b->GetPosition().y, tex, FALSE);
		TextInf text = b->GetName();
		int fontHandle = mFont->GetFont(text.size);
		if (fontHandle != -1) {
			DrawStringToHandle(text.x, text.y, text.textInformation.c_str(), text.color, fontHandle);
		}
	}
}

void UIScreen::ProcessInput(const InputState& state)
{
	//Do we have buttons?
	if (!mButtons.empty())
	{
		Position mousePos = state.Mouse.GetPosition();
		Pos sendPos(mousePos.x, mousePos.y);

		//Highlight any buttons
		/* 
		//mouse
		for (auto b : mButtons)
		{
			if (b->ContainsPoint(sendPos))
			{
				b->SetHighlighted(true);
			}
			else
			{
				b->SetHighlighted(false);
			}

			if (state.Keyboard.GetKeyState(KEY_INPUT_RETURN) == ButtonState::EPressed)
				if (b->GetHighlighted())
				{
					b->OnClick();
					break;
				}
		}
		*/

		//keyboard
		for (auto b : mButtons)
		{
			if (state.Keyboard.GetKeyState(b->GetInHighLight()) == ButtonState::EPressed)
			{
				b->SetHighlighted(true);
			}
			if(state.Keyboard.GetKeyState(b->GetOutHighLight()) == ButtonState::EPressed)
			{
				b->SetHighlighted(false);
			}

			if (state.Keyboard.GetKeyState(KEY_INPUT_RETURN) == ButtonState::EPressed)
				if (b->GetHighlighted())
				{
					b->OnClick();
					break;
				}
		}
	}
}

void UIScreen::Close()
{
	mState = EClosing;
}

void UIScreen::SetTitleTextAndPos(TextInf& textTitle)
{
	mTitleText.emplace_back(textTitle);
}

void UIScreen::AddButton(const TextInf& name, std::function<void()> onClick)
{
	int texX, texY;
	GetGraphSize(mButtonOn, &texX, &texY);
	Pos dims(texX, texY);
	Button* b = new Button(name, onClick, mNextButtonPos, dims);
	mButtons.emplace_back(b);
	//Update position of next button
	//Move down by height of button plus padding
	GetGraphSize(mButtonOff, &texX, &texY);
	mNextButtonPos.y += texY + 20.0f;
}

void UIScreen::AddButton(const TextInf& name, std::function<void()> onClick, bool highLighted,int in, int out)
{
	int texX, texY;
	GetGraphSize(mButtonOn, &texX, &texY);
	Pos dims(texX, texY);
	Button* b = new Button(name, onClick, Pos(name.x,name.y), dims);
	//--keyboard--
	b->SetHighlighted(highLighted);
	b->SetInHighLight(in);
	b->SetOutHighLight(out);
	//------------
	mButtons.emplace_back(b);
	//Update position of next button
	//Move down by height of button plus padding
	GetGraphSize(mButtonOff, &texX, &texY);
}

Button::Button(const TextInf name,
	std::function<void()> onClick,
	const Pos& pos, const Pos& dims)
	: mOnClick(onClick)
	, mName(name)
	, mPosition(pos)
	, mDimentions(dims)
	, mHighlighted(false)
{

}

Button::~Button()
{
}

bool Button::ContainsPoint(const Pos& pt) const
{
	bool no =
		pt.x < (mPosition.x) ||
		pt.x >(mPosition.x + mDimentions.x) ||
		pt.y < (mPosition.y) ||
		pt.y >(mPosition.y + mDimentions.y);
	return !no;
}

void Button::OnClick()
{
	//call attached handler, if it exists
	if (mOnClick)
	{
		mOnClick();
	}
}
#include "..\..\main_header\Objects\TutorialSelectSceen.h"
#include "..\..\main_header\Objects\define.h"
#include "..\..\Game.h"
#include "..\..\main_header\Objects\Font.h"
#include "..\..\main_header\Objects\InputSystem.h"

TutorialSelectScreen::TutorialSelectScreen(Game* game)
	: UIScreen(game)
{
	mBackground = LoadGraph("object/Select_Background.png");
	mBGPos = Pos(0, 0);

	mSelectMenuBg = LoadGraph("object/Select_Menu_Background.png");
	GetGraphSize(mSelectMenuBg, &mSelectMenuSize.x, &mSelectMenuSize.y);
	mSelectMenuPos = Pos(0, WINDOW_HEIGHT / 2 - mSelectMenuSize.y / 2);

	TextInf ExplainText("チュートリアル",WINDOW_WIDTH/2 - 125,WINDOW_HEIGHT/2 - 50,GetColor(0,0,0),40);
	SetTitleTextAndPos(ExplainText);

	int texX, texY;
	GetGraphSize(mButtonOn, &texX, &texY);
	//play true
	TextInf text_1("プレイしない", WINDOW_WIDTH/2 - texX - 10, WINDOW_HEIGHT/2, GetColor(0, 0, 255), 20);
	AddButton(text_1, [this,game]()
	{
		game->DeleteTutorialSelectScreen(Game::RhythmGame::ESelectScene);
	}
		,false,KEY_INPUT_LEFT,KEY_INPUT_RIGHT
	);

	//play false
	TextInf text_2("プレイする", WINDOW_WIDTH/2 + 10, WINDOW_HEIGHT/2, GetColor(0, 0, 255), 20);
	AddButton(text_2, [this,game]()
	{
		game->DeleteTutorialSelectScreen(Game::RhythmGame::ETutorialScene);
	}
		, true, KEY_INPUT_RIGHT, KEY_INPUT_LEFT
		);
}


TutorialSelectScreen::~TutorialSelectScreen()
{
	DeleteGraph(mSelectMenuBg);
}

void TutorialSelectScreen::Draw()
{
	if (mBackground)
	{
		DrawGraph(mBGPos.x, mBGPos.y, mBackground, TRUE);
	}

	DrawGraph(mSelectMenuPos.x, mSelectMenuPos.y, mSelectMenuBg, FALSE);

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

void TutorialSelectScreen::ProcessInput(const InputState& state)
{
	if (!mButtons.empty())
	{
		//keyboard
		for (auto b : mButtons)
		{
			if (state.Keyboard.GetKeyState(b->GetInHighLight()) == ButtonState::EPressed)
			{
				b->SetHighlighted(true);
				PlayMusic("Music/scroll.wav", DX_PLAYTYPE_BACK);
			}
			if (state.Keyboard.GetKeyState(b->GetOutHighLight()) == ButtonState::EPressed)
			{
				b->SetHighlighted(false);
			}

			if (state.Keyboard.GetKeyState(KEY_INPUT_RETURN) == ButtonState::EPressed)
				if (b->GetHighlighted())
				{
					PlayMusic("Music/select.mp3", DX_PLAYTYPE_BACK);
					b->OnClick();
					break;
				}
		}
	}
}

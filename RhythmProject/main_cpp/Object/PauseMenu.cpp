#include "../../main_header/Objects/PauseMenu.h"
#include "../../main_header/Objects/InputSystem.h"
#include "../../main_header/Objects/DialogBox.h"
#include "../../Game.h"

PauseMenu::PauseMenu(Game* game)
	: UIScreen(game)
{
	mGame->SetState(Game::GameState::EPaused);
	TextInf textTemp("PAUSED", 100, 100, GetColor(0, 0, 255), 72);
	SetTitleTextAndPos(textTemp);

	TextInf ButtonTemp1("ResumeButton", mNextButtonPos.x, mNextButtonPos.y, GetColor(0, 0, 255), 20);
	AddButton(ButtonTemp1, [this]() {Close(); });
	TextInf ButtonTemp2("QuitButton", mNextButtonPos.x, mNextButtonPos.y, GetColor(0, 0, 255), 20);
	DialogBoxTemp = TextInf("QuitText", 400, 600, GetColor(0, 0, 255), 20);
	AddButton(ButtonTemp2, [this]() {
		new DialogBoxClass(mGame, DialogBoxTemp, [this]() {
			mGame->SetState(Game::GameState::EQuit);
			});
		});
}

PauseMenu::~PauseMenu()
{
	mGame->SetState(Game::GameState::EGamePlay);
}

void PauseMenu::ProcessInput(const InputState& state)
{
	UIScreen::ProcessInput(state);
	if (state.Keyboard.GetKeyState(KEY_INPUT_TAB) == EReleased)
	{
		Close();
	}
}
#pragma once
#include "UIScreen.h"

class TutorialSelectScreen :public UIScreen
{
public:
	TutorialSelectScreen(class Game* game);
	~TutorialSelectScreen();

	void ProcessInput(const struct InputState& state) override;
	void Draw() override;
private:
	int mSelectMenuBg; Pos mSelectMenuPos; Pos mSelectMenuSize;
};
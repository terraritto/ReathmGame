#pragma once
#include "UIScreen.h"

class MainUI : public UIScreen
{
public:
	MainUI(class Game* game);
	~MainUI();

	void Update(float deltaTime) override;
	void Draw() override;
private:
	class JacketUI* mJacket;
};
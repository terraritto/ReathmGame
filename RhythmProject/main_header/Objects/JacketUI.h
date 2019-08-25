#pragma once
#include "UIScreen.h"

class JacketUI : public UIScreen
{
public:
	JacketUI(class Game* game);
	~JacketUI();

	void Update(float deltaTime) override;
	void Draw() override;
};
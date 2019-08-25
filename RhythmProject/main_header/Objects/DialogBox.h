#pragma once
#include "UIScreen.h"

class DialogBoxClass : public UIScreen
{
public:
	DialogBoxClass(class Game* game, TextInf& text,
		std::function<void()> onOK);
	~DialogBoxClass();
};
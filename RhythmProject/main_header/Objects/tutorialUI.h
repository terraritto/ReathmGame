#pragma once
#include "UIScreen.h"

class tutorialUI : public UIScreen
{
public:
	tutorialUI(class Game* game);
	~tutorialUI();
	void Draw() override;
	
	void SetTitleText(std::string text) { mTitleText = text; }
	void SetExplainText(std::vector<std::string> text) { mExplainText = text; }
private:
	Pos mBGSize;
	int mTitleBg; Pos mTitlePos; Pos mTitleSize;
	std::string mTitleText;
	std::vector<std::string> mExplainText;
};
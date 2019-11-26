#pragma once
#include "UIScreen.h"
#include "MusicSelectBox.h"
#include "OptionSelectBox.h"
#include "SelectObj.h"
#include "SettingBox.h"
#include <memory>
#include <vector>

class SelectMenu : public UIScreen
{
public:
	enum class SelectState
	{
		ESelectState,
		ESettingState,
		EOptionState
	};
	SelectMenu(class Game* game);
	~SelectMenu();

	void ProcessInput(const struct InputState& state) override;
	void Draw() override;
	void MoveBox(int moveDir);
	void MoveOption(int moveDir);
	void MoveSetting(int moveDir);
	void GameSetting();

	//music
	void StartSelectMusic();
	void StartScrollMusic();
private:
	std::vector<MusicSelectBox*> mBox;
	std::vector<OptionSelectBox*> mOption;
	std::vector<SelectObj*> mSelectObj;
	std::vector<SettingBox*> mSetting;

	int mBiGImageIndex;
	int mOptionBigImageIndex;
	int mSettingBigImageIndex;
	SelectState mState;
	//time
	int mInputTimeManager;
	//image
	int mSelectMenuBg; Pos mSelectMenuPos; Pos mSelectMenuSize;
	int mSelectTab; Pos mSelectTabPos; TextInf mTabText;
	//sound
	std::vector<int> mSelectSound; int mSelectSoundCount;
	std::vector<int> mScrollSound; int mScrollSoundCount;
};
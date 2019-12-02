#include "../../main_header/Objects/SelectMenu.h"
#include "../../Game.h"
#include "../../main_header/Objects/define.h"
#include "../../main_header/Objects/InputSystem.h"
#include "../../main_header/Objects/Font.h"

#include "../../sqlite3.h"

SelectMenu::SelectMenu(Game* game)
	:UIScreen(game)
	,mBiGImageIndex(0)
	,mOptionBigImageIndex(1)
	,mState(SelectState::ESelectState)
	,mInputTimeManager(GetNowCount())
{
	int width = 0, height = 0;

	// image input
	mBackground = LoadGraph("object/Select_Background.png");
	mBGPos = Pos(0, 0);

	mSelectMenuBg = LoadGraph("object/Select_Menu_Background.png");
	GetGraphSize(mSelectMenuBg, &mSelectMenuSize.x, &mSelectMenuSize.y);
	mSelectMenuPos = Pos(0, WINDOW_HEIGHT / 2 - mSelectMenuSize.y / 2);
	
	mSelectTab = LoadGraph("object/Select_Tab.png");
	GetGraphSize(mSelectTab, &width, &height);
	mSelectTabPos = Pos(WINDOW_WIDTH / 2 - width / 2, mSelectMenuPos.y + 10);

	mTabText.color = GetColor(255, 255, 255);
	mTabText.size = 40;
	mTabText.x = 75;
	mTabText.y = 40;
	mTabText.textInformation = "ORIGINAL";
	//-----box input-----
	//open
	std::string dbName = "Text/music.db";
	sqlite3* db;
	auto status = sqlite3_open(dbName.c_str(), &db);

	if (status != SQLITE_OK)
	{
		ErrorLogAdd("error for opening db \n");
	}
	else
	{
		ErrorLogAdd("open successed! \n");
	}

	//select
	sqlite3_stmt* stmt = nullptr;
	char* errMsg = NULL;
	auto err = sqlite3_prepare_v2(db, "SELECT * FROM MUSICDATA", 64, &stmt, nullptr);

	if (err != SQLITE_OK)
	{
		ErrorLogAdd(errMsg);
		sqlite3_free(errMsg);
		errMsg = NULL;
	}
	else
	{
		while (SQLITE_ROW == (err = sqlite3_step(stmt)))
		{
			int id = sqlite3_column_int(stmt, 0);
			const unsigned char* notesFile = sqlite3_column_text(stmt, 1);
			const unsigned char* soundFile = sqlite3_column_text(stmt, 2);
			const unsigned char* imageFile = sqlite3_column_text(stmt, 3);
			const unsigned char* soundName = sqlite3_column_text(stmt, 4);
			int lv = sqlite3_column_int(stmt, 5);

			MusicSelectBox* box = new MusicSelectBox(game);
			std::string text = reinterpret_cast<const char*>(notesFile);
			box->SetNotesFile(text);
			text = reinterpret_cast<const char*>(soundFile);
			text = "Music/" + text;
			box->SetSoundFile(text);
			text = reinterpret_cast<const char*>(imageFile);
			text = "object/" + text;
			box->SetImage(text);
			text = reinterpret_cast<const char*>(soundName);
			box->SetSoundInf(text);
			box->SetLvInf(lv);

			mBox.emplace_back(box);
		}
	}
	sqlite3_finalize(stmt);
	
	//close
	err = sqlite3_close(db);
	if (err != SQLITE_OK)
	{
		ErrorLogAdd("error for closing db\n");
	}
	else
	{
		ErrorLogAdd("close successed!\n");
	}

	
	for (int i = 1; i < mBox.size(); i++)
	{
		mBox[i]->SetSize(0.65);
	}

	//-------------------
	//init box
	for (int i = 1; i < mBox.size(); i++)
	{
		mBox[i]->SetSpaceX(mBox[i - 1]->GetPosX() + mBox[i - 1]->GetBGSizeX() * mBox[i - 1]->GetSize() + BOX_LEFT_SPACE);
		mBox[i]->SetSpaceY(BOX_UP_SPACE);
	}

	//setting input
	mSetting.emplace_back(new SettingBox(game));
	for (int i = 1; i <= 2; i++)
	{
		mSetting.emplace_back(new SettingBox(game));
		mSetting[i]->SetSize(0.65);
	}
	//Setting init
	for (int i = 1; i < mSetting.size(); i++)
	{
		mSetting[i]->SetSpaceX(mSetting[i - 1]->GetPosX() + mSetting[i - 1]->GetBGSizeX() * mSetting[i - 1]->GetSize() + BOX_LEFT_SPACE);
		mSetting[i]->SetSpaceY(BOX_UP_SPACE);
	}

	mSetting[0]->SetBackGroundText("スピード");
	mSetting[0]->SetBackGroundTextPosX(BACKGROUND_TEXT_X_SPEED_OFFSET);
	mSetting[0]->SetSettingValue(5.0f);
	mSetting[0]->SetSettingLimitMin(0.5f);
	mSetting[0]->SetSettingLimitMax(10.0f);
	mSetting[0]->SetExplainText
	("      ノーツの速度を\n        設定します");

	mSetting[1]->SetBackGroundText("判定タイミング");
	mSetting[1]->SetBackGroundTextPosX(BACKGROUND_TEXT_X_JUDGE_OFFSET);
	mSetting[1]->SetSettingValue(0.0f);
	mSetting[1]->SetSettingLimitMin(-10.0f);
	mSetting[1]->SetSettingLimitMax(+10.0f);
	mSetting[1]->SetExplainText
	("  ノーツの判定タイミングを\n         設定します");

	mSetting[2]->SetBackGroundText("音量");
	mSetting[2]->SetBackGroundTextPosX(BACKGROUND_TEXT_X_VOLUME_OFFSET);
	mSetting[2]->SetSettingValue(8.0f);
	mSetting[2]->SetSettingLimitMin(0.0f);
	mSetting[2]->SetSettingLimitMax(10.0f);
	mSetting[2]->SetExplainText
	("      ゲーム内の音量を\n         調節します");

	//Option input
	OptionSelectBox* opt = new OptionSelectBox(game);
	opt->SetBackGround("object/Select_GoBack.png");
	opt->SetSize(0.65);
	opt->SetSpaceY(BOX_UP_SPACE);
	opt->SetState(OptionSelectBox::OptionState::EBack);
	mOption.emplace_back(opt);

	opt = new OptionSelectBox(game);
	opt->SetBackGround("object/Select_BattleStart.png");
	opt->SetState(OptionSelectBox::OptionState::EStart);
	mOption.emplace_back(opt);

	opt = new OptionSelectBox(game);
	opt->SetBackGround("object/Select_Option.png");
	opt->SetSize(0.65);
	opt->SetSpaceY(BOX_UP_SPACE);
	opt->SetState(OptionSelectBox::OptionState::ESetting);
	mOption.emplace_back(opt);

	//init option
	mOption[0]->SetSpaceX(mOption[0]->GetPosX() - mOption[0]->GetBGSizeX() * mOption[0]->GetSize() - BOX_LEFT_SPACE);

	for (int i = 1; i < mOption.size(); i++)
	{
		mOption[i]->SetSpaceX(mOption[i - 1]->GetPosX() + mOption[i - 1]->GetBGSizeX() * mOption[i - 1]->GetSize() + BOX_LEFT_SPACE);
	}

	//select obj input
	SelectObj* object = new SelectObj(game);
	object->SetImage("object/Select_Current.png");
	object->SetPos(Pos(mBox[0]->GetPosX() - 10, mBox[0]->GetPosY() - 10));
	mSelectObj.emplace_back(object);

	object = new SelectObj(game);
	object->SetImage("object/Select_Arrow_Left.png");
	object->SetPos(Pos(mBox[0]->GetPosX() - 80, WINDOW_HEIGHT / 2 + 25 - (object->mObjSize.y) / 2 + ALLOW_DOWN));
	mSelectObj.emplace_back(object);

	object = new SelectObj(game);
	object->SetImage("object/Select_Arrow_Right.png");
	object->SetPos(Pos(mBox[0]->GetPosX() + mBox[0]->GetBGSizeX() - 20, WINDOW_HEIGHT / 2 + 25 - (object->mObjSize.y) / 2 + ALLOW_DOWN));
	mSelectObj.emplace_back(object);

	//sound
	for (int i = 0; i < 20; i++)
	{
		mScrollSound.emplace_back(LoadSoundMem("Music/scroll.wav"));
		ChangeVolumeSoundMem(255, mScrollSound[i]);
	}
	for (int i = 0; i < 10; i++)
	{
		mSelectSound.emplace_back(LoadSoundMem("Music/select.mp3"));
		ChangeVolumeSoundMem(255, mSelectSound[i]);
	}
	mScrollSoundCount = 0;
	mSelectSoundCount = 0;
}

SelectMenu::~SelectMenu()
{
	DeleteGraph(mSelectMenuBg);
	DeleteGraph(mSelectTab);

	for (auto object : mBox)
	{
		object->Close();
	}

	for (auto object : mOption)
	{
		object->Close();
	}

	for (auto object : mSelectObj)
	{
		object->Close();
	}

	for (auto object : mSetting)
	{
		object->Close();
	}
}

void SelectMenu::ProcessInput(const InputState& state)
{
	UIScreen::ProcessInput(state);
	if (GetNowCount() - mInputTimeManager <= 100) { return; }

	if (mState == SelectState::ESelectState) {
		if (state.Keyboard.GetKeyState(KEY_INPUT_RIGHT) == ButtonState::EPressed)
		{
			MoveBox(+1);
			StartScrollMusic();
			mInputTimeManager = GetNowCount();
		}

		if (state.Keyboard.GetKeyState(KEY_INPUT_LEFT) == ButtonState::EPressed)
		{
			MoveBox(-1);
			StartScrollMusic();
			mInputTimeManager = GetNowCount();

		}

		if (state.Keyboard.GetKeyState(KEY_INPUT_RETURN) == ButtonState::EPressed)
		{
			mState = SelectState::ESettingState;

			for (auto object : mBox)
			{
				object->SetDraw(false);
			}

			for (auto object : mOption)
			{
				object->SetDraw(true);
			}
			
			StartSelectMusic();
			mInputTimeManager = GetNowCount();

		}
		return;
	}

	if (mState == SelectState::ESettingState)
	{
		if (state.Keyboard.GetKeyState(KEY_INPUT_RIGHT) == ButtonState::EPressed)
		{
			MoveOption(+1);
			StartScrollMusic();
			mInputTimeManager = GetNowCount();
		}

		if (state.Keyboard.GetKeyState(KEY_INPUT_LEFT) == ButtonState::EPressed)
		{
			MoveOption(-1);
			StartScrollMusic();
			mInputTimeManager = GetNowCount();
		}

		if (state.Keyboard.GetKeyState(KEY_INPUT_RETURN) == ButtonState::EPressed)
		{
			if (mOption[mOptionBigImageIndex]->GetState() == OptionSelectBox::OptionState::EBack) {
				mState = SelectState::ESelectState;

				for (auto object : mBox)
				{
					object->SetDraw(true);
				}

				for (auto object : mOption)
				{
					object->SetDraw(false);
				}
			}

			if (mOption[mOptionBigImageIndex]->GetState() == OptionSelectBox::OptionState::EStart) 
			{
				mGame->DeleteSelectScreen(Game::RhythmGame::EGameScene);

			}

			if (mOption[mOptionBigImageIndex]->GetState() == OptionSelectBox::OptionState::ESetting) 
			{
				mState = SelectState::EOptionState;

				for (auto object : mOption)
				{
					object->SetDraw(false);
				}

				for (auto object : mSetting)
				{
					object->SetDraw(true);
				}
			}
			
			StartSelectMusic();
			mInputTimeManager = GetNowCount();
		}
		return;
	}

	if (mState == SelectState::EOptionState)
	{
		if (state.Keyboard.GetKeyState(KEY_INPUT_RIGHT) == ButtonState::EPressed)
		{
			MoveSetting(+1);
			StartScrollMusic();
			mInputTimeManager = GetNowCount();
		}

		if (state.Keyboard.GetKeyState(KEY_INPUT_LEFT) == ButtonState::EPressed)
		{
			MoveSetting(-1);
			StartScrollMusic();
			mInputTimeManager = GetNowCount();
		}

		if (state.Keyboard.GetKeyState(KEY_INPUT_S) == ButtonState::EPressed)
		{
			mSetting[mSettingBigImageIndex]->MoveValue(-0.5);
			StartScrollMusic();
		}

		if (state.Keyboard.GetKeyState(KEY_INPUT_F) == ButtonState::EPressed)
		{
			mSetting[mSettingBigImageIndex]->MoveValue(0.5);
			StartScrollMusic();
		}

		if (state.Keyboard.GetKeyState(KEY_INPUT_RETURN) == ButtonState::EPressed)
		{
			mState = SelectState::ESettingState;

			for (auto object : mSetting)
			{
				object->SetDraw(false);
			}

			for (auto object : mOption)
			{
				object->SetDraw(true);
			}

			StartSelectMusic();
			mInputTimeManager = GetNowCount();
		}
		return;

	}
}

void SelectMenu::Draw()
{
	UIScreen::Draw();

	DrawGraph(mSelectMenuPos.x, mSelectMenuPos.y, mSelectMenuBg, FALSE);

	DrawGraph(mSelectTabPos.x, mSelectTabPos.y, mSelectTab, TRUE);

	int fontHandle = mFont->GetFont(mTabText.size);

	if (fontHandle != -1)
	{
		DrawStringFToHandle(mSelectTabPos.x + mTabText.x, mSelectTabPos.y + mTabText.y,
			mTabText.textInformation.c_str(), mTabText.color, fontHandle);
	}
}

void SelectMenu::MoveBox(int moveDir)
{
	//特異値
	if (mBiGImageIndex + moveDir == mBox.size())
	{
		mBox[mBiGImageIndex]->SetSize(0.65);
		mBox[mBiGImageIndex]->SetSpaceY(BOX_UP_SPACE);
		mBox[0]->SetSize(1.0);
		mBox[0]->SetSpaceY(-BOX_UP_SPACE);
		mBox[0]->SetSpaceX(WINDOW_WIDTH / 2 - mBox[0]->GetBGSizeX() / 2);
		
		for (int i = 1; i < mBox.size(); i++)
		{
			mBox[i]->SetSpaceX(mBox[i - 1]->GetPosX() + mBox[i - 1]->GetBGSizeX() * mBox[i - 1]->GetSize() + BOX_LEFT_SPACE);
		}
		mBiGImageIndex = 0;
		return;
	}

	if (mBiGImageIndex + moveDir == -1)
	{
		mBox[mBiGImageIndex]->SetSize(0.65);
		mBox[mBiGImageIndex]->SetSpaceY(BOX_UP_SPACE);
		mBox[mBox.size()-1]->SetSize(1.0);
		mBox[mBox.size()-1]->SetSpaceY(-BOX_UP_SPACE);
		mBox[0]->SetSpaceX(mBox[0]->GetPosX() + mBox[0]->GetBGSizeX() * mBox[0]->GetSize() + BOX_LEFT_SPACE);

		mBox[0]->SetSpaceX(mBox[0]->GetPosX() - mBox[0]->GetBGSizeX() * mBox[0]->GetSize() * (mBox.size())
			- BOX_LEFT_SPACE * (mBox.size()));

		for (int i = 1; i < mBox.size(); i++)
		{
			mBox[i]->SetSpaceX(mBox[i - 1]->GetPosX() + mBox[i - 1]->GetBGSizeX() * mBox[i - 1]->GetSize() + BOX_LEFT_SPACE);
		}
		mBiGImageIndex = mBox.size() - 1;
		return;
	}

	//普通の処理
	mBox[mBiGImageIndex]->SetSize(0.65);
	mBox[mBiGImageIndex]->SetSpaceY(BOX_UP_SPACE);
	mBox[mBiGImageIndex + moveDir]->SetSize(1.0);
	mBox[mBiGImageIndex + moveDir]->SetSpaceY(-BOX_UP_SPACE);
	mBiGImageIndex += moveDir;

	if (moveDir == 1)
	{
		mBox[0]->SetSpaceX(mBox[0]->GetPosX() - mBox[0]->GetBGSizeX() * mBox[0]->GetSize() - BOX_LEFT_SPACE);

		for (int i = 1; i < mBox.size(); i++)
		{
			mBox[i]->SetSpaceX(mBox[i - 1]->GetPosX() + mBox[i-1]->GetBGSizeX() * mBox[i-1]->GetSize() + BOX_LEFT_SPACE);
		}
	}
	else
	{
		mBox[0]->SetSpaceX(mBox[0]->GetPosX() + mBox[0]->GetBGSizeX() * mBox[0]->GetSize() + BOX_LEFT_SPACE);

		if (mBiGImageIndex == 0) {
			mBox[0]->SetSpaceX(WINDOW_WIDTH / 2 - mBox[0]->GetBGSizeX() / 2);

		}

		for (int i = 1; i < mBox.size(); i++)
		{
			mBox[i]->SetSpaceX(mBox[i - 1]->GetPosX() + mBox[i - 1]->GetBGSizeX() * mBox[i - 1]->GetSize() + BOX_LEFT_SPACE);
		}
	}
}

void SelectMenu::MoveOption(int moveDir)
{
	//特異値
	if (mOptionBigImageIndex + moveDir == mOption.size())
	{
		return;
	}

	if (mOptionBigImageIndex + moveDir == -1)
	{
		return;
	}

	//普通の処理
	mOption[mOptionBigImageIndex]->SetSize(0.65);
	mOption[mOptionBigImageIndex]->SetSpaceY(BOX_UP_SPACE);
	mOption[mOptionBigImageIndex + moveDir]->SetSize(1.0);
	mOption[mOptionBigImageIndex + moveDir]->SetSpaceY(-BOX_UP_SPACE);
	mOptionBigImageIndex += moveDir;

	if (moveDir == 1)
	{
		mOption[0]->SetSpaceX(mOption[0]->GetPosX() - mOption[0]->GetBGSizeX() * mOption[0]->GetSize() - BOX_LEFT_SPACE);

		for (int i = 1; i < mOption.size(); i++)
		{
			mOption[i]->SetSpaceX(mOption[i - 1]->GetPosX() + mOption[i - 1]->GetBGSizeX() * mOption[i - 1]->GetSize() + BOX_LEFT_SPACE);
		}
	}
	else
	{
		mOption[0]->SetSpaceX(mOption[0]->GetPosX() + mOption[0]->GetBGSizeX() * mOption[0]->GetSize() + BOX_LEFT_SPACE);

		if (mOptionBigImageIndex == 0) {
			mOption[0]->SetSpaceX(WINDOW_WIDTH / 2 - mOption[0]->GetBGSizeX() / 2);
		}

		for (int i = 1; i < mOption.size(); i++)
		{
			mOption[i]->SetSpaceX(mOption[i - 1]->GetPosX() + mOption[i - 1]->GetBGSizeX() * mOption[i - 1]->GetSize() + BOX_LEFT_SPACE);
		}
	}
}

void SelectMenu::MoveSetting(int moveDir)
{
	//特異値
	if (mSettingBigImageIndex + moveDir == mSetting.size())
	{
		mSetting[mSettingBigImageIndex]->SetSize(0.65);
		mSetting[mSettingBigImageIndex]->SetSpaceY(BOX_UP_SPACE);
		mSetting[0]->SetSize(1.0);
		mSetting[0]->SetSpaceY(-BOX_UP_SPACE);
		mSetting[0]->SetSpaceX(WINDOW_WIDTH / 2 - mSetting[0]->GetBGSizeX() / 2);

		for (int i = 1; i < mSetting.size(); i++)
		{
			mSetting[i]->SetSpaceX(mSetting[i - 1]->GetPosX() + mSetting[i - 1]->GetBGSizeX() * mSetting[i - 1]->GetSize() + BOX_LEFT_SPACE);
		}
		mSettingBigImageIndex = 0;
		return;
	}

	if (mSettingBigImageIndex + moveDir == -1)
	{
		mSetting[mSettingBigImageIndex]->SetSize(0.65);
		mSetting[mSettingBigImageIndex]->SetSpaceY(BOX_UP_SPACE);
		mSetting[mSetting.size() - 1]->SetSize(1.0);
		mSetting[mSetting.size() - 1]->SetSpaceY(-BOX_UP_SPACE);
		mSetting[0]->SetSpaceX(mSetting[0]->GetPosX() + mSetting[0]->GetBGSizeX() * mSetting[0]->GetSize() + BOX_LEFT_SPACE);

		mSetting[0]->SetSpaceX(mSetting[0]->GetPosX() - mSetting[0]->GetBGSizeX() * mSetting[0]->GetSize() * (mSetting.size())
			- BOX_LEFT_SPACE * (mSetting.size()));

		for (int i = 1; i < mSetting.size(); i++)
		{
			mSetting[i]->SetSpaceX(mSetting[i - 1]->GetPosX() + mSetting[i - 1]->GetBGSizeX() * mSetting[i - 1]->GetSize() + BOX_LEFT_SPACE);
		}
		mSettingBigImageIndex = mSetting.size() - 1;
		return;
	}

	//普通の処理
	mSetting[mSettingBigImageIndex]->SetSize(0.65);
	mSetting[mSettingBigImageIndex]->SetSpaceY(BOX_UP_SPACE);
	mSetting[mSettingBigImageIndex + moveDir]->SetSize(1.0);
	mSetting[mSettingBigImageIndex + moveDir]->SetSpaceY(-BOX_UP_SPACE);
	mSettingBigImageIndex += moveDir;

	if (moveDir == 1)
	{
		mSetting[0]->SetSpaceX(mSetting[0]->GetPosX() - mSetting[0]->GetBGSizeX() * mSetting[0]->GetSize() - BOX_LEFT_SPACE);

		for (int i = 1; i < mSetting.size(); i++)
		{
			mSetting[i]->SetSpaceX(mSetting[i - 1]->GetPosX() + mSetting[i - 1]->GetBGSizeX() * mSetting[i - 1]->GetSize() + BOX_LEFT_SPACE);
		}
	}
	else
	{
		mSetting[0]->SetSpaceX(mSetting[0]->GetPosX() + mSetting[0]->GetBGSizeX() * mSetting[0]->GetSize() + BOX_LEFT_SPACE);

		if (mSettingBigImageIndex == 0) {
			mSetting[0]->SetSpaceX(WINDOW_WIDTH / 2 - mSetting[0]->GetBGSizeX() / 2);

		}

		for (int i = 1; i < mSetting.size(); i++)
		{
			mSetting[i]->SetSpaceX(mSetting[i - 1]->GetPosX() + mSetting[i - 1]->GetBGSizeX() * mSetting[i - 1]->GetSize() + BOX_LEFT_SPACE);
		}
	}
}

void SelectMenu::GameSetting()
{
	mGame->SetGameSpeed(mSetting[0]->GetSettingValue());
	mGame->SetGameTiming(mSetting[1]->GetSettingValue());
	mGame->SetGameVolume(mSetting[2]->GetSettingValue());
	mGame->SetGameNortsFile(mBox[mBiGImageIndex]->GetNotesFile());
}

void SelectMenu::StartSelectMusic()
{
	PlaySoundMem(mSelectSound[mSelectSoundCount], DX_PLAYTYPE_BACK);
	mSelectSoundCount++;
	if (mSelectSound.size() == mSelectSoundCount) { mSelectSoundCount = 0; }
}

void SelectMenu::StartScrollMusic()
{
	PlaySoundMem(mScrollSound[mScrollSoundCount], DX_PLAYTYPE_BACK);
	mScrollSoundCount++;
	if (mScrollSound.size() == mScrollSoundCount) { mScrollSoundCount = 0; }
}

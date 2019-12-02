#include "../../main_header/Objects/Player.h"
#include "../../main_header/Objects/InputSystem.h"
#include "../../Game.h"
#include "../../main_header/Objects/MainScreen.h"
#include "../../main_header/wrapper/Math.h"
#include <string>
#include "DxLib.h"

Player::Player(Game* game,MainScreen* main)
	:Actor(game)
{
	SetModelHandle<std::string>("object/Chara01.mv1");
	SetPosition<VECTOR>(VGet(main->GetPosOffset().first + 100.0f * 3,  //レーン位置
		main->GetPosOffset().second, //y軸位置
		10.0f));
}

Player::~Player()
{

}

void Player::ActorInput(const InputState& keyState)
{
	for (int i = 0; i < mIsInputRight.size(); i++)
	{
		mIsInputRight[i] = false;
		mIsInputLeft[i] = false;
	}
	
	for (int i = 0; i < mIsLongInputRight.size(); i++)
	{
		mIsLongInputRight[i] = false;
		mIsLongInputLeft[i] = false;
	}

	VECTOR pos = GetPosition<VECTOR>();
	/*keyboard*/
	if (keyState.Keyboard.GetKeyValue(KEY_INPUT_LEFT) == 1)
	{
		pos.x -= mSpeed;
	}

	if (keyState.Keyboard.GetKeyValue(KEY_INPUT_RIGHT) == 1)
	{
		pos.x += mSpeed;
	}

	if (keyState.Keyboard.GetKeyValue(KEY_INPUT_UP) == 1)
	{
		pos.z += mSpeed;
	}

	if (keyState.Keyboard.GetKeyValue(KEY_INPUT_DOWN) == 1)
	{
		pos.z -= mSpeed;
	}

	if (GetGame<Game>()->GetMainScreen()->GetScene() == MainScreen::MainScene::StartScene) {
		//norts
		if (keyState.Keyboard.GetKeyState(KEY_INPUT_S) == EPressed)
		{
			mInputTimeRight[static_cast<int>(EColor::ERed)] = GetNowHiPerformanceCount();
			mIsInputRight[static_cast<int>(EColor::ERed)] = true;
		}

		if (keyState.Keyboard.GetKeyState(KEY_INPUT_J) == EPressed)
		{
			mInputTimeLeft[static_cast<int>(EColor::ERed)] = GetNowHiPerformanceCount();
			mIsInputLeft[static_cast<int>(EColor::ERed)] = true;
		}

		//longnorts
		if (keyState.Keyboard.GetKeyState(KEY_INPUT_S) == EPressed)
		{
			mInputLongTimeRight[static_cast<int>(EColor::ERed)] = GetNowHiPerformanceCount();
			mIsLongInputRight[static_cast<int>(EColor::ERed)] = true;
		}

		if (keyState.Keyboard.GetKeyState(KEY_INPUT_S) == EHeld)
		{
			mInputLongTimeRight[static_cast<int>(EColor::ERed)+1] = GetNowHiPerformanceCount();
			mIsLongInputRight[static_cast<int>(EColor::ERed)+1] = true;
		}

		if (keyState.Keyboard.GetKeyState(KEY_INPUT_S) == EReleased)
		{
			mInputLongTimeRight[static_cast<int>(EColor::ERed)+2] = GetNowHiPerformanceCount();
			mIsLongInputRight[static_cast<int>(EColor::ERed)+2] = true;
		}

		if (keyState.Keyboard.GetKeyState(KEY_INPUT_J) == EPressed)
		{
			mInputLongTimeLeft[static_cast<int>(EColor::ERed)] = GetNowHiPerformanceCount();
			mIsLongInputLeft[static_cast<int>(EColor::ERed)] = true;
		}

		if (keyState.Keyboard.GetKeyState(KEY_INPUT_J) == EHeld)
		{
			mInputLongTimeLeft[static_cast<int>(EColor::ERed) + 1] = GetNowHiPerformanceCount();
			mIsLongInputLeft[static_cast<int>(EColor::ERed) + 1] = true;
		}

		if (keyState.Keyboard.GetKeyState(KEY_INPUT_J) == EReleased)
		{
			mInputLongTimeLeft[static_cast<int>(EColor::ERed) + 2] = GetNowHiPerformanceCount();
			mIsLongInputLeft[static_cast<int>(EColor::ERed) + 2] = true;
		}

		//norts
		if (keyState.Keyboard.GetKeyState(KEY_INPUT_D) == EPressed)
		{
			mInputTimeRight[static_cast<int>(EColor::EGreen)] = GetNowHiPerformanceCount();
			mIsInputRight[static_cast<int>(EColor::EGreen)] = true;
		}

		if (keyState.Keyboard.GetKeyState(KEY_INPUT_K) == EPressed)
		{
			mInputTimeLeft[static_cast<int>(EColor::EGreen)] = GetNowHiPerformanceCount();
			mIsInputLeft[static_cast<int>(EColor::EGreen)] = true;
		}

		//long norts
		if (keyState.Keyboard.GetKeyState(KEY_INPUT_D) == EPressed)
		{
			int col = static_cast<int>(EColor::EGreen) * 3;
			mInputLongTimeRight[col] = GetNowHiPerformanceCount();
			mIsLongInputRight[col] = true;
		}

		if (keyState.Keyboard.GetKeyState(KEY_INPUT_D) == EHeld)
		{
			int col = static_cast<int>(EColor::EGreen) * 3 + 1;

			mInputLongTimeRight[col] = GetNowHiPerformanceCount();
			mIsLongInputRight[col] = true;
		}

		if (keyState.Keyboard.GetKeyState(KEY_INPUT_D) == EReleased)
		{
			int col = static_cast<int>(EColor::EGreen) * 3 + 2;
			mInputLongTimeRight[col] = GetNowHiPerformanceCount();
			mIsLongInputRight[col] = true;
		}

		if (keyState.Keyboard.GetKeyState(KEY_INPUT_K) == EPressed)
		{
			mInputLongTimeLeft[static_cast<int>(EColor::EGreen)*3] = GetNowHiPerformanceCount();
			mIsLongInputLeft[static_cast<int>(EColor::EGreen)*3] = true;
		}

		if (keyState.Keyboard.GetKeyState(KEY_INPUT_K) == EHeld)
		{
			mInputLongTimeLeft[static_cast<int>(EColor::EGreen)*3 + 1] = GetNowHiPerformanceCount();
			mIsLongInputLeft[static_cast<int>(EColor::EGreen)*3 + 1] = true;
		}

		if (keyState.Keyboard.GetKeyState(KEY_INPUT_K) == EReleased)
		{
			mInputLongTimeLeft[static_cast<int>(EColor::EGreen)*3 + 2] = GetNowHiPerformanceCount();
			mIsLongInputLeft[static_cast<int>(EColor::EGreen)*3 + 2] = true;
		}

		//norts
		if (keyState.Keyboard.GetKeyState(KEY_INPUT_F) == EPressed)
		{
			mInputTimeRight[static_cast<int>(EColor::EBlue)] = GetNowHiPerformanceCount();
			mIsInputRight[static_cast<int>(EColor::EBlue)] = true;
		}

		if (keyState.Keyboard.GetKeyState(KEY_INPUT_L) == EPressed)
		{
			mInputTimeLeft[static_cast<int>(EColor::EBlue)] = GetNowHiPerformanceCount();
			mIsInputLeft[static_cast<int>(EColor::EBlue)] = true;
		}

		//long norts
		if (keyState.Keyboard.GetKeyState(KEY_INPUT_F) == EPressed)
		{
			mInputLongTimeRight[static_cast<int>(EColor::EBlue)*3] = GetNowHiPerformanceCount();
			mIsLongInputRight[static_cast<int>(EColor::EBlue)*3] = true;
		}

		if (keyState.Keyboard.GetKeyState(KEY_INPUT_F) == EHeld)
		{
			mInputLongTimeRight[static_cast<int>(EColor::EBlue)*3 + 1] = GetNowHiPerformanceCount();
			mIsLongInputRight[static_cast<int>(EColor::EBlue)*3 + 1] = true;
		}

		if (keyState.Keyboard.GetKeyState(KEY_INPUT_F) == EReleased)
		{
			mInputLongTimeRight[static_cast<int>(EColor::EBlue)*3 + 2] = GetNowHiPerformanceCount();
			mIsLongInputRight[static_cast<int>(EColor::EBlue)*3 + 2] = true;
		}

		if (keyState.Keyboard.GetKeyState(KEY_INPUT_L) == EPressed)
		{
			mInputLongTimeLeft[static_cast<int>(EColor::EBlue)*3] = GetNowHiPerformanceCount();
			mIsLongInputLeft[static_cast<int>(EColor::EBlue)*3] = true;
		}

		if (keyState.Keyboard.GetKeyState(KEY_INPUT_L) == EHeld)
		{
			mInputLongTimeLeft[static_cast<int>(EColor::EBlue)*3 + 1] = GetNowHiPerformanceCount();
			mIsLongInputLeft[static_cast<int>(EColor::EBlue)*3 + 1] = true;
		}

		if (keyState.Keyboard.GetKeyState(KEY_INPUT_L) == EReleased)
		{
			mInputLongTimeLeft[static_cast<int>(EColor::EBlue)*3 + 2] = GetNowHiPerformanceCount();
			mIsLongInputLeft[static_cast<int>(EColor::EBlue)*3 + 2] = true;
		}
	}
	pos.x = Math::Clamp(pos.x, -260.0f, 260.0f);
	//SetPosition(pos);

	/*joystick*/
	if (keyState.Controller.GetIsConnected())
	{
		pos.x = (keyState.Controller.GetStick().x / 1000.0f) * 260.0f;
		if (keyState.Controller.GetButtonValue(1))
		{
			pos.z += PLAYER_SPEED;
		}
		else
		{
			pos.z -= PLAYER_SPEED;
		}
	}
	pos.z = Math::Clamp(pos.z, 0.0f, 240.0f);

	SetPosition(pos);
}

void Player::UpdateActor(float deltaTime)
{
	VECTOR pos = Math::VectorTransAxis(GetPosition<VECTOR>());
	
	//playerより後にnotes objectは生成されるため、こちらの判定の方が先
	MV1SetMaterialDrawBlendMode(GetModelHandle(), 0, DX_BLENDMODE_ADD);
	MV1SetMaterialDrawBlendParam(GetModelHandle(), 0, 255);

}
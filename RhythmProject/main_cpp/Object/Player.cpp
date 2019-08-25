#include "../../main_header/Objects/Player.h"
#include "../../main_header/Objects/InputSystem.h"
#include "../../Game.h"
#include "../../main_header/Objects/MainScreen.h"
#include "../../main_header/wrapper/Math.h"
#include <string>

Player::Player(Game* game,MainScreen* main)
	:Actor(game)
{
	SetModelHandle<std::string>("object/Player.mv1");
	SetPosition<VECTOR>(VGet(main->GetPosOffset().first + 100.0f * 3,  //レーン位置
		main->GetPosOffset().second, //y軸位置
		10.0f));
}

Player::~Player()
{

}

void Player::ActorInput(const InputState& keyState)
{
	for (int i = 0; i < mIsInput.size(); i++)
	{
		mIsInput[i] = false;
	}
	
	for (int i = 0; i < mIsLongInput.size(); i++)
	{
		mIsLongInput[i] = false;
	}

	VECTOR pos = GetPosition<VECTOR>();
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

	if (keyState.Keyboard.GetKeyState(KEY_INPUT_Z) == EPressed)
	{
		mInputTime[static_cast<int>(EColor::ERed)] = GetNowHiPerformanceCount();
		mIsInput[static_cast<int>(EColor::ERed)] = true;
	}

	if (keyState.Keyboard.GetKeyState(KEY_INPUT_Z) == EHeld ||
		keyState.Keyboard.GetKeyState(KEY_INPUT_Z) == EPressed)
	{
		mInputLongTime[static_cast<int>(EColor::ERed)] = GetNowHiPerformanceCount();
		mIsLongInput[static_cast<int>(EColor::ERed)] = true;
	}
	
	if (keyState.Keyboard.GetKeyValue(KEY_INPUT_X) == EPressed)
	{
		mInputTime[static_cast<int>(EColor::EGreen)] = GetNowHiPerformanceCount();
		mIsInput[static_cast<int>(EColor::EGreen)] = true;
	}

	if (keyState.Keyboard.GetKeyState(KEY_INPUT_X) == EHeld ||
		keyState.Keyboard.GetKeyState(KEY_INPUT_X) == EPressed)
	{
		mInputLongTime[static_cast<int>(EColor::EGreen)] = GetNowHiPerformanceCount();
		mIsLongInput[static_cast<int>(EColor::EGreen)] = true;
	}

	if (keyState.Keyboard.GetKeyValue(KEY_INPUT_C) == EPressed)
	{
		mInputTime[static_cast<int>(EColor::EBlue)] = GetNowHiPerformanceCount();
		mIsInput[static_cast<int>(EColor::EBlue)] = true;
	}

	if (keyState.Keyboard.GetKeyState(KEY_INPUT_C) == EHeld ||
		keyState.Keyboard.GetKeyState(KEY_INPUT_C) == EPressed)
	{
		mInputLongTime[static_cast<int>(EColor::EBlue)] = GetNowHiPerformanceCount();
		mIsLongInput[static_cast<int>(EColor::EBlue)] = true;
	}

	pos.x = Math::Clamp(pos.x, -260.0f, 260.0f);
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
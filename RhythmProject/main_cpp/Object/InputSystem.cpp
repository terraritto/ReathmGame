#pragma once
#include "../../main_header/Objects/InputSystem.h"
#include <stdlib.h>

bool KeyboardState::GetKeyValue(const int KeyCode) const
{
	return mCurrState[KeyCode] == 1;
}

ButtonState KeyboardState::GetKeyState(const int keyCode) const
{
	if (mPrevState[keyCode] == 0)
	{
		if (mCurrState[keyCode] == 0)
		{
			return ENone;
		}
		else
		{
			return EPressed;
		}
	}
	else
	{
		if (mCurrState[keyCode] == 0)
		{
			return EReleased;
		}
		else
		{
			return EHeld;
		}
	}
}

bool MouseState::GetButtonValue(int button) const
{
	return (mCurrButtons & button) == 1;
}

ButtonState MouseState::GetButtonState(int button) const
{
	if ((button & mPrevButtons) == 0)
	{
		if ((button & mCurrButtons) == 0)
		{
			return ENone;
		}
		else
		{
			return EPressed;
		}
	}
	else
	{
		if ((button & mCurrButtons) == 0)
		{
			return EReleased;
		}
		else
		{
			return EHeld;
		}
	}
}

bool ControllerState::GetButtonValue(int button) const
{
	return (mCurrButtons.Buttons[button]);
}

ButtonState ControllerState::GetButtonState(int button) const
{
	if (mPrevButtons.Buttons[button] == 0)
	{
		if (mCurrButtons.Buttons[button] == 0)
		{
			return ENone;
		}
		else
		{
			return EPressed;
		}
	}
	else
	{
		if (mCurrButtons.Buttons[button] == 0)
		{
			return EReleased;
		}
		else
		{
			return EHeld;
		}
	}
}

bool InputSystem::Initialize()
{
	//keyboard
	GetHitKeyStateAll(mState.Keyboard.mCurrState);
	GetHitKeyStateAll(mState.Keyboard.mPrevState);

	//Mouse
	mState.Mouse.mCurrButtons = 0;
	mState.Mouse.mPrevButtons = 0;

	// Get the connected controller, if it exists
	mState.Controller.mIsConnected = GetJoypadNum() > 0;
	// Initialize controller state
	GetJoypadDirectInputState(DX_INPUT_PAD1,&mState.Controller.mCurrButtons);
	GetJoypadDirectInputState(DX_INPUT_PAD1,&mState.Controller.mCurrButtons);
	//set DeadZone
	SetJoypadDeadZone(DX_INPUT_PAD1, 0.35);
	return true;
}

void InputSystem::Shutdown()
{

}

void InputSystem::PrepareForUpdate()
{
	//copy curent state to previous

	//keyboard
	for (int i = 0; i < 256; i++)
	{
		mState.Keyboard.mPrevState[i] = mState.Keyboard.mCurrState[i];
	}

	//mouse
	mState.Mouse.mPrevButtons = mState.Mouse.mCurrButtons;
	mState.Mouse.mIsRelative = false;
	mState.Mouse.mScrollWheel = 0;

	// Controller
	mState.Controller.mPrevButtons = mState.Controller.mCurrButtons;
}

void InputSystem::Update()
{
	//keyboard
	GetHitKeyStateAll(mState.Keyboard.mCurrState);

	//Mouse
	int x = 0, y = 0;
	//Mouse Button
	mState.Mouse.mCurrButtons = GetMouseInput();
	//Mouse Position
	GetMousePoint(&x, &y);
	mState.Mouse.mMousePos.x = static_cast<float>(x);
	mState.Mouse.mMousePos.y = static_cast<float>(y);
	//Mouse Wheel
	mState.Mouse.mScrollWheel = GetMouseWheelRotVol();

	//Controller
	mState.Controller.mIsConnected = GetJoypadNum() > 0;
	//buttons
	GetJoypadDirectInputState(DX_INPUT_PAD1,&mState.Controller.mCurrButtons);
	//trigers
	mState.Controller.mStick.x = mState.Controller.mCurrButtons.X;
	mState.Controller.mStick.y = mState.Controller.mCurrButtons.Y;
}

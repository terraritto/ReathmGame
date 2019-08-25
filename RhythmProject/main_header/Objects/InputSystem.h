#pragma once

//The different button states
enum ButtonState
{
	ENone,
	EPressed,
	EReleased,
	EHeld
};

//take (x,y) plane.
struct Position
{
	float x;
	float y;
};

//Helper for keyboard input
class KeyboardState
{
public:
	//Friend so InputSystem can easily update it
	friend class InputSystem;
	//Get just the boolean true/false value of key
	bool GetKeyValue(const int keyCode) const;
	//Get a state based on current and previous frame
	ButtonState GetKeyState(const int keyCode) const;
private:
	char mCurrState[256];
	char mPrevState[256];
};

//Helper for mouse input
class MouseState
{
public:
	friend class InputSystem;

	//for mouse position
	const Position& GetPosition() const { return mMousePos; }
	const int& GetScrollWheel() const { return mScrollWheel; }

	//for buttons
	bool GetButtonValue(int button) const;
	ButtonState GetButtonState(int button) const;
private:
	//Store current mouse position
	Position mMousePos;
	//Motion of scroll wheel
	int mScrollWheel;
	//Store button data
	int mCurrButtons;
	int mPrevButtons;
	//Are we in relative mouse mode
	bool mIsRelative;
};

//Helper for controller input

//Wrapper that contains current state of input
struct InputState
{
	KeyboardState Keyboard;
	MouseState Mouse;
};

class InputSystem
{
public:
	bool Initialize();
	void Shutdown();
	
	//Prev State get Curr State.
	void PrepareForUpdate();
	//Update
	void Update();

	const InputState& GetState() const { return mState; }
private:
	InputState mState;
};
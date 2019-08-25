#pragma once
#include <vector>
#include <string>
#include <utility>
#include <functional>
#include "DxLib.h"

struct TextInf {
	std::string textInformation;
	int x;
	int y; //position
	unsigned int color; //font color
	int size;//font size

	TextInf()
		: x(0)
		, y(0)
		, color(GetColor(255, 255, 255))
		, size(30)
	{
	}

	TextInf(std::string text, int posX, int posY, unsigned int textColor, int textSize)
		: textInformation(text)
		, x(posX)
		, y(posY)
		, color(textColor)
		, size(textSize)
	{
	}
};

struct Pos
{
	int x;
	int y;
	Pos() :x(0), y(0) {}
	Pos(int posX, int posY) :x(posX), y(posY) {}
};

class Button
{
public:
	Button(const TextInf name,
		std::function<void()> onClick, const Pos& pos,
		const Pos& dims);
	~Button();

	//Set the name of the button
	//void SetName(const std::string& name);

	// Getters/Setters
	const Pos& GetPosition() const { return mPosition; }
	void SetHighlighted(bool sel) { mHighlighted = sel; }
	bool GetHighlighted() const { return mHighlighted; }
	const TextInf& GetName() const { return mName; }
	//returns true if the point is within the button's bounds
	bool ContainsPoint(const Pos& pt) const;
	//Called when button is clicked
	void OnClick();
private:
	std::function<void()> mOnClick;
	TextInf mName;
	Pos mPosition;
	Pos mDimentions;
	bool mHighlighted;
};


class UIScreen
{
public:
	UIScreen(class Game* game);
	virtual ~UIScreen();
	//UIScreen subclasses can override these
	virtual void Update(float deltaTime);
	virtual void Draw();
	virtual void ProcessInput(const struct InputState& state);

	//Tracks if the UI is active or closing
	enum UIState
	{
		EActive,
		EClosing
	};
	//Set state to closing
	void Close();
	//Get state of UI screen
	UIState GetState() const { return mState; }
	//Change the title text
	void SetTitleTextAndPos(TextInf& textTitle);
	void AddButton(const TextInf& name, std::function<void()> onClick);
protected:
	//Sets the mouse mode to relative or not
	class Game* mGame;

	class Font* mFont;

	//texture
	int mButtonOn;
	int mButtonOff;
	int mBackground;
	//text
	std::vector<TextInf> mTitleText;
	//position
	Pos mNextButtonPos;
	Pos mBGPos;
	//State
	UIState mState;
	std::vector<Button*> mButtons;
};
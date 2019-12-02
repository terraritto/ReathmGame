#pragma once

enum class EColor
{
	ERed,
	EGreen,
	EBlue
};

enum class EKeyboardDirection
{
	ELeft,
	ERight
};

constexpr int WINDOW_WIDTH = 768;
constexpr int WINDOW_HEIGHT = 1024;

constexpr LONGLONG JUDGE_TIME = 32000;
constexpr LONGLONG JUDGE_OFFSET = 128000;
constexpr float ONE_TIME = 6000000;

constexpr float PLAYER_RAD_OFFSET = 40.0f;
constexpr float PLAYER_SPEED = 10.0f;

constexpr int BOX_UP_DOWN = 5;
constexpr int BOX_LEFT_SPACE = 20;
constexpr int BOX_UP_SPACE = 75;

constexpr int ALLOW_DOWN = 50;

//setting
constexpr int BACKGROUND_TEXT_X_SPEED_OFFSET = 65;
constexpr int BACKGROUND_TEXT_X_JUDGE_OFFSET = 3;
constexpr int BACKGROUND_TEXT_X_VOLUME_OFFSET = 105;
constexpr int BACKGROUND_TEXT_Y_OFFSET = 17;
constexpr int VALUE_TEXT_X_OFFSET = 120;
constexpr int VALUE_TEXT_Y_OFFSET = 20;
constexpr int EXPLAIN_TEXT_X_OFFSET = 10;
constexpr int EXPLAIN_TEXT_Y_OFFSET = 30;

//music box
constexpr int LV_TEXT_X_OFFSET = 30;
constexpr int LV_TEXT_Y_OFFSET = 30;
constexpr int SOUND_TEXT_X_OFFSET = 10;
constexpr int SOUND_TEXT_Y_OFFSET = 25;

//tutorial text
constexpr int TUTORIAL_TEXT_OFFSET = 20;
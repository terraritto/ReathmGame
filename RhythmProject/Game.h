#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include "DxLib.h"
#include "main_header\Objects\MainScreen.h"

class Game
{
public:
	enum class GameState
	{
		EGamePlay,
		EPaused,
		EQuit
	};

	enum class RhythmGame
	{
		EStartScene,
		EGameScene
	};

	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();
	
	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	//Manage UI Stack
	const std::vector<class UIScreen*>& GetUIStack() { return mUIStack; }
	class Font* GetFont(const std::string& fileName);
	void PushUI(class UIScreen* screen);
	//Game State
	GameState GetState() const { return mGameState; }
	void SetState(GameState state) { mGameState = state; }
	//game-specific
	MainScreen* GetMainScreen() { return mMainScreen; }
	
	//deleter to use any sceen.
	void DeleteStartScreen(const RhythmGame& state);
	void DeleteMainScreen(const RhythmGame& state);
private:
	//helper function for game loop
	//helper function for game loop
	void ProcessInput();
	//void HandleKeyPress(int key);
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnLoadData();

	//Game
	//time
	int mTickCount;
	//GameState
	GameState mGameState; //general
	RhythmGame mRhythmGameState; //rhythm game specific

	//Actor
	std::vector<class Actor*> mActors; //Active Actors
	std::vector<class UIScreen*> mUIStack; // UI Stack of game Unique
	std::unordered_map<std::string, class Font*> mFonts; //Font Information
	std::vector<class Actor*> mPendingActors; //Pending Actors
	bool mUpdatingActors; //Updating?

	//deleter
	void DeleteManager();
	RhythmGame mNextState;

	//function
	class InputSystem* mInputSystem;

	//Game-specific code
	 class MainScreen* mMainScreen;
	 class StartScene* mStartSceen;
};
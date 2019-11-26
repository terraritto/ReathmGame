#pragma once
#include "..//wrapper//CardReader.h"

class StartScene
{
public:
	StartScene(class Game* game);
	~StartScene();

	void ProcessInput(const struct InputState& keyState);
	void Update(); 
	void DrawStr();

	enum class FlashStr
	{
		Up,
		Down
	};
private:
	class Actor* mStage; //‰æ–Ê
	class Game* mGame;
	class FPSActor* mFPSActor;
	float mStageRotation;
	float mAlpha;
	FlashStr mStrState;

	//LazyPCSCFelicaLite::PCSCFelicaLite mCardReader;
};
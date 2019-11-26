#pragma once
#include "UIScreen.h"

class SelectObj : public UIScreen 
{
public:
	SelectObj(class Game* game);
	~SelectObj();

	void SetPos(Pos p) { mBGPos = p; }
	void SetImage(std::string image) { 
		mBackground = LoadGraph(image.c_str());
		GetGraphSize(mBackground, &mObjSize.x, &mObjSize.y);
	}
	Pos mObjSize;
};
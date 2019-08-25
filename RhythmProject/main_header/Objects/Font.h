#pragma once
#include <string>
#include <unordered_map>
#include "DxLib.h"

class Font
{
public:
	Font(class Game* game);
	~Font();

	//Load/Unload Font
	bool Load(const std::string& fontName);
	void Unload();
	//get handle about font
	int GetFont(int size);
private:
	//Map of point sizes to font data
	//<Font Size, Font Handle>
	std::unordered_map<int, int> mFontData;
	class Game* mGame;
};
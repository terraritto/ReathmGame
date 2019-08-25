#include "../../main_header/Objects/Font.h"
#include "../../Game.h"

Font::Font(Game* game)
	:mGame(game)
{

}

Font::~Font()
{

}

bool Font::Load(const std::string& fontName)
{
	//We support these font sizes
	std::vector<int> fontSizes = {
		8,9,
		10,11,12,14,16,18,
		20,22,24,26,28,
		30,32,34,36,38,
		40,42,44,46,48,
		52,56,
		60,64,68,
		72
	};

	for (const auto& size : fontSizes)
	{
		int font = CreateFontToHandle(fontName.c_str(), size, 3); //create font
		if (font == -1)
		{
			return false;
		}
		mFontData.emplace(size, font);
	}

	return true;
}

void Font::Unload()
{
	for (const auto& font : mFontData)
	{
		DeleteFontToHandle(font.second); //delete font
	}
}

int Font::GetFont(int size)
{
	auto iter = mFontData.find(size);
	if (iter != mFontData.end())
	{
		return iter->second;
	}
	else
	{
		return -1;
	}
}
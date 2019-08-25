#pragma once

struct MusicInfo
{
	int index;
	int bpmIndex;
	int bpmPage;
	int lpbIndex;
	int lane;
	char color;
	int longNotesIndex;

	MusicInfo()
		: index(0)
		, bpmIndex(0)
		, bpmPage(0)
		, lpbIndex(0)
		, lane(0)
		, color('n')
		, longNotesIndex(-1)
	{
		
	}
};
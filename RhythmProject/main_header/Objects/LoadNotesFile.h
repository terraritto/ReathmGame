#pragma once
#include <string>
#include <vector>
#include <array>
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>
#include "MainScreen.h"
#include "../wrapper/MusicInf.h"
#include "notes.h"
#include "LongNotes.h"
#include "WallNotes.h"
#include "TraceNotes.h"

std::vector<std::string> Split(std::string& input, char delimiter)
{
	std::istringstream stream(input);
	std::string field;
	std::vector<std::string> result;
	while (std::getline(stream, field, delimiter))
	{
		result.push_back(field);
	}
	return result;
}

void FileRead(std::string fileName, MainScreen* screen, Game* game)
{
	std::ifstream ifs("Text\\" + fileName);
	std::string line;
	int count = 0;
	
	//longNotesån
	std::vector<MusicInfo> longNortsObject;
	int maxIndex = INT_MIN;

	float shortTimeBpm;//1 bpm ÇÃéûÇÃéûä‘
	float shortTimeLpb; //1lpbÇÃéûä‘
	while (std::getline(ifs, line))
	{
		auto musicData = Split(line, ',');
		if (count == 0)
		{
			std::string music;
			music = musicData[0];
			music = "Music\\" + music;
			screen->mBpm = std::stoi(musicData[1]);
			screen->mLpb = std::stoi(musicData[2]);
		
			screen->SetMusicFile(music);
			shortTimeBpm = 60000000.0f / screen->mBpm;
			shortTimeLpb = shortTimeBpm / screen->mLpb;
		}
		else
		{
			MusicInfo music;
			switch (std::stoi(musicData[0]))
			{
			case 1:
			{
				music.bpmIndex = std::stoi(musicData[1]);
				music.bpmPage = std::stoi(musicData[2]);
				music.lpbIndex = std::stoi(musicData[3]);
				music.lane = std::stoi(musicData[4]) - 1;
				music.color = (musicData[5])[0];

				Notes* note = new Notes(game); //ê∂ê¨

				//èÓïÒÇÃset
				float time =
					shortTimeBpm * ((music.bpmPage * 4) + music.bpmIndex) +  //bpmï™ÇÃéûä‘
					shortTimeLpb * music.lpbIndex; //lpbï™ÇÃéûä‘
				note->SetLane(music.lane);
				note->SetColor(music.color);
				note->SetArrivalTime(time);
				break;
			}
			default:
				music.index = std::stoi(musicData[0]);
				music.bpmIndex = std::stoi(musicData[1]);
				music.bpmPage = std::stoi(musicData[2]);
				music.lpbIndex = std::stoi(musicData[3]);
				music.lane = std::stoi(musicData[4]) - 1;
				music.color = (musicData[5])[0];
				music.longNotesIndex = std::stoi(musicData[6]);
				longNortsObject.emplace_back(music);
				
				maxIndex = maxIndex < music.longNotesIndex ? music.longNotesIndex : maxIndex;
				break;
			}
		}
		count++;
	}

	for (int i = 1; i <= maxIndex; i++) //longNortsånÇÃê∂ê¨
	{
		std::vector<MusicInfo> selectObject;
		for (auto object : longNortsObject)//indexî‘ñ⁄ÇÃobjectÇÃëIï 
		{
			if (object.longNotesIndex == i)
			{
				selectObject.emplace_back(object);
			}
		}

		switch (selectObject[0].index)
		{
		case 2:
		{
			LongNotes* note = new LongNotes(game); //ê∂ê¨
			std::array<LONGLONG, 2> time;
			note->SetLane(selectObject[0].lane);
			note->SetColor(selectObject[0].color);

			time[0] = shortTimeBpm * ((selectObject[0].bpmPage * 4) + selectObject[0].bpmIndex) +  //bpmï™ÇÃéûä‘
				shortTimeLpb * selectObject[0].lpbIndex; //lpbï™ÇÃéûä‘
			time[1] = shortTimeBpm * ((selectObject[1].bpmPage * 4) + selectObject[1].bpmIndex) +  //bpmï™ÇÃéûä‘
				shortTimeLpb * selectObject[1].lpbIndex; //lpbï™ÇÃéûä‘
			note->SetArrivalTime(time);
			note->SetScale();
			break;
		}
		case 3:
		{
			WallNotes* note = new WallNotes(game); //ê∂ê¨
			std::array<LONGLONG, 2> time;
			note->SetLane(selectObject[0].lane);
			
			time[0] = shortTimeBpm * ((selectObject[0].bpmPage * 4) + selectObject[0].bpmIndex) +  //bpmï™ÇÃéûä‘
				shortTimeLpb * selectObject[0].lpbIndex; //lpbï™ÇÃéûä‘
			time[1] = shortTimeBpm * ((selectObject[1].bpmPage * 4) + selectObject[1].bpmIndex) +  //bpmï™ÇÃéûä‘
				shortTimeLpb * selectObject[1].lpbIndex; //lpbï™ÇÃéûä‘
		
			note->SetArrivalTime(time);
			note->SetHeight(0.0f);
			note->SetScale();
			break;
		}
		case 4:
		{
			WallNotes* note = new WallNotes(game); //ê∂ê¨
			std::array<LONGLONG, 2> time;
			note->SetLane(selectObject[0].lane);

			time[0] = shortTimeBpm * ((selectObject[0].bpmPage * 4) + selectObject[0].bpmIndex) +  //bpmï™ÇÃéûä‘
				shortTimeLpb * selectObject[0].lpbIndex; //lpbï™ÇÃéûä‘
			time[1] = shortTimeBpm * ((selectObject[1].bpmPage * 4) + selectObject[1].bpmIndex) +  //bpmï™ÇÃéûä‘
				shortTimeLpb * selectObject[1].lpbIndex; //lpbï™ÇÃéûä‘
		
			note->SetArrivalTime(time);
			note->SetHeight(240.0f);
			note->SetScale();
			break;
		}
		case 5:
		{	
			for (int j = 0; j < selectObject.size() - 1; j++)
			{
				TraceNotes* note = new TraceNotes(game);
				std::array<LONGLONG, 2> time;
				std::array<int, 2> lane;

				lane[0] = selectObject[j].lane;
				lane[1] = selectObject[j + 1].lane;
				note->SetLane(lane);

				time[0] = shortTimeBpm * ((selectObject[j].bpmPage * 4) + selectObject[j].bpmIndex) +  //bpmï™ÇÃéûä‘
					shortTimeLpb * selectObject[j].lpbIndex; //lpbï™ÇÃéûä‘
				time[1] = shortTimeBpm * ((selectObject[j + 1].bpmPage * 4) + selectObject[j+1].bpmIndex) +  //bpmï™ÇÃéûä‘
					shortTimeLpb * selectObject[j+1].lpbIndex; //lpbï™ÇÃéûä‘
				note->SetArrivalTime(time);

				note->SetScale();
			}
			break;
		}
		default:
			break;
		}
	}

	ifs.close();
}

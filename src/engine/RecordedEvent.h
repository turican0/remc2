#pragma once
#include <cstdint>
#include <map>
struct RecordedEventHeader
{
	uint16_t Level = 0;
	uint16_t PlayerCount = 0;
};

struct RecordedEventTurn
{
	uint32_t Turn = 0;
	uint32_t SizeBytes = 0;
	uint8_t* Bytes = nullptr;

	~RecordedEventTurn()
	{
		delete[] Bytes;
	}
};

struct RecordedEventPlayer
{
	uint16_t PlayerIdx = 0;
	uint32_t TurnCount = 0;
	std::map<uint32_t, RecordedEventTurn*>* Turns = nullptr;
	int16_t* SpellsEnabled = nullptr;
	uint8_t* SpellIndexes = nullptr;
	uint8_t* SpellLevels = nullptr;
	int32_t* SpellsExperience = nullptr;

	~RecordedEventPlayer()
	{
		delete[] SpellsEnabled;
		delete[] SpellIndexes;
		delete[] SpellLevels;
		delete[] SpellsExperience;
	}
};

struct RecordedEvent
{
	RecordedEventHeader* Header = nullptr;
	std::map<uint16_t, RecordedEventPlayer*>* Players = nullptr;
};

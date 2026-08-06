#pragma once
#include <cstdint>
struct InputEventHeader
{
	uint16_t Level = 0;
	uint16_t PlayerCount = 0;
};

struct InputTurn
{
	uint32_t Turn = 0;
	uint32_t SizeBytes = 0;
	uint8_t* Bytes = nullptr;
};

struct InputPlayer
{
	uint16_t PlayerIdx = 0;
	uint32_t TurnCount = 0;
	std::map<uint32_t, InputTurn*>* Turns = nullptr;
};

struct RecordedEvent
{
	InputEventHeader* Header = nullptr;
	std::map<uint16_t, InputPlayer*>* Players = nullptr;
};

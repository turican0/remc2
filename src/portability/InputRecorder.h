#pragma once
#include <cstdint>
#include <cstring>
#include <map>
#include <fstream>
#include <vector>
#include "engine/RecordedEvent.h"
#include "engine/GameState.h"

class InputRecorder
{
private:
	const std::string m_FileSignature = "MC2-HD-Recording";
	std::string m_FilePath;
	std::map<uint16_t, RecordedEvent*>* m_InputEvents;

	bool SaveRecordingToFile(const char* outputFileName);
	bool LoadRecordingFile(const char* inputFileName);

public:
	bool m_IsRecording = false;
	bool m_IsPlaying = false;

	InputRecorder(const char* filePath);
	~InputRecorder();

	void StartRecording();
	bool SaveRecording();
	bool StopRecording();
	void ClearInputEvents();
	
	bool StartPlayback();
	void StopPlayback();

	RecordedEventPlayer* GetCurrentPlayer(int level, int playerIdx);
	RecordedEventTurn* GetCurrentPlayerActions(int level, int playerIdx, int turn);

	void RecordPlayerActions(uint16_t level, uint16_t playerIdx, uint32_t turn, uint64_t sizeBytes, uint8_t* buffer);
	void RecordPlayerSpells(int level, int playerIdx, int16_t* spellsEnabled, uint8_t* spellIndexes, uint8_t* spellLevels, int32_t* spellsExperience);
};


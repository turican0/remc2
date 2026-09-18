#pragma once
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <map>
#include <fstream>
#include <vector>
#include "../engine/RecordedEvent.h"
#include "../engine/GameState.h"

class InputRecorder
{
private:
	const std::string m_FileSignature = "MC2-HD-RecordV03";
	std::map<uint16_t, size_t> m_LevelStarts;//how often a level has started in this playback
	std::string m_FilePath;
	std::map<uint16_t, RecordedEvent*>* m_InputEvents;

	bool SaveRecordingToFile(const char* outputFileName);
	bool LoadRecordingFile(const char* inputFileName);
	RecordedEvent* GetLevel(uint16_t level);

public:
	bool m_IsRecording = false;
	bool m_IsPlaying = false;
	std::string m_ResavePath;//playback rewritten with level saves

	InputRecorder(const char* filePath);
	~InputRecorder();

	void StartRecording();
	bool SaveRecording();
	bool StopRecording();
	void ClearInputEvents();
	
	bool StartPlayback();
	void StopPlayback();

	uint32_t GetLevelRand(int level);
	RecordedEventPlayer* GetCurrentPlayer(int level, int playerIdx);
	RecordedEventTurn* GetCurrentPlayerActions(int level, int playerIdx, int turn);

	void RecordLevelSave(uint16_t level, std::vector<uint8_t> save);
	const std::vector<uint8_t>* GetLevelSave(int level);//of the current start of the level
	void LevelStarted(uint16_t level);
	void RecordPlayerActions(uint16_t level, uint16_t playerIdx, uint32_t turn, uint32_t rand, uint64_t sizeBytes, uint8_t* buffer);
	void RecordPlayerSpells(int level, int playerIdx, int16_t* spellsEnabled, uint8_t* spellIndexes, uint8_t* spellLevels, int32_t* spellsExperience);
};


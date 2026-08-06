#include "InputRecorder.h"
#include "engine/EventDispatcher.h"
#include "engine/GameState.h"
using namespace std;

InputRecorder::InputRecorder(const char* filePath)
{
	m_FilePath = filePath;
	m_InputEvents = new std::map<uint16_t, RecordedEvent*>();
}

InputRecorder::~InputRecorder()
{
	ClearInputEvents();
	delete m_InputEvents;
}

void InputRecorder::StartRecording()
{
	m_IsRecording = true;
}

void InputRecorder::ClearInputEvents()
{
	map<uint16_t, RecordedEvent*>::iterator levelIt;
	map<uint16_t, RecordedEventPlayer*>::iterator playIt;
	map<uint32_t, RecordedEventTurn*>::iterator turnIt;

	for (levelIt = m_InputEvents->begin(); levelIt != m_InputEvents->end(); levelIt++)
	{

		for (playIt = levelIt->second->Players->begin(); playIt != levelIt->second->Players->end(); playIt++)
		{
			for (turnIt = playIt->second->Turns->begin(); turnIt != playIt->second->Turns->end(); turnIt++)
			{
				delete turnIt->second;
			}
			playIt->second->Turns->clear();
			delete playIt->second;
		}
		levelIt->second->Players->clear();
		delete levelIt->second;
	}
	m_InputEvents->clear();
}

bool InputRecorder::SaveRecording()
{
	return SaveRecordingToFile(m_FilePath.c_str());
}

bool InputRecorder::StopRecording()
{
	m_IsRecording = false;
	if (SaveRecording())
	{
		ClearInputEvents();
		return true;
	}
	return false;
}

bool InputRecorder::StartPlayback()
{
	if (LoadRecordingFile(m_FilePath.c_str()))
		m_IsPlaying = true;

	return m_IsPlaying;
}

void InputRecorder::StopPlayback()
{
	m_IsPlaying = false;
}

RecordedEventPlayer* InputRecorder::GetCurrentPlayer(int level, int playerIdx)
{
	if (!m_IsPlaying || m_InputEvents->count(level) == 0 || m_InputEvents->at(level)->Players->count(playerIdx) == 0 || m_InputEvents->at(level)->Players->count(playerIdx) == 0)
		return nullptr;

	return m_InputEvents->at(level)->Players->at(playerIdx);
}

RecordedEventTurn* InputRecorder::GetCurrentPlayerActions(int level, int playerIdx, int turn)
{
	if (!m_IsPlaying || m_InputEvents->count(level) == 0 || m_InputEvents->at(level)->Players->count(playerIdx) == 0 || m_InputEvents->at(level)->Players->at(playerIdx)->Turns->count(turn) == 0)
		return nullptr;

	return m_InputEvents->at(level)->Players->at(playerIdx)->Turns->at(turn);
}

void InputRecorder::RecordPlayerSpells(int level, int playerIdx, int16_t* spellsEnabled, uint8_t* spellIndexes, uint8_t* spellLevels, int32_t* spellsExperience)
{
	if (!m_IsRecording)
		return;

	if (m_InputEvents->count(level) == 0)
	{
		m_InputEvents->insert(std::pair<uint16_t, RecordedEvent*>(level, new RecordedEvent()));
		m_InputEvents->at(level)->Header = new RecordedEventHeader();
		m_InputEvents->at(level)->Header->Level = level;
		m_InputEvents->at(level)->Players = new std::map<uint16_t, RecordedEventPlayer*>();
	}
	if (m_InputEvents->at(level)->Players->count(playerIdx) == 0)
	{
		m_InputEvents->at(level)->Players->insert(std::pair<uint16_t, RecordedEventPlayer*>(playerIdx, new RecordedEventPlayer()));
		m_InputEvents->at(level)->Players->at(playerIdx) = new RecordedEventPlayer();
		m_InputEvents->at(level)->Players->at(playerIdx)->PlayerIdx = playerIdx;
		m_InputEvents->at(level)->Players->at(playerIdx)->Turns = new std::map<uint32_t, RecordedEventTurn*>();
		m_InputEvents->at(level)->Players->at(playerIdx)->SpellsEnabled = new int16_t[26];
		m_InputEvents->at(level)->Players->at(playerIdx)->SpellIndexes = new uint8_t[26];
		m_InputEvents->at(level)->Players->at(playerIdx)->SpellLevels = new uint8_t[26];
		m_InputEvents->at(level)->Players->at(playerIdx)->SpellsExperience = new int32_t[26];
	}

	for (int i = 0; i < 26; i++)
	{
		m_InputEvents->at(level)->Players->at(playerIdx)->SpellsEnabled[i] = spellsEnabled[i];
		m_InputEvents->at(level)->Players->at(playerIdx)->SpellIndexes[i] = spellIndexes[i];
		m_InputEvents->at(level)->Players->at(playerIdx)->SpellLevels[i] = spellLevels[i];
		m_InputEvents->at(level)->Players->at(playerIdx)->SpellsExperience[i] = spellsExperience[i];
	}
}

void InputRecorder::RecordPlayerActions(uint16_t level, uint16_t playerIdx, uint32_t turn, uint64_t sizeBytes, uint8_t* buffer)
{
	if (!m_IsRecording)
		return;

	if (m_InputEvents->count(level) == 0) 
	{
		m_InputEvents->insert(std::pair<uint16_t, RecordedEvent*>(level, new RecordedEvent()));
		m_InputEvents->at(level)->Header = new RecordedEventHeader();
		m_InputEvents->at(level)->Header->Level = level;
		m_InputEvents->at(level)->Players = new std::map<uint16_t, RecordedEventPlayer*>();
	}
	if (m_InputEvents->at(level)->Players->count(playerIdx) == 0)
	{
		m_InputEvents->at(level)->Players->insert(std::pair<uint16_t, RecordedEventPlayer*>(playerIdx, new RecordedEventPlayer()));
		m_InputEvents->at(level)->Players->at(playerIdx) = new RecordedEventPlayer();
		m_InputEvents->at(level)->Players->at(playerIdx)->PlayerIdx = playerIdx;
		m_InputEvents->at(level)->Players->at(playerIdx)->Turns = new std::map<uint32_t, RecordedEventTurn*>();
	}
	if (m_InputEvents->at(level)->Players->at(playerIdx)->Turns->count(turn) == 0)
	{
		m_InputEvents->at(level)->Players->at(playerIdx)->Turns->insert({ turn, { new RecordedEventTurn() } });
	}
	m_InputEvents->at(level)->Players->at(playerIdx)->Turns->at(turn)->Turn = turn;

	m_InputEvents->at(level)->Players->at(playerIdx)->Turns->at(turn)->SizeBytes = sizeBytes;
	m_InputEvents->at(level)->Players->at(playerIdx)->Turns->at(turn)->Bytes = new uint8_t[sizeBytes];
	memcpy(m_InputEvents->at(level)->Players->at(playerIdx)->Turns->at(turn)->Bytes, buffer, sizeBytes);
}

bool InputRecorder::SaveRecordingToFile(const char* outputFileName)
{
	try
	{
		if (m_InputEvents == nullptr || m_InputEvents->empty())
			return false;

		FILE* eventsFile = fopen(outputFileName, "wb");
		if (!eventsFile)
			return false;

		std::string fileSignature = "MC2-HD-Recording";

		fwrite((uint8_t*)fileSignature.c_str(), fileSignature.length() * sizeof(char), 1, eventsFile);

		std::vector<RecordedEventTurn*>* playerTurns = new std::vector<RecordedEventTurn*>();

		map<uint16_t, RecordedEvent*>::iterator levelIt;
		map<uint16_t, RecordedEventPlayer*>::iterator playIt;
		map<uint32_t, RecordedEventTurn*>::iterator turnIt;

		for (levelIt = m_InputEvents->begin(); levelIt != m_InputEvents->end(); levelIt++)
		{
			int level = levelIt->first;
			auto* inputEventHeader = new RecordedEventHeader();
			inputEventHeader->Level = level;
			inputEventHeader->PlayerCount = levelIt->second->Players->size();
			
			fwrite((uint8_t*)inputEventHeader, sizeof(RecordedEventHeader), 1, eventsFile);

			for (playIt = levelIt->second->Players->begin(); playIt != levelIt->second->Players->end(); playIt++)
			{
				uint16_t playerIndex = playIt->first;
				for (turnIt = playIt->second->Turns->begin(); turnIt != playIt->second->Turns->end(); turnIt++)
				{
					playerTurns->push_back(turnIt->second);
				}
				uint32_t turnCount = playerTurns->size();

				fwrite(&playerIndex, sizeof(uint16_t), 1, eventsFile);
				fwrite(&turnCount, sizeof(uint32_t), 1, eventsFile);
				fwrite(playIt->second->SpellsEnabled, sizeof(int16_t), 26, eventsFile);
				fwrite(playIt->second->SpellIndexes, sizeof(uint8_t), 26, eventsFile);
				fwrite(playIt->second->SpellLevels, sizeof(uint8_t), 26, eventsFile);
				fwrite(playIt->second->SpellsExperience, sizeof(int32_t), 26, eventsFile);

				for (int i = 0; i < playerTurns->size(); i++)
				{
					auto turn = playerTurns->at(i);
					fwrite(turn, 8, 1, eventsFile);
					fwrite(turn->Bytes, playerTurns->at(i)->SizeBytes, 1, eventsFile);
				}
				
				playerTurns->clear();
			}
			delete inputEventHeader;
		}
		delete playerTurns;
		return fclose(eventsFile) == 0;
	}
	catch (exception ex)
	{
		return false;
	}
}

bool InputRecorder::LoadRecordingFile(const char* inputFileName)
{
	try
	{
		FILE* eventsFile = fopen(inputFileName, "rb");
		if (eventsFile == nullptr)
			return false;

		uint16_t level = 0;
		uint16_t playerCount = 0;

		char* fileSignature = new char[17];

		fread(fileSignature, sizeof(char), 16, eventsFile);
		fileSignature[16] = NULL;

		if (strcmp(fileSignature, m_FileSignature.c_str()) != 0)
			return false;

		while (fread(&level, sizeof(RecordedEventHeader::Level), 1, eventsFile))
		{
			fread(&playerCount, sizeof(RecordedEventHeader::PlayerCount), 1, eventsFile);

			if (m_InputEvents->count(level) == 0)
			{
				m_InputEvents->insert(std::pair<uint16_t, RecordedEvent*>(level, new RecordedEvent()));
				m_InputEvents->at(level)->Header = new RecordedEventHeader();
				m_InputEvents->at(level)->Header->Level = level;
				m_InputEvents->at(level)->Header->PlayerCount = playerCount;
				m_InputEvents->at(level)->Players = new std::map<uint16_t, RecordedEventPlayer*>();
			}


			uint16_t playerIdx = 0;
			uint32_t turnCount = 0;
			int16_t spellsEnabled[26];
			int8_t spellIndexes[26];
			int8_t spellLevels[26];
			int32_t spellsExperience[26];

			while (playerCount--)
			{
				fread(&playerIdx, sizeof(RecordedEventPlayer::PlayerIdx), 1, eventsFile);
				fread(&turnCount, sizeof(RecordedEventPlayer::TurnCount), 1, eventsFile);
				fread(&spellsEnabled, sizeof(int16_t), 26, eventsFile);
				fread(&spellIndexes, sizeof(uint8_t), 26, eventsFile);
				fread(&spellLevels, sizeof(uint8_t), 26, eventsFile);
				fread(&spellsExperience, sizeof(int32_t), 26, eventsFile);

				if (m_InputEvents->at(level)->Players->count(playerIdx) == 0)
				{
					m_InputEvents->at(level)->Players->insert(std::pair<uint16_t, RecordedEventPlayer*>(playerIdx, new RecordedEventPlayer()));
					m_InputEvents->at(level)->Players->at(playerIdx)->PlayerIdx = playerIdx;
					m_InputEvents->at(level)->Players->at(playerIdx)->TurnCount = turnCount;
					m_InputEvents->at(level)->Players->at(playerIdx)->Turns = new std::map<uint32_t, RecordedEventTurn*>();
					m_InputEvents->at(level)->Players->at(playerIdx)->SpellsEnabled = new int16_t[26];
					m_InputEvents->at(level)->Players->at(playerIdx)->SpellIndexes = new uint8_t[26];
					m_InputEvents->at(level)->Players->at(playerIdx)->SpellLevels = new uint8_t[26];
					m_InputEvents->at(level)->Players->at(playerIdx)->SpellsExperience = new int32_t[26];

					for (int i = 0; i < 26; i++)
					{
						m_InputEvents->at(level)->Players->at(playerIdx)->SpellsEnabled[i] = spellsEnabled[i];
						m_InputEvents->at(level)->Players->at(playerIdx)->SpellIndexes[i] = spellIndexes[i];
						m_InputEvents->at(level)->Players->at(playerIdx)->SpellLevels[i] = spellLevels[i];
						m_InputEvents->at(level)->Players->at(playerIdx)->SpellsExperience[i] = spellsExperience[i];
					}
				}

				for (int i = 0; i < turnCount; i++)
				{
					RecordedEventTurn* turn = new RecordedEventTurn();
					fread(turn, 8, 1, eventsFile);
					turn->Bytes = new uint8_t[turn->SizeBytes];
					fread(turn->Bytes, turn->SizeBytes, 1, eventsFile);
					m_InputEvents->at(level)->Players->at(playerIdx)->Turns->insert({ turn->Turn, turn });
				}
			}
		}
		return fclose(eventsFile) == 0;
	}
	catch (exception ex)
	{
		return false;
	}
}

#include "Mc1Save.h"

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <string>
#include <vector>

#include "Mc1Entity.h"
#include "Mc1Files.h"
#include "Mc1Game.h"
#include "Mc1Level.h"
#include "Mc1Shared.h"
#include "../../remc2/utilities/DataFileIO.h"

// the state in DOS: AE400 / AE3F0 holds 1F690 in both games (DOSBox), the data = IDA + 1BE000h
constexpr uint32_t dosStateSize = 0x38D09;
constexpr uint32_t dosState = 0x1F690;
constexpr uint32_t dosEntities = dosState + 0x7463;
constexpr uint32_t dosTypes = 0x98F38 + 0x1BE000;
constexpr uint32_t dosWizardType = 0x99018 + 0x1BE000; // dword_99018: the row 7 of creatureTypes_98F38
constexpr uint32_t dosPlayerDataMc1 = 0xB7330 + 0x1BE000;
constexpr uint32_t dosPlayerDataHw = 0xB7320 + 0x1BE000;
// the offsets in the state of DOS: up to the lists of the free entities and from the options to the entities the
// layout of the port, the lists and the entities have pointers
constexpr uint32_t dosFreeEntities = 0x251;
constexpr uint32_t dosReuseTop = 0x11F1;
constexpr uint32_t dosReuseEntities = 0x11F5;
constexpr uint32_t dosOptions = 0x2195;
constexpr uint32_t dosEntityArray = 0x7463;
constexpr uint32_t dosLevel = 0x2F503; // the level and the statistics, to the end

// port: a game with entities over the 1000 of the original (mc1EntityLimit) goes to gax%05d.dat, which the original
// does not read: this header, the number of the entities, the state of DOS with the first 1000, the others after it
static const uint8_t extendedMagic[8] = { 'M', 'C', '1', 'G', 'A', 'X', '0', '1' };

static uint32_t DosPlayerData()
{
	return mc1Variant == MC1_VARIANT_HW ? dosPlayerDataHw : dosPlayerDataMc1;
}

static uint32_t ReadU32(const uint8_t* data)
{
	uint32_t value;
	memcpy(&value, data, sizeof(value));
	return value;
}

static void WriteU32(uint8_t* data, uint32_t value)
{
	memcpy(data, &value, sizeof(value));
}

// the DOS address of an entity of the port (0 none)
static uint32_t DosEntity(const Type_Mc1Entity* entity)
{
	if (!entity || entity == EntityAtAddressZero())
		return 0;
	return dosEntities + (uint32_t)(entity - mc1Entities) * 0xA4;
}

// the DOS address of a pointer into the types, the table B7330 or the state
static uint32_t DosPointer(const uint8_t* pointer)
{
	const uint8_t* state = (const uint8_t*)mc1State_AE400_AE3F0;
	const uint8_t* types = &creatureTypes_98F38_98F38[0][0];
	const uint8_t* options = &mc1State_AE400_AE3F0->reflections_0x2195_8597;
	const uint8_t* entities = (const uint8_t*)mc1State_AE400_AE3F0->entities_0x7463_29795;
	if (!pointer || pointer == DosAddressZero())
		return 0;
	if (pointer >= types && pointer < types + sizeof(creatureTypes_98F38_98F38))
		return dosTypes + (uint32_t)(pointer - types);
	if (pointer >= playerData_B7330_B7320 && pointer < playerData_B7330_B7320 + sizeof(playerData_B7330_B7320))
		return DosPlayerData() + (uint32_t)(pointer - playerData_B7330_B7320);
	if (pointer >= options && pointer < entities)
		return dosState + dosOptions + (uint32_t)(pointer - options);
	if (pointer >= state && pointer < state + dosFreeEntities)
		return dosState + (uint32_t)(pointer - state);
	return 0;
}

static Type_Mc1Entity* PortEntity(uint32_t address, int32_t count)
{
	if (address < dosEntities || (address - dosEntities) % 0xA4 || (address - dosEntities) / 0xA4 >= (uint32_t)count)
		return nullptr; // a list end (entity 0 is a pointer too) or an address of another build of the game
	return &mc1Entities[(address - dosEntities) / 0xA4];
}

static uint8_t* PortTypes(uint32_t address)
{
	if (address >= dosTypes && address < dosTypes + sizeof(creatureTypes_98F38_98F38))
		return &creatureTypes_98F38_98F38[0][0] + (address - dosTypes);
	//!!!!! a type out of the table: the original reads the DOS memory there
	return creatureTypes_98F38_98F38[0];
}

static uint8_t* PortPlayerData(uint32_t address)
{
	if (!address)
		return DosAddressZero();
	if (address >= DosPlayerData() && address < DosPlayerData() + sizeof(playerData_B7330_B7320))
		return playerData_B7330_B7320 + (address - DosPlayerData());
	if (address >= dosState + dosOptions && address < dosState + dosEntityArray)
		return &mc1State_AE400_AE3F0->reflections_0x2195_8597 + (address - dosState - dosOptions);
	//!!!!! the movies of the game were saved by another build (B7330 at 515330h ...): the original keeps that address
	//!!!!! for all but the wizards of the players (416B0) and reads the DOS memory there
	return playerData_B7330_B7320;
}

// an entity in the layout of DOS (the pointers as DOS addresses)
static void EntityToDos(const Type_Mc1Entity& entity, uint8_t* dos)
{
	WriteU32(dos, DosEntity(entity.next_0x00_0));
	memcpy(dos + 4, (const uint8_t*)&entity + offsetof(Type_Mc1Entity, rand_0x04_4), 0x98);
	WriteU32(dos + 0x9C, DosPointer(entity.typeParams_0x9C_156));
	WriteU32(dos + 0xA0, DosPointer(entity.playerData_0xA0_160));
}

static void EntityFromDos(const uint8_t* dos, Type_Mc1Entity& entity, int32_t count)
{
	entity.next_0x00_0 = PortEntity(ReadU32(dos), count);
	memcpy((uint8_t*)&entity + offsetof(Type_Mc1Entity, rand_0x04_4), dos + 4, 0x98);
	entity.typeParams_0x9C_156 = PortTypes(ReadU32(dos + 0x9C));
	entity.playerData_0xA0_160 = PortPlayerData(ReadU32(dos + 0xA0));
}

// the state of the port as the 38D09h bytes of DOS (the entities 0-999)
static std::vector<uint8_t> StateToDos()
{
	const Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	std::vector<uint8_t> dos(dosStateSize, 0);
	memcpy(dos.data(), state, dosFreeEntities);
	for (int i = 0; i < 1000; i++)
		WriteU32(&dos[dosFreeEntities + i * 4], DosEntity(state->freeEntities_0x251_593[i]));
	WriteU32(&dos[dosReuseTop], (uint32_t)state->reuseTop_0x11F1_4593);
	for (int i = 0; i < 1000; i++)
		WriteU32(&dos[dosReuseEntities + i * 4], DosEntity(state->reuseEntities_0x11F5_4597[i]));
	memcpy(&dos[dosOptions], &state->reflections_0x2195_8597, dosEntityArray - dosOptions);
	for (int i = 0; i < 1000; i++)
		EntityToDos(mc1Entities[i], &dos[dosEntityArray + i * 0xA4]);
	memcpy(&dos[dosLevel], &state->level_0x2F503_193795, dosStateSize - dosLevel);
	return dos;
}

static uint8_t* DosEntityAt(std::vector<uint8_t>& dos, uint32_t index, int32_t count)
{
	if (index >= (uint32_t)count)
		return nullptr; //!!!!! a missing wizard (FC3Ah): the original writes the DOS memory there
	return index < 1000 ? &dos[dosEntityArray + index * 0xA4] : &dos[dosStateSize + (index - 1000) * 0xA4];
}

// 416B0 / 419F0 on the state of DOS: the player data of the wizards of the players, the types of the entities moved
// to the addresses of this game by the one of the wizard of player 0 (the row 7 of 98F38)
static void FixLoadedState_416B0_419F0(std::vector<uint8_t>& dos, int32_t count)
{
	uint16_t playerCount;
	memcpy(&playerCount, &dos[0x0A], sizeof(playerCount));
	for (uint32_t i = 0; i < playerCount; i++)
	{
		uint16_t wizard;
		memcpy(&wizard, &dos[0x3415 + i * 0x801], sizeof(wizard));
		if (uint8_t* entity = DosEntityAt(dos, wizard, count))
			WriteU32(entity + 0xA0, dosState + 0x340B + i * 0x801 + 0x44F);
	}
	uint16_t wizard;
	memcpy(&wizard, &dos[0x3415], sizeof(wizard));
	const uint8_t* wizardEntity = DosEntityAt(dos, wizard, count);
	const uint32_t delta = dosWizardType - (wizardEntity ? ReadU32(wizardEntity + 0x9C) : 0);
	for (int32_t i = 1; i < count; i++)
	{
		uint8_t* entity = DosEntityAt(dos, (uint32_t)i, count);
		if (entity[0x40])
			WriteU32(entity + 0x9C, ReadU32(entity + 0x9C) + delta);
	}
}

// the state of DOS (with the entities over 1000 after it) to the port
static void StateFromDos(std::vector<uint8_t>& dos, int32_t count)
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	memcpy(state, dos.data(), dosFreeEntities);
	for (int i = 0; i < 1000; i++)
		state->freeEntities_0x251_593[i] = PortEntity(ReadU32(&dos[dosFreeEntities + i * 4]), count);
	state->reuseTop_0x11F1_4593 = (int32_t)ReadU32(&dos[dosReuseTop]);
	for (int i = 0; i < 1000; i++)
		state->reuseEntities_0x11F5_4597[i] = PortEntity(ReadU32(&dos[dosReuseEntities + i * 4]), count);
	memcpy(&state->reflections_0x2195_8597, &dos[dosOptions], dosEntityArray - dosOptions);
	for (int32_t i = 0; i < count; i++)
		EntityFromDos(DosEntityAt(dos, (uint32_t)i, count), mc1Entities[i], count);
	for (int32_t i = count; i < mc1EntityCount; i++)
		memset(&mc1Entities[i], 0, sizeof(Type_Mc1Entity));
	memcpy(&state->level_0x2F503_193795, &dos[dosLevel], dosStateSize - dosLevel);
}

// the paths of DOS ("c:/CARPET.CD/save/gam00199.dat", "movie/gam00000.dat"), Mc1DataFilePath makes the port ones
static std::string GamePath(const char* directory, int16_t number, bool install, bool extended)
{
	char path[0x50];
	snprintf(path, sizeof(path), install ? "c:/CARPET.CD/%s/%s%05d.dat" : "%s/%s%05d.dat", directory, extended ? "gax" : "gam", number);
	return path;
}

static std::string MapPath(const char* directory, int16_t number, bool install)
{
	char path[0x50];
	snprintf(path, sizeof(path), install ? "c:/CARPET.CD/%s/map%05d.dat" : "%s/map%05d.dat", directory, number);
	return path;
}

static FILE* OpenFile(const std::string& path, int mode)
{
	return DataFileIO::CreateOrOpenFile(Mc1DataFilePath(path.c_str()).c_str(), mode);
}

static bool FileExists(const std::string& path)
{
	FILE* file = OpenFile(path, 0x200);
	if (!file)
		return false;
	DataFileIO::Close(file);
	return true;
}

// 3E7A0 / 3E460: the saved state exists (port: also the one with more entities)
static int32_t GameExists(const char* directory, int16_t number, bool install)
{
	return FileExists(GamePath(directory, number, install, false)) || FileExists(GamePath(directory, number, install, true));
}

// 3E690 / 3E350
static int32_t LoadGame(const char* directory, int16_t number, bool install)
{
	if (!GameExists(directory, number, install))
		return 0;
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	// the options of the game (2195h-21B8h) and the dword at 0 stay
	uint8_t options[0x24];
	uint32_t first;
	memcpy(options, &state->reflections_0x2195_8597, sizeof(options));
	memcpy(&first, state, sizeof(first));
	// 3EEA0: the file read over the state (a shorter one leaves its end) and unpacked
	std::vector<uint8_t> dos = StateToDos();
	int32_t count = 1000;
	const std::string extended = GamePath(directory, number, install, true);
	std::vector<uint8_t> file;
	if (FILE* handle = OpenFile(extended, 0x200))
	{
		file.resize((size_t)DataFileIO::FileLengthBytes(handle));
		DataFileIO::Read(handle, file.data(), (uint32_t)file.size());
		DataFileIO::Close(handle);
	}
	if (file.size() >= sizeof(extendedMagic) + 4 + dosStateSize && !memcmp(file.data(), extendedMagic, sizeof(extendedMagic)))
	{
		const uint32_t entities = ReadU32(&file[sizeof(extendedMagic)]);
		count = (int32_t)(std::min)((std::max)(entities, 1000u), (uint32_t)mc1EntityCount);
		dos.assign(file.begin() + sizeof(extendedMagic) + 4, file.end());
		dos.resize(dosStateSize + (count - 1000) * 0xA4, 0);
	}
	else
	{
		// port: room for a longer (packed) file, the original reads it over the memory after the state
		std::vector<uint8_t> buffer(dos);
		buffer.resize(0x100000);
		ReadPackedFile_3EEA0_3F1E0(GamePath(directory, number, install, false).c_str(), buffer.data());
		memcpy(dos.data(), buffer.data(), dosStateSize);
	}
	FixLoadedState_416B0_419F0(dos, count);
	StateFromDos(dos, count);
	memcpy(&state->reflections_0x2195_8597, options, sizeof(options));
	memcpy(state, &first, sizeof(first));
	RebuildFreeLists_37220_375E0();
	state->reuseTop_0x11F1_4593 = -1;
	return 1;
}

// 3E750 / 3E410: the state written, 1 when all of it was
static int32_t SaveGame(const char* directory, int16_t number, bool install)
{
	const std::string path = GamePath(directory, number, install, false);
	const std::string extended = GamePath(directory, number, install, true);
	// port: the folder made when it is missing
	std::error_code error;
	std::filesystem::create_directories(std::filesystem::path(Mc1DataFilePath(path.c_str())).parent_path(), error);
	std::vector<uint8_t> dos = StateToDos();
	int32_t count = 1000;
	for (int32_t i = mc1EntityCount - 1; i >= 1000; i--)
		if (mc1Entities[i].class_0x40_64)
		{
			count = i + 1;
			break;
		}
	if (count == 1000)
	{
		// the format of the original while only its 1000 entities are used
		std::filesystem::remove(Mc1DataFilePath(extended.c_str()), error);
		return OpenWriteClose_62E60_63370(Mc1DataFilePath(path.c_str()).c_str(), dos.data(), dosStateSize) == (int32_t)dosStateSize;
	}
	std::vector<uint8_t> file(extendedMagic, extendedMagic + sizeof(extendedMagic));
	file.resize(file.size() + 4);
	WriteU32(&file[sizeof(extendedMagic)], (uint32_t)count);
	file.insert(file.end(), dos.begin(), dos.end());
	for (int32_t i = 1000; i < count; i++)
	{
		uint8_t shadow[0xA4];
		EntityToDos(mc1Entities[i], shadow);
		file.insert(file.end(), shadow, shadow + sizeof(shadow));
	}
	std::filesystem::remove(Mc1DataFilePath(path.c_str()), error);
	return OpenWriteClose_62E60_63370(Mc1DataFilePath(extended.c_str()).c_str(), file.data(), (uint32_t)file.size()) == (int32_t)file.size();
}

// 3E980 / 3E640: the maps exist
static int32_t MapsExist(const char* directory, int16_t number, bool install)
{
	return FileExists(MapPath(directory, number, install));
}

// 3E7F0 / 3E4B0: the terrain, the heights, the shading, the angles, the entities by the cell and the table B5D40
static int32_t LoadMaps(const char* directory, int16_t number, bool install)
{
	if (!MapsExist(directory, number, install))
		return 0;
	FILE* file = OpenFile(MapPath(directory, number, install), 0x200);
	DataFileIO::Read(file, mapTerrainType_CC1E0_CC1D0, 0x10000);
	DataFileIO::Read(file, mapHeightmap_DC1E0_DC1D0, 0x10000);
	DataFileIO::Read(file, mapShading_EC1E0_EC1D0, 0x10000);
	DataFileIO::Read(file, mapAngle_FC1E0_FC1D0, 0x10000);
	DataFileIO::Read(file, (uint8_t*)mapEntityIndex_10C1E0_10C1D0, 0x20000);
	DataFileIO::Read(file, (uint8_t*)angleTypes_B5D40_B5D30, 0x12C2);
	DataFileIO::Close(file);
	return 1;
}

// 3E8C0 / 3E580: always 0
static int32_t SaveMaps(const char* directory, int16_t number, bool install)
{
	FILE* file = OpenFile(MapPath(directory, number, install), 0x222);
	if (!file)
		return 0; //!!!!! the original writes to the handle -1
	WriteFile_62ED0_633E0(file, mapTerrainType_CC1E0_CC1D0, 0x10000);
	WriteFile_62ED0_633E0(file, mapHeightmap_DC1E0_DC1D0, 0x10000);
	WriteFile_62ED0_633E0(file, mapShading_EC1E0_EC1D0, 0x10000);
	WriteFile_62ED0_633E0(file, mapAngle_FC1E0_FC1D0, 0x10000);
	WriteFile_62ED0_633E0(file, (uint8_t*)mapEntityIndex_10C1E0_10C1D0, 0x20000);
	WriteFile_62ED0_633E0(file, (uint8_t*)angleTypes_B5D40_B5D30, 0x12C2);
	DataFileIO::Close(file);
	return 0;
}

int32_t LoadMovieGame_3E350_3E690(int16_t movie) { return LoadGame("movie", movie, false); }
int32_t SaveMovieGame_3E410_3E750(int16_t movie) { return SaveGame("movie", movie, false); }
int32_t MovieGameExists_3E460_3E7A0(int16_t movie) { return GameExists("movie", movie, false); }
int32_t LoadMovieMaps_3E4B0_3E7F0(int16_t movie) { return LoadMaps("movie", movie, false); }
int32_t SaveMovieMaps_3E580_3E8C0(int16_t movie) { return SaveMaps("movie", movie, false); }
int32_t MovieMapsExist_3E640_3E980(int16_t movie) { return MapsExist("movie", movie, false); }
int32_t LoadGame_3E690_3E9D0(int16_t number) { return LoadGame("save", number, true); }
int32_t SaveGame_3E750_3EA90(int16_t number) { return SaveGame("save", number, true); }
int32_t GameExists_3E7A0_3EAE0(int16_t number) { return GameExists("save", number, true); }
int32_t LoadMaps_3E7F0_3EB30(int16_t number) { return LoadMaps("save", number, true); }
int32_t SaveMaps_3E8C0_3EC00(int16_t number) { return SaveMaps("save", number, true); }
int32_t MapsExist_3E980_3ECC0(int16_t number) { return MapsExist("save", number, true); }

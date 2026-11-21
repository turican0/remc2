#include "Mc1Regression.h"

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <fstream>

#include "Mc1Game.h"
#include "Mc1Shared.h"
#include "../../remc2/engine/engine_support.h"
#include "../../remc2/engine/lzcompress.h"

// reference.binz: "MC1REGZ1", u32 the record size, u32 the frames between the records, u32 the records, then each
// record as u32 the packed size + LzCompress of the record XOR the previous one (the first XOR zeros)

std::string mc1RegressionFolder;

static const char referenceMagic[8] = { 'M', 'C', '1', 'R', 'E', 'G', 'Z', '1' };

struct Mc1Reference
{
	uint32_t every = 0;
	std::vector<std::vector<uint8_t>> records;
};

static bool ReadReference(const std::string& file, Mc1Reference& reference, bool headerOnly)
{
	std::ifstream in(file, std::ios::binary);
	char magic[8];
	uint32_t header[3];
	if (!in.read(magic, 8) || memcmp(magic, referenceMagic, 8) || !in.read((char*)header, sizeof(header)) || header[0] != mc1RegressionRecordSize)
		return false;
	reference.every = header[1];
	reference.records.resize(header[2]);
	if (headerOnly)
		return true;
	std::vector<uint8_t> previous(mc1RegressionRecordSize, 0);
	for (std::vector<uint8_t>& record : reference.records)
	{
		uint32_t packedSize = 0;
		if (!in.read((char*)&packedSize, 4))
			return false;
		std::vector<uint8_t> packed(packedSize);
		if (!in.read((char*)packed.data(), packedSize))
			return false;
		record = LzDecompress(packed.data(), packed.size(), mc1RegressionRecordSize);
		if (record.size() != mc1RegressionRecordSize)
			return false;
		for (uint32_t i = 0; i < mc1RegressionRecordSize; i++)
			record[i] ^= previous[i];
		previous = record;
	}
	return true;
}

int32_t Mc1RegressionFrames(const std::string& folder)
{
	Mc1Reference reference;
	if (!ReadReference(folder + "/reference.binz", reference, true))
		return 0;
	return (int32_t)(reference.every * reference.records.size());
}

std::string Mc1RegressionInput()
{
	return mc1RegressionFolder.empty() ? "" : mc1RegressionFolder + "/input.rec";
}

static bool ReadWhole(const std::string& file, std::vector<uint8_t>& data)
{
	std::ifstream in(file, std::ios::binary | std::ios::ate);
	if (!in)
		return false;
	data.resize((size_t)in.tellg());
	in.seekg(0);
	return (bool)in.read((char*)data.data(), data.size());
}

bool PackMc1Reference(const std::string& dumpFolder, const std::string& outFile, uint32_t every)
{
	// the parts of a record in its order, each file the dumps of all the records
	static const struct { const char* name; uint32_t size; } parts[] = {
		{ "rand.bin", 4 }, { "entities.bin", 1000 * 0xA4 }, { "players.bin", 8 * 0x801 },
		{ "settings.bin", mc1RegressionSettingsSize }, { "terrain.bin", 0x10000 }, { "height.bin", 0x10000 }, { "angle.bin", 0x10000 },
	};
	std::vector<std::vector<uint8_t>> data(sizeof(parts) / sizeof(parts[0]));
	uint32_t count = 0;
	for (size_t i = 0; i < data.size(); i++)
	{
		if (!ReadWhole(dumpFolder + "/" + parts[i].name, data[i]) || data[i].size() % parts[i].size)
			return false;
		const uint32_t records = (uint32_t)(data[i].size() / parts[i].size);
		if (i && records != count)
			return false;
		count = records;
	}
	std::ofstream out(outFile, std::ios::binary);
	const uint32_t header[3] = { mc1RegressionRecordSize, every, count };
	out.write(referenceMagic, 8);
	out.write((const char*)header, sizeof(header));
	std::vector<uint8_t> previous(mc1RegressionRecordSize, 0);
	for (uint32_t r = 0; r < count; r++)
	{
		std::vector<uint8_t> record;
		for (size_t i = 0; i < data.size(); i++)
			record.insert(record.end(), data[i].begin() + (size_t)r * parts[i].size, data[i].begin() + (size_t)(r + 1) * parts[i].size);
		std::vector<uint8_t> delta(record);
		for (uint32_t i = 0; i < mc1RegressionRecordSize; i++)
			delta[i] ^= previous[i];
		const std::vector<uint8_t> packed = LzCompress(delta.data(), delta.size());
		const uint32_t packedSize = (uint32_t)packed.size();
		out.write((const char*)&packedSize, 4);
		out.write((const char*)packed.data(), packed.size());
		previous = record;
	}
	return (bool)out;
}

// the offsets of the parts in a record
constexpr uint32_t entitiesAt = 4;
constexpr uint32_t playersAt = entitiesAt + 1000 * 0xA4;
constexpr uint32_t settingsAt = playersAt + 8 * 0x801;
constexpr uint32_t mapsAt = settingsAt + mc1RegressionSettingsSize;
// the DOS entities at 26AF3h: a pointer as its value in DOS
constexpr uint32_t dosEntities = 0x26AF3;

// the bytes not compared: the pointers of the entities (next, typeParams, playerData), the movie handle and the
// heap of the settings
static bool Ignored(uint32_t i)
{
	if (i >= entitiesAt && i < playersAt)
	{
		const uint32_t offset = (i - entitiesAt) % 0xA4;
		return offset < 4 || offset >= 0x9C;
	}
	if (i >= settingsAt && i < mapsAt)
	{
		const uint32_t offset = i - settingsAt;
		return (offset >= 0x09 && offset < 0x0D) || (offset >= 0xA8 && offset < 0xAC);
	}
	return false;
}

// the settings in the DOS layout: the pointers 4 bytes, the lists of the entities as their DOS addresses
static void SettingsRecord(uint8_t* at)
{
	const Type_Mc1Settings_AE408_AE3F8* settings = mc1Settings_AE408_AE3F8;
	const uint8_t* from = (const uint8_t*)settings;
	const size_t movie = offsetof(Type_Mc1Settings_AE408_AE3F8, movieFile_0x09_9);
	const size_t heap = offsetof(Type_Mc1Settings_AE408_AE3F8, heap_0xA8_168);
	memcpy(at, from, 0x09);
	memcpy(at + 0x0D, from + movie + sizeof(settings->movieFile_0x09_9), 0xA8 - 0x0D);
	memcpy(at + 0xAC, from + heap + sizeof(settings->heap_0xA8_168), 0x8E1A - 0xAC);
	static_assert(sizeof(Type_Mc1Settings_AE408_AE3F8) - offsetof(Type_Mc1Settings_AE408_AE3F8, entityLists_0x8E1A_36378) == 25 * sizeof(Type_Mc1Entity*), "the lists at the end");
	for (int i = 0; i < 25; i++)
	{
		const Type_Mc1Entity* entity = settings->entityLists_0x8E1A_36378[i];
		const uint32_t value = entity ? dosEntities + (uint32_t)(entity - mc1State_AE400_AE3F0->entities_0x7463_29795) * 0xA4 : 0;
		memcpy(at + 0x8E1A + i * 4, &value, 4);
	}
}

// the live state as a record: the entities without their pointers (next, typeParams, playerData stay 0)
static std::vector<uint8_t> LiveRecord()
{
	static_assert(offsetof(Type_Mc1Entity, typeParams_0x9C_156) - offsetof(Type_Mc1Entity, rand_0x04_4) == 0x98, "the entity in the DOS layout");
	static_assert(sizeof(mc1State_AE400_AE3F0->players_0x340B_13323) == 8 * 0x801, "the players in the DOS layout");
	std::vector<uint8_t> record(mc1RegressionRecordSize, 0);
	uint8_t* at = record.data();
	memcpy(at, &mc1State_AE400_AE3F0->rand_0x04_4, 4);
	at += 4;
	for (int i = 0; i < 1000; i++, at += 0xA4)
		memcpy(at + 4, (const uint8_t*)&mc1State_AE400_AE3F0->entities_0x7463_29795[i] + offsetof(Type_Mc1Entity, rand_0x04_4), 0x98);
	memcpy(at, mc1State_AE400_AE3F0->players_0x340B_13323, 8 * 0x801);
	at += 8 * 0x801;
	SettingsRecord(at);
	at += mc1RegressionSettingsSize;
	memcpy(at, mapTerrainType_CC1E0_CC1D0, 0x10000);
	memcpy(at + 0x10000, mapHeightmap_DC1E0_DC1D0, 0x10000);
	memcpy(at + 0x20000, mapAngle_FC1E0_FC1D0, 0x10000);
	return record;
}

// the first differences as text: the part, the entity or the player and the offset
static void LogDifferences(const std::vector<uint8_t>& expected, const std::vector<uint8_t>& live, int32_t frame)
{
	int shown = 0;
	uint32_t count = 0;
	for (uint32_t i = 0; i < mc1RegressionRecordSize; i++)
	{
		if (Ignored(i) || expected[i] == live[i])
			continue;
		count++;
		if (shown >= 16)
			continue;
		shown++;
		char text[128];
		if (i < entitiesAt)
			snprintf(text, sizeof(text), "rand +%u", i);
		else if (i < playersAt)
			snprintf(text, sizeof(text), "entity %u +%02Xh", (i - entitiesAt) / 0xA4, (i - entitiesAt) % 0xA4);
		else if (i < settingsAt)
			snprintf(text, sizeof(text), "player %u +%03Xh", (i - playersAt) / 0x801, (i - playersAt) % 0x801);
		else if (i < mapsAt)
			snprintf(text, sizeof(text), "settings +%04Xh", i - settingsAt);
		else
		{
			static const char* maps[3] = { "terrain", "height", "angle" };
			const uint32_t map = i - mapsAt;
			snprintf(text, sizeof(text), "%s %04Xh", maps[map >> 16], map & 0xFFFF);
		}
		Logger->error("MC1 regression frame {}: {} DOSBox {:02X} port {:02X}", frame, text, expected[i], live[i]);
	}
	Logger->error("MC1 regression frame {}: {} bytes differ", frame, count);
}

void Mc1RegressionFrame()
{
	if (mc1RegressionFolder.empty())
		return;
	static Mc1Reference reference;
	static bool loaded = false;
	static int32_t frame = 0;
	if (!loaded)
	{
		loaded = true;
		if (!ReadReference(mc1RegressionFolder + "/reference.binz", reference, false) || !reference.every || reference.records.empty())
		{
			Logger->error("MC1 regression: no reference in {}", mc1RegressionFolder);
			End_thread(-1);
			return;
		}
	}
	frame++;
	if (unitTestsProgress)
	{
		printf("@PROGRESS %d\n", frame);
		fflush(stdout);
	}
	if (frame % reference.every)
		return;
	const size_t index = frame / reference.every - 1;
	const std::vector<uint8_t> live = LiveRecord();
	const std::vector<uint8_t>& expected = reference.records[index];
	for (uint32_t i = 0; i < mc1RegressionRecordSize; i++)
	{
		if (Ignored(i))
			continue;
		if (expected[i] != live[i])
		{
			LogDifferences(expected, live, frame);
			End_thread(-1);
			return;
		}
	}
	if (index + 1 == reference.records.size())
	{
		Logger->info("MC1 regression: {} frames equal to DOSBox", frame);
		End_thread(20);
	}
}

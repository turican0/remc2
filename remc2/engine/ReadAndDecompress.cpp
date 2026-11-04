#include "ReadAndDecompress.h"
#include "TextureMaps.h"
#include <cstring>
#include <vector>
uint8_t BigTextureBuffer[128 * 128 * 160];

//----- (00054630) --------------------------------------------------------
void sub_54630_load_psxblock(uint16_t TextSize)//235630
{
	switch (TextSize)
	{
	case 16:
		DataFileIO::LoadFileArray_84250(psxadatablock16dat);
		break;
	case 32:
		DataFileIO::LoadFileArray_84250(psxadatablock32dat);
		break;
	case 128:
		break;
	}
}

void UpdateTileTextures(uint32_t tileSize, uint8_t* tileBuffer, MapType_t mapType)
{
	constexpr uint32_t kTilesWide = 8;
	constexpr uint32_t kTilesTall = 19;
	// The block buffers are always 256 pixels wide (see sub_5B7A0_prepare_textures), so
	// 32px tiles sit 8 to a row but 128px tiles only 2. Repack into the kTilesWide atlas
	// GameRenderHW addresses, whatever the tile size.
	constexpr uint32_t kSourceWidth = 256;
	const uint32_t sourceTilesWide = kSourceWidth / tileSize;
	const uint32_t atlasWidth = tileSize * kTilesWide;
	const uint32_t atlasHeight = tileSize * kTilesTall;

	std::vector<uint8_t> atlas(atlasWidth * atlasHeight);
	for (uint32_t tile = 0; tile < kTilesWide * kTilesTall; tile++)
	{
		const uint8_t* src = tileBuffer + (tile / sourceTilesWide) * tileSize * kSourceWidth + (tile % sourceTilesWide) * tileSize;
		uint8_t* dst = atlas.data() + (tile / kTilesWide) * tileSize * atlasWidth + (tile % kTilesWide) * tileSize;
		for (uint32_t row = 0; row < tileSize; row++)
			memcpy(dst + row * atlasWidth, src + row * kSourceWidth, tileSize);
	}

	EventDispatcher::I->DispatchEvent<ResourceType, uint32_t, uint8_t*, uint32_t, uint32_t>(
		EventType::E_RESOURCE_CHANGE, ResourceType::TEXTURE_LOADED,
		kTerrainAtlasId, atlas.data(), atlasWidth, atlasHeight);
	
	//WriteTextureMapToBmp(kTerrainAtlasId, atlas.data(), atlasWidth, atlasHeight, mapType);
}

void UpdateSkyTexture(uint32_t size, uint8_t* skyBuffer)
{
	EventDispatcher::I->DispatchEvent<ResourceType, uint32_t, uint8_t*, uint32_t, uint32_t>(
		EventType::E_RESOURCE_CHANGE, ResourceType::TEXTURE_LOADED,
		kSkyId, skyBuffer, size, size);
}

//----- (00054660) --------------------------------------------------------
void ReadAndDecompressSkyAndBlocks_54660(MapType_t GraphicsType, uint8_t GraphicsSize)//235660
{
	char dataPath[MAX_PATH];

	switch (GraphicsType)
	{
	case MapType_t::Day://basic graphics
	{
		switch (GraphicsSize)
		{
		case 16:
		{
			sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/BLOCK16.DAT");
			DataFileIO::ReadFileAndDecompress(dataPath, &BLOCK32DAT_BEGIN_BUFFER);//2bac2c
			break;
		}
		case 32:
		{
			sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/BLOCK32.DAT");
			DataFileIO::ReadFileAndDecompress(dataPath, &BLOCK32DAT_BEGIN_BUFFER);//2bac2c
			sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/SKYD0-0.DAT");
			DataFileIO::ReadFileAndDecompress(dataPath, &off_D41A8_sky);//2a51a8
			UpdateSkyTexture(256, off_D41A8_sky);
			break;
		}
		case 128:
		{
			sprintf(dataPath, "%s/%s", highResGraphicsPath.c_str(), "block128.data");
			ReadGraphicsfile(dataPath, BigTextureBuffer);//advance graphics
			sprintf(dataPath, "%s/%s", highResGraphicsPath.c_str(), "skyd1024.data");
			ReadGraphicsfile(dataPath, off_D41A8_sky);//2a51a8
			UpdateSkyTexture(1024, off_D41A8_sky);
			break;
		}
		}
		sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/TMAPS0-0.TAB");
		DataFileIO::ReadFileAndDecompress(dataPath, (uint8_t**)&str_TMAPS00TAB_BEGIN_BUFFER);//2c7ed0
		break;
	}
	case MapType_t::Night://? and night
	{
		switch (GraphicsSize)
		{
		case 16:
		{
			if (D41A0_0.terrain_2FECE.byte_0x2FED2 & 2)
			{
				sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/BL16F0-0.DAT");
				DataFileIO::ReadFileAndDecompress(dataPath, &BLOCK32DAT_BEGIN_BUFFER);//2bac2c
			}
			else
			{
				sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/BL16N0-0.DAT");
				DataFileIO::ReadFileAndDecompress(dataPath, &BLOCK32DAT_BEGIN_BUFFER);//2bac2c
			}
			break;
		}
		case 32:
		{
			if (D41A0_0.terrain_2FECE.byte_0x2FED2 & 2)
			{
				sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/BL32F0-0.DAT");
				DataFileIO::ReadFileAndDecompress(dataPath, &BLOCK32DAT_BEGIN_BUFFER);//2bac2c
			}
			else
			{
				sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/BL32N0-0.DAT");
				DataFileIO::ReadFileAndDecompress(dataPath, &BLOCK32DAT_BEGIN_BUFFER);//2bac2c
			}
			sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/SKYN0-0.DAT");
			DataFileIO::ReadFileAndDecompress(dataPath, &off_D41A8_sky);//2a51a8
			UpdateSkyTexture(256, off_D41A8_sky);
			break;
		}
		case 128:
		{
			if (D41A0_0.terrain_2FECE.byte_0x2FED2 & 2)
			{
				sprintf(dataPath, "%s/%s", highResGraphicsPath.c_str(), "bl128f0-0.data");
				ReadGraphicsfile(dataPath, BigTextureBuffer);//advance graphics
			}
			else
			{
				sprintf(dataPath, "%s/%s", highResGraphicsPath.c_str(), "bl128n0-0.data");
				ReadGraphicsfile(dataPath, BigTextureBuffer);//advance graphics
			}
			sprintf(dataPath, "%s/%s", highResGraphicsPath.c_str(), "skyn1024.data");
			ReadGraphicsfile(dataPath, off_D41A8_sky);//2a51a8
			UpdateSkyTexture(1024, off_D41A8_sky);
			break;
		}
		}
		sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/TMAPS1-0.TAB");
		DataFileIO::ReadFileAndDecompress(dataPath, (uint8_t**)&str_TMAPS00TAB_BEGIN_BUFFER);//2c7ed0
		break;
	}
	case MapType_t::Cave://cave
	{
		switch (GraphicsSize)
		{
		case 16:
		{
			sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/BL16C0-0.DAT");
			DataFileIO::ReadFileAndDecompress(dataPath, &BLOCK32DAT_BEGIN_BUFFER);//2bac2c
			break;
		}
		case 32:
		{
			sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/BL32C0-0.DAT");
			DataFileIO::ReadFileAndDecompress(dataPath, &BLOCK32DAT_BEGIN_BUFFER);//2bac2c
			break;
		}
		case 128:
		{
			sprintf(dataPath, "%s/%s", highResGraphicsPath.c_str(), "bl128c0-0.data");
			ReadGraphicsfile(dataPath, BigTextureBuffer);//advance graphics
			break;
		}
		}
		sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/TMAPS2-0.TAB");
		DataFileIO::ReadFileAndDecompress(dataPath, (uint8_t**)&str_TMAPS00TAB_BEGIN_BUFFER);//2c7ed0
		break;
	}
	}
	if (GraphicsSize < 128)
		UpdateTileTextures(GraphicsSize, BLOCK32DAT_BEGIN_BUFFER, GraphicsType);
	else
		UpdateTileTextures(GraphicsSize, BigTextureBuffer, GraphicsType);
}


//----- (00054800) --------------------------------------------------------
void sub_54800_read_and_decompress_tables(MapType_t a1)//235800
{
	char dataPath[MAX_PATH];

	if (a1 == MapType_t::Day)
	{
		sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/TABLESD.DAT");
		DataFileIO::ReadFileAndDecompress(dataPath, &x_BYTE_F6EE0_tablesx_pre);//2c7ee0
		m_BackgroundColour_D4B7E = 0x00;
		m_BackgroundColour_D4B7C = 0xfe;
	}
	else if (a1 == MapType_t::Night)
	{
		sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/TABLESN.DAT");
		DataFileIO::ReadFileAndDecompress(dataPath, &x_BYTE_F6EE0_tablesx_pre);
		m_BackgroundColour_D4B7E = 0xff;
		m_BackgroundColour_D4B7C = 0x00;
	}
	else if (a1 == MapType_t::Cave)
	{
		sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/TABLESC.DAT");
		DataFileIO::ReadFileAndDecompress(dataPath, &x_BYTE_F6EE0_tablesx_pre);
		m_BackgroundColour_D4B7C = 0xfe;
		m_BackgroundColour_D4B7E = 0xff;
	}
}


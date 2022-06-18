#include "GameRender.h"

GameRender::GameRender(uint8_t* pScreenBuffer, uint8_t* pColorPalette, uint16_t screenWidth, uint16_t screenHeight, uint16_t viewPortPosX, uint16_t viewPortPosY, uint16_t viewPortWidth, uint16_t viewPortHeight, std::array<uint8_t*, 256> &textureAdresses, uint8_t pX_BYTE_F6EE0_tablesx[], uint8_t renderThreads, bool assignToSpecificCores)
{
	m_ptrScreenBuffer = pScreenBuffer;
	m_ptrColorPalette = pColorPalette;
	SetRenderViewPortSize_BCD45(ViewPort(viewPortPosX, viewPortPosY, viewPortWidth, viewPortHeight), screenWidth, screenHeight);
	SetTextures(textureAdresses);
	m_ptrX_BYTE_F6EE0_tablesx = pX_BYTE_F6EE0_tablesx;

	m_assignToSpecificCores = assignToSpecificCores;
	SetRenderThreads(renderThreads);
}

GameRender::~GameRender()
{
	if (m_renderThreads.size() > 0)
	{
		StopWorkerThreads();
	}
}

void GameRender::DrawWorld(int posX, int posY, int16_t yaw, int16_t posZ, int16_t pitch, int16_t roll, int16_t fov, uint8_t heightmap[], type_particle_str** str_DWORD_F66F0x[], uint8_t x_BYTE_E88E0x[], int32_t x_DWORD_F5730[], uint8_t unk_F0A20x[], type_event_0x6E8E* x_DWORD_EA3E4[], type_str_unk_1804B0ar str_unk_1804B0ar, int16_t x_WORD_180660_VGA_type_resolution, int16_t x_WORD_D4B7C, char isCaveLevel)
{
	m_x_WORD_180660_VGA_type_resolution = x_WORD_180660_VGA_type_resolution;
	unsigned __int16 v8; // ax
	int v9; // ecx
	int v10; // ebx
	int v11; // edx
	int v12; // ecx
	int v13; // ebx
	int v14; // edx
	int v15; // ecx
	int v16; // ebx
	int v17; // edx
	int v18; // ecx
	int v19; // ebx
	int v20; // edx
	int vYaw; // esi
	int v22; // edx
	int v23; // ebx
	uint32_t v24; // edx
	int v25; // ebx
	int v26; // edi
	//char result; // al
	int v28; // ebx
	uint32_t v29; // edx
	int v30; // ebx
	int v31; // edi
	uint8_t* v32; // ST2C_4
	//uint8_t* v33; // edx
	__int64 v34; // rax
	uint8_t* v35; // edi
	int v36; // eax
	x_BYTE* v37; // esi
	signed int v38; // ecx
	unsigned __int16 v39; // bx
	unsigned __int16 v40; // dx
	unsigned __int16 v41; // bx
	unsigned __int16 v42; // dx
	x_BYTE* v43; // edi
	int v44; // esi
	int v45; // ecx
	int v46; // eax
	int v47; // ebx
	int v48; // edx
	int v49; // [esp+0h] [ebp-1Ch]
	int v50; // [esp+4h] [ebp-18h]
	signed int v51; // [esp+8h] [ebp-14h]
	char v52; // [esp+Ch] [ebp-10h]
	uint8_t* v53; // [esp+14h] [ebp-8h]
	int i; // [esp+18h] [ebp-4h]
	int vPosX; // [esp+34h] [ebp+18h]
	int vPosY; // [esp+38h] [ebp+1Ch]

	LOBYTE(v8) = HIBYTE(posX);
	HIBYTE(v8) = HIBYTE(posY);
	if ((signed int)(unsigned __int8)posX < 128)
		LOBYTE(v8) = HIBYTE(posX) - 1;
	if ((signed int)(unsigned __int8)posY < 128)
		HIBYTE(v8) = HIBYTE(posY) - 1;
	v9 = heightmap[v8];
	LOBYTE(v8) += 2;
	v10 = v9;
	v11 = v9;
	v12 = heightmap[v8];
	HIBYTE(v8) += 2;
	v13 = v10 - v12;
	v14 = v12 + v11;
	v15 = heightmap[v8];
	LOBYTE(v8) -= 2;
	v16 = v13 - v15;
	v17 = v14 - v15;
	v18 = heightmap[v8];
	v19 = 2 * (v18 + v16);
	v20 = 2 * (v17 - v18);
	if (v19 <= 100)
	{
		if (v19 < -100)
			v19 = -100;
	}
	else
	{
		v19 = 100;
	}
	if (v20 <= 100)
	{
		if (v20 < -100)
			v20 = -100;
	}
	else
	{
		v20 = 100;
	}
	vYaw = yaw & 0x7FF;
	x_DWORD_D4794 += (v19 - x_DWORD_D4794) >> 3;
	x_DWORD_D4798 += (v20 - x_DWORD_D4798) >> 3;
	vPosX = x_DWORD_D4794 + posX;
	vPosY = x_DWORD_D4798 + posY;

	if (D41A0_0.m_GameSettings.str_0x2192.xxxx_0x2193 && m_uiScreenSize && m_uiScreenWidth == 640)
	{
		uint16_t origScreenWidth = m_uiScreenWidth;
		uint16_t origScreenHeight = m_uiScreenHeight;

		//VR interlaced render
		SetRenderViewPortSize_BCD45(
			m_ptrScreenBuffer,
			(origScreenWidth / 2) - 8,
			(origScreenHeight / 2) - 40,
			2 * origScreenWidth);
		v22 = Maths::x_DWORD_DB750[vYaw];
		x_DWORD_D4790 = 20;
		v23 = 5 * v22;
		v24 = Maths::x_DWORD_DB750[0x200 + vYaw];
		x_DWORD_D4324 = -5;
		v25 = 4 * v23 >> 16;
		v26 = 20 * (signed int)v24 >> 16;
		DrawTerrainAndParticles_3C080(vPosX - v26, vPosY - v25, vYaw, posZ, pitch, roll, fov, str_DWORD_F66F0x, x_BYTE_E88E0x, x_DWORD_F5730, unk_F0A20x, x_DWORD_EA3E4, str_unk_1804B0ar, x_WORD_D4B7C, isCaveLevel, m_viewPort, origScreenWidth);
		SetRenderViewPortSize_BCD45(m_ptrScreenBuffer + (origScreenWidth / 2), 0, 0, 0);
		x_DWORD_D4324 = 5;
		DrawTerrainAndParticles_3C080(vPosX + v26, vPosY + v25, vYaw, posZ, pitch, roll, fov, str_DWORD_F66F0x, x_BYTE_E88E0x, x_DWORD_F5730, unk_F0A20x, x_DWORD_EA3E4, str_unk_1804B0ar, x_WORD_D4B7C, isCaveLevel, m_viewPort, origScreenWidth);
		x_DWORD_D4324 = 0;
		SetRenderViewPortSize_BCD45(m_ptrScreenBuffer, origScreenWidth, origScreenHeight, origScreenWidth);
	}
	else if (m_uiScreenSize != 1 || D41A0_0.m_GameSettings.str_0x2192.xxxx_0x2193)
	{
		v52 = D41A0_0.m_GameSettings.m_Display.xxxx_0x2191;
		if (x_WORD_180660_VGA_type_resolution == 1)
		{
			if (!D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].byte_0x3DF_2BE4_12221)
			{
				if (x_D41A0_BYTEARRAY_4_struct.m_wHighSpeedSystem)
				{
					if (x_DWORD_E9C3C)
					{
						if (D41A0_0.m_GameSettings.m_Graphics.m_wViewPortSize == 40)
						{
							v34 = x_DWORD_EA3E4[D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].word_0x00a_2BE4_11240]->word_0x82_130;
							if ((signed int)((HIDWORD(v34) ^ v34) - HIDWORD(v34)) > 80)
								D41A0_0.m_GameSettings.m_Display.xxxx_0x2191 = 1;
						}
					}
				}
			}
		}
		if (D41A0_0.str_0x21AE.xxxx_0x21B1 && D41A0_0.m_GameSettings.m_Display.xxxx_0x2191 && x_DWORD_E9C3C)
		{
			//Blur
			v35 = m_ptrViewPortRenderBufferStart;
			SetRenderViewPortSize_BCD45(x_DWORD_E9C3C, 0, 0, 0);
			DrawTerrainAndParticles_3C080(vPosX, vPosY, vYaw, posZ, pitch, roll, fov, str_DWORD_F66F0x, x_BYTE_E88E0x, x_DWORD_F5730, unk_F0A20x, x_DWORD_EA3E4, str_unk_1804B0ar, x_WORD_D4B7C, isCaveLevel, m_viewPort, m_uiScreenWidth);
			//Apply Blur
			SetRenderViewPortSize_BCD45(v35, 0, 0, 0);
			v51 = (signed int)(unsigned __int16)m_viewPort.Width >> 2;
			v49 = m_uiScreenWidth - (unsigned __int16)m_viewPort.Width;
			v50 = (unsigned __int16)m_viewPort.Height;

			if (D41A0_0.m_GameSettings.m_Display.xxxx_0x2191 != 1)
			{
				v37 = (x_BYTE*)x_DWORD_E9C3C;
				goto LABEL_33;
			}
			v37 = (x_BYTE*)x_DWORD_E9C3C;
			v38 = (signed int)(unsigned __int16)m_viewPort.Width >> 2;
			LOBYTE(v39) = *(x_BYTE*)(x_DWORD_E9C3C + 2);
			HIBYTE(v39) = v35[2];
			LOBYTE(v40) = *(x_BYTE*)(x_DWORD_E9C3C + 3);
			LOBYTE(v36) = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v39];
			HIBYTE(v40) = v35[3];
			HIBYTE(v36) = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v40];
			v36 <<= 16;
			LOBYTE(v39) = *(x_BYTE*)x_DWORD_E9C3C;
			HIBYTE(v39) = *v35;
			LOBYTE(v40) = *(x_BYTE*)(x_DWORD_E9C3C + 1);
			LOBYTE(v36) = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v39];
			HIBYTE(v40) = v35[1];
			for (BYTE1(v36) = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v40]; ; BYTE1(v36) = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v42])
			{
				*(x_DWORD*)v35 = v36;
				v35 += 4;
				v37 += 4;
				if (!--v38)
				{
					HIWORD(v36) = HIWORD(v49);
					v37 += v49;
					v35 += v49;
					if (!--v50)
						goto LABEL_44;
				LABEL_33:
					v38 = v51;
				}
				HIBYTE(v41) = v37[2];
				LOBYTE(v41) = v35[2];
				HIBYTE(v42) = v37[3];
				LOBYTE(v36) = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v41];
				LOBYTE(v42) = v35[3];
				HIBYTE(v36) = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v42];
				v36 <<= 16;
				HIBYTE(v41) = *v37;
				LOBYTE(v41) = *v35;
				HIBYTE(v42) = v37[1];
				LOBYTE(v36) = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v41];
				LOBYTE(v42) = v35[1];
			}
		}

		DrawTerrainAndParticles_3C080(vPosX, vPosY, vYaw, posZ, pitch, roll, fov, str_DWORD_F66F0x, x_BYTE_E88E0x, x_DWORD_F5730, unk_F0A20x, x_DWORD_EA3E4, str_unk_1804B0ar, x_WORD_D4B7C, isCaveLevel, m_viewPort, m_uiScreenWidth);

		if (D41A0_0.m_GameSettings.str_0x2192.xxxx_0x2192)
		{
			v53 = m_ptrViewPortRenderBufferStart;
			for (i = (unsigned __int16)m_viewPort.Height - 1; i; i--)
			{
				v43 = (x_BYTE*)v53;
				v44 = m_uiScreenWidth;
				v45 = (unsigned __int16)m_viewPort.Width - 1;
				HIWORD(v46) = 0;
				HIWORD(v47) = 0;
				HIWORD(v48) = 0;
				do
				{
					LOBYTE(v46) = v43[0];
					LOBYTE(v47) = v43[1];
					BYTE1(v46) = v43[v44];
					LOBYTE(v48) = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v46];
					BYTE1(v47) = v43[v44 + 1];
					BYTE1(v48) = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v47];
					*v43++ = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v48];
					v45--;
				} while (v45);
				v53 += m_uiScreenWidth;
			}
		}
	LABEL_44:
		//result = (int)x_D41A0_BYTEARRAY_0;
		D41A0_0.m_GameSettings.m_Display.xxxx_0x2191 = v52;
	}
	else
	{
		v28 = 5 * Maths::x_DWORD_DB750[vYaw];
		v29 = Maths::x_DWORD_DB750[0x200 + vYaw];
		x_DWORD_D4790 = 20;
		x_DWORD_D4324 = m_uiScreenWidth / 40;
		v30 = 4 * v28 >> 16;
		v31 = 20 * (signed int)v29 >> 16;
		DrawTerrainAndParticles_3C080(v31 + vPosX, v30 + vPosY, vYaw, posZ, pitch, roll, fov, str_DWORD_F66F0x, x_BYTE_E88E0x, x_DWORD_F5730, unk_F0A20x, x_DWORD_EA3E4, str_unk_1804B0ar, x_WORD_D4B7C, isCaveLevel, m_viewPort, m_uiScreenWidth);
		v32 = m_ptrViewPortRenderBufferStart;
		SetRenderViewPortSize_BCD45(x_DWORD_E9C3C, 0, 0, 0);
		x_DWORD_D4324 = 0 - (m_uiScreenWidth / 40);
		DrawTerrainAndParticles_3C080(vPosX - v31, vPosY - v30, vYaw, posZ, pitch, roll, fov, str_DWORD_F66F0x, x_BYTE_E88E0x, x_DWORD_F5730, unk_F0A20x, x_DWORD_EA3E4, str_unk_1804B0ar, x_WORD_D4B7C, isCaveLevel, m_viewPort, m_uiScreenWidth);
		SetRenderViewPortSize_BCD45(v32, 0, 0, 0);
		x_DWORD_D4324 = 0;
	}
}

void GameRender::WriteWorldToBMP()
{
	char path[MAX_PATH];
	GetSubDirectoryPath(path, "BufferOut");
	if (myaccess(path, 0) < 0)
	{
		mymkdir(path);
	}

	GetSubDirectoryPath(path, "BufferOut/PaletteOut.bmp");
	BitmapIO::WritePaletteAsImageBMP(path, 256, m_ptrColorPalette);
	GetSubDirectoryPath(path, "BufferOut/BufferOut.bmp");
	BitmapIO::WriteImageBufferAsImageBMP(path, m_uiScreenWidth, m_uiScreenHeight, m_ptrColorPalette, m_ptrScreenBuffer);
}

void GameRender::ClearGraphicsBuffer(uint8_t colorIdx)
{
	if (colorIdx > 255)
	{
		colorIdx = 255;
	}
	memset32(m_ptrScreenBuffer, colorIdx, m_uiScreenWidth * m_uiScreenWidth);
}

void GameRender::DrawSky_40950_TH(int16_t roll, uint8_t* ptrViewPortRenderBufferStart, uint16_t viewPortWidth, uint16_t viewPortHeight, uint16_t pitch)
{
	if (m_renderThreads.size() > 0)
	{
		uint8_t drawEveryNthLine = m_renderThreads.size() + 1;
		uint8_t i = 0;

		for (i = 0; i < m_renderThreads.size(); i++)
		{
			m_renderThreads[i]->Run([this, roll, ptrViewPortRenderBufferStart, viewPortWidth, viewPortHeight, pitch, i, drawEveryNthLine] {
				this->DrawSky_40950(roll, ptrViewPortRenderBufferStart, viewPortWidth, viewPortHeight, pitch, i, drawEveryNthLine);
				});
		}

		DrawSky_40950(roll, ptrViewPortRenderBufferStart, viewPortWidth, viewPortHeight, pitch, i, drawEveryNthLine);

		WaitForRenderFinish();

	}
	else
	{
		DrawSky_40950(roll, ptrViewPortRenderBufferStart, viewPortWidth, viewPortHeight, pitch, 0, 1);
	}
}

/*
* Sky texture is currently 256x256
*/
void GameRender::DrawSky_40950(int16_t roll, uint8_t* ptrViewPortRenderBufferStart, uint16_t viewPortWidth, uint16_t viewPortHeight, uint16_t pitch, uint8_t startLine, uint8_t drawEveryNthLine)
{
	if (drawEveryNthLine < 1)
	{
		drawEveryNthLine = 1;
	}

	int v1; // ebx
	int v2; // edx
	int v3; // esi
	int v4; // ebx
	char* v5; // edx
	int v6; // eax
	int v7; // edx
	int v8; // eax
	int v9; // ecx
	int v10; // edx
	__int16 result; // ax
	int v12; // eax
	char* v13; // esi
	int* v14; // edi
	uint8_t* v15; // edx
	int v16; // ecx
	int v17; // ebx
	int v18; // eax
	char v19ar[3840]; // [esp+0h] [ebp-52Ch]
	int v20; // [esp+500h] [ebp-2Ch]
	int v21; // [esp+504h] [ebp-28h]
	int* v22; // [esp+508h] [ebp-24h]
	int v23; // [esp+50Ch] [ebp-20h]
	int v24; // [esp+510h] [ebp-1Ch]
	int v25; // [esp+514h] [ebp-18h]
	int v26; // [esp+518h] [ebp-14h]
	int v27; // [esp+51Ch] [ebp-10h]
	char v28; // [esp+520h] [ebp-Ch]
	char v29; // [esp+524h] [ebp-8h]
	unsigned __int8 v30; // [esp+528h] [ebp-4h]

	v1 = roll & 0x7FF;
	v2 = (x_DWORD)Maths::x_DWORD_DB750[512 + v1] << 8;
	v26 = (Maths::x_DWORD_DB750[v1] << 8) / viewPortWidth;
	v3 = 0;
	v25 = v2 / viewPortWidth;
	v4 = 0;
	v29 = 0;
	v5 = v19ar;
	v30 = 0;

	uint16_t width = viewPortWidth;
	while (width)
	{
		v28 = BYTE2(v3);
		*v5 = BYTE2(v3) - v29;
		v21 = BYTE2(v4);
		v20 = BYTE2(v4) - v30;
		v5 += 2;
		width--;
		*(v5 - 1) = BYTE2(v4) - v30;
		v29 = v28;
		v30 = BYTE2(v4);
		v4 += v26;
		v3 += v25;
	}

	v7 = (-(m_str_F2C20ar.dword0x0d * m_str_F2C20ar.dword0x22) >> 16) + m_str_F2C20ar.dword0x24;
	v8 = m_str_F2C20ar.dword0x10 - (m_str_F2C20ar.dword0x11 * m_str_F2C20ar.dword0x22 >> 16);
	v9 = v7 * v25 - v8 * v26;
	v10 = v25 * v8 + v26 * v7;
	v23 = ((unsigned __int16)x_WORD_F2CC0 << 15) - v9;
	uint8_t* viewPortRenderBufferStart = (ptrViewPortRenderBufferStart + (startLine * pitch));
	result = viewPortHeight;
	v27 = -v10;
	uint16_t height = viewPortHeight;

	if (viewPortHeight)
	{
		do
		{
			int32_t v12 = (viewPortWidth
				- (__CFSHL__((signed int)viewPortWidth >> 31, 2)
					+ 4 * ((signed int)viewPortWidth >> 31))) >> 2;
			v13 = v19ar;
			v20 = v27 >> 16;
			int32_t* viewPortLineRenderBufferStart = (int32_t*)viewPortRenderBufferStart;
			v21 = v23 >> 16;
			BYTE1(v17) = BYTE2(v27);
			int32_t v16 = (viewPortWidth
				- (__CFSHL__((signed int)viewPortWidth >> 31, 2)
					+ 4 * ((signed int)viewPortWidth >> 31))) >> 2;
			LOBYTE(v17) = BYTE2(v23);
			v17 = (unsigned __int16)v17;

			//Scales sky texture to viewport
			do
			{
				LOBYTE(v12) = off_D41A8_sky[v17];
				LOBYTE(v17) = *v13 + v17;
				BYTE1(v17) += v13[1];
				BYTE1(v12) = off_D41A8_sky[v17];
				LOBYTE(v17) = v13[2] + v17;
				BYTE1(v17) += v13[3];
				v18 = v12 << 16;
				LOBYTE(v18) = off_D41A8_sky[v17];
				LOBYTE(v17) = v13[4] + v17;
				BYTE1(v17) += v13[5];
				BYTE1(v18) = off_D41A8_sky[v17];
				LOBYTE(v17) = v13[6] + v17;
				BYTE1(v17) += v13[7];
				v12 = __ROL4__(v18, 16);
				*viewPortLineRenderBufferStart = v12;
				viewPortLineRenderBufferStart++;
				v13 += 8;
				v16--;
			} while (v16);

			viewPortRenderBufferStart = viewPortRenderBufferStart + (pitch * drawEveryNthLine);
			result = v25;
			height = Maths::SubtrackUntilZero(height, drawEveryNthLine);
			v23 -= (v26 * drawEveryNthLine);
			v27 += (v25 * drawEveryNthLine);
		} while (height);
	}
}

void GameRender::DrawTerrainAndParticles_3C080(__int16 posX, __int16 posY, __int16 yaw, signed int posZ, int pitch, int16_t roll, int fov, type_particle_str** str_DWORD_F66F0x[], uint8_t x_BYTE_E88E0x[], int32_t x_DWORD_F5730[], uint8_t unk_F0A20x[], type_event_0x6E8E* x_DWORD_EA3E4[], type_str_unk_1804B0ar str_unk_1804B0ar, int16_t x_WORD_D4B7C, char isCaveLevel, ViewPort viewPort, uint16_t screenWidth)
{
	int v9; // eax
	int v10; // edx
	int v11; // ecx
	int v12; // edx
	int v13; // edi
	char v14; // dh
	int v15x;
	char v16; // dl
	char v17; // dl
	int v18x;
	char v19; // dh
	int v20; // ebx
	int v21; // ecx
	char v22; // ch
	int v23; // eax
	uint8_t* v25x; // edi
	int v25z;
	unsigned __int16 v26; // dx
	int v27; // ebx
	int v28; // eax
	__int16 v29; // si
	int v30; // edx
	__int16 v31; // cx
	int v32; // eax
	int v33; // ecx
	signed int v34; // esi
	int v35; // ebx
	unsigned __int16 v36; // dx
	int v37; // eax
	__int16 v38; // ax
	int v39; // eax
	int v40; // edi
	int v41x; // edx
	unsigned __int16 v42; // bx
	int v43x;
	uint8_t* v44; // eax
	char v45; // bh
	signed int v46; // edx
	int v47x;
	int v48; // esi
	int v49; // ecx
	int v50; // ecx
	int v51; // ebx
	int v52; // ecx
	int v53; // ebx
	signed int v54; // esi
	signed int v55; // esi
	int v56x;
	int v57x;
	char v58; // ah
	int jx;
	char v60; // dl
	char v61; // dh
	char v62; // ch
	char v63; // ah
	char v64; // dl
	char v65; // dh
	char v66; // ch
	char v67; // dl
	int v68x;
	int v69; // eax
	char v71; // dl
	char v72; // dh
	char v73; // ch
	char v74; // ah
	char v75; // dl
	char v76; // dh
	char v77; // ch
	char v78; // dl
	char v79; // dh
	int v80x;
	int v82x;
	int v83x;
	char v84; // dl
	char v85; // cl
	char v86; // dh
	char v87; // al
	char v88; // dl
	char v89; // dh
	int v90; // eax
	int v91x; // ebx
	char v92; // cl
	char v93; // dl
	int v94x;
	char v96; // al
	char v97; // dl
	char v98; // dh
	char v99; // ah
	char v100; // dl
	char v101; // dh
	char v102; // ch
	int v103; // eax
	int v104x;
	char v105; // dl
	char v106; // dh
	int v107x;
	signed int v109; // esi
	int v110; // ebx
	unsigned __int16 v111; // dx
	__int16 v112; // ax
	int v113; // eax
	int v114x;
	signed int v115; // edx
	int v116; // eax
	int v117x;
	unsigned __int16 v118; // bx
	unsigned __int8 v119; // al
	int v120x;
	uint8_t* v121; // eax
	char v122; // bh
	signed int v123; // ebx
	int v124x;
	int v125; // esi
	int v126; // ecx
	int v127; // ecx
	int v128; // edx
	int v129; // ecx
	int v130; // edx
	signed int v131; // esi
	signed int v132; // esi
	int v133x;
	int v134x;
	int v135; // eax
	char v136; // dl
	char v137; // ch
	char v138; // dl
	int v139; // eax
	int v140x;
	int v141; // eax
	char v142; // ch
	int v143x;
	char v144; // dl
	int v145; // eax
	int v147x;
	char v148; // dl
	char v149; // dl
	int v150; // eax
	int v151x;
	int v152; // eax
	char v153; // cl
	int v154; // eax
	int v155x;
	char v156; // dl
	int v157; // eax
	int v159; // eax
	int v160;
	int v161;
	int v162; // eax
	char v163; // dl
	char v164; // dh
	char v165; // ah
	char v166; // dl
	char v167; // dh
	int v168; // eax
	int v169x;
	char v170; // ch
	int v171; // eax
	int v172x;
	char v173; // dl
	char v174; // dh
	int v177x;
	int v178x;
	char v179; // dl
	char v180; // ch
	char v181; // dh
	char v182; // ah
	char v183; // dl
	char v184; // dh
	int v185; // eax
	int v186x;
	int v187; // eax
	int v188; // eax
	char v189; // ch
	int v190x;
	char v191; // dl
	char v192; // dh
	char v194; // ch
	char v196; // ch
	int v197; // ecx
	signed int v198; // esi
	int v199; // ebx
	unsigned __int16 v200; // di
	__int16 v201; // ax
	int v202; // eax
	int v203; // eax
	unsigned __int16 v204; // bx
	int v205x;
	int v206x;
	uint8_t* v207; // eax
	int v208; // eax
	signed int v209; // ebx
	int v210; // edx
	uint32_t v211; // eax
	int v212; // ecx
	int v213; // edx
	int v214; // ecx
	int v215; // edx
	signed int v216; // esi
	int v217x, v218x;
	char v219; // dl
	char v220; // dh
	char v221; // al
	char v222; // dl
	char v223; // dh
	int v224; // eax
	int v225x;
	char v226; // cl
	int v227x;
	char v228; // dl
	char v229; // dh
	int v231x, v232x;
	char v233; // dl
	char v234; // ch
	char v235; // dh
	char v236; // ah
	char v237; // dl
	char v238; // dh
	int v239; // eax
	int v240x;
	int v241; // eax
	char v242; // ch
	int v243x;
	char v244; // dl
	char v245; // dh
	int v248x[33]; // [esp+0h] [ebp-62h]//v248x[0]
	uint8_t* v277; // [esp+84h] [ebp+22h]
	uint8_t* v278;
	int v278x;
	unsigned __int16 v279; // [esp+8Ch] [ebp+2Ah]
	char l; // [esp+90h] [ebp+2Eh]
	char v281; // [esp+94h] [ebp+32h]
	char v282; // [esp+98h] [ebp+36h]
	char v283; // [esp+9Ch] [ebp+3Ah]
	char k; // [esp+A0h] [ebp+3Eh]
	char v285; // [esp+A4h] [ebp+42h]
	char i; // [esp+A8h] [ebp+46h]
	char jj; // [esp+ACh] [ebp+4Ah]
	char m; // [esp+B0h] [ebp+4Eh]
	char v289; // [esp+B4h] [ebp+52h]
	char n; // [esp+B8h] [ebp+56h]
	char ii; // [esp+BCh] [ebp+5Ah]
	char kk; // [esp+C0h] [ebp+5Eh]
	char v293; // [esp+C4h] [ebp+62h]

	int a1;
	int a2;

	x_BYTE_F2CC7 = D41A0_0.m_GameSettings.m_Graphics.m_wShadows;//21d080
	x_BYTE_D4320 = D41A0_0.terrain_2FECE.MapType != MapType_t::Day;
	m_str_F2C20ar.dword0x10 = (signed int)(unsigned __int16)viewPort.Height >> 1;
	x_WORD_F2CC4 = posX;
	x_WORD_F2CC0 = yaw & 0x7FF;
	x_WORD_F2CC2 = posY;
	v9 = (yaw & 0x7FF) + 256;
	m_str_F2C20ar.dword0x20 = posZ;
	v10 = Maths::x_DWORD_DB750[0x100 + v9];
	m_str_F2C20ar.dword0x24 = x_DWORD_D4324 + ((signed int)(unsigned __int16)viewPort.Width >> 1);
	m_str_F2C20ar.dword0x0f = v10;
	v11 = *x_DWORD_DB350_ret(v9);
	v12 = ((((yaw & 0x7FF) + 256) & 0x1FF) - 256) & 0x7FF;
	v248x[32] = (v9 >> 9) & 3;
	v248x[30] = Maths::x_DWORD_DB750[0x200 + v12];
	m_str_F2C20ar.dword0x17 = v11;
	v13 = Maths::x_DWORD_DB750[v12];
	SetBillboards_3B560(-roll & 0x7FF, unk_F0A20x, viewPort.Width, viewPort.Height, screenWidth);//21d1aa
	m_str_F2C20ar.dword0x18 = 7
		* Maths::sub_7277A_radix_3d(
			(unsigned __int16)viewPort.Width * (unsigned __int16)viewPort.Width
			+ (unsigned __int16)viewPort.Height * (unsigned __int16)viewPort.Height)
		* fov >> 11;
	v277 = unk_D4328x + 10 * v248x[32];
	switch ((unsigned __int8)v248x[32])//fixed? //rotations
	{
	case 0u:
		a2 = (unsigned __int8)posY - 256;
		a1 = -(unsigned __int8)posX - 4864;
		break;
	case 1u:
		a1 = -(unsigned __int8)posY - 4864;
		a2 = -(unsigned __int8)posX;
		break;
	case 2u:
		a1 = (unsigned __int8)posX - 4864;
		a2 = -(unsigned __int8)posY;
		break;
	case 3u:
		a1 = (unsigned __int8)posY - 4864;
		a2 = (unsigned __int8)posX - 256;
		break;
	default:
		break;
	}
	v14 = 40;//21d231
	v15x = 0;
	do//filling first pointer of x_DWORD_E9C38_smalltit(3f52a4)//prepare billboards
	{
		v248x[29] = a1 * v13 >> 16;
		v16 = 21;
		v248x[28] = a1 * (signed int)v248x[30] >> 16;
		while (v16)
		{
			m_Str_E9C38_smalltit[v15x].dword0_rot = v248x[28];
			m_Str_E9C38_smalltit[v15x].dword12 = v248x[29];
			if (a1 < 0)
			{
				m_Str_E9C38_smalltit[v15x].word38 = 0;
			}
			else
			{
				m_Str_E9C38_smalltit[v15x].word38 = 4;
			}
			v15x += 40;
			v16--;
		}
		v15x -= 839;
		a1 += 256;
		v14--;
	} while (v14);

	v17 = 21;//21d29c not drawing
	v18x = 0;
	while (v17)
	{
		v248x[27] = a2 * v13 >> 16;
		v19 = 40;
		v20 = a2 * (signed int)v248x[30] >> 16;
		while (v19)
		{
			m_Str_E9C38_smalltit[v18x].dword0_rot -= v248x[27];
			m_Str_E9C38_smalltit[v18x].dword12 += v20;// +v21;
			v18x++;
			v19--;
		}
		a2 += 256;
		v17--;
	}
	m_str_F2C20ar.dword0x15 = 26214400;//21d2df not drawing
	v278x = 0;
	m_str_F2C20ar.dword0x12 = 8912896;
	v22 = *v277;
	m_str_F2C20ar.dword0x22 = pitch * (unsigned __int16)viewPort.Width >> 8;
	LOBYTE(v279) = v22 + HIBYTE(posX);
	HIBYTE(v279) = v277[1] + HIBYTE(posY);
	v23 = roll & 0x7FF;
	m_str_F2C20ar.dword0x11 = (int)Maths::x_DWORD_DB750[0x200 + v23];
	m_str_F2C20ar.dword0x16 = 23658496;
	m_str_F2C20ar.dword0x0d = Maths::x_DWORD_DB750[v23];
	m_str_F2C20ar.dword0x13 = 14745600;
	if (!D41A0_0.m_GameSettings.m_Graphics.m_wSky || isCaveLevel)
	{
		v26 = viewPort.Width;
		v27 = screenWidth - viewPort.Width;
		v28 = (v26 - (__CFSHL__((signed int)v26 >> 31, 2) + 4 * ((signed int)v26 >> 31))) >> 2;
		v29 = viewPort.Height;
		v25x = m_ptrViewPortRenderBufferStart;
		v30 = (v26 - (__CFSHL__((signed int)v26 >> 31, 2) + 4 * ((signed int)v26 >> 31))) >> 2;
		LOBYTE(v28) = x_WORD_D4B7C;
		HIBYTE(v28) = x_WORD_D4B7C;
		v31 = v28;
		v32 = v28 << 16;
		LOWORD(v32) = v31;
		do
		{
			memset32(v25x, v32, v30 * 4);
			v25x += 4 * v30 + v27;
			v29--;
		} while (v29);
	}
	else
	{
		if (m_renderThreads.size() > 0)
		{
			DrawSky_40950_TH(roll, m_ptrViewPortRenderBufferStart, viewPort.Width, viewPort.Height, screenWidth);
		}
		else
		{
			DrawSky_40950(roll, m_ptrViewPortRenderBufferStart, viewPort.Width, viewPort.Height, screenWidth, 0, 1);
		}
	}
	if (isCaveLevel)//21d3e3 cleaned screen
	{
		//Cave Level Render
		for (i = 21; ; i--)
		{
			if (!i)
			{
				v46 = 840;
				v47x = 0;
				while (v46)
				{
					v48 = ((m_Str_E9C38_smalltit[v47x].dword16 * m_str_F2C20ar.dword0x11 - m_str_F2C20ar.dword0x0d * m_Str_E9C38_smalltit[v47x].dword20) >> 16) + m_str_F2C20ar.dword0x24;
					v248x[25] = ((m_Str_E9C38_smalltit[v47x].dword16 * m_str_F2C20ar.dword0x11 - m_str_F2C20ar.dword0x0d * m_Str_E9C38_smalltit[v47x].dword28) >> 16) + m_str_F2C20ar.dword0x24;
					v49 = m_Str_E9C38_smalltit[v47x].dword16 * m_str_F2C20ar.dword0x0d;
					v248x[24] = m_str_F2C20ar.dword0x10 - ((v49 + m_str_F2C20ar.dword0x11 * m_Str_E9C38_smalltit[v47x].dword20) >> 16);
					v50 = m_str_F2C20ar.dword0x10 - ((v49 + m_str_F2C20ar.dword0x11 * m_Str_E9C38_smalltit[v47x].dword28) >> 16);
					m_Str_E9C38_smalltit[v47x].dword16 = v48;
					v51 = v50;
					v52 = v248x[24];
					m_Str_E9C38_smalltit[v47x].dword28 = v51;
					m_Str_E9C38_smalltit[v47x].dword20 = v52;
					v53 = m_Str_E9C38_smalltit[v47x].dword16;
					m_Str_E9C38_smalltit[v47x].dword24 = v248x[25];
					if (v53 >= 0)
					{
						if ((signed int)(unsigned __int16)viewPort.Width <= m_Str_E9C38_smalltit[v47x].dword16)
							m_Str_E9C38_smalltit[v47x].word38 |= 0x10u;
					}
					else
					{
						m_Str_E9C38_smalltit[v47x].word38 |= 8u;
					}
					v54 = m_Str_E9C38_smalltit[v47x].dword20;
					if (v54 >= 0)
					{
						if ((unsigned __int16)viewPort.Height <= v54)
							m_Str_E9C38_smalltit[v47x].word38 |= 0x40u;
					}
					else
					{
						m_Str_E9C38_smalltit[v47x].word38 |= 0x20u;
					}
					if (m_Str_E9C38_smalltit[v47x].dword24 >= 0)
					{
						if ((signed int)(unsigned __int16)viewPort.Width <= m_Str_E9C38_smalltit[v47x].dword24)
							m_Str_E9C38_smalltit[v47x].word39 |= 0x200u;
					}
					else
					{
						m_Str_E9C38_smalltit[v47x].word39 |= 0x100u;
					}
					v55 = m_Str_E9C38_smalltit[v47x].dword28;
					if (v55 >= 0)
					{
						if ((unsigned __int16)viewPort.Height <= v55)
							m_Str_E9C38_smalltit[v47x].word39 |= 0x800u;
					}
					else
					{
						m_Str_E9C38_smalltit[v47x].word39 |= 0x400u;
					}
					v47x++;
					v46--;
				}
				v57x = 800;
				v281 = 20;
				do
				{
					v58 = 39;
					for (jx = v57x; ; jx = (v80x + 1))
					{
						v293 = v58;
						if (!v58)
							break;
						v248x[18] = m_Str_E9C38_smalltit[jx].dword24;
						v248x[19] = m_Str_E9C38_smalltit[jx].dword28;
						v248x[22] = m_Str_E9C38_smalltit[jx].dword32;
						jx++;
						v60 = m_Str_E9C38_smalltit[jx - 1].word38;
						v61 = m_Str_E9C38_smalltit[jx - 1].word38;
						if (m_Str_E9C38_smalltit[jx].word38 & 4)
							break;
						v248x[12] = m_Str_E9C38_smalltit[jx].dword24;
						v248x[13] = m_Str_E9C38_smalltit[jx].dword28;
						v248x[16] = m_Str_E9C38_smalltit[jx].dword32;
						v62 = m_Str_E9C38_smalltit[jx].word38;
						v248x[6] = m_Str_E9C38_smalltit[jx - 40].dword24;
						v248x[7] = m_Str_E9C38_smalltit[jx - 40].dword28;
						v248x[10] = m_Str_E9C38_smalltit[jx - 40].dword32;

						v63 = m_Str_E9C38_smalltit[jx - 40].word38;
						v64 = v63 | v62 | v60;
						v65 = v63 & v62 & v61;
						v248x[0] = m_Str_E9C38_smalltit[jx - 41].dword24;
						v248x[1] = m_Str_E9C38_smalltit[jx - 41].dword28;
						v248x[4] = m_Str_E9C38_smalltit[jx - 41].dword32;
						v66 = m_Str_E9C38_smalltit[jx - 41].word38;
						v67 = v66 | v64;
						v68x = jx - 1;
						if ((v66 & v65 & 0x80u) == 0)
						{
							if (m_Str_E9C38_smalltit[v68x].word39 & 0x1000)
							{
								x_BYTE_E126D = 7;
								x_BYTE_E126C = ((signed int)v248x[10] + v248x[16] + v248x[22] + v248x[4]) >> 18;
							}
							else
							{
								x_BYTE_E126D = 5;
							}
							if (!(v67 & 2))
							{
								v69 = 0;
								if (!(v69 & 0xF00))
								{
									DrawInverseSquareInProjectionSpace(v248x, v68x, m_textureAddresses[1], viewPort.Width, viewPort.Height, screenWidth);
								}
							}
						}
						v248x[18] = m_Str_E9C38_smalltit[v68x].dword16;
						v248x[19] = m_Str_E9C38_smalltit[v68x].dword20;
						v248x[22] = m_Str_E9C38_smalltit[v68x].dword32;
						jx = v68x + 1;
						v71 = m_Str_E9C38_smalltit[jx - 1].word38;
						v72 = m_Str_E9C38_smalltit[jx - 1].word38;
						if (m_Str_E9C38_smalltit[jx].word38 & 4)
							break;
						v248x[12] = m_Str_E9C38_smalltit[jx].dword16;
						v248x[13] = m_Str_E9C38_smalltit[jx].dword20;
						v248x[16] = m_Str_E9C38_smalltit[jx].dword32;
						v73 = m_Str_E9C38_smalltit[jx].word38;
						v248x[6] = m_Str_E9C38_smalltit[jx - 40].dword16;
						v248x[7] = m_Str_E9C38_smalltit[jx - 40].dword20;
						v248x[10] = m_Str_E9C38_smalltit[jx - 40].dword32;
						v74 = m_Str_E9C38_smalltit[jx - 40].word38;
						v75 = v74 | v73 | v71;
						v76 = v74 & v73 & v72;
						v248x[0] = m_Str_E9C38_smalltit[jx - 41].dword16;
						v248x[1] = m_Str_E9C38_smalltit[jx - 41].dword20;
						v248x[4] = m_Str_E9C38_smalltit[jx - 41].dword32;
						v77 = m_Str_E9C38_smalltit[jx - 41].word38;
						v78 = v77 | v75;
						v79 = v77 & v76;
						v80x = jx - 1;
						if (v79 >= 0)
						{
							if (m_Str_E9C38_smalltit[v80x].word39 & 0x1000)
							{
								x_BYTE_E126D = 7;
								x_BYTE_E126C = ((signed int)v248x[10] + v248x[16] + v248x[22] + v248x[4]) >> 18;
							}
							else
							{
								x_BYTE_E126D = 5;
							}
							if (!(v78 & 2) && !(v79 & 0x78))
							{
								DrawSquareInProjectionSpace(v248x, v80x, viewPort.Width, viewPort.Height, screenWidth);
							}
							if (m_Str_E9C38_smalltit[v80x].word36)
								DrawParticles_3E360(v80x, str_DWORD_F66F0x, x_BYTE_E88E0x, x_DWORD_F5730, x_DWORD_EA3E4, str_unk_1804B0ar, viewPort, screenWidth);
						}
						v58 = v293 - 1;
					}
					if (v293)
					{
						v82x = jx - 1;
						v83x = v57x + 38;
						do
						{
							v248x[18] = m_Str_E9C38_smalltit[v83x].dword24;
							v248x[19] = m_Str_E9C38_smalltit[v83x].dword28;
							v248x[22] = m_Str_E9C38_smalltit[v83x].dword32;
							v84 = m_Str_E9C38_smalltit[v83x].word38;
							v248x[12] = m_Str_E9C38_smalltit[v83x + 1].dword24;
							v248x[13] = m_Str_E9C38_smalltit[v83x + 1].dword28;
							v248x[16] = m_Str_E9C38_smalltit[v83x + 1].dword32;
							v85 = m_Str_E9C38_smalltit[v83x + 1].word38;
							v248x[6] = m_Str_E9C38_smalltit[v83x - 39].dword24;
							v248x[7] = m_Str_E9C38_smalltit[v83x - 39].dword28;
							v86 = v84;
							v248x[10] = m_Str_E9C38_smalltit[v83x - 39].dword32;
							v87 = m_Str_E9C38_smalltit[v83x - 39].word38;
							v88 = v87 | v85 | v84;
							v89 = v87 & v85 & v86;
							v248x[0] = m_Str_E9C38_smalltit[v83x - 40].dword24;
							v248x[1] = m_Str_E9C38_smalltit[v83x - 40].dword28;
							v90 = m_Str_E9C38_smalltit[v83x - 40].dword32;
							v91x = v83x + 1;
							v248x[4] = v90;
							v92 = m_Str_E9C38_smalltit[v83x - 41].word38;
							v93 = v92 | v88;
							v94x = v91x - 1;
							if ((v92 & v89 & 0x80u) == 0)
							{
								if (m_Str_E9C38_smalltit[v94x].word39 & 0x1000)
								{
									x_BYTE_E126D = 7;
									x_BYTE_E126C = ((signed int)v248x[10] + v248x[16] + v248x[22] + v248x[4]) >> 18;
								}
								else
								{
									x_BYTE_E126D = 5;
								}
								if (!(v93 & 2))
								{

									DrawInverseSquareInProjectionSpace(v248x, v94x, m_textureAddresses[1], viewPort.Width, viewPort.Height, screenWidth);
								}
							}
							v248x[18] = m_Str_E9C38_smalltit[v94x].dword16;
							v248x[19] = m_Str_E9C38_smalltit[v94x].dword20;
							v248x[22] = m_Str_E9C38_smalltit[v94x].dword32;
							v97 = m_Str_E9C38_smalltit[v94x].word38;
							v248x[12] = m_Str_E9C38_smalltit[v94x + 1].dword16;
							v248x[13] = m_Str_E9C38_smalltit[v94x + 1].dword20;
							v248x[16] = m_Str_E9C38_smalltit[v94x + 1].dword32;
							v98 = v97;
							v99 = m_Str_E9C38_smalltit[v94x + 1].word38;
							v100 = v99 | v97;
							v101 = v99 & v98;
							v248x[6] = m_Str_E9C38_smalltit[v94x - 39].dword16;
							v248x[7] = m_Str_E9C38_smalltit[v94x - 39].dword20;
							v248x[10] = m_Str_E9C38_smalltit[v94x - 39].dword32;
							v102 = m_Str_E9C38_smalltit[v94x - 39].word38;

							v248x[0] = m_Str_E9C38_smalltit[v94x - 40].dword16;
							v103 = m_Str_E9C38_smalltit[v94x - 40].dword20;
							v104x = v94x + 1;
							v248x[1] = v103;
							v248x[4] = m_Str_E9C38_smalltit[v104x - 41].dword32;
							v105 = (m_Str_E9C38_smalltit[v104x - 41].word38 & 0xff) | v102 | v100;
							v106 = (m_Str_E9C38_smalltit[v104x - 41].word38 & 0xff) & v102 & v101;
							v107x = v104x - 1;
							if (v106 >= 0)
							{
								if (m_Str_E9C38_smalltit[v107x].word39 & 0x1000)
								{
									x_BYTE_E126D = 7;
									x_BYTE_E126C = ((signed int)v248x[10] + v248x[16] + v248x[22] + v248x[4]) >> 18;
								}
								else
								{
									x_BYTE_E126D = 5;
								}
								if (!(v105 & 2) && !(v106 & 0x78))
								{
									DrawSquareInProjectionSpace(v248x, v107x, viewPort.Width, viewPort.Height, screenWidth);
								}
								if (m_Str_E9C38_smalltit[v107x].word36)
									DrawParticles_3E360(v107x, str_DWORD_F66F0x, x_BYTE_E88E0x, x_DWORD_F5730, x_DWORD_EA3E4, str_unk_1804B0ar, viewPort, screenWidth);
							}
							v83x = v107x - 1;
						} while (v83x >= v82x);
					}
					v57x -= 40;
					v281--;
				} while (v281);
				return;
			}
			for (k = 40; k; k--)
			{
				v33 = ((unsigned __int8)x_BYTE_12B4E0_shading[v279] << 8) + 128;
				v34 = m_Str_E9C38_smalltit[v278x].dword12;
				v35 = v34 * v34 + m_Str_E9C38_smalltit[v278x].dword0_rot * m_Str_E9C38_smalltit[v278x].dword0_rot;
				m_Str_E9C38_smalltit[v278x].word36 = 0;
				if (v34 <= -256 || v35 >= m_str_F2C20ar.dword0x15)
				{
					m_Str_E9C38_smalltit[v278x].word38 |= 2u;
					goto LABEL_46;
				}
				if (v34 < 128)
					v34 = 128;
				m_Str_E9C38_smalltit[v278x].dword16 = m_str_F2C20ar.dword0x18 * m_Str_E9C38_smalltit[v278x].dword0_rot / v34;
				v36 = v279;
				m_Str_E9C38_smalltit[v278x].dword4_height = 32 * x_BYTE_11B4E0_heightmap[v279] - posZ;
				m_Str_E9C38_smalltit[v278x].dword8 = ((unsigned __int8)x_BYTE_14B4E0_second_heightmap[v36] << 15 >> 10) - posZ;
				v37 = 0;
				if (!x_BYTE_10B4E0_terraintype[v36])
				{
					v38 = 32 * D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dword_0x012_2BE0_11248;
					v37 = (Maths::x_DWORD_DB750[(v38 + (HIBYTE(v279) << 7)) & 0x7FF] >> 8)
						* (Maths::x_DWORD_DB750[(((unsigned __int8)v279 << 7) + v38) & 0x7FF] >> 8);
					m_Str_E9C38_smalltit[v278x].dword4_height -= v37 >> 13;
					if (v33 >= 14464)
						v37 = 0;
				}
				v39 = (v33 << 8) + 8 * v37;
				if (v35 <= m_str_F2C20ar.dword0x13)
					goto LABEL_39;
				if (v35 < m_str_F2C20ar.dword0x16)
				{
					v39 = v39 * (signed __int64)(m_str_F2C20ar.dword0x16 - v35) / m_str_F2C20ar.dword0x12;
				LABEL_39:
					m_Str_E9C38_smalltit[v278x].dword32 = v39;
					goto LABEL_40;
				}
				m_Str_E9C38_smalltit[v278x].dword32 = 0;
			LABEL_40:
				if (x_BYTE_13B4E0_angle[v279] & 8)
					m_Str_E9C38_smalltit[v278x].word38 |= 0x80u;
				v40 = m_str_F2C20ar.dword0x18;
				v41x = v278x;
				m_Str_E9C38_smalltit[v278x].dword20 = m_str_F2C20ar.dword0x22 + m_str_F2C20ar.dword0x18 * m_Str_E9C38_smalltit[v278x].dword4_height / v34;
				m_Str_E9C38_smalltit[v278x].dword28 = m_str_F2C20ar.dword0x22 + v40 * m_Str_E9C38_smalltit[v278x].dword8 / v34;
				LOBYTE(v42) = v277[2] + v279;
				HIBYTE(v42) = v277[3] + HIBYTE(v279);
				v43x = v278x;
				m_Str_E9C38_smalltit[v278x].byte41 = x_BYTE_10B4E0_terraintype[v42];
				if (D41A0_0.m_GameSettings.str_0x2196.flat_0x2199)
					m_Str_E9C38_smalltit[v43x].word39 |= 0x1000u;
				m_Str_E9C38_smalltit[v278x].byte43 = Maths::x_BYTE_D41D8[m_Str_E9C38_smalltit[v278x].byte41];
				m_Str_E9C38_smalltit[v278x].byte42_std = v248x[32] + (((signed int)(unsigned __int8)x_BYTE_13B4E0_angle[v42] >> 2) & 0x1C);
				LOBYTE(v42) = v277[4] + v42;
				HIBYTE(v42) += v277[5];
				m_Str_E9C38_smalltit[v278x].word36 = mapEntityIndex_15B4E0[v42];
			LABEL_46:
				v44 = v277;
				m_Str_E9C38_smalltit[v278x].word38 |= ((x_BYTE)v279 + HIBYTE(v279)) & 1;
				LOBYTE(v279) = v44[8] + v279;
				HIBYTE(v279) += v277[9];
				v278x++;
			}
			v45 = v277[6] + v279;
			HIBYTE(v279) += v277[7];
			LOBYTE(v279) = v45;
		}
	}
	if (D41A0_0.m_GameSettings.m_Graphics.m_wReflections)
	{
		for (l = 21; ; l--)
		{
			if (!l)
			{
				v123 = 840;
				v124x = 0;
				while (v123)
				{
					v125 = ((m_str_F2C20ar.dword0x11 * m_Str_E9C38_smalltit[v124x].dword16 - m_str_F2C20ar.dword0x0d * m_Str_E9C38_smalltit[v124x].dword20) >> 16) + m_str_F2C20ar.dword0x24;
					v248x[25] = ((m_str_F2C20ar.dword0x11 * m_Str_E9C38_smalltit[v124x].dword16 - m_str_F2C20ar.dword0x0d * m_Str_E9C38_smalltit[v124x].dword28) >> 16) + m_str_F2C20ar.dword0x24;
					v126 = m_Str_E9C38_smalltit[v124x].dword16 * m_str_F2C20ar.dword0x0d;
					v248x[24] = m_str_F2C20ar.dword0x10 - ((v126 + m_str_F2C20ar.dword0x11 * m_Str_E9C38_smalltit[v124x].dword20) >> 16);
					v127 = m_str_F2C20ar.dword0x10 - ((v126 + m_str_F2C20ar.dword0x11 * m_Str_E9C38_smalltit[v124x].dword28) >> 16);
					m_Str_E9C38_smalltit[v124x].dword16 = v125;
					v128 = v127;
					v129 = v248x[24];
					m_Str_E9C38_smalltit[v124x].dword28 = v128;
					m_Str_E9C38_smalltit[v124x].dword20 = v129;
					v130 = m_Str_E9C38_smalltit[v124x].dword16;
					m_Str_E9C38_smalltit[v124x].dword24 = v248x[25];
					if (v130 >= 0)
					{
						if ((signed int)(unsigned __int16)viewPort.Width <= m_Str_E9C38_smalltit[v124x].dword16)
							m_Str_E9C38_smalltit[v124x].word38 |= 0x10u;
					}
					else
					{
						m_Str_E9C38_smalltit[v124x].word38 |= 8u;
					}
					v131 = m_Str_E9C38_smalltit[v124x].dword20;
					if (v131 >= 0)
					{
						if ((unsigned __int16)viewPort.Height <= v131)
							m_Str_E9C38_smalltit[v124x].word38 |= 0x40u;
					}
					else
					{
						m_Str_E9C38_smalltit[v124x].word38 |= 0x20u;
					}
					if (m_Str_E9C38_smalltit[v124x].dword24 >= 0)
					{
						if ((signed int)(unsigned __int16)viewPort.Width <= m_Str_E9C38_smalltit[v124x].dword24)
							m_Str_E9C38_smalltit[v124x].word38 |= 0x200u;
					}
					else
					{
						m_Str_E9C38_smalltit[v124x].word38 |= 0x100u;
					}
					v132 = m_Str_E9C38_smalltit[v124x].dword28;
					if (v132 >= 0)
					{
						if ((unsigned __int16)viewPort.Height <= v132)
							m_Str_E9C38_smalltit[v124x].word38 |= 0x800u;
					}
					else
					{
						m_Str_E9C38_smalltit[v124x].word38 |= 0x400u;
					}
					v124x++;
					v123--;
				}
				if (posZ < 4096)
				{
					v133x = 800;
					for (m = 20; m; --m)
					{
						v134x = v133x;
						for (n = 39; n; --n)
						{
							v248x[18] = m_Str_E9C38_smalltit[v134x].dword24;
							v248x[19] = m_Str_E9C38_smalltit[v134x].dword28;
							v135 = m_Str_E9C38_smalltit[v134x].dword32;
							v134x++;
							v248x[22] = v135;
							v136 = m_Str_E9C38_smalltit[v134x - 1].word38;
							if (m_Str_E9C38_smalltit[v134x].word38 & 4)
								break;
							v248x[12] = m_Str_E9C38_smalltit[v134x].dword24;
							v248x[13] = m_Str_E9C38_smalltit[v134x].dword28;
							v248x[16] = m_Str_E9C38_smalltit[v134x].dword32;
							v137 = m_Str_E9C38_smalltit[v134x].word38;
							v248x[6] = m_Str_E9C38_smalltit[v134x - 40].dword24;
							v248x[7] = m_Str_E9C38_smalltit[v134x - 40].dword28;
#ifdef TEST_x64
							allert_error();
#endif
#ifdef COMPILE_FOR_64BIT // FIXME: 64bit
							std::cout << "FIXME: 64bit @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
							v248x[10] = (int)*(x_DWORD**)&m_Str_E9C38_smalltit[v134x - 40].dword32;
#endif
							v138 = m_Str_E9C38_smalltit[v134x - 40].word38 | v137 | v136;
							v248x[0] = m_Str_E9C38_smalltit[v134x - 41].dword24;
							v139 = m_Str_E9C38_smalltit[v134x - 41].dword28;
							v140x = v134x - 40;
							v248x[1] = v139;
							v141 = m_Str_E9C38_smalltit[v140x].dword12;
							v140x--;
							v248x[4] = v141;
							v142 = m_Str_E9C38_smalltit[v140x].word38;
							v143x = v140x + 40;
							v144 = v142 | v138;
							if (m_Str_E9C38_smalltit[v143x].byte41)
							{
								if (m_Str_E9C38_smalltit[v143x].word38 & 0x1000)
								{
									x_BYTE_E126D = 7;
									x_BYTE_E126C = ((signed int)v248x[10] + v248x[16] + v248x[22] + v248x[4]) >> 18;
								}
								else
								{
									x_BYTE_E126D = 5;
								}
								if (!(v144 & 2))
								{
									v145 = 0;
									if (!(v145 & 0xF00))
									{
										DrawInverseSquareInProjectionSpace(v248x, v143x, viewPort.Width, viewPort.Height, screenWidth);
									}
								}
							}
							if (m_Str_E9C38_smalltit[v143x].word36)
								sub_3FD60(v143x, x_BYTE_E88E0x, x_DWORD_EA3E4, str_unk_1804B0ar, str_DWORD_F66F0x, x_DWORD_F5730, viewPort, screenWidth);
							v134x = v143x + 1;
						}
						if (n)
						{
							v25z = v134x - 1;
							v147x = v133x + 38;
							do
							{
								v248x[18] = m_Str_E9C38_smalltit[v147x].dword24;
								v248x[19] = m_Str_E9C38_smalltit[v147x].dword28;
								v248x[22] = m_Str_E9C38_smalltit[v147x].dword32;
								v148 = m_Str_E9C38_smalltit[v147x].word38;
								v248x[12] = m_Str_E9C38_smalltit[v147x + 1].dword24;
								v248x[13] = m_Str_E9C38_smalltit[v147x + 1].dword28;
								v248x[16] = m_Str_E9C38_smalltit[v147x + 1].dword32;
								v149 = m_Str_E9C38_smalltit[v147x + 1].word38 | v148;
								v248x[6] = m_Str_E9C38_smalltit[v147x - 39].dword24;
								v150 = m_Str_E9C38_smalltit[v147x - 39].dword28;
								v151x = v147x + 1;
								v248x[7] = v150;
								v152 = m_Str_E9C38_smalltit[v151x - 40].dword32;
								v151x -= 40;
#ifdef TEST_x64
								allert_error();
#endif
#ifdef COMPILE_FOR_64BIT // FIXME: 64bit
								std::cout << "FIXME: 64bit @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
								v248x[10] = (int)(x_DWORD*)v152;
#endif
								v153 = m_Str_E9C38_smalltit[v151x].word38;
								v248x[0] = m_Str_E9C38_smalltit[v151x - 1].dword24;
								v154 = m_Str_E9C38_smalltit[v151x - 1].dword28;
								v151x--;
								v248x[1] = v154;
								v248x[4] = m_Str_E9C38_smalltit[v151x].dword32;
								LOBYTE(v154) = m_Str_E9C38_smalltit[v151x].word38;
								v155x = v151x + 40;
								v156 = v154 | v153 | v149;
								if (m_Str_E9C38_smalltit[v155x].byte41)
								{
									if (m_Str_E9C38_smalltit[v155x].word38 & 0x1000)
									{
										x_BYTE_E126D = 7;
										x_BYTE_E126C = ((signed int)v248x[10] + v248x[16] + v248x[22] + v248x[4]) >> 18;
									}
									else
									{
										x_BYTE_E126D = 5;
									}
									if (!(v156 & 2))
									{
										v157 = 0;
										if (!(v157 & 0xF00))
										{
											DrawInverseSquareInProjectionSpace(v248x, v155x, viewPort.Width, viewPort.Height, screenWidth);
										}
									}
								}
								if (m_Str_E9C38_smalltit[v155x].word36)
									sub_3FD60(v155x, x_BYTE_E88E0x, x_DWORD_EA3E4, str_unk_1804B0ar, str_DWORD_F66F0x, x_DWORD_F5730, viewPort, screenWidth);
								v147x = v155x - 1;
							} while (v147x >= v25z);
						}
						v133x -= 40;
					}
				}
				v160 = 800;
				v282 = 20;
				do
				{
					v161 = v160;
					for (ii = 39; ii; --ii)
					{
						v248x[18] = m_Str_E9C38_smalltit[v161].dword16;
						v248x[19] = m_Str_E9C38_smalltit[v161].dword20;
						v162 = m_Str_E9C38_smalltit[v161].dword32;
						v161++;
						v248x[22] = v162;
						v163 = m_Str_E9C38_smalltit[v161 - 1].word38;
						v164 = m_Str_E9C38_smalltit[v161 - 1].word38;
						if (m_Str_E9C38_smalltit[v161].word38 & 4)
							break;
						v248x[12] = m_Str_E9C38_smalltit[v161].dword16;
						v248x[13] = m_Str_E9C38_smalltit[v161].dword20;
						v248x[16] = m_Str_E9C38_smalltit[v161].dword32;
						v165 = m_Str_E9C38_smalltit[v161].word38;
						v166 = v165 | v163;
						v167 = v165 & v164;
						v248x[6] = m_Str_E9C38_smalltit[v161 - 40].dword16;
						v248x[7] = m_Str_E9C38_smalltit[v161 - 40].dword20;
						v168 = m_Str_E9C38_smalltit[v161 - 40].dword32;
						v169x = v161 - 40;
#ifdef TEST_x64
						allert_error();
#endif
#ifdef COMPILE_FOR_64BIT // FIXME: 64bit
						std::cout << "FIXME: 64bit @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
						v248x[10] = (int)(x_DWORD*)v168;
#endif
						v170 = m_Str_E9C38_smalltit[v169x].word38;
						v248x[0] = m_Str_E9C38_smalltit[v169x - 1].dword16;
						v171 = m_Str_E9C38_smalltit[v169x - 1].dword20;
						v169x--;
						v248x[1] = v171;
						v248x[4] = m_Str_E9C38_smalltit[v169x].dword32;
						BYTE1(v171) = m_Str_E9C38_smalltit[v169x].word38;
						v172x = v169x + 40;
						v173 = BYTE1(v171) | v170 | v166;
						v174 = BYTE1(v171) & v170 & v167;
						if ((int8_t)(m_Str_E9C38_smalltit[v172x].word38 & 0xff) >= 0)
						{
							if (m_Str_E9C38_smalltit[v172x].word38 & 0x1000)
							{
								x_BYTE_E126D = 7;
								x_BYTE_E126C = ((signed int)v248x[10] + v248x[16] + v248x[22] + v248x[4]) >> 18;
							}
							else
							{
								x_BYTE_E126D = 5;
							}
							if (!(v173 & 2) && !(v174 & 0x78))
							{
								DrawSquareInProjectionSpace(v248x, v172x, viewPort.Width, viewPort.Height, screenWidth);
							}
						}
						else
						{
							x_BYTE_E126D = 26;
							if (!(v173 & 2) && !(v174 & 0x78))
							{
								DrawSquareInProjectionSpace(v248x, v172x, viewPort.Width, viewPort.Height, screenWidth);
							}
						}
						if (m_Str_E9C38_smalltit[v172x].word36)
							DrawParticles_3E360(v172x, str_DWORD_F66F0x, x_BYTE_E88E0x, x_DWORD_F5730, x_DWORD_EA3E4, str_unk_1804B0ar, viewPort, screenWidth);
						v161 = v172x + 1;
					}
					if (ii)
					{
						v177x = v161 - 1;
						v178x = v160 + 38;
						do
						{
							v248x[18] = m_Str_E9C38_smalltit[v178x].dword16;
							v248x[19] = m_Str_E9C38_smalltit[v178x].dword20;
							v248x[22] = m_Str_E9C38_smalltit[v178x].dword32;
							v179 = m_Str_E9C38_smalltit[v178x].word38;
							v248x[12] = m_Str_E9C38_smalltit[v178x + 1].dword16;
							v248x[13] = m_Str_E9C38_smalltit[v178x + 1].dword20;
							v248x[16] = m_Str_E9C38_smalltit[v178x + 1].dword32;
							v180 = m_Str_E9C38_smalltit[v178x + 1].word38;
							v248x[6] = m_Str_E9C38_smalltit[v178x - 39].dword16;
							v248x[7] = m_Str_E9C38_smalltit[v178x - 39].dword20;
							v181 = v179;
#ifdef TEST_x64
							allert_error();
#endif
#ifdef COMPILE_FOR_64BIT // FIXME: 64bit
							std::cout << "FIXME: 64bit @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
							v248x[10] = (int)*(x_DWORD**)&m_Str_E9C38_smalltit[v178x - 39].dword32;
#endif
							v182 = m_Str_E9C38_smalltit[v178x - 39].word38;
							v183 = v182 | v180 | v179;
							v184 = v182 & v180 & v181;
							v185 = m_Str_E9C38_smalltit[v178x - 40].dword16;
							v186x = v178x + 1;
							v248x[0] = v185;
							v187 = m_Str_E9C38_smalltit[v186x - 41].dword20;
							v186x -= 40;
							v248x[1] = v187;
							v188 = m_Str_E9C38_smalltit[v186x - 1].dword32;
							v186x--;
							v248x[4] = v188;
							v189 = m_Str_E9C38_smalltit[v186x].word38;
							v190x = v186x + 40;
							v191 = v189 | v183;
							v192 = v189 & v184;
							if ((int8_t)(m_Str_E9C38_smalltit[v190x].word38 & 0xff) >= 0)
							{
								if (m_Str_E9C38_smalltit[v190x].word38 & 0x1000)
								{
									x_BYTE_E126D = 7;
									x_BYTE_E126C = ((signed int)v248x[10] + v248x[16] + v248x[22] + v248x[4]) >> 18;
								}
								else
								{
									x_BYTE_E126D = 5;
								}
								if (!(v191 & 2) && !(v192 & 0x78))
								{
									DrawSquareInProjectionSpace(v248x, v190x, viewPort.Width, viewPort.Height, screenWidth);
								}
							}
							else
							{
								x_BYTE_E126D = 26;
								if (!(v191 & 2) && !(v192 & 0x78))
								{
									DrawSquareInProjectionSpace(v248x, v190x, viewPort.Width, viewPort.Height, screenWidth);
								}
							}
							if (m_Str_E9C38_smalltit[v190x].word36)
								DrawParticles_3E360(v190x, str_DWORD_F66F0x, x_BYTE_E88E0x, x_DWORD_F5730, x_DWORD_EA3E4, str_unk_1804B0ar, viewPort, screenWidth);
							v178x = v190x - 1;
						} while (v178x >= v177x);
					}
					v160 -= 40;
					v282--;
				} while (v282);
				return/* v56b*/;
			}
			for (jj = 40; jj; --jj)
			{
				v248x[31] = ((unsigned __int8)x_BYTE_12B4E0_shading[v279] << 8) + 128;
				v109 = m_Str_E9C38_smalltit[v278x].dword12;
				v110 = v109 * v109 + m_Str_E9C38_smalltit[v278x].dword0_rot * m_Str_E9C38_smalltit[v278x].dword0_rot;
				m_Str_E9C38_smalltit[v278x].word36 = 0;
				if (v109 <= -256 || v110 >= m_str_F2C20ar.dword0x15)
				{
					m_Str_E9C38_smalltit[v278x].word38 |= 2u;
					goto LABEL_140;
				}
				if (v109 < 128)
					v109 = 128;
				m_Str_E9C38_smalltit[v278x].dword16 = m_str_F2C20ar.dword0x18 * m_Str_E9C38_smalltit[v278x].dword0_rot / v109;
				v111 = v279;
				m_Str_E9C38_smalltit[v278x].dword4_height = 32 * x_BYTE_11B4E0_heightmap[v279] - posZ;
				v112 = (unsigned __int16)D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dword_0x012_2BE0_11248 << 6;
				v248x[26] = Maths::x_DWORD_DB750[(v112 + (HIBYTE(v279) << 7)) & 0x7FF] >> 8;
				v113 = v248x[26] * (Maths::x_DWORD_DB750[(((unsigned __int8)v279 << 7) + v112) & 0x7FF] >> 8);
				v248x[26] = x_BYTE_11B4E0_heightmap[v111];
				m_Str_E9C38_smalltit[v278x].dword8 = -(v248x[26] * ((v113 >> 4) + 0x8000) >> 10) - posZ;
				if (!(x_BYTE_13B4E0_angle[v111] & 8) || (m_Str_E9C38_smalltit[v278x].dword4_height -= v113 >> 10, v248x[31] >= 14464))
				{
					v113 = 0;
				}
				v116 = (v248x[31] << 8) + 8 * v113;
				if (v110 <= m_str_F2C20ar.dword0x13)
					goto LABEL_133;
				if (v110 < m_str_F2C20ar.dword0x16)
				{
					v116 = v116 * (signed __int64)(m_str_F2C20ar.dword0x16 - v110) / m_str_F2C20ar.dword0x12;
				LABEL_133:
					m_Str_E9C38_smalltit[v278x].dword32 = v116;
					goto LABEL_134;
				}
				m_Str_E9C38_smalltit[v278x].dword32 = 0;
			LABEL_134:
				v117x = v278x;
				m_Str_E9C38_smalltit[v278x].dword20 = m_str_F2C20ar.dword0x22 + m_str_F2C20ar.dword0x18 * m_Str_E9C38_smalltit[v278x].dword4_height / v109;
				m_Str_E9C38_smalltit[v278x].dword28 = m_str_F2C20ar.dword0x22 + m_str_F2C20ar.dword0x18 * m_Str_E9C38_smalltit[v117x].dword8 / v109;
				LOBYTE(v118) = v277[2] + v279;
				HIBYTE(v118) = v277[3] + HIBYTE(v279);
				v119 = x_BYTE_10B4E0_terraintype[v118];
				m_Str_E9C38_smalltit[v278x].byte41 = v119;
				if (Maths::x_BYTE_D41D8[0xa4 + v119])
					m_Str_E9C38_smalltit[v278x].word38 |= 0x80u;
				if (D41A0_0.m_GameSettings.str_0x2196.flat_0x2199)
					m_Str_E9C38_smalltit[v278x].word38 |= 0x1000u;
				v120x = v278x;
				m_Str_E9C38_smalltit[v278x].byte43 = Maths::x_BYTE_D41D8[m_Str_E9C38_smalltit[v278x].byte41];
				m_Str_E9C38_smalltit[v120x].byte42_std = v248x[32] + (((signed int)(unsigned __int8)x_BYTE_13B4E0_angle[v118] >> 2) & 0x1C);
				LOBYTE(v118) = v277[4] + v118;
				HIBYTE(v118) += v277[5];
				m_Str_E9C38_smalltit[v278x].word36 = mapEntityIndex_15B4E0[v118];
			LABEL_140:
				v121 = v277;
				m_Str_E9C38_smalltit[v278x].word38 |= ((x_BYTE)v279 + HIBYTE(v279)) & 1;
				LOBYTE(v279) = v121[8] + v279;
				HIBYTE(v279) += v277[9];
				v278x += 1;
			}
			v122 = v277[6] + v279;
			HIBYTE(v279) += v277[7];
			LOBYTE(v279) = v122;
		}
	}
	v283 = 21;//21eb44 nothing changed
LABEL_259:
	if (v283)
	{
		v285 = 40;
		while (1)
		{
			if (!v285)
			{
				LOBYTE(v279) = v277[6] + v279;
				HIBYTE(v279) += v277[7];
				v283--;
				goto LABEL_259;
			}
			v197 = ((unsigned __int8)x_BYTE_12B4E0_shading[v279] << 8) + 128;
			v198 = m_Str_E9C38_smalltit[v278x].dword12;
			v199 = v198 * v198 + m_Str_E9C38_smalltit[v278x].dword0_rot * m_Str_E9C38_smalltit[v278x].dword0_rot;
			m_Str_E9C38_smalltit[v278x].word36 = 0;
			if (v198 > -256 && v199 < m_str_F2C20ar.dword0x15)
				break;
			m_Str_E9C38_smalltit[v278x].word38 |= 2u;
		LABEL_256:
			v206x = v278x;
			v207 = v277;
			m_Str_E9C38_smalltit[v278x].word38 |= ((x_BYTE)v279 + HIBYTE(v279)) & 1;
			LOBYTE(v279) = v207[8] + v279;
			HIBYTE(v279) += v277[9];
			v285--;
			v278x = v206x + 1;
		}
		if (v198 < 128)
			v198 = 128;
		v200 = v279;
		m_Str_E9C38_smalltit[v278x].dword16 = m_str_F2C20ar.dword0x18 * m_Str_E9C38_smalltit[v278x].dword0_rot / v198;
		m_Str_E9C38_smalltit[v278x].dword4_height = 32 * x_BYTE_11B4E0_heightmap[v200] - posZ;
		v201 = (unsigned __int16)D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dword_0x012_2BE0_11248 << 6;
		v248x[26] = Maths::x_DWORD_DB750[(v201 + (HIBYTE(v279) << 7)) & 0x7FF] >> 8;
		v202 = v248x[26] * (Maths::x_DWORD_DB750[(((unsigned __int8)v279 << 7) + v201) & 0x7FF] >> 8);
		if (!(x_BYTE_13B4E0_angle[v200] & 8) || (m_Str_E9C38_smalltit[v278x].dword4_height -= v202 >> 10, v197 >= 14464))
			v202 = 0;
		v203 = (v197 << 8) + 8 * v202;
		if (v199 > m_str_F2C20ar.dword0x13)
		{
			if (v199 >= m_str_F2C20ar.dword0x16)
			{
				m_Str_E9C38_smalltit[v278x].dword32 = 0;
			LABEL_254:
				m_Str_E9C38_smalltit[v278x].dword20 = m_str_F2C20ar.dword0x22 + m_str_F2C20ar.dword0x18 * m_Str_E9C38_smalltit[v278x].dword4_height / v198;
				LOBYTE(v204) = v277[2] + v279;
				HIBYTE(v204) = v277[3] + HIBYTE(v279);
				v205x = v278x;
				m_Str_E9C38_smalltit[v278x].byte41 = x_BYTE_10B4E0_terraintype[v204];
				m_Str_E9C38_smalltit[v205x].byte43 = Maths::x_BYTE_D41D8[m_Str_E9C38_smalltit[v205x].byte41];
				m_Str_E9C38_smalltit[v205x].byte42_std = v248x[32] + (((signed int)(unsigned __int8)x_BYTE_13B4E0_angle[v204] >> 2) & 0x1C);
				LOBYTE(v204) = v277[4] + v204;
				HIBYTE(v204) += v277[5];
				m_Str_E9C38_smalltit[v278x].word36 = mapEntityIndex_15B4E0[v204];
				goto LABEL_256;
			}
			v203 = v203 * (signed __int64)(m_str_F2C20ar.dword0x16 - v199) / m_str_F2C20ar.dword0x12;
		}
		m_Str_E9C38_smalltit[v278x].dword32 = v203;
		goto LABEL_254;
	}//21edb7 nothing changed
	v208 = roll & 0x7FF;//21edb7
	v209 = 840;
	v210 = Maths::x_DWORD_DB750[v208];
	v211 = Maths::x_DWORD_DB750[0x200 + v208];
	m_str_F2C20ar.dword0x0d = v210;
	m_str_F2C20ar.dword0x11 = v211;
	v56x = 0;
	while (v209)
	{
		v212 = m_str_F2C20ar.dword0x10;
		v213 = (m_str_F2C20ar.dword0x0d * m_Str_E9C38_smalltit[v56x].dword16 + m_str_F2C20ar.dword0x11 * m_Str_E9C38_smalltit[v56x].dword20) >> 16;
		m_Str_E9C38_smalltit[v56x].dword16 = ((m_Str_E9C38_smalltit[v56x].dword16 * m_str_F2C20ar.dword0x11 - m_str_F2C20ar.dword0x0d * m_Str_E9C38_smalltit[v56x].dword20) >> 16)
			+ m_str_F2C20ar.dword0x24;
		v214 = v212 - v213;
		v215 = m_Str_E9C38_smalltit[v56x].dword16;
		m_Str_E9C38_smalltit[v56x].dword20 = v214;
		if (v215 >= 0)
		{
			if ((signed int)(unsigned __int16)viewPort.Width <= m_Str_E9C38_smalltit[v56x].dword16)
				m_Str_E9C38_smalltit[v56x].word38 |= 0x10u;
		}
		else
		{
			m_Str_E9C38_smalltit[v56x].word38 |= 8u;
		}
		v216 = m_Str_E9C38_smalltit[v56x].dword20;
		if (v216 >= 0)
		{
			if ((unsigned __int16)viewPort.Height <= v216)
				m_Str_E9C38_smalltit[v56x].word38 |= 0x40u;
		}
		else
		{
			m_Str_E9C38_smalltit[v56x].word38 |= 0x20u;
		}
		v56x++;
		v209--;
	}
	//adress 3de7d
	v217x = 800;
	v289 = 20;
	do
	{
		v218x = v217x;
		for (kk = 39; kk; kk--)
		{
			v248x[18] = m_Str_E9C38_smalltit[v218x].dword16;
			v248x[19] = m_Str_E9C38_smalltit[v218x].dword20;
			v248x[22] = m_Str_E9C38_smalltit[v218x].dword32;
			v218x++;
			v219 = m_Str_E9C38_smalltit[v218x - 1].word38;
			v220 = m_Str_E9C38_smalltit[v218x - 1].word38;
			if (m_Str_E9C38_smalltit[v218x].word38 & 4)
				break;
			v248x[12] = m_Str_E9C38_smalltit[v218x].dword16;
			v248x[13] = m_Str_E9C38_smalltit[v218x].dword20;
			v248x[16] = m_Str_E9C38_smalltit[v218x].dword32;
			v221 = m_Str_E9C38_smalltit[v218x].word38;
			v222 = v221 | v219;
			v223 = v221 & v220;
			v248x[6] = m_Str_E9C38_smalltit[v218x - 40].dword16;
			v248x[7] = m_Str_E9C38_smalltit[v218x - 40].dword20;
			v224 = m_Str_E9C38_smalltit[v218x - 40].dword32;
			v225x = v218x - 40;
#ifdef TEST_x64
			allert_error();
#endif
#ifdef COMPILE_FOR_64BIT // FIXME: 64bit
			std::cout << "FIXME: 64bit @ function " << __FUNCTION__ << ", line " << __LINE__ << std::endl;
#else
			v248x[10] = (int)(x_DWORD*)v224;
#endif
			v226 = m_Str_E9C38_smalltit[v225x].word38;
			v248x[0] = m_Str_E9C38_smalltit[v225x - 1].dword16;
			v248x[1] = m_Str_E9C38_smalltit[v225x - 1].dword20;
			v225x--;
			v248x[4] = m_Str_E9C38_smalltit[v225x].dword32;
			v227x = v225x + 40;
			v228 = (m_Str_E9C38_smalltit[v225x].word38 & 0xff) | v226 | v222;
			v229 = (m_Str_E9C38_smalltit[v225x].word38 & 0xff) & v226 & v223;
			if (m_Str_E9C38_smalltit[v227x].word38 & 0x1000)
			{
				x_BYTE_E126D = 7;
				x_BYTE_E126C = ((signed int)v248x[10] + v248x[16] + v248x[22] + v248x[4]) >> 18;
			}
			else
			{
				x_BYTE_E126D = 5;
			}
			if (!(v228 & 2) && !(v229 & 0x78))
			{
				DrawSquareInProjectionSpace(v248x, v227x, viewPort.Width, viewPort.Height, screenWidth);
			}
			if (m_Str_E9C38_smalltit[v227x].word36)
				DrawParticles_3E360(v227x, str_DWORD_F66F0x, x_BYTE_E88E0x, x_DWORD_F5730, x_DWORD_EA3E4, str_unk_1804B0ar, viewPort, screenWidth);//21f01b
			v218x = v227x + 1;
		}
		if (kk)
		{
			v231x = v218x - 1;
			v232x = v217x + 38;
			do
			{
				v248x[18] = m_Str_E9C38_smalltit[v232x].dword16;
				v248x[19] = m_Str_E9C38_smalltit[v232x].dword20;
				v248x[22] = m_Str_E9C38_smalltit[v232x].dword32;
				v233 = m_Str_E9C38_smalltit[v232x].word38;
				v248x[12] = m_Str_E9C38_smalltit[v232x + 1].dword16;
				v248x[13] = m_Str_E9C38_smalltit[v232x + 1].dword20;
				v248x[16] = m_Str_E9C38_smalltit[v232x + 1].dword32;
				v234 = m_Str_E9C38_smalltit[v232x + 1].word38;
				v248x[6] = m_Str_E9C38_smalltit[v232x - 39].dword16;
				v248x[7] = m_Str_E9C38_smalltit[v232x - 39].dword20;
				v235 = v233;
				v248x[10] = m_Str_E9C38_smalltit[v232x - 39].dword32;
				v236 = m_Str_E9C38_smalltit[v232x - 39].word38;
				v237 = v236 | v234 | v233;
				v238 = v236 & v234 & v235;
				v239 = m_Str_E9C38_smalltit[v232x - 40].dword16;
				v240x = v232x + 1;
				v248x[0] = v239;
				v241 = m_Str_E9C38_smalltit[v240x - 41].dword20;
				v240x -= 40;
				v248x[1] = v241;
				v248x[4] = m_Str_E9C38_smalltit[v240x - 1].dword32;
				v240x -= 1;
				v242 = m_Str_E9C38_smalltit[v240x].word38;
				v243x = v240x + 40;
				v244 = v242 | v237;
				v245 = v242 & v238;
				if (m_Str_E9C38_smalltit[v240x].word38 & 0x1000)
				{
					x_BYTE_E126D = 7;
					x_BYTE_E126C = ((signed int)v248x[10] + v248x[16] + v248x[22] + v248x[4]) >> 18;
				}
				else
				{
					x_BYTE_E126D = 5;
				}
				if (!(v244 & 2) && !(v245 & 0x78))
				{
					DrawSquareInProjectionSpace(v248x, v243x, viewPort.Width, viewPort.Height, screenWidth);
				}
				if (m_Str_E9C38_smalltit[v243x].word36)//address 21f1b5 aex 360000 ebx 3f78a0 ecx 0 edx 414eb0
					DrawParticles_3E360(v243x, str_DWORD_F66F0x, x_BYTE_E88E0x, x_DWORD_F5730, x_DWORD_EA3E4, str_unk_1804B0ar, viewPort, screenWidth);
				v232x = v243x - 1;
			} while (v232x >= v231x);
		}
		v217x -= 40;
		v289--;
	} while (v289);
}

int32_t* GameRender::x_DWORD_DB350_ret(uint32_t adress) {
	if (adress >= 0x100) return &Maths::x_DWORD_DB750[adress - 0x100];
	allert_error();
}

uint16_t GameRender::sub_3FD60(int a2x, uint8_t x_BYTE_E88E0x[], type_event_0x6E8E* x_DWORD_EA3E4[], type_str_unk_1804B0ar str_unk_1804B0ar, type_particle_str** str_DWORD_F66F0x[], int32_t x_DWORD_F5730[], ViewPort viewPort, uint16_t screenWidth)
{
	uint16_t result; // ax
	type_event_0x6E8E* v3x; // eax
	int v4; // edx
	int v5; // eax
	int v6; // ecx
	int v7; // esi
	int v8; // edx
	type_WORD_D951C* v9x; // esi
	int v10; // ecx
	int v11; // ST0C_4
	char v12; // al
	//int v13; // ebx
	//int v14; // eax
	//int v15; // ebx
	int v16; // ebx
	type_particle_str** v17x; // edi
	int v18; // eax
	int v19; // ebx
	int v20; // edx
	int v21; // eax
	int v22; // eax
	int v23; // eax
	int v24; // eax
	int v25; // eax
	int v26; // ebx
	int v27; // eax
	int v28; // eax
	int v29; // ebx
	int v30; // eax
	int v31; // eax
	int v32; // ebx
	int v33; // eax
	int v34; // eax
	int v35; // eax
	int v36; // eax
	//char v37; // cl
	int v38; // eax
	unsigned __int8 v39; // al
	int v40; // [esp+0h] [ebp-Ch]
	type_event_0x6E8E* v41x; // [esp+4h] [ebp-8h]
	int v42; // [esp+8h] [ebp-4h]

	//fix
	v41x = 0;
	type_particle_str* a1x = 0;
	//fix

	//result = *(x_WORD *)(a2 + 36);
	result = m_Str_E9C38_smalltit[a2x].word36;
	do
	{
		if (result < 0x3E8u)
		{
			v3x = x_DWORD_EA3E4[result];
			v41x = v3x;
			if (!(v3x->struct_byte_0xc_12_15.byte[0] & 0x21))
			{
				v4 = (signed __int16)(v3x->axis_0x4C_76.x - x_WORD_F2CC4);
				v5 = (signed __int16)(x_WORD_F2CC2 - v3x->axis_0x4C_76.y);
				v42 = -v3x->axis_0x4C_76.z - m_str_F2C20ar.dword0x20;
				v6 = (v4 * m_str_F2C20ar.dword0x0f - v5 * m_str_F2C20ar.dword0x17) >> 16;
				v40 = (m_str_F2C20ar.dword0x17 * v4 + m_str_F2C20ar.dword0x0f * v5) >> 16;
				v7 = (m_str_F2C20ar.dword0x17 * v4 + m_str_F2C20ar.dword0x0f * v5) >> 16;
				v8 = v40 * v40 + v6 * v6;
				if (v7 > 64 && v8 < m_str_F2C20ar.dword0x15)
				{
					if (v8 <= m_str_F2C20ar.dword0x13)
					{
						m_str_F2C20ar.dword0x00 = 0x2000;
					}
					else if (v8 < m_str_F2C20ar.dword0x16)
					{
						m_str_F2C20ar.dword0x00 = 32 * (m_str_F2C20ar.dword0x16 - (v40 * v40 + v6 * v6)) / m_str_F2C20ar.dword0x12 << 8;
					}
					else
					{
						m_str_F2C20ar.dword0x00 = 0;
					}
					//v9 = (short*)&x_WORD_D951C[7 * v41x->word_0x5A_90];
					v9x = &str_WORD_D951C[v41x->word_0x5A_90];
					v10 = v6 * m_str_F2C20ar.dword0x18 / v40;
					v11 = m_str_F2C20ar.dword0x18 * v42 / v40 + m_str_F2C20ar.dword0x22;
					m_str_F2C20ar.dword0x04_screenY = ((v10 * m_str_F2C20ar.dword0x11 - m_str_F2C20ar.dword0x0d * v11) >> 16) + m_str_F2C20ar.dword0x24;
					m_str_F2C20ar.dword0x03_screenX = m_str_F2C20ar.dword0x10 - ((m_str_F2C20ar.dword0x0d * v10 + v11 * m_str_F2C20ar.dword0x11) >> 16);
					v12 = v9x->byte_12;
					x_BYTE_F2CC6 = 0;
					switch (v12)
					{
					case 0:
						//v13 = v9x->word_0;
						if (str_DWORD_F66F0x[v9x->word_0])
						{
							//v14 = v9x->word_0;
							//v15 = 4 * v9x->word_0;
							goto LABEL_16;
						}
						if (MainInitTmaps_71520(v9x->word_0))
						{
							//v14 = v9x->word_0;
							//v15 = 4 * v14;
						LABEL_16:
							x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v9x->word_0].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
							a1x = *str_DWORD_F66F0x[v9x->word_0];
							goto LABEL_47;
						}
						break;
					case 1:
						if (!str_DWORD_F66F0x[v9x->word_0] && !MainInitTmaps_71520(v9x->word_0))
							break;
						x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v9x->word_0].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
						a1x = *str_DWORD_F66F0x[v9x->word_0];
						goto LABEL_47;
					case 2:
					case 3:
					case 4:
					case 5:
					case 6:
					case 7:
					case 8:
					case 9:
					case 10:
					case 11:
					case 12:
					case 13:
					case 14:
					case 15:
					case 16:
						goto LABEL_26;
					case 17:
						v26 = (((v41x->word_0x1C_28 - (unsigned __int16)x_WORD_F2CC0) >> 3) & 0xF0) >> 4;
						if (v26 < 8)
						{
							if (str_DWORD_F66F0x[v26 + v9x->word_0])
							{
								x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v26 + v9x->word_0].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
							}
							else
							{
								if (!MainInitTmaps_71520(v26 + v9x->word_0))
									break;
								x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v26 + v9x->word_0].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
							}
							a1x = *str_DWORD_F66F0x[v26 + v9x->word_0];
							goto LABEL_47;
						}
						if (str_DWORD_F66F0x[v9x->word_0 + 15 - v26])
						{
							x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v9x->word_0 + 15 - v26].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
						}
						else
						{
							if (!MainInitTmaps_71520(v9x->word_0 + 15 - v26))
								break;
							x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v9x->word_0 + 15 - v26].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
						}
						a1x = *str_DWORD_F66F0x[v9x->word_0 + 15 - v26];
						m_str_F2C20ar.dword0x08_width = a1x->width;
						m_str_F2C20ar.dword0x06_height = a1x->height;
						/*str_F2C20ar.dword0x08_width = *(unsigned __int16*)(a1 + 2);
						str_F2C20ar.dword0x06_height = *(unsigned __int16*)(a1 + 4);*/
						v28 = (signed __int64)(m_str_F2C20ar.dword0x18 * v9x->rotSpeed_8) / v40;
						m_str_F2C20ar.dword0x0c_realHeight = v28;
						m_str_F2C20ar.dword0x09_realWidth = v28 * m_str_F2C20ar.dword0x08_width / m_str_F2C20ar.dword0x06_height;
						v23 = -m_str_F2C20ar.dword0x08_width;
						goto LABEL_69;
					case 18:
						v29 = (((v41x->word_0x1C_28 - (unsigned __int16)x_WORD_F2CC0) >> 3) & 0xF0) >> 4;
						v30 = v29 + v9x->word_0;
						if (str_DWORD_F66F0x[v30])
						{
							x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v30].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
						}
						else
						{
							if (!MainInitTmaps_71520(v29 + v9x->word_0))
								break;
							x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v29 + v9x->word_0].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
						}
						a1x = *str_DWORD_F66F0x[v9x->word_0 + v29];
						m_str_F2C20ar.dword0x08_width = a1x->width;
						m_str_F2C20ar.dword0x06_height = a1x->height;
						/*str_F2C20ar.dword0x08_width = *(unsigned __int16*)(a1 + 2);
						str_F2C20ar.dword0x06_height = *(unsigned __int16*)(a1 + 4);*/
						v31 = (signed __int64)(m_str_F2C20ar.dword0x18 * v9x->rotSpeed_8) / v40;
						m_str_F2C20ar.dword0x0c_realHeight = v31;
						m_str_F2C20ar.dword0x09_realWidth = v31 * m_str_F2C20ar.dword0x08_width / m_str_F2C20ar.dword0x06_height;
						v23 = m_str_F2C20ar.dword0x08_width;
						goto LABEL_69;
					case 19:
						v19 = (((v41x->word_0x1C_28 - (unsigned __int16)x_WORD_F2CC0) >> 3) & 0xF0) >> 4;
						if (v19 >= 8)
						{
							v24 = v9x->word_0 + (unsigned __int8)x_BYTE_D4750[0xc + v19];
							if (!str_DWORD_F66F0x[v24])
							{
								if (!MainInitTmaps_71520(v9x->word_0 + (unsigned __int8)x_BYTE_D4750[0xc + v19]))
									break;
								v24 = v9x->word_0 + (unsigned __int8)x_BYTE_D4750[0xc + v19];
							}
							x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v24].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
							a1x = *str_DWORD_F66F0x[v9x->word_0 + (unsigned __int8)x_BYTE_D4750[0xc + v19]];
							m_str_F2C20ar.dword0x08_width = a1x->width;
							m_str_F2C20ar.dword0x06_height = a1x->height;
							/*str_F2C20ar.dword0x08_width = *(unsigned __int16*)(a1 + 2);
							str_F2C20ar.dword0x06_height = *(unsigned __int16*)(a1 + 4);*/
							v25 = (signed __int64)(m_str_F2C20ar.dword0x18 * v9x->rotSpeed_8) / v40;
							m_str_F2C20ar.dword0x0c_realHeight = v25;
							m_str_F2C20ar.dword0x09_realWidth = v25 * m_str_F2C20ar.dword0x08_width / m_str_F2C20ar.dword0x06_height;
							v23 = -m_str_F2C20ar.dword0x08_width;
						}
						else
						{
							v20 = (unsigned __int8)x_BYTE_D4750[0xc + v19];
							v21 = v20 + v9x->word_0;
							if (str_DWORD_F66F0x[v21])
							{
								x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v21].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
							}
							else
							{
								if (!MainInitTmaps_71520(v9x->word_0 + (unsigned __int8)v20))
									break;
								x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v9x->word_0 + (unsigned __int8)x_BYTE_D4750[0xc + v19]].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
							}
							a1x = *str_DWORD_F66F0x[v9x->word_0 + (unsigned __int8)x_BYTE_D4750[0xc + v19]];
							m_str_F2C20ar.dword0x08_width = a1x->width;
							m_str_F2C20ar.dword0x06_height = a1x->height;
							/*str_F2C20ar.dword0x08_width = *(unsigned __int16*)(a1 + 2);
							str_F2C20ar.dword0x06_height = *(unsigned __int16*)(a1 + 4);*/
							v22 = (signed __int64)(m_str_F2C20ar.dword0x18 * v9x->rotSpeed_8) / v40;
							m_str_F2C20ar.dword0x0c_realHeight = v22;
							m_str_F2C20ar.dword0x09_realWidth = v22 * m_str_F2C20ar.dword0x08_width / m_str_F2C20ar.dword0x06_height;
							v23 = m_str_F2C20ar.dword0x08_width;
						}
						goto LABEL_69;
					case 20:
						v32 = (((v41x->word_0x1C_28 - (unsigned __int16)x_WORD_F2CC0) >> 3) & 0xF0) >> 4;
						if (v32 >= 8)
						{
							v35 = v9x->word_0 + (unsigned __int8)x_BYTE_D4750[0x1c + v32];
							if (str_DWORD_F66F0x[v35])
							{
								x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v35].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
							}
							else
							{
								if (!MainInitTmaps_71520(v9x->word_0 + (unsigned __int8)x_BYTE_D4750[0x1c + v32]))
									break;
								x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v9x->word_0 + (unsigned __int8)x_BYTE_D4750[0x1c + v32]].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
							}
							a1x = *str_DWORD_F66F0x[(unsigned __int8)x_BYTE_D4750[0x1c + v32] + v9x->word_0];
							m_str_F2C20ar.dword0x08_width = a1x->width;
							m_str_F2C20ar.dword0x06_height = a1x->height;
							/*str_F2C20ar.dword0x08_width = *(unsigned __int16*)(a1 + 2);
							str_F2C20ar.dword0x06_height = *(unsigned __int16*)(a1 + 4);*/
							v36 = (signed __int64)(m_str_F2C20ar.dword0x18 * v9x->rotSpeed_8) / v40;
							m_str_F2C20ar.dword0x0c_realHeight = v36;
							m_str_F2C20ar.dword0x09_realWidth = v36 * m_str_F2C20ar.dword0x08_width / m_str_F2C20ar.dword0x06_height;
							v23 = -m_str_F2C20ar.dword0x08_width;
						}
						else
						{
							v33 = v9x->word_0 + (unsigned __int8)x_BYTE_D4750[0x1c + v32];
							if (str_DWORD_F66F0x[v33])
							{
								x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v33].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
							}
							else
							{
								if (!MainInitTmaps_71520(v9x->word_0 + (unsigned __int8)x_BYTE_D4750[0x1c + v32]))
									break;
								x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v9x->word_0 + (unsigned __int8)x_BYTE_D4750[0x1c + v32]].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
							}
							a1x = *str_DWORD_F66F0x[v9x->word_0 + (unsigned __int8)x_BYTE_D4750[0x1c + v32]];
							m_str_F2C20ar.dword0x08_width = a1x->width;
							m_str_F2C20ar.dword0x06_height = a1x->height;
							/*str_F2C20ar.dword0x08_width = *(unsigned __int16*)(a1 + 2);
							str_F2C20ar.dword0x06_height = *(unsigned __int16*)(a1 + 4);*/
							v34 = (signed __int64)(m_str_F2C20ar.dword0x18 * v9x->rotSpeed_8) / v40;
							m_str_F2C20ar.dword0x0c_realHeight = v34;
							m_str_F2C20ar.dword0x09_realWidth = v34 * m_str_F2C20ar.dword0x08_width / m_str_F2C20ar.dword0x06_height;
							v23 = m_str_F2C20ar.dword0x08_width;
						}
						goto LABEL_69;
					case 21:
						v16 = v9x->word_0;
						if (str_DWORD_F66F0x[v16])
						{
							x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v16].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
						}
						else
						{
							if (!MainInitTmaps_71520(v16))
								break;
							x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v9x->word_0].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
						}
						v17x = str_DWORD_F66F0x[v9x->word_0];
						x_BYTE_F2CC6 = 1;
						a1x = *v17x;
						goto LABEL_47;
					case 22:
					case 23:
					case 24:
					case 25:
					case 26:
					case 27:
					case 28:
					case 29:
					case 30:
					case 31:
					case 32:
					case 33:
					case 34:
					case 35:
					case 36:
						x_BYTE_F2CC6 = 1;
					LABEL_26:
						v18 = v41x->byte_0x5C_92 + v9x->word_0;
						if (str_DWORD_F66F0x[v18])
						{
							x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v18].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
						}
						else
						{
							if (!MainInitTmaps_71520(v9x->word_0 + v41x->byte_0x5C_92))
								break;
							x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v9x->word_0 + v41x->byte_0x5C_92].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
						}
						a1x = *str_DWORD_F66F0x[v9x->word_0 + v41x->byte_0x5C_92];
					LABEL_47:
						m_str_F2C20ar.dword0x08_width = a1x->width;
						m_str_F2C20ar.dword0x06_height = a1x->height;
						/*str_F2C20ar.dword0x08_width = *(unsigned __int16*)(a1 + 2);
						str_F2C20ar.dword0x06_height = *(unsigned __int16*)(a1 + 4);*/
						v27 = (signed __int64)(m_str_F2C20ar.dword0x18 * v9x->rotSpeed_8) / v40;
						m_str_F2C20ar.dword0x0c_realHeight = v27;
						m_str_F2C20ar.dword0x09_realWidth = v27 * m_str_F2C20ar.dword0x08_width / m_str_F2C20ar.dword0x06_height;
						v23 = m_str_F2C20ar.dword0x08_width;
					LABEL_69:
						m_str_F2C20ar.dword0x05 = v23;
					LABEL_70:
						//v37 = *(x_BYTE*)a1 | 8;

						//str_F2C20ar.dword0x02_data = a1 + 6;
						m_str_F2C20ar.dword0x02_data = a1x->textureBuffer;
						v38 = m_str_F2C20ar.dword0x00;
						//*(x_BYTE*)a1 = v37;
						/*v37 = a1x->un_0.byte[0] | 8;
						a1x->un_0.byte[0] = v37;*/
						a1x->word_0 |= 8;
						if (v38 == 0x2000)
							v39 = x_BYTE_D4750[v9x->byte_10];
						else
							v39 = x_BYTE_D4750[6 + v9x->byte_10];
						m_str_F2C20ar.dword0x01_rotIdx = v39;
						m_str_F2C20ar.dword0x09_realWidth++;
						m_str_F2C20ar.dword0x0c_realHeight++;
						DrawSprite_41BD3(2u, x_BYTE_E88E0x, x_DWORD_EA3E4, str_unk_1804B0ar, viewPort, screenWidth);
						break;
					default:
						goto LABEL_70;
					}
				}
			}
		}
		result = v41x->oldMapEntity_0x16_22;
	} while (result);
	return result;
}

void GameRender::sub_88740(type_event_0x6E8E* a1x, type_event_0x6E8E* x_DWORD_EA3E4[], type_str_unk_1804B0ar str_unk_1804B0ar, int a2, int a3)
{
	int v3; // esi
	type_event_0x6E8E* v4x; // edx
	unsigned __int8 v5; // al
	unsigned __int8 v6; // al
	unsigned __int8 v7; // al
	unsigned __int8 v8; // al
	//char v9; // cl
	signed int v10; // eax
	unsigned __int8 v11; // al
	//unsigned int v12; // edi
	signed int v13; // eax
	//int v14; // esi
	//char v15; // dl
	//char v16; // dh
	//char v17; // bl
	char v18; // [esp+0h] [ebp-4h]

	v3 = 0;
	if (str_unk_1804B0ar.byte_0x9e & 1)
		return;
	v4x = x_DWORD_EA3E4[D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].word_0x00a_2BE4_11240];
	v5 = a1x->type_0x3F_63;
	if (v5 < 5u)
	{
		if (v5 >= 2u)
		{
			if (v5 <= 2u)
			{
				v6 = a1x->subtype_0x40_64;
				if (v6 >= 1u)
				{
					if (v6 <= 1u)
					{
						v3 = 27;
					}
					else if (v6 == 2)
					{
						v3 = 22;
					}
				}
			}
			else if (v5 == 3)
			{
				v11 = a1x->subtype_0x40_64;
				if (v11 < 2u)
				{
					if (v11 == 1)
						v3 = 28;
				}
				else if (v11 <= 2u)
				{
					v3 = (a1x->word_0x1A_26 != v4x->word_0x1A_26) + 24;
				}
				else if (v11 == 3)
				{
					if (a1x->word_0x1A_26 == v4x->word_0x1A_26)
						v3 = 23;
					else
						v3 = 26;
				}
			}
		}
		goto LABEL_48;
	}
	if (v5 > 5u)
	{
		if (v5 >= 0xAu)
		{
			if (v5 <= 0xAu)
			{
				if (a1x->subtype_0x40_64 == 39 && a1x->word_0x94_148 != v4x->word_0x1A_26)
					v3 = 18;
			}
			else if (v5 == 15 && !(a1x->struct_byte_0xc_12_15.byte[0] & 1))
			{
				v3 = 20;
			}
		}
		goto LABEL_48;
	}
	if (a1x->word_0x1A_26 != v4x->word_0x1A_26)
	{
		v7 = a1x->subtype_0x40_64;
		if (v7 < 0xCu)
			goto LABEL_30;
		if (v7 > 0xEu)
		{
			if (v7 == 22)
			{
				if (a1x->byte_0x45_69 != -76 && a1x->word_0x94_148 != v4x->word_0x1A_26)
					v3 = 18;
				goto LABEL_48;
			}
		LABEL_30:
			v8 = a1x->byte_0x45_69;
			if (v8 < 0xE8u || v8 > 0xEAu)
			{
				//v9 = a1x->byte_0x49_73;
				v10 = 1;
				if ((a1x->StageVar2_0x49_73 == 14 || a1x->StageVar2_0x49_73 == 13) && a1x->word_0x28_40 == v4x->word_0x1A_26)
					v10 = 0;
				if (v10)
					v3 = 19;
			}
			goto LABEL_48;
		}
	}
LABEL_48:
	if (v3)
	{
		if (m_x_WORD_180660_VGA_type_resolution & 1)
		{
			a2 *= 2;
			a3 *= 2;
		}
		//v12 = 30 * v3;
		if (str_E2A74[v3].axis_2[0] & 2)
		{
			if (a1x == str_E2A74[v3].dword_12)
			{
				//v16 = str_unk_1804B0ar.byte_0x9f;
				if (!(str_unk_1804B0ar.byte_0x9f & 0x1))
				{
					//v17 = array_E2A74[0x2 + v12] | 8;
					str_E2A74[v3].axis_2[3] = a2;
					str_E2A74[v3].axis_2[0] |= 8;
					str_E2A74[v3].axis_2[4] = a3;
					str_unk_1804B0ar.byte_0x9f |= 2;
				}
			}
		}
		else
		{
			v18 = 0;
			v13 = Maths::sub_58490_radix_3d_2(&v4x->axis_0x4C_76, &a1x->axis_0x4C_76);
			if (!str_E2A74[v3].dword_12 || v13 < str_E2A74[v3].dword_20 && v13 > 1024)
				v18 = 1;
			if (v18)
			{
				//v14 = 15 * v3;
				str_E2A74[v3].dword_20 = v13;
				//v15 = array_E2A74[0x2 + 30 * v3];
				str_E2A74[v3].dword_12 = a1x;
				str_E2A74[v3].axis_2[0] |= 8;
			}
		}
	}
}

void GameRender::SetBillboards_3B560(int16_t roll, uint8_t unk_F0A20x[], uint16_t viewPortWidth, uint16_t viewPortHeight, uint16_t screenWidth)
{
	int v1; // edx
	type_unk_F0E20x* v2x; // edi
	signed int* v3; // esi
	//type_E9C38_smalltit_59360* v3x;
	int32_t v4; // eax
	__int16 v5; // bx
	signed int v6; // ecx
	int v7; // edx
	unsigned __int8 v8; // cf
	int v9; // eax
	int v10; // esi
	uint32_t v11; // eax
	type_unk_F0E20x* v12x; // edi
	signed int* v13; // esi
	int v14; // eax
	signed int v15; // ecx
	int v16; // edx
	type_unk_F0E20x* v17x; // edi
	signed int* v18; // esi
	int v19; // eax
	__int16 v20; // bx
	signed int v21; // ecx
	int v22; // edx
	type_unk_F0E20x* v23x; // edi
	signed int* v24; // esi
	int v25; // eax
	__int16 v26; // bx
	signed int v27; // ecx
	int v28; // edx
	type_unk_F0E20x* resultx; // eax
	//int v30; // eax
	signed int* v31; // esi
	type_unk_F0E20x* v32x; // edi
	int v33; // eax
	signed int v34; // ecx
	int v35; // edx
	type_unk_F0E20x* v36x; // edi
	signed int* v37; // esi
	int v38; // eax
	__int16 v39; // bx
	signed int v40; // ecx
	int v41; // edx
	uint8_t* v42x; // edx
	type_unk_F0E20x* v43x; // edi
	signed int* v44; // esi
	int v45; // eax
	__int16 v46; // bx
	signed int v47; // ecx
	int v48; // edx
	int v49; // edx
	int v50; // esi
	//int v51; // esi
	signed int* v52; // esi
	type_unk_F0E20x* v53x; // edi
	int v54; // eax
	signed int v55; // ecx
	int v56; // edx
	type_unk_F0E20x* v57x; // edi
	signed int* v58; // esi
	int v59; // eax
	__int16 v60; // bx
	signed int v61; // ecx
	int v62; // edx
	type_unk_F0E20x* v63x; // edi
	signed int* v64; // esi
	int v65; // eax
	__int16 v66; // bx
	signed int v67; // ecx
	int v68; // edx
	int v69; // esi
	int v70; // eax
	type_unk_F0E20x* v71x; // edi
	signed int* v72; // esi
	int v73; // eax
	signed int v74; // ecx
	int v75; // edx
	type_unk_F0E20x* v76x; // edi
	signed int* v77; // esi
	int v78; // eax
	__int16 v79; // bx
	signed int v80; // ecx
	int v81; // edx
	int v82; // edx
	int v83; // ecx
	int v84; // [esp+0h] [ebp-10h]
	int v85; // [esp+0h] [ebp-10h]
	int v86; // [esp+0h] [ebp-10h]
	int v87; // [esp+0h] [ebp-10h]
	int v88; // [esp+4h] [ebp-Ch]
	int v89; // [esp+4h] [ebp-Ch]
	int v90; // [esp+4h] [ebp-Ch]
	int v91; // [esp+4h] [ebp-Ch]
	int v92; // [esp+8h] [ebp-8h]
	int v93; // [esp+8h] [ebp-8h]
	int v94; // [esp+8h] [ebp-8h]
	int v95; // [esp+8h] [ebp-8h]
	int v96; // [esp+8h] [ebp-8h]
	int v97; // [esp+8h] [ebp-8h]
	int v98; // [esp+8h] [ebp-8h]
	int v99; // [esp+8h] [ebp-8h]
	int v100; // [esp+8h] [ebp-8h]
	__int16 v101; // [esp+Ch] [ebp-4h]
	__int16 v102; // [esp+Ch] [ebp-4h]
	__int16 v103; // [esp+Ch] [ebp-4h]
	__int16 v104; // [esp+Ch] [ebp-4h]
	__int16 v105; // [esp+Ch] [ebp-4h]
	__int16 v106; // [esp+Ch] [ebp-4h]
	__int16 v107; // [esp+Ch] [ebp-4h]
	__int16 v108; // [esp+Ch] [ebp-4h]

	v1 = roll & 0x7FF;
	m_str_F2C20ar.dword0x1e = v1 >> 8;
	switch (v1 >> 8)
	{
	case 0:
		//str_F2C20ar.dword0x27 = x_DWORD_DB750[v1];
		//str_F2C20ar.dword0x27 = *(int32_t*)x_DWORD_DB750ar_ret(v1 * 4);//copy to other //db750
		m_str_F2C20ar.dword0x27 = Maths::x_DWORD_DB750[v1];
		//str_F2C20ar.dword0x1b = (int)off_DBF50[v1];
		//str_F2C20ar.dword0x1b = *(int32_t*)x_DWORD_DB750ar_ret(0x800+v1 * 4);//copy to other//db750
		m_str_F2C20ar.dword0x1b = Maths::x_DWORD_DB750[0x200 + v1];

		v88 = screenWidth;
		v92 = (unsigned __int16)viewPortWidth;
		m_str_F2C20ar.dword0x1f = (m_str_F2C20ar.dword0x27 << 8) / (m_str_F2C20ar.dword0x1b >> 8);
		v101 = (m_str_F2C20ar.dword0x27 << 8) / (m_str_F2C20ar.dword0x1b >> 8);
		v2x = m_str_F0E20x;
		v3 = (signed int*)(x_DWORD_E9C38_smalltit + 59360);
		//v3x = &str_E9C38_smalltit_59360;
		v4 = 0;
		v5 = 0;
		v6 = 1;
		v7 = 0;
		do
		{
			v2x->dword_1 = v4;
			v2x->dword_2 = v7;
			v8 = __CFADD__(v101, v5);
			v5 += v101;
			if (v8)
			{
				v4 += v88;
				++v7;
				*v3 = v6;
				++v3;
			}
			//v2 += 3;
			v2x++;
			++v4;
			++v6;
			--v92;
		} while (v92);
		m_str_F2C20ar.dword0x1d = v7;
		m_str_F2C20ar.dword0x21 = -v7;
		m_str_F2C20ar.width0x25 = (unsigned __int16)viewPortHeight;
		m_str_F2C20ar.height0x26 = (unsigned __int16)viewPortWidth;
		m_str_F2C20ar.dword0x1c = (unsigned __int16)viewPortHeight + (unsigned __int16)viewPortWidth;
		m_str_F2C20ar.pbyte0x1a = (4 * (v7 - 1) + &x_DWORD_E9C38_smalltit[59360]);
		v9 = screenWidth;
		m_str_F2C20ar.Height_0x19 = viewPortHeight - v7;
		goto LABEL_66;
	case 1:
		//v10 = x_DWORD_DB750[v1];
		//v10 = *(int32_t*)x_DWORD_DB750ar_ret(v1 * 4);//copy to other
		v10 = Maths::x_DWORD_DB750[v1];
		//v11 = off_DBF50[v1];
		//v11 = *(int32_t*)x_DWORD_DB750ar_ret(0x800 + v1 * 4);//copy to other
		v11 = Maths::x_DWORD_DB750[0x200 + v1];

		m_str_F2C20ar.dword0x27 = v10;
		m_str_F2C20ar.dword0x1b = (int)v11;
		v84 = screenWidth;
		v93 = (unsigned __int16)viewPortHeight;
		if (v1 == 256)
		{
			m_str_F2C20ar.dword0x1f = 0x10000;
			v12x = m_str_F0E20x;
			v13 = (signed int*)&x_DWORD_E9C38_smalltit[59360];
			v14 = 0;
			v15 = 1;
			v16 = 0;
			do
			{
				v12x->dword_1 = v14;
				v12x->dword_2 = v16++;
				*v13 = v15;
				++v13;
				//v12 += 3;
				v12x++;
				v14 += v84 + 1;
				++v15;
				--v93;
			} while (v93);
			m_str_F2C20ar.dword0x1d = (unsigned __int16)viewPortHeight;
			m_str_F2C20ar.dword0x21 = -(unsigned __int16)viewPortHeight;
		}
		else
		{
			m_str_F2C20ar.dword0x1f = (m_str_F2C20ar.dword0x1b << 8) / (v10 >> 8);
			v102 = (m_str_F2C20ar.dword0x1b << 8) / (v10 >> 8);
			v17x = m_str_F0E20x;
			v18 = (signed int*)(x_DWORD_E9C38_smalltit + 59360);
			v19 = 0;
			v20 = 0;
			v21 = 1;
			v22 = 0;
			do
			{
				v17x->dword_1 = v19;
				v17x->dword_2 = v22;
				v8 = __CFADD__(v102, v20);
				v20 += v102;
				if (v8)
				{
					v19++;
					v22++;
					*v18 = v21;
					v18++;
				}
				//v17 += 3;
				v17x++;
				v19 += v84;
				v21++;
				v93--;
			} while (v93);
			m_str_F2C20ar.dword0x1d = v22;
			m_str_F2C20ar.dword0x21 = -v22;
		}
		m_str_F2C20ar.width0x25 = (unsigned __int16)viewPortWidth;
		m_str_F2C20ar.height0x26 = (unsigned __int16)viewPortHeight;
		m_str_F2C20ar.dword0x1c = (unsigned __int16)viewPortHeight + (unsigned __int16)viewPortWidth;
		m_str_F2C20ar.pbyte0x1a = (4 * (-1 - m_str_F2C20ar.dword0x21) + &x_DWORD_E9C38_smalltit[59360]);
		v9 = -1;
		m_str_F2C20ar.Height_0x19 = (unsigned __int16)viewPortWidth + m_str_F2C20ar.dword0x21;
		goto LABEL_66;
	case 2:
		//str_F2C20ar.dword0x27 = *(int32_t*)&x_DWORD_DAF50ar[v1*4];
		m_str_F2C20ar.dword0x27 = Maths::x_DWORD_DB750[v1 - 0x200];//copy to other
		//str_F2C20ar.dword0x1b = x_DWORD_DB750[v1];
		//str_F2C20ar.dword0x1b = *(int32_t*)x_DWORD_DB750ar_ret(v1 * 4);//copy to other
		m_str_F2C20ar.dword0x1b = Maths::x_DWORD_DB750[v1];

		v85 = screenWidth;
		v94 = (unsigned __int16)viewPortHeight;
		m_str_F2C20ar.dword0x1f = (m_str_F2C20ar.dword0x27 << 8) / (m_str_F2C20ar.dword0x1b >> 8);
		v103 = (m_str_F2C20ar.dword0x27 << 8) / (m_str_F2C20ar.dword0x1b >> 8);
		v23x = m_str_F0E20x;
		v24 = (signed int*)&x_DWORD_E9C38_smalltit[59360];
		v25 = 0;
		v26 = 0;
		v27 = 1;
		v28 = 0;
		do
		{
			v23x->dword_1 = v25;
			v23x->dword_2 = v28;
			v8 = __CFADD__(v103, v26);
			v26 += v103;
			if (v8)
			{
				v25--;
				v28++;
				*v24 = v27;
				v24++;
			}
			//v23 += 3;
			v23x++;
			v25 += v85;
			v27++;
			v94--;
		} while (v94);
		m_str_F2C20ar.dword0x1d = v28;
		m_str_F2C20ar.dword0x21 = -v28;
		m_str_F2C20ar.width0x25 = (unsigned __int16)viewPortWidth;
		m_str_F2C20ar.dword0x1c = (unsigned __int16)viewPortHeight + (unsigned __int16)viewPortWidth;
		m_str_F2C20ar.height0x26 = (unsigned __int16)viewPortHeight;
		m_str_F2C20ar.dword0x23 = -1;
		m_str_F2C20ar.Height_0x19 = (unsigned __int16)viewPortWidth - v28;
		m_str_F2C20ar.pbyte0x1a = (4 * (v28 - 1) + &x_DWORD_E9C38_smalltit[59360]);
		v95 = (unsigned __int16)viewPortHeight - 1;
		resultx = m_str_F0E20x;
		if (v95 < 0)
			return;
		goto LABEL_68;
	case 3:
		//str_F2C20ar.dword0x27 = *(int32_t*)&x_DWORD_DAF50ar[v1*4];
		m_str_F2C20ar.dword0x27 = Maths::x_DWORD_DB750[v1 - 0x200];//copy to other
		//str_F2C20ar.dword0x1b = x_DWORD_DB750[v1];
		//str_F2C20ar.dword0x1b = *(int32_t*)x_DWORD_DB750ar_ret(v1 * 4);//copy to other
		m_str_F2C20ar.dword0x1b = Maths::x_DWORD_DB750[v1];

		v89 = screenWidth;
		v96 = (unsigned __int16)viewPortWidth;
		if (v1 == 768)
		{
			m_str_F2C20ar.dword0x1f = 0x10000;
			v31 = (signed int*)&x_DWORD_E9C38_smalltit[59360];
			v32x = m_str_F0E20x;
			v33 = 0;
			v34 = 1;
			v35 = 0;
			do
			{
				v32x->dword_1 = v33;
				v32x->dword_2 = v35++;
				*v31 = v34;
				v31++;
				//v32 += 3;
				v32x++;
				v33 = v89 + v33 - 1;
				v34++;
				v96--;
			} while (v96);
			m_str_F2C20ar.dword0x1d = (unsigned __int16)viewPortWidth;
			m_str_F2C20ar.dword0x21 = -(unsigned __int16)viewPortWidth;
		}
		else
		{
			m_str_F2C20ar.dword0x1f = (m_str_F2C20ar.dword0x1b << 8) / (m_str_F2C20ar.dword0x27 >> 8);
			v104 = (m_str_F2C20ar.dword0x1b << 8) / (m_str_F2C20ar.dword0x27 >> 8);
			v36x = m_str_F0E20x;
			v37 = (signed int*)(x_DWORD_E9C38_smalltit + 59360);
			v38 = 0;
			v39 = 0;
			v40 = 1;
			v41 = 0;
			do
			{
				v36x->dword_1 = v38;
				v36x->dword_2 = v41;
				v8 = __CFADD__(v104, v39);
				v39 += v104;
				if (v8)
				{
					v38 += v89;
					v41++;
					*v37 = v40;
					v37++;
				}
				//v36 += 3;
				v36x++;
				v38--;
				v40++;
				v96--;
			} while (v96);
			m_str_F2C20ar.dword0x1d = v41;
			m_str_F2C20ar.dword0x21 = -v41;
		}
		m_str_F2C20ar.width0x25 = (unsigned __int16)viewPortHeight;
		m_str_F2C20ar.height0x26 = (unsigned __int16)viewPortWidth;
		m_str_F2C20ar.dword0x1c = (unsigned __int16)viewPortHeight + (unsigned __int16)viewPortWidth;
		v42x = (4 * (-1 - m_str_F2C20ar.dword0x21) + &x_DWORD_E9C38_smalltit[59360]);
		m_str_F2C20ar.Height_0x19 = (unsigned __int16)viewPortHeight + m_str_F2C20ar.dword0x21;
		v9 = -screenWidth;
		goto LABEL_65;
	case 4:
		//str_F2C20ar.dword0x27 = *(int32_t*)&x_DWORD_DA750ar[v1*4];
		m_str_F2C20ar.dword0x27 = Maths::x_DWORD_DB750[v1 - 0x400];//copy to other
		//str_F2C20ar.dword0x1b = *(int32_t*)&x_DWORD_DAF50ar[v1*4];
		m_str_F2C20ar.dword0x1b = Maths::x_DWORD_DB750[v1 - 0x200];//copy to other

		v90 = -screenWidth;
		v97 = (unsigned __int16)viewPortWidth;
		m_str_F2C20ar.dword0x1f = (m_str_F2C20ar.dword0x27 << 8) / (m_str_F2C20ar.dword0x1b >> 8);
		v105 = (m_str_F2C20ar.dword0x27 << 8) / (m_str_F2C20ar.dword0x1b >> 8);
		v43x = m_str_F0E20x;
		v44 = (signed int*)&x_DWORD_E9C38_smalltit[59360];
		v45 = 0;
		v46 = 0;
		v47 = 1;
		v48 = 0;
		do
		{
			v43x->dword_1 = v45;
			v43x->dword_2 = v48;
			v8 = __CFADD__(v105, v46);
			v46 += v105;
			if (v8)
			{
				v45 += v90;
				v48++;
				*v44 = v47;
				v44++;
			}
			//v43 += 3;
			v43x++;
			v45--;
			v47++;
			v97--;
		} while (v97);
		m_str_F2C20ar.dword0x1d = v48;
		v49 = -v48;
		m_str_F2C20ar.dword0x21 = v49;
		v50 = (unsigned __int16)viewPortHeight + v49;
		m_str_F2C20ar.width0x25 = (unsigned __int16)viewPortHeight;
		m_str_F2C20ar.height0x26 = (unsigned __int16)viewPortWidth;
		m_str_F2C20ar.dword0x1c = (unsigned __int16)viewPortHeight + (unsigned __int16)viewPortWidth;
		v42x = (4 * (-1 - v49) + &x_DWORD_E9C38_smalltit[59360]);
		m_str_F2C20ar.Height_0x19 = v50;
		v9 = -screenWidth;
		goto LABEL_65;
	case 5:
		//str_F2C20ar.dword0x1b = *(int32_t*)&x_DWORD_DAF50ar[v1*4];
		m_str_F2C20ar.dword0x1b = Maths::x_DWORD_DB750[v1 - 0x200];//copy to other
		//str_F2C20ar.dword0x27 = *(int32_t*)&x_DWORD_DA750ar[v1 * 4];
		m_str_F2C20ar.dword0x27 = Maths::x_DWORD_DB750[v1 - 0x400];//copy to other

		v86 = -screenWidth;
		v98 = (unsigned __int16)viewPortHeight;
		if (v1 == 1280)
		{
			m_str_F2C20ar.dword0x1f = 0x10000;
			v52 = (signed int*)&x_DWORD_E9C38_smalltit[59360];
			v53x = m_str_F0E20x;
			v54 = 0;
			v55 = 1;
			v56 = 0;
			do
			{
				v53x->dword_1 = v54;
				v53x->dword_2 = v56++;
				*v52 = v55;
				++v52;
				//v53 += 3;
				v53x++;
				v54 = v86 + v54 - 1;
				++v55;
				--v98;
			} while (v98);
			m_str_F2C20ar.dword0x1d = (unsigned __int16)viewPortHeight;
			m_str_F2C20ar.dword0x21 = -(unsigned __int16)viewPortHeight;
		}
		else
		{
			m_str_F2C20ar.dword0x1f = (m_str_F2C20ar.dword0x1b << 8) / (m_str_F2C20ar.dword0x27 >> 8);
			v106 = (m_str_F2C20ar.dword0x1b << 8) / (m_str_F2C20ar.dword0x27 >> 8);
			v57x = m_str_F0E20x;
			v58 = (signed int*)(x_DWORD_E9C38_smalltit + 59360);
			v59 = 0;
			v60 = 0;
			v61 = 1;
			v62 = 0;
			do
			{
				v57x->dword_1 = v59;
				v57x->dword_2 = v62;
				v8 = __CFADD__(v106, v60);
				v60 += v106;
				if (v8)
				{
					v59--;
					v62++;
					*v58 = v61;
					v58++;
				}
				//v57 += 3;
				v57x++;
				v59 += v86;
				v61++;
				v98--;
			} while (v98);
			m_str_F2C20ar.dword0x1d = v62;
			m_str_F2C20ar.dword0x21 = -v62;
		}
		m_str_F2C20ar.width0x25 = (unsigned __int16)viewPortWidth;
		m_str_F2C20ar.height0x26 = (unsigned __int16)viewPortHeight;
		m_str_F2C20ar.dword0x1c = (unsigned __int16)viewPortHeight + (unsigned __int16)viewPortWidth;
		m_str_F2C20ar.dword0x23 = 1;
		m_str_F2C20ar.Height_0x19 = (unsigned __int16)viewPortWidth + m_str_F2C20ar.dword0x21;
		m_str_F2C20ar.pbyte0x1a = (4 * (-1 - m_str_F2C20ar.dword0x21) + &x_DWORD_E9C38_smalltit[59360]);
		v95 = (unsigned __int16)viewPortHeight - 1;
		resultx = m_str_F0E20x;
		if (v95 < 0)
			return;
		goto LABEL_68;
	case 6:
		//str_F2C20ar.dword0x27 = *(int32_t*)&x_DWORD_D9F50ar[v1*4];
		m_str_F2C20ar.dword0x27 = Maths::x_DWORD_DB750[v1 - 0x600];//copy to other
		//str_F2C20ar.dword0x1b = *(int32_t*)&x_DWORD_DA750ar[v1*4];
		m_str_F2C20ar.dword0x1b = Maths::x_DWORD_DB750[v1 - 0x400];//copy to other

		v87 = -screenWidth;
		v99 = (unsigned __int16)viewPortHeight;
		m_str_F2C20ar.dword0x1f = (m_str_F2C20ar.dword0x27 << 8) / (m_str_F2C20ar.dword0x1b >> 8);
		v107 = (m_str_F2C20ar.dword0x27 << 8) / (m_str_F2C20ar.dword0x1b >> 8);
		v63x = m_str_F0E20x;
		v64 = (signed int*)&x_DWORD_E9C38_smalltit[59360];
		v65 = 0;
		v66 = 0;
		v67 = 1;
		v68 = 0;
		do
		{
			v63x->dword_1 = v65;
			v63x->dword_2 = v68;
			v8 = __CFADD__(v107, v66);
			v66 += v107;
			if (v8)
			{
				v65++;
				v68++;
				*v64 = v67;
				v64++;
			}
			//v63 += 3;
			v63x++;
			v65 += v87;
			v67++;
			v99--;
		} while (v99);
		m_str_F2C20ar.dword0x1d = v68;
		m_str_F2C20ar.dword0x21 = -v68;
		m_str_F2C20ar.Height_0x19 = (unsigned __int16)viewPortWidth - v68;
		m_str_F2C20ar.width0x25 = (unsigned __int16)viewPortWidth;
		m_str_F2C20ar.dword0x1c = (unsigned __int16)viewPortHeight + (unsigned __int16)viewPortWidth;
		m_str_F2C20ar.height0x26 = (unsigned __int16)viewPortHeight;
		m_str_F2C20ar.dword0x23 = 1;
		m_str_F2C20ar.pbyte0x1a = (4 * (v68 - 1) + &x_DWORD_E9C38_smalltit[59360]);
		v95 = (unsigned __int16)viewPortHeight - 1;
		resultx = m_str_F0E20x;
		if (v95 < 0)
			return;
		goto LABEL_68;
	case 7:

		//v69 = *(int32_t*)&x_DWORD_D9F50ar[v1*4];
		v69 = Maths::x_DWORD_DB750[v1 - 0x600];//copy to other
		//v70 = *(int32_t*)&x_DWORD_DA750ar[v1*4];
		v70 = Maths::x_DWORD_DB750[v1 - 0x400];//copy to other

		m_str_F2C20ar.dword0x27 = v69;
		m_str_F2C20ar.dword0x1b = v70;
		v91 = -screenWidth;
		v100 = (unsigned __int16)viewPortWidth;
		if (v1 == 1792)
		{
			v71x = m_str_F0E20x;
			v72 = (signed int*)&x_DWORD_E9C38_smalltit[59360];
			m_str_F2C20ar.dword0x1f = 0x10000;
			v73 = 0;
			v74 = 1;
			v75 = 0;
			do
			{
				v71x->dword_1 = v73;
				v71x->dword_2 = v75++;
				v72[0] = v74;
				v72++;
				//v71 += 3;
				v71x++;
				v73 += v91 + 1;
				v74++;
				v100--;
			} while (v100);
			m_str_F2C20ar.dword0x1d = (unsigned __int16)viewPortWidth;
			m_str_F2C20ar.dword0x21 = -(unsigned __int16)viewPortWidth;
		}
		else
		{
			m_str_F2C20ar.dword0x1f = (m_str_F2C20ar.dword0x1b << 8) / (v69 >> 8);
			v108 = (m_str_F2C20ar.dword0x1b << 8) / (v69 >> 8);
			v76x = m_str_F0E20x;
			v77 = (signed int*)(x_DWORD_E9C38_smalltit + 59360);
			v78 = 0;
			v79 = 0;
			v80 = 1;
			v81 = 0;
			do
			{
				v76x->dword_1 = v78;
				v76x->dword_2 = v81;
				v8 = __CFADD__(v108, v79);
				v79 += v108;
				if (v8)
				{
					v78 += v91;
					v81++;
					*v77 = v80;
					v77++;
				}
				//v76 += 3;
				v76x++;
				v78++;
				v80++;
				v100--;
			} while (v100);
			m_str_F2C20ar.dword0x1d = v81;
			m_str_F2C20ar.dword0x21 = -v81;
		}
		m_str_F2C20ar.width0x25 = (unsigned __int16)viewPortHeight;
		m_str_F2C20ar.height0x26 = (unsigned __int16)viewPortWidth;
		m_str_F2C20ar.dword0x1c = (unsigned __int16)viewPortHeight + (unsigned __int16)viewPortWidth;
		m_str_F2C20ar.Height_0x19 = (unsigned __int16)viewPortHeight + m_str_F2C20ar.dword0x21;
		v42x = (4 * (-1 - m_str_F2C20ar.dword0x21) + &x_DWORD_E9C38_smalltit[59360]);
		v9 = screenWidth;
	LABEL_65:
		m_str_F2C20ar.pbyte0x1a = v42x;
	LABEL_66:
		m_str_F2C20ar.dword0x23 = v9;
		break;
	default:
		break;
	}
	v95 = m_str_F2C20ar.height0x26 - 1;
	for (resultx = m_str_F0E20x; v95 >= 0; --v95)
	{
	LABEL_68:
		v82 = resultx->dword_1;
		// FIXME: This is what would have happend in the original memory layout in which 
		//        the array unk_F0A20x is directly located before unk_F0E20x.
		//        But not sure if this is intended. Maybe it becomes clearer when the logic get refactored.
		/*if (resultx == m_str_F0E20x) {
			v83 = *(x_DWORD*)&unk_F0A20x[0x3f8];
		}
		else {
			v83 = *(result - 2);
		}
		result += 3;
		*(result - 3) = v82 - v83;*/
		if (resultx == m_str_F0E20x) {
			v83 = *(x_DWORD*)&unk_F0A20x[0x3f8];
		}
		else {
			v83 = resultx[-1].dword_1;
		}
		resultx->dword_0 = v82 - v83;
		resultx++;
	}
	//return result;
}

void GameRender::DrawSorcererNameAndHealthBar_2CB30(type_event_0x6E8E* a1x, uint8_t x_BYTE_E88E0x[], __int16 a2, int a3, __int16 a4, int16_t viewPortX, int16_t viewPortY, uint16_t viewPortWidth, uint16_t viewPortHeight, uint16_t pitch)
{
	//char *v4; // edi
	char* v5; // esi
	//char v6; // al
	//char v7; // al
	//int v8; // esi
	int v9x; // eax
	int v9y; // eax
	__int16 v10; // bx
	__int16 v11; // bx
	int v12; // edi
	int v13; // esi
	/*unsigned __int8 v14; // ST10_1
	unsigned __int8 v15; // ST10_1
	unsigned __int8 v16; // ST10_1
	unsigned __int8 v17; // ST10_1
	unsigned __int8 v18; // ST10_1
	unsigned __int8 v19; // ST10_1*/
	//char *v21; // [esp-4h] [ebp-5Ch]
	//unsigned __int8 v22; // [esp-4h] [ebp-5Ch]
	//unsigned __int8 v23; // [esp-4h] [ebp-5Ch]
	char v24[32]; // [esp+0h] [ebp-58h]
	int v25; // [esp+20h] [ebp-38h]
	int v26; // [esp+24h] [ebp-34h]
	int v27; // [esp+28h] [ebp-30h]
	int v28; // [esp+2Ch] [ebp-2Ch]
	int v29; // [esp+30h] [ebp-28h]
	int v30; // [esp+34h] [ebp-24h]
	int v31; // [esp+38h] [ebp-20h]
	int v32; // [esp+3Ch] [ebp-1Ch]
	unsigned __int8 v33; // [esp+40h] [ebp-18h]
	char v34; // [esp+44h] [ebp-14h]
	char v35; // [esp+48h] [ebp-10h]
	char v36; // [esp+4Ch] [ebp-Ch]
	char v37; // [esp+50h] [ebp-8h]
	unsigned __int8 v38; // [esp+54h] [ebp-4h]
	int v39; // [esp+74h] [ebp+1Ch]

	// fix if begin
	/*v14 = 0;
	v15 = 0;
	v16 = 0;
	v17 = 0;
	v18 = 0;
	v19 = 0;
	v22 = 0;
	v23 = 0;*/
	// end

	v31 = viewPortWidth + viewPortX - 4;
	v29 = viewPortHeight + viewPortY - 22;
	v25 = a1x->dword_0xA4_164x->word_0x38_56;
	//v4 = v24;
	//D41A0_BYTESTR_0.array_0x2BDE[v25].array_0x39f_2BFA_12157
	v5 = D41A0_0.array_0x2BDE[v25].array_0x39f_2BFA_12157;//wizard name
	//v21 = v24;
	strcpy(v24, v5);
	/*do
	{
		v6 = *v5;
		*v4 = *v5;
		if (!v6)
			break;
		v7 = v5[1];
		v5 += 2;
		v4[1] = v7;
		v4 += 2;
	} while (v7);*/
	v36 = x_BYTE_E88E0x[3 * sub_61790(v25)];//c
	v35 = (*xadataclrd0dat.var28_begin_buffer)[0];//10 //v19
	v34 = x_BYTE_E88E0x[3 * sub_61790(v25)];	//14 //v18
	//v8 = 5 * static_cast<std::underlying_type<MapType_t>::type>(D41A0_BYTESTR_0.terrain_2FECE.MapType);
	v33 = str_D94F0_bldgprmbuffer[static_cast<std::underlying_type<MapType_t>::type>(D41A0_0.terrain_2FECE.MapType)][2];//18 v14
	v38 = str_D94F0_bldgprmbuffer[static_cast<std::underlying_type<MapType_t>::type>(D41A0_0.terrain_2FECE.MapType)][3];//4 v15
	v37 = str_D94F0_bldgprmbuffer[static_cast<std::underlying_type<MapType_t>::type>(D41A0_0.terrain_2FECE.MapType)][0];//?v22
	//v9 = a4 >> 1;
	v10 = (a4 >> 1) + a2;
	if (m_x_WORD_180660_VGA_type_resolution & 1)
	{
		v10 *= 2;
		a3 *= 2;
	}
	//LOWORD(v9) = x_DWORD_EA3CC;
	v11 = viewPortX + v10;
	v12 = viewPortY + a3 - 20;
	v39 = viewPortY + a3 - 20;
	if (v11 >= viewPortX)
	{
		//LOWORD(v9) = v12;
		if ((signed __int16)v12 >= viewPortY && v11 < v31 && (signed __int16)v12 < v29)
		{
			v9x = strlen(v24);
			v13 = 8 * v9x + 4;
			if (v11 + v13 > v31)
			{
				v13 = v31 - v11;
				v9x = ((v31 - v11 - 4) - (my_sign32(v31 - v11 - 4) << 3) + my_sign32(v31 - v11 - 4)) >> 3;
				//v9 = (v31 - v11 - 4 - (__CFSHL__((v31 - v11 - 4) >> 31, 3) + 8 * ((v31 - v11 - 4) >> 31))) >> 3;
			}
			if (v9x > 0)
			{
				v24[v9x] = 0;
				v32 = v13 + 2;
				v26 = (signed __int16)(v13 + 2);
				v30 = v11;
				DrawLine_2BC80(v11, v39, v13 + 2, 18, pitch, v37);//8
				v27 = v33;//30// v16
				DrawLine_2BC80(v30, v39, v26, 2, pitch, v33);//18
				v28 = v38;//2c//v17
				DrawLine_2BC80(v30, v39 + 16, v26, 2, pitch, v38);//4
				DrawLine_2BC80(v30, v39, 2, 16, pitch, v33);//30,tj.18
				DrawLine_2BC80(v11 + v32 - 2, v39, 2, 18, pitch, v38);//2c tj. 4
				DrawText_2BC10(v24, v11 + 4, v39, v34, pitch);//14
				DrawLine_2BC80(v11 + 2, v39 + 14, v13 - 2, 2, pitch, v35);//10
				//LOWORD(v9) = (x_WORD)a1;
				if (a1x->dword_0x4)
				{
					v9y = a1x->dword_0x8 * (v13 - 2) / a1x->dword_0x4;
					if (v30 + 2 + v9y > v31 - 2)
						v9y = v31 - 2 - (v30 + 2);
					if (v9y > 0)
						DrawLine_2BC80(v11 + 2, v39 + 14, v9y, 2, pitch, v36);
				}
			}
		}
	}
	//return v9;
}

void GameRender::StartWorkerThreads(uint8_t numOfThreads, bool assignToSpecificCores)
{
	m_multiThreadRender = true;
	if (m_renderThreads.size() < numOfThreads)
	{
		WaitForRenderFinish();

		for (int i = 0; i < numOfThreads; i++)
		{
			if (assignToSpecificCores)
			{
				StartWorkerThread(i + 1);
			}
			else
			{
				StartWorkerThread();
			}
		}
	}
}

void GameRender::StartWorkerThread()
{
	RenderThread* renderThread = new RenderThread();

	if (renderThread->IsRunning())
	{
		m_renderThreads.push_back(renderThread);
	}
}

void GameRender::StartWorkerThread(int core)
{
	RenderThread* renderThread = new RenderThread(core);

	if (renderThread->IsRunning())
	{
		m_renderThreads.push_back(renderThread);
	}
}

void GameRender::StopWorkerThreads()
{
	if (m_renderThreads.size() > 0)
	{
		for (RenderThread* thread : m_renderThreads) {
			if (thread->IsRunning())
			{
				thread->StopWorkerThread();
				delete thread;
			}
		}
		m_renderThreads.clear();
	}
}

//Coordinates Already transformed into "Screen Space" (x & y, top left 0,0)
void GameRender::DrawSquareInProjectionSpace(int* vertexs, int index, uint16_t viewPortWidth, uint16_t viewPortHeight, uint16_t pitch)
{
	//Set Texture coordinates for polys
	vertexs[20] = xunk_D4350[m_Str_E9C38_smalltit[index].byte42_std][0];
	vertexs[21] = xunk_D4350[m_Str_E9C38_smalltit[index].byte42_std][1];
	vertexs[14] = xunk_D4350[m_Str_E9C38_smalltit[index].byte42_std][2];
	vertexs[15] = xunk_D4350[m_Str_E9C38_smalltit[index].byte42_std][3];
	vertexs[8] = xunk_D4350[m_Str_E9C38_smalltit[index].byte42_std][4];
	vertexs[9] = xunk_D4350[m_Str_E9C38_smalltit[index].byte42_std][5];
	vertexs[2] = xunk_D4350[m_Str_E9C38_smalltit[index].byte42_std][6];
	vertexs[3] = xunk_D4350[m_Str_E9C38_smalltit[index].byte42_std][7];

	//Get Texture
	uint8_t* pTexture = m_textureAddresses.at(m_Str_E9C38_smalltit[index].byte41);

	//Render
	if ((uint8_t)m_Str_E9C38_smalltit[index].word38 & 1)
	{
		if (m_renderThreads.size() > 0)
		{
			uint8_t drawEveryNthLine = m_renderThreads.size() + 1;
			uint8_t i = 0;

			for (i = 0; i < m_renderThreads.size(); i++)
			{
				m_renderThreads[i]->Run([this, &vertexs, pTexture, viewPortWidth, viewPortHeight, pitch, i, drawEveryNthLine] {
					auto vertex0 = ProjectionPolygon(&vertexs[0]);
					auto vertex6 = ProjectionPolygon(&vertexs[6]);
					auto vertex12 = ProjectionPolygon(&vertexs[12]);
					auto vertex18 = ProjectionPolygon(&vertexs[18]);
					this->DrawTriangleInProjectionSpace_B6253(&vertex18, &vertex12, &vertex0, pTexture, unk_DE56Cx[i], viewPortWidth, viewPortHeight, pitch, i, drawEveryNthLine);
					this->DrawTriangleInProjectionSpace_B6253(&vertex0, &vertex12, &vertex6, pTexture, unk_DE56Cx[i], viewPortWidth, viewPortHeight, pitch, i, drawEveryNthLine);
					});
			}

			auto vertex0 = ProjectionPolygon(&vertexs[0]);
			auto vertex6 = ProjectionPolygon(&vertexs[6]);
			auto vertex12 = ProjectionPolygon(&vertexs[12]);
			auto vertex18 = ProjectionPolygon(&vertexs[18]);
			DrawTriangleInProjectionSpace_B6253(&vertex18, &vertex12, &vertex0, pTexture, unk_DE56Cx[m_renderThreads.size()], viewPortWidth, viewPortHeight, pitch, i, drawEveryNthLine);
			DrawTriangleInProjectionSpace_B6253(&vertex0, &vertex12, &vertex6, pTexture, unk_DE56Cx[m_renderThreads.size()], viewPortWidth, viewPortHeight, pitch, i, drawEveryNthLine);

			WaitForRenderFinish();
		}
		else
		{
			auto vertex0 = ProjectionPolygon(&vertexs[0]);
			auto vertex6 = ProjectionPolygon(&vertexs[6]);
			auto vertex12 = ProjectionPolygon(&vertexs[12]);
			auto vertex18 = ProjectionPolygon(&vertexs[18]);
			DrawTriangleInProjectionSpace_B6253(&vertex18, &vertex12, &vertex0, pTexture, unk_DE56Cx[0], viewPortWidth, viewPortHeight, pitch, 0, 1);
			DrawTriangleInProjectionSpace_B6253(&vertex0, &vertex12, &vertex6, pTexture, unk_DE56Cx[0], viewPortWidth, viewPortHeight, pitch, 0, 1);
		}
	}
	else
	{
		if (m_renderThreads.size() > 0)
		{
			uint8_t drawEveryNthLine = m_renderThreads.size() + 1;
			uint8_t i = 0;

			for (i = 0; i < m_renderThreads.size(); i++)
			{
				m_renderThreads[i]->Run([this, &vertexs, pTexture, viewPortWidth, viewPortHeight, pitch, i, drawEveryNthLine] {
					auto vertex0 = ProjectionPolygon(&vertexs[0]);
					auto vertex6 = ProjectionPolygon(&vertexs[6]);
					auto vertex12 = ProjectionPolygon(&vertexs[12]);
					auto vertex18 = ProjectionPolygon(&vertexs[18]);
					this->DrawTriangleInProjectionSpace_B6253(&vertex18, &vertex12, &vertex6, pTexture, unk_DE56Cx[i], viewPortWidth, viewPortHeight, pitch, i, drawEveryNthLine);
					this->DrawTriangleInProjectionSpace_B6253(&vertex18, &vertex6, &vertex0, pTexture, unk_DE56Cx[i], viewPortWidth, viewPortHeight, pitch, i, drawEveryNthLine);
					});
			}

			auto vertex0 = ProjectionPolygon(&vertexs[0]);
			auto vertex6 = ProjectionPolygon(&vertexs[6]);
			auto vertex12 = ProjectionPolygon(&vertexs[12]);
			auto vertex18 = ProjectionPolygon(&vertexs[18]);
			DrawTriangleInProjectionSpace_B6253(&vertex18, &vertex12, &vertex6, pTexture, unk_DE56Cx[m_renderThreads.size()], viewPortWidth, viewPortHeight, pitch, i, drawEveryNthLine);
			DrawTriangleInProjectionSpace_B6253(&vertex18, &vertex6, &vertex0, pTexture, unk_DE56Cx[m_renderThreads.size()], viewPortWidth, viewPortHeight, pitch, i, drawEveryNthLine);

			WaitForRenderFinish();
		}
		else
		{
			auto vertex0 = ProjectionPolygon(&vertexs[0]);
			auto vertex6 = ProjectionPolygon(&vertexs[6]);
			auto vertex12 = ProjectionPolygon(&vertexs[12]);
			auto vertex18 = ProjectionPolygon(&vertexs[18]);
			DrawTriangleInProjectionSpace_B6253(&vertex18, &vertex12, &vertex6, pTexture, unk_DE56Cx[0], viewPortWidth, viewPortHeight, pitch, 0, 1);
			DrawTriangleInProjectionSpace_B6253(&vertex18, &vertex6, &vertex0, pTexture, unk_DE56Cx[0], viewPortWidth, viewPortHeight, pitch, 0, 1);
		}
	}
}

void GameRender::DrawInverseSquareInProjectionSpace(int* vertexs, int index, uint16_t viewPortWidth, uint16_t viewPortHeight, uint16_t pitch)
{
	DrawInverseSquareInProjectionSpace(vertexs, index, m_textureAddresses.at(m_Str_E9C38_smalltit[index].byte41), viewPortWidth, viewPortHeight, pitch);
}

void GameRender::DrawInverseSquareInProjectionSpace(int* vertexs, int index, uint8_t* pTexture, uint16_t viewPortWidth, uint16_t viewPortHeight, uint16_t pitch)
{
	//Set Texture coordinates for polys
	vertexs[20] = xunk_D4350[m_Str_E9C38_smalltit[index].byte42_std][0];
	vertexs[21] = xunk_D4350[m_Str_E9C38_smalltit[index].byte42_std][1];
	vertexs[14] = xunk_D4350[m_Str_E9C38_smalltit[index].byte42_std][2];
	vertexs[15] = xunk_D4350[m_Str_E9C38_smalltit[index].byte42_std][3];
	vertexs[8] = xunk_D4350[m_Str_E9C38_smalltit[index].byte42_std][4];
	vertexs[9] = xunk_D4350[m_Str_E9C38_smalltit[index].byte42_std][5];
	vertexs[2] = xunk_D4350[m_Str_E9C38_smalltit[index].byte42_std][6];
	vertexs[3] = xunk_D4350[m_Str_E9C38_smalltit[index].byte42_std][7];
	x_BYTE_E126D = 5;

	//Render
	if (m_Str_E9C38_smalltit[index].word38 & 1)
	{
		if (m_renderThreads.size() > 0)
		{
			uint8_t drawEveryNthLine = m_renderThreads.size() + 1;
			uint8_t i = 0;

			for (i = 0; i < m_renderThreads.size(); i++)
			{
				m_renderThreads[i]->Run([this, &vertexs, pTexture, viewPortWidth, viewPortHeight, pitch, i, drawEveryNthLine] {
					auto vertex0 = ProjectionPolygon(&vertexs[0]);
					auto vertex6 = ProjectionPolygon(&vertexs[6]);
					auto vertex12 = ProjectionPolygon(&vertexs[12]);
					auto vertex18 = ProjectionPolygon(&vertexs[18]);
					this->DrawTriangleInProjectionSpace_B6253(&vertex18, &vertex0, &vertex12, pTexture, unk_DE56Cx[i], viewPortWidth, viewPortHeight, pitch, i, drawEveryNthLine);
					this->DrawTriangleInProjectionSpace_B6253(&vertex0, &vertex6, &vertex12, pTexture, unk_DE56Cx[i], viewPortWidth, viewPortHeight, pitch, i, drawEveryNthLine);
					});
			}

			auto vertex0 = ProjectionPolygon(&vertexs[0]);
			auto vertex6 = ProjectionPolygon(&vertexs[6]);
			auto vertex12 = ProjectionPolygon(&vertexs[12]);
			auto vertex18 = ProjectionPolygon(&vertexs[18]);
			DrawTriangleInProjectionSpace_B6253(&vertex18, &vertex0, &vertex12, pTexture, unk_DE56Cx[m_renderThreads.size()], viewPortWidth, viewPortHeight, pitch, i, drawEveryNthLine);
			DrawTriangleInProjectionSpace_B6253(&vertex0, &vertex6, &vertex12, pTexture, unk_DE56Cx[m_renderThreads.size()], viewPortWidth, viewPortHeight, pitch, i, drawEveryNthLine);

			WaitForRenderFinish();
		}
		else
		{
			auto vertex0 = ProjectionPolygon(&vertexs[0]);
			auto vertex6 = ProjectionPolygon(&vertexs[6]);
			auto vertex12 = ProjectionPolygon(&vertexs[12]);
			auto vertex18 = ProjectionPolygon(&vertexs[18]);
			DrawTriangleInProjectionSpace_B6253(&vertex18, &vertex0, &vertex12, pTexture, unk_DE56Cx[0], viewPortWidth, viewPortHeight, pitch, 0, 1);
			DrawTriangleInProjectionSpace_B6253(&vertex0, &vertex6, &vertex12, pTexture, unk_DE56Cx[0], viewPortWidth, viewPortHeight, pitch, 0, 1);
		}

	}
	else
	{
		if (m_renderThreads.size() > 0)
		{
			uint8_t drawEveryNthLine = m_renderThreads.size() + 1;
			uint8_t i = 0;

			for (i = 0; i < m_renderThreads.size(); i++)
			{
				m_renderThreads[i]->Run([this, &vertexs, pTexture, viewPortWidth, viewPortHeight, pitch, i, drawEveryNthLine] {
					auto vertex0 = ProjectionPolygon(&vertexs[0]);
					auto vertex6 = ProjectionPolygon(&vertexs[6]);
					auto vertex12 = ProjectionPolygon(&vertexs[12]);
					auto vertex18 = ProjectionPolygon(&vertexs[18]);
					this->DrawTriangleInProjectionSpace_B6253(&vertex18, &vertex6, &vertex12, pTexture, unk_DE56Cx[i], viewPortWidth, viewPortHeight, pitch, i, drawEveryNthLine);
					this->DrawTriangleInProjectionSpace_B6253(&vertex18, &vertex0, &vertex6, pTexture, unk_DE56Cx[i], viewPortWidth, viewPortHeight, pitch, i, drawEveryNthLine);
					});
			}

			auto vertex0 = ProjectionPolygon(&vertexs[0]);
			auto vertex6 = ProjectionPolygon(&vertexs[6]);
			auto vertex12 = ProjectionPolygon(&vertexs[12]);
			auto vertex18 = ProjectionPolygon(&vertexs[18]);
			DrawTriangleInProjectionSpace_B6253(&vertex18, &vertex6, &vertex12, pTexture, unk_DE56Cx[m_renderThreads.size()], viewPortWidth, viewPortHeight, pitch, i, drawEveryNthLine);
			DrawTriangleInProjectionSpace_B6253(&vertex18, &vertex0, &vertex6, pTexture, unk_DE56Cx[m_renderThreads.size()], viewPortWidth, viewPortHeight, pitch, i, drawEveryNthLine);

			WaitForRenderFinish();
		}
		else
		{
			auto vertex0 = ProjectionPolygon(&vertexs[0]);
			auto vertex6 =  ProjectionPolygon(&vertexs[6]);
			auto vertex12 = ProjectionPolygon(&vertexs[12]);
			auto vertex18 = ProjectionPolygon(&vertexs[18]);
			DrawTriangleInProjectionSpace_B6253(&vertex18, &vertex6, &vertex12, pTexture, unk_DE56Cx[0], viewPortWidth, viewPortHeight, pitch, 0, 1);
			DrawTriangleInProjectionSpace_B6253(&vertex18, &vertex0, &vertex6, pTexture, unk_DE56Cx[0], viewPortWidth, viewPortHeight, pitch, 0, 1);
		}
	}
}

void GameRender::DrawParticles_3E360(int a2x, type_particle_str** str_DWORD_F66F0x[], uint8_t x_BYTE_E88E0x[], int32_t x_DWORD_F5730[], type_event_0x6E8E* x_DWORD_EA3E4[], type_str_unk_1804B0ar str_unk_1804B0ar, ViewPort viewPort, uint16_t screenWidth)
{
	unsigned __int16 result; // ax
	type_event_0x6E8E* v3x; // eax
	__int16 v4; // cx
	int v5; // ecx
	int v6; // edx
	type_WORD_D951C* v7x; // edi
	int v8; // ecx
	int v9; // ST18_4
	char v10; // al
	//int v11; // ebx
	//int v12; // eax
	//int v13; // ebx
	//int v14; // ebx
	//int v15; // eax
	//int v16; // ebx
	int v17; // ebx
	int v18; // ebx
	int v19; // eax
	int v20; // eax
	int v21; // eax
	int v22; // edx
	int v23; // eax
	int v24; // eax
	int v25; // ebx
	//int v26; // edx
	unsigned __int16 v27; // ax
	int v28; // eax
	//int v29; // edx
	unsigned __int16 v30; // ax
	int v31; // eax
	int v32; // ebx
	int v33; // eax
	//int v34; // edx
	int v35; // eax
	int v36; // eax
	int v37; // ebx
	int v38; // edx
	int v39; // eax
	int v40; // eax
	int v41; // eax
	int v42; // eax
	int v43; // ebx
	//char v44; // ch
	unsigned __int8 v45; // al
	int v46; // ecx
	int v47; // eax
	int v48; // eax
	int v49; // ecx
	type_D404C* v50x; // ebx
	int v51; // edx
	type_WORD_D951C* v52x; // edi
	int v53; // ecx
	int v54; // ST1C_4
	char v55; // al
	//int v56; // ebx
	//int v57; // eax
	//int v58; // ebx
	int v59; // ebx
	//uint32_t* v60; // esi
	int v61; // ebx
	int v62; // ebx
	int v63; // eax
	//int v64; // edx
	unsigned __int16 v65; // ax
	int v66; // eax
	int v67; // eax
	int v68; // eax
	//int v69; // edx
	int v70; // eax
	int v71; // eax
	int v72; // ebx
	int v73; // eax
	//int v74; // edx
	int v75; // eax
	int v76; // eax
	int v77; // eax
	int v78; // eax
	int v79; // ebx
	int v80; // eax
	int v81; // eax
	int v82; // ebx
	int v83; // edx
	int v84; // eax
	int v85; // eax
	int v86; // eax
	//int v87; // edx
	unsigned __int16 v88; // ax
	int v89; // eax
	type_event_0x6E8E* v90x; // ebx
	__int16 v91; // cx
	unsigned __int8 v92; // al
	char v93; // cl
	int v94; // eax
	int v95; // eax
	int v96; // [esp+0h] [ebp-20h]
	int v97; // [esp+8h] [ebp-18h]
	int v98; // [esp+10h] [ebp-10h]
	int v99; // [esp+18h] [ebp-8h]
	int v100; // [esp+1Ch] [ebp-4h]

	type_particle_str* a1y = NULL;
	//fix

	result = m_Str_E9C38_smalltit[a2x].word36;
	do
	{
		//adress 21f370

		v3x = x_DWORD_EA3E4[result];
		m_str_F2C20ar.dword0x14x = v3x;
		if (!(v3x->struct_byte_0xc_12_15.byte[0] & 0x21))
		{
			v4 = v3x->axis_0x4C_76.y;
			v96 = (signed __int16)(v3x->axis_0x4C_76.x - x_WORD_F2CC4);
			v97 = (signed __int16)(x_WORD_F2CC2 - v4);
			if (x_BYTE_F2CC7)
			{
				if (!m_Str_E9C38_smalltit[a2x].byte43 && !(v3x->struct_byte_0xc_12_15.word[1] & 0x808))
				{
					//adress 21f40c
					v98 = sub_B5C60_getTerrainAlt2(v3x->axis_0x4C_76.x, v4) - m_str_F2C20ar.dword0x20;
					v5 = (m_str_F2C20ar.dword0x0f * v96 - m_str_F2C20ar.dword0x17 * v97) >> 16;
					v99 = (m_str_F2C20ar.dword0x17 * v96 + m_str_F2C20ar.dword0x0f * v97) >> 16;
					v6 = v99 * v99 + v5 * v5;
					if (v99 > 64 && v6 < m_str_F2C20ar.dword0x15)
					{
						if (v6 <= m_str_F2C20ar.dword0x13)
							m_str_F2C20ar.dword0x00 = 0x2000;
						else
							m_str_F2C20ar.dword0x00 = v6 < m_str_F2C20ar.dword0x16 ? 32 * (m_str_F2C20ar.dword0x16 - (v99 * v99 + v5 * v5)) / m_str_F2C20ar.dword0x12 << 8 : 0;
						//v7 = (short*)&x_WORD_D951C[7 * str_F2C20ar.dword0x14x->word_0x5A_90];
						v7x = &str_WORD_D951C[m_str_F2C20ar.dword0x14x->word_0x5A_90];
						if (!v7x->byte_10)
						{
							v8 = v5 * m_str_F2C20ar.dword0x18 / v99;
							v9 = m_str_F2C20ar.dword0x18 * v98 / v99 + m_str_F2C20ar.dword0x22;
							m_str_F2C20ar.dword0x04_screenY = ((v8 * m_str_F2C20ar.dword0x11 - m_str_F2C20ar.dword0x0d * v9) >> 16) + m_str_F2C20ar.dword0x24;
							m_str_F2C20ar.dword0x03_screenX = m_str_F2C20ar.dword0x10 - ((m_str_F2C20ar.dword0x0d * v8 + v9 * m_str_F2C20ar.dword0x11) >> 16);
							v10 = v7x->byte_12;
							x_BYTE_F2CC6 = 0;
							switch (v10)
							{
							case 0:
								//v11 = v7x->word_0;
								if (str_DWORD_F66F0x[v7x->word_0])
								{
									//v12 = v7x->word_0;
									//v13 = 4 * v7x->word_0;
								}
								else
								{
									if (!MainInitTmaps_71520(v7x->word_0))
										goto LABEL_178;
									//v12 = v7x->word_0;
									//v13 = 4 * v12;
								}
								x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v7x->word_0].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
								a1y = *str_DWORD_F66F0x[v7x->word_0];
								goto LABEL_51;
							case 1:
								if (!str_DWORD_F66F0x[v7x->word_0] && !MainInitTmaps_71520(v7x->word_0))
									goto LABEL_178;
								x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v7x->word_0].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
								a1y = *str_DWORD_F66F0x[v7x->word_0];
								goto LABEL_51;
							case 2:
							case 3:
							case 4:
							case 5:
							case 6:
							case 7:
							case 8:
							case 9:
							case 10:
							case 11:
							case 12:
							case 13:
							case 14:
							case 15:
							case 16:
								goto LABEL_29;
							case 17:
								v25 = (((m_str_F2C20ar.dword0x14x->word_0x1C_28 - (unsigned __int16)x_WORD_F2CC0) >> 3) & 0xF0) >> 4;
								if (v25 < 8)
								{
									if (str_DWORD_F66F0x[v25 + v7x->word_0])
									{
										//v26 = (int)x_D41A0_BYTEARRAY_4;
										v27 = str_TMAPS00TAB_BEGIN_BUFFER[v25 + v7x->word_0].word_8;
									}
									else
									{
										if (!MainInitTmaps_71520(v25 + v7x->word_0))
											goto LABEL_178;
										//v26 = (int)x_D41A0_BYTEARRAY_4;
										v27 = str_TMAPS00TAB_BEGIN_BUFFER[v25 + v7x->word_0].word_8;
									}
									x_DWORD_F5730[v27] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
									a1y = *str_DWORD_F66F0x[v25 + v7x->word_0];
									goto LABEL_51;
								}
								if (str_DWORD_F66F0x[v7x->word_0 + 15 - v25])
								{
									//v29 = (int)x_D41A0_BYTEARRAY_4;
									v30 = str_TMAPS00TAB_BEGIN_BUFFER[v7x->word_0 + 15 - v25].word_8;
								}
								else
								{
									if (!MainInitTmaps_71520(v7x->word_0 + 15 - v25))
										goto LABEL_178;
									//v29 = (int)x_D41A0_BYTEARRAY_4;
									v30 = str_TMAPS00TAB_BEGIN_BUFFER[v7x->word_0 + 15 - v25].word_8;
								}
								x_DWORD_F5730[v30] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
								a1y = *str_DWORD_F66F0x[v7x->word_0 + 15 - v25];
								m_str_F2C20ar.dword0x08_width = a1y->width;
								m_str_F2C20ar.dword0x06_height = a1y->height;
								v31 = (signed __int64)(m_str_F2C20ar.dword0x18 * v7x->rotSpeed_8) / v99;
								m_str_F2C20ar.dword0x0c_realHeight = v31;
								m_str_F2C20ar.dword0x09_realWidth = v31 * m_str_F2C20ar.dword0x08_width / m_str_F2C20ar.dword0x06_height;
								v21 = -m_str_F2C20ar.dword0x08_width;
								goto LABEL_72;
							case 18:
								v32 = (((m_str_F2C20ar.dword0x14x->word_0x1C_28 - (unsigned __int16)x_WORD_F2CC0) >> 3) & 0xF0) >> 4;
								v33 = v32 + v7x->word_0;
								if (str_DWORD_F66F0x[v33])
								{
									v35 = str_TMAPS00TAB_BEGIN_BUFFER[v33].word_8;
									//v34 = (int)x_D41A0_BYTEARRAY_4;
								}
								else
								{
									if (!MainInitTmaps_71520(v32 + v7x->word_0))
										goto LABEL_178;
									//v34 = (int)x_D41A0_BYTEARRAY_4;
									v35 = str_TMAPS00TAB_BEGIN_BUFFER[v32 + v7x->word_0].word_8;
								}
								x_DWORD_F5730[v35] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
								a1y = *str_DWORD_F66F0x[v32 + v7x->word_0];
								m_str_F2C20ar.dword0x08_width = a1y->width;
								m_str_F2C20ar.dword0x06_height = a1y->height;
								v36 = (signed __int64)(m_str_F2C20ar.dword0x18 * v7x->rotSpeed_8) / v99;
								m_str_F2C20ar.dword0x0c_realHeight = v36;
								m_str_F2C20ar.dword0x09_realWidth = v36 * m_str_F2C20ar.dword0x08_width / m_str_F2C20ar.dword0x06_height;
								v21 = m_str_F2C20ar.dword0x08_width;
								goto LABEL_72;
							case 19:
								v18 = (((m_str_F2C20ar.dword0x14x->word_0x1C_28 - (unsigned __int16)x_WORD_F2CC0) >> 3) & 0xF0) >> 4;
								if (v18 >= 8)
								{
									v22 = (unsigned __int8)x_BYTE_D4750[0xc + v18];
									v23 = v22 + v7x->word_0;
									if (str_DWORD_F66F0x[v23])
									{
										x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v23].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
									}
									else
									{
										if (!MainInitTmaps_71520(v7x->word_0 + (unsigned __int8)v22))
											goto LABEL_178;
										x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v7x->word_0 + (unsigned __int8)x_BYTE_D4750[0xc + v18]].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
									}
									a1y = *str_DWORD_F66F0x[v7x->word_0 + (unsigned __int8)x_BYTE_D4750[0xc + v18]];
									m_str_F2C20ar.dword0x08_width = a1y->width;
									m_str_F2C20ar.dword0x06_height = a1y->height;
									v24 = (signed __int64)(m_str_F2C20ar.dword0x18 * v7x->rotSpeed_8) / v99;
									m_str_F2C20ar.dword0x0c_realHeight = v24;
									m_str_F2C20ar.dword0x09_realWidth = v24 * m_str_F2C20ar.dword0x08_width / m_str_F2C20ar.dword0x06_height;
									v21 = -m_str_F2C20ar.dword0x08_width;
								}
								else
								{
									v19 = (unsigned __int8)x_BYTE_D4750[0xc + v18] + v7x->word_0;
									if (str_DWORD_F66F0x[v19])
									{
										x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v19].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
									}
									else
									{
										if (!MainInitTmaps_71520(v7x->word_0 + (unsigned __int8)x_BYTE_D4750[0xc + v18]))
											goto LABEL_178;
										x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v7x->word_0 + (unsigned __int8)x_BYTE_D4750[0xc + v18]].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
									}
									a1y = *str_DWORD_F66F0x[v7x->word_0 + (unsigned __int8)x_BYTE_D4750[0xc + v18]];
									m_str_F2C20ar.dword0x08_width = a1y->width;
									m_str_F2C20ar.dword0x06_height = a1y->height;
									v20 = (signed __int64)(m_str_F2C20ar.dword0x18 * v7x->rotSpeed_8) / v99;
									m_str_F2C20ar.dword0x0c_realHeight = v20;
									m_str_F2C20ar.dword0x09_realWidth = v20 * m_str_F2C20ar.dword0x08_width / m_str_F2C20ar.dword0x06_height;
									v21 = m_str_F2C20ar.dword0x08_width;
								}
								goto LABEL_72;
							case 20:
								v37 = (((m_str_F2C20ar.dword0x14x->word_0x1C_28 - (unsigned __int16)x_WORD_F2CC0) >> 3) & 0xF0) >> 4;
								if (v37 >= 8)
								{
									v41 = v7x->word_0 + (unsigned __int8)x_BYTE_D4750[0x1c + v37];
									if (!str_DWORD_F66F0x[v41])
									{
										if (!MainInitTmaps_71520(v7x->word_0 + (unsigned __int8)x_BYTE_D4750[0x1c + v37]))
											goto LABEL_178;
										v41 = (unsigned __int8)x_BYTE_D4750[0x1c + v37] + v7x->word_0;
									}
									x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v41].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
									a1y = *str_DWORD_F66F0x[v7x->word_0 + (unsigned __int8)x_BYTE_D4750[0x1c + v37]];
									m_str_F2C20ar.dword0x08_width = a1y->width;
									m_str_F2C20ar.dword0x06_height = a1y->height;
									v42 = (signed __int64)(m_str_F2C20ar.dword0x18 * v7x->rotSpeed_8) / v99;
									m_str_F2C20ar.dword0x0c_realHeight = v42;
									m_str_F2C20ar.dword0x09_realWidth = v42 * m_str_F2C20ar.dword0x08_width / m_str_F2C20ar.dword0x06_height;
									v21 = -m_str_F2C20ar.dword0x08_width;
								}
								else
								{
									v38 = (unsigned __int8)x_BYTE_D4750[0x1c + v37];
									v39 = v38 + v7x->word_0;
									if (str_DWORD_F66F0x[v39])
									{
										x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v39].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
									}
									else
									{
										if (!MainInitTmaps_71520(v7x->word_0 + (unsigned __int8)v38))
											goto LABEL_178;
										x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v7x->word_0 + (unsigned __int8)x_BYTE_D4750[0x1c + v37]].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
									}
									a1y = *str_DWORD_F66F0x[v7x->word_0 + (unsigned __int8)x_BYTE_D4750[0x1c + v37]];
									m_str_F2C20ar.dword0x08_width = a1y->width;
									m_str_F2C20ar.dword0x06_height = a1y->height;
									v40 = (signed __int64)(m_str_F2C20ar.dword0x18 * v7x->rotSpeed_8) / v99;
									m_str_F2C20ar.dword0x0c_realHeight = v40;
									m_str_F2C20ar.dword0x09_realWidth = v40 * m_str_F2C20ar.dword0x08_width / m_str_F2C20ar.dword0x06_height;
									v21 = m_str_F2C20ar.dword0x08_width;
								}
								goto LABEL_72;
							case 21:
								//v14 = v7x->word_0;
								if (str_DWORD_F66F0x[v7x->word_0])
								{
									//v15 = v7x->word_0;
									//v16 = 4 * v7x->word_0;
								}
								else
								{
									if (!MainInitTmaps_71520(v7x->word_0))
										goto LABEL_178;
									//v15 = v7x->word_0;
									//v16 = 4 * v15;
								}
								x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v7x->word_0].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
								a1y = *str_DWORD_F66F0x[v7x->word_0];
								goto LABEL_51;
							case 22:
							case 23:
							case 24:
							case 25:
							case 26:
							case 27:
							case 28:
							case 29:
							case 30:
							case 31:
							case 32:
							case 33:
							case 34:
							case 35:
							case 36:
								x_BYTE_F2CC6 = 1;
							LABEL_29:
								v17 = v7x->word_0 + m_str_F2C20ar.dword0x14x->byte_0x5C_92;
								if (str_DWORD_F66F0x[v17])
								{
									x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v17].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
								}
								else
								{
									if (!MainInitTmaps_71520(v7x->word_0 + m_str_F2C20ar.dword0x14x->byte_0x5C_92))
										goto LABEL_178;
									x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v7x->word_0 + m_str_F2C20ar.dword0x14x->byte_0x5C_92].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
								}
								a1y = (type_particle_str*)*str_DWORD_F66F0x[v7x->word_0 + m_str_F2C20ar.dword0x14x->byte_0x5C_92];
							LABEL_51:
								m_str_F2C20ar.dword0x08_width = a1y->width;
								m_str_F2C20ar.dword0x06_height = a1y->height;
								v28 = (signed __int64)(m_str_F2C20ar.dword0x18 * v7x->rotSpeed_8) / v99;
								m_str_F2C20ar.dword0x0c_realHeight = v28;
								m_str_F2C20ar.dword0x09_realWidth = v28 * m_str_F2C20ar.dword0x08_width / m_str_F2C20ar.dword0x06_height;
								v21 = m_str_F2C20ar.dword0x08_width;
							LABEL_72:
								m_str_F2C20ar.dword0x05 = v21;
							LABEL_73:
								v43 = m_str_F2C20ar.dword0x00;
								m_str_F2C20ar.dword0x02_data = a1y->textureBuffer;
								/*v44 = a1y->un_0.byte[0] | 8;
								a1y->un_0.byte[0] = v44;*/
								a1y->word_0 |= 8;
								if (v43 == 0x2000)
									v45 = x_BYTE_D4750[v7x->byte_10];
								else
									v45 = x_BYTE_D4750[6 + v7x->byte_10];
								m_str_F2C20ar.dword0x01_rotIdx = v45;
								v46 = m_str_F2C20ar.dword0x0c_realHeight >> 2;
								m_str_F2C20ar.dword0x0c_realHeight >>= 2;
								if (m_str_F2C20ar.dword0x09_realWidth > 0 && v46 > 0)
								{
									v47 = m_str_F2C20ar.dword0x00 >> 2;
									if (x_BYTE_D4320)
										m_str_F2C20ar.dword0x00 = 0x2000 - v47;
									else
										m_str_F2C20ar.dword0x00 = v47 + 0x2000;
									m_str_F2C20ar.dword0x01_rotIdx = 8;
									DrawSprite_41BD3(0, x_BYTE_E88E0x, x_DWORD_EA3E4, str_unk_1804B0ar, viewPort, screenWidth);
								}
								break;
							default:
								goto LABEL_73;
							}
						}
					}
				}
			}
			if (m_str_F2C20ar.dword0x14x->struct_byte_0xc_12_15.byte[3] >= 0)
				v48 = m_str_F2C20ar.dword0x14x->axis_0x4C_76.z;
			else
				v48 = m_str_F2C20ar.dword0x14x->axis_0x4C_76.z - 160;
			v100 = (m_str_F2C20ar.dword0x17 * v96 + m_str_F2C20ar.dword0x0f * v97) >> 16;
			v49 = (m_str_F2C20ar.dword0x0f * v96 - m_str_F2C20ar.dword0x17 * v97) >> 16;
			if (m_str_F2C20ar.dword0x14x->struct_byte_0xc_12_15.byte[3] & 0x20)
			{
				//v50 = (signed __int16*)((char*)ar_D404C + 22 * str_F2C20ar.dword0x14x->byte_0x3B_59);
				v50x = &str_D404C[m_str_F2C20ar.dword0x14x->byte_0x3B_59];
				switch ((((x_DWORD_EA3E4[m_str_F2C20ar.dword0x14x->word_0x32_50]->word_0x1C_28
					- (unsigned __int16)x_WORD_F2CC0) >> 3) & 0xF0) >> 4)
				{
				case 0:
				case 15:
					v100 -= v50x->word_16;
					break;
				case 1:
				case 14:
					v100 -= v50x->word_18;
					break;
				case 2:
				case 13:
					v100 -= v50x->word_20;
					break;
				case 5:
				case 10:
					v100 += v50x->word_20;
					break;
				case 6:
				case 9:
					v100 += v50x->word_18;
					break;
				case 7:
				case 8:
					v100 += v50x->word_16;
					break;
				default:
					break;
				}
			}
			v51 = v100 * v100 + v49 * v49;
			if (v100 > 64 && v51 < m_str_F2C20ar.dword0x15)
			{
				if (v51 <= m_str_F2C20ar.dword0x13)
				{
					m_str_F2C20ar.dword0x00 = 0x2000;
				}
				else if (v51 < m_str_F2C20ar.dword0x16)
				{
					m_str_F2C20ar.dword0x00 = 32 * (m_str_F2C20ar.dword0x16 - (v100 * v100 + v49 * v49)) / m_str_F2C20ar.dword0x12 << 8;
				}
				else
				{
					m_str_F2C20ar.dword0x00 = 0;
				}
				//v52 = (short*)&x_WORD_D951C[7 * str_F2C20ar.dword0x14x->word_0x5A_90];
				v52x = &str_WORD_D951C[m_str_F2C20ar.dword0x14x->word_0x5A_90];
				v53 = v49 * m_str_F2C20ar.dword0x18 / v100;
				v54 = m_str_F2C20ar.dword0x18 * (v48 - m_str_F2C20ar.dword0x20) / v100 + m_str_F2C20ar.dword0x22;
				m_str_F2C20ar.dword0x04_screenY = ((v53 * m_str_F2C20ar.dword0x11 - m_str_F2C20ar.dword0x0d * v54) >> 16) + m_str_F2C20ar.dword0x24;
				m_str_F2C20ar.dword0x03_screenX = m_str_F2C20ar.dword0x10 - ((m_str_F2C20ar.dword0x0d * v53 + v54 * m_str_F2C20ar.dword0x11) >> 16);
				v55 = v52x->byte_12;
				x_BYTE_F2CC6 = 0;
				switch (v55)
				{
				case 0:
					//v56 = v52x->word_0;
					if (str_DWORD_F66F0x[v52x->word_0])
					{
						//v57 = v52x->word_0;
						//v58 = 4 * v52x->word_0;
						goto LABEL_105;
					}
					if (MainInitTmaps_71520(v52x->word_0))
					{
						//v57 = v52x->word_0;
						//v58 = 4 * v57;
					LABEL_105:
						x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v52x->word_0].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
						a1y = *str_DWORD_F66F0x[v52x->word_0];
						goto LABEL_141;
					}
					break;
				case 1:
					if (str_DWORD_F66F0x[v52x->word_0])
					{
						x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v52x->word_0].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
					}
					else
					{
						if (!MainInitTmaps_71520(v52x->word_0))
							break;
						x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v52x->word_0].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
					}
					a1y = *str_DWORD_F66F0x[v52x->word_0];
					goto LABEL_141;
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
				case 15:
				case 16:
					goto LABEL_117;
				case 17:
					v72 = (((m_str_F2C20ar.dword0x14x->word_0x1C_28 - (unsigned __int16)x_WORD_F2CC0) >> 3) & 0xF0) >> 4;
					if (m_str_F2C20ar.dword0x14x->struct_byte_0xc_12_15.byte[3] & 0x40)
						v72 = (unsigned __int8)x_BYTE_D4750[0x2c + v72];
					if (v72 < 8)
					{
						v73 = v72 + v52x->word_0;
						if (str_DWORD_F66F0x[v73])
						{
							//v74 = (int)x_D41A0_BYTEARRAY_4;							
							v75 = str_TMAPS00TAB_BEGIN_BUFFER[v73].word_8;
						}
						else
						{
							if (!MainInitTmaps_71520(v72 + v52x->word_0))
								break;
							//v74 = (int)x_D41A0_BYTEARRAY_4;
							v75 = str_TMAPS00TAB_BEGIN_BUFFER[v72 + v52x->word_0].word_8;
						}
						x_DWORD_F5730[v75] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
						a1y = *str_DWORD_F66F0x[v52x->word_0 + v72];
						goto LABEL_141;
					}
					v77 = v52x->word_0 + 15 - v72;
					if (str_DWORD_F66F0x[v77])
					{
						x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v77].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
					}
					else
					{
						if (!MainInitTmaps_71520(v52x->word_0 + 15 - v72))
							break;
						x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v52x->word_0 + 15 - v72].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
					}
					a1y = *str_DWORD_F66F0x[v52x->word_0 + 15 - v72];
					m_str_F2C20ar.dword0x08_width = a1y->width;
					m_str_F2C20ar.dword0x06_height = a1y->height;
					v78 = (signed __int64)(m_str_F2C20ar.dword0x18 * v52x->rotSpeed_8) / v100;
					m_str_F2C20ar.dword0x0c_realHeight = v78;
					m_str_F2C20ar.dword0x09_realWidth = v78 * m_str_F2C20ar.dword0x08_width / m_str_F2C20ar.dword0x06_height;
					v67 = -m_str_F2C20ar.dword0x08_width;
					goto LABEL_163;
				case 18:
					v79 = (((m_str_F2C20ar.dword0x14x->word_0x1C_28 - (unsigned __int16)x_WORD_F2CC0) >> 3) & 0xF0) >> 4;
					v80 = v79 + v52x->word_0;
					if (str_DWORD_F66F0x[v80])
					{
						x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v80].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
					}
					else
					{
						if (!MainInitTmaps_71520(v79 + v52x->word_0))
							break;
						x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v79 + v52x->word_0].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
					}
					a1y = *str_DWORD_F66F0x[v79 + v52x->word_0];
					m_str_F2C20ar.dword0x08_width = a1y->width;
					m_str_F2C20ar.dword0x06_height = a1y->height;
					v81 = (signed __int64)(m_str_F2C20ar.dword0x18 * v52x->rotSpeed_8) / v100;
					m_str_F2C20ar.dword0x0c_realHeight = v81;
					m_str_F2C20ar.dword0x09_realWidth = v81 * m_str_F2C20ar.dword0x08_width / m_str_F2C20ar.dword0x06_height;
					v67 = m_str_F2C20ar.dword0x08_width;
					goto LABEL_163;
				case 19:
					v62 = (((m_str_F2C20ar.dword0x14x->word_0x1C_28 - (unsigned __int16)x_WORD_F2CC0) >> 3) & 0xF0) >> 4;
					if (v62 >= 8)
					{
						v68 = v52x->word_0 + (unsigned __int8)x_BYTE_D4750[0xc + v62];
						if (str_DWORD_F66F0x[v68])
						{
							v70 = str_TMAPS00TAB_BEGIN_BUFFER[v68].word_8;
							//v69 = (int)x_D41A0_BYTEARRAY_4;
						}
						else
						{
							if (!MainInitTmaps_71520(v52x->word_0 + (unsigned __int8)x_BYTE_D4750[0xc + v62]))
								break;
							//v69 = (int)x_D41A0_BYTEARRAY_4;
							v70 = str_TMAPS00TAB_BEGIN_BUFFER[v52x->word_0 + (unsigned __int8)x_BYTE_D4750[0xc + v62]].word_8;
						}
						x_DWORD_F5730[v70] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
						a1y = *str_DWORD_F66F0x[v52x->word_0 + (unsigned __int8)x_BYTE_D4750[0xc + v62]];
						m_str_F2C20ar.dword0x08_width = a1y->width;
						m_str_F2C20ar.dword0x06_height = a1y->height;
						v71 = (signed __int64)(m_str_F2C20ar.dword0x18 * v52x->rotSpeed_8) / v100;
						m_str_F2C20ar.dword0x0c_realHeight = v71;
						m_str_F2C20ar.dword0x09_realWidth = v71 * m_str_F2C20ar.dword0x08_width / m_str_F2C20ar.dword0x06_height;
						v67 = -m_str_F2C20ar.dword0x08_width;
					}
					else
					{
						v63 = v52x->word_0 + (unsigned __int8)x_BYTE_D4750[0xc + v62];
						if (str_DWORD_F66F0x[v63])
						{
							//v64 = (int)x_D41A0_BYTEARRAY_4;
							v65 = str_TMAPS00TAB_BEGIN_BUFFER[v63].word_8;
						}
						else
						{
							if (!MainInitTmaps_71520(v52x->word_0 + (unsigned __int8)x_BYTE_D4750[0xc + v62]))
								break;
							//v64 = (int)x_D41A0_BYTEARRAY_4;
							v65 = str_TMAPS00TAB_BEGIN_BUFFER[(unsigned __int8)x_BYTE_D4750[0xc + v62] + v52x->word_0].word_8;
						}
						x_DWORD_F5730[v65] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
						a1y = *str_DWORD_F66F0x[v52x->word_0 + (unsigned __int8)x_BYTE_D4750[0xc + v62]];
						m_str_F2C20ar.dword0x08_width = a1y->width;
						m_str_F2C20ar.dword0x06_height = a1y->height;
						v66 = (signed __int64)(m_str_F2C20ar.dword0x18 * v52x->rotSpeed_8) / v100;
						m_str_F2C20ar.dword0x0c_realHeight = v66;
						m_str_F2C20ar.dword0x09_realWidth = v66 * m_str_F2C20ar.dword0x08_width / m_str_F2C20ar.dword0x06_height;
						v67 = m_str_F2C20ar.dword0x08_width;
					}
					goto LABEL_163;
				case 20:
					v82 = (((m_str_F2C20ar.dword0x14x->word_0x1C_28 - (unsigned __int16)x_WORD_F2CC0) >> 3) & 0xF0) >> 4;
					if (v82 >= 8)
					{
						v86 = (unsigned __int8)x_BYTE_D4750[0x1c + v82] + v52x->word_0;
						if (str_DWORD_F66F0x[v86])
						{
							//v87 = (int)x_D41A0_BYTEARRAY_4;
							v88 = str_TMAPS00TAB_BEGIN_BUFFER[v86].word_8;
						}
						else
						{
							if (!MainInitTmaps_71520(v52x->word_0 + (unsigned __int8)x_BYTE_D4750[0x1c + v82]))
								break;
							//v87 = (int)x_D41A0_BYTEARRAY_4;
							v88 = str_TMAPS00TAB_BEGIN_BUFFER[v52x->word_0 + (unsigned __int8)x_BYTE_D4750[0x1c + v82]].word_8;
						}
						x_DWORD_F5730[v88] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
						a1y = *str_DWORD_F66F0x[v52x->word_0 + (unsigned __int8)x_BYTE_D4750[0x1c + v82]];
						m_str_F2C20ar.dword0x08_width = a1y->width;
						m_str_F2C20ar.dword0x06_height = a1y->height;
						v89 = (signed __int64)(m_str_F2C20ar.dword0x18 * v52x->rotSpeed_8) / v100;
						m_str_F2C20ar.dword0x0c_realHeight = v89;
						m_str_F2C20ar.dword0x09_realWidth = v89 * m_str_F2C20ar.dword0x08_width / m_str_F2C20ar.dword0x06_height;
						v67 = -m_str_F2C20ar.dword0x08_width;
					}
					else
					{
						v83 = (unsigned __int8)x_BYTE_D4750[0x1c + v82];
						v84 = v83 + v52x->word_0;
						if (str_DWORD_F66F0x[v84])
						{
							x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v84].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
						}
						else
						{
							if (!MainInitTmaps_71520(v52x->word_0 + (unsigned __int8)v83))
								break;
							x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v52x->word_0 + (unsigned __int8)x_BYTE_D4750[0x1c + v82]].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
						}
						a1y = *str_DWORD_F66F0x[v52x->word_0 + (unsigned __int8)x_BYTE_D4750[0x1c + v82]];
						m_str_F2C20ar.dword0x08_width = a1y->width;
						m_str_F2C20ar.dword0x06_height = a1y->height;
						v85 = (signed __int64)(m_str_F2C20ar.dword0x18 * v52x->rotSpeed_8) / v100;
						m_str_F2C20ar.dword0x0c_realHeight = v85;
						m_str_F2C20ar.dword0x09_realWidth = v85 * m_str_F2C20ar.dword0x08_width / m_str_F2C20ar.dword0x06_height;
						v67 = m_str_F2C20ar.dword0x08_width;
					}
					goto LABEL_163;
				case 21:
					v59 = v52x->word_0;
					if (str_DWORD_F66F0x[v59])
					{
						x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v59].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
					}
					else
					{
						if (!MainInitTmaps_71520(v59))
							break;
						x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v52x->word_0].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
					}
					a1y = *str_DWORD_F66F0x[v52x->word_0];
					x_BYTE_F2CC6 = 1;
					//a1 = (uint8_t*)*v60;
					goto LABEL_141;
				case 22:
				case 23:
				case 24:
				case 25:
				case 26:
				case 27:
				case 28:
				case 29:
				case 30:
				case 31:
				case 32:
				case 33:
				case 34:
				case 35:
				case 36:
					x_BYTE_F2CC6 = 1;
				LABEL_117:
					v61 = v52x->word_0 + m_str_F2C20ar.dword0x14x->byte_0x5C_92;
					if (str_DWORD_F66F0x[v61])
					{
						x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v61].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
					}
					else
					{
						if (!MainInitTmaps_71520(v52x->word_0 + m_str_F2C20ar.dword0x14x->byte_0x5C_92))
							break;
						x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v52x->word_0 + m_str_F2C20ar.dword0x14x->byte_0x5C_92].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
					}
					a1y = *str_DWORD_F66F0x[v52x->word_0 + m_str_F2C20ar.dword0x14x->byte_0x5C_92];
				LABEL_141:
					m_str_F2C20ar.dword0x08_width = a1y->width;
					m_str_F2C20ar.dword0x06_height = a1y->height;
					v76 = (signed __int64)(m_str_F2C20ar.dword0x18 * v52x->rotSpeed_8) / v100;
					m_str_F2C20ar.dword0x0c_realHeight = v76;
					m_str_F2C20ar.dword0x09_realWidth = v76 * m_str_F2C20ar.dword0x08_width / m_str_F2C20ar.dword0x06_height;
					v67 = m_str_F2C20ar.dword0x08_width;
				LABEL_163:
					m_str_F2C20ar.dword0x05 = v67;
				LABEL_164:
					v90x = m_str_F2C20ar.dword0x14x;
					a1y->word_0 |= 8u;
					v91 = v90x->struct_byte_0xc_12_15.byte[2];
					m_str_F2C20ar.dword0x02_data = a1y->textureBuffer;
					if (v91 & 0x380)
					{
						v93 = v90x->struct_byte_0xc_12_15.byte[3];
						if (v93 & 2)
						{
							v94 = (unsigned __int8)x_BYTE_E88E0x[0x2 + 3
								* x_DWORD_EA3E4[v90x->word_0x28_40]->dword_0xA4_164x->word_0x38_56];
							m_str_F2C20ar.dword0x01_rotIdx = 4;
							m_str_F2C20ar.dword0x07 = v94;
						}
						else if (v93 & 4)
						{
							v95 = (unsigned __int8)x_BYTE_E88E0x[0x2 + 3
								* x_DWORD_EA3E4[v90x->word_0x28_40]->dword_0xA4_164x->word_0x38_56];
							m_str_F2C20ar.dword0x01_rotIdx = 5;
							m_str_F2C20ar.dword0x07 = v95;
						}
						else if (v90x->struct_byte_0xc_12_15.byte[2] >= 0)
						{
							if (v93 & 1)
								m_str_F2C20ar.dword0x01_rotIdx = 3;
						}
						else
						{
							m_str_F2C20ar.dword0x01_rotIdx = 2;
						}
					}
					else
					{
						if (m_str_F2C20ar.dword0x00 == 0x2000)
							v92 = x_BYTE_D4750[v52x->byte_10];
						else
							v92 = x_BYTE_D4750[6 + v52x->byte_10];
						m_str_F2C20ar.dword0x01_rotIdx = v92;
					}
					m_str_F2C20ar.dword0x09_realWidth++;
					m_str_F2C20ar.dword0x0c_realHeight++;
					DrawSprite_41BD3(1u, x_BYTE_E88E0x, x_DWORD_EA3E4, str_unk_1804B0ar, viewPort, screenWidth);
					break;
				default:
					goto LABEL_164;
				}
			}
		}
	LABEL_178:
		result = m_str_F2C20ar.dword0x14x->oldMapEntity_0x16_22;
	} while (result);
}

void GameRender::DrawSprite_41BD3(uint32 a1, uint8_t x_BYTE_E88E0x[], type_event_0x6E8E* x_DWORD_EA3E4[], type_str_unk_1804B0ar str_unk_1804B0ar, ViewPort viewPort, uint16_t pitch)
{
	//int v1; // eax
	int8_t* v2x; // ebx
	x_DWORD* v3; // esi
	uint8_t* v4; // edi
	int v5; // ecx
	char v6; // cf
	int v7; // ecx
	int v8; // ecx
	char v9; // al
	char v10; // al
	char v11; // al
	char v12; // al
	int v13; // eax
	x_BYTE* v14; // ebx
	x_DWORD* v15; // esi
	uint8_t* v16; // edi
	int v17; // ecx
	x_BYTE* v18; // ebx
	x_DWORD* v19; // esi
	uint8_t* v20; // edi
	int v21; // eax
	int v22; // ecx
	x_BYTE* v23; // ebx
	x_DWORD* v24; // esi
	uint8_t* v25; // edi
	int v26; // eax
	int v27; // ecx
	x_BYTE* v28; // ebx
	x_DWORD* v29; // esi
	uint8_t* v30; // edi
	int v31; // eax
	int v32; // ecx
	int v33; // eax
	x_BYTE* v34; // ebx
	x_DWORD* v35; // esi
	uint8_t* v36; // edi
	int v37; // ecx
	int v38; // edx
	x_BYTE* v39; // ebx
	x_DWORD* v40; // esi
	uint8_t* v41; // edi
	int v42; // eax
	int v43; // ecx
	int v44; // edx
	x_BYTE* v45; // ebx
	x_DWORD* v46; // esi
	uint8_t* v47; // edi
	int v48; // eax
	int v49; // ecx
	int v50; // ST4C_4
	int8_t* v51x; // ebx
	x_BYTE* v52; // edx
	x_DWORD* v53; // esi
	type_unk_F0E20x* v54x; // edi
	int v55; // ecx
	int v56; // ecx
	int v57; // ecx
	char v58; // al
	char v59; // al
	char v60; // al
	char v61; // al
	int8_t* v62x; // ebx
	int v63; // eax
	int8_t* v64; // edx
	x_DWORD* v65; // esi
	type_unk_F0E20x* v66x; // edi
	int v67; // ecx
	int8_t* v68x; // ebx
	x_BYTE* v69; // edx
	x_DWORD* v70; // esi
	type_unk_F0E20x* v71x; // edi
	int v72; // eax
	int v73; // ecx
	int8_t* v74x; // ebx
	x_BYTE* v75; // edx
	x_DWORD* v76; // esi
	type_unk_F0E20x* v77x; // edi
	int v78; // eax
	int v79; // ecx
	int8_t* v80x; // ebx
	x_BYTE* v81; // edx
	x_DWORD* v82; // esi
	type_unk_F0E20x* v83x; // edi
	int v84; // eax
	int v85; // ecx
	int8_t* v86x; // ebx
	int v87; // eax
	x_BYTE* v88; // edx
	x_DWORD* v89; // esi
	type_unk_F0E20x* v90x; // edi
	int v91; // ecx
	int v92; // ecx
	int8_t* v93x; // ebx
	x_BYTE* v94; // edx
	int v95; // eax
	x_DWORD* v96; // esi
	type_unk_F0E20x* v97x; // edi
	int v98; // ecx
	int8_t* v99x; // ebx
	x_BYTE* v100; // edx
	int v101; // eax
	x_DWORD* v102; // esi
	type_unk_F0E20x* v103x; // edi
	int8_t* v104x; // ebx
	int v105; // eax
	x_BYTE* v106; // edx
	x_DWORD* v107; // esi
	type_unk_F0E20x* v108x; // edi
	int v109; // ecx
	char v110; // al
	char v111; // al
	int v112; // eax
	int v113; // edx
	int v114; // edx
	signed int v116; // [esp+10h] [ebp-54h]
	x_DWORD* v117; // [esp+14h] [ebp-50h]
	x_DWORD* v118; // [esp+14h] [ebp-50h]
	int32_t* v119; // [esp+14h] [ebp-50h]
	x_DWORD* v120; // [esp+14h] [ebp-50h]
	int8_t* v121x; // [esp+1Ch] [ebp-48h]
	uint8_t* v122x; // [esp+20h] [ebp-44h]
	uint8_t* v123; // [esp+20h] [ebp-44h]
	int v124; // [esp+24h] [ebp-40h]
	int v125; // [esp+24h] [ebp-40h]
	int i; // [esp+24h] [ebp-40h]
	//uint8_t* j; // [esp+24h] [ebp-40h]
	int k; // [esp+24h] [ebp-40h]
	int v129; // [esp+24h] [ebp-40h]
	int v130; // [esp+24h] [ebp-40h]
	int l; // [esp+28h] [ebp-3Ch]
	int v132; // [esp+28h] [ebp-3Ch]
	int v133; // [esp+28h] [ebp-3Ch]
	int v134; // [esp+34h] [ebp-30h]
	int v135; // [esp+34h] [ebp-30h]
	int v136; // [esp+38h] [ebp-2Ch]
	int v137; // [esp+38h] [ebp-2Ch]
	int v138; // [esp+3Ch] [ebp-28h]
	int v139; // [esp+3Ch] [ebp-28h]
	int v140; // [esp+3Ch] [ebp-28h]
	int v141; // [esp+3Ch] [ebp-28h]
	int v142; // [esp+3Ch] [ebp-28h]
	int v143; // [esp+3Ch] [ebp-28h]
	int v144; // [esp+3Ch] [ebp-28h]
	int v145; // [esp+3Ch] [ebp-28h]
	int v146; // [esp+3Ch] [ebp-28h]
	int v147; // [esp+3Ch] [ebp-28h]
	int v148; // [esp+3Ch] [ebp-28h]
	int v149; // [esp+3Ch] [ebp-28h]
	int v150; // [esp+3Ch] [ebp-28h]
	int v151; // [esp+3Ch] [ebp-28h]
	int v152; // [esp+3Ch] [ebp-28h]
	x_DWORD* v153; // [esp+40h] [ebp-24h]
	x_DWORD* v154; // [esp+40h] [ebp-24h]
	uint8_t* v155; // [esp+40h] [ebp-24h]
	int v156; // [esp+44h] [ebp-20h]
	int v157; // [esp+48h] [ebp-1Ch]
	int v158; // [esp+48h] [ebp-1Ch]
	int v159; // [esp+4Ch] [ebp-18h]
	int v160; // [esp+50h] [ebp-14h]
	int v161; // [esp+50h] [ebp-14h]
	int* v162; // [esp+54h] [ebp-10h]
	x_DWORD* v163; // [esp+54h] [ebp-10h]
	x_DWORD* v164; // [esp+54h] [ebp-10h]
	x_DWORD* v165; // [esp+54h] [ebp-10h]
	int* v166; // [esp+54h] [ebp-10h]
	x_DWORD* v167; // [esp+54h] [ebp-10h]
	x_DWORD* v168; // [esp+54h] [ebp-10h]
	type_unk_F0E20x* v169x; // [esp+58h] [ebp-Ch]
	int v170; // [esp+5Ch] [ebp-8h]
	int v171; // [esp+5Ch] [ebp-8h]
	int v172; // [esp+60h] [ebp-4h]
	int v173; // [esp+60h] [ebp-4h]
	int v174; // [esp+60h] [ebp-4h]
	int v175; // [esp+60h] [ebp-4h]

	int jy;

	/*uint8_t origbyte2y[100];
	uint8_t remakebyte2y[100];
	int remakepos2y[100];
	int comp22a = compare_with_sequence_array_222BD3((char*)"00222BD3", (uint8_t*)x_DWORD_F2C20ar, 0x222bd3, debugcounter_sub_41BD3_subDrawSprite, 0x28 * 4, origbyte2y, remakebyte2y, remakepos2y);

	if (comp22a< 0x28 * 4)
		comp22a = comp22a;
		*/
		/*if (debugafterload)
			VGA_Debug_Blit(640, 480, pdwScreenBuffer);*/

	if (!x_BYTE_F2CC6)
	{
		if (a1 < 1)
		{
			if (a1)//a1==0
				goto LABEL_126;
		}
		else
		{
			if (a1 <= 1)//a1==1
			{
				m_str_F2C20ar.dword0x04_screenY -= ((m_str_F2C20ar.dword0x11 * m_str_F2C20ar.dword0x09_realWidth >> 1) + m_str_F2C20ar.dword0x0d * m_str_F2C20ar.dword0x0c_realHeight) >> 16;
				m_str_F2C20ar.dword0x03_screenX -= (m_str_F2C20ar.dword0x11 * m_str_F2C20ar.dword0x0c_realHeight - (m_str_F2C20ar.dword0x0d * m_str_F2C20ar.dword0x09_realWidth >> 1)) >> 16;
				goto LABEL_126;
			}
			if (a1 != 2)//a1 == 0,1
			{
			LABEL_126:
				//LOWORD(v1) = str_F2C20ar.dword0x1e;
				if ((unsigned int)m_str_F2C20ar.dword0x1e <= 7)
				{
					switch (m_str_F2C20ar.dword0x1e)//mirroring
					{
					case 0:
						//v1 = str_F2C20ar.dword0x1b * str_F2C20ar.dword0x09_realWidth >> 16;
						v156 = m_str_F2C20ar.dword0x1b * m_str_F2C20ar.dword0x09_realWidth >> 16;
						if (v156 <= 0)
							break;
						//v1 = (str_F2C20ar.dword0x0c_realHeight << 16) / str_F2C20ar.dword0x1b;
						v159 = (m_str_F2C20ar.dword0x0c_realHeight << 16) / m_str_F2C20ar.dword0x1b;
						if (v159 <= 0)
							break;
						v135 = (m_str_F2C20ar.dword0x06_height << 16) / v159;
						if (a1 == 1)
						{
							m_str_F2C20ar.dword0x0a_actIdx = 0;
						}
						else
						{
							m_str_F2C20ar.dword0x0a_actIdx = (v159 - 1) * v135;
							v135 = -v135;
						}
						v160 = m_str_F2C20ar.dword0x27 * m_str_F2C20ar.dword0x0c_realHeight / v159;
						v162 = (int*)(&x_DWORD_E9C38_smalltit[45920]);
						v157 = m_str_F2C20ar.dword0x04_screenY << 16;
						v170 = m_str_F2C20ar.dword0x03_screenX - (m_str_F2C20ar.dword0x1f * m_str_F2C20ar.dword0x04_screenY >> 16);
						if (m_str_F2C20ar.dword0x03_screenX - (m_str_F2C20ar.dword0x1f * m_str_F2C20ar.dword0x04_screenY >> 16) >= m_str_F2C20ar.dword0x21)
							goto LABEL_136;
						v139 = m_str_F2C20ar.dword0x21 - v170;
						//LOWORD(v1) = str_F2C20ar.dword0x21 - v170;
						v159 -= m_str_F2C20ar.dword0x21 - v170;
						if (v159 > 0)
						{
							m_str_F2C20ar.dword0x0a_actIdx += v139 * v135;
							v157 -= v139 * v160;
							v170 = m_str_F2C20ar.dword0x21;
						LABEL_136:
							v123 = pitch * v170 + m_ptrViewPortRenderBufferStart;
							goto LABEL_137;
						}
						break;
					case 1:
						//v1 = str_F2C20ar.dword0x27 * str_F2C20ar.dword0x09_realWidth >> 16;
						v156 = m_str_F2C20ar.dword0x27 * m_str_F2C20ar.dword0x09_realWidth >> 16;
						if (v156 <= 0)
							break;
						//v1 = (str_F2C20ar.dword0x0c_realHeight << 16) / str_F2C20ar.dword0x27;
						v159 = (m_str_F2C20ar.dword0x0c_realHeight << 16) / m_str_F2C20ar.dword0x27;
						if (v159 <= 0)
							break;
						v135 = (m_str_F2C20ar.dword0x06_height << 16) / v159;
						if (a1 == 1)
						{
							m_str_F2C20ar.dword0x0a_actIdx = 0;
						}
						else
						{
							m_str_F2C20ar.dword0x0a_actIdx = (v159 - 1) * v135;
							v135 = -v135;
						}
						v161 = m_str_F2C20ar.dword0x1b * m_str_F2C20ar.dword0x0c_realHeight / v159;
						v166 = (int*)(&x_DWORD_E9C38_smalltit[45920]);
						v158 = m_str_F2C20ar.dword0x03_screenX << 16;
						v171 = m_str_F2C20ar.dword0x04_screenY - (m_str_F2C20ar.dword0x1f * m_str_F2C20ar.dword0x03_screenX >> 16);
						if (m_str_F2C20ar.dword0x04_screenY - (m_str_F2C20ar.dword0x1f * m_str_F2C20ar.dword0x03_screenX >> 16) < m_str_F2C20ar.width0x25)
						{
							//LOWORD(v1) = x_DWORD_F2C30 - ((unsigned int)(str_F2C20ar.dword0x1f * x_DWORD_F2C2C) >> 16);
							if (v171 >= m_str_F2C20ar.dword0x21)
								goto LABEL_284;
						}
						else
						{
							v141 = v171 - m_str_F2C20ar.width0x25;
							//LOWORD(v1) = v171 - str_F2C20ar.dword0x25;
							v159 -= v171 - m_str_F2C20ar.width0x25;
							if (v159 > 0)
							{
								m_str_F2C20ar.dword0x0a_actIdx += v141 * v135;
								v158 += v141 * v161;
								v171 = m_str_F2C20ar.width0x25;
							LABEL_284:
								v123 = v171 + m_ptrViewPortRenderBufferStart;
								//myprintf("v123-2 %08X", v123);
								goto LABEL_285;
							}
						}
						break;
					case 2:
						//v1 = str_F2C20ar.dword0x1b * str_F2C20ar.dword0x09_realWidth >> 16;
						v156 = m_str_F2C20ar.dword0x1b * m_str_F2C20ar.dword0x09_realWidth >> 16;
						if (v156 <= 0)
							break;
						//v1 = (str_F2C20ar.dword0x0c_realHeight << 16) / str_F2C20ar.dword0x1b;
						v159 = (m_str_F2C20ar.dword0x0c_realHeight << 16) / m_str_F2C20ar.dword0x1b;
						if (v159 <= 0)
							break;
						v135 = (m_str_F2C20ar.dword0x06_height << 16) / v159;
						if (a1 == 1)
						{
							m_str_F2C20ar.dword0x0a_actIdx = 0;
						}
						else
						{
							m_str_F2C20ar.dword0x0a_actIdx = (v159 - 1) * v135;
							v135 = -v135;
						}
						v160 = m_str_F2C20ar.dword0x27 * m_str_F2C20ar.dword0x0c_realHeight / v159;
						v162 = (int*)(&x_DWORD_E9C38_smalltit[45920]);
						v157 = m_str_F2C20ar.dword0x03_screenX << 16;
						v170 = m_str_F2C20ar.width0x25 - m_str_F2C20ar.dword0x04_screenY - (m_str_F2C20ar.dword0x1f * m_str_F2C20ar.dword0x03_screenX >> 16);
						if (v170 >= m_str_F2C20ar.dword0x21)
							goto LABEL_329;
						v147 = m_str_F2C20ar.dword0x21 - v170;
						//LOWORD(v1) = str_F2C20ar.dword0x21 - v170;
						v159 -= m_str_F2C20ar.dword0x21 - v170;
						if (v159 <= 0)
							break;
						m_str_F2C20ar.dword0x0a_actIdx += v147 * v135;
						v157 -= v147 * v160;
						v170 = m_str_F2C20ar.dword0x21;
					LABEL_329:
						v123 = m_str_F2C20ar.width0x25 + m_ptrViewPortRenderBufferStart - 1 - v170;
						//myprintf("v123-3 %08X", v123);
						goto LABEL_137;
					case 3:
						//v1 = str_F2C20ar.dword0x27 * str_F2C20ar.dword0x09_realWidth >> 16;
						v156 = m_str_F2C20ar.dword0x27 * m_str_F2C20ar.dword0x09_realWidth >> 16;
						if (v156 <= 0)
							break;
						//v1 = (str_F2C20ar.dword0x0c_realHeight << 16) / str_F2C20ar.dword0x27;
						v159 = (m_str_F2C20ar.dword0x0c_realHeight << 16) / m_str_F2C20ar.dword0x27;
						if (v159 <= 0)
							break;
						v135 = (m_str_F2C20ar.dword0x06_height << 16) / v159;
						if (a1 == 1)
						{
							m_str_F2C20ar.dword0x0a_actIdx = 0;
						}
						else
						{
							m_str_F2C20ar.dword0x0a_actIdx = (v159 - 1) * v135;
							v135 = -v135;
						}
						v161 = m_str_F2C20ar.dword0x1b * m_str_F2C20ar.dword0x0c_realHeight / v159;
						v166 = (int*)(&x_DWORD_E9C38_smalltit[45920]);
						v158 = (m_str_F2C20ar.height0x26 - m_str_F2C20ar.dword0x04_screenY) << 16;
						v171 = m_str_F2C20ar.dword0x03_screenX - (m_str_F2C20ar.dword0x1f * (m_str_F2C20ar.height0x26 - m_str_F2C20ar.dword0x04_screenY) >> 16);
						if (m_str_F2C20ar.dword0x03_screenX - (m_str_F2C20ar.dword0x1f * (m_str_F2C20ar.height0x26 - m_str_F2C20ar.dword0x04_screenY) >> 16) < m_str_F2C20ar.width0x25)
						{
							//LOWORD(v1) = x_DWORD_F2C2C - ((unsigned int)(str_F2C20ar.dword0x1f * (str_F2C20ar.dword0x26 - x_DWORD_F2C30)) >> 16);
							if (v171 < m_str_F2C20ar.dword0x21)
								break;
						}
						else
						{
							v148 = v171 - m_str_F2C20ar.width0x25;
							//LOWORD(v1) = v171 - str_F2C20ar.dword0x25;
							v159 -= v171 - m_str_F2C20ar.width0x25;
							if (v159 <= 0)
								break;
							m_str_F2C20ar.dword0x0a_actIdx += v148 * v135;
							v158 += v148 * v161;
							v171 = m_str_F2C20ar.width0x25;
						}
						v123 = m_str_F2C20ar.height0x26 + pitch * v171 - 1 + m_ptrViewPortRenderBufferStart;
						//myprintf("v123-4 %08X", v123);
						goto LABEL_285;
					case 4:
						//v1 = str_F2C20ar.dword0x1b * str_F2C20ar.dword0x09_realWidth >> 16;
						v156 = m_str_F2C20ar.dword0x1b * m_str_F2C20ar.dword0x09_realWidth >> 16;
						if (v156 <= 0)
							break;
						//v1 = (str_F2C20ar.dword0x0c_realHeight << 16) / str_F2C20ar.dword0x1b;
						v159 = (m_str_F2C20ar.dword0x0c_realHeight << 16) / m_str_F2C20ar.dword0x1b;
						if (v159 <= 0)
							break;
						v135 = (m_str_F2C20ar.dword0x06_height << 16) / v159;
						if (a1 == 1)
						{
							m_str_F2C20ar.dword0x0a_actIdx = 0;
						}
						else
						{
							m_str_F2C20ar.dword0x0a_actIdx = (v159 - 1) * v135;
							v135 = -v135;
						}
						v160 = m_str_F2C20ar.dword0x27 * m_str_F2C20ar.dword0x0c_realHeight / v159;
						v162 = (int*)(&x_DWORD_E9C38_smalltit[45920]);
						v157 = (m_str_F2C20ar.height0x26 - m_str_F2C20ar.dword0x04_screenY) << 16;
						v170 = m_str_F2C20ar.width0x25 - m_str_F2C20ar.dword0x03_screenX - (m_str_F2C20ar.dword0x1f * (m_str_F2C20ar.height0x26 - m_str_F2C20ar.dword0x04_screenY) >> 16);
						if (v170 >= m_str_F2C20ar.dword0x21)
							goto LABEL_348;
						v149 = m_str_F2C20ar.dword0x21 - v170;
						//LOWORD(v1) = str_F2C20ar.dword0x21 - v170;
						v159 -= m_str_F2C20ar.dword0x21 - v170;
						if (v159 <= 0)
							break;
						m_str_F2C20ar.dword0x0a_actIdx += v149 * v135;
						v157 -= v149 * v160;
						v170 = m_str_F2C20ar.dword0x21;
					LABEL_348:
						v123 = m_str_F2C20ar.height0x26 + m_ptrViewPortRenderBufferStart + pitch * (m_str_F2C20ar.width0x25 - v170 - 1) - 1;
						//myprintf("v123-5 %08X", v123);
						goto LABEL_137;
					case 5:
						//v1 = str_F2C20ar.dword0x27 * str_F2C20ar.dword0x09_realWidth >> 16;
						v156 = m_str_F2C20ar.dword0x27 * m_str_F2C20ar.dword0x09_realWidth >> 16;
						if (v156 <= 0)
							break;
						//v1 = (str_F2C20ar.dword0x0c_realHeight << 16) / str_F2C20ar.dword0x27;
						v159 = (m_str_F2C20ar.dword0x0c_realHeight << 16) / m_str_F2C20ar.dword0x27;
						if (v159 <= 0)
							break;
						v135 = (m_str_F2C20ar.dword0x06_height << 16) / v159;
						if (a1 == 1)
						{
							m_str_F2C20ar.dword0x0a_actIdx = 0;
						}
						else
						{
							m_str_F2C20ar.dword0x0a_actIdx = (v159 - 1) * v135;
							v135 = -v135;
						}
						v161 = m_str_F2C20ar.dword0x1b * m_str_F2C20ar.dword0x0c_realHeight / v159;
						v166 = (int*)(&x_DWORD_E9C38_smalltit[45920]);
						v158 = (m_str_F2C20ar.height0x26 - m_str_F2C20ar.dword0x03_screenX) << 16;
						v113 = m_str_F2C20ar.dword0x1f * (m_str_F2C20ar.height0x26 - m_str_F2C20ar.dword0x03_screenX) >> 16;
						v171 = m_str_F2C20ar.width0x25 - m_str_F2C20ar.dword0x04_screenY - v113;
						if (v171 < m_str_F2C20ar.width0x25)
						{
							//LOWORD(v1) = str_F2C20ar.dword0x25 - x_DWORD_F2C30 - v113;
							if (v171 < m_str_F2C20ar.dword0x21)
								break;
						}
						else
						{
							v150 = v171 - m_str_F2C20ar.width0x25;
							//LOWORD(v1) = v171 - str_F2C20ar.dword0x25;
							v159 -= v171 - m_str_F2C20ar.width0x25;
							if (v159 <= 0)
								break;
							m_str_F2C20ar.dword0x0a_actIdx += v150 * v135;
							v158 += v150 * v161;
							v171 = m_str_F2C20ar.width0x25;
						}
						v123 = (m_str_F2C20ar.height0x26 - 1) * pitch + m_str_F2C20ar.width0x25 + m_ptrViewPortRenderBufferStart - 1 - v171;
						//myprintf("v123-6 %08X", v123);
						goto LABEL_285;
					case 6:
						//v1 = str_F2C20ar.dword0x1b * str_F2C20ar.dword0x09_realWidth >> 16;
						v156 = m_str_F2C20ar.dword0x1b * m_str_F2C20ar.dword0x09_realWidth >> 16;
						if (v156 <= 0)
							break;
						//v1 = (str_F2C20ar.dword0x0c_realHeight << 16) / str_F2C20ar.dword0x1b;
						v159 = (m_str_F2C20ar.dword0x0c_realHeight << 16) / m_str_F2C20ar.dword0x1b;
						if (v159 <= 0)
							break;
						v135 = (m_str_F2C20ar.dword0x06_height << 16) / v159;
						if (a1 == 1)
						{
							m_str_F2C20ar.dword0x0a_actIdx = 0;
						}
						else
						{
							m_str_F2C20ar.dword0x0a_actIdx = (v159 - 1) * v135;
							v135 = -v135;
						}
						v160 = m_str_F2C20ar.dword0x27 * m_str_F2C20ar.dword0x0c_realHeight / v159;
						v162 = (int*)(&x_DWORD_E9C38_smalltit[45920]);
						v157 = (m_str_F2C20ar.height0x26 - m_str_F2C20ar.dword0x03_screenX) << 16;
						v170 = m_str_F2C20ar.dword0x04_screenY - (m_str_F2C20ar.dword0x1f * (m_str_F2C20ar.height0x26 - m_str_F2C20ar.dword0x03_screenX) >> 16);
						if (m_str_F2C20ar.dword0x04_screenY - (m_str_F2C20ar.dword0x1f * (m_str_F2C20ar.height0x26 - m_str_F2C20ar.dword0x03_screenX) >> 16) >= m_str_F2C20ar.dword0x21)
							goto LABEL_367;
						v151 = m_str_F2C20ar.dword0x21 - v170;
						//LOWORD(v1) = str_F2C20ar.dword0x21 - v170;
						v159 -= m_str_F2C20ar.dword0x21 - v170;
						if (v159 <= 0)
							break;
						m_str_F2C20ar.dword0x0a_actIdx += v151 * v135;
						v157 -= v151 * v160;
						v170 = m_str_F2C20ar.dword0x21;
					LABEL_367:
						v123 = m_ptrViewPortRenderBufferStart + pitch * (m_str_F2C20ar.height0x26 - 1) + v170;
						//myprintf("v123-7 %08X", v123);
					LABEL_137:
						v140 = m_str_F2C20ar.width0x25 - m_str_F2C20ar.dword0x21;
						if (v170 <= 0)
						{
							if (v159 > v140)
								v159 = m_str_F2C20ar.width0x25 - m_str_F2C20ar.dword0x21;
						}
						else if (v170 + v159 > v140)
						{
							//LOWORD(v1) = v140 - v170;
							v159 = v140 - v170;
							if (v140 - v170 <= 0)
								break;
						}
						v116 = 9999999;
						v124 = v159;
						while (2)
						{
							if (!v124)
								goto LABEL_154;
							v172 = v157 >> 16;
							if (v157 >> 16 >= 0)
							{
								v162[0] = v172;
								v162[1] = v156;
								v162[2] = 0;
								v116 = 0;
							LABEL_151:
								if (v162[1] + *v162 > m_str_F2C20ar.height0x26)
									v162[1] = m_str_F2C20ar.height0x26 - *v162;
								v157 -= v160;
								v162 += 3;
								v124--;
								continue;
							}
							break;
						}
						v173 = -v172;
						v162[0] = 0;
						v162[1] = v156 - v173;
						if (v162[1] > 0)
						{
							v162[2] = v173;
							if (v173 < v116)
								v116 = v173;
							goto LABEL_151;
						}
						v159 -= v124;
					LABEL_154:
						if (v170 + v159 > m_str_F2C20ar.Height_0x19)
						{
							if (m_str_F2C20ar.Height_0x19 - v170 <= 0)
							{
								v163 = (x_DWORD*)(&x_DWORD_E9C38_smalltit[45920]);
								v117 = (x_DWORD*)(4 * (m_str_F2C20ar.Height_0x19 - v170) + m_str_F2C20ar.pbyte0x1a);
								v125 = v159;
							}
							else
							{
								v163 = (x_DWORD*)(12 * (m_str_F2C20ar.Height_0x19 - v170) + &x_DWORD_E9C38_smalltit[45920]);
								v117 = (x_DWORD*)m_str_F2C20ar.pbyte0x1a;
								v125 = v170 + v159 - m_str_F2C20ar.Height_0x19;
							}
							while (v125)
							{
								if (*v163 + v163[1] > *v117)
								{
									if (*v163 >= *v117)
									{
										v159 -= v125;
										break;
									}
									v163[1] = *v117 - *v163;
								}
								v163 += 3;
								v117--;
								v125--;
							}
						}
						if (v170 < 0)
						{
							v118 = (x_DWORD*)(4 * (-1 - v170) + x_DWORD_E9C38_smalltit + 59360);
							v164 = (x_DWORD*)(&x_DWORD_E9C38_smalltit[45920]);
							while (v170)
							{
								if (*v118 > *v164)
								{
									if (v164[1] + *v164 >= *v118)
									{
										v50 = *v118 - *v164;
										v164[0] = *v118;
										v164[2] += v50;
										v164[1] -= v50;
									}
									else
									{
										v164[1] = 0;
									}
								}
								v118--;
								v164 += 3;
								v170++;
							}
						}
					LABEL_172:
						v137 = (m_str_F2C20ar.dword0x05 << 16) / v156;
						m_str_F2C20ar.dword0x0b = 0;
						if (m_str_F2C20ar.dword0x05 < 0)
							m_str_F2C20ar.dword0x0b -= v137 * (v156 - 1);
						//LOWORD(v1) = v156 - v116;
						v132 = v156 - v116;
						if (v156 - v116 > 0)
						{
							if (v132 > m_str_F2C20ar.dword0x1c)
								v132 = m_str_F2C20ar.dword0x1c;
							m_str_F2C20ar.dword0x0b += v137 * v116;
							v154 = (x_DWORD*)(&x_DWORD_E9C38_smalltit[36960]);
							while (v132 >= 0)
							{
								v154[1] = m_str_F2C20ar.dword0x0b >> 16;
								*v154 = v154[1] - *(v154 - 1);
								m_str_F2C20ar.dword0x0b += v137;
								v154 += 2;
								v132--;
							}
							v165 = (x_DWORD*)(&x_DWORD_E9C38_smalltit[45920]);
							//LOWORD(v1) = v159;
							for (i = v159; i; i--)
							{
								v133 = v165[1];
								if (v133 > 0)
								{
									//adress 2237d3

									v169x = &m_str_F0E20x[*v165];
									v155 = 8 * (v165[2] - v116) + &x_DWORD_E9C38_smalltit[36960];
									//v121 = str_F2C20ar.dword0x08_width * (str_F2C20ar.dword0x0a_actIdx >> 16) + (int)str_F2C20ar.dword0x02_data;
									v121x = &m_str_F2C20ar.dword0x02_data[m_str_F2C20ar.dword0x08_width * (m_str_F2C20ar.dword0x0a_actIdx >> 16)];
									if ((unsigned int)m_str_F2C20ar.dword0x01_rotIdx <= 8)
									{
										switch (m_str_F2C20ar.dword0x01_rotIdx)
										{
										case 0:
											//v51 = (char*)(*(x_DWORD*)(v155 + 4) + v121);
											v51x = &v121x[*(x_DWORD*)(v155 + 4)];
											//v52 = (x_BYTE*)(*((int32_t*)v169 + 1) + v123);
											v52 = (x_BYTE*)(v169x->dword_1 + v123);
											v53 = (x_DWORD*)(8 * (v165[2] - v116) + &x_DWORD_E9C38_smalltit[36960]);
											v54x = &m_str_F0E20x[*v165];
											v55 = v133 >> 1;
											if (!(v133 & 1))
											{
												v6 = v55 & 1;
												v56 = v133 >> 2;
												if (v6)
												{
													v56++;
													v53 = (x_DWORD*)(v155 - 8);
													v54x = &v169x[-1];
													goto LABEL_197;
												}
												v53 = (x_DWORD*)(v155 + 8);
												v54x = &v169x[1];
												goto LABEL_191;
											}
											v57 = v55 + 2;
											v6 = v57 & 1;
											v56 = v57 >> 1;
											if (!v6)
											{
												v53 = (x_DWORD*)(v155 - 16);
												v54x = &v169x[-2];
												goto LABEL_200;
											}
											while (1)
											{
												v59 = v51x[0];
												v51x += v53[2];
												if (v59)
													*v52 = v59;
												//v52 += *((x_DWORD*)v54 + 3);
												v52 += v54x[1].dword_0;
											LABEL_197:
												v60 = v51x[0];
												v51x += v53[4];
												if (v60)
													*v52 = v60;
												v52 += v54x[2].dword_0;
											LABEL_200:
												v61 = v51x[0];
												v51x += v53[6];
												if (v61)
													*v52 = v61;
												v52 += v54x[3].dword_0;
												v53 += 8;
												//v54 += 48;
												v54x += 4;
												if (!--v56)
													break;
											LABEL_191:
												v58 = v51x[0];
												v51x += *v53;
												if (v58)
													*v52 = v58;
												v52 += v54x[0].dword_0;
											}
											break;
										case 1:
											//v62 = (x_BYTE*)(*(x_DWORD*)(v155 + 4) + v121);
											v62x = &v121x[*(x_DWORD*)(v155 + 4)];
											v63 = m_str_F2C20ar.dword0x00;
											v64 = (int8_t*)(v169x->dword_1 + v123);
											v65 = (x_DWORD*)(8 * (v165[2] - v116) + &x_DWORD_E9C38_smalltit[36960]);
											//v66 = (x_DWORD*)((char*)unk_F0E20x + 12 * *v165);
											v66x = &m_str_F0E20x[*v165];
											v67 = v133 >> 1;
											if (!(v133 & 1))
											{
												v65 = (x_DWORD*)(v155 + 8);
												//v66 = (x_DWORD*)(v169 + 12);
												v66x = &v169x[1];
												goto LABEL_207;
											}
											v67++;
											while (1)
											{
												LOBYTE(v63) = v62x[0];
												v62x += v65[2];
												if ((x_BYTE)v63)
													*v64 = m_ptrX_BYTE_F6EE0_tablesx[v63];
												v64 += v66x[1].dword_0;
												v65 += 4;
												//v66 += 6;
												v66x += 2;
												if (!--v67)
													break;
											LABEL_207:
												LOBYTE(v63) = v62x[0];
												v62x += *v65;
												if ((x_BYTE)v63)
													*v64 = m_ptrX_BYTE_F6EE0_tablesx[v63];
												v64 += v66x->dword_0;
											}
											break;
										case 2:
											//v68 = (x_BYTE*)(*(x_DWORD*)(v155 + 4) + v121);
											v68x = &v121x[*(x_DWORD*)(v155 + 4)];
											//v69 = (x_BYTE*)(*((x_DWORD*)v169 + 1) + v123);
											v69 = (x_BYTE*)(v169x->dword_1 + v123);
											v70 = (x_DWORD*)(8 * (v165[2] - v116) + &x_DWORD_E9C38_smalltit[36960]);
											//v71 = (x_DWORD*)((char*)unk_F0E20x + 12 * *v165);
											v71x = &m_str_F0E20x[*v165];
											HIWORD(v72) = 0;
											v73 = v133 >> 1;
											if (!(v133 & 1))
											{
												v70 = (x_DWORD*)(v155 + 8);
												v71x = &v169x[1];
												goto LABEL_217;
											}
											v73++;
											while (1)
											{
												HIBYTE(v72) = v68x[0];
												v68x += v70[2];
												if (HIBYTE(v72))
												{
													LOBYTE(v72) = *v69;
													*v69 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v72];
												}
												v69 += v71x[1].dword_0;
												v70 += 4;
												//v71 += 6;
												v71x += 2;
												if (!--v73)
													break;
											LABEL_217:
												HIBYTE(v72) = v68x[0];
												v68x += *v70;
												if (HIBYTE(v72))
												{
													LOBYTE(v72) = *v69;
													*v69 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v72];
												}
												v69 += v71x->dword_0;
											}
											break;
										case 3:
											//v74 = (x_BYTE*)(*(x_DWORD*)(v155 + 4) + v121);
											v74x = &v121x[*(x_DWORD*)(v155 + 4)];
											//v75 = (x_BYTE*)(*((x_DWORD*)v169 + 1) + v123);
											v75 = (x_BYTE*)(v169x->dword_1 + v123);
											v76 = (x_DWORD*)(8 * (v165[2] - v116) + &x_DWORD_E9C38_smalltit[36960]);//to position
											//v77 = (x_DWORD*)((char*)unk_F0E20x + 12 * v165[0]);//from position
											v77x = &m_str_F0E20x[v165[0]];//from position
											HIWORD(v78) = 0;
											v79 = v133 >> 1;
											if (!(v133 & 1))
											{
												v76 = (x_DWORD*)(v155 + 8);
												v77x = &v169x[1];
												goto LABEL_227;
											}
											v79++;
											while (1)
											{
												LOBYTE(v78) = v74x[0];
												v74x += v76[2];
												if ((x_BYTE)v78)
												{
													HIBYTE(v78) = *v75;
													*v75 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v78];
												}
												v75 += v77x[1].dword_0;
												v76 += 4;
												//v77 += 6;
												v77x += 2;
												if (!--v79)
													break;
											LABEL_227:
												LOBYTE(v78) = v74x[0];
												v74x += *v76;
												if ((x_BYTE)v78)
												{
													HIBYTE(v78) = *v75;
													*v75 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v78];
												}
												v75 += v77x->dword_0;
											}
											break;
										case 4:
											//v80 = (x_BYTE*)(*(x_DWORD*)(v155 + 4) + v121);
											v80x = &v121x[*(x_DWORD*)(v155 + 4)];
											HIWORD(v84) = HIWORD(m_str_F2C20ar.dword0x07);
											//v81 = (x_BYTE*)(*((x_DWORD*)v169 + 1) + v123);
											v81 = (x_BYTE*)(v169x->dword_1 + v123);
											v82 = (x_DWORD*)(8 * (v165[2] - v116) + &x_DWORD_E9C38_smalltit[36960]);
											//v83 = (x_DWORD*)((char*)unk_F0E20x + 12 * *v165);
											v83x = &m_str_F0E20x[*v165];
											BYTE1(v84) = m_str_F2C20ar.dword0x07;
											v85 = v133 >> 1;
											if (!(v133 & 1))
											{
												v82 = (x_DWORD*)(v155 + 8);
												v83x = &v169x[1];
												goto LABEL_237;
											}
											v85++;
											while (1)
											{
												LOBYTE(v84) = v80x[0];
												v80x += v82[2];
												if ((x_BYTE)v84)
													*v81 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v84];
												v81 += v83x[1].dword_0;
												v82 += 4;
												//v83 += 6;
												v83x += 2;
												if (!--v85)
													break;
											LABEL_237:
												LOBYTE(v84) = v80x[0];
												v80x += *v82;
												if ((x_BYTE)v84)
													*v81 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v84];
												v81 += v83x->dword_0;
											}
											break;
										case 5:
											//v86 = (x_BYTE*)(*(x_DWORD*)(v155 + 4) + v121);
											v86x = &v121x[*(x_DWORD*)(v155 + 4)];
											v87 = m_str_F2C20ar.dword0x07;
											//v88 = (x_BYTE*)(*((x_DWORD*)v169 + 1) + v123);
											v88 = (x_BYTE*)(v169x->dword_1 + v123);
											v89 = (x_DWORD*)(8 * (v165[2] - v116) + &x_DWORD_E9C38_smalltit[36960]);
											v90x = &m_str_F0E20x[*v165];
											v91 = v133 >> 1;
											if (!(v133 & 1))
											{
												v89 = (x_DWORD*)(v155 + 8);
												//v90 = (x_DWORD*)(v169 + 12);
												v90x = &m_str_F0E20x[1];
												goto LABEL_247;
											}
											v91++;
											while (1)
											{
												BYTE1(v87) = v86x[0];
												v86x += v89[2];
												if (BYTE1(v87))
													*v88 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v87];
												v88 += v90x[1].dword_0;
												v89 += 4;
												//v90 += 6;
												v90x += 2;
												if (!--v91)
													break;
											LABEL_247:
												BYTE1(v87) = v86x[0];
												v86x += *v89;
												if (BYTE1(v87))
													*v88 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v87];
												v88 += v90x->dword_0;
											}
											break;
										case 6:
											v92 = m_str_F2C20ar.dword0x00;
											//v93 = (x_BYTE*)(*(x_DWORD*)(v155 + 4) + v121);
											v93x = &v121x[*(x_DWORD*)(v155 + 4)];
											//v94 = (x_BYTE*)(*((x_DWORD*)v169 + 1) + v123);
											v94 = (x_BYTE*)(v169x->dword_1 + v123);
											HIWORD(v95) = 0;
											v96 = (x_DWORD*)(v155 + 8);
											//v97 = (x_DWORD*)(v169 + 12);
											v97x = &v169x[1];
											do
											{
												BYTE1(v95) = v93x[0];
												v93x += *v96;
												if (BYTE1(v95))
												{
													LOBYTE(v95) = *v94;
													LOBYTE(v92) = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v95];
													*v94 = m_ptrX_BYTE_F6EE0_tablesx[v92];
												}
												v94 += v97x->dword_0;
												v96 += 2;
												//v97 += 3;
												v97x++;
												v133--;
											} while (v133);
											break;
										case 7:
											v98 = m_str_F2C20ar.dword0x00;
											//v99 = (x_BYTE*)(*(x_DWORD*)(v155 + 4) + v121);
											v99x = &v121x[*(x_DWORD*)(v155 + 4)];
											//v100 = (x_BYTE*)(*((x_DWORD*)v169 + 1) + v123);
											v100 = (x_BYTE*)(v169x->dword_1 + v123);
											HIWORD(v101) = 0;
											v102 = (x_DWORD*)(v155 + 8);
											//v103 = (x_DWORD*)(v169 + 12);
											v103x = &v169x[1];
											do
											{
												LOBYTE(v101) = v99x[0];
												v99x += *v102;
												if ((x_BYTE)v101)
												{
													BYTE1(v101) = *v100;
													LOBYTE(v98) = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v101];
													*v100 = m_ptrX_BYTE_F6EE0_tablesx[v98];
												}
												v100 += v103x->dword_0;
												v102 += 2;
												//v103 += 3;
												v103x++;
												v133--;
											} while (v133);
											break;
										case 8:
											//v104 = (char*)(*(x_DWORD*)(v155 + 4) + v121);
											v104x = &v121x[*(x_DWORD*)(v155 + 4)];
											v105 = m_str_F2C20ar.dword0x00;
											//v106 = (x_BYTE*)(*((x_DWORD*)v169 + 1) + v123);
											v106 = (x_BYTE*)(v169x->dword_1 + v123);
											v107 = (x_DWORD*)(8 * (v165[2] - v116) + &x_DWORD_E9C38_smalltit[36960]);
											//v108 = (x_DWORD*)((char*)unk_F0E20x + 12 * *v165);
											v108x = &m_str_F0E20x[*v165];
											v109 = v133 >> 1;
											if (!(v133 & 1))
											{
												v107 = (x_DWORD*)(v155 + 8);
												//v108 = (x_DWORD*)(v169 + 12);
												v108x = &v169x[1];
												goto LABEL_267;
											}
											v109++;
											while (1)
											{
												v111 = v104x[0];
												v104x += v107[2];
												if (v111)
												{
													LOBYTE(v105) = *v106;
													*v106 = m_ptrX_BYTE_F6EE0_tablesx[v105];
												}
												v106 += v108x[3].dword_0;
												v107 += 4;
												//v108 += 6;
												v108x += 2;
												if (!--v109)
													break;
											LABEL_267:
												v110 = v104x[0];
												v104x += *v107;
												if (v110)
												{
													LOBYTE(v105) = *v106;
													*v106 = m_ptrX_BYTE_F6EE0_tablesx[v105];
												}
												v106 += v108x->dword_0;
											}
											break;
										}
									}
								}
								m_str_F2C20ar.dword0x0a_actIdx += v135;
								v123 += m_str_F2C20ar.dword0x23;
								//myprintf("v123-10 %08X,%d\n", v123,i);
								v165 += 3;
								//LOWORD(v1) = i;
							}
						}
						break;
					case 7:
						//v1 = str_F2C20ar.dword0x27 * str_F2C20ar.dword0x09_realWidth >> 16;
						v156 = m_str_F2C20ar.dword0x27 * m_str_F2C20ar.dword0x09_realWidth >> 16;
						if (v156 <= 0)
							break;
						//v1 = (str_F2C20ar.dword0x0c_realHeight << 16) / str_F2C20ar.dword0x27;
						v159 = (m_str_F2C20ar.dword0x0c_realHeight << 16) / m_str_F2C20ar.dword0x27;
						if (v159 <= 0)
							break;
						//LOWORD(v1) = x_DWORD_F2C30;
						if (m_str_F2C20ar.dword0x04_screenY >= m_str_F2C20ar.height0x26)
							break;
						v135 = (m_str_F2C20ar.dword0x06_height << 16) / v159;
						if (a1 == 1)
						{
							m_str_F2C20ar.dword0x0a_actIdx = 0;
						}
						else
						{
							m_str_F2C20ar.dword0x0a_actIdx = (v159 - 1) * v135;
							v135 = -v135;
						}
						v161 = m_str_F2C20ar.dword0x1b * m_str_F2C20ar.dword0x0c_realHeight / v159;
						v166 = (int*)(&x_DWORD_E9C38_smalltit[45920]);
						v158 = m_str_F2C20ar.dword0x04_screenY << 16;
						v114 = m_str_F2C20ar.dword0x1f * m_str_F2C20ar.dword0x04_screenY >> 16;
						v171 = m_str_F2C20ar.width0x25 - m_str_F2C20ar.dword0x03_screenX - v114;
						if (v171 < m_str_F2C20ar.width0x25)
						{
							//LOWORD(v1) = str_F2C20ar.dword0x25 - x_DWORD_F2C2C - v114;
							if (v171 < m_str_F2C20ar.dword0x21)
								break;
						}
						else
						{
							v152 = v171 - m_str_F2C20ar.width0x25;
							//LOWORD(v1) = v171 - str_F2C20ar.dword0x25;
							v159 -= v171 - m_str_F2C20ar.width0x25;
							if (v159 <= 0)
								break;
							m_str_F2C20ar.dword0x0a_actIdx += v152 * v135;
							v158 += v152 * v161;
							v171 = m_str_F2C20ar.width0x25;
						}
						v123 = pitch * (m_str_F2C20ar.width0x25 - 1 - v171) + m_ptrViewPortRenderBufferStart;
						//myprintf("v123-11 %08X", v123);
					LABEL_285:
						v142 = m_str_F2C20ar.width0x25 - m_str_F2C20ar.dword0x21;
						if (v171 < m_str_F2C20ar.width0x25)
						{
							if (v159 > v142)
								v159 = m_str_F2C20ar.width0x25 - m_str_F2C20ar.dword0x21;
						}
						else if (v171 + v159 - m_str_F2C20ar.width0x25 > v142)
						{
							//LOWORD(v1) = v142 - v171;
							v159 = v142 - v171 + m_str_F2C20ar.width0x25;
							if (v159 <= 0)
								break;
						}
						//adress 223d83
						v116 = 9999999;
						for (jy = v159; jy; jy--)
						{
							v174 = v158 >> 16;
							if (v158 >> 16 < 0)
							{
								v175 = -v174;
								v166[0] = 0;
								v166[1] = v156 - v175;
								v166[2] = v175;
								if (v175 < v116)
									v116 = v175;
							}
							else
							{
								v166[0] = v174;
								v166[1] = v156;
								v166[2] = 0;
								v116 = 0;
							}
							if (v166[1] + v166[0] > m_str_F2C20ar.height0x26)
								v166[1] = m_str_F2C20ar.height0x26 - v166[0];
							v158 += v161;
							v166 += 3;
						}
						v143 = v171 + m_str_F2C20ar.dword0x1d - m_str_F2C20ar.width0x25 + 1;
						if (v143 > 0)
						{
							v167 = (x_DWORD*)(12 * v143 + &x_DWORD_E9C38_smalltit[45920]);
							v119 = (int32_t*)m_str_F2C20ar.pbyte0x1a;
							for (k = v171 + m_str_F2C20ar.dword0x1d - m_str_F2C20ar.width0x25 + 2; k; k--)
							{
								v167 -= 3;
								if (&x_DWORD_E9C38_smalltit[45920] > (uint8_t*)v167)
									break;
								v144 = v167[1] + v167[0] - v119[0];
								if (v144 > 0)
								{
									v167[1] -= v144;
									if (v167[1] < 0)
										v167[1] = 0;
								}
								v119--;
							}
						}
						if (v171 - v159 < 0)
						{
							v129 = v159 - v171;
							v168 = (x_DWORD*)(12 * v171 + &x_DWORD_E9C38_smalltit[45920]);
							v120 = (x_DWORD*)(x_DWORD_E9C38_smalltit + 59360);
							v145 = v171 - v159 - m_str_F2C20ar.dword0x21;
							if (v145 < 0)
							{
								//LOWORD(v1) = v171 - v159 - str_F2C20ar.dword0x21;
								v159 = v171 - m_str_F2C20ar.dword0x21;
								if (v171 - m_str_F2C20ar.dword0x21 <= 0)
									break;
								v129 += v145;
							}
							v112 = v129;
							v130 = v129 - 1;
							if (v112 > 0)
							{
								while (v130)
								{
									v168 += 3;
									if (&x_DWORD_E9C38_smalltit[45920] <= (uint8_t*)v168)
									{
										v146 = *v120 - *v168;
										if (v146 > 0)
										{
											v168[0] = *v120;
											v168[1] -= v146;
											if (v168[1] < 0)
												v168[1] = 0;
											v168[2] += v146;
										}
									}
									v120++;
									v130--;
								}
							}
						}
						goto LABEL_172;
					}
				}
				if (a1 == 1)
				{
					if (!x_D41A0_BYTEARRAY_4_struct.byteindex_207
						&& m_str_F2C20ar.dword0x14x->type_0x3F_63 == 3
						&& (!m_str_F2C20ar.dword0x14x->subtype_0x40_64 || m_str_F2C20ar.dword0x14x->subtype_0x40_64 == 1))
					{
						DrawSorcererNameAndHealthBar_2CB30(m_str_F2C20ar.dword0x14x, x_BYTE_E88E0x, m_str_F2C20ar.dword0x04_screenY, (signed __int16)m_str_F2C20ar.dword0x03_screenX, m_str_F2C20ar.dword0x09_realWidth, viewPort.PosX, viewPort.PosY, viewPort.Width, viewPort.Height, pitch);
					}
					if (x_D41A0_BYTEARRAY_4_struct.byteindex_10)
						sub_88740(
							m_str_F2C20ar.dword0x14x,
							x_DWORD_EA3E4,
							str_unk_1804B0ar,
							(signed __int16)(m_str_F2C20ar.dword0x04_screenY + (m_str_F2C20ar.dword0x09_realWidth >> 1)),
							(signed __int16)(m_str_F2C20ar.dword0x03_screenX + (m_str_F2C20ar.dword0x0c_realHeight >> 1)));
					//LOWORD(v1) = str_F2C20ar.dword0x14;
					if (m_str_F2C20ar.dword0x14x->struct_byte_0xc_12_15.byte[3] & 0x40)
					{
						//v1 = str_F2C20ar.dword0x14;
						m_str_F2C20ar.dword0x14x->word_0x2A_42 |= 0x40u;
					}
				}
				return;
			}
		}
		m_str_F2C20ar.dword0x04_screenY -= m_str_F2C20ar.dword0x11 * m_str_F2C20ar.dword0x09_realWidth >> 17;
		m_str_F2C20ar.dword0x03_screenX -= -(m_str_F2C20ar.dword0x0d * m_str_F2C20ar.dword0x09_realWidth) >> 17;
		goto LABEL_126;
	}
	v138 = (m_str_F2C20ar.dword0x0c_realHeight + m_str_F2C20ar.dword0x09_realWidth) >> 2;
	if (a1 >= 1)
	{
		if (a1 <= 1)
		{
			m_str_F2C20ar.dword0x04_screenY += -(m_str_F2C20ar.dword0x0d * v138 >> 16) - v138;
			m_str_F2C20ar.dword0x03_screenX += -(m_str_F2C20ar.dword0x11 * v138 >> 16) - v138;
		}
		else if (a1 == 2)
		{
			m_str_F2C20ar.dword0x04_screenY += (m_str_F2C20ar.dword0x0d * v138 >> 16) - v138;
			m_str_F2C20ar.dword0x03_screenX += (m_str_F2C20ar.dword0x11 * v138 >> 16) - v138;
		}
	}
	//LOWORD(v1) = iViewPortWidth;
	if ((unsigned __int16)viewPort.Width > m_str_F2C20ar.dword0x04_screenY)
	{
		v136 = (m_str_F2C20ar.dword0x05 << 16) / m_str_F2C20ar.dword0x09_realWidth;
		if (-m_str_F2C20ar.dword0x04_screenY < 0 || m_str_F2C20ar.dword0x04_screenY == 0)
		{
			m_str_F2C20ar.dword0x0b = 0;
			if (m_str_F2C20ar.dword0x09_realWidth + m_str_F2C20ar.dword0x04_screenY - (unsigned __int16)viewPort.Width > 0)
				m_str_F2C20ar.dword0x09_realWidth -= m_str_F2C20ar.dword0x09_realWidth + m_str_F2C20ar.dword0x04_screenY - (unsigned __int16)viewPort.Width;
		}
		else
		{
			//LOWORD(v1) = -(signed __int16)x_DWORD_F2C30;
			m_str_F2C20ar.dword0x09_realWidth += m_str_F2C20ar.dword0x04_screenY;
			if (m_str_F2C20ar.dword0x09_realWidth <= 0)
				return;
			m_str_F2C20ar.dword0x0b = v136 * -m_str_F2C20ar.dword0x04_screenY;
			m_str_F2C20ar.dword0x04_screenY = 0;
			if ((unsigned __int16)viewPort.Width <= m_str_F2C20ar.dword0x09_realWidth)
				m_str_F2C20ar.dword0x09_realWidth = (unsigned __int16)viewPort.Width;
		}
		//LOWORD(v1) = viewPortHeight;
		if ((unsigned __int16)viewPort.Height > m_str_F2C20ar.dword0x03_screenX)
		{
			v134 = (m_str_F2C20ar.dword0x06_height << 16) / m_str_F2C20ar.dword0x0c_realHeight;
			if (-m_str_F2C20ar.dword0x03_screenX < 0 || m_str_F2C20ar.dword0x03_screenX == 0)
			{
				m_str_F2C20ar.dword0x0a_actIdx = 0;
				if (m_str_F2C20ar.dword0x0c_realHeight + m_str_F2C20ar.dword0x03_screenX - (unsigned __int16)viewPort.Height > 0)
					m_str_F2C20ar.dword0x0c_realHeight -= m_str_F2C20ar.dword0x0c_realHeight + m_str_F2C20ar.dword0x03_screenX - (unsigned __int16)viewPort.Height;
			}
			else
			{
				//LOWORD(v1) = -(signed __int16)x_DWORD_F2C2C;
				m_str_F2C20ar.dword0x0c_realHeight += m_str_F2C20ar.dword0x03_screenX;
				if (m_str_F2C20ar.dword0x0c_realHeight <= 0)
					return;
				m_str_F2C20ar.dword0x0a_actIdx = v134 * -m_str_F2C20ar.dword0x03_screenX;
				m_str_F2C20ar.dword0x03_screenX = 0;
				if ((unsigned __int16)viewPort.Height <= m_str_F2C20ar.dword0x0c_realHeight)
					m_str_F2C20ar.dword0x0c_realHeight = (unsigned __int16)viewPort.Height;
			}
			v153 = (x_DWORD*)(&x_DWORD_E9C38_smalltit[36960]);
			for (l = m_str_F2C20ar.dword0x09_realWidth; l; l--)
			{
				v153[1] = m_str_F2C20ar.dword0x0b >> 16;
				if ((x_DWORD*)(&x_DWORD_E9C38_smalltit[36960]) == v153)
					*v153 = 22;
				else
					*v153 = v153[1] - *(v153 - 1);
				m_str_F2C20ar.dword0x0b += v136;
				v153 += 2;
			}
			if (a1 == 1 && x_D41A0_BYTEARRAY_4_struct.byteindex_10)
				sub_88740(
					m_str_F2C20ar.dword0x14x,
					x_DWORD_EA3E4,
					str_unk_1804B0ar,
					(signed __int16)(m_str_F2C20ar.dword0x04_screenY + (m_str_F2C20ar.dword0x09_realWidth >> 1)),
					(signed __int16)(m_str_F2C20ar.dword0x03_screenX + (m_str_F2C20ar.dword0x0c_realHeight >> 1)));
			//v1 = (int)(x_DWORD_F2C2C * pitch + x_DWORD_F2C30 + x_DWORD_DE558);

			//               screen-Y                                            screen-X
			v122x = &m_ptrViewPortRenderBufferStart[m_str_F2C20ar.dword0x03_screenX * pitch + m_str_F2C20ar.dword0x04_screenY];
			//height
			while (m_str_F2C20ar.dword0x0c_realHeight)
			{
				if ((unsigned int)m_str_F2C20ar.dword0x01_rotIdx <= 7)
				{
					switch (m_str_F2C20ar.dword0x01_rotIdx)//mirroring
					{
					case 0:
						//               width                  actual line                   base adress                 add index
						//v2 = (char*)(str_F2C20ar.dword0x08_width * (str_F2C20ar.dword0x0a_actIdx >> 16) + (int)str_F2C20ar.dword0x02_data + *(x_DWORD*)(x_DWORD_E9C38_smalltit + 36964));//sprite
						v2x = &m_str_F2C20ar.dword0x02_data[m_str_F2C20ar.dword0x08_width * (m_str_F2C20ar.dword0x0a_actIdx >> 16) + *(x_DWORD*)(x_DWORD_E9C38_smalltit + 36964)];//sprite
						v3 = (x_DWORD*)&(x_DWORD_E9C38_smalltit[36960]);
						v4 = v122x;//screen
						//   parametres
						v5 = m_str_F2C20ar.dword0x09_realWidth >> 1;
						if (!(m_str_F2C20ar.dword0x09_realWidth & 1))//bit 1
						{
							v6 = v5 & 1;//bit 2
							v7 = m_str_F2C20ar.dword0x09_realWidth >> 2;//bits 3-8 -> 1-6
							if (v6)
							{
								v7++;
								v3 = (x_DWORD*)(x_DWORD_E9C38_smalltit + 36952);
								v4 = (uint8_t*)(v122x - 2);
								goto LABEL_49;
							}
							v3 = (x_DWORD*)(x_DWORD_E9C38_smalltit + 36968);
							goto LABEL_45;
						}
						v8 = v5 + 2;
						v6 = v8 & 1;
						v7 = v8 >> 1;
						if (!v6)
						{
							v3 = (x_DWORD*)(x_DWORD_E9C38_smalltit + 36944);
							v4 = (uint8_t*)(v122x - 3);
							goto LABEL_51;
						}
						v4 = (uint8_t*)(v122x - 1);
						while (1)
						{
							v10 = v2x[0];
							v2x += v3[2];
							if (v10)
								v4[1] = v10;
						LABEL_49:
							v11 = v2x[0];
							v2x += v3[4];
							if (v11)
								v4[2] = v11;
						LABEL_51:
							v12 = v2x[0];
							v2x += v3[6];
							if (v12)
								v4[3] = v12;
							v4 += 4;
							v3 += 8;
							if (!--v7)
								break;
						LABEL_45:
							v9 = v2x[0];
							v2x += v3[0];
							if (v9)//if not transparent pixel
								*v4 = v9;
						}
						break;
					case 1:
						v13 = m_str_F2C20ar.dword0x00;
						v14 = (x_BYTE*)(m_str_F2C20ar.dword0x08_width * (m_str_F2C20ar.dword0x0a_actIdx >> 16) + m_str_F2C20ar.dword0x02_data + *(x_DWORD*)(x_DWORD_E9C38_smalltit + 36964));
						v15 = (x_DWORD*)(&x_DWORD_E9C38_smalltit[36960]);
						v16 = (uint8_t*)v122x;
						v17 = m_str_F2C20ar.dword0x09_realWidth >> 1;
						if (!(m_str_F2C20ar.dword0x09_realWidth & 1))
						{
							v15 = (x_DWORD*)(x_DWORD_E9C38_smalltit + 36968);
							goto LABEL_58;
						}
						v17++;
						v16 = (uint8_t*)(v122x - 1);
						while (1)
						{
							LOBYTE(v13) = *v14;
							v14 += v15[2];
							if ((x_BYTE)v13)
								v16[1] = m_ptrX_BYTE_F6EE0_tablesx[v13];
							v16 += 2;
							v15 += 4;
							if (!--v17)
								break;
						LABEL_58:
							LOBYTE(v13) = *v14;
							v14 += *v15;
							if ((x_BYTE)v13)
								*v16 = m_ptrX_BYTE_F6EE0_tablesx[v13];
						}
						break;
					case 2:
						v18 = (x_BYTE*)(m_str_F2C20ar.dword0x08_width * (m_str_F2C20ar.dword0x0a_actIdx >> 16) + m_str_F2C20ar.dword0x02_data + *(x_DWORD*)(x_DWORD_E9C38_smalltit + 36964));
						v19 = (x_DWORD*)(&x_DWORD_E9C38_smalltit[36960]);
						v20 = (uint8_t*)v122x;
						HIWORD(v21) = 0;
						v22 = m_str_F2C20ar.dword0x09_realWidth >> 1;
						if (!(m_str_F2C20ar.dword0x09_realWidth & 1))
						{
							v19 = (x_DWORD*)(x_DWORD_E9C38_smalltit + 36968);
							goto LABEL_67;
						}
						v22++;
						v20 = (uint8_t*)(v122x - 1);
						while (1)
						{
							HIBYTE(v21) = *v18;
							v18 += v19[2];
							if (HIBYTE(v21))
							{
								LOBYTE(v21) = v20[1];
								v20[1] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v21];
							}
							v20 += 2;
							v19 += 4;
							if (!--v22)
								break;
						LABEL_67:
							HIBYTE(v21) = *v18;//zde
							v18 += *v19;
							if (HIBYTE(v21))
							{
								LOBYTE(v21) = *v20;
								v20[0] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v21];
							}
						}
						break;
					case 3:
						v23 = (x_BYTE*)(m_str_F2C20ar.dword0x08_width * (m_str_F2C20ar.dword0x0a_actIdx >> 16) + m_str_F2C20ar.dword0x02_data + *(x_DWORD*)(x_DWORD_E9C38_smalltit + 36964));//from mask
						v24 = (x_DWORD*)(&x_DWORD_E9C38_smalltit[36960]);//from image
						v25 = v122x;//to adress
						HIWORD(v26) = 0;
						v27 = m_str_F2C20ar.dword0x09_realWidth >> 1;
						if (!(m_str_F2C20ar.dword0x09_realWidth & 1))
						{
							v24 = (x_DWORD*)(x_DWORD_E9C38_smalltit + 36968);
							goto LABEL_76;
						}
						v27++;
						v25 = (uint8_t*)(v122x - 1);
						while (1)
						{
							LOBYTE(v26) = *v23;
							v23 += v24[2];
							if ((x_BYTE)v26)
							{
								HIBYTE(v26) = v25[1];
								v25[1] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v26];
							}
							v25 += 2;
							v24 += 4;
							if (!--v27)
								break;
						LABEL_76:
							LOBYTE(v26) = *v23;
							v23 += *v24;
							if ((x_BYTE)v26)
							{
								HIBYTE(v26) = v25[0];
								v25[0] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v26];
							}
						}
						break;
					case 4:
						HIWORD(v31) = HIWORD(m_str_F2C20ar.dword0x07);
						v28 = (x_BYTE*)(m_str_F2C20ar.dword0x08_width * (m_str_F2C20ar.dword0x0a_actIdx >> 16) + m_str_F2C20ar.dword0x02_data + *(x_DWORD*)(x_DWORD_E9C38_smalltit + 36964));
						v29 = (x_DWORD*)(&x_DWORD_E9C38_smalltit[36960]);
						v30 = v122x;
						HIBYTE(v31) = m_str_F2C20ar.dword0x07;
						v32 = m_str_F2C20ar.dword0x09_realWidth >> 1;
						if (!(m_str_F2C20ar.dword0x09_realWidth & 1))
						{
							v29 = (x_DWORD*)(x_DWORD_E9C38_smalltit + 36968);
							goto LABEL_85;
						}
						v32++;
						v30 = (uint8_t*)(v122x - 1);
						while (1)
						{
							LOBYTE(v31) = *v28;
							v28 += v29[2];
							if ((x_BYTE)v31)
								v30[1] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v31];
							v30 += 2;
							v29 += 4;
							if (!--v32)
								break;
						LABEL_85:
							LOBYTE(v31) = *v28;
							v28 += *v29;
							if ((x_BYTE)v31)
								*v30 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v31];
						}
						break;
					case 5:
						v33 = m_str_F2C20ar.dword0x07;
						v34 = (x_BYTE*)(m_str_F2C20ar.dword0x08_width * (m_str_F2C20ar.dword0x0a_actIdx >> 16) + m_str_F2C20ar.dword0x02_data + *(x_DWORD*)(x_DWORD_E9C38_smalltit + 36964));
						v35 = (x_DWORD*)(&x_DWORD_E9C38_smalltit[36960]);
						v36 = v122x;
						v37 = m_str_F2C20ar.dword0x09_realWidth >> 1;
						if (!(m_str_F2C20ar.dword0x09_realWidth & 1))
						{
							v35 = (x_DWORD*)(x_DWORD_E9C38_smalltit + 36968);
							goto LABEL_94;
						}
						v37++;
						v36 = (uint8_t*)(v122x - 1);
						while (1)
						{
							BYTE1(v33) = *v34;
							v34 += v35[2];
							if (BYTE1(v33))
								v36[1] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v33];
							v36 += 2;
							v35 += 4;
							if (!--v37)
								break;
						LABEL_94:
							BYTE1(v33) = *v34;
							v34 += *v35;
							if (BYTE1(v33))
								*v36 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v33];
						}
						break;
					case 6:
						v38 = m_str_F2C20ar.dword0x00;
						v39 = (x_BYTE*)(m_str_F2C20ar.dword0x08_width * (m_str_F2C20ar.dword0x0a_actIdx >> 16) + m_str_F2C20ar.dword0x02_data + *(x_DWORD*)(x_DWORD_E9C38_smalltit + 36964));
						v40 = (x_DWORD*)(&x_DWORD_E9C38_smalltit[36960]);
						v41 = v122x;
						HIWORD(v42) = 0;
						v43 = m_str_F2C20ar.dword0x09_realWidth >> 1;
						if (!(m_str_F2C20ar.dword0x09_realWidth & 1))
						{
							v40 = (x_DWORD*)(x_DWORD_E9C38_smalltit + 36968);
							goto LABEL_103;
						}
						v43++;
						v41 = (uint8_t*)(v122x - 1);
						while (1)
						{
							BYTE1(v42) = *v39;
							v39 += v40[2];
							if (BYTE1(v42))
							{
								LOBYTE(v42) = v41[1];
								LOBYTE(v38) = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v42];
								v41[1] = m_ptrX_BYTE_F6EE0_tablesx[v38];
							}
							v41 += 2;
							v40 += 4;
							if (!--v43)
								break;
						LABEL_103:
							BYTE1(v42) = *v39;
							v39 += *v40;
							if (BYTE1(v42))
							{
								LOBYTE(v42) = *v41;
								LOBYTE(v38) = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v42];
								*v41 = m_ptrX_BYTE_F6EE0_tablesx[v38];
							}
						}
						break;
					case 7:
						v44 = m_str_F2C20ar.dword0x00;
						v45 = (x_BYTE*)(m_str_F2C20ar.dword0x08_width * (m_str_F2C20ar.dword0x0a_actIdx >> 16) + m_str_F2C20ar.dword0x02_data + *(x_DWORD*)(x_DWORD_E9C38_smalltit + 36964));
						v46 = (x_DWORD*)(&x_DWORD_E9C38_smalltit[36960]);
						v47 = v122x;
						HIWORD(v48) = 0;
						v49 = m_str_F2C20ar.dword0x09_realWidth >> 1;
						if (!(m_str_F2C20ar.dword0x09_realWidth & 1))
						{
							v46 = (x_DWORD*)(x_DWORD_E9C38_smalltit + 36968);
							goto LABEL_112;
						}
						v49++;
						v47 = (uint8_t*)(v122x - 1);
						while (1)
						{
							LOBYTE(v48) = *v45;
							v45 += v46[2];
							if ((x_BYTE)v48)
							{
								BYTE1(v48) = v47[1];
								LOBYTE(v44) = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v48];
								v47[1] = m_ptrX_BYTE_F6EE0_tablesx[v44];
							}
							v47 += 2;
							v46 += 4;
							if (!--v49)
								break;
						LABEL_112:
							LOBYTE(v48) = *v45;
							v45 += *v46;
							if ((x_BYTE)v48)
							{
								BYTE1(v48) = *v47;
								LOBYTE(v44) = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v48];
								*v47 = m_ptrX_BYTE_F6EE0_tablesx[v44];
							}
						}
						break;
					}
				}
				m_str_F2C20ar.dword0x0a_actIdx += v134;
				v122x += pitch;
				m_str_F2C20ar.dword0x0c_realHeight--;
			}
		}
	}
}

void GameRender::DrawTriangleInProjectionSpace_B6253(ProjectionPolygon* vertex1, ProjectionPolygon* vertex2, ProjectionPolygon* vertex3, uint8_t* pTexture, uint8_t unk_DE56Cx[], uint16_t viewPortWidth, uint16_t viewPortHeight, uint16_t pitch, uint8_t startLine, uint8_t drawEveryNthLine)
{
	uint8_t line1 = startLine;
	uint8_t line2 = startLine;
	uint8_t line3 = startLine;
	uint8_t line4 = startLine;
	uint8_t line5 = startLine;
	uint8_t line6 = startLine;
	uint8_t line7 = startLine;
	uint8_t line8 = startLine;
	uint8_t line9 = startLine;
	uint8_t line10 = startLine;
	uint8_t line11 = startLine;
	uint8_t line12 = startLine;
	uint8_t line13 = startLine;
	uint8_t line14 = startLine;
	uint8_t line15 = startLine;
	uint8_t line16 = startLine;
	uint8_t line17 = startLine;
	uint8_t line18 = startLine;
	uint8_t line19 = startLine;
	uint8_t line20 = startLine;
	uint8_t line21 = startLine;
	uint8_t line22 = startLine;
	uint8_t line23 = startLine;
	uint8_t line24 = startLine;
	uint8_t line25 = startLine;

	ProjectionPolygon* v3; // esi
	ProjectionPolygon* v4; // edi
	ProjectionPolygon* v5; // ecx
	int y1; // eax
	int y2; // ebx
	int y3; // edx
	int v9; // eax
	int v10; // ebx
	int v11; // ebx
	int v12; // ebx
	int v13; // ebx
	int v14; // eax
	int v15; // ebx
	bool v16; // zf
	bool v17; // sf
	unsigned __int8 v18; // of
	int v19; // ebx
	int v20; // ebx
	int v21; // eax
	int v22; // ebx
	int v23; // ecx
	int v24; // edx
	int v25; // esi
	int v26; // ST48_4
	int v27; // ebx
	x_DWORD* v28; // edi
	int v29; // edi
	int v30; // edi
	unsigned int v31; // eax
	int v32; // eax
	int v33; // ebx
	bool v34; // zf
	int v35; // ebx
	int v36; // ebx
	int v37; // eax
	int v38; // ebx
	int v39; // ecx
	int v40; // edx
	int v41; // ST48_4
	int v42; // ebx
	x_DWORD* v43; // edi
	int v44; // edi
	int v45; // edi
	int v46; // eax
	int v47; // ebx
	bool v48; // zf
	int v49; // ebx
	int v50; // eax
	int v51; // ebx
	int v52; // esi
	int v53; // ST48_4
	int v54; // ebx
	x_DWORD* v55; // edi
	int v56; // edi
	int v57; // edi
	int v58; // eax
	int v59; // ebx
	int v60; // ST48_4
	int v61; // ebx
	x_DWORD* v62; // edi
	int v63; // edi
	int v64; // edi
	int v65; // eax
	int v66; // ebx
	int v67; // ebx
	int v68; // ebx
	int v69; // eax
	int v70; // ebx
	bool v71; // zf
	int v72; // ebx
	int v73; // ebx
	int v74; // eax
	int v75; // ebx
	int v76; // ecx
	int v77; // edx
	int v78; // esi
	int v79; // ST48_4
	int v80; // eax
	int* v81; // edi
	int v82; // edi
	int v83; // edi
	int v84; // eax
	int v85; // ebx
	bool v86; // zf
	int v87; // ebx
	int v88; // ebx
	int v89; // eax
	int v90; // ebx
	int v91; // ecx
	int v92; // edx
	int v93; // ST48_4
	int v94; // eax
	int* v95; // edi
	int v96; // edi
	int v97; // edi
	int v98; // eax
	int v99; // ebx
	bool v100; // zf
	int v101; // ebx
	int v102; // eax
	int v103; // ebx
	int v104; // esi
	int v105; // ST48_4
	int v106; // eax
	int* v107; // edi
	int v108; // edi
	int v109; // edi
	int v110; // eax
	int v111; // ebx
	int v112; // ST48_4
	int v113; // eax
	int* v114; // edi
	int v115; // edi
	int v116; // edi
	int v117; // eax
	int v118; // ebx
	int v119; // ebx
	int v120; // eax
	int v121; // ebx
	int v122; // ecx
	int v123; // edx
	int v124; // esi
	int v125; // edi
	x_DWORD* v126; // edi
	int v127; // ebx
	int v128; // eax
	int v129; // ebx
	int v130; // ecx
	int v131; // edx
	int v132; // edi
	x_DWORD* v133; // edi
	int v134; // eax
	int v135; // ebx
	int v136; // esi
	int v137; // edi
	x_DWORD* v138; // edi
	int v139; // eax
	int v140; // ebx
	int v141; // edi
	x_DWORD* v142; // edi
	int v143; // eax
	int v144; // ebx
	int v145; // ebx
	int v146; // eax
	int v147; // ebx
	int v148; // ecx
	int v149; // edx
	int v150; // esi
	int v151; // edi
	x_DWORD* v152; // edi
	int v153; // ebx
	int v154; // eax
	int v155; // ebx
	int v156; // ecx
	int v157; // edx
	int v158; // edi
	x_DWORD* v159; // edi
	int v160; // eax
	int v161; // ebx
	int v162; // esi
	int v163; // edi
	x_DWORD* v164; // edi
	int v165; // eax
	int v166; // ebx
	int v167; // edi
	x_DWORD* v168; // edi
	unsigned __int16* v169; // esi
	char* v170; // edx
	char v171; // al
	int v172; // ebx
	signed int v173; // ecx
	char* v174; // edi
	unsigned __int16* v175; // esi
	int v176; // ecx
	x_BYTE* v177; // edi
	unsigned int v178; // eax
	__int16 v179; // bx
	unsigned __int8 v180; // cf
	unsigned __int16 v181; // bx
	__int16 v182; // bx
	char v183; // ah
	__int16 v184; // cx
	__int16 v185; // bx
	char v186; // ah
	__int16 v187; // cx
	__int16 v188; // bx
	char v189; // ah
	__int16 v190; // cx
	__int16 v191; // bx
	char v192; // ah
	__int16 v193; // cx
	__int16 v194; // bx
	char v195; // ah
	__int16 v196; // cx
	__int16 v197; // bx
	char v198; // ah
	__int16 v199; // cx
	__int16 v200; // bx
	char v201; // ah
	__int16 v202; // cx
	__int16 v203; // bx
	char v204; // ah
	__int16 v205; // cx
	__int16 v206; // bx
	char v207; // ah
	__int16 v208; // cx
	__int16 v209; // bx
	char v210; // ah
	__int16 v211; // cx
	__int16 v212; // bx
	char v213; // ah
	__int16 v214; // cx
	__int16 v215; // bx
	char v216; // ah
	__int16 v217; // cx
	__int16 v218; // bx
	char v219; // ah
	__int16 v220; // cx
	__int16 v221; // bx
	char v222; // ah
	__int16 v223; // cx
	__int16 v224; // bx
	char v225; // ah
	__int16 v226; // cx
	unsigned __int16* v227; // esi
	int v228; // eax
	int v229; // ebx
	int v230; // ecx
	x_BYTE* v231; // edi
	int v232; // eax
	unsigned int v233; // eax
	int v234; // edx
	unsigned int v235; // eax
	uint8_t* v236; // esi
	char v237; // al
	int v238; // edx
	__int16 v239; // cx
	char v240; // al
	int v241; // edx
	__int16 v242; // cx
	char v243; // al
	int v244; // edx
	__int16 v245; // cx
	char v246; // al
	int v247; // edx
	__int16 v248; // cx
	char v249; // al
	int v250; // edx
	__int16 v251; // cx
	char v252; // al
	int v253; // edx
	__int16 v254; // cx
	char v255; // al
	int v256; // edx
	__int16 v257; // cx
	char v258; // al
	int v259; // edx
	__int16 v260; // cx
	char v261; // al
	int v262; // edx
	__int16 v263; // cx
	char v264; // al
	int v265; // edx
	__int16 v266; // cx
	char v267; // al
	int v268; // edx
	__int16 v269; // cx
	char v270; // al
	int v271; // edx
	__int16 v272; // cx
	char v273; // al
	int v274; // edx
	__int16 v275; // cx
	char v276; // al
	int v277; // edx
	__int16 v278; // cx
	char v279; // al
	int v280; // edx
	__int16 v281; // cx
	char v282; // al
	unsigned __int16* v283; // esi
	int v284; // eax
	int v285; // ebx
	int v286; // ecx
	x_BYTE* v287; // edi
	int v288; // eax
	unsigned int v289; // eax
	int v290; // edx
	unsigned int v291; // eax
	uint8_t* v292; // esi
	char v293; // al
	int v294; // edx
	__int16 v295; // cx
	char v296; // al
	int v297; // edx
	__int16 v298; // cx
	char v299; // al
	int v300; // edx
	__int16 v301; // cx
	char v302; // al
	int v303; // edx
	__int16 v304; // cx
	char v305; // al
	int v306; // edx
	__int16 v307; // cx
	char v308; // al
	int v309; // edx
	__int16 v310; // cx
	char v311; // al
	int v312; // edx
	__int16 v313; // cx
	char v314; // al
	int v315; // edx
	__int16 v316; // cx
	char v317; // al
	int v318; // edx
	__int16 v319; // cx
	char v320; // al
	int v321; // edx
	__int16 v322; // cx
	char v323; // al
	int v324; // edx
	__int16 v325; // cx
	char v326; // al
	int v327; // edx
	__int16 v328; // cx
	char v329; // al
	int v330; // edx
	__int16 v331; // cx
	char v332; // al
	int v333; // edx
	__int16 v334; // cx
	char v335; // al
	int v336; // edx
	__int16 v337; // cx
	char v338; // al
	unsigned __int16* v339; // esi
	int v340; // ecx
	x_BYTE* v341; // edi
	unsigned int v342; // eax
	__int16 v343; // bx
	unsigned __int16 v344; // bx
	__int16 v345; // bx
	__int16 v346; // cx
	__int16 v347; // bx
	__int16 v348; // cx
	__int16 v349; // bx
	__int16 v350; // cx
	__int16 v351; // bx
	__int16 v352; // cx
	__int16 v353; // bx
	__int16 v354; // cx
	__int16 v355; // bx
	__int16 v356; // cx
	__int16 v357; // bx
	__int16 v358; // cx
	__int16 v359; // bx
	__int16 v360; // cx
	__int16 v361; // bx
	__int16 v362; // cx
	__int16 v363; // bx
	__int16 v364; // cx
	__int16 v365; // bx
	__int16 v366; // cx
	__int16 v367; // bx
	__int16 v368; // cx
	__int16 v369; // bx
	__int16 v370; // cx
	__int16 v371; // bx
	__int16 v372; // cx
	__int16 v373; // bx
	__int16 v374; // cx
	unsigned int v375; // eax
	int v376; // ebx
	char* v377; // esi
	int v378; // ecx
	uint8_t* v379; // edi
	int v380; // eax
	int v381; // ecx
	unsigned int v382; // eax
	int v383; // edx
	int v384; // ecx
	__int16 v385; // cx
	__int16 v386; // bp
	int v387; // esi
	x_BYTE* v388; // edi
	uint8_t* v389; // ebp
	unsigned int v390; // eax
	int v391; // ebx
	char* v392; // esi
	int v393; // ecx
	uint8_t* v394; // edi
	int v395; // eax
	int v396; // ecx
	unsigned int v397; // eax
	int v398; // edx
	int v399; // ecx
	__int16 v400; // cx
	__int16 v401; // bp
	int v402; // esi
	x_BYTE* v403; // edi
	uint8_t* v404; // ebp
	unsigned __int16* v405; // esi
	int v406; // eax
	int v407; // ebx
	int v408; // ecx
	x_BYTE* v409; // edi
	int v410; // eax
	unsigned int v411; // eax
	int v412; // edx
	unsigned int v413; // eax
	uint8_t* v414; // esi
	int v415; // edx
	__int16 v416; // cx
	int v417; // edx
	__int16 v418; // cx
	int v419; // edx
	__int16 v420; // cx
	int v421; // edx
	__int16 v422; // cx
	int v423; // edx
	__int16 v424; // cx
	int v425; // edx
	__int16 v426; // cx
	int v427; // edx
	__int16 v428; // cx
	int v429; // edx
	__int16 v430; // cx
	int v431; // edx
	__int16 v432; // cx
	int v433; // edx
	__int16 v434; // cx
	int v435; // edx
	__int16 v436; // cx
	int v437; // edx
	__int16 v438; // cx
	int v439; // edx
	__int16 v440; // cx
	int v441; // edx
	__int16 v442; // cx
	int v443; // edx
	__int16 v444; // cx
	unsigned __int16* v445; // esi
	int v446; // eax
	int v447; // ebx
	int v448; // ecx
	x_BYTE* v449; // edi
	int v450; // eax
	unsigned int v451; // eax
	int v452; // edx
	unsigned int v453; // eax
	uint8_t* v454; // esi
	int v455; // edx
	__int16 v456; // cx
	int v457; // edx
	__int16 v458; // cx
	int v459; // edx
	__int16 v460; // cx
	int v461; // edx
	__int16 v462; // cx
	int v463; // edx
	__int16 v464; // cx
	int v465; // edx
	__int16 v466; // cx
	int v467; // edx
	__int16 v468; // cx
	int v469; // edx
	__int16 v470; // cx
	int v471; // edx
	__int16 v472; // cx
	int v473; // edx
	__int16 v474; // cx
	int v475; // edx
	__int16 v476; // cx
	int v477; // edx
	__int16 v478; // cx
	int v479; // edx
	__int16 v480; // cx
	int v481; // edx
	__int16 v482; // cx
	int v483; // edx
	__int16 v484; // cx
	unsigned __int16* v485; // esi
	int v486; // eax
	int v487; // ebx
	int v488; // ecx
	x_BYTE* v489; // edi
	int v490; // eax
	unsigned int v491; // eax
	int v492; // edx
	unsigned int v493; // eax
	uint8_t* v494; // esi
	int v495; // edx
	__int16 v496; // cx
	int v497; // edx
	__int16 v498; // cx
	int v499; // edx
	__int16 v500; // cx
	int v501; // edx
	__int16 v502; // cx
	int v503; // edx
	__int16 v504; // cx
	int v505; // edx
	__int16 v506; // cx
	int v507; // edx
	__int16 v508; // cx
	int v509; // edx
	__int16 v510; // cx
	int v511; // edx
	__int16 v512; // cx
	int v513; // edx
	__int16 v514; // cx
	int v515; // edx
	__int16 v516; // cx
	int v517; // edx
	__int16 v518; // cx
	int v519; // edx
	__int16 v520; // cx
	int v521; // edx
	__int16 v522; // cx
	int v523; // edx
	__int16 v524; // cx
	unsigned __int16* v525; // esi
	int v526; // eax
	int v527; // ebx
	int v528; // ecx
	x_BYTE* v529; // edi
	int v530; // eax
	unsigned int v531; // eax
	int v532; // edx
	unsigned int v533; // eax
	uint8_t* v534; // esi
	int v535; // edx
	__int16 v536; // cx
	int v537; // edx
	__int16 v538; // cx
	int v539; // edx
	__int16 v540; // cx
	int v541; // edx
	__int16 v542; // cx
	int v543; // edx
	__int16 v544; // cx
	int v545; // edx
	__int16 v546; // cx
	int v547; // edx
	__int16 v548; // cx
	int v549; // edx
	__int16 v550; // cx
	int v551; // edx
	__int16 v552; // cx
	int v553; // edx
	__int16 v554; // cx
	int v555; // edx
	__int16 v556; // cx
	int v557; // edx
	__int16 v558; // cx
	int v559; // edx
	__int16 v560; // cx
	int v561; // edx
	__int16 v562; // cx
	int v563; // edx
	__int16 v564; // cx
	unsigned __int16* v565; // esi
	int v566; // eax
	int v567; // ebx
	int v568; // ecx
	x_BYTE* v569; // edi
	int v570; // eax
	unsigned int v571; // eax
	int v572; // edx
	unsigned int v573; // eax
	uint8_t* v574; // esi
	int v575; // edx
	__int16 v576; // cx
	int v577; // edx
	__int16 v578; // cx
	int v579; // edx
	__int16 v580; // cx
	int v581; // edx
	__int16 v582; // cx
	int v583; // edx
	__int16 v584; // cx
	int v585; // edx
	__int16 v586; // cx
	int v587; // edx
	__int16 v588; // cx
	int v589; // edx
	__int16 v590; // cx
	int v591; // edx
	__int16 v592; // cx
	int v593; // edx
	__int16 v594; // cx
	int v595; // edx
	__int16 v596; // cx
	int v597; // edx
	__int16 v598; // cx
	int v599; // edx
	__int16 v600; // cx
	int v601; // edx
	__int16 v602; // cx
	int v603; // edx
	__int16 v604; // cx
	unsigned __int16* v605; // esi
	x_BYTE* v606; // edx
	int v607; // eax
	int v608; // ebx
	int v609; // ecx
	x_BYTE* i; // edi
	__int16 v611; // cx
	__int16 v612; // cx
	__int16 v613; // cx
	__int16 v614; // cx
	__int16 v615; // cx
	__int16 v616; // cx
	__int16 v617; // cx
	__int16 v618; // cx
	__int16 v619; // cx
	__int16 v620; // cx
	__int16 v621; // cx
	__int16 v622; // cx
	__int16 v623; // cx
	__int16 v624; // cx
	__int16 v625; // cx
	unsigned __int16* v626; // esi
	x_BYTE* v627; // edx
	int v628; // eax
	int v629; // ebx
	int v630; // ecx
	x_BYTE* j; // edi
	__int16 v632; // cx
	__int16 v633; // cx
	__int16 v634; // cx
	__int16 v635; // cx
	__int16 v636; // cx
	__int16 v637; // cx
	__int16 v638; // cx
	__int16 v639; // cx
	__int16 v640; // cx
	__int16 v641; // cx
	__int16 v642; // cx
	__int16 v643; // cx
	__int16 v644; // cx
	__int16 v645; // cx
	__int16 v646; // cx
	unsigned __int16* v647; // esi
	int v648; // edx
	int v649; // ecx
	x_BYTE* v650; // edi
	unsigned int v651; // eax
	__int16 v652; // bx
	unsigned __int16 v653; // bx
	__int16 v654; // bx
	__int16 v655; // cx
	__int16 v656; // bx
	__int16 v657; // cx
	__int16 v658; // bx
	__int16 v659; // cx
	__int16 v660; // bx
	__int16 v661; // cx
	__int16 v662; // bx
	__int16 v663; // cx
	__int16 v664; // bx
	__int16 v665; // cx
	__int16 v666; // bx
	__int16 v667; // cx
	__int16 v668; // bx
	__int16 v669; // cx
	__int16 v670; // bx
	__int16 v671; // cx
	__int16 v672; // bx
	__int16 v673; // cx
	__int16 v674; // bx
	__int16 v675; // cx
	__int16 v676; // bx
	__int16 v677; // cx
	__int16 v678; // bx
	__int16 v679; // cx
	__int16 v680; // bx
	__int16 v681; // cx
	__int16 v682; // bx
	__int16 v683; // cx
	unsigned __int16* v684; // esi
	int v685; // edx
	int v686; // ecx
	x_BYTE* v687; // edi
	unsigned int v688; // eax
	__int16 v689; // bx
	unsigned __int16 v690; // bx
	__int16 v691; // bx
	__int16 v692; // cx
	__int16 v693; // bx
	__int16 v694; // cx
	__int16 v695; // bx
	__int16 v696; // cx
	__int16 v697; // bx
	__int16 v698; // cx
	__int16 v699; // bx
	__int16 v700; // cx
	__int16 v701; // bx
	__int16 v702; // cx
	__int16 v703; // bx
	__int16 v704; // cx
	__int16 v705; // bx
	__int16 v706; // cx
	__int16 v707; // bx
	__int16 v708; // cx
	__int16 v709; // bx
	__int16 v710; // cx
	__int16 v711; // bx
	__int16 v712; // cx
	__int16 v713; // bx
	__int16 v714; // cx
	__int16 v715; // bx
	__int16 v716; // cx
	__int16 v717; // bx
	__int16 v718; // cx
	__int16 v719; // bx
	__int16 v720; // cx
	unsigned __int16* v721; // esi
	int v722; // eax
	int v723; // ebx
	int v724; // ecx
	x_BYTE* v725; // edi
	int v726; // eax
	unsigned int v727; // eax
	int v728; // edx
	unsigned int v729; // eax
	uint8_t* v730; // esi
	int v731; // edx
	__int16 v732; // cx
	int v733; // edx
	__int16 v734; // cx
	int v735; // edx
	__int16 v736; // cx
	int v737; // edx
	__int16 v738; // cx
	int v739; // edx
	__int16 v740; // cx
	int v741; // edx
	__int16 v742; // cx
	int v743; // edx
	__int16 v744; // cx
	int v745; // edx
	__int16 v746; // cx
	int v747; // edx
	__int16 v748; // cx
	int v749; // edx
	__int16 v750; // cx
	int v751; // edx
	__int16 v752; // cx
	int v753; // edx
	__int16 v754; // cx
	int v755; // edx
	__int16 v756; // cx
	int v757; // edx
	__int16 v758; // cx
	int v759; // edx
	__int16 v760; // cx
	unsigned __int16* v761; // esi
	int v762; // eax
	int v763; // ebx
	int v764; // ecx
	x_BYTE* v765; // edi
	int v766; // eax
	unsigned int v767; // eax
	int v768; // edx
	unsigned int v769; // eax
	uint8_t* v770; // esi
	int v771; // edx
	__int16 v772; // cx
	int v773; // edx
	__int16 v774; // cx
	int v775; // edx
	__int16 v776; // cx
	int v777; // edx
	__int16 v778; // cx
	int v779; // edx
	__int16 v780; // cx
	int v781; // edx
	__int16 v782; // cx
	int v783; // edx
	__int16 v784; // cx
	int v785; // edx
	__int16 v786; // cx
	int v787; // edx
	__int16 v788; // cx
	int v789; // edx
	__int16 v790; // cx
	int v791; // edx
	__int16 v792; // cx
	int v793; // edx
	__int16 v794; // cx
	int v795; // edx
	__int16 v796; // cx
	int v797; // edx
	__int16 v798; // cx
	int v799; // edx
	__int16 v800; // cx
	unsigned __int16* v801; // esi
	unsigned int v802; // eax
	int v803; // ebx
	int v804; // ecx
	x_BYTE* v805; // edi
	int v806; // eax
	int v807; // ecx
	unsigned int v808; // eax
	int v809; // edx
	int v810; // ecx
	uint8_t* v811; // esi
	int v812; // edx
	int v813; // ecx
	int v814; // edx
	int v815; // ecx
	int v816; // edx
	int v817; // ecx
	int v818; // edx
	int v819; // ecx
	int v820; // edx
	int v821; // ecx
	int v822; // edx
	int v823; // ecx
	int v824; // edx
	int v825; // ecx
	int v826; // edx
	int v827; // ecx
	int v828; // edx
	int v829; // ecx
	int v830; // edx
	int v831; // ecx
	int v832; // edx
	int v833; // ecx
	int v834; // edx
	int v835; // ecx
	int v836; // edx
	int v837; // ecx
	int v838; // edx
	int v839; // ecx
	int v840; // edx
	int v841; // ecx
	unsigned __int16* v842; // esi
	unsigned int v843; // eax
	int v844; // ebx
	int v845; // ecx
	x_BYTE* v846; // edi
	int v847; // eax
	int v848; // ecx
	unsigned int v849; // eax
	int v850; // edx
	int v851; // ecx
	uint8_t* v852; // esi
	int v853; // edx
	int v854; // ecx
	int v855; // edx
	int v856; // ecx
	int v857; // edx
	int v858; // ecx
	int v859; // edx
	int v860; // ecx
	int v861; // edx
	int v862; // ecx
	int v863; // edx
	int v864; // ecx
	int v865; // edx
	int v866; // ecx
	int v867; // edx
	int v868; // ecx
	int v869; // edx
	int v870; // ecx
	int v871; // edx
	int v872; // ecx
	int v873; // edx
	int v874; // ecx
	int v875; // edx
	int v876; // ecx
	int v877; // edx
	int v878; // ecx
	int v879; // edx
	int v880; // ecx
	int v881; // edx
	int v882; // ecx
	unsigned __int16* v883; // esi
	int v884; // eax
	int v885; // ebx
	int v886; // ecx
	x_BYTE* v887; // edi
	int v888; // eax
	unsigned int v889; // eax
	int v890; // edx
	unsigned int v891; // eax
	uint8_t* v892; // esi
	int v893; // edx
	__int16 v894; // cx
	int v895; // edx
	__int16 v896; // cx
	int v897; // edx
	__int16 v898; // cx
	int v899; // edx
	__int16 v900; // cx
	int v901; // edx
	__int16 v902; // cx
	int v903; // edx
	__int16 v904; // cx
	int v905; // edx
	__int16 v906; // cx
	int v907; // edx
	__int16 v908; // cx
	int v909; // edx
	__int16 v910; // cx
	int v911; // edx
	__int16 v912; // cx
	int v913; // edx
	__int16 v914; // cx
	int v915; // edx
	__int16 v916; // cx
	int v917; // edx
	__int16 v918; // cx
	int v919; // edx
	__int16 v920; // cx
	int v921; // edx
	__int16 v922; // cx
	unsigned __int16* v923; // esi
	int v924; // eax
	int v925; // ebx
	int v926; // ecx
	x_BYTE* v927; // edi
	int v928; // eax
	unsigned int v929; // eax
	int v930; // edx
	unsigned int v931; // eax
	uint8_t* v932; // esi
	int v933; // edx
	__int16 v934; // cx
	int v935; // edx
	__int16 v936; // cx
	int v937; // edx
	__int16 v938; // cx
	int v939; // edx
	__int16 v940; // cx
	int v941; // edx
	__int16 v942; // cx
	int v943; // edx
	__int16 v944; // cx
	int v945; // edx
	__int16 v946; // cx
	int v947; // edx
	__int16 v948; // cx
	int v949; // edx
	__int16 v950; // cx
	int v951; // edx
	__int16 v952; // cx
	int v953; // edx
	__int16 v954; // cx
	int v955; // edx
	__int16 v956; // cx
	int v957; // edx
	__int16 v958; // cx
	int v959; // edx
	__int16 v960; // cx
	int v961; // edx
	__int16 v962; // cx
	unsigned __int16* v963; // esi
	unsigned int v964; // eax
	int v965; // ebx
	int v966; // ecx
	x_BYTE* v967; // edi
	int v968; // eax
	int v969; // ecx
	unsigned int v970; // eax
	int v971; // edx
	int v972; // ecx
	uint8_t* v973; // esi
	int v974; // edx
	int v975; // ecx
	int v976; // edx
	int v977; // ecx
	int v978; // edx
	int v979; // ecx
	int v980; // edx
	int v981; // ecx
	int v982; // edx
	int v983; // ecx
	int v984; // edx
	int v985; // ecx
	int v986; // edx
	int v987; // ecx
	int v988; // edx
	int v989; // ecx
	int v990; // edx
	int v991; // ecx
	int v992; // edx
	int v993; // ecx
	int v994; // edx
	int v995; // ecx
	int v996; // edx
	int v997; // ecx
	int v998; // edx
	int v999; // ecx
	int v1000; // edx
	int v1001; // ecx
	int v1002; // edx
	int v1003; // ecx
	unsigned __int16* v1004; // esi
	unsigned int v1005; // eax
	int v1006; // ebx
	int v1007; // ecx
	x_BYTE* v1008; // edi
	int v1009; // eax
	int v1010; // ecx
	unsigned int v1011; // eax
	int v1012; // edx
	int v1013; // ecx
	uint8_t* v1014; // esi
	int v1015; // edx
	int v1016; // ecx
	int v1017; // edx
	int v1018; // ecx
	int v1019; // edx
	int v1020; // ecx
	int v1021; // edx
	int v1022; // ecx
	int v1023; // edx
	int v1024; // ecx
	int v1025; // edx
	int v1026; // ecx
	int v1027; // edx
	int v1028; // ecx
	int v1029; // edx
	int v1030; // ecx
	int v1031; // edx
	int v1032; // ecx
	int v1033; // edx
	int v1034; // ecx
	int v1035; // edx
	int v1036; // ecx
	int v1037; // edx
	int v1038; // ecx
	int v1039; // edx
	int v1040; // ecx
	int v1041; // edx
	int v1042; // ecx
	int v1043; // edx
	int v1044; // ecx
	unsigned __int16* v1045; // esi
	unsigned int v1046; // eax
	int v1047; // ebx
	int v1048; // ecx
	char* v1049; // edi
	int v1050; // eax
	int v1051; // ecx
	unsigned int v1052; // eax
	int v1053; // edx
	int v1054; // ecx
	uint8_t* v1055; // esi
	char v1056; // al
	int v1057; // edx
	int v1058; // ecx
	char v1059; // al
	int v1060; // edx
	int v1061; // ecx
	char v1062; // al
	int v1063; // edx
	int v1064; // ecx
	char v1065; // al
	int v1066; // edx
	int v1067; // ecx
	char v1068; // al
	int v1069; // edx
	int v1070; // ecx
	char v1071; // al
	int v1072; // edx
	int v1073; // ecx
	char v1074; // al
	int v1075; // edx
	int v1076; // ecx
	char v1077; // al
	int v1078; // edx
	int v1079; // ecx
	char v1080; // al
	int v1081; // edx
	int v1082; // ecx
	char v1083; // al
	int v1084; // edx
	int v1085; // ecx
	char v1086; // al
	int v1087; // edx
	int v1088; // ecx
	char v1089; // al
	int v1090; // edx
	int v1091; // ecx
	char v1092; // al
	int v1093; // edx
	int v1094; // ecx
	char v1095; // al
	int v1096; // edx
	int v1097; // ecx
	char v1098; // al
	int v1099; // edx
	int v1100; // ecx
	char v1101; // al
	uint8_t* v1102; // [esp+0h] [ebp-88h]
	int v1103; // [esp+4h] [ebp-84h]
	int v1104; // [esp+4h] [ebp-84h]
	int v1105; // [esp+4h] [ebp-84h]
	int v1106; // [esp+4h] [ebp-84h]
	int v1107; // [esp+8h] [ebp-80h]
	int v1108; // [esp+8h] [ebp-80h]
	int v1109; // [esp+8h] [ebp-80h]
	int v1110; // [esp+8h] [ebp-80h]
	int v1111; // [esp+Ch] [ebp-7Ch]
	int v1112; // [esp+Ch] [ebp-7Ch]
	int v1113; // [esp+10h] [ebp-78h]
	int v1114; // [esp+10h] [ebp-78h]
	int v1115; // [esp+10h] [ebp-78h]
	int v1116; // [esp+10h] [ebp-78h]
	int v1117; // [esp+14h] [ebp-74h]
	int v1118; // [esp+14h] [ebp-74h]
	int v1119; // [esp+18h] [ebp-70h]
	int v1120; // [esp+18h] [ebp-70h]
	int v1121; // [esp+1Ch] [ebp-6Ch]
	int v1122; // [esp+1Ch] [ebp-6Ch]
	int triLn_v1123; // [esp+20h] [ebp-68h]
	int v1124; // [esp+24h] [ebp-64h]
	int v1125; // [esp+28h] [ebp-60h]
	int v1126; // [esp+28h] [ebp-60h]
	int v1127; // [esp+28h] [ebp-60h]
	int v1128; // [esp+28h] [ebp-60h]
	int v1129; // [esp+28h] [ebp-60h]
	int v1130; // [esp+28h] [ebp-60h]
	int v1131; // [esp+28h] [ebp-60h]
	int v1132; // [esp+28h] [ebp-60h]
	int v1133; // [esp+2Ch] [ebp-5Ch]
	int v1134; // [esp+2Ch] [ebp-5Ch]
	int v1135; // [esp+30h] [ebp-58h]
	int v1136; // [esp+34h] [ebp-54h]
	int v1137; // [esp+34h] [ebp-54h]
	int v1138; // [esp+34h] [ebp-54h]
	int v1139; // [esp+34h] [ebp-54h]
	int v1140; // [esp+34h] [ebp-54h]
	int v1141; // [esp+34h] [ebp-54h]
	int v1142; // [esp+34h] [ebp-54h]
	int v1143; // [esp+34h] [ebp-54h]
	int v1144; // [esp+38h] [ebp-50h]
	int v1145; // [esp+38h] [ebp-50h]
	int v1146; // [esp+3Ch] [ebp-4Ch]
	int v1147; // [esp+40h] [ebp-48h]
	int v1148; // [esp+40h] [ebp-48h]
	int v1149; // [esp+40h] [ebp-48h]
	int v1150; // [esp+40h] [ebp-48h]
	int v1151; // [esp+40h] [ebp-48h]
	int v1152; // [esp+40h] [ebp-48h]
	int v1153; // [esp+40h] [ebp-48h]
	int v1154; // [esp+40h] [ebp-48h]
	int v1155; // [esp+44h] [ebp-44h]
	int v1156; // [esp+44h] [ebp-44h]
	int v1157; // [esp+48h] [ebp-40h]
	int v1158; // [esp+48h] [ebp-40h]
	int v1159; // [esp+48h] [ebp-40h]
	int v1160; // [esp+48h] [ebp-40h]
	int v1161; // [esp+48h] [ebp-40h]
	int v1162; // [esp+48h] [ebp-40h]
	int v1163; // [esp+48h] [ebp-40h]
	int v1164; // [esp+48h] [ebp-40h]
	int v1165; // [esp+4Ch] [ebp-3Ch]
	int v1166; // [esp+4Ch] [ebp-3Ch]
	int v1167; // [esp+4Ch] [ebp-3Ch]
	int v1168; // [esp+4Ch] [ebp-3Ch]
	int v1169; // [esp+4Ch] [ebp-3Ch]
	int v1170; // [esp+4Ch] [ebp-3Ch]
	int v1171; // [esp+4Ch] [ebp-3Ch]
	int v1172; // [esp+4Ch] [ebp-3Ch]
	int v1173; // [esp+4Ch] [ebp-3Ch]
	int v1174; // [esp+4Ch] [ebp-3Ch]
	int v1175; // [esp+4Ch] [ebp-3Ch]
	int v1176; // [esp+4Ch] [ebp-3Ch]
	int v1177; // [esp+4Ch] [ebp-3Ch]
	int v1178; // [esp+4Ch] [ebp-3Ch]
	int v1179; // [esp+4Ch] [ebp-3Ch]
	int v1180; // [esp+4Ch] [ebp-3Ch]
	int v1181; // [esp+4Ch] [ebp-3Ch]
	int v1182; // [esp+4Ch] [ebp-3Ch]
	int v1183; // [esp+50h] [ebp-38h]
	int v1184; // [esp+50h] [ebp-38h]
	int v1185; // [esp+50h] [ebp-38h]
	int v1186; // [esp+50h] [ebp-38h]
	int v1187; // [esp+50h] [ebp-38h]
	int v1188; // [esp+50h] [ebp-38h]
	int v1189; // [esp+50h] [ebp-38h]
	int v1190; // [esp+54h] [ebp-34h]
	int v1191; // [esp+54h] [ebp-34h]
	int v1192; // [esp+54h] [ebp-34h]
	int v1193; // [esp+54h] [ebp-34h]
	int v1194; // [esp+58h] [ebp-30h]
	int v1195; // [esp+58h] [ebp-30h]
	int v1196; // [esp+58h] [ebp-30h]
	int v1197; // [esp+58h] [ebp-30h]
	int v1198; // [esp+58h] [ebp-30h]
	int v1199; // [esp+58h] [ebp-30h]
	int v1200; // [esp+58h] [ebp-30h]
	int v1201; // [esp+58h] [ebp-30h]
	int v1202; // [esp+58h] [ebp-30h]
	int v1203; // [esp+58h] [ebp-30h]
	int v1204; // [esp+58h] [ebp-30h]
	int v1205; // [esp+58h] [ebp-30h]
	int v1206; // [esp+58h] [ebp-30h]
	int v1207; // [esp+58h] [ebp-30h]
	int v1208; // [esp+58h] [ebp-30h]
	int v1209; // [esp+58h] [ebp-30h]
	int v1210; // [esp+58h] [ebp-30h]
	int v1211; // [esp+58h] [ebp-30h]
	int v1212; // [esp+58h] [ebp-30h]
	int v1213; // [esp+58h] [ebp-30h]
	int v1214; // [esp+58h] [ebp-30h]
	int v1215; // [esp+58h] [ebp-30h]
	int v1216; // [esp+58h] [ebp-30h]
	int v1217; // [esp+58h] [ebp-30h]
	int v1218; // [esp+58h] [ebp-30h]
	int v1219; // [esp+58h] [ebp-30h]
	int v1220; // [esp+58h] [ebp-30h]
	int v1221; // [esp+58h] [ebp-30h]
	int v1222; // [esp+58h] [ebp-30h]
	int v1223; // [esp+58h] [ebp-30h]
	int v1224; // [esp+58h] [ebp-30h]
	int v1225; // [esp+58h] [ebp-30h]
	int v1226; // [esp+58h] [ebp-30h]
	int v1227; // [esp+58h] [ebp-30h]
	int v1228; // [esp+58h] [ebp-30h]
	int v1229; // [esp+58h] [ebp-30h]
	int v1230; // [esp+58h] [ebp-30h]
	int v1231; // [esp+58h] [ebp-30h]
	int v1232; // [esp+58h] [ebp-30h]
	int v1233; // [esp+58h] [ebp-30h]
	int v1234; // [esp+58h] [ebp-30h]
	int v1235; // [esp+58h] [ebp-30h]
	int v1236; // [esp+58h] [ebp-30h]
	int v1237; // [esp+58h] [ebp-30h]
	int v1238; // [esp+58h] [ebp-30h]
	int v1239; // [esp+58h] [ebp-30h]
	int v1240; // [esp+58h] [ebp-30h]
	int v1241; // [esp+58h] [ebp-30h]
	int v1242; // [esp+58h] [ebp-30h]
	int v1243; // [esp+58h] [ebp-30h]
	int v1244; // [esp+58h] [ebp-30h]
	int v1245; // [esp+58h] [ebp-30h]
	int v1246; // [esp+58h] [ebp-30h]
	int v1247; // [esp+58h] [ebp-30h]
	int v1248; // [esp+58h] [ebp-30h]
	int v1249; // [esp+58h] [ebp-30h]
	int v1250; // [esp+58h] [ebp-30h]
	int v1251; // [esp+58h] [ebp-30h]
	int v1252; // [esp+58h] [ebp-30h]
	int v1253; // [esp+58h] [ebp-30h]
	int v1254; // [esp+58h] [ebp-30h]
	int v1255; // [esp+58h] [ebp-30h]
	int v1256; // [esp+58h] [ebp-30h]
	int v1257; // [esp+58h] [ebp-30h]
	int v1258; // [esp+58h] [ebp-30h]
	int v1259; // [esp+58h] [ebp-30h]
	int v1260; // [esp+58h] [ebp-30h]
	int v1261; // [esp+58h] [ebp-30h]
	int v1262; // [esp+58h] [ebp-30h]
	int v1263; // [esp+58h] [ebp-30h]
	int v1264; // [esp+58h] [ebp-30h]
	int v1265; // [esp+58h] [ebp-30h]
	int v1266; // [esp+58h] [ebp-30h]
	int v1267; // [esp+58h] [ebp-30h]
	int v1268; // [esp+58h] [ebp-30h]
	int v1269; // [esp+58h] [ebp-30h]
	int v1270; // [esp+58h] [ebp-30h]
	int v1271; // [esp+58h] [ebp-30h]
	int v1272; // [esp+58h] [ebp-30h]
	int v1273; // [esp+58h] [ebp-30h]
	unsigned __int16* v1274; // [esp+5Ch] [ebp-2Ch]
	unsigned __int16* v1275; // [esp+5Ch] [ebp-2Ch]
	char* v1276; // [esp+5Ch] [ebp-2Ch]
	char* v1277; // [esp+5Ch] [ebp-2Ch]
	unsigned __int16* v1278; // [esp+5Ch] [ebp-2Ch]
	unsigned __int16* v1279; // [esp+5Ch] [ebp-2Ch]
	unsigned __int16* v1280; // [esp+5Ch] [ebp-2Ch]
	unsigned __int16* v1281; // [esp+5Ch] [ebp-2Ch]
	unsigned __int16* v1282; // [esp+5Ch] [ebp-2Ch]
	unsigned __int16* v1283; // [esp+5Ch] [ebp-2Ch]
	unsigned __int16* v1284; // [esp+5Ch] [ebp-2Ch]
	unsigned __int16* v1285; // [esp+5Ch] [ebp-2Ch]
	unsigned __int16* v1286; // [esp+5Ch] [ebp-2Ch]
	unsigned __int16* v1287; // [esp+5Ch] [ebp-2Ch]
	unsigned __int16* v1288; // [esp+5Ch] [ebp-2Ch]
	unsigned __int16* v1289; // [esp+5Ch] [ebp-2Ch]
	unsigned __int16* v1290; // [esp+5Ch] [ebp-2Ch]
	unsigned __int16* v1291; // [esp+5Ch] [ebp-2Ch]
	char v1292; // [esp+62h] [ebp-26h]
	char v1293; // [esp+62h] [ebp-26h]
	char v1294; // [esp+62h] [ebp-26h]
	char v1295; // [esp+62h] [ebp-26h]
	char v1296; // [esp+63h] [ebp-25h]
	char v1297; // [esp+63h] [ebp-25h]
	bool v1298; // [esp+63h] [ebp-25h]
	bool v1299; // [esp+63h] [ebp-25h]
	bool v1300; // [esp+64h] [ebp-24h]
	bool v1301; // [esp+64h] [ebp-24h]

	/*if(debugafterload)
	{
		if (debugcounter_297253 >= 0x37) {
			debugcounter_297253++;
			debugcounter_297253--;
		}
		debugcounter_297253++;
	}*/
	//add_compare(0x297257, debugafterload,0x37);

	//fix it
	v1045 = 0;
	v1124 = 0;
	v1135 = 0;
	//fix it

	v3 = vertex1;
	v4 = vertex2;
	v5 = vertex3;
	y1 = vertex1->Y;
	y2 = vertex2->Y;
	y3 = vertex3->Y;

	if (y1 == y2)
	{
		if (y1 == y3)
			return;
		if (y1 >= y3)
		{
			if (vertex1->X <= vertex2->X)
				return;
			v3 = vertex3;
			v4 = vertex1;
			v5 = vertex2;
			goto LABEL_234;
		}
		if (vertex2->X <= vertex1->X)
			return;
		goto LABEL_277;
	}
	if (y1 <= y2)
	{
		if (y1 != y3)
		{
			if (y1 >= y3)
			{
				v3 = vertex3;
				v4 = vertex1;
				v5 = vertex2;
			LABEL_24:
				v9 = v3->Y;
				v1190 = v9;
				if (v9 >= 0)
				{
					if (v9 >= viewPortHeight)
						return;
					v1102 = m_ptrViewPortRenderBufferAltStart + pitch * v9;
					v1292 = 0;
				}
				else
				{
					v1102 = m_ptrViewPortRenderBufferAltStart;
					v1292 = 1;
				}
				v10 = v5->Y;
				v1300 = v10 > viewPortHeight;
				v11 = v10 - v9;
				v1113 = v11;
				triLn_v1123 = v11;
				v12 = v4->Y;
				v1296 = v12 > viewPortHeight;
				v13 = v12 - v9;
				v1117 = v13;
				v1103 = ((v5->X - v3->X) << 16) / v1113;
				if (((v4->X - v3->X) << 16) / v13 > v1103)
				{
					v1107 = ((v4->X - v3->X) << 16) / v13;
					v1111 = ((v5->X - v4->X) << 16) / (v5->Y - v4->Y);
					v1119 = v5->Y - v4->Y;
					v1121 = v4->X << 16;
					switch (x_BYTE_E126D)
					{
					case 0:
					case 0xE:
					case 0xF:
						v58 = v3->X << 16;
						v59 = v3->X << 16;
						if (!v1292)
						{
							if (v1300)
							{
								v63 = viewPortHeight - v1190;
								triLn_v1123 = viewPortHeight - v1190;
								if (v1296)
								{
									v1117 = viewPortHeight - v1190;
								}
								else
								{
									v18 = __OFSUB__(v63, v1117);
									v64 = v63 - v1117;
									v1296 = (v64 < 0) ^ v18 | (v64 == 0);
									v1119 = v64;
								}
							}
							goto LABEL_121;
						}
						v18 = __OFSUB__(triLn_v1123, -v1190);
						v16 = triLn_v1123 == -v1190;
						v17 = triLn_v1123 + v1190 < 0;
						triLn_v1123 += v1190;
						if (!((unsigned __int8)(v17 ^ v18) | (unsigned __int8)v16))
						{
							v1160 = -v1190;
							if (-v1190 - v1117 >= 0)
							{
								v1119 -= v1160 - v1117;
								v60 = v1160 - v1117;
								v58 += v1103 * v60 + v1117 * v1103;
								v61 = v1111 * v60 + v1121;
								if (v1300)
								{
									v1119 = viewPortHeight;
									triLn_v1123 = viewPortHeight;
								}
								v62 = (x_DWORD*)unk_DE56Cx;
								goto LABEL_124;
							}
							v1117 += v1190;
							v58 += v1103 * v1160;
							v59 += v1160 * v1107;
							if (v1300)
							{
								triLn_v1123 = viewPortHeight;
								if (v1296)
								{
									v1117 = viewPortHeight;
								}
								else
								{
									v1296 = viewPortHeight <= v1117;
									v1119 = viewPortHeight - v1117;
								}
							}
						LABEL_121:
							v62 = LoadPolygon((x_DWORD*)unk_DE56Cx, &v58, &v59, v1103, v1107, &v1117);
							v61 = v1121;
						LABEL_124:
							if (v1296)
							{
								v31 = (unsigned __int8)x_BYTE_E126D;
							}
							else
							{
								v62 = LoadPolygon(v62, &v58, &v61, v1103, v1111, &v1119);
								v31 = (unsigned __int8)x_BYTE_E126D;
							}
							goto LABEL_53;
						}
						return;
					case 1:
					case 4:
					case 0x10:
					case 0x11:
						v46 = v13 * (signed __int64)(v3->X - v5->X) / v1113;
						v47 = v4->X - v3->X;
						v18 = __OFADD__(v46, v47);
						v48 = v46 + v47 == 0;
						v17 = v46 + v47 < 0;
						v49 = v46 + v47;
						if ((unsigned __int8)v17 ^ v18)
							return;
						if (!v48)
							v1146 = (signed int)(v4->Brightness + (unsigned __int64)(v1117 * (signed __int64)(v3->Brightness - v5->Brightness) / v1113) - v3->Brightness)
							/ (v49 + 1);
						v1148 = (v5->Brightness - v3->Brightness) / v1113;
						v50 = v3->X << 16;
						v51 = v3->X << 16;
						v52 = v3->Brightness;
						if (v1292)
						{
							v18 = __OFSUB__(triLn_v1123, -v1190);
							v16 = triLn_v1123 == -v1190;
							v17 = triLn_v1123 + v1190 < 0;
							triLn_v1123 += v1190;
							if ((unsigned __int8)(v17 ^ v18) | (unsigned __int8)v16)
								return;
							v1159 = -v1190;
							if (-v1190 - v1117 >= 0)
							{
								v1119 -= v1159 - v1117;
								v53 = v1159 - v1117;
								v50 += v1103 * v53 + v1117 * v1103;
								v54 = v1111 * v53 + v1121;
								v52 += v53 * v1148 + v1117 * v1148;
								if (v1300)
								{
									v1119 = viewPortHeight;
									triLn_v1123 = viewPortHeight;
								}
								v55 = (x_DWORD*)unk_DE56Cx;
							LABEL_102:
								if (v1296)
								{
									v31 = (unsigned __int8)x_BYTE_E126D;
								}
								else
								{
									v55 = LoadPolygon(v55, &v50, &v54, &v52, v1103, v1111, v1148, &v1119);
									v31 = (unsigned __int8)x_BYTE_E126D;
								}
								goto LABEL_53;
							}
							v1117 += v1190;
							v50 += v1103 * v1159;
							v51 += v1159 * v1107;
							v52 += v1159 * v1148;
							if (v1300)
							{
								triLn_v1123 = viewPortHeight;
								if (v1296)
								{
									v1117 = viewPortHeight;
								}
								else
								{
									v1296 = viewPortHeight <= v1117;
									v1119 = viewPortHeight - v1117;
								}
							}
						}
						else if (v1300)
						{
							v56 = viewPortHeight - v1190;
							triLn_v1123 = viewPortHeight - v1190;
							if (v1296)
							{
								v1117 = viewPortHeight - v1190;
							}
							else
							{
								v18 = __OFSUB__(v56, v1117);
								v57 = v56 - v1117;
								v1296 = (v57 < 0) ^ v18 | (v57 == 0);
								v1119 = v57;
							}
						}
						v55 = LoadPolygon((x_DWORD*)unk_DE56Cx, &v50, &v51, &v52, v1103, v1107, v1148, &v1117);
						v54 = v1121;
						goto LABEL_102;
					case 2:
					case 3:
					case 7:
					case 8:
					case 9:
					case 0xA:
					case 0xB:
					case 0xC:
					case 0xD:
					case 0x12:
					case 0x13:
					case 0x16:
					case 0x17:
						v32 = v13 * (signed __int64)(v3->X - v5->X) / v1113;
						v33 = v4->X - v3->X;
						v18 = __OFADD__(v32, v33);
						v34 = v32 + v33 == 0;
						v17 = v32 + v33 < 0;
						v35 = v32 + v33;
						if ((unsigned __int8)v17 ^ v18)
							return;
						if (!v34)
						{
							v36 = v35 + 1;
							v1124 = (signed int)(v4->U + (unsigned __int64)(v1117 * (signed __int64)(v3->U - v5->U) / v1113) - v3->U)
								/ v36;
							v1135 = (signed int)(v4->V + (unsigned __int64)(v1117 * (signed __int64)(v3->V - v5->V) / v1113) - v3->V)
								/ v36;
						}
						v1126 = (v5->U - v3->U) / v1113;
						v1137 = (v5->V - v3->V) / v1113;
						v37 = v3->X << 16;
						v38 = v3->X << 16;
						v39 = v3->U;
						v40 = v3->V;
						if (v1292)
						{
							v18 = __OFSUB__(triLn_v1123, -v1190);
							v16 = triLn_v1123 == -v1190;
							v17 = triLn_v1123 + v1190 < 0;
							triLn_v1123 += v1190;
							if ((unsigned __int8)(v17 ^ v18) | (unsigned __int8)v16)
								return;
							v1158 = -v1190;
							if (-v1190 - v1117 >= 0)
							{
								v1119 -= v1158 - v1117;
								v41 = v1158 - v1117;
								v37 += v1103 * v41 + v1117 * v1103;
								v42 = v1111 * v41 + v1121;
								v39 += v41 * v1126 + v1117 * v1126;
								v40 += v41 * v1137 + v1117 * v1137;
								if (v1300)
								{
									v1119 = viewPortHeight;
									triLn_v1123 = viewPortHeight;
								}
								v43 = (x_DWORD*)unk_DE56Cx;
							LABEL_77:
								if (v1296)
								{
									v31 = (unsigned __int8)x_BYTE_E126D;
								}
								else
								{
									v43 = LoadPolygon(v43, &v37, &v42, &v39, &v40, v1103, v1111, v1126, v1137, &v1119);
									v31 = (unsigned __int8)x_BYTE_E126D;
								}
								goto LABEL_53;
							}
							v1117 += v1190;
							v37 += v1103 * v1158;
							v38 += v1158 * v1107;
							v39 += v1158 * v1126;
							v40 += v1158 * v1137;
							if (v1300)
							{
								triLn_v1123 = viewPortHeight;
								if (v1296)
								{
									v1117 = viewPortHeight;
								}
								else
								{
									v1296 = viewPortHeight <= v1117;
									v1119 = viewPortHeight - v1117;
								}
							}
						}
						else if (v1300)
						{
							v44 = viewPortHeight - v1190;
							triLn_v1123 = viewPortHeight - v1190;
							if (v1296)
							{
								v1117 = viewPortHeight - v1190;
							}
							else
							{
								v18 = __OFSUB__(v44, v1117);
								v45 = v44 - v1117;
								v1296 = (v45 < 0) ^ v18 | (v45 == 0);
								v1119 = v45;
							}
						}
						v43 = LoadPolygon((x_DWORD*)unk_DE56Cx, &v37, &v38, &v39, &v40, v1103, v1107, v1126, v1137, &v1117);
						v42 = v1121;
						goto LABEL_77;
					case 5:
					case 6:
					case 0x14:
					case 0x15:
					case 0x18:
					case 0x19:
					case 0x1A:
						v14 = v13 * (signed __int64)(v3->X - v5->X) / v1113;
						v15 = v4->X - v3->X;
						v18 = __OFADD__(v14, v15);
						v16 = v14 + v15 == 0;
						v17 = v14 + v15 < 0;
						v19 = v14 + v15;
						if ((unsigned __int8)v17 ^ v18)
							return;
						if (!v16)
						{
							v20 = v19 + 1;
							v1124 = (signed int)(v4->U + (unsigned __int64)(v1117 * (signed __int64)(v3->U - v5->U) / v1113) - v3->U)
								/ v20;
							v1135 = (signed int)(v4->V + (unsigned __int64)(v1117 * (signed __int64)(v3->V - v5->V) / v1113) - v3->V)
								/ v20;
							v1146 = (signed int)(v4->Brightness + (unsigned __int64)(v1117 * (signed __int64)(v3->Brightness - v5->Brightness) / v1113) - v3->Brightness)
								/ v20;
						}
						v1125 = (v5->U - v3->U) / v1113;
						v1136 = (v5->V - v3->V) / v1113;
						v1147 = (v5->Brightness - v3->Brightness) / v1113;
						v21 = v3->X << 16;
						v22 = v3->X << 16;
						v23 = v3->U;
						v24 = v3->V;
						v25 = v3->Brightness;
						if (v1292)
						{
							v18 = __OFSUB__(triLn_v1123, -v1190);
							v16 = triLn_v1123 == -v1190;
							v17 = triLn_v1123 + v1190 < 0;
							triLn_v1123 += v1190;
							if ((unsigned __int8)(v17 ^ v18) | (unsigned __int8)v16)
								return;
							v1157 = -v1190;
							if (-v1190 - v1117 >= 0)
							{
								v1119 -= v1157 - v1117;
								v26 = v1157 - v1117;
								v21 += v1103 * v26 + v1117 * v1103;
								v27 = v1111 * v26 + v1121;
								v23 += v26 * v1125 + v1117 * v1125;
								v24 += v26 * v1136 + v1117 * v1136;
								v25 += v26 * v1147 + v1117 * v1147;
								if (v1300)
								{
									v1119 = viewPortHeight;
									triLn_v1123 = viewPortHeight;
								}
								v28 = (x_DWORD*)unk_DE56Cx;
							LABEL_51:
								if (v1296)
								{
									v31 = (unsigned __int8)x_BYTE_E126D;
								}
								else
								{
									v28 = LoadPolygon(v28, &v21, &v27, &v23, &v24, &v25, v1103, v1111, v1125, v1136, v1147, &v1119);
									v31 = (unsigned __int8)x_BYTE_E126D;
								}
								goto LABEL_53;
							}
							v1117 += v1190;
							v21 += v1103 * v1157;
							v22 += v1157 * v1107;
							v23 += v1157 * v1125;
							v24 += v1157 * v1136;
							v25 += v1157 * v1147;
							if (v1300)
							{
								triLn_v1123 = viewPortHeight;
								if (v1296)
								{
									v1117 = viewPortHeight;
								}
								else
								{
									v1296 = viewPortHeight <= v1117;
									v1119 = viewPortHeight - v1117;
								}
							}
						}
						else if (v1300)
						{
							v29 = viewPortHeight - v1190;
							triLn_v1123 = viewPortHeight - v1190;
							if (v1296)
							{
								v1117 = viewPortHeight - v1190;
							}
							else
							{
								v18 = __OFSUB__(v29, v1117);
								v30 = v29 - v1117;
								v1296 = (v30 < 0) ^ v18 | (v30 == 0);
								v1119 = v30;
							}
						}
						v28 = LoadPolygon((x_DWORD*)unk_DE56Cx, &v21, &v22, &v23, &v24, &v25, v1103, v1107, v1125, v1136, v1147, &v1117);
						v27 = v1121;
						goto LABEL_51;
					}
				}
				return;
			}
			if (y2 != y3)
			{
				if (y2 <= y3)
					goto LABEL_24;
				goto LABEL_129;
			}
			if (vertex2->X <= vertex3->X)
				return;
		LABEL_234:
			v117 = v3->Y;
			v1192 = v117;
			if (v117 >= 0)
			{
				if (v117 >= viewPortHeight)
					return;
				v1102 = m_ptrViewPortRenderBufferAltStart + pitch * v117;
				v1294 = 0;
			}
			else
			{
				v1102 = m_ptrViewPortRenderBufferAltStart;
				v1294 = 1;
			}
			v118 = v5->Y;
			v1298 = v118 > viewPortHeight;
			v1115 = v118 - v117;
			triLn_v1123 = v118 - v117;
			v1105 = ((v5->X - v3->X) << 16) / (v118 - v117);
			v1109 = ((v4->X - v3->X) << 16) / (v118 - v117);
			switch (x_BYTE_E126D)
			{
			case 0:
			case 0xE:
			case 0xF:
				v139 = v3->X << 16;
				v140 = v3->X << 16;
				if (v1294)
				{
					v141 = -v1192;
					v1115 += v1192;
					v18 = __OFSUB__(triLn_v1123, -v1192);
					v16 = triLn_v1123 == -v1192;
					v17 = triLn_v1123 + v1192 < 0;
					triLn_v1123 += v1192;
					if ((unsigned __int8)(v17 ^ v18) | (unsigned __int8)v16)
						return;
					v139 += v1105 * v141;
					v140 += v141 * v1109;
					if (v1298)
					{
						triLn_v1123 = viewPortHeight;
						v1115 = viewPortHeight;
					}
				}
				else if (v1298)
				{
					triLn_v1123 = viewPortHeight - v1192;
					v1115 = viewPortHeight - v1192;
				}
				v142 = LoadPolygon((x_DWORD*)unk_DE56Cx, &v139, &v140, v1105, v1109, &v1115);
				v31 = (unsigned __int8)x_BYTE_E126D;
				goto LABEL_53;
			case 1:
			case 4:
			case 0x10:
			case 0x11:
				v1146 = (v4->Brightness - v5->Brightness) / (v4->X - v5->X);
				v1152 = (v5->Brightness - v3->Brightness) / triLn_v1123;
				v134 = v3->X << 16;
				v135 = v3->X << 16;
				v136 = v3->Brightness;
				if (v1294)
				{
					v137 = -v1192;
					v1115 += v1192;
					v18 = __OFSUB__(triLn_v1123, -v1192);
					v16 = triLn_v1123 == -v1192;
					v17 = triLn_v1123 + v1192 < 0;
					triLn_v1123 += v1192;
					if ((unsigned __int8)(v17 ^ v18) | (unsigned __int8)v16)
						return;
					v134 += v1105 * v137;
					v135 += v137 * v1109;
					v136 += v137 * v1152;
					if (v1298)
					{
						triLn_v1123 = viewPortHeight;
						v1115 = viewPortHeight;
					}
				}
				else if (v1298)
				{
					triLn_v1123 = viewPortHeight - v1192;
					v1115 = viewPortHeight - v1192;
				}
				v138 = LoadPolygon((x_DWORD*)unk_DE56Cx, &v134, &v135, &v136, v1105, v1109, v1152, &v1115);
				v31 = (unsigned __int8)x_BYTE_E126D;
				goto LABEL_53;
			case 2:
			case 3:
			case 7:
			case 8:
			case 9:
			case 0xA:
			case 0xB:
			case 0xC:
			case 0xD:
			case 0x12:
			case 0x13:
			case 0x16:
			case 0x17:
				v127 = v4->X - v5->X;
				v1124 = (v4->U - v5->U) / v127;
				v1135 = (v4->V - v5->V) / v127;
				v1130 = (v5->U - v3->U) / triLn_v1123;
				v1141 = (v5->V - v3->V) / triLn_v1123;
				v128 = v3->X << 16;
				v129 = v3->X << 16;
				v130 = v3->U;
				v131 = v3->V;
				if (v1294)
				{
					v132 = -v1192;
					v1115 += v1192;
					v18 = __OFSUB__(triLn_v1123, -v1192);
					v16 = triLn_v1123 == -v1192;
					v17 = triLn_v1123 + v1192 < 0;
					triLn_v1123 += v1192;
					if ((unsigned __int8)(v17 ^ v18) | (unsigned __int8)v16)
						return;
					v128 += v1105 * v132;
					v129 += v132 * v1109;
					v130 += v132 * v1130;
					v131 += v132 * v1141;
					if (v1298)
					{
						triLn_v1123 = viewPortHeight;
						v1115 = viewPortHeight;
					}
				}
				else if (v1298)
				{
					triLn_v1123 = viewPortHeight - v1192;
					v1115 = viewPortHeight - v1192;
				}
				v133 = LoadPolygon((x_DWORD*)unk_DE56Cx, &v128, &v129, &v130, &v131, v1105, v1109, v1130, v1141, &v1115);
				v31 = (unsigned __int8)x_BYTE_E126D;
				goto LABEL_53;
			case 5:
			case 6:
			case 0x14:
			case 0x15:
			case 0x18:
			case 0x19:
			case 0x1A:
				v119 = v4->X - v5->X;
				v1124 = (v4->U - v5->U) / v119;
				v1135 = (v4->V - v5->V) / v119;
				v1146 = (v4->Brightness - v5->Brightness) / v119;
				v1129 = (v5->U - v3->U) / triLn_v1123;
				v1140 = (v5->V - v3->V) / triLn_v1123;
				v1151 = (v5->Brightness - v3->Brightness) / triLn_v1123;
				v120 = v3->X << 16;
				v121 = v3->X << 16;
				v122 = v3->U;
				v123 = v3->V;
				v124 = v3->Brightness;
				if (v1294)
				{
					v125 = -v1192;
					v1115 += v1192;
					v18 = __OFSUB__(triLn_v1123, -v1192);
					v16 = triLn_v1123 == -v1192;
					v17 = triLn_v1123 + v1192 < 0;
					triLn_v1123 += v1192;
					if ((unsigned __int8)(v17 ^ v18) | (unsigned __int8)v16)
						return;
					v120 += v1105 * v125;
					v121 += v125 * v1109;
					v122 += v125 * v1129;
					v123 += v125 * v1140;
					v124 += v125 * v1151;
					if (v1298)
					{
						triLn_v1123 = viewPortHeight;
						v1115 = viewPortHeight;
					}
				}
				else if (v1298)
				{
					triLn_v1123 = viewPortHeight - v1192;
					v1115 = viewPortHeight - v1192;
				}
				v126 = LoadPolygon((x_DWORD*)unk_DE56Cx, &v120, &v121, &v122, &v123, &v124, v1105, v1109, v1129, v1140, v1151, &v1115);
				v31 = (unsigned __int8)x_BYTE_E126D;
				goto LABEL_53;
			}
		}
		if (vertex1->X <= vertex3->X)
			return;
		v3 = vertex3;
		v4 = vertex1;
		v5 = vertex2;
	LABEL_277:
		v143 = v3->Y;
		v1193 = v143;
		if (v143 >= 0)
		{
			if (v143 >= viewPortHeight)
				return;
			v1102 = m_ptrViewPortRenderBufferAltStart + pitch * v143;
			v1295 = 0;
		}
		else
		{
			v1102 = m_ptrViewPortRenderBufferAltStart;
			v1295 = 1;
		}
		v144 = v5->Y;
		v1299 = v144 > viewPortHeight;
		v1116 = v144 - v143;
		triLn_v1123 = v144 - v143;
		v1106 = ((v5->X - v3->X) << 16) / (v144 - v143);
		v1110 = ((v5->X - v4->X) << 16) / (v144 - v143);
		switch (x_BYTE_E126D)
		{
		case 0:
		case 0xE:
		case 0xF:
			v165 = v3->X << 16;
			v166 = v4->X << 16;
			if (v1295)
			{
				v167 = -v1193;
				v1116 += v1193;
				v18 = __OFSUB__(triLn_v1123, -v1193);
				v16 = triLn_v1123 == -v1193;
				v17 = triLn_v1123 + v1193 < 0;
				triLn_v1123 += v1193;
				if ((unsigned __int8)(v17 ^ v18) | (unsigned __int8)v16)
					return;
				v165 += v1106 * v167;
				v166 += v167 * v1110;
				if (v1299)
				{
					triLn_v1123 = viewPortHeight;
					v1116 = viewPortHeight;
				}
			}
			else if (v1299)
			{
				triLn_v1123 = viewPortHeight - v1193;
				v1116 = viewPortHeight - v1193;
			}
			v168 = LoadPolygon((x_DWORD*)unk_DE56Cx, &v165, &v166, v1106, v1110, &v1116);
			v31 = (unsigned __int8)x_BYTE_E126D;
			goto LABEL_53;
		case 1:
		case 4:
		case 0x10:
		case 0x11:
			v1146 = (v4->Brightness - v3->Brightness) / (v4->X - v3->X);
			v1154 = (v5->Brightness - v3->Brightness) / triLn_v1123;
			v160 = v3->X << 16;
			v161 = v4->X << 16;
			v162 = v3->Brightness;
			if (v1295)
			{
				v163 = -v1193;
				v1116 += v1193;
				v18 = __OFSUB__(triLn_v1123, -v1193);
				v16 = triLn_v1123 == -v1193;
				v17 = triLn_v1123 + v1193 < 0;
				triLn_v1123 += v1193;
				if ((unsigned __int8)(v17 ^ v18) | (unsigned __int8)v16)
					return;
				v160 += v1106 * v163;
				v161 += v163 * v1110;
				v162 += v163 * v1154;
				if (v1299)
				{
					triLn_v1123 = viewPortHeight;
					v1116 = viewPortHeight;
				}
			}
			else if (v1299)
			{
				triLn_v1123 = viewPortHeight - v1193;
				v1116 = viewPortHeight - v1193;
			}
			v164 = LoadPolygon((x_DWORD*)unk_DE56Cx, &v160, &v161, &v162, v1106, v1110, v1154, &v1116);
			v31 = (unsigned __int8)x_BYTE_E126D;
			goto LABEL_53;
		case 2:
		case 3:
		case 7:
		case 8:
		case 9:
		case 0xA:
		case 0xB:
		case 0xC:
		case 0xD:
		case 0x12:
		case 0x13:
		case 0x16:
		case 0x17:
			v153 = v4->X - v3->X;
			v1124 = (v4->U - v3->U) / v153;
			v1135 = (v4->V - v3->V) / v153;
			v1132 = (v5->U - v3->U) / triLn_v1123;
			v1143 = (v5->V - v3->V) / triLn_v1123;
			v154 = v3->X << 16;
			v155 = v4->X << 16;
			v156 = v3->U;
			v157 = v3->V;
			if (v1295)
			{
				v158 = -v1193;
				v1116 += v1193;
				v18 = __OFSUB__(triLn_v1123, -v1193);
				v16 = triLn_v1123 == -v1193;
				v17 = triLn_v1123 + v1193 < 0;
				triLn_v1123 += v1193;
				if ((unsigned __int8)(v17 ^ v18) | (unsigned __int8)v16)
					return;
				v154 += v1106 * v158;
				v155 += v158 * v1110;
				v156 += v158 * v1132;
				v157 += v158 * v1143;
				if (v1299)
				{
					triLn_v1123 = viewPortHeight;
					v1116 = viewPortHeight;
				}
			}
			else if (v1299)
			{
				triLn_v1123 = viewPortHeight - v1193;
				v1116 = viewPortHeight - v1193;
			}
			v159 = LoadPolygon((x_DWORD*)unk_DE56Cx, &v154, &v155, &v156, &v157, v1106, v1110, v1132, v1143, &v1116);
			v31 = (unsigned __int8)x_BYTE_E126D;
			goto LABEL_53;
		case 5:
		case 6:
		case 0x14:
		case 0x15:
		case 0x18:
		case 0x19:
		case 0x1A:
			v145 = v4->X - v3->X;
			v1124 = (v4->U - v3->U) / v145;
			v1135 = (v4->V - v3->V) / v145;
			v1146 = (v4->Brightness - v3->Brightness) / v145;
			v1131 = (v5->U - v3->U) / triLn_v1123;
			v1142 = (v5->V - v3->V) / triLn_v1123;
			v1153 = (v5->Brightness - v3->Brightness) / triLn_v1123;
			v146 = v3->X << 16;
			v147 = v4->X << 16;
			v148 = v3->U;
			v149 = v3->V;
			v150 = v3->Brightness;
			if (v1295)
			{
				v151 = -v1193;
				v1116 += v1193;
				v18 = __OFSUB__(triLn_v1123, -v1193);
				v16 = triLn_v1123 == -v1193;
				v17 = triLn_v1123 + v1193 < 0;
				triLn_v1123 += v1193;
				if ((unsigned __int8)(v17 ^ v18) | (unsigned __int8)v16)
					return;
				v146 += v1106 * v151;
				v147 += v151 * v1110;
				v148 += v151 * v1131;
				v149 += v151 * v1142;
				v150 += v151 * v1153;
				if (v1299)
				{
					triLn_v1123 = viewPortHeight;
					v1116 = viewPortHeight;
				}
			}
			else if (v1299)
			{
				triLn_v1123 = viewPortHeight - v1193;
				v1116 = viewPortHeight - v1193;
			}
			v152 = LoadPolygon((x_DWORD*)unk_DE56Cx, &v146, &v147, &v148, &v149, &v150, v1106, v1110, v1131, v1142, v1153, &v1116);
			v31 = (unsigned __int8)x_BYTE_E126D;
			goto LABEL_53;
		}
	}
	if (y1 == y3)
	{
		if (vertex3->X <= vertex1->X)
			return;
		v3 = vertex2;
		v4 = vertex3;
		v5 = vertex1;
		goto LABEL_234;
	}
	if (y1 < y3)
	{
		v3 = vertex2;
		v4 = vertex3;
		v5 = vertex1;
		goto LABEL_129;
	}
	if (y2 == y3)
	{
		if (vertex3->X <= vertex2->X)
			return;
		v3 = vertex2;
		v4 = vertex3;
		v5 = vertex1;
		goto LABEL_277;
	}
	if (y2 < y3)
	{
		v3 = vertex2;
		v4 = vertex3;
		v5 = vertex1;
		goto LABEL_24;
	}
	v3 = vertex3;
	v4 = vertex1;
	v5 = vertex2;
LABEL_129:
	v65 = v3->Y;
	v1191 = v65;
	if (v65 >= 0)
	{
		if (v65 >= viewPortHeight)
			return;
		v1102 = m_ptrViewPortRenderBufferAltStart + pitch * v65;
		v1293 = 0;
	}
	else
	{
		v1102 = m_ptrViewPortRenderBufferAltStart;
		v1293 = 1;
	}
	v66 = v5->Y;
	v1297 = v66 > viewPortHeight;
	v1114 = v66 - v65;
	v67 = v4->Y;
	v1301 = v67 > viewPortHeight;
	v68 = v67 - v65;
	v1118 = v68;
	triLn_v1123 = v68;
	v1104 = ((v5->X - v3->X) << 16) / v1114;
	if (((v4->X - v3->X) << 16) / v68 > v1104)
	{
		v1108 = ((v4->X - v3->X) << 16) / v68;
		v1112 = ((v4->X - v5->X) << 16) / (v4->Y - v5->Y);
		v1120 = v4->Y - v5->Y;
		v1122 = v5->X << 16;
		switch (x_BYTE_E126D)
		{
		case 0:
		case 0xE:
		case 0xF:
			v110 = v3->X << 16;
			v111 = v3->X << 16;
			if (v1293)
			{
				v18 = __OFSUB__(triLn_v1123, -v1191);
				v16 = triLn_v1123 == -v1191;
				v17 = triLn_v1123 + v1191 < 0;
				triLn_v1123 += v1191;
				if ((unsigned __int8)(v17 ^ v18) | (unsigned __int8)v16)
					return;
				v1164 = -v1191;
				if (-v1191 - v1114 >= 0)
				{
					v112 = v1164 - v1114;
					v1120 -= v112;
					v113 = v1112 * v112 + v1122;
					v111 += v112 * v1108 + v1114 * v1108;
					if (v1301)
					{
						v1120 = viewPortHeight;
						triLn_v1123 = viewPortHeight;
					}
					v114 = (int*)unk_DE56Cx;
					goto LABEL_228;
				}
				v1114 += v1191;
				v110 += v1104 * v1164;
				v111 += v1164 * v1108;
				if (v1301)
				{
					triLn_v1123 = viewPortHeight;
					if (v1297)
					{
						v1114 = viewPortHeight;
					}
					else
					{
						v1297 = viewPortHeight <= v1114;
						v1120 = viewPortHeight - v1114;
					}
				}
			}
			else if (v1301)
			{
				v115 = viewPortHeight - v1191;
				triLn_v1123 = viewPortHeight - v1191;
				if (v1297)
				{
					v1114 = viewPortHeight - v1191;
				}
				else
				{
					v18 = __OFSUB__(v115, v1114);
					v116 = v115 - v1114;
					v1297 = (v116 < 0) ^ v18 | (v116 == 0);
					v1120 = v116;
				}
			}
			v114 = LoadPolygon((x_DWORD*)unk_DE56Cx, &v110, &v111, v1104, v1108, &v1114);
			v113 = v1122;
		LABEL_228:
			if (v1297)
			{
				v31 = (unsigned __int8)x_BYTE_E126D;
			}
			else
			{
				v114 = LoadPolygon(v114, &v113, &v111, v1112, v1108, &v1120);
				v31 = (unsigned __int8)x_BYTE_E126D;
			}
			goto LABEL_53;
		case 1:
		case 4:
		case 0x10:
		case 0x11:
			v98 = v1114 * (signed __int64)(v4->X - v3->X) / v68;
			v99 = v3->X - v5->X;
			v18 = __OFADD__(v98, v99);
			v100 = v98 + v99 == 0;
			v17 = v98 + v99 < 0;
			v101 = v98 + v99;
			if ((unsigned __int8)v17 ^ v18)
				return;
			if (!v100)
				v1146 = (signed int)(v3->Brightness + (unsigned __int64)(v1114 * (signed __int64)(v4->Brightness - v3->Brightness) / v1118) - v5->Brightness)
				/ (v101 + 1);
			v1150 = (v5->Brightness - v3->Brightness) / v1114;
			v1156 = (v4->Brightness - v5->Brightness) / v1120;
			v102 = v3->X << 16;
			v103 = v3->X << 16;
			v104 = v3->Brightness;
			if (v1293)
			{
				v18 = __OFSUB__(triLn_v1123, -v1191);
				v16 = triLn_v1123 == -v1191;
				v17 = triLn_v1123 + v1191 < 0;
				triLn_v1123 += v1191;
				if ((unsigned __int8)(v17 ^ v18) | (unsigned __int8)v16)
					return;
				v1163 = -v1191;
				if (-v1191 - v1114 >= 0)
				{
					v105 = v1163 - v1114;
					v1120 -= v105;
					v106 = v1112 * v105 + v1122;
					v103 += v105 * v1108 + v1114 * v1108;
					v104 += v105 * v1156 + v1114 * v1150;
					if (v1301)
					{
						v1120 = viewPortHeight;
						triLn_v1123 = viewPortHeight;
					}
					v107 = (int*)unk_DE56Cx;
				LABEL_206:
					if (v1297)
					{
						v31 = (unsigned __int8)x_BYTE_E126D;
					}
					else
					{
						v107 = LoadPolygon(v107, &v106, &v103, &v104, v1112, v1108, v1156, &v1120);
						v31 = (unsigned __int8)x_BYTE_E126D;
					}
					goto LABEL_53;
				}
				v1114 += v1191;
				v102 += v1104 * v1163;
				v103 += v1163 * v1108;
				v104 += v1163 * v1150;
				if (v1301)
				{
					triLn_v1123 = viewPortHeight;
					if (v1297)
					{
						v1114 = viewPortHeight;
					}
					else
					{
						v1297 = viewPortHeight <= v1114;
						v1120 = viewPortHeight - v1114;
					}
				}
			}
			else if (v1301)
			{
				v108 = viewPortHeight - v1191;
				triLn_v1123 = viewPortHeight - v1191;
				if (v1297)
				{
					v1114 = viewPortHeight - v1191;
				}
				else
				{
					v18 = __OFSUB__(v108, v1114);
					v109 = v108 - v1114;
					v1297 = (v109 < 0) ^ v18 | (v109 == 0);
					v1120 = v109;
				}
			}
			v107 = LoadPolygon((x_DWORD*)unk_DE56Cx, &v102, &v103, &v104, v1104, v1108, v1150, &v1114);
			v106 = v1122;
			goto LABEL_206;
		case 2:
		case 3:
		case 7:
		case 8:
		case 9:
		case 0xA:
		case 0xB:
		case 0xC:
		case 0xD:
		case 0x12:
		case 0x13:
		case 0x16:
		case 0x17:
			v84 = v1114 * (signed __int64)(v4->X - v3->X) / v68;
			v85 = v3->X - v5->X;
			v18 = __OFADD__(v84, v85);
			v86 = v84 + v85 == 0;
			v17 = v84 + v85 < 0;
			v87 = v84 + v85;
			if ((unsigned __int8)v17 ^ v18)
				return;
			if (!v86)
			{
				v88 = v87 + 1;
				v1124 = (signed int)(v3->U + (unsigned __int64)(v1114 * (signed __int64)(v4->U - v3->U) / v1118) - v5->U)
					/ v88;
				v1135 = (signed int)(v3->V + (unsigned __int64)(v1114 * (signed __int64)(v4->V - v3->V) / v1118) - v5->V)
					/ v88;
			}
			v1128 = (v5->U - v3->U) / v1114;
			v1139 = (v5->V - v3->V) / v1114;
			v1134 = (v4->U - v5->U) / v1120;
			v1145 = (v4->V - v5->V) / v1120;
			v89 = v3->X << 16;
			v90 = v3->X << 16;
			v91 = v3->U;
			v92 = v3->V;
			if (v1293)
			{
				v18 = __OFSUB__(triLn_v1123, -v1191);
				v16 = triLn_v1123 == -v1191;
				v17 = triLn_v1123 + v1191 < 0;
				triLn_v1123 += v1191;
				if ((unsigned __int8)(v17 ^ v18) | (unsigned __int8)v16)
					return;
				v1162 = -v1191;
				if (-v1191 - v1114 >= 0)
				{
					v93 = v1162 - v1114;
					v1120 -= v93;
					v94 = v1112 * v93 + v1122;
					v90 += v93 * v1108 + v1114 * v1108;
					v91 += v93 * v1134 + v1114 * v1128;
					v92 += v93 * v1145 + v1114 * v1139;
					if (v1301)
					{
						v1120 = viewPortHeight;
						triLn_v1123 = viewPortHeight;
					}
					v95 = (int*)unk_DE56Cx;
				LABEL_181:
					if (v1297)
					{
						v31 = (unsigned __int8)x_BYTE_E126D;
					}
					else
					{
						v95 = LoadPolygon(v95, &v94, &v90, &v91, &v92, v1112, v1108, v1134, v1145, &v1120);
						v31 = (unsigned __int8)x_BYTE_E126D;
					}
					goto LABEL_53;
				}
				v1114 += v1191;
				v89 += v1104 * v1162;
				v90 += v1162 * v1108;
				v91 += v1162 * v1128;
				v92 += v1162 * v1139;
				if (v1301)
				{
					triLn_v1123 = viewPortHeight;
					if (v1297)
					{
						v1114 = viewPortHeight;
					}
					else
					{
						v1297 = viewPortHeight <= v1114;
						v1120 = viewPortHeight - v1114;
					}
				}
			}
			else if (v1301)
			{
				v96 = viewPortHeight - v1191;
				triLn_v1123 = viewPortHeight - v1191;
				if (v1297)
				{
					v1114 = viewPortHeight - v1191;
				}
				else
				{
					v18 = __OFSUB__(v96, v1114);
					v97 = v96 - v1114;
					v1297 = (v97 < 0) ^ v18 | (v97 == 0);
					v1120 = v97;
				}
			}
			v95 = LoadPolygon((x_DWORD*)unk_DE56Cx, &v89, &v90, &v91, &v92, v1104, v1108, v1128, v1139, &v1114);
			v94 = v1122;
			goto LABEL_181;
		case 5:
		case 6:
		case 0x14:
		case 0x15:
		case 0x18:
		case 0x19:
		case 0x1A:
			v69 = v1114 * (signed __int64)(v4->X - v3->X) / v68;
			v70 = v3->X - v5->X;
			v18 = __OFADD__(v69, v70);
			v71 = v69 + v70 == 0;
			v17 = v69 + v70 < 0;
			v72 = v69 + v70;
			if ((unsigned __int8)v17 ^ v18)
				return;
			if (!v71)
			{
				v73 = v72 + 1;
				v1124 = (signed int)(v3->U + (unsigned __int64)(v1114 * (signed __int64)(v4->U - v3->U) / v1118) - v5->U)
					/ v73;
				v1135 = (signed int)(v3->V + (unsigned __int64)(v1114 * (signed __int64)(v4->V - v3->V) / v1118) - v5->V)
					/ v73;
				v69 = (signed int)(v3->Brightness + (unsigned __int64)(v1114 * (signed __int64)(v4->Brightness - v3->Brightness) / v1118) - v5->Brightness) / v73;
			}
			v1146 = v69;
			v1127 = (v5->U - v3->U) / v1114;
			v1138 = (v5->V - v3->V) / v1114;
			v1149 = (v5->Brightness - v3->Brightness) / v1114;
			v1133 = (v4->U - v5->U) / v1120;
			v1144 = (v4->V - v5->V) / v1120;
			v1155 = (v4->Brightness - v5->Brightness) / v1120;
			v74 = v3->X << 16;
			v75 = v3->X << 16;
			v76 = v3->U;
			v77 = v3->V;
			v78 = v3->Brightness;
			if (v1293)
			{
				v18 = __OFSUB__(triLn_v1123, -v1191);
				v16 = triLn_v1123 == -v1191;
				v17 = triLn_v1123 + v1191 < 0;
				triLn_v1123 += v1191;
				if ((unsigned __int8)(v17 ^ v18) | (unsigned __int8)v16)
					return;
				v1161 = -v1191;
				if (-v1191 - v1114 >= 0)
				{
					v79 = v1161 - v1114;
					v1120 -= v79;
					v80 = v1112 * v79 + v1122;
					v75 += v79 * v1108 + v1114 * v1108;
					v76 += v79 * v1133 + v1114 * v1127;
					v77 += v79 * v1144 + v1114 * v1138;
					v78 += v79 * v1155 + v1114 * v1149;
					if (v1301)
					{
						v1120 = viewPortHeight;
						triLn_v1123 = viewPortHeight;
					}
					v81 = (x_DWORD*)unk_DE56Cx;
				LABEL_156:
					if (v1297)
					{
						v31 = (unsigned __int8)x_BYTE_E126D;
					}
					else
					{
						v81 = LoadPolygon(v81, &v80, &v75, &v76, &v77, &v78, v1112, v1108, v1133, v1144, v1155, &v1120);
						v31 = (unsigned __int8)x_BYTE_E126D;
					}
				LABEL_53:
					switch (x_BYTE_E126D)
					{
					case 0:
						v169 = (unsigned __int16*)unk_DE56Cx;
						v170 = (char*)v1102;
						v171 = x_BYTE_E126C;
						HIWORD(v172) = 0;
						while (1)
						{
							LOWORD(v172) = v169[1];
							v173 = v169[3];
							v170 += pitch;
							line1++;
							if (line1 >= drawEveryNthLine)
							{
								line1 = startLine;
								if ((v172 & 0x8000u) == 0)
									break;
								if ((signed __int16)v173 > 0)
								{
									if (v173 > viewPortWidth)
										v173 = viewPortWidth;
									v174 = v170;
								LABEL_328:
									memset(v174, v171, v173);
								}
							}
							LABEL_329:
								v169 += 10;
							if (!--triLn_v1123)
								return;
						}
						if (v173 > viewPortWidth)
							v173 = viewPortWidth;
						v18 = __OFSUB__((x_WORD)v173, (x_WORD)v172);
						LOWORD(v173) = v173 - v172;
						if ((unsigned __int8)(((v173 & 0x8000u) != 0) ^ v18) | ((x_WORD)v173 == 0))
							goto LABEL_329;
						v174 = &v170[v172];
						goto LABEL_328;
					case 1:
						v175 = (unsigned __int16*)unk_DE56Cx;
						while (1)
						{
							LOWORD(v31) = v175[1];
							v176 = v175[3];
							v177 = (x_BYTE*)(pitch + v1102);
							v1102 += pitch;
							line2++;
							if (line2 >= drawEveryNthLine)
							{
								line2 = 0;
								if ((v31 & 0x8000u) == 0)
									break;
								if ((signed __int16)v176 > 0)
								{
									v178 = v1146 * (unsigned __int16)-(signed __int16)v31;
									v179 = v178;
									v31 = v178 >> 8;
									v180 = __CFADD__(v175[8], v179);
									v181 = v175[8] + v179;
									BYTE1(v31) += *((x_BYTE*)v175 + 18) + v180;
									if (v176 > viewPortWidth)
										LOWORD(v176) = viewPortWidth;
									v31 = (unsigned __int16)v31;
								LABEL_341:
									while (1)
									{
										*v177 = BYTE1(v31);
										v180 = __CFADD__((x_WORD)v1146, v181);
										v182 = v1146 + v181;
										v183 = BYTE2(v1146) + v180 + BYTE1(v31);
										v184 = v176 - 1;
										if (!v184)
											break;
										v177[1] = v183;
										v180 = __CFADD__((x_WORD)v1146, v182);
										v185 = v1146 + v182;
										v186 = BYTE2(v1146) + v180 + v183;
										v187 = v184 - 1;
										if (!v187)
											break;
										v177[2] = v186;
										v180 = __CFADD__((x_WORD)v1146, v185);
										v188 = v1146 + v185;
										v189 = BYTE2(v1146) + v180 + v186;
										v190 = v187 - 1;
										if (!v190)
											break;
										v177[3] = v189;
										v180 = __CFADD__((x_WORD)v1146, v188);
										v191 = v1146 + v188;
										v192 = BYTE2(v1146) + v180 + v189;
										v193 = v190 - 1;
										if (!v193)
											break;
										v177[4] = v192;
										v180 = __CFADD__((x_WORD)v1146, v191);
										v194 = v1146 + v191;
										v195 = BYTE2(v1146) + v180 + v192;
										v196 = v193 - 1;
										if (!v196)
											break;
										v177[5] = v195;
										v180 = __CFADD__((x_WORD)v1146, v194);
										v197 = v1146 + v194;
										v198 = BYTE2(v1146) + v180 + v195;
										v199 = v196 - 1;
										if (!v199)
											break;
										v177[6] = v198;
										v180 = __CFADD__((x_WORD)v1146, v197);
										v200 = v1146 + v197;
										v201 = BYTE2(v1146) + v180 + v198;
										v202 = v199 - 1;
										if (!v202)
											break;
										v177[7] = v201;
										v180 = __CFADD__((x_WORD)v1146, v200);
										v203 = v1146 + v200;
										v204 = BYTE2(v1146) + v180 + v201;
										v205 = v202 - 1;
										if (!v205)
											break;
										v177[8] = v204;
										v180 = __CFADD__((x_WORD)v1146, v203);
										v206 = v1146 + v203;
										v207 = BYTE2(v1146) + v180 + v204;
										v208 = v205 - 1;
										if (!v208)
											break;
										v177[9] = v207;
										v180 = __CFADD__((x_WORD)v1146, v206);
										v209 = v1146 + v206;
										v210 = BYTE2(v1146) + v180 + v207;
										v211 = v208 - 1;
										if (!v211)
											break;
										v177[10] = v210;
										v180 = __CFADD__((x_WORD)v1146, v209);
										v212 = v1146 + v209;
										v213 = BYTE2(v1146) + v180 + v210;
										v214 = v211 - 1;
										if (!v214)
											break;
										v177[11] = v213;
										v180 = __CFADD__((x_WORD)v1146, v212);
										v215 = v1146 + v212;
										v216 = BYTE2(v1146) + v180 + v213;
										v217 = v214 - 1;
										if (!v217)
											break;
										v177[12] = v216;
										v180 = __CFADD__((x_WORD)v1146, v215);
										v218 = v1146 + v215;
										v219 = BYTE2(v1146) + v180 + v216;
										v220 = v217 - 1;
										if (!v220)
											break;
										v177[13] = v219;
										v180 = __CFADD__((x_WORD)v1146, v218);
										v221 = v1146 + v218;
										v222 = BYTE2(v1146) + v180 + v219;
										v223 = v220 - 1;
										if (!v223)
											break;
										v177[14] = v222;
										v180 = __CFADD__((x_WORD)v1146, v221);
										v224 = v1146 + v221;
										v225 = BYTE2(v1146) + v180 + v222;
										v226 = v223 - 1;
										if (!v226)
											break;
										v177[15] = v225;
										v180 = __CFADD__((x_WORD)v1146, v224);
										v181 = v1146 + v224;
										BYTE1(v31) = BYTE2(v1146) + v180 + v225;
										LOWORD(v176) = v226 - 1;
										if (!(x_WORD)v176)
											break;
										v177 += 16;
									}
								}
							}
						LABEL_358:
							v175 += 10;
							if (!--triLn_v1123)
								return;
						}
						if (v176 > viewPortWidth)
							LOWORD(v176) = viewPortWidth;
						v18 = __OFSUB__((x_WORD)v176, (x_WORD)v31);
						LOWORD(v176) = v176 - v31;
						if ((unsigned __int8)(((v176 & 0x8000u) != 0) ^ v18) | ((x_WORD)v176 == 0))
							goto LABEL_358;
						v177 += v31;
						v31 = (unsigned __int8)x_BYTE_E126C;
						v181 = v175[8];
						BYTE1(v31) = *((x_BYTE*)v175 + 18);
						goto LABEL_341;
					case 2:
						v227 = (unsigned __int16*)unk_DE56Cx;
						v1165 = v1135 << 16;
						HIWORD(v228) = 0;
						HIWORD(v229) = 0;
						while (1)
						{
							LOWORD(v228) = v227[1];
							v230 = v227[3];
							v231 = (x_BYTE*)(pitch + v1102);
							v1102 += pitch;
							line3++;
							if (line3 >= drawEveryNthLine)
							{
								line3 = 0;
								if ((v228 & 0x8000u) == 0)
									break;
								if ((signed __int16)v230 > 0)
								{
									v232 = (unsigned __int16)-(signed __int16)v228;
									v234 = __ROL4__(*((x_DWORD*)v227 + 3) + v1135 * v232, 16);
									BYTE1(v229) = v234;
									v233 = *((x_DWORD*)v227 + 2) + v1124 * v232;
									LOWORD(v234) = v233;
									v235 = v233 >> 8;
									LOBYTE(v229) = BYTE1(v235);
									if (v230 > viewPortWidth)
										LOWORD(v230) = viewPortWidth;
									v228 = (unsigned __int16)v235;
								LABEL_370:
									v1274 = v227;
									v236 = pTexture;
									while (1)
									{
										v237 = *(x_BYTE*)(v229 + v236);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v234);
										LOWORD(v234) = v1124 + v234;
										LOBYTE(v229) = BYTE2(v1124) + v180 + v229;
										v231[0] = v237;
										v180 = __CFADD__(v1165, v234);
										v238 = v1165 + v234;
										BYTE1(v229) += BYTE2(v1135) + v180;
										v239 = v230 - 1;
										if (!v239)
											break;
										v240 = *(x_BYTE*)(v229 + v236);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v238);
										LOWORD(v238) = v1124 + v238;
										LOBYTE(v229) = BYTE2(v1124) + v180 + v229;
										v231[1] = v240;
										v180 = __CFADD__(v1165, v238);
										v241 = v1165 + v238;
										BYTE1(v229) += BYTE2(v1135) + v180;
										v242 = v239 - 1;
										if (!v242)
											break;
										v243 = *(x_BYTE*)(v229 + v236);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v241);
										LOWORD(v241) = v1124 + v241;
										LOBYTE(v229) = BYTE2(v1124) + v180 + v229;
										v231[2] = v243;
										v180 = __CFADD__(v1165, v241);
										v244 = v1165 + v241;
										BYTE1(v229) += BYTE2(v1135) + v180;
										v245 = v242 - 1;
										if (!v245)
											break;
										v246 = *(x_BYTE*)(v229 + v236);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v244);
										LOWORD(v244) = v1124 + v244;
										LOBYTE(v229) = BYTE2(v1124) + v180 + v229;
										v231[3] = v246;
										v180 = __CFADD__(v1165, v244);
										v247 = v1165 + v244;
										BYTE1(v229) += BYTE2(v1135) + v180;
										v248 = v245 - 1;
										if (!v248)
											break;
										v249 = *(x_BYTE*)(v229 + v236);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v247);
										LOWORD(v247) = v1124 + v247;
										LOBYTE(v229) = BYTE2(v1124) + v180 + v229;
										v231[4] = v249;
										v180 = __CFADD__(v1165, v247);
										v250 = v1165 + v247;
										BYTE1(v229) += BYTE2(v1135) + v180;
										v251 = v248 - 1;
										if (!v251)
											break;
										v252 = *(x_BYTE*)(v229 + v236);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v250);
										LOWORD(v250) = v1124 + v250;
										LOBYTE(v229) = BYTE2(v1124) + v180 + v229;
										v231[5] = v252;
										v180 = __CFADD__(v1165, v250);
										v253 = v1165 + v250;
										BYTE1(v229) += BYTE2(v1135) + v180;
										v254 = v251 - 1;
										if (!v254)
											break;
										v255 = *(x_BYTE*)(v229 + v236);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v253);
										LOWORD(v253) = v1124 + v253;
										LOBYTE(v229) = BYTE2(v1124) + v180 + v229;
										v231[6] = v255;
										v180 = __CFADD__(v1165, v253);
										v256 = v1165 + v253;
										BYTE1(v229) += BYTE2(v1135) + v180;
										v257 = v254 - 1;
										if (!v257)
											break;
										v258 = *(x_BYTE*)(v229 + v236);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v256);
										LOWORD(v256) = v1124 + v256;
										LOBYTE(v229) = BYTE2(v1124) + v180 + v229;
										v231[7] = v258;
										v180 = __CFADD__(v1165, v256);
										v259 = v1165 + v256;
										BYTE1(v229) += BYTE2(v1135) + v180;
										v260 = v257 - 1;
										if (!v260)
											break;
										v261 = *(x_BYTE*)(v229 + v236);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v259);
										LOWORD(v259) = v1124 + v259;
										LOBYTE(v229) = BYTE2(v1124) + v180 + v229;
										v231[8] = v261;
										v180 = __CFADD__(v1165, v259);
										v262 = v1165 + v259;
										BYTE1(v229) += BYTE2(v1135) + v180;
										v263 = v260 - 1;
										if (!v263)
											break;
										v264 = *(x_BYTE*)(v229 + v236);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v262);
										LOWORD(v262) = v1124 + v262;
										LOBYTE(v229) = BYTE2(v1124) + v180 + v229;
										v231[9] = v264;
										v180 = __CFADD__(v1165, v262);
										v265 = v1165 + v262;
										BYTE1(v229) += BYTE2(v1135) + v180;
										v266 = v263 - 1;
										if (!v266)
											break;
										v267 = *(x_BYTE*)(v229 + v236);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v265);
										LOWORD(v265) = v1124 + v265;
										LOBYTE(v229) = BYTE2(v1124) + v180 + v229;
										v231[10] = v267;
										v180 = __CFADD__(v1165, v265);
										v268 = v1165 + v265;
										BYTE1(v229) += BYTE2(v1135) + v180;
										v269 = v266 - 1;
										if (!v269)
											break;
										v270 = *(x_BYTE*)(v229 + v236);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v268);
										LOWORD(v268) = v1124 + v268;
										LOBYTE(v229) = BYTE2(v1124) + v180 + v229;
										v231[11] = v270;
										v180 = __CFADD__(v1165, v268);
										v271 = v1165 + v268;
										BYTE1(v229) += BYTE2(v1135) + v180;
										v272 = v269 - 1;
										if (!v272)
											break;
										v273 = *(x_BYTE*)(v229 + v236);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v271);
										LOWORD(v271) = v1124 + v271;
										LOBYTE(v229) = BYTE2(v1124) + v180 + v229;
										v231[12] = v273;
										v180 = __CFADD__(v1165, v271);
										v274 = v1165 + v271;
										BYTE1(v229) += BYTE2(v1135) + v180;
										v275 = v272 - 1;
										if (!v275)
											break;
										v276 = *(x_BYTE*)(v229 + v236);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v274);
										LOWORD(v274) = v1124 + v274;
										LOBYTE(v229) = BYTE2(v1124) + v180 + v229;
										v231[13] = v276;
										v180 = __CFADD__(v1165, v274);
										v277 = v1165 + v274;
										BYTE1(v229) += BYTE2(v1135) + v180;
										v278 = v275 - 1;
										if (!v278)
											break;
										v279 = *(x_BYTE*)(v229 + v236);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v277);
										LOWORD(v277) = v1124 + v277;
										LOBYTE(v229) = BYTE2(v1124) + v180 + v229;
										v231[14] = v279;
										v180 = __CFADD__(v1165, v277);
										v280 = v1165 + v277;
										BYTE1(v229) += BYTE2(v1135) + v180;
										v281 = v278 - 1;
										if (!v281)
											break;
										v282 = *(x_BYTE*)(v229 + v236);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v280);
										LOWORD(v280) = v1124 + v280;
										LOBYTE(v229) = BYTE2(v1124) + v180 + v229;
										v231[15] = v282;
										v180 = __CFADD__(v1165, v280);
										v234 = v1165 + v280;
										BYTE1(v229) += BYTE2(v1135) + v180;
										LOWORD(v230) = v281 - 1;
										if (!(x_WORD)v230)
											break;
										v231 += 16;
									}
									v227 = v1274;
								}
							}
						LABEL_389:
							v227 += 10;
							if (!--triLn_v1123)
								return;
						}
						if (v230 > viewPortWidth)
							LOWORD(v230) = viewPortWidth;
						v18 = __OFSUB__((x_WORD)v230, (x_WORD)v228);
						LOWORD(v230) = v230 - v228;
						if ((unsigned __int8)(((v230 & 0x8000u) != 0) ^ v18) | ((x_WORD)v230 == 0))
							goto LABEL_389;
						v231 += v228;
						v234 = __ROL4__(*((x_DWORD*)v227 + 3), 16);
						BYTE1(v229) = v234;
						LOWORD(v234) = v227[4];
						LOBYTE(v229) = *((x_BYTE*)v227 + 10);
						goto LABEL_370;
					case 3:
						v283 = (unsigned __int16*)unk_DE56Cx;
						v1166 = v1135 << 16;
						HIWORD(v284) = 0;
						HIWORD(v285) = 0;
						while (1)
						{
							LOWORD(v284) = v283[1];
							v286 = v283[3];
							v287 = (x_BYTE*)(pitch + v1102);
							v1102 += pitch;
							line4++;
							if (line4 >= drawEveryNthLine)
							{
								line4 = 0;
								if ((v284 & 0x8000u) == 0)
									break;
								if ((signed __int16)v286 > 0)
								{
									v288 = (unsigned __int16)-(signed __int16)v284;
									v290 = __ROL4__(*((x_DWORD*)v283 + 3) + v1135 * v288, 16);
									BYTE1(v285) = v290;
									v289 = *((x_DWORD*)v283 + 2) + v1124 * v288;
									LOWORD(v290) = v289;
									v291 = v289 >> 8;
									LOBYTE(v285) = BYTE1(v291);
									if (v286 > viewPortWidth)
										LOWORD(v286) = viewPortWidth;
									v284 = (unsigned __int16)v291;
								LABEL_401:
									v1275 = v283;
									v292 = pTexture;
									while (1)
									{
										v293 = *(x_BYTE*)(v285 + v292);
										if (v293)
											*v287 = v293;
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v290);
										LOWORD(v290) = v1124 + v290;
										LOBYTE(v285) = BYTE2(v1124) + v180 + v285;
										v180 = __CFADD__(v1166, v290);
										v294 = v1166 + v290;
										BYTE1(v285) += BYTE2(v1135) + v180;
										v295 = v286 - 1;
										if (!v295)
											break;
										v296 = *(x_BYTE*)(v285 + v292);
										if (v296)
											v287[1] = v296;
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v294);
										LOWORD(v294) = v1124 + v294;
										LOBYTE(v285) = BYTE2(v1124) + v180 + v285;
										v180 = __CFADD__(v1166, v294);
										v297 = v1166 + v294;
										BYTE1(v285) += BYTE2(v1135) + v180;
										v298 = v295 - 1;
										if (!v298)
											break;
										v299 = *(x_BYTE*)(v285 + v292);
										if (v299)
											v287[2] = v299;
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v297);
										LOWORD(v297) = v1124 + v297;
										LOBYTE(v285) = BYTE2(v1124) + v180 + v285;
										v180 = __CFADD__(v1166, v297);
										v300 = v1166 + v297;
										BYTE1(v285) += BYTE2(v1135) + v180;
										v301 = v298 - 1;
										if (!v301)
											break;
										v302 = *(x_BYTE*)(v285 + v292);
										if (v302)
											v287[3] = v302;
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v300);
										LOWORD(v300) = v1124 + v300;
										LOBYTE(v285) = BYTE2(v1124) + v180 + v285;
										v180 = __CFADD__(v1166, v300);
										v303 = v1166 + v300;
										BYTE1(v285) += BYTE2(v1135) + v180;
										v304 = v301 - 1;
										if (!v304)
											break;
										v305 = *(x_BYTE*)(v285 + v292);
										if (v305)
											v287[4] = v305;
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v303);
										LOWORD(v303) = v1124 + v303;
										LOBYTE(v285) = BYTE2(v1124) + v180 + v285;
										v180 = __CFADD__(v1166, v303);
										v306 = v1166 + v303;
										BYTE1(v285) += BYTE2(v1135) + v180;
										v307 = v304 - 1;
										if (!v307)
											break;
										v308 = *(x_BYTE*)(v285 + v292);
										if (v308)
											v287[5] = v308;
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v306);
										LOWORD(v306) = v1124 + v306;
										LOBYTE(v285) = BYTE2(v1124) + v180 + v285;
										v180 = __CFADD__(v1166, v306);
										v309 = v1166 + v306;
										BYTE1(v285) += BYTE2(v1135) + v180;
										v310 = v307 - 1;
										if (!v310)
											break;
										v311 = *(x_BYTE*)(v285 + v292);
										if (v311)
											v287[6] = v311;
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v309);
										LOWORD(v309) = v1124 + v309;
										LOBYTE(v285) = BYTE2(v1124) + v180 + v285;
										v180 = __CFADD__(v1166, v309);
										v312 = v1166 + v309;
										BYTE1(v285) += BYTE2(v1135) + v180;
										v313 = v310 - 1;
										if (!v313)
											break;
										v314 = *(x_BYTE*)(v285 + v292);
										if (v314)
											v287[7] = v314;
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v312);
										LOWORD(v312) = v1124 + v312;
										LOBYTE(v285) = BYTE2(v1124) + v180 + v285;
										v180 = __CFADD__(v1166, v312);
										v315 = v1166 + v312;
										BYTE1(v285) += BYTE2(v1135) + v180;
										v316 = v313 - 1;
										if (!v316)
											break;
										v317 = *(x_BYTE*)(v285 + v292);
										if (v317)
											v287[8] = v317;
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v315);
										LOWORD(v315) = v1124 + v315;
										LOBYTE(v285) = BYTE2(v1124) + v180 + v285;
										v180 = __CFADD__(v1166, v315);
										v318 = v1166 + v315;
										BYTE1(v285) += BYTE2(v1135) + v180;
										v319 = v316 - 1;
										if (!v319)
											break;
										v320 = *(x_BYTE*)(v285 + v292);
										if (v320)
											v287[9] = v320;
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v318);
										LOWORD(v318) = v1124 + v318;
										LOBYTE(v285) = BYTE2(v1124) + v180 + v285;
										v180 = __CFADD__(v1166, v318);
										v321 = v1166 + v318;
										BYTE1(v285) += BYTE2(v1135) + v180;
										v322 = v319 - 1;
										if (!v322)
											break;
										v323 = *(x_BYTE*)(v285 + v292);
										if (v323)
											v287[10] = v323;
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v321);
										LOWORD(v321) = v1124 + v321;
										LOBYTE(v285) = BYTE2(v1124) + v180 + v285;
										v180 = __CFADD__(v1166, v321);
										v324 = v1166 + v321;
										BYTE1(v285) += BYTE2(v1135) + v180;
										v325 = v322 - 1;
										if (!v325)
											break;
										v326 = *(x_BYTE*)(v285 + v292);
										if (v326)
											v287[11] = v326;
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v324);
										LOWORD(v324) = v1124 + v324;
										LOBYTE(v285) = BYTE2(v1124) + v180 + v285;
										v180 = __CFADD__(v1166, v324);
										v327 = v1166 + v324;
										BYTE1(v285) += BYTE2(v1135) + v180;
										v328 = v325 - 1;
										if (!v328)
											break;
										v329 = *(x_BYTE*)(v285 + v292);
										if (v329)
											v287[12] = v329;
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v327);
										LOWORD(v327) = v1124 + v327;
										LOBYTE(v285) = BYTE2(v1124) + v180 + v285;
										v180 = __CFADD__(v1166, v327);
										v330 = v1166 + v327;
										BYTE1(v285) += BYTE2(v1135) + v180;
										v331 = v328 - 1;
										if (!v331)
											break;
										v332 = *(x_BYTE*)(v285 + v292);
										if (v332)
											v287[13] = v332;
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v330);
										LOWORD(v330) = v1124 + v330;
										LOBYTE(v285) = BYTE2(v1124) + v180 + v285;
										v180 = __CFADD__(v1166, v330);
										v333 = v1166 + v330;
										BYTE1(v285) += BYTE2(v1135) + v180;
										v334 = v331 - 1;
										if (!v334)
											break;
										v335 = *(x_BYTE*)(v285 + v292);
										if (v335)
											v287[14] = v335;
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v333);
										LOWORD(v333) = v1124 + v333;
										LOBYTE(v285) = BYTE2(v1124) + v180 + v285;
										v180 = __CFADD__(v1166, v333);
										v336 = v1166 + v333;
										BYTE1(v285) += BYTE2(v1135) + v180;
										v337 = v334 - 1;
										if (!v337)
											break;
										v338 = *(x_BYTE*)(v285 + v292);
										if (v338)
											v287[15] = v338;
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v336);
										LOWORD(v336) = v1124 + v336;
										LOBYTE(v285) = BYTE2(v1124) + v180 + v285;
										v180 = __CFADD__(v1166, v336);
										v290 = v1166 + v336;
										BYTE1(v285) += BYTE2(v1135) + v180;
										LOWORD(v286) = v337 - 1;
										if (!(x_WORD)v286)
											break;
										v287 += 16;
									}
									v283 = v1275;
								}
							}
						LABEL_452:
							v283 += 10;
							if (!--triLn_v1123)
								return;
						}
						if (v286 > viewPortWidth)
							LOWORD(v286) = viewPortWidth;
						v18 = __OFSUB__((x_WORD)v286, (x_WORD)v284);
						LOWORD(v286) = v286 - v284;
						if ((unsigned __int8)(((v286 & 0x8000u) != 0) ^ v18) | ((x_WORD)v286 == 0))
							goto LABEL_452;
						v287 += v284;
						v290 = __ROL4__(*((x_DWORD*)v283 + 3), 16);
						BYTE1(v285) = v290;
						LOWORD(v290) = v283[4];
						LOBYTE(v285) = *((x_BYTE*)v283 + 10);
						goto LABEL_401;
					case 4:
						v339 = (unsigned __int16*)unk_DE56Cx;
						while (1)
						{
							LOWORD(v31) = v339[1];
							v340 = v339[3];
							v341 = (x_BYTE*)(pitch + v1102);
							v1102 += pitch;
							line5++;
							if (line5 >= drawEveryNthLine)
							{
								line5 = 0;
								if ((v31 & 0x8000u) == 0)
									break;
								if ((signed __int16)v340 > 0)
								{
									v342 = v1146 * (unsigned __int16)-(signed __int16)v31;
									v343 = v342;
									v31 = v342 >> 8;
									v180 = __CFADD__(v339[8], v343);
									v344 = v339[8] + v343;
									BYTE1(v31) += *((x_BYTE*)v339 + 18) + v180;
									if (v340 > viewPortWidth)
										LOWORD(v340) = viewPortWidth;
									v31 = (unsigned __int16)v31;
									LOBYTE(v31) = x_BYTE_E126C;
								LABEL_464:
									while (1)
									{
										*v341 = m_ptrX_BYTE_F6EE0_tablesx[v31];
										v180 = __CFADD__((x_WORD)v1146, v344);
										v345 = v1146 + v344;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v346 = v340 - 1;
										if (!v346)
											break;
										v341[1] = m_ptrX_BYTE_F6EE0_tablesx[v31];
										v180 = __CFADD__((x_WORD)v1146, v345);
										v347 = v1146 + v345;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v348 = v346 - 1;
										if (!v348)
											break;
										v341[2] = m_ptrX_BYTE_F6EE0_tablesx[v31];
										v180 = __CFADD__((x_WORD)v1146, v347);
										v349 = v1146 + v347;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v350 = v348 - 1;
										if (!v350)
											break;
										v341[3] = m_ptrX_BYTE_F6EE0_tablesx[v31];
										v180 = __CFADD__((x_WORD)v1146, v349);
										v351 = v1146 + v349;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v352 = v350 - 1;
										if (!v352)
											break;
										v341[4] = m_ptrX_BYTE_F6EE0_tablesx[v31];
										v180 = __CFADD__((x_WORD)v1146, v351);
										v353 = v1146 + v351;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v354 = v352 - 1;
										if (!v354)
											break;
										v341[5] = m_ptrX_BYTE_F6EE0_tablesx[v31];
										v180 = __CFADD__((x_WORD)v1146, v353);
										v355 = v1146 + v353;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v356 = v354 - 1;
										if (!v356)
											break;
										v341[6] = m_ptrX_BYTE_F6EE0_tablesx[v31];
										v180 = __CFADD__((x_WORD)v1146, v355);
										v357 = v1146 + v355;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v358 = v356 - 1;
										if (!v358)
											break;
										v341[7] = m_ptrX_BYTE_F6EE0_tablesx[v31];
										v180 = __CFADD__((x_WORD)v1146, v357);
										v359 = v1146 + v357;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v360 = v358 - 1;
										if (!v360)
											break;
										v341[8] = m_ptrX_BYTE_F6EE0_tablesx[v31];
										v180 = __CFADD__((x_WORD)v1146, v359);
										v361 = v1146 + v359;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v362 = v360 - 1;
										if (!v362)
											break;
										v341[9] = m_ptrX_BYTE_F6EE0_tablesx[v31];
										v180 = __CFADD__((x_WORD)v1146, v361);
										v363 = v1146 + v361;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v364 = v362 - 1;
										if (!v364)
											break;
										v341[10] = m_ptrX_BYTE_F6EE0_tablesx[v31];
										v180 = __CFADD__((x_WORD)v1146, v363);
										v365 = v1146 + v363;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v366 = v364 - 1;
										if (!v366)
											break;
										v341[11] = m_ptrX_BYTE_F6EE0_tablesx[v31];
										v180 = __CFADD__((x_WORD)v1146, v365);
										v367 = v1146 + v365;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v368 = v366 - 1;
										if (!v368)
											break;
										v341[12] = m_ptrX_BYTE_F6EE0_tablesx[v31];
										v180 = __CFADD__((x_WORD)v1146, v367);
										v369 = v1146 + v367;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v370 = v368 - 1;
										if (!v370)
											break;
										v341[13] = m_ptrX_BYTE_F6EE0_tablesx[v31];
										v180 = __CFADD__((x_WORD)v1146, v369);
										v371 = v1146 + v369;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v372 = v370 - 1;
										if (!v372)
											break;
										v341[14] = m_ptrX_BYTE_F6EE0_tablesx[v31];
										v180 = __CFADD__((x_WORD)v1146, v371);
										v373 = v1146 + v371;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v374 = v372 - 1;
										if (!v374)
											break;
										v341[15] = m_ptrX_BYTE_F6EE0_tablesx[v31];
										v180 = __CFADD__((x_WORD)v1146, v373);
										v344 = v1146 + v373;
										BYTE1(v31) += BYTE2(v1146) + v180;
										LOWORD(v340) = v374 - 1;
										if (!(x_WORD)v340)
											break;
										v341 += 16;
									}
								}
							}
						LABEL_481:
							v339 += 10;
							if (!--triLn_v1123)
								return;
						}
						if (v340 > viewPortWidth)
							LOWORD(v340) = viewPortWidth;
						v18 = __OFSUB__((x_WORD)v340, (x_WORD)v31);
						LOWORD(v340) = v340 - v31;
						if ((unsigned __int8)(((v340 & 0x8000u) != 0) ^ v18) | ((x_WORD)v340 == 0))
							goto LABEL_481;
						v341 += v31;
						v31 = (unsigned __int8)x_BYTE_E126C;
						v344 = v339[8];
						BYTE1(v31) = *((x_BYTE*)v339 + 18);
						goto LABEL_464;
					case 5:
						v1276 = (char*)unk_DE56Cx;
						v1167 = v1135 << 16;
						v1183 = v1146 << 16;
						HIWORD(v375) = 0;
						HIWORD(v376) = 0;
						while (1)
						{
							v377 = v1276;
							v1276 += 20;
							LOWORD(v375) = *((x_WORD*)v377 + 1);
							v378 = *((unsigned __int16*)v377 + 3);
							v379 = pitch + v1102;
							v1102 += pitch;
							line6++;
							if (line6 >= drawEveryNthLine)
							{
								line6 = 0;
								if ((v375 & 0x8000u) == 0)
									break;
								if ((signed __int16)v378 > 0)
								{
									v380 = (unsigned __int16)-(signed __int16)v375;
									v381 = v380;
									v383 = __ROL4__(*((x_DWORD*)v377 + 3) + v1135 * v380, 16);
									BYTE1(v376) = v383;
									v382 = *((x_DWORD*)v377 + 2) + v1124 * v380;
									LOWORD(v383) = v382;
									v375 = v382 >> 8;
									LOBYTE(v376) = BYTE1(v375);
									v384 = __ROL4__(*((x_DWORD*)v377 + 4) + v1146 * v381, 16);
									BYTE1(v375) = v384;
									LOWORD(v384) = *((x_WORD*)v377 + 3);
									v375 = (unsigned __int16)v375;
									if ((signed __int16)v384 > (signed __int16)viewPortWidth)
										LOWORD(v384) = viewPortWidth;
								LABEL_493:
									v387 = v384 & 0xF;
									v388 = (x_BYTE*)(x_DWORD_B8845[v387] + v379);
									v389 = pTexture;
									switch (v387)
									{
									case 0:
										goto LABEL_494;
									case 1:
										goto LABEL_509;
									case 2:
										goto LABEL_508;
									case 3:
										goto LABEL_507;
									case 4:
										goto LABEL_506;
									case 5:
										goto LABEL_505;
									case 6:
										goto LABEL_504;
									case 7:
										goto LABEL_503;
									case 8:
										goto LABEL_502;
									case 9:
										goto LABEL_501;
									case 10:
										goto LABEL_500;
									case 11:
										goto LABEL_499;
									case 12:
										goto LABEL_498;
									case 13:
										goto LABEL_497;
									case 14:
										goto LABEL_496;
									case 15:
										while (1)
										{
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v383);
											LOWORD(v383) = v1124 + v383;
											LOBYTE(v375) = *(x_BYTE*)(v376 + v389);
											LOBYTE(v376) = BYTE2(v1124) + v180 + v376;
											v180 = __CFADD__(v1167, v383);
											v383 += v1167;
											BYTE1(v376) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1183, v384);
											v384 += v1183;
											v388[1] = m_ptrX_BYTE_F6EE0_tablesx[v375];
											BYTE1(v375) += BYTE2(v1146) + v180;
										LABEL_496:
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v383);
											LOWORD(v383) = v1124 + v383;
											LOBYTE(v375) = *(x_BYTE*)(v376 + v389);
											LOBYTE(v376) = BYTE2(v1124) + v180 + v376;
											v180 = __CFADD__(v1167, v383);
											v383 += v1167;
											BYTE1(v376) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1183, v384);
											v384 += v1183;
											v388[2] = m_ptrX_BYTE_F6EE0_tablesx[v375];
											BYTE1(v375) += BYTE2(v1146) + v180;
										LABEL_497:
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v383);
											LOWORD(v383) = v1124 + v383;
											LOBYTE(v375) = *(x_BYTE*)(v376 + v389);
											LOBYTE(v376) = BYTE2(v1124) + v180 + v376;
											v180 = __CFADD__(v1167, v383);
											v383 += v1167;
											BYTE1(v376) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1183, v384);
											v384 += v1183;
											v388[3] = m_ptrX_BYTE_F6EE0_tablesx[v375];
											BYTE1(v375) += BYTE2(v1146) + v180;
										LABEL_498:
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v383);
											LOWORD(v383) = v1124 + v383;
											LOBYTE(v375) = *(x_BYTE*)(v376 + v389);
											LOBYTE(v376) = BYTE2(v1124) + v180 + v376;
											v180 = __CFADD__(v1167, v383);
											v383 += v1167;
											BYTE1(v376) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1183, v384);
											v384 += v1183;
											v388[4] = m_ptrX_BYTE_F6EE0_tablesx[v375];
											BYTE1(v375) += BYTE2(v1146) + v180;
										LABEL_499:
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v383);
											LOWORD(v383) = v1124 + v383;
											LOBYTE(v375) = *(x_BYTE*)(v376 + v389);
											LOBYTE(v376) = BYTE2(v1124) + v180 + v376;
											v180 = __CFADD__(v1167, v383);
											v383 += v1167;
											BYTE1(v376) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1183, v384);
											v384 += v1183;
											v388[5] = m_ptrX_BYTE_F6EE0_tablesx[v375];
											BYTE1(v375) += BYTE2(v1146) + v180;
										LABEL_500:
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v383);
											LOWORD(v383) = v1124 + v383;
											LOBYTE(v375) = *(x_BYTE*)(v376 + v389);
											LOBYTE(v376) = BYTE2(v1124) + v180 + v376;
											v180 = __CFADD__(v1167, v383);
											v383 += v1167;
											BYTE1(v376) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1183, v384);
											v384 += v1183;
											v388[6] = m_ptrX_BYTE_F6EE0_tablesx[v375];
											BYTE1(v375) += BYTE2(v1146) + v180;
										LABEL_501:
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v383);
											LOWORD(v383) = v1124 + v383;
											LOBYTE(v375) = *(x_BYTE*)(v376 + v389);
											LOBYTE(v376) = BYTE2(v1124) + v180 + v376;
											v180 = __CFADD__(v1167, v383);
											v383 += v1167;
											BYTE1(v376) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1183, v384);
											v384 += v1183;
											v388[7] = m_ptrX_BYTE_F6EE0_tablesx[v375];
											BYTE1(v375) += BYTE2(v1146) + v180;
										LABEL_502:
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v383);
											LOWORD(v383) = v1124 + v383;
											LOBYTE(v375) = *(x_BYTE*)(v376 + v389);
											LOBYTE(v376) = BYTE2(v1124) + v180 + v376;
											v180 = __CFADD__(v1167, v383);
											v383 += v1167;
											BYTE1(v376) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1183, v384);
											v384 += v1183;
											v388[8] = m_ptrX_BYTE_F6EE0_tablesx[v375];
											BYTE1(v375) += BYTE2(v1146) + v180;
										LABEL_503:
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v383);
											LOWORD(v383) = v1124 + v383;
											LOBYTE(v375) = *(x_BYTE*)(v376 + v389);
											LOBYTE(v376) = BYTE2(v1124) + v180 + v376;
											v180 = __CFADD__(v1167, v383);
											v383 += v1167;
											BYTE1(v376) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1183, v384);
											v384 += v1183;
											v388[9] = m_ptrX_BYTE_F6EE0_tablesx[v375];
											BYTE1(v375) += BYTE2(v1146) + v180;
										LABEL_504:
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v383);
											LOWORD(v383) = v1124 + v383;
											LOBYTE(v375) = *(x_BYTE*)(v376 + v389);
											LOBYTE(v376) = BYTE2(v1124) + v180 + v376;
											v180 = __CFADD__(v1167, v383);
											v383 += v1167;
											BYTE1(v376) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1183, v384);
											v384 += v1183;
											v388[10] = m_ptrX_BYTE_F6EE0_tablesx[v375];
											BYTE1(v375) += BYTE2(v1146) + v180;
										LABEL_505:
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v383);
											LOWORD(v383) = v1124 + v383;
											LOBYTE(v375) = *(x_BYTE*)(v376 + v389);
											LOBYTE(v376) = BYTE2(v1124) + v180 + v376;
											v180 = __CFADD__(v1167, v383);
											v383 += v1167;
											BYTE1(v376) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1183, v384);
											v384 += v1183;
											v388[11] = m_ptrX_BYTE_F6EE0_tablesx[v375];
											BYTE1(v375) += BYTE2(v1146) + v180;
										LABEL_506:
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v383);
											LOWORD(v383) = v1124 + v383;
											LOBYTE(v375) = *(x_BYTE*)(v376 + v389);
											LOBYTE(v376) = BYTE2(v1124) + v180 + v376;
											v180 = __CFADD__(v1167, v383);
											v383 += v1167;
											BYTE1(v376) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1183, v384);
											v384 += v1183;
											v388[12] = m_ptrX_BYTE_F6EE0_tablesx[v375];
											BYTE1(v375) += BYTE2(v1146) + v180;
										LABEL_507:
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v383);
											LOWORD(v383) = v1124 + v383;
											LOBYTE(v375) = *(x_BYTE*)(v376 + v389);
											LOBYTE(v376) = BYTE2(v1124) + v180 + v376;
											v180 = __CFADD__(v1167, v383);
											v383 += v1167;
											BYTE1(v376) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1183, v384);
											v384 += v1183;
											v388[13] = m_ptrX_BYTE_F6EE0_tablesx[v375];
											BYTE1(v375) += BYTE2(v1146) + v180;
										LABEL_508:
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v383);
											LOWORD(v383) = v1124 + v383;
											LOBYTE(v375) = *(x_BYTE*)(v376 + v389);
											LOBYTE(v376) = BYTE2(v1124) + v180 + v376;
											v180 = __CFADD__(v1167, v383);
											v383 += v1167;
											BYTE1(v376) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1183, v384);
											v384 += v1183;
											v388[14] = m_ptrX_BYTE_F6EE0_tablesx[v375];
											BYTE1(v375) += BYTE2(v1146) + v180;
										LABEL_509:
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v383);
											LOWORD(v383) = v1124 + v383;
											LOBYTE(v375) = *(x_BYTE*)(v376 + v389);
											LOBYTE(v376) = BYTE2(v1124) + v180 + v376;
											v180 = __CFADD__(v1167, v383);
											v383 += v1167;
											BYTE1(v376) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1183, v384);
											v384 += v1183;
											v388[15] = m_ptrX_BYTE_F6EE0_tablesx[v375];
											BYTE1(v375) += BYTE2(v1146) + v180;
											v388 += 16;
											v18 = __OFSUB__((x_WORD)v384, 16);
											LOWORD(v384) = v384 - 16;
											if ((unsigned __int8)(((v384 & 0x8000u) != 0) ^ v18) | ((x_WORD)v384 == 0))
												break;
										LABEL_494:
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v383);
											LOWORD(v383) = v1124 + v383;
											LOBYTE(v375) = *(x_BYTE*)(v376 + v389);
											LOBYTE(v376) = BYTE2(v1124) + v180 + v376;
											v180 = __CFADD__(v1167, v383);
											v383 += v1167;
											BYTE1(v376) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1183, v384);
											v384 += v1183;
											*v388 = m_ptrX_BYTE_F6EE0_tablesx[v375];
											BYTE1(v375) += BYTE2(v1146) + v180;
										}
										break;
									}
								}
							}
						LABEL_510:
							if (!--triLn_v1123)
								return;
						}
						if (v378 > viewPortWidth)
							LOWORD(v378) = viewPortWidth;
						v18 = __OFSUB__((x_WORD)v378, (x_WORD)v375);
						v385 = v378 - v375;
						if ((unsigned __int8)((v385 < 0) ^ v18) | (v385 == 0))
							goto LABEL_510;
						v379 += v375;
						LOBYTE(v376) = v377[10];
						v383 = __ROL4__(*((x_DWORD*)v377 + 3), 16);
						v386 = v385;
						BYTE1(v376) = v383;
						LOWORD(v383) = *((x_WORD*)v377 + 4);
						v384 = __ROL4__(*((x_DWORD*)v377 + 4), 16);
						BYTE1(v375) = v384;
						LOWORD(v384) = v386;
						goto LABEL_493;
					case 6:
						v1277 = (char*)unk_DE56Cx;
						v1168 = v1135 << 16;
						v1184 = v1146 << 16;
						HIWORD(v390) = 0;
						HIWORD(v391) = 0;
						while (1)
						{
							v392 = v1277;
							v1277 += 20;
							LOWORD(v390) = *((x_WORD*)v392 + 1);
							v393 = *((unsigned __int16*)v392 + 3);
							v394 = pitch + v1102;
							v1102 += pitch;
							line7++;
							if (line7 >= drawEveryNthLine)
							{
								line7 = 0;
								if ((v390 & 0x8000u) == 0)
									break;
								if ((signed __int16)v393 > 0)
								{
									v395 = (unsigned __int16)-(signed __int16)v390;
									v396 = v395;
									v398 = __ROL4__(*((x_DWORD*)v392 + 3) + v1135 * v395, 16);
									BYTE1(v391) = v398;
									v397 = *((x_DWORD*)v392 + 2) + v1124 * v395;
									LOWORD(v398) = v397;
									v390 = v397 >> 8;
									LOBYTE(v391) = BYTE1(v390);
									v399 = __ROL4__(*((x_DWORD*)v392 + 4) + v1146 * v396, 16);
									BYTE1(v390) = v399;
									LOWORD(v399) = *((x_WORD*)v392 + 3);
									v390 = (unsigned __int16)v390;
									if ((signed __int16)v399 > (signed __int16)viewPortWidth)
										LOWORD(v399) = viewPortWidth;
								LABEL_522:
									v402 = v399 & 0xF;
									v403 = (x_BYTE*)(x_DWORD_B8845[v402] + v394);
									v404 = pTexture;
									switch (v402)
									{
									case 0:
										goto LABEL_523;
									case 1:
										goto LABEL_568;
									case 2:
										goto LABEL_565;
									case 3:
										goto LABEL_562;
									case 4:
										goto LABEL_559;
									case 5:
										goto LABEL_556;
									case 6:
										goto LABEL_553;
									case 7:
										goto LABEL_550;
									case 8:
										goto LABEL_547;
									case 9:
										goto LABEL_544;
									case 10:
										goto LABEL_541;
									case 11:
										goto LABEL_538;
									case 12:
										goto LABEL_535;
									case 13:
										goto LABEL_532;
									case 14:
										goto LABEL_529;
									case 15:
										while (1)
										{
											LOBYTE(v390) = *(x_BYTE*)(v391 + v404);
											if ((x_BYTE)v390)
												v403[1] = m_ptrX_BYTE_F6EE0_tablesx[v390];
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v398);
											LOWORD(v398) = v1124 + v398;
											LOBYTE(v391) = BYTE2(v1124) + v180 + v391;
											v180 = __CFADD__(v1168, v398);
											v398 += v1168;
											BYTE1(v391) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1184, v399);
											v399 += v1184;
											BYTE1(v390) += BYTE2(v1146) + v180;
										LABEL_529:
											LOBYTE(v390) = *(x_BYTE*)(v391 + v404);
											if ((x_BYTE)v390)
												v403[2] = m_ptrX_BYTE_F6EE0_tablesx[v390];
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v398);
											LOWORD(v398) = v1124 + v398;
											LOBYTE(v391) = BYTE2(v1124) + v180 + v391;
											v180 = __CFADD__(v1168, v398);
											v398 += v1168;
											BYTE1(v391) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1184, v399);
											v399 += v1184;
											BYTE1(v390) += BYTE2(v1146) + v180;
										LABEL_532:
											LOBYTE(v390) = *(x_BYTE*)(v391 + v404);
											if ((x_BYTE)v390)
												v403[3] = m_ptrX_BYTE_F6EE0_tablesx[v390];
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v398);
											LOWORD(v398) = v1124 + v398;
											LOBYTE(v391) = BYTE2(v1124) + v180 + v391;
											v180 = __CFADD__(v1168, v398);
											v398 += v1168;
											BYTE1(v391) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1184, v399);
											v399 += v1184;
											BYTE1(v390) += BYTE2(v1146) + v180;
										LABEL_535:
											LOBYTE(v390) = *(x_BYTE*)(v391 + v404);
											if ((x_BYTE)v390)
												v403[4] = m_ptrX_BYTE_F6EE0_tablesx[v390];
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v398);
											LOWORD(v398) = v1124 + v398;
											LOBYTE(v391) = BYTE2(v1124) + v180 + v391;
											v180 = __CFADD__(v1168, v398);
											v398 += v1168;
											BYTE1(v391) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1184, v399);
											v399 += v1184;
											BYTE1(v390) += BYTE2(v1146) + v180;
										LABEL_538:
											LOBYTE(v390) = *(x_BYTE*)(v391 + v404);
											if ((x_BYTE)v390)
												v403[5] = m_ptrX_BYTE_F6EE0_tablesx[v390];
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v398);
											LOWORD(v398) = v1124 + v398;
											LOBYTE(v391) = BYTE2(v1124) + v180 + v391;
											v180 = __CFADD__(v1168, v398);
											v398 += v1168;
											BYTE1(v391) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1184, v399);
											v399 += v1184;
											BYTE1(v390) += BYTE2(v1146) + v180;
										LABEL_541:
											LOBYTE(v390) = *(x_BYTE*)(v391 + v404);
											if ((x_BYTE)v390)
												v403[6] = m_ptrX_BYTE_F6EE0_tablesx[v390];
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v398);
											LOWORD(v398) = v1124 + v398;
											LOBYTE(v391) = BYTE2(v1124) + v180 + v391;
											v180 = __CFADD__(v1168, v398);
											v398 += v1168;
											BYTE1(v391) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1184, v399);
											v399 += v1184;
											BYTE1(v390) += BYTE2(v1146) + v180;
										LABEL_544:
											LOBYTE(v390) = *(x_BYTE*)(v391 + v404);
											if ((x_BYTE)v390)
												v403[7] = m_ptrX_BYTE_F6EE0_tablesx[v390];
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v398);
											LOWORD(v398) = v1124 + v398;
											LOBYTE(v391) = BYTE2(v1124) + v180 + v391;
											v180 = __CFADD__(v1168, v398);
											v398 += v1168;
											BYTE1(v391) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1184, v399);
											v399 += v1184;
											BYTE1(v390) += BYTE2(v1146) + v180;
										LABEL_547:
											LOBYTE(v390) = *(x_BYTE*)(v391 + v404);
											if ((x_BYTE)v390)
												v403[8] = m_ptrX_BYTE_F6EE0_tablesx[v390];
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v398);
											LOWORD(v398) = v1124 + v398;
											LOBYTE(v391) = BYTE2(v1124) + v180 + v391;
											v180 = __CFADD__(v1168, v398);
											v398 += v1168;
											BYTE1(v391) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1184, v399);
											v399 += v1184;
											BYTE1(v390) += BYTE2(v1146) + v180;
										LABEL_550:
											LOBYTE(v390) = *(x_BYTE*)(v391 + v404);
											if ((x_BYTE)v390)
												v403[9] = m_ptrX_BYTE_F6EE0_tablesx[v390];
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v398);
											LOWORD(v398) = v1124 + v398;
											LOBYTE(v391) = BYTE2(v1124) + v180 + v391;
											v180 = __CFADD__(v1168, v398);
											v398 += v1168;
											BYTE1(v391) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1184, v399);
											v399 += v1184;
											BYTE1(v390) += BYTE2(v1146) + v180;
										LABEL_553:
											LOBYTE(v390) = *(x_BYTE*)(v391 + v404);
											if ((x_BYTE)v390)
												v403[10] = m_ptrX_BYTE_F6EE0_tablesx[v390];
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v398);
											LOWORD(v398) = v1124 + v398;
											LOBYTE(v391) = BYTE2(v1124) + v180 + v391;
											v180 = __CFADD__(v1168, v398);
											v398 += v1168;
											BYTE1(v391) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1184, v399);
											v399 += v1184;
											BYTE1(v390) += BYTE2(v1146) + v180;
										LABEL_556:
											LOBYTE(v390) = *(x_BYTE*)(v391 + v404);
											if ((x_BYTE)v390)
												v403[11] = m_ptrX_BYTE_F6EE0_tablesx[v390];
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v398);
											LOWORD(v398) = v1124 + v398;
											LOBYTE(v391) = BYTE2(v1124) + v180 + v391;
											v180 = __CFADD__(v1168, v398);
											v398 += v1168;
											BYTE1(v391) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1184, v399);
											v399 += v1184;
											BYTE1(v390) += BYTE2(v1146) + v180;
										LABEL_559:
											LOBYTE(v390) = *(x_BYTE*)(v391 + v404);
											if ((x_BYTE)v390)
												v403[12] = m_ptrX_BYTE_F6EE0_tablesx[v390];
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v398);
											LOWORD(v398) = v1124 + v398;
											LOBYTE(v391) = BYTE2(v1124) + v180 + v391;
											v180 = __CFADD__(v1168, v398);
											v398 += v1168;
											BYTE1(v391) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1184, v399);
											v399 += v1184;
											BYTE1(v390) += BYTE2(v1146) + v180;
										LABEL_562:
											LOBYTE(v390) = *(x_BYTE*)(v391 + v404);
											if ((x_BYTE)v390)
												v403[13] = m_ptrX_BYTE_F6EE0_tablesx[v390];
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v398);
											LOWORD(v398) = v1124 + v398;
											LOBYTE(v391) = BYTE2(v1124) + v180 + v391;
											v180 = __CFADD__(v1168, v398);
											v398 += v1168;
											BYTE1(v391) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1184, v399);
											v399 += v1184;
											BYTE1(v390) += BYTE2(v1146) + v180;
										LABEL_565:
											LOBYTE(v390) = *(x_BYTE*)(v391 + v404);
											if ((x_BYTE)v390)
												v403[14] = m_ptrX_BYTE_F6EE0_tablesx[v390];
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v398);
											LOWORD(v398) = v1124 + v398;
											LOBYTE(v391) = BYTE2(v1124) + v180 + v391;
											v180 = __CFADD__(v1168, v398);
											v398 += v1168;
											BYTE1(v391) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1184, v399);
											v399 += v1184;
											BYTE1(v390) += BYTE2(v1146) + v180;
										LABEL_568:
											LOBYTE(v390) = *(x_BYTE*)(v391 + v404);
											if ((x_BYTE)v390)
												v403[15] = m_ptrX_BYTE_F6EE0_tablesx[v390];
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v398);
											LOWORD(v398) = v1124 + v398;
											LOBYTE(v391) = BYTE2(v1124) + v180 + v391;
											v180 = __CFADD__(v1168, v398);
											v398 += v1168;
											BYTE1(v391) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1184, v399);
											v399 += v1184;
											BYTE1(v390) += BYTE2(v1146) + v180;
											v403 += 16;
											v18 = __OFSUB__((x_WORD)v399, 16);
											LOWORD(v399) = v399 - 16;
											if ((unsigned __int8)(((v399 & 0x8000u) != 0) ^ v18) | ((x_WORD)v399 == 0))
												break;
										LABEL_523:
											LOBYTE(v390) = *(x_BYTE*)(v391 + v404);
											if ((x_BYTE)v390)
												*v403 = m_ptrX_BYTE_F6EE0_tablesx[v390];
											v180 = __CFADD__((x_WORD)v1124, (x_WORD)v398);
											LOWORD(v398) = v1124 + v398;
											LOBYTE(v391) = BYTE2(v1124) + v180 + v391;
											v180 = __CFADD__(v1168, v398);
											v398 += v1168;
											BYTE1(v391) += BYTE2(v1135) + v180;
											v180 = __CFADD__(v1184, v399);
											v399 += v1184;
											BYTE1(v390) += BYTE2(v1146) + v180;
										}
										break;
									}
								}
							}
							LABEL_571:
								if (!--triLn_v1123)
									return;
						}
						if (v393 > viewPortWidth)
							LOWORD(v393) = viewPortWidth;
						v18 = __OFSUB__((x_WORD)v393, (x_WORD)v390);
						v400 = v393 - v390;
						if ((unsigned __int8)((v400 < 0) ^ v18) | (v400 == 0))
							goto LABEL_571;
						v394 += v390;
						LOBYTE(v391) = v392[10];
						v398 = __ROL4__(*((x_DWORD*)v392 + 3), 16);
						v401 = v400;
						BYTE1(v391) = v398;
						LOWORD(v398) = *((x_WORD*)v392 + 4);
						v399 = __ROL4__(*((x_DWORD*)v392 + 4), 16);
						BYTE1(v390) = v399;
						LOWORD(v399) = v401;
						goto LABEL_522;
					case 7:
					case 0xB:
						v405 = (unsigned __int16*)unk_DE56Cx;
						v1169 = v1135 << 16;
						HIWORD(v406) = 0;
						HIWORD(v407) = 0;
						while (1)
						{
							LOWORD(v406) = v405[1];
							v408 = v405[3];
							v409 = (x_BYTE*)(pitch + v1102);
							v1102 += pitch;
							line8++;
							if (line8 >= drawEveryNthLine)
							{
								line8 = 0;
								if ((v406 & 0x8000u) == 0)
									break;
								if ((signed __int16)v408 > 0)
								{
									v410 = (unsigned __int16)-(signed __int16)v406;
									v412 = __ROL4__(*((x_DWORD*)v405 + 3) + v1135 * v410, 16);
									BYTE1(v407) = v412;
									v411 = *((x_DWORD*)v405 + 2) + v1124 * v410;
									LOWORD(v412) = v411;
									v413 = v411 >> 8;
									LOBYTE(v407) = BYTE1(v413);
									if (v408 > viewPortWidth)
										LOWORD(v408) = viewPortWidth;
									v406 = (unsigned __int16)v413;
								LABEL_583:
									v1278 = v405;
									v414 = pTexture;
									BYTE1(v406) = x_BYTE_E126C;
									while (1)
									{
										LOBYTE(v406) = *(x_BYTE*)(v407 + v414);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v412);
										LOWORD(v412) = v1124 + v412;
										LOBYTE(v407) = BYTE2(v1124) + v180 + v407;
										v180 = __CFADD__(v1169, v412);
										v415 = v1169 + v412;
										*v409 = m_ptrX_BYTE_F6EE0_tablesx[v406];
										BYTE1(v407) += BYTE2(v1135) + v180;
										v416 = v408 - 1;
										if (!v416)
											break;
										LOBYTE(v406) = *(x_BYTE*)(v407 + v414);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v415);
										LOWORD(v415) = v1124 + v415;
										LOBYTE(v407) = BYTE2(v1124) + v180 + v407;
										v180 = __CFADD__(v1169, v415);
										v417 = v1169 + v415;
										v409[1] = m_ptrX_BYTE_F6EE0_tablesx[v406];
										BYTE1(v407) += BYTE2(v1135) + v180;
										v418 = v416 - 1;
										if (!v418)
											break;
										LOBYTE(v406) = *(x_BYTE*)(v407 + v414);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v417);
										LOWORD(v417) = v1124 + v417;
										LOBYTE(v407) = BYTE2(v1124) + v180 + v407;
										v180 = __CFADD__(v1169, v417);
										v419 = v1169 + v417;
										v409[2] = m_ptrX_BYTE_F6EE0_tablesx[v406];
										BYTE1(v407) += BYTE2(v1135) + v180;
										v420 = v418 - 1;
										if (!v420)
											break;
										LOBYTE(v406) = *(x_BYTE*)(v407 + v414);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v419);
										LOWORD(v419) = v1124 + v419;
										LOBYTE(v407) = BYTE2(v1124) + v180 + v407;
										v180 = __CFADD__(v1169, v419);
										v421 = v1169 + v419;
										v409[3] = m_ptrX_BYTE_F6EE0_tablesx[v406];
										BYTE1(v407) += BYTE2(v1135) + v180;
										v422 = v420 - 1;
										if (!v422)
											break;
										LOBYTE(v406) = *(x_BYTE*)(v407 + v414);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v421);
										LOWORD(v421) = v1124 + v421;
										LOBYTE(v407) = BYTE2(v1124) + v180 + v407;
										v180 = __CFADD__(v1169, v421);
										v423 = v1169 + v421;
										v409[4] = m_ptrX_BYTE_F6EE0_tablesx[v406];
										BYTE1(v407) += BYTE2(v1135) + v180;
										v424 = v422 - 1;
										if (!v424)
											break;
										LOBYTE(v406) = *(x_BYTE*)(v407 + v414);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v423);
										LOWORD(v423) = v1124 + v423;
										LOBYTE(v407) = BYTE2(v1124) + v180 + v407;
										v180 = __CFADD__(v1169, v423);
										v425 = v1169 + v423;
										v409[5] = m_ptrX_BYTE_F6EE0_tablesx[v406];
										BYTE1(v407) += BYTE2(v1135) + v180;
										v426 = v424 - 1;
										if (!v426)
											break;
										LOBYTE(v406) = *(x_BYTE*)(v407 + v414);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v425);
										LOWORD(v425) = v1124 + v425;
										LOBYTE(v407) = BYTE2(v1124) + v180 + v407;
										v180 = __CFADD__(v1169, v425);
										v427 = v1169 + v425;
										v409[6] = m_ptrX_BYTE_F6EE0_tablesx[v406];
										BYTE1(v407) += BYTE2(v1135) + v180;
										v428 = v426 - 1;
										if (!v428)
											break;
										LOBYTE(v406) = *(x_BYTE*)(v407 + v414);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v427);
										LOWORD(v427) = v1124 + v427;
										LOBYTE(v407) = BYTE2(v1124) + v180 + v407;
										v180 = __CFADD__(v1169, v427);
										v429 = v1169 + v427;
										v409[7] = m_ptrX_BYTE_F6EE0_tablesx[v406];
										BYTE1(v407) += BYTE2(v1135) + v180;
										v430 = v428 - 1;
										if (!v430)
											break;
										LOBYTE(v406) = *(x_BYTE*)(v407 + v414);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v429);
										LOWORD(v429) = v1124 + v429;
										LOBYTE(v407) = BYTE2(v1124) + v180 + v407;
										v180 = __CFADD__(v1169, v429);
										v431 = v1169 + v429;
										v409[8] = m_ptrX_BYTE_F6EE0_tablesx[v406];
										BYTE1(v407) += BYTE2(v1135) + v180;
										v432 = v430 - 1;
										if (!v432)
											break;
										LOBYTE(v406) = *(x_BYTE*)(v407 + v414);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v431);
										LOWORD(v431) = v1124 + v431;
										LOBYTE(v407) = BYTE2(v1124) + v180 + v407;
										v180 = __CFADD__(v1169, v431);
										v433 = v1169 + v431;
										v409[9] = m_ptrX_BYTE_F6EE0_tablesx[v406];
										BYTE1(v407) += BYTE2(v1135) + v180;
										v434 = v432 - 1;
										if (!v434)
											break;
										LOBYTE(v406) = *(x_BYTE*)(v407 + v414);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v433);
										LOWORD(v433) = v1124 + v433;
										LOBYTE(v407) = BYTE2(v1124) + v180 + v407;
										v180 = __CFADD__(v1169, v433);
										v435 = v1169 + v433;
										v409[10] = m_ptrX_BYTE_F6EE0_tablesx[v406];
										BYTE1(v407) += BYTE2(v1135) + v180;
										v436 = v434 - 1;
										if (!v436)
											break;
										LOBYTE(v406) = *(x_BYTE*)(v407 + v414);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v435);
										LOWORD(v435) = v1124 + v435;
										LOBYTE(v407) = BYTE2(v1124) + v180 + v407;
										v180 = __CFADD__(v1169, v435);
										v437 = v1169 + v435;
										v409[11] = m_ptrX_BYTE_F6EE0_tablesx[v406];
										BYTE1(v407) += BYTE2(v1135) + v180;
										v438 = v436 - 1;
										if (!v438)
											break;
										LOBYTE(v406) = *(x_BYTE*)(v407 + v414);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v437);
										LOWORD(v437) = v1124 + v437;
										LOBYTE(v407) = BYTE2(v1124) + v180 + v407;
										v180 = __CFADD__(v1169, v437);
										v439 = v1169 + v437;
										v409[12] = m_ptrX_BYTE_F6EE0_tablesx[v406];
										BYTE1(v407) += BYTE2(v1135) + v180;
										v440 = v438 - 1;
										if (!v440)
											break;
										LOBYTE(v406) = *(x_BYTE*)(v407 + v414);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v439);
										LOWORD(v439) = v1124 + v439;
										LOBYTE(v407) = BYTE2(v1124) + v180 + v407;
										v180 = __CFADD__(v1169, v439);
										v441 = v1169 + v439;
										v409[13] = m_ptrX_BYTE_F6EE0_tablesx[v406];
										BYTE1(v407) += BYTE2(v1135) + v180;
										v442 = v440 - 1;
										if (!v442)
											break;
										LOBYTE(v406) = *(x_BYTE*)(v407 + v414);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v441);
										LOWORD(v441) = v1124 + v441;
										LOBYTE(v407) = BYTE2(v1124) + v180 + v407;
										v180 = __CFADD__(v1169, v441);
										v443 = v1169 + v441;
										v409[14] = m_ptrX_BYTE_F6EE0_tablesx[v406];
										BYTE1(v407) += BYTE2(v1135) + v180;
										v444 = v442 - 1;
										if (!v444)
											break;
										LOBYTE(v406) = *(x_BYTE*)(v407 + v414);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v443);
										LOWORD(v443) = v1124 + v443;
										LOBYTE(v407) = BYTE2(v1124) + v180 + v407;
										v180 = __CFADD__(v1169, v443);
										v412 = v1169 + v443;
										v409[15] = m_ptrX_BYTE_F6EE0_tablesx[v406];
										BYTE1(v407) += BYTE2(v1135) + v180;
										LOWORD(v408) = v444 - 1;
										if (!(x_WORD)v408)
											break;
										v409 += 16;
									}
									v405 = v1278;
								}
							}
						LABEL_602:
							v405 += 10;
							if (!--triLn_v1123)
								return;
						}
						if (v408 > viewPortWidth)
							LOWORD(v408) = viewPortWidth;
						v18 = __OFSUB__((x_WORD)v408, (x_WORD)v406);
						LOWORD(v408) = v408 - v406;
						if ((unsigned __int8)(((v408 & 0x8000u) != 0) ^ v18) | ((x_WORD)v408 == 0))
							goto LABEL_602;
						v409 += v406;
						v412 = __ROL4__(*((x_DWORD*)v405 + 3), 16);
						BYTE1(v407) = v412;
						LOWORD(v412) = v405[4];
						LOBYTE(v407) = *((x_BYTE*)v405 + 10);
						goto LABEL_583;
					case 8:
						v445 = (unsigned __int16*)unk_DE56Cx;
						v1170 = v1135 << 16;
						HIWORD(v446) = 0;
						HIWORD(v447) = 0;
						while (1)
						{
							LOWORD(v446) = v445[1];
							v448 = v445[3];
							v449 = (x_BYTE*)(pitch + v1102);
							v1102 += pitch;
							line9++;
							if (line9 >= drawEveryNthLine)
							{
								line9 = 0;
								if ((v446 & 0x8000u) == 0)
									break;
								if ((signed __int16)v448 > 0)
								{
									v450 = (unsigned __int16)-(signed __int16)v446;
									v452 = __ROL4__(*((x_DWORD*)v445 + 3) + v1135 * v450, 16);
									BYTE1(v447) = v452;
									v451 = *((x_DWORD*)v445 + 2) + v1124 * v450;
									LOWORD(v452) = v451;
									v453 = v451 >> 8;
									LOBYTE(v447) = BYTE1(v453);
									if (v448 > viewPortWidth)
										LOWORD(v448) = viewPortWidth;
									v446 = (unsigned __int16)v453;
								LABEL_614:
									v1279 = v445;
									v454 = pTexture;
									BYTE1(v446) = x_BYTE_E126C;
									while (1)
									{
										LOBYTE(v446) = *(x_BYTE*)(v447 + v454);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v452);
										LOWORD(v452) = v1124 + v452;
										LOBYTE(v447) = BYTE2(v1124) + v180 + v447;
										if ((x_BYTE)v446)
											*v449 = m_ptrX_BYTE_F6EE0_tablesx[v446];
										v180 = __CFADD__(v1170, v452);
										v455 = v1170 + v452;
										BYTE1(v447) += BYTE2(v1135) + v180;
										v456 = v448 - 1;
										if (!v456)
											break;
										LOBYTE(v446) = *(x_BYTE*)(v447 + v454);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v455);
										LOWORD(v455) = v1124 + v455;
										LOBYTE(v447) = BYTE2(v1124) + v180 + v447;
										if ((x_BYTE)v446)
											v449[1] = m_ptrX_BYTE_F6EE0_tablesx[v446];
										v180 = __CFADD__(v1170, v455);
										v457 = v1170 + v455;
										BYTE1(v447) += BYTE2(v1135) + v180;
										v458 = v456 - 1;
										if (!v458)
											break;
										LOBYTE(v446) = *(x_BYTE*)(v447 + v454);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v457);
										LOWORD(v457) = v1124 + v457;
										LOBYTE(v447) = BYTE2(v1124) + v180 + v447;
										if ((x_BYTE)v446)
											v449[2] = m_ptrX_BYTE_F6EE0_tablesx[v446];
										v180 = __CFADD__(v1170, v457);
										v459 = v1170 + v457;
										BYTE1(v447) += BYTE2(v1135) + v180;
										v460 = v458 - 1;
										if (!v460)
											break;
										LOBYTE(v446) = *(x_BYTE*)(v447 + v454);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v459);
										LOWORD(v459) = v1124 + v459;
										LOBYTE(v447) = BYTE2(v1124) + v180 + v447;
										if ((x_BYTE)v446)
											v449[3] = m_ptrX_BYTE_F6EE0_tablesx[v446];
										v180 = __CFADD__(v1170, v459);
										v461 = v1170 + v459;
										BYTE1(v447) += BYTE2(v1135) + v180;
										v462 = v460 - 1;
										if (!v462)
											break;
										LOBYTE(v446) = *(x_BYTE*)(v447 + v454);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v461);
										LOWORD(v461) = v1124 + v461;
										LOBYTE(v447) = BYTE2(v1124) + v180 + v447;
										if ((x_BYTE)v446)
											v449[4] = m_ptrX_BYTE_F6EE0_tablesx[v446];
										v180 = __CFADD__(v1170, v461);
										v463 = v1170 + v461;
										BYTE1(v447) += BYTE2(v1135) + v180;
										v464 = v462 - 1;
										if (!v464)
											break;
										LOBYTE(v446) = *(x_BYTE*)(v447 + v454);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v463);
										LOWORD(v463) = v1124 + v463;
										LOBYTE(v447) = BYTE2(v1124) + v180 + v447;
										if ((x_BYTE)v446)
											v449[5] = m_ptrX_BYTE_F6EE0_tablesx[v446];
										v180 = __CFADD__(v1170, v463);
										v465 = v1170 + v463;
										BYTE1(v447) += BYTE2(v1135) + v180;
										v466 = v464 - 1;
										if (!v466)
											break;
										LOBYTE(v446) = *(x_BYTE*)(v447 + v454);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v465);
										LOWORD(v465) = v1124 + v465;
										LOBYTE(v447) = BYTE2(v1124) + v180 + v447;
										if ((x_BYTE)v446)
											v449[6] = m_ptrX_BYTE_F6EE0_tablesx[v446];
										v180 = __CFADD__(v1170, v465);
										v467 = v1170 + v465;
										BYTE1(v447) += BYTE2(v1135) + v180;
										v468 = v466 - 1;
										if (!v468)
											break;
										LOBYTE(v446) = *(x_BYTE*)(v447 + v454);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v467);
										LOWORD(v467) = v1124 + v467;
										LOBYTE(v447) = BYTE2(v1124) + v180 + v447;
										if ((x_BYTE)v446)
											v449[7] = m_ptrX_BYTE_F6EE0_tablesx[v446];
										v180 = __CFADD__(v1170, v467);
										v469 = v1170 + v467;
										BYTE1(v447) += BYTE2(v1135) + v180;
										v470 = v468 - 1;
										if (!v470)
											break;
										LOBYTE(v446) = *(x_BYTE*)(v447 + v454);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v469);
										LOWORD(v469) = v1124 + v469;
										LOBYTE(v447) = BYTE2(v1124) + v180 + v447;
										if ((x_BYTE)v446)
											v449[8] = m_ptrX_BYTE_F6EE0_tablesx[v446];
										v180 = __CFADD__(v1170, v469);
										v471 = v1170 + v469;
										BYTE1(v447) += BYTE2(v1135) + v180;
										v472 = v470 - 1;
										if (!v472)
											break;
										LOBYTE(v446) = *(x_BYTE*)(v447 + v454);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v471);
										LOWORD(v471) = v1124 + v471;
										LOBYTE(v447) = BYTE2(v1124) + v180 + v447;
										if ((x_BYTE)v446)
											v449[9] = m_ptrX_BYTE_F6EE0_tablesx[v446];
										v180 = __CFADD__(v1170, v471);
										v473 = v1170 + v471;
										BYTE1(v447) += BYTE2(v1135) + v180;
										v474 = v472 - 1;
										if (!v474)
											break;
										LOBYTE(v446) = *(x_BYTE*)(v447 + v454);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v473);
										LOWORD(v473) = v1124 + v473;
										LOBYTE(v447) = BYTE2(v1124) + v180 + v447;
										if ((x_BYTE)v446)
											v449[10] = m_ptrX_BYTE_F6EE0_tablesx[v446];
										v180 = __CFADD__(v1170, v473);
										v475 = v1170 + v473;
										BYTE1(v447) += BYTE2(v1135) + v180;
										v476 = v474 - 1;
										if (!v476)
											break;
										LOBYTE(v446) = *(x_BYTE*)(v447 + v454);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v475);
										LOWORD(v475) = v1124 + v475;
										LOBYTE(v447) = BYTE2(v1124) + v180 + v447;
										if ((x_BYTE)v446)
											v449[11] = m_ptrX_BYTE_F6EE0_tablesx[v446];
										v180 = __CFADD__(v1170, v475);
										v477 = v1170 + v475;
										BYTE1(v447) += BYTE2(v1135) + v180;
										v478 = v476 - 1;
										if (!v478)
											break;
										LOBYTE(v446) = *(x_BYTE*)(v447 + v454);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v477);
										LOWORD(v477) = v1124 + v477;
										LOBYTE(v447) = BYTE2(v1124) + v180 + v447;
										if ((x_BYTE)v446)
											v449[12] = m_ptrX_BYTE_F6EE0_tablesx[v446];
										v180 = __CFADD__(v1170, v477);
										v479 = v1170 + v477;
										BYTE1(v447) += BYTE2(v1135) + v180;
										v480 = v478 - 1;
										if (!v480)
											break;
										LOBYTE(v446) = *(x_BYTE*)(v447 + v454);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v479);
										LOWORD(v479) = v1124 + v479;
										LOBYTE(v447) = BYTE2(v1124) + v180 + v447;
										if ((x_BYTE)v446)
											v449[13] = m_ptrX_BYTE_F6EE0_tablesx[v446];
										v180 = __CFADD__(v1170, v479);
										v481 = v1170 + v479;
										BYTE1(v447) += BYTE2(v1135) + v180;
										v482 = v480 - 1;
										if (!v482)
											break;
										LOBYTE(v446) = *(x_BYTE*)(v447 + v454);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v481);
										LOWORD(v481) = v1124 + v481;
										LOBYTE(v447) = BYTE2(v1124) + v180 + v447;
										if ((x_BYTE)v446)
											v449[14] = m_ptrX_BYTE_F6EE0_tablesx[v446];
										v180 = __CFADD__(v1170, v481);
										v483 = v1170 + v481;
										BYTE1(v447) += BYTE2(v1135) + v180;
										v484 = v482 - 1;
										if (!v484)
											break;
										LOBYTE(v446) = *(x_BYTE*)(v447 + v454);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v483);
										LOWORD(v483) = v1124 + v483;
										LOBYTE(v447) = BYTE2(v1124) + v180 + v447;
										if ((x_BYTE)v446)
											v449[15] = m_ptrX_BYTE_F6EE0_tablesx[v446];
										v180 = __CFADD__(v1170, v483);
										v452 = v1170 + v483;
										BYTE1(v447) += BYTE2(v1135) + v180;
										LOWORD(v448) = v484 - 1;
										if (!(x_WORD)v448)
											break;
										v449 += 16;
									}
									v445 = v1279;
								}
							}
						LABEL_665:
							v445 += 10;
							if (!--triLn_v1123)
								return;
						}
						if (v448 > viewPortWidth)
							LOWORD(v448) = viewPortWidth;
						v18 = __OFSUB__((x_WORD)v448, (x_WORD)v446);
						LOWORD(v448) = v448 - v446;
						if ((unsigned __int8)(((v448 & 0x8000u) != 0) ^ v18) | ((x_WORD)v448 == 0))
							goto LABEL_665;
						v449 += v446;
						v452 = __ROL4__(*((x_DWORD*)v445 + 3), 16);
						BYTE1(v447) = v452;
						LOWORD(v452) = v445[4];
						LOBYTE(v447) = *((x_BYTE*)v445 + 10);
						goto LABEL_614;
					case 9:
					case 0xA:
						v485 = (unsigned __int16*)unk_DE56Cx;
						v1171 = v1135 << 16;
						HIWORD(v486) = 0;
						HIWORD(v487) = 0;
						while (1)
						{
							LOWORD(v486) = v485[1];
							v488 = v485[3];
							v489 = (x_BYTE*)(pitch + v1102);
							v1102 += pitch;
							line10++;
							if (line10 >= drawEveryNthLine)
							{
								line10 = 0;
								if ((v486 & 0x8000u) == 0)
									break;
								if ((signed __int16)v488 > 0)
								{
									v490 = (unsigned __int16)-(signed __int16)v486;
									v492 = __ROL4__(*((x_DWORD*)v485 + 3) + v1135 * v490, 16);
									BYTE1(v487) = v492;
									v491 = *((x_DWORD*)v485 + 2) + v1124 * v490;
									LOWORD(v492) = v491;
									v493 = v491 >> 8;
									LOBYTE(v487) = BYTE1(v493);
									if (v488 > viewPortWidth)
										LOWORD(v488) = viewPortWidth;
									v486 = (unsigned __int16)v493;
								LABEL_677:
									v1280 = v485;
									v494 = pTexture;
									while (1)
									{
										BYTE1(v486) = *(x_BYTE*)(v487 + v494);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v492);
										LOWORD(v492) = v1124 + v492;
										LOBYTE(v487) = BYTE2(v1124) + v180 + v487;
										if (BYTE1(v486))
										{
											LOBYTE(v486) = *v489;
											*v489 = m_ptrX_BYTE_F6EE0_tablesx[v486];
										}
										v180 = __CFADD__(v1171, v492);
										v495 = v1171 + v492;
										BYTE1(v487) += BYTE2(v1135) + v180;
										v496 = v488 - 1;
										if (!v496)
											break;
										BYTE1(v486) = *(x_BYTE*)(v487 + v494);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v495);
										LOWORD(v495) = v1124 + v495;
										LOBYTE(v487) = BYTE2(v1124) + v180 + v487;
										if (BYTE1(v486))
										{
											LOBYTE(v486) = v489[1];
											v489[1] = m_ptrX_BYTE_F6EE0_tablesx[v486];
										}
										v180 = __CFADD__(v1171, v495);
										v497 = v1171 + v495;
										BYTE1(v487) += BYTE2(v1135) + v180;
										v498 = v496 - 1;
										if (!v498)
											break;
										BYTE1(v486) = *(x_BYTE*)(v487 + v494);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v497);
										LOWORD(v497) = v1124 + v497;
										LOBYTE(v487) = BYTE2(v1124) + v180 + v487;
										if (BYTE1(v486))
										{
											LOBYTE(v486) = v489[2];
											v489[2] = m_ptrX_BYTE_F6EE0_tablesx[v486];
										}
										v180 = __CFADD__(v1171, v497);
										v499 = v1171 + v497;
										BYTE1(v487) += BYTE2(v1135) + v180;
										v500 = v498 - 1;
										if (!v500)
											break;
										BYTE1(v486) = *(x_BYTE*)(v487 + v494);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v499);
										LOWORD(v499) = v1124 + v499;
										LOBYTE(v487) = BYTE2(v1124) + v180 + v487;
										if (BYTE1(v486))
										{
											LOBYTE(v486) = v489[3];
											v489[3] = m_ptrX_BYTE_F6EE0_tablesx[v486];
										}
										v180 = __CFADD__(v1171, v499);
										v501 = v1171 + v499;
										BYTE1(v487) += BYTE2(v1135) + v180;
										v502 = v500 - 1;
										if (!v502)
											break;
										BYTE1(v486) = *(x_BYTE*)(v487 + v494);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v501);
										LOWORD(v501) = v1124 + v501;
										LOBYTE(v487) = BYTE2(v1124) + v180 + v487;
										if (BYTE1(v486))
										{
											LOBYTE(v486) = v489[4];
											v489[4] = m_ptrX_BYTE_F6EE0_tablesx[v486];
										}
										v180 = __CFADD__(v1171, v501);
										v503 = v1171 + v501;
										BYTE1(v487) += BYTE2(v1135) + v180;
										v504 = v502 - 1;
										if (!v504)
											break;
										BYTE1(v486) = *(x_BYTE*)(v487 + v494);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v503);
										LOWORD(v503) = v1124 + v503;
										LOBYTE(v487) = BYTE2(v1124) + v180 + v487;
										if (BYTE1(v486))
										{
											LOBYTE(v486) = v489[5];
											v489[5] = m_ptrX_BYTE_F6EE0_tablesx[v486];
										}
										v180 = __CFADD__(v1171, v503);
										v505 = v1171 + v503;
										BYTE1(v487) += BYTE2(v1135) + v180;
										v506 = v504 - 1;
										if (!v506)
											break;
										BYTE1(v486) = *(x_BYTE*)(v487 + v494);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v505);
										LOWORD(v505) = v1124 + v505;
										LOBYTE(v487) = BYTE2(v1124) + v180 + v487;
										if (BYTE1(v486))
										{
											LOBYTE(v486) = v489[6];
											v489[6] = m_ptrX_BYTE_F6EE0_tablesx[v486];
										}
										v180 = __CFADD__(v1171, v505);
										v507 = v1171 + v505;
										BYTE1(v487) += BYTE2(v1135) + v180;
										v508 = v506 - 1;
										if (!v508)
											break;
										BYTE1(v486) = *(x_BYTE*)(v487 + v494);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v507);
										LOWORD(v507) = v1124 + v507;
										LOBYTE(v487) = BYTE2(v1124) + v180 + v487;
										if (BYTE1(v486))
										{
											LOBYTE(v486) = v489[7];
											v489[7] = m_ptrX_BYTE_F6EE0_tablesx[v486];
										}
										v180 = __CFADD__(v1171, v507);
										v509 = v1171 + v507;
										BYTE1(v487) += BYTE2(v1135) + v180;
										v510 = v508 - 1;
										if (!v510)
											break;
										BYTE1(v486) = *(x_BYTE*)(v487 + v494);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v509);
										LOWORD(v509) = v1124 + v509;
										LOBYTE(v487) = BYTE2(v1124) + v180 + v487;
										if (BYTE1(v486))
										{
											LOBYTE(v486) = v489[8];
											v489[8] = m_ptrX_BYTE_F6EE0_tablesx[v486];
										}
										v180 = __CFADD__(v1171, v509);
										v511 = v1171 + v509;
										BYTE1(v487) += BYTE2(v1135) + v180;
										v512 = v510 - 1;
										if (!v512)
											break;
										BYTE1(v486) = *(x_BYTE*)(v487 + v494);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v511);
										LOWORD(v511) = v1124 + v511;
										LOBYTE(v487) = BYTE2(v1124) + v180 + v487;
										if (BYTE1(v486))
										{
											LOBYTE(v486) = v489[9];
											v489[9] = m_ptrX_BYTE_F6EE0_tablesx[v486];
										}
										v180 = __CFADD__(v1171, v511);
										v513 = v1171 + v511;
										BYTE1(v487) += BYTE2(v1135) + v180;
										v514 = v512 - 1;
										if (!v514)
											break;
										BYTE1(v486) = *(x_BYTE*)(v487 + v494);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v513);
										LOWORD(v513) = v1124 + v513;
										LOBYTE(v487) = BYTE2(v1124) + v180 + v487;
										if (BYTE1(v486))
										{
											LOBYTE(v486) = v489[10];
											v489[10] = m_ptrX_BYTE_F6EE0_tablesx[v486];
										}
										v180 = __CFADD__(v1171, v513);
										v515 = v1171 + v513;
										BYTE1(v487) += BYTE2(v1135) + v180;
										v516 = v514 - 1;
										if (!v516)
											break;
										BYTE1(v486) = *(x_BYTE*)(v487 + v494);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v515);
										LOWORD(v515) = v1124 + v515;
										LOBYTE(v487) = BYTE2(v1124) + v180 + v487;
										if (BYTE1(v486))
										{
											LOBYTE(v486) = v489[11];
											v489[11] = m_ptrX_BYTE_F6EE0_tablesx[v486];
										}
										v180 = __CFADD__(v1171, v515);
										v517 = v1171 + v515;
										BYTE1(v487) += BYTE2(v1135) + v180;
										v518 = v516 - 1;
										if (!v518)
											break;
										BYTE1(v486) = *(x_BYTE*)(v487 + v494);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v517);
										LOWORD(v517) = v1124 + v517;
										LOBYTE(v487) = BYTE2(v1124) + v180 + v487;
										if (BYTE1(v486))
										{
											LOBYTE(v486) = v489[12];
											v489[12] = m_ptrX_BYTE_F6EE0_tablesx[v486];
										}
										v180 = __CFADD__(v1171, v517);
										v519 = v1171 + v517;
										BYTE1(v487) += BYTE2(v1135) + v180;
										v520 = v518 - 1;
										if (!v520)
											break;
										BYTE1(v486) = *(x_BYTE*)(v487 + v494);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v519);
										LOWORD(v519) = v1124 + v519;
										LOBYTE(v487) = BYTE2(v1124) + v180 + v487;
										if (BYTE1(v486))
										{
											LOBYTE(v486) = v489[13];
											v489[13] = m_ptrX_BYTE_F6EE0_tablesx[v486];
										}
										v180 = __CFADD__(v1171, v519);
										v521 = v1171 + v519;
										BYTE1(v487) += BYTE2(v1135) + v180;
										v522 = v520 - 1;
										if (!v522)
											break;
										BYTE1(v486) = *(x_BYTE*)(v487 + v494);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v521);
										LOWORD(v521) = v1124 + v521;
										LOBYTE(v487) = BYTE2(v1124) + v180 + v487;
										if (BYTE1(v486))
										{
											LOBYTE(v486) = v489[14];
											v489[14] = m_ptrX_BYTE_F6EE0_tablesx[v486];
										}
										v180 = __CFADD__(v1171, v521);
										v523 = v1171 + v521;
										BYTE1(v487) += BYTE2(v1135) + v180;
										v524 = v522 - 1;
										if (!v524)
											break;
										BYTE1(v486) = *(x_BYTE*)(v487 + v494);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v523);
										LOWORD(v523) = v1124 + v523;
										LOBYTE(v487) = BYTE2(v1124) + v180 + v487;
										if (BYTE1(v486))
										{
											LOBYTE(v486) = v489[15];
											v489[15] = m_ptrX_BYTE_F6EE0_tablesx[v486];
										}
										v180 = __CFADD__(v1171, v523);
										v492 = v1171 + v523;
										BYTE1(v487) += BYTE2(v1135) + v180;
										LOWORD(v488) = v524 - 1;
										if (!(x_WORD)v488)
											break;
										v489 += 16;
									}
									v485 = v1280;
								}
							}
						LABEL_728:
							v485 += 10;
							if (!--triLn_v1123)
								return;
						}
						if (v488 > viewPortWidth)
							LOWORD(v488) = viewPortWidth;
						v18 = __OFSUB__((x_WORD)v488, (x_WORD)v486);
						LOWORD(v488) = v488 - v486;
						if ((unsigned __int8)(((v488 & 0x8000u) != 0) ^ v18) | ((x_WORD)v488 == 0))
							goto LABEL_728;
						v489 += v486;
						v492 = __ROL4__(*((x_DWORD*)v485 + 3), 16);
						BYTE1(v487) = v492;
						LOWORD(v492) = v485[4];
						LOBYTE(v487) = *((x_BYTE*)v485 + 10);
						goto LABEL_677;
					case 0xC:
						v525 = (unsigned __int16*)unk_DE56Cx;
						v1172 = v1135 << 16;
						HIWORD(v526) = 0;
						HIWORD(v527) = 0;
						while (1)
						{
							LOWORD(v526) = v525[1];
							v528 = v525[3];
							v529 = (x_BYTE*)(pitch + v1102);
							v1102 += pitch;
							line11++;
							if (line11 >= drawEveryNthLine)
							{
								line11 = 0;
								if ((v526 & 0x8000u) == 0)
									break;
								if ((signed __int16)v528 > 0)
								{
									v530 = (unsigned __int16)-(signed __int16)v526;
									v532 = __ROL4__(*((x_DWORD*)v525 + 3) + v1135 * v530, 16);
									BYTE1(v527) = v532;
									v531 = *((x_DWORD*)v525 + 2) + v1124 * v530;
									LOWORD(v532) = v531;
									v533 = v531 >> 8;
									LOBYTE(v527) = BYTE1(v533);
									if (v528 > viewPortWidth)
										LOWORD(v528) = viewPortWidth;
									v526 = (unsigned __int16)v533;
								LABEL_740:
									v1281 = v525;
									v534 = pTexture;
									LOBYTE(v526) = x_BYTE_E126C;
									while (1)
									{
										BYTE1(v526) = *(x_BYTE*)(v527 + v534);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v532);
										LOWORD(v532) = v1124 + v532;
										LOBYTE(v527) = BYTE2(v1124) + v180 + v527;
										v180 = __CFADD__(v1172, v532);
										v535 = v1172 + v532;
										*v529 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v526];
										BYTE1(v527) += BYTE2(v1135) + v180;
										v536 = v528 - 1;
										if (!v536)
											break;
										BYTE1(v526) = *(x_BYTE*)(v527 + v534);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v535);
										LOWORD(v535) = v1124 + v535;
										LOBYTE(v527) = BYTE2(v1124) + v180 + v527;
										v180 = __CFADD__(v1172, v535);
										v537 = v1172 + v535;
										v529[1] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v526];
										BYTE1(v527) += BYTE2(v1135) + v180;
										v538 = v536 - 1;
										if (!v538)
											break;
										BYTE1(v526) = *(x_BYTE*)(v527 + v534);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v537);
										LOWORD(v537) = v1124 + v537;
										LOBYTE(v527) = BYTE2(v1124) + v180 + v527;
										v180 = __CFADD__(v1172, v537);
										v539 = v1172 + v537;
										v529[2] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v526];
										BYTE1(v527) += BYTE2(v1135) + v180;
										v540 = v538 - 1;
										if (!v540)
											break;
										BYTE1(v526) = *(x_BYTE*)(v527 + v534);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v539);
										LOWORD(v539) = v1124 + v539;
										LOBYTE(v527) = BYTE2(v1124) + v180 + v527;
										v180 = __CFADD__(v1172, v539);
										v541 = v1172 + v539;
										v529[3] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v526];
										BYTE1(v527) += BYTE2(v1135) + v180;
										v542 = v540 - 1;
										if (!v542)
											break;
										BYTE1(v526) = *(x_BYTE*)(v527 + v534);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v541);
										LOWORD(v541) = v1124 + v541;
										LOBYTE(v527) = BYTE2(v1124) + v180 + v527;
										v180 = __CFADD__(v1172, v541);
										v543 = v1172 + v541;
										v529[4] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v526];
										BYTE1(v527) += BYTE2(v1135) + v180;
										v544 = v542 - 1;
										if (!v544)
											break;
										BYTE1(v526) = *(x_BYTE*)(v527 + v534);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v543);
										LOWORD(v543) = v1124 + v543;
										LOBYTE(v527) = BYTE2(v1124) + v180 + v527;
										v180 = __CFADD__(v1172, v543);
										v545 = v1172 + v543;
										v529[5] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v526];
										BYTE1(v527) += BYTE2(v1135) + v180;
										v546 = v544 - 1;
										if (!v546)
											break;
										BYTE1(v526) = *(x_BYTE*)(v527 + v534);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v545);
										LOWORD(v545) = v1124 + v545;
										LOBYTE(v527) = BYTE2(v1124) + v180 + v527;
										v180 = __CFADD__(v1172, v545);
										v547 = v1172 + v545;
										v529[6] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v526];
										BYTE1(v527) += BYTE2(v1135) + v180;
										v548 = v546 - 1;
										if (!v548)
											break;
										BYTE1(v526) = *(x_BYTE*)(v527 + v534);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v547);
										LOWORD(v547) = v1124 + v547;
										LOBYTE(v527) = BYTE2(v1124) + v180 + v527;
										v180 = __CFADD__(v1172, v547);
										v549 = v1172 + v547;
										v529[7] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v526];
										BYTE1(v527) += BYTE2(v1135) + v180;
										v550 = v548 - 1;
										if (!v550)
											break;
										BYTE1(v526) = *(x_BYTE*)(v527 + v534);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v549);
										LOWORD(v549) = v1124 + v549;
										LOBYTE(v527) = BYTE2(v1124) + v180 + v527;
										v180 = __CFADD__(v1172, v549);
										v551 = v1172 + v549;
										v529[8] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v526];
										BYTE1(v527) += BYTE2(v1135) + v180;
										v552 = v550 - 1;
										if (!v552)
											break;
										BYTE1(v526) = *(x_BYTE*)(v527 + v534);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v551);
										LOWORD(v551) = v1124 + v551;
										LOBYTE(v527) = BYTE2(v1124) + v180 + v527;
										v180 = __CFADD__(v1172, v551);
										v553 = v1172 + v551;
										v529[9] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v526];
										BYTE1(v527) += BYTE2(v1135) + v180;
										v554 = v552 - 1;
										if (!v554)
											break;
										BYTE1(v526) = *(x_BYTE*)(v527 + v534);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v553);
										LOWORD(v553) = v1124 + v553;
										LOBYTE(v527) = BYTE2(v1124) + v180 + v527;
										v180 = __CFADD__(v1172, v553);
										v555 = v1172 + v553;
										v529[10] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v526];
										BYTE1(v527) += BYTE2(v1135) + v180;
										v556 = v554 - 1;
										if (!v556)
											break;
										BYTE1(v526) = *(x_BYTE*)(v527 + v534);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v555);
										LOWORD(v555) = v1124 + v555;
										LOBYTE(v527) = BYTE2(v1124) + v180 + v527;
										v180 = __CFADD__(v1172, v555);
										v557 = v1172 + v555;
										v529[11] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v526];
										BYTE1(v527) += BYTE2(v1135) + v180;
										v558 = v556 - 1;
										if (!v558)
											break;
										BYTE1(v526) = *(x_BYTE*)(v527 + v534);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v557);
										LOWORD(v557) = v1124 + v557;
										LOBYTE(v527) = BYTE2(v1124) + v180 + v527;
										v180 = __CFADD__(v1172, v557);
										v559 = v1172 + v557;
										v529[12] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v526];
										BYTE1(v527) += BYTE2(v1135) + v180;
										v560 = v558 - 1;
										if (!v560)
											break;
										BYTE1(v526) = *(x_BYTE*)(v527 + v534);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v559);
										LOWORD(v559) = v1124 + v559;
										LOBYTE(v527) = BYTE2(v1124) + v180 + v527;
										v180 = __CFADD__(v1172, v559);
										v561 = v1172 + v559;
										v529[13] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v526];
										BYTE1(v527) += BYTE2(v1135) + v180;
										v562 = v560 - 1;
										if (!v562)
											break;
										BYTE1(v526) = *(x_BYTE*)(v527 + v534);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v561);
										LOWORD(v561) = v1124 + v561;
										LOBYTE(v527) = BYTE2(v1124) + v180 + v527;
										v180 = __CFADD__(v1172, v561);
										v563 = v1172 + v561;
										v529[14] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v526];
										BYTE1(v527) += BYTE2(v1135) + v180;
										v564 = v562 - 1;
										if (!v564)
											break;
										BYTE1(v526) = *(x_BYTE*)(v527 + v534);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v563);
										LOWORD(v563) = v1124 + v563;
										LOBYTE(v527) = BYTE2(v1124) + v180 + v527;
										v180 = __CFADD__(v1172, v563);
										v532 = v1172 + v563;
										v529[15] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v526];
										BYTE1(v527) += BYTE2(v1135) + v180;
										LOWORD(v528) = v564 - 1;
										if (!(x_WORD)v528)
											break;
										v529 += 16;
									}
									v525 = v1281;
								}
							}
						LABEL_759:
							v525 += 10;
							if (!--triLn_v1123)
								return;
						}
						if (v528 > viewPortWidth)
							LOWORD(v528) = viewPortWidth;
						v18 = __OFSUB__((x_WORD)v528, (x_WORD)v526);
						LOWORD(v528) = v528 - v526;
						if ((unsigned __int8)(((v528 & 0x8000u) != 0) ^ v18) | ((x_WORD)v528 == 0))
							goto LABEL_759;
						v529 += v526;
						v532 = __ROL4__(*((x_DWORD*)v525 + 3), 16);
						BYTE1(v527) = v532;
						LOWORD(v532) = v525[4];
						LOBYTE(v527) = *((x_BYTE*)v525 + 10);
						goto LABEL_740;
					case 0xD:
						v565 = (unsigned __int16*)unk_DE56Cx;
						v1173 = v1135 << 16;
						HIWORD(v566) = 0;
						HIWORD(v567) = 0;
						while (1)
						{
							LOWORD(v566) = v565[1];
							v568 = v565[3];
							v569 = (x_BYTE*)(pitch + v1102);
							v1102 += pitch;
							line12++;
							if (line12 >= drawEveryNthLine)
							{
								line12 = 0;
								if ((v566 & 0x8000u) == 0)
									break;
								if ((signed __int16)v568 > 0)
								{
									v570 = (unsigned __int16)-(signed __int16)v566;
									v572 = __ROL4__(*((x_DWORD*)v565 + 3) + v1135 * v570, 16);
									BYTE1(v567) = v572;
									v571 = *((x_DWORD*)v565 + 2) + v1124 * v570;
									LOWORD(v572) = v571;
									v573 = v571 >> 8;
									LOBYTE(v567) = BYTE1(v573);
									if (v568 > viewPortWidth)
										LOWORD(v568) = viewPortWidth;
									v566 = (unsigned __int16)v573;
								LABEL_771:
									v1282 = v565;
									v574 = pTexture;
									BYTE1(v566) = x_BYTE_E126C;
									while (1)
									{
										LOBYTE(v566) = *(x_BYTE*)(v567 + v574);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v572);
										LOWORD(v572) = v1124 + v572;
										LOBYTE(v567) = BYTE2(v1124) + v180 + v567;
										v180 = __CFADD__(v1173, v572);
										v575 = v1173 + v572;
										*v569 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v566];
										BYTE1(v567) += BYTE2(v1135) + v180;
										v576 = v568 - 1;
										if (!v576)
											break;
										LOBYTE(v566) = *(x_BYTE*)(v567 + v574);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v575);
										LOWORD(v575) = v1124 + v575;
										LOBYTE(v567) = BYTE2(v1124) + v180 + v567;
										v180 = __CFADD__(v1173, v575);
										v577 = v1173 + v575;
										v569[1] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v566];
										BYTE1(v567) += BYTE2(v1135) + v180;
										v578 = v576 - 1;
										if (!v578)
											break;
										LOBYTE(v566) = *(x_BYTE*)(v567 + v574);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v577);
										LOWORD(v577) = v1124 + v577;
										LOBYTE(v567) = BYTE2(v1124) + v180 + v567;
										v180 = __CFADD__(v1173, v577);
										v579 = v1173 + v577;
										v569[2] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v566];
										BYTE1(v567) += BYTE2(v1135) + v180;
										v580 = v578 - 1;
										if (!v580)
											break;
										LOBYTE(v566) = *(x_BYTE*)(v567 + v574);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v579);
										LOWORD(v579) = v1124 + v579;
										LOBYTE(v567) = BYTE2(v1124) + v180 + v567;
										v180 = __CFADD__(v1173, v579);
										v581 = v1173 + v579;
										v569[3] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v566];
										BYTE1(v567) += BYTE2(v1135) + v180;
										v582 = v580 - 1;
										if (!v582)
											break;
										LOBYTE(v566) = *(x_BYTE*)(v567 + v574);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v581);
										LOWORD(v581) = v1124 + v581;
										LOBYTE(v567) = BYTE2(v1124) + v180 + v567;
										v180 = __CFADD__(v1173, v581);
										v583 = v1173 + v581;
										v569[4] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v566];
										BYTE1(v567) += BYTE2(v1135) + v180;
										v584 = v582 - 1;
										if (!v584)
											break;
										LOBYTE(v566) = *(x_BYTE*)(v567 + v574);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v583);
										LOWORD(v583) = v1124 + v583;
										LOBYTE(v567) = BYTE2(v1124) + v180 + v567;
										v180 = __CFADD__(v1173, v583);
										v585 = v1173 + v583;
										v569[5] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v566];
										BYTE1(v567) += BYTE2(v1135) + v180;
										v586 = v584 - 1;
										if (!v586)
											break;
										LOBYTE(v566) = *(x_BYTE*)(v567 + v574);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v585);
										LOWORD(v585) = v1124 + v585;
										LOBYTE(v567) = BYTE2(v1124) + v180 + v567;
										v180 = __CFADD__(v1173, v585);
										v587 = v1173 + v585;
										v569[6] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v566];
										BYTE1(v567) += BYTE2(v1135) + v180;
										v588 = v586 - 1;
										if (!v588)
											break;
										LOBYTE(v566) = *(x_BYTE*)(v567 + v574);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v587);
										LOWORD(v587) = v1124 + v587;
										LOBYTE(v567) = BYTE2(v1124) + v180 + v567;
										v180 = __CFADD__(v1173, v587);
										v589 = v1173 + v587;
										v569[7] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v566];
										BYTE1(v567) += BYTE2(v1135) + v180;
										v590 = v588 - 1;
										if (!v590)
											break;
										LOBYTE(v566) = *(x_BYTE*)(v567 + v574);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v589);
										LOWORD(v589) = v1124 + v589;
										LOBYTE(v567) = BYTE2(v1124) + v180 + v567;
										v180 = __CFADD__(v1173, v589);
										v591 = v1173 + v589;
										v569[8] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v566];
										BYTE1(v567) += BYTE2(v1135) + v180;
										v592 = v590 - 1;
										if (!v592)
											break;
										LOBYTE(v566) = *(x_BYTE*)(v567 + v574);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v591);
										LOWORD(v591) = v1124 + v591;
										LOBYTE(v567) = BYTE2(v1124) + v180 + v567;
										v180 = __CFADD__(v1173, v591);
										v593 = v1173 + v591;
										v569[9] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v566];
										BYTE1(v567) += BYTE2(v1135) + v180;
										v594 = v592 - 1;
										if (!v594)
											break;
										LOBYTE(v566) = *(x_BYTE*)(v567 + v574);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v593);
										LOWORD(v593) = v1124 + v593;
										LOBYTE(v567) = BYTE2(v1124) + v180 + v567;
										v180 = __CFADD__(v1173, v593);
										v595 = v1173 + v593;
										v569[10] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v566];
										BYTE1(v567) += BYTE2(v1135) + v180;
										v596 = v594 - 1;
										if (!v596)
											break;
										LOBYTE(v566) = *(x_BYTE*)(v567 + v574);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v595);
										LOWORD(v595) = v1124 + v595;
										LOBYTE(v567) = BYTE2(v1124) + v180 + v567;
										v180 = __CFADD__(v1173, v595);
										v597 = v1173 + v595;
										v569[11] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v566];
										BYTE1(v567) += BYTE2(v1135) + v180;
										v598 = v596 - 1;
										if (!v598)
											break;
										LOBYTE(v566) = *(x_BYTE*)(v567 + v574);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v597);
										LOWORD(v597) = v1124 + v597;
										LOBYTE(v567) = BYTE2(v1124) + v180 + v567;
										v180 = __CFADD__(v1173, v597);
										v599 = v1173 + v597;
										v569[12] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v566];
										BYTE1(v567) += BYTE2(v1135) + v180;
										v600 = v598 - 1;
										if (!v600)
											break;
										LOBYTE(v566) = *(x_BYTE*)(v567 + v574);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v599);
										LOWORD(v599) = v1124 + v599;
										LOBYTE(v567) = BYTE2(v1124) + v180 + v567;
										v180 = __CFADD__(v1173, v599);
										v601 = v1173 + v599;
										v569[13] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v566];
										BYTE1(v567) += BYTE2(v1135) + v180;
										v602 = v600 - 1;
										if (!v602)
											break;
										LOBYTE(v566) = *(x_BYTE*)(v567 + v574);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v601);
										LOWORD(v601) = v1124 + v601;
										LOBYTE(v567) = BYTE2(v1124) + v180 + v567;
										v180 = __CFADD__(v1173, v601);
										v603 = v1173 + v601;
										v569[14] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v566];
										BYTE1(v567) += BYTE2(v1135) + v180;
										v604 = v602 - 1;
										if (!v604)
											break;
										LOBYTE(v566) = *(x_BYTE*)(v567 + v574);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v603);
										LOWORD(v603) = v1124 + v603;
										LOBYTE(v567) = BYTE2(v1124) + v180 + v567;
										v180 = __CFADD__(v1173, v603);
										v572 = v1173 + v603;
										v569[15] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v566];
										BYTE1(v567) += BYTE2(v1135) + v180;
										LOWORD(v568) = v604 - 1;
										if (!(x_WORD)v568)
											break;
										v569 += 16;
									}
									v565 = v1282;
								}
							}
						LABEL_790:
							v565 += 10;
							if (!--triLn_v1123)
								return;
						}
						if (v568 > viewPortWidth)
							LOWORD(v568) = viewPortWidth;
						v18 = __OFSUB__((x_WORD)v568, (x_WORD)v566);
						LOWORD(v568) = v568 - v566;
						if ((unsigned __int8)(((v568 & 0x8000u) != 0) ^ v18) | ((x_WORD)v568 == 0))
							goto LABEL_790;
						v569 += v566;
						v572 = __ROL4__(*((x_DWORD*)v565 + 3), 16);
						BYTE1(v567) = v572;
						LOWORD(v572) = v565[4];
						LOBYTE(v567) = *((x_BYTE*)v565 + 10);
						goto LABEL_771;
					case 0xE:
						v605 = (unsigned __int16*)unk_DE56Cx;
						v606 = (x_BYTE*)v1102;
						HIWORD(v607) = 0;
						BYTE1(v607) = x_BYTE_E126C;
						HIWORD(v608) = 0;
						while (1)
						{
							LOWORD(v608) = v605[1];
							v609 = v605[3];
							v606 += pitch;
							line13++;
							if (line13 >= drawEveryNthLine)
							{
								line13 = 0;
								if ((v608 & 0x8000u) == 0)
									break;
								if ((signed __int16)v609 > 0)
								{
									if (v609 > viewPortWidth)
										LOWORD(v609) = viewPortWidth;
								LABEL_802:
									for (i = v606; ; i += 16)
									{
										LOBYTE(v607) = *i;
										*i = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v607];
										v611 = v609 - 1;
										if (!v611)
											break;
										LOBYTE(v607) = i[1];
										i[1] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v607];
										v612 = v611 - 1;
										if (!v612)
											break;
										LOBYTE(v607) = i[2];
										i[2] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v607];
										v613 = v612 - 1;
										if (!v613)
											break;
										LOBYTE(v607) = i[3];
										i[3] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v607];
										v614 = v613 - 1;
										if (!v614)
											break;
										LOBYTE(v607) = i[4];
										i[4] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v607];
										v615 = v614 - 1;
										if (!v615)
											break;
										LOBYTE(v607) = i[5];
										i[5] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v607];
										v616 = v615 - 1;
										if (!v616)
											break;
										LOBYTE(v607) = i[6];
										i[6] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v607];
										v617 = v616 - 1;
										if (!v617)
											break;
										LOBYTE(v607) = i[7];
										i[7] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v607];
										v618 = v617 - 1;
										if (!v618)
											break;
										LOBYTE(v607) = i[8];
										i[8] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v607];
										v619 = v618 - 1;
										if (!v619)
											break;
										LOBYTE(v607) = i[9];
										i[9] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v607];
										v620 = v619 - 1;
										if (!v620)
											break;
										LOBYTE(v607) = i[10];
										i[10] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v607];
										v621 = v620 - 1;
										if (!v621)
											break;
										LOBYTE(v607) = i[11];
										i[11] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v607];
										v622 = v621 - 1;
										if (!v622)
											break;
										LOBYTE(v607) = i[12];
										i[12] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v607];
										v623 = v622 - 1;
										if (!v623)
											break;
										LOBYTE(v607) = i[13];
										i[13] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v607];
										v624 = v623 - 1;
										if (!v624)
											break;
										LOBYTE(v607) = i[14];
										i[14] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v607];
										v625 = v624 - 1;
										if (!v625)
											break;
										LOBYTE(v607) = i[15];
										i[15] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v607];
										LOWORD(v609) = v625 - 1;
										if (!(x_WORD)v609)
											break;
									}
								}
							}
						LABEL_819:
							v605 += 10;
							if (!--triLn_v1123)
								return;
						}
						if (v609 > viewPortWidth)
							LOWORD(v609) = viewPortWidth;
						v18 = __OFSUB__((x_WORD)v609, (x_WORD)v608);
						LOWORD(v609) = v609 - v608;
						if ((unsigned __int8)(((v609 & 0x8000u) != 0) ^ v18) | ((x_WORD)v609 == 0))
							goto LABEL_819;
						i = &v606[v608];
						goto LABEL_802;
					case 0xF:
						v626 = (unsigned __int16*)unk_DE56Cx;
						v627 = (x_BYTE*)v1102;
						v628 = (unsigned __int8)x_BYTE_E126C;
						HIWORD(v629) = 0;
						while (1)
						{
							LOWORD(v629) = v626[1];
							v630 = v626[3];
							v627 += pitch;
							line14++;
							if (line14 >= drawEveryNthLine)
							{
								line14 = 0;
								if ((v629 & 0x8000u) == 0)
									break;
								if ((signed __int16)v630 > 0)
								{
									if (v630 > viewPortWidth)
										LOWORD(v630) = viewPortWidth;
								LABEL_831:
									for (j = v627; ; j += 16)
									{
										BYTE1(v628) = *j;
										*j = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v628];
										v632 = v630 - 1;
										if (!v632)
											break;
										BYTE1(v628) = j[1];
										j[1] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v628];
										v633 = v632 - 1;
										if (!v633)
											break;
										BYTE1(v628) = j[2];
										j[2] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v628];
										v634 = v633 - 1;
										if (!v634)
											break;
										BYTE1(v628) = j[3];
										j[3] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v628];
										v635 = v634 - 1;
										if (!v635)
											break;
										BYTE1(v628) = j[4];
										j[4] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v628];
										v636 = v635 - 1;
										if (!v636)
											break;
										BYTE1(v628) = j[5];
										j[5] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v628];
										v637 = v636 - 1;
										if (!v637)
											break;
										BYTE1(v628) = j[6];
										j[6] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v628];
										v638 = v637 - 1;
										if (!v638)
											break;
										BYTE1(v628) = j[7];
										j[7] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v628];
										v639 = v638 - 1;
										if (!v639)
											break;
										BYTE1(v628) = j[8];
										j[8] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v628];
										v640 = v639 - 1;
										if (!v640)
											break;
										BYTE1(v628) = j[9];
										j[9] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v628];
										v641 = v640 - 1;
										if (!v641)
											break;
										BYTE1(v628) = j[10];
										j[10] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v628];
										v642 = v641 - 1;
										if (!v642)
											break;
										BYTE1(v628) = j[11];
										j[11] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v628];
										v643 = v642 - 1;
										if (!v643)
											break;
										BYTE1(v628) = j[12];
										j[12] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v628];
										v644 = v643 - 1;
										if (!v644)
											break;
										BYTE1(v628) = j[13];
										j[13] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v628];
										v645 = v644 - 1;
										if (!v645)
											break;
										BYTE1(v628) = j[14];
										j[14] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v628];
										v646 = v645 - 1;
										if (!v646)
											break;
										BYTE1(v628) = j[15];
										j[15] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v628];
										LOWORD(v630) = v646 - 1;
										if (!(x_WORD)v630)
											break;
									}
								}
							}
						LABEL_848:
							v626 += 10;
							if (!--triLn_v1123)
								return;
						}
						if (v630 > viewPortWidth)
							LOWORD(v630) = viewPortWidth;
						v18 = __OFSUB__((x_WORD)v630, (x_WORD)v629);
						LOWORD(v630) = v630 - v629;
						if ((unsigned __int8)(((v630 & 0x8000u) != 0) ^ v18) | ((x_WORD)v630 == 0))
							goto LABEL_848;
						j = &v627[v629];
						goto LABEL_831;
					case 0x10:
						v647 = (unsigned __int16*)unk_DE56Cx;
						HIWORD(v648) = 0;
						while (1)
						{
							LOWORD(v31) = v647[1];
							v649 = v647[3];
							v650 = (x_BYTE*)(pitch + v1102);
							v1102 += pitch;
							line15++;
							if (line15 >= drawEveryNthLine)
							{
								line15 = 0;
								if ((v31 & 0x8000u) == 0)
									break;
								if ((signed __int16)v649 > 0)
								{
									v651 = v1146 * (unsigned __int16)-(signed __int16)v31;
									v652 = v651;
									v31 = v651 >> 8;
									v180 = __CFADD__(v647[8], v652);
									v653 = v647[8] + v652;
									BYTE1(v31) += *((x_BYTE*)v647 + 18) + v180;
									if (v649 > viewPortWidth)
										LOWORD(v649) = viewPortWidth;
									v31 = (unsigned __int16)v31;
									LOBYTE(v31) = x_BYTE_E126C;
								LABEL_860:
									while (1)
									{
										BYTE1(v648) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										LOBYTE(v648) = *v650;
										*v650 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v648];
										v180 = __CFADD__((x_WORD)v1146, v653);
										v654 = v1146 + v653;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v655 = v649 - 1;
										if (!v655)
											break;
										BYTE1(v648) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										LOBYTE(v648) = v650[1];
										v650[1] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v648];
										v180 = __CFADD__((x_WORD)v1146, v654);
										v656 = v1146 + v654;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v657 = v655 - 1;
										if (!v657)
											break;
										BYTE1(v648) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										LOBYTE(v648) = v650[2];
										v650[2] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v648];
										v180 = __CFADD__((x_WORD)v1146, v656);
										v658 = v1146 + v656;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v659 = v657 - 1;
										if (!v659)
											break;
										BYTE1(v648) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										LOBYTE(v648) = v650[3];
										v650[3] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v648];
										v180 = __CFADD__((x_WORD)v1146, v658);
										v660 = v1146 + v658;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v661 = v659 - 1;
										if (!v661)
											break;
										BYTE1(v648) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										LOBYTE(v648) = v650[4];
										v650[4] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v648];
										v180 = __CFADD__((x_WORD)v1146, v660);
										v662 = v1146 + v660;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v663 = v661 - 1;
										if (!v663)
											break;
										BYTE1(v648) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										LOBYTE(v648) = v650[5];
										v650[5] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v648];
										v180 = __CFADD__((x_WORD)v1146, v662);
										v664 = v1146 + v662;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v665 = v663 - 1;
										if (!v665)
											break;
										BYTE1(v648) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										LOBYTE(v648) = v650[6];
										v650[6] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v648];
										v180 = __CFADD__((x_WORD)v1146, v664);
										v666 = v1146 + v664;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v667 = v665 - 1;
										if (!v667)
											break;
										BYTE1(v648) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										LOBYTE(v648) = v650[7];
										v650[7] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v648];
										v180 = __CFADD__((x_WORD)v1146, v666);
										v668 = v1146 + v666;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v669 = v667 - 1;
										if (!v669)
											break;
										BYTE1(v648) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										LOBYTE(v648) = v650[8];
										v650[8] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v648];
										v180 = __CFADD__((x_WORD)v1146, v668);
										v670 = v1146 + v668;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v671 = v669 - 1;
										if (!v671)
											break;
										BYTE1(v648) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										LOBYTE(v648) = v650[9];
										v650[9] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v648];
										v180 = __CFADD__((x_WORD)v1146, v670);
										v672 = v1146 + v670;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v673 = v671 - 1;
										if (!v673)
											break;
										BYTE1(v648) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										LOBYTE(v648) = v650[10];
										v650[10] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v648];
										v180 = __CFADD__((x_WORD)v1146, v672);
										v674 = v1146 + v672;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v675 = v673 - 1;
										if (!v675)
											break;
										BYTE1(v648) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										LOBYTE(v648) = v650[11];
										v650[11] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v648];
										v180 = __CFADD__((x_WORD)v1146, v674);
										v676 = v1146 + v674;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v677 = v675 - 1;
										if (!v677)
											break;
										BYTE1(v648) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										LOBYTE(v648) = v650[12];
										v650[12] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v648];
										v180 = __CFADD__((x_WORD)v1146, v676);
										v678 = v1146 + v676;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v679 = v677 - 1;
										if (!v679)
											break;
										BYTE1(v648) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										LOBYTE(v648) = v650[13];
										v650[13] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v648];
										v180 = __CFADD__((x_WORD)v1146, v678);
										v680 = v1146 + v678;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v681 = v679 - 1;
										if (!v681)
											break;
										BYTE1(v648) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										LOBYTE(v648) = v650[14];
										v650[14] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v648];
										v180 = __CFADD__((x_WORD)v1146, v680);
										v682 = v1146 + v680;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v683 = v681 - 1;
										if (!v683)
											break;
										BYTE1(v648) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										LOBYTE(v648) = v650[15];
										v650[15] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v648];
										v180 = __CFADD__((x_WORD)v1146, v682);
										v653 = v1146 + v682;
										BYTE1(v31) += BYTE2(v1146) + v180;
										LOWORD(v649) = v683 - 1;
										if (!(x_WORD)v649)
											break;
										v650 += 16;
									}
								}
							}
						LABEL_877:
							v647 += 10;
							if (!--triLn_v1123)
								return;
						}
						if (v649 > viewPortWidth)
							LOWORD(v649) = viewPortWidth;
						v18 = __OFSUB__((x_WORD)v649, (x_WORD)v31);
						LOWORD(v649) = v649 - v31;
						if ((unsigned __int8)(((v649 & 0x8000u) != 0) ^ v18) | ((x_WORD)v649 == 0))
							goto LABEL_877;
						v650 += v31;
						v31 = (unsigned __int8)x_BYTE_E126C;
						v653 = v647[8];
						BYTE1(v31) = *((x_BYTE*)v647 + 18);
						goto LABEL_860;
					case 0x11:
						v684 = (unsigned __int16*)unk_DE56Cx;
						HIWORD(v685) = 0;
						while (1)
						{
							LOWORD(v31) = v684[1];
							v686 = v684[3];
							v687 = (x_BYTE*)(pitch + v1102);
							v1102 += pitch;
							line16++;
							if (line16 >= drawEveryNthLine)
							{
								line16 = 0;
								if ((v31 & 0x8000u) == 0)
									break;
								if ((signed __int16)v686 > 0)
								{
									v688 = v1146 * (unsigned __int16)-(signed __int16)v31;
									v689 = v688;
									v31 = v688 >> 8;
									v180 = __CFADD__(v684[8], v689);
									v690 = v684[8] + v689;
									BYTE1(v31) += *((x_BYTE*)v684 + 18) + v180;
									if (v686 > viewPortWidth)
										LOWORD(v686) = viewPortWidth;
									v31 = (unsigned __int16)v31;
									LOBYTE(v31) = x_BYTE_E126C;
								LABEL_889:
									while (1)
									{
										LOBYTE(v685) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										BYTE1(v685) = *v687;
										*v687 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v685];
										v180 = __CFADD__((x_WORD)v1146, v690);
										v691 = v1146 + v690;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v692 = v686 - 1;
										if (!v692)
											break;
										LOBYTE(v685) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										BYTE1(v685) = v687[1];
										v687[1] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v685];
										v180 = __CFADD__((x_WORD)v1146, v691);
										v693 = v1146 + v691;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v694 = v692 - 1;
										if (!v694)
											break;
										LOBYTE(v685) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										BYTE1(v685) = v687[2];
										v687[2] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v685];
										v180 = __CFADD__((x_WORD)v1146, v693);
										v695 = v1146 + v693;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v696 = v694 - 1;
										if (!v696)
											break;
										LOBYTE(v685) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										BYTE1(v685) = v687[3];
										v687[3] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v685];
										v180 = __CFADD__((x_WORD)v1146, v695);
										v697 = v1146 + v695;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v698 = v696 - 1;
										if (!v698)
											break;
										LOBYTE(v685) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										BYTE1(v685) = v687[4];
										v687[4] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v685];
										v180 = __CFADD__((x_WORD)v1146, v697);
										v699 = v1146 + v697;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v700 = v698 - 1;
										if (!v700)
											break;
										LOBYTE(v685) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										BYTE1(v685) = v687[5];
										v687[5] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v685];
										v180 = __CFADD__((x_WORD)v1146, v699);
										v701 = v1146 + v699;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v702 = v700 - 1;
										if (!v702)
											break;
										LOBYTE(v685) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										BYTE1(v685) = v687[6];
										v687[6] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v685];
										v180 = __CFADD__((x_WORD)v1146, v701);
										v703 = v1146 + v701;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v704 = v702 - 1;
										if (!v704)
											break;
										LOBYTE(v685) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										BYTE1(v685) = v687[7];
										v687[7] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v685];
										v180 = __CFADD__((x_WORD)v1146, v703);
										v705 = v1146 + v703;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v706 = v704 - 1;
										if (!v706)
											break;
										LOBYTE(v685) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										BYTE1(v685) = v687[8];
										v687[8] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v685];
										v180 = __CFADD__((x_WORD)v1146, v705);
										v707 = v1146 + v705;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v708 = v706 - 1;
										if (!v708)
											break;
										LOBYTE(v685) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										BYTE1(v685) = v687[9];
										v687[9] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v685];
										v180 = __CFADD__((x_WORD)v1146, v707);
										v709 = v1146 + v707;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v710 = v708 - 1;
										if (!v710)
											break;
										LOBYTE(v685) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										BYTE1(v685) = v687[10];
										v687[10] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v685];
										v180 = __CFADD__((x_WORD)v1146, v709);
										v711 = v1146 + v709;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v712 = v710 - 1;
										if (!v712)
											break;
										LOBYTE(v685) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										BYTE1(v685) = v687[11];
										v687[11] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v685];
										v180 = __CFADD__((x_WORD)v1146, v711);
										v713 = v1146 + v711;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v714 = v712 - 1;
										if (!v714)
											break;
										LOBYTE(v685) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										BYTE1(v685) = v687[12];
										v687[12] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v685];
										v180 = __CFADD__((x_WORD)v1146, v713);
										v715 = v1146 + v713;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v716 = v714 - 1;
										if (!v716)
											break;
										LOBYTE(v685) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										BYTE1(v685) = v687[13];
										v687[13] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v685];
										v180 = __CFADD__((x_WORD)v1146, v715);
										v717 = v1146 + v715;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v718 = v716 - 1;
										if (!v718)
											break;
										LOBYTE(v685) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										BYTE1(v685) = v687[14];
										v687[14] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v685];
										v180 = __CFADD__((x_WORD)v1146, v717);
										v719 = v1146 + v717;
										BYTE1(v31) += BYTE2(v1146) + v180;
										v720 = v718 - 1;
										if (!v720)
											break;
										LOBYTE(v685) = m_ptrX_BYTE_F6EE0_tablesx[v31];
										BYTE1(v685) = v687[15];
										v687[15] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v685];
										v180 = __CFADD__((x_WORD)v1146, v719);
										v690 = v1146 + v719;
										BYTE1(v31) += BYTE2(v1146) + v180;
										LOWORD(v686) = v720 - 1;
										if (!(x_WORD)v686)
											break;
										v687 += 16;
									}
								}
							}
						LABEL_906:
							v684 += 10;
							if (!--triLn_v1123)
								return;
						}
						if (v686 > viewPortWidth)
							LOWORD(v686) = viewPortWidth;
						v18 = __OFSUB__((x_WORD)v686, (x_WORD)v31);
						LOWORD(v686) = v686 - v31;
						if ((unsigned __int8)(((v686 & 0x8000u) != 0) ^ v18) | ((x_WORD)v686 == 0))
							goto LABEL_906;
						v687 += v31;
						v31 = (unsigned __int8)x_BYTE_E126C;
						v690 = v684[8];
						BYTE1(v31) = *((x_BYTE*)v684 + 18);
						goto LABEL_889;
					case 0x12:
						v721 = (unsigned __int16*)unk_DE56Cx;
						v1174 = v1135 << 16;
						HIWORD(v722) = 0;
						HIWORD(v723) = 0;
						while (1)
						{
							LOWORD(v722) = v721[1];
							v724 = v721[3];
							v725 = (x_BYTE*)(pitch + v1102);
							v1102 += pitch;
							line17++;
							if (line17 >= drawEveryNthLine)
							{
								line17 = 0;
								if ((v722 & 0x8000u) == 0)
									break;
								if ((signed __int16)v724 > 0)
								{
									v726 = (unsigned __int16)-(signed __int16)v722;
									v728 = __ROL4__(*((x_DWORD*)v721 + 3) + v1135 * v726, 16);
									BYTE1(v723) = v728;
									v727 = *((x_DWORD*)v721 + 2) + v1124 * v726;
									LOWORD(v728) = v727;
									v729 = v727 >> 8;
									LOBYTE(v723) = BYTE1(v729);
									if (v724 > viewPortWidth)
										LOWORD(v724) = viewPortWidth;
									v722 = (unsigned __int16)v729;
								LABEL_918:
									v1283 = v721;
									v730 = pTexture;
									while (1)
									{
										BYTE1(v722) = *(x_BYTE*)(v723 + v730);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v728);
										LOWORD(v728) = v1124 + v728;
										LOBYTE(v722) = *v725;
										LOBYTE(v723) = BYTE2(v1124) + v180 + v723;
										v180 = __CFADD__(v1174, v728);
										v731 = v1174 + v728;
										*v725 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v722];
										BYTE1(v723) += BYTE2(v1135) + v180;
										v732 = v724 - 1;
										if (!v732)
											break;
										BYTE1(v722) = *(x_BYTE*)(v723 + v730);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v731);
										LOWORD(v731) = v1124 + v731;
										LOBYTE(v722) = v725[1];
										LOBYTE(v723) = BYTE2(v1124) + v180 + v723;
										v180 = __CFADD__(v1174, v731);
										v733 = v1174 + v731;
										v725[1] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v722];
										BYTE1(v723) += BYTE2(v1135) + v180;
										v734 = v732 - 1;
										if (!v734)
											break;
										BYTE1(v722) = *(x_BYTE*)(v723 + v730);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v733);
										LOWORD(v733) = v1124 + v733;
										LOBYTE(v722) = v725[2];
										LOBYTE(v723) = BYTE2(v1124) + v180 + v723;
										v180 = __CFADD__(v1174, v733);
										v735 = v1174 + v733;
										v725[2] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v722];
										BYTE1(v723) += BYTE2(v1135) + v180;
										v736 = v734 - 1;
										if (!v736)
											break;
										BYTE1(v722) = *(x_BYTE*)(v723 + v730);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v735);
										LOWORD(v735) = v1124 + v735;
										LOBYTE(v722) = v725[3];
										LOBYTE(v723) = BYTE2(v1124) + v180 + v723;
										v180 = __CFADD__(v1174, v735);
										v737 = v1174 + v735;
										v725[3] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v722];
										BYTE1(v723) += BYTE2(v1135) + v180;
										v738 = v736 - 1;
										if (!v738)
											break;
										BYTE1(v722) = *(x_BYTE*)(v723 + v730);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v737);
										LOWORD(v737) = v1124 + v737;
										LOBYTE(v722) = v725[4];
										LOBYTE(v723) = BYTE2(v1124) + v180 + v723;
										v180 = __CFADD__(v1174, v737);
										v739 = v1174 + v737;
										v725[4] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v722];
										BYTE1(v723) += BYTE2(v1135) + v180;
										v740 = v738 - 1;
										if (!v740)
											break;
										BYTE1(v722) = *(x_BYTE*)(v723 + v730);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v739);
										LOWORD(v739) = v1124 + v739;
										LOBYTE(v722) = v725[5];
										LOBYTE(v723) = BYTE2(v1124) + v180 + v723;
										v180 = __CFADD__(v1174, v739);
										v741 = v1174 + v739;
										v725[5] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v722];
										BYTE1(v723) += BYTE2(v1135) + v180;
										v742 = v740 - 1;
										if (!v742)
											break;
										BYTE1(v722) = *(x_BYTE*)(v723 + v730);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v741);
										LOWORD(v741) = v1124 + v741;
										LOBYTE(v722) = v725[6];
										LOBYTE(v723) = BYTE2(v1124) + v180 + v723;
										v180 = __CFADD__(v1174, v741);
										v743 = v1174 + v741;
										v725[6] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v722];
										BYTE1(v723) += BYTE2(v1135) + v180;
										v744 = v742 - 1;
										if (!v744)
											break;
										BYTE1(v722) = *(x_BYTE*)(v723 + v730);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v743);
										LOWORD(v743) = v1124 + v743;
										LOBYTE(v722) = v725[7];
										LOBYTE(v723) = BYTE2(v1124) + v180 + v723;
										v180 = __CFADD__(v1174, v743);
										v745 = v1174 + v743;
										v725[7] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v722];
										BYTE1(v723) += BYTE2(v1135) + v180;
										v746 = v744 - 1;
										if (!v746)
											break;
										BYTE1(v722) = *(x_BYTE*)(v723 + v730);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v745);
										LOWORD(v745) = v1124 + v745;
										LOBYTE(v722) = v725[8];
										LOBYTE(v723) = BYTE2(v1124) + v180 + v723;
										v180 = __CFADD__(v1174, v745);
										v747 = v1174 + v745;
										v725[8] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v722];
										BYTE1(v723) += BYTE2(v1135) + v180;
										v748 = v746 - 1;
										if (!v748)
											break;
										BYTE1(v722) = *(x_BYTE*)(v723 + v730);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v747);
										LOWORD(v747) = v1124 + v747;
										LOBYTE(v722) = v725[9];
										LOBYTE(v723) = BYTE2(v1124) + v180 + v723;
										v180 = __CFADD__(v1174, v747);
										v749 = v1174 + v747;
										v725[9] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v722];
										BYTE1(v723) += BYTE2(v1135) + v180;
										v750 = v748 - 1;
										if (!v750)
											break;
										BYTE1(v722) = *(x_BYTE*)(v723 + v730);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v749);
										LOWORD(v749) = v1124 + v749;
										LOBYTE(v722) = v725[10];
										LOBYTE(v723) = BYTE2(v1124) + v180 + v723;
										v180 = __CFADD__(v1174, v749);
										v751 = v1174 + v749;
										v725[10] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v722];
										BYTE1(v723) += BYTE2(v1135) + v180;
										v752 = v750 - 1;
										if (!v752)
											break;
										BYTE1(v722) = *(x_BYTE*)(v723 + v730);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v751);
										LOWORD(v751) = v1124 + v751;
										LOBYTE(v722) = v725[11];
										LOBYTE(v723) = BYTE2(v1124) + v180 + v723;
										v180 = __CFADD__(v1174, v751);
										v753 = v1174 + v751;
										v725[11] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v722];
										BYTE1(v723) += BYTE2(v1135) + v180;
										v754 = v752 - 1;
										if (!v754)
											break;
										BYTE1(v722) = *(x_BYTE*)(v723 + v730);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v753);
										LOWORD(v753) = v1124 + v753;
										LOBYTE(v722) = v725[12];
										LOBYTE(v723) = BYTE2(v1124) + v180 + v723;
										v180 = __CFADD__(v1174, v753);
										v755 = v1174 + v753;
										v725[12] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v722];
										BYTE1(v723) += BYTE2(v1135) + v180;
										v756 = v754 - 1;
										if (!v756)
											break;
										BYTE1(v722) = *(x_BYTE*)(v723 + v730);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v755);
										LOWORD(v755) = v1124 + v755;
										LOBYTE(v722) = v725[13];
										LOBYTE(v723) = BYTE2(v1124) + v180 + v723;
										v180 = __CFADD__(v1174, v755);
										v757 = v1174 + v755;
										v725[13] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v722];
										BYTE1(v723) += BYTE2(v1135) + v180;
										v758 = v756 - 1;
										if (!v758)
											break;
										BYTE1(v722) = *(x_BYTE*)(v723 + v730);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v757);
										LOWORD(v757) = v1124 + v757;
										LOBYTE(v722) = v725[14];
										LOBYTE(v723) = BYTE2(v1124) + v180 + v723;
										v180 = __CFADD__(v1174, v757);
										v759 = v1174 + v757;
										v725[14] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v722];
										BYTE1(v723) += BYTE2(v1135) + v180;
										v760 = v758 - 1;
										if (!v760)
											break;
										BYTE1(v722) = *(x_BYTE*)(v723 + v730);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v759);
										LOWORD(v759) = v1124 + v759;
										LOBYTE(v722) = v725[15];
										LOBYTE(v723) = BYTE2(v1124) + v180 + v723;
										v180 = __CFADD__(v1174, v759);
										v728 = v1174 + v759;
										v725[15] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v722];
										BYTE1(v723) += BYTE2(v1135) + v180;
										LOWORD(v724) = v760 - 1;
										if (!(x_WORD)v724)
											break;
										v725 += 16;
									}
									v721 = v1283;
								}
							}
						LABEL_937:
							v721 += 10;
							if (!--triLn_v1123)
								return;
						}
						if (v724 > viewPortWidth)
							LOWORD(v724) = viewPortWidth;
						v18 = __OFSUB__((x_WORD)v724, (x_WORD)v722);
						LOWORD(v724) = v724 - v722;
						if ((unsigned __int8)(((v724 & 0x8000u) != 0) ^ v18) | ((x_WORD)v724 == 0))
							goto LABEL_937;
						v725 += v722;
						v728 = __ROL4__(*((x_DWORD*)v721 + 3), 16);
						BYTE1(v723) = v728;
						LOWORD(v728) = v721[4];
						LOBYTE(v723) = *((x_BYTE*)v721 + 10);
						goto LABEL_918;
					case 0x13:
						v761 = (unsigned __int16*)unk_DE56Cx;
						v1175 = v1135 << 16;
						HIWORD(v762) = 0;
						HIWORD(v763) = 0;
						while (1)
						{
							LOWORD(v762) = v761[1];
							v764 = v761[3];
							v765 = (x_BYTE*)(pitch + v1102);
							v1102 += pitch;
							line18++;
							if (line18 >= drawEveryNthLine)
							{
								line18 = 0;
								if ((v762 & 0x8000u) == 0)
									break;
								if ((signed __int16)v764 > 0)
								{
									v766 = (unsigned __int16)-(signed __int16)v762;
									v768 = __ROL4__(*((x_DWORD*)v761 + 3) + v1135 * v766, 16);
									BYTE1(v763) = v768;
									v767 = *((x_DWORD*)v761 + 2) + v1124 * v766;
									LOWORD(v768) = v767;
									v769 = v767 >> 8;
									LOBYTE(v763) = BYTE1(v769);
									if (v764 > viewPortWidth)
										LOWORD(v764) = viewPortWidth;
									v762 = (unsigned __int16)v769;
								LABEL_949:
									v1284 = v761;
									v770 = pTexture;
									while (1)
									{
										LOBYTE(v762) = *(x_BYTE*)(v763 + v770);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v768);
										LOWORD(v768) = v1124 + v768;
										BYTE1(v762) = *v765;
										LOBYTE(v763) = BYTE2(v1124) + v180 + v763;
										v180 = __CFADD__(v1175, v768);
										v771 = v1175 + v768;
										*v765 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v762];
										BYTE1(v763) += BYTE2(v1135) + v180;
										v772 = v764 - 1;
										if (!v772)
											break;
										LOBYTE(v762) = *(x_BYTE*)(v763 + v770);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v771);
										LOWORD(v771) = v1124 + v771;
										BYTE1(v762) = v765[1];
										LOBYTE(v763) = BYTE2(v1124) + v180 + v763;
										v180 = __CFADD__(v1175, v771);
										v773 = v1175 + v771;
										v765[1] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v762];
										BYTE1(v763) += BYTE2(v1135) + v180;
										v774 = v772 - 1;
										if (!v774)
											break;
										LOBYTE(v762) = *(x_BYTE*)(v763 + v770);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v773);
										LOWORD(v773) = v1124 + v773;
										BYTE1(v762) = v765[2];
										LOBYTE(v763) = BYTE2(v1124) + v180 + v763;
										v180 = __CFADD__(v1175, v773);
										v775 = v1175 + v773;
										v765[2] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v762];
										BYTE1(v763) += BYTE2(v1135) + v180;
										v776 = v774 - 1;
										if (!v776)
											break;
										LOBYTE(v762) = *(x_BYTE*)(v763 + v770);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v775);
										LOWORD(v775) = v1124 + v775;
										BYTE1(v762) = v765[3];
										LOBYTE(v763) = BYTE2(v1124) + v180 + v763;
										v180 = __CFADD__(v1175, v775);
										v777 = v1175 + v775;
										v765[3] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v762];
										BYTE1(v763) += BYTE2(v1135) + v180;
										v778 = v776 - 1;
										if (!v778)
											break;
										LOBYTE(v762) = *(x_BYTE*)(v763 + v770);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v777);
										LOWORD(v777) = v1124 + v777;
										BYTE1(v762) = v765[4];
										LOBYTE(v763) = BYTE2(v1124) + v180 + v763;
										v180 = __CFADD__(v1175, v777);
										v779 = v1175 + v777;
										v765[4] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v762];
										BYTE1(v763) += BYTE2(v1135) + v180;
										v780 = v778 - 1;
										if (!v780)
											break;
										LOBYTE(v762) = *(x_BYTE*)(v763 + v770);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v779);
										LOWORD(v779) = v1124 + v779;
										BYTE1(v762) = v765[5];
										LOBYTE(v763) = BYTE2(v1124) + v180 + v763;
										v180 = __CFADD__(v1175, v779);
										v781 = v1175 + v779;
										v765[5] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v762];
										BYTE1(v763) += BYTE2(v1135) + v180;
										v782 = v780 - 1;
										if (!v782)
											break;
										LOBYTE(v762) = *(x_BYTE*)(v763 + v770);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v781);
										LOWORD(v781) = v1124 + v781;
										BYTE1(v762) = v765[6];
										LOBYTE(v763) = BYTE2(v1124) + v180 + v763;
										v180 = __CFADD__(v1175, v781);
										v783 = v1175 + v781;
										v765[6] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v762];
										BYTE1(v763) += BYTE2(v1135) + v180;
										v784 = v782 - 1;
										if (!v784)
											break;
										LOBYTE(v762) = *(x_BYTE*)(v763 + v770);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v783);
										LOWORD(v783) = v1124 + v783;
										BYTE1(v762) = v765[7];
										LOBYTE(v763) = BYTE2(v1124) + v180 + v763;
										v180 = __CFADD__(v1175, v783);
										v785 = v1175 + v783;
										v765[7] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v762];
										BYTE1(v763) += BYTE2(v1135) + v180;
										v786 = v784 - 1;
										if (!v786)
											break;
										LOBYTE(v762) = *(x_BYTE*)(v763 + v770);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v785);
										LOWORD(v785) = v1124 + v785;
										BYTE1(v762) = v765[8];
										LOBYTE(v763) = BYTE2(v1124) + v180 + v763;
										v180 = __CFADD__(v1175, v785);
										v787 = v1175 + v785;
										v765[8] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v762];
										BYTE1(v763) += BYTE2(v1135) + v180;
										v788 = v786 - 1;
										if (!v788)
											break;
										LOBYTE(v762) = *(x_BYTE*)(v763 + v770);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v787);
										LOWORD(v787) = v1124 + v787;
										BYTE1(v762) = v765[9];
										LOBYTE(v763) = BYTE2(v1124) + v180 + v763;
										v180 = __CFADD__(v1175, v787);
										v789 = v1175 + v787;
										v765[9] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v762];
										BYTE1(v763) += BYTE2(v1135) + v180;
										v790 = v788 - 1;
										if (!v790)
											break;
										LOBYTE(v762) = *(x_BYTE*)(v763 + v770);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v789);
										LOWORD(v789) = v1124 + v789;
										BYTE1(v762) = v765[10];
										LOBYTE(v763) = BYTE2(v1124) + v180 + v763;
										v180 = __CFADD__(v1175, v789);
										v791 = v1175 + v789;
										v765[10] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v762];
										BYTE1(v763) += BYTE2(v1135) + v180;
										v792 = v790 - 1;
										if (!v792)
											break;
										LOBYTE(v762) = *(x_BYTE*)(v763 + v770);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v791);
										LOWORD(v791) = v1124 + v791;
										BYTE1(v762) = v765[11];
										LOBYTE(v763) = BYTE2(v1124) + v180 + v763;
										v180 = __CFADD__(v1175, v791);
										v793 = v1175 + v791;
										v765[11] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v762];
										BYTE1(v763) += BYTE2(v1135) + v180;
										v794 = v792 - 1;
										if (!v794)
											break;
										LOBYTE(v762) = *(x_BYTE*)(v763 + v770);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v793);
										LOWORD(v793) = v1124 + v793;
										BYTE1(v762) = v765[12];
										LOBYTE(v763) = BYTE2(v1124) + v180 + v763;
										v180 = __CFADD__(v1175, v793);
										v795 = v1175 + v793;
										v765[12] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v762];
										BYTE1(v763) += BYTE2(v1135) + v180;
										v796 = v794 - 1;
										if (!v796)
											break;
										LOBYTE(v762) = *(x_BYTE*)(v763 + v770);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v795);
										LOWORD(v795) = v1124 + v795;
										BYTE1(v762) = v765[13];
										LOBYTE(v763) = BYTE2(v1124) + v180 + v763;
										v180 = __CFADD__(v1175, v795);
										v797 = v1175 + v795;
										v765[13] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v762];
										BYTE1(v763) += BYTE2(v1135) + v180;
										v798 = v796 - 1;
										if (!v798)
											break;
										LOBYTE(v762) = *(x_BYTE*)(v763 + v770);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v797);
										LOWORD(v797) = v1124 + v797;
										BYTE1(v762) = v765[14];
										LOBYTE(v763) = BYTE2(v1124) + v180 + v763;
										v180 = __CFADD__(v1175, v797);
										v799 = v1175 + v797;
										v765[14] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v762];
										BYTE1(v763) += BYTE2(v1135) + v180;
										v800 = v798 - 1;
										if (!v800)
											break;
										LOBYTE(v762) = *(x_BYTE*)(v763 + v770);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v799);
										LOWORD(v799) = v1124 + v799;
										BYTE1(v762) = v765[15];
										LOBYTE(v763) = BYTE2(v1124) + v180 + v763;
										v180 = __CFADD__(v1175, v799);
										v768 = v1175 + v799;
										v765[15] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v762];
										BYTE1(v763) += BYTE2(v1135) + v180;
										LOWORD(v764) = v800 - 1;
										if (!(x_WORD)v764)
											break;
										v765 += 16;
									}
									v761 = v1284;
								}
							}
						LABEL_968:
							v761 += 10;
							if (!--triLn_v1123)
								return;
						}
						if (v764 > viewPortWidth)
							LOWORD(v764) = viewPortWidth;
						v18 = __OFSUB__((x_WORD)v764, (x_WORD)v762);
						LOWORD(v764) = v764 - v762;
						if ((unsigned __int8)(((v764 & 0x8000u) != 0) ^ v18) | ((x_WORD)v764 == 0))
							goto LABEL_968;
						v765 += v762;
						v768 = __ROL4__(*((x_DWORD*)v761 + 3), 16);
						BYTE1(v763) = v768;
						LOWORD(v768) = v761[4];
						LOBYTE(v763) = *((x_BYTE*)v761 + 10);
						goto LABEL_949;
					case 0x14:
						v801 = (unsigned __int16*)unk_DE56Cx;
						v1176 = v1135 << 16;
						v1185 = v1146 << 16;
						HIWORD(v802) = 0;
						HIWORD(v803) = 0;
						while (1)
						{
							LOWORD(v802) = v801[1];
							v804 = v801[3];
							v805 = (x_BYTE*)(pitch + v1102);
							v1102 += pitch;
							line19++;
							if (line19 >= drawEveryNthLine)
							{
								line19 = 0;
								if ((v802 & 0x8000u) == 0)
									break;
								if ((signed __int16)v804 > 0)
								{
									if (v804 > viewPortWidth)
										v804 = viewPortWidth;
									v1194 = v804;
									v806 = (unsigned __int16)-(signed __int16)v802;
									v807 = v806;
									v809 = __ROL4__(*((x_DWORD*)v801 + 3) + v1135 * v806, 16);
									BYTE1(v803) = v809;
									v808 = *((x_DWORD*)v801 + 2) + v1124 * v806;
									LOWORD(v809) = v808;
									v802 = v808 >> 8;
									LOBYTE(v803) = BYTE1(v802);
									v810 = __ROL4__(*((x_DWORD*)v801 + 4) + v1146 * v807, 16);
									v802 = (unsigned __int16)v802;
								LABEL_980:
									v1285 = v801;
									v811 = pTexture;
									while (1)
									{
										LOBYTE(v802) = *(x_BYTE*)(v803 + v811);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v809);
										LOWORD(v809) = v1124 + v809;
										BYTE1(v802) = v810;
										LOBYTE(v803) = BYTE2(v1124) + v180 + v803;
										BYTE1(v802) = m_ptrX_BYTE_F6EE0_tablesx[v802];
										v180 = __CFADD__(v1176, v809);
										v812 = v1176 + v809;
										LOBYTE(v802) = *v805;
										BYTE1(v803) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1185, v810);
										v813 = v1185 + v810;
										*v805 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v802];
										LOBYTE(v813) = BYTE2(v1146) + v180 + v813;
										v1195 = v1194 - 1;
										if (!v1195)
											break;
										LOBYTE(v802) = *(x_BYTE*)(v803 + v811);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v812);
										LOWORD(v812) = v1124 + v812;
										BYTE1(v802) = v813;
										LOBYTE(v803) = BYTE2(v1124) + v180 + v803;
										BYTE1(v802) = m_ptrX_BYTE_F6EE0_tablesx[v802];
										v180 = __CFADD__(v1176, v812);
										v814 = v1176 + v812;
										LOBYTE(v802) = v805[1];
										BYTE1(v803) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1185, v813);
										v815 = v1185 + v813;
										v805[1] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v802];
										LOBYTE(v815) = BYTE2(v1146) + v180 + v815;
										v1196 = v1195 - 1;
										if (!v1196)
											break;
										LOBYTE(v802) = *(x_BYTE*)(v803 + v811);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v814);
										LOWORD(v814) = v1124 + v814;
										BYTE1(v802) = v815;
										LOBYTE(v803) = BYTE2(v1124) + v180 + v803;
										BYTE1(v802) = m_ptrX_BYTE_F6EE0_tablesx[v802];
										v180 = __CFADD__(v1176, v814);
										v816 = v1176 + v814;
										LOBYTE(v802) = v805[2];
										BYTE1(v803) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1185, v815);
										v817 = v1185 + v815;
										v805[2] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v802];
										LOBYTE(v817) = BYTE2(v1146) + v180 + v817;
										v1197 = v1196 - 1;
										if (!v1197)
											break;
										LOBYTE(v802) = *(x_BYTE*)(v803 + v811);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v816);
										LOWORD(v816) = v1124 + v816;
										BYTE1(v802) = v817;
										LOBYTE(v803) = BYTE2(v1124) + v180 + v803;
										BYTE1(v802) = m_ptrX_BYTE_F6EE0_tablesx[v802];
										v180 = __CFADD__(v1176, v816);
										v818 = v1176 + v816;
										LOBYTE(v802) = v805[3];
										BYTE1(v803) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1185, v817);
										v819 = v1185 + v817;
										v805[3] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v802];
										LOBYTE(v819) = BYTE2(v1146) + v180 + v819;
										v1198 = v1197 - 1;
										if (!v1198)
											break;
										LOBYTE(v802) = *(x_BYTE*)(v803 + v811);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v818);
										LOWORD(v818) = v1124 + v818;
										BYTE1(v802) = v819;
										LOBYTE(v803) = BYTE2(v1124) + v180 + v803;
										BYTE1(v802) = m_ptrX_BYTE_F6EE0_tablesx[v802];
										v180 = __CFADD__(v1176, v818);
										v820 = v1176 + v818;
										LOBYTE(v802) = v805[4];
										BYTE1(v803) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1185, v819);
										v821 = v1185 + v819;
										v805[4] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v802];
										LOBYTE(v821) = BYTE2(v1146) + v180 + v821;
										v1199 = v1198 - 1;
										if (!v1199)
											break;
										LOBYTE(v802) = *(x_BYTE*)(v803 + v811);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v820);
										LOWORD(v820) = v1124 + v820;
										BYTE1(v802) = v821;
										LOBYTE(v803) = BYTE2(v1124) + v180 + v803;
										BYTE1(v802) = m_ptrX_BYTE_F6EE0_tablesx[v802];
										v180 = __CFADD__(v1176, v820);
										v822 = v1176 + v820;
										LOBYTE(v802) = v805[5];
										BYTE1(v803) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1185, v821);
										v823 = v1185 + v821;
										v805[5] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v802];
										LOBYTE(v823) = BYTE2(v1146) + v180 + v823;
										v1200 = v1199 - 1;
										if (!v1200)
											break;
										LOBYTE(v802) = *(x_BYTE*)(v803 + v811);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v822);
										LOWORD(v822) = v1124 + v822;
										BYTE1(v802) = v823;
										LOBYTE(v803) = BYTE2(v1124) + v180 + v803;
										BYTE1(v802) = m_ptrX_BYTE_F6EE0_tablesx[v802];
										v180 = __CFADD__(v1176, v822);
										v824 = v1176 + v822;
										LOBYTE(v802) = v805[6];
										BYTE1(v803) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1185, v823);
										v825 = v1185 + v823;
										v805[6] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v802];
										LOBYTE(v825) = BYTE2(v1146) + v180 + v825;
										v1201 = v1200 - 1;
										if (!v1201)
											break;
										LOBYTE(v802) = *(x_BYTE*)(v803 + v811);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v824);
										LOWORD(v824) = v1124 + v824;
										BYTE1(v802) = v825;
										LOBYTE(v803) = BYTE2(v1124) + v180 + v803;
										BYTE1(v802) = m_ptrX_BYTE_F6EE0_tablesx[v802];
										v180 = __CFADD__(v1176, v824);
										v826 = v1176 + v824;
										LOBYTE(v802) = v805[7];
										BYTE1(v803) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1185, v825);
										v827 = v1185 + v825;
										v805[7] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v802];
										LOBYTE(v827) = BYTE2(v1146) + v180 + v827;
										v1202 = v1201 - 1;
										if (!v1202)
											break;
										LOBYTE(v802) = *(x_BYTE*)(v803 + v811);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v826);
										LOWORD(v826) = v1124 + v826;
										BYTE1(v802) = v827;
										LOBYTE(v803) = BYTE2(v1124) + v180 + v803;
										BYTE1(v802) = m_ptrX_BYTE_F6EE0_tablesx[v802];
										v180 = __CFADD__(v1176, v826);
										v828 = v1176 + v826;
										LOBYTE(v802) = v805[8];
										BYTE1(v803) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1185, v827);
										v829 = v1185 + v827;
										v805[8] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v802];
										LOBYTE(v829) = BYTE2(v1146) + v180 + v829;
										v1203 = v1202 - 1;
										if (!v1203)
											break;
										LOBYTE(v802) = *(x_BYTE*)(v803 + v811);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v828);
										LOWORD(v828) = v1124 + v828;
										BYTE1(v802) = v829;
										LOBYTE(v803) = BYTE2(v1124) + v180 + v803;
										BYTE1(v802) = m_ptrX_BYTE_F6EE0_tablesx[v802];
										v180 = __CFADD__(v1176, v828);
										v830 = v1176 + v828;
										LOBYTE(v802) = v805[9];
										BYTE1(v803) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1185, v829);
										v831 = v1185 + v829;
										v805[9] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v802];
										LOBYTE(v831) = BYTE2(v1146) + v180 + v831;
										v1204 = v1203 - 1;
										if (!v1204)
											break;
										LOBYTE(v802) = *(x_BYTE*)(v803 + v811);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v830);
										LOWORD(v830) = v1124 + v830;
										BYTE1(v802) = v831;
										LOBYTE(v803) = BYTE2(v1124) + v180 + v803;
										BYTE1(v802) = m_ptrX_BYTE_F6EE0_tablesx[v802];
										v180 = __CFADD__(v1176, v830);
										v832 = v1176 + v830;
										LOBYTE(v802) = v805[10];
										BYTE1(v803) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1185, v831);
										v833 = v1185 + v831;
										v805[10] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v802];
										LOBYTE(v833) = BYTE2(v1146) + v180 + v833;
										v1205 = v1204 - 1;
										if (!v1205)
											break;
										LOBYTE(v802) = *(x_BYTE*)(v803 + v811);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v832);
										LOWORD(v832) = v1124 + v832;
										BYTE1(v802) = v833;
										LOBYTE(v803) = BYTE2(v1124) + v180 + v803;
										BYTE1(v802) = m_ptrX_BYTE_F6EE0_tablesx[v802];
										v180 = __CFADD__(v1176, v832);
										v834 = v1176 + v832;
										LOBYTE(v802) = v805[11];
										BYTE1(v803) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1185, v833);
										v835 = v1185 + v833;
										v805[11] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v802];
										LOBYTE(v835) = BYTE2(v1146) + v180 + v835;
										v1206 = v1205 - 1;
										if (!v1206)
											break;
										LOBYTE(v802) = *(x_BYTE*)(v803 + v811);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v834);
										LOWORD(v834) = v1124 + v834;
										BYTE1(v802) = v835;
										LOBYTE(v803) = BYTE2(v1124) + v180 + v803;
										BYTE1(v802) = m_ptrX_BYTE_F6EE0_tablesx[v802];
										v180 = __CFADD__(v1176, v834);
										v836 = v1176 + v834;
										LOBYTE(v802) = v805[12];
										BYTE1(v803) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1185, v835);
										v837 = v1185 + v835;
										v805[12] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v802];
										LOBYTE(v837) = BYTE2(v1146) + v180 + v837;
										v1207 = v1206 - 1;
										if (!v1207)
											break;
										LOBYTE(v802) = *(x_BYTE*)(v803 + v811);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v836);
										LOWORD(v836) = v1124 + v836;
										BYTE1(v802) = v837;
										LOBYTE(v803) = BYTE2(v1124) + v180 + v803;
										BYTE1(v802) = m_ptrX_BYTE_F6EE0_tablesx[v802];
										v180 = __CFADD__(v1176, v836);
										v838 = v1176 + v836;
										LOBYTE(v802) = v805[13];
										BYTE1(v803) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1185, v837);
										v839 = v1185 + v837;
										v805[13] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v802];
										LOBYTE(v839) = BYTE2(v1146) + v180 + v839;
										v1208 = v1207 - 1;
										if (!v1208)
											break;
										LOBYTE(v802) = *(x_BYTE*)(v803 + v811);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v838);
										LOWORD(v838) = v1124 + v838;
										BYTE1(v802) = v839;
										LOBYTE(v803) = BYTE2(v1124) + v180 + v803;
										BYTE1(v802) = m_ptrX_BYTE_F6EE0_tablesx[v802];
										v180 = __CFADD__(v1176, v838);
										v840 = v1176 + v838;
										LOBYTE(v802) = v805[14];
										BYTE1(v803) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1185, v839);
										v841 = v1185 + v839;
										v805[14] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v802];
										LOBYTE(v841) = BYTE2(v1146) + v180 + v841;
										v1209 = v1208 - 1;
										if (!v1209)
											break;
										LOBYTE(v802) = *(x_BYTE*)(v803 + v811);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v840);
										LOWORD(v840) = v1124 + v840;
										BYTE1(v802) = v841;
										LOBYTE(v803) = BYTE2(v1124) + v180 + v803;
										BYTE1(v802) = m_ptrX_BYTE_F6EE0_tablesx[v802];
										v180 = __CFADD__(v1176, v840);
										v809 = v1176 + v840;
										LOBYTE(v802) = v805[15];
										BYTE1(v803) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1185, v841);
										v810 = v1185 + v841;
										v805[15] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v802];
										LOBYTE(v810) = BYTE2(v1146) + v180 + v810;
										v1194 = v1209 - 1;
										if (!v1194)
											break;
										v805 += 16;
									}
									v801 = v1285;
								}
							}
						LABEL_999:
							v801 += 10;
							if (!--triLn_v1123)
								return;
						}
						if (v804 > viewPortWidth)
							v804 = viewPortWidth;
						v18 = __OFSUB__((x_WORD)v804, (x_WORD)v802);
						LOWORD(v804) = v804 - v802;
						if ((unsigned __int8)(((v804 & 0x8000u) != 0) ^ v18) | ((x_WORD)v804 == 0))
							goto LABEL_999;
						v805 += v802;
						v809 = __ROL4__(*((x_DWORD*)v801 + 3), 16);
						BYTE1(v803) = v809;
						LOWORD(v809) = v801[4];
						LOBYTE(v803) = *((x_BYTE*)v801 + 10);
						v1194 = v804;
						v810 = __ROL4__(*((x_DWORD*)v801 + 4), 16);
						goto LABEL_980;
					case 0x15:
						v842 = (unsigned __int16*)unk_DE56Cx;
						v1177 = v1135 << 16;
						v1186 = v1146 << 16;
						HIWORD(v843) = 0;
						HIWORD(v844) = 0;
						while (1)
						{
							LOWORD(v843) = v842[1];
							v845 = v842[3];
							v846 = (x_BYTE*)(pitch + v1102);
							v1102 += pitch;
							line20++;
							if (line20 >= drawEveryNthLine)
							{
								line20 = 0;
								if ((v843 & 0x8000u) == 0)
									break;
								if ((signed __int16)v845 > 0)
								{
									if (v845 > viewPortWidth)
										v845 = viewPortWidth;
									v1210 = v845;
									v847 = (unsigned __int16)-(signed __int16)v843;
									v848 = v847;
									v850 = __ROL4__(*((x_DWORD*)v842 + 3) + v1135 * v847, 16);
									BYTE1(v844) = v850;
									v849 = *((x_DWORD*)v842 + 2) + v1124 * v847;
									LOWORD(v850) = v849;
									v843 = v849 >> 8;
									LOBYTE(v844) = BYTE1(v843);
									v851 = __ROL4__(*((x_DWORD*)v842 + 4) + v1146 * v848, 16);
									v843 = (unsigned __int16)v843;
								LABEL_1011:
									v1286 = v842;
									v852 = pTexture;
									while (1)
									{
										LOBYTE(v843) = *(x_BYTE*)(v844 + v852);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v850);
										LOWORD(v850) = v1124 + v850;
										BYTE1(v843) = v851;
										LOBYTE(v844) = BYTE2(v1124) + v180 + v844;
										LOBYTE(v843) = m_ptrX_BYTE_F6EE0_tablesx[v843];
										v180 = __CFADD__(v1177, v850);
										v853 = v1177 + v850;
										BYTE1(v843) = *v846;
										BYTE1(v844) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1186, v851);
										v854 = v1186 + v851;
										*v846 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v843];
										LOBYTE(v854) = BYTE2(v1146) + v180 + v854;
										v1211 = v1210 - 1;
										if (!v1211)
											break;
										LOBYTE(v843) = *(x_BYTE*)(v844 + v852);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v853);
										LOWORD(v853) = v1124 + v853;
										BYTE1(v843) = v854;
										LOBYTE(v844) = BYTE2(v1124) + v180 + v844;
										LOBYTE(v843) = m_ptrX_BYTE_F6EE0_tablesx[v843];
										v180 = __CFADD__(v1177, v853);
										v855 = v1177 + v853;
										BYTE1(v843) = v846[1];
										BYTE1(v844) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1186, v854);
										v856 = v1186 + v854;
										v846[1] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v843];
										LOBYTE(v856) = BYTE2(v1146) + v180 + v856;
										v1212 = v1211 - 1;
										if (!v1212)
											break;
										LOBYTE(v843) = *(x_BYTE*)(v844 + v852);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v855);
										LOWORD(v855) = v1124 + v855;
										BYTE1(v843) = v856;
										LOBYTE(v844) = BYTE2(v1124) + v180 + v844;
										LOBYTE(v843) = m_ptrX_BYTE_F6EE0_tablesx[v843];
										v180 = __CFADD__(v1177, v855);
										v857 = v1177 + v855;
										BYTE1(v843) = v846[2];
										BYTE1(v844) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1186, v856);
										v858 = v1186 + v856;
										v846[2] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v843];
										LOBYTE(v858) = BYTE2(v1146) + v180 + v858;
										v1213 = v1212 - 1;
										if (!v1213)
											break;
										LOBYTE(v843) = *(x_BYTE*)(v844 + v852);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v857);
										LOWORD(v857) = v1124 + v857;
										BYTE1(v843) = v858;
										LOBYTE(v844) = BYTE2(v1124) + v180 + v844;
										LOBYTE(v843) = m_ptrX_BYTE_F6EE0_tablesx[v843];
										v180 = __CFADD__(v1177, v857);
										v859 = v1177 + v857;
										BYTE1(v843) = v846[3];
										BYTE1(v844) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1186, v858);
										v860 = v1186 + v858;
										v846[3] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v843];
										LOBYTE(v860) = BYTE2(v1146) + v180 + v860;
										v1214 = v1213 - 1;
										if (!v1214)
											break;
										LOBYTE(v843) = *(x_BYTE*)(v844 + v852);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v859);
										LOWORD(v859) = v1124 + v859;
										BYTE1(v843) = v860;
										LOBYTE(v844) = BYTE2(v1124) + v180 + v844;
										LOBYTE(v843) = m_ptrX_BYTE_F6EE0_tablesx[v843];
										v180 = __CFADD__(v1177, v859);
										v861 = v1177 + v859;
										BYTE1(v843) = v846[4];
										BYTE1(v844) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1186, v860);
										v862 = v1186 + v860;
										v846[4] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v843];
										LOBYTE(v862) = BYTE2(v1146) + v180 + v862;
										v1215 = v1214 - 1;
										if (!v1215)
											break;
										LOBYTE(v843) = *(x_BYTE*)(v844 + v852);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v861);
										LOWORD(v861) = v1124 + v861;
										BYTE1(v843) = v862;
										LOBYTE(v844) = BYTE2(v1124) + v180 + v844;
										LOBYTE(v843) = m_ptrX_BYTE_F6EE0_tablesx[v843];
										v180 = __CFADD__(v1177, v861);
										v863 = v1177 + v861;
										BYTE1(v843) = v846[5];
										BYTE1(v844) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1186, v862);
										v864 = v1186 + v862;
										v846[5] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v843];
										LOBYTE(v864) = BYTE2(v1146) + v180 + v864;
										v1216 = v1215 - 1;
										if (!v1216)
											break;
										LOBYTE(v843) = *(x_BYTE*)(v844 + v852);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v863);
										LOWORD(v863) = v1124 + v863;
										BYTE1(v843) = v864;
										LOBYTE(v844) = BYTE2(v1124) + v180 + v844;
										LOBYTE(v843) = m_ptrX_BYTE_F6EE0_tablesx[v843];
										v180 = __CFADD__(v1177, v863);
										v865 = v1177 + v863;
										BYTE1(v843) = v846[6];
										BYTE1(v844) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1186, v864);
										v866 = v1186 + v864;
										v846[6] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v843];
										LOBYTE(v866) = BYTE2(v1146) + v180 + v866;
										v1217 = v1216 - 1;
										if (!v1217)
											break;
										LOBYTE(v843) = *(x_BYTE*)(v844 + v852);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v865);
										LOWORD(v865) = v1124 + v865;
										BYTE1(v843) = v866;
										LOBYTE(v844) = BYTE2(v1124) + v180 + v844;
										LOBYTE(v843) = m_ptrX_BYTE_F6EE0_tablesx[v843];
										v180 = __CFADD__(v1177, v865);
										v867 = v1177 + v865;
										BYTE1(v843) = v846[7];
										BYTE1(v844) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1186, v866);
										v868 = v1186 + v866;
										v846[7] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v843];
										LOBYTE(v868) = BYTE2(v1146) + v180 + v868;
										v1218 = v1217 - 1;
										if (!v1218)
											break;
										LOBYTE(v843) = *(x_BYTE*)(v844 + v852);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v867);
										LOWORD(v867) = v1124 + v867;
										BYTE1(v843) = v868;
										LOBYTE(v844) = BYTE2(v1124) + v180 + v844;
										LOBYTE(v843) = m_ptrX_BYTE_F6EE0_tablesx[v843];
										v180 = __CFADD__(v1177, v867);
										v869 = v1177 + v867;
										BYTE1(v843) = v846[8];
										BYTE1(v844) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1186, v868);
										v870 = v1186 + v868;
										v846[8] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v843];
										LOBYTE(v870) = BYTE2(v1146) + v180 + v870;
										v1219 = v1218 - 1;
										if (!v1219)
											break;
										LOBYTE(v843) = *(x_BYTE*)(v844 + v852);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v869);
										LOWORD(v869) = v1124 + v869;
										BYTE1(v843) = v870;
										LOBYTE(v844) = BYTE2(v1124) + v180 + v844;
										LOBYTE(v843) = m_ptrX_BYTE_F6EE0_tablesx[v843];
										v180 = __CFADD__(v1177, v869);
										v871 = v1177 + v869;
										BYTE1(v843) = v846[9];
										BYTE1(v844) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1186, v870);
										v872 = v1186 + v870;
										v846[9] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v843];
										LOBYTE(v872) = BYTE2(v1146) + v180 + v872;
										v1220 = v1219 - 1;
										if (!v1220)
											break;
										LOBYTE(v843) = *(x_BYTE*)(v844 + v852);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v871);
										LOWORD(v871) = v1124 + v871;
										BYTE1(v843) = v872;
										LOBYTE(v844) = BYTE2(v1124) + v180 + v844;
										LOBYTE(v843) = m_ptrX_BYTE_F6EE0_tablesx[v843];
										v180 = __CFADD__(v1177, v871);
										v873 = v1177 + v871;
										BYTE1(v843) = v846[10];
										BYTE1(v844) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1186, v872);
										v874 = v1186 + v872;
										v846[10] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v843];
										LOBYTE(v874) = BYTE2(v1146) + v180 + v874;
										v1221 = v1220 - 1;
										if (!v1221)
											break;
										LOBYTE(v843) = *(x_BYTE*)(v844 + v852);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v873);
										LOWORD(v873) = v1124 + v873;
										BYTE1(v843) = v874;
										LOBYTE(v844) = BYTE2(v1124) + v180 + v844;
										LOBYTE(v843) = m_ptrX_BYTE_F6EE0_tablesx[v843];
										v180 = __CFADD__(v1177, v873);
										v875 = v1177 + v873;
										BYTE1(v843) = v846[11];
										BYTE1(v844) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1186, v874);
										v876 = v1186 + v874;
										v846[11] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v843];
										LOBYTE(v876) = BYTE2(v1146) + v180 + v876;
										v1222 = v1221 - 1;
										if (!v1222)
											break;
										LOBYTE(v843) = *(x_BYTE*)(v844 + v852);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v875);
										LOWORD(v875) = v1124 + v875;
										BYTE1(v843) = v876;
										LOBYTE(v844) = BYTE2(v1124) + v180 + v844;
										LOBYTE(v843) = m_ptrX_BYTE_F6EE0_tablesx[v843];
										v180 = __CFADD__(v1177, v875);
										v877 = v1177 + v875;
										BYTE1(v843) = v846[12];
										BYTE1(v844) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1186, v876);
										v878 = v1186 + v876;
										v846[12] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v843];
										LOBYTE(v878) = BYTE2(v1146) + v180 + v878;
										v1223 = v1222 - 1;
										if (!v1223)
											break;
										LOBYTE(v843) = *(x_BYTE*)(v844 + v852);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v877);
										LOWORD(v877) = v1124 + v877;
										BYTE1(v843) = v878;
										LOBYTE(v844) = BYTE2(v1124) + v180 + v844;
										LOBYTE(v843) = m_ptrX_BYTE_F6EE0_tablesx[v843];
										v180 = __CFADD__(v1177, v877);
										v879 = v1177 + v877;
										BYTE1(v843) = v846[13];
										BYTE1(v844) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1186, v878);
										v880 = v1186 + v878;
										v846[13] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v843];
										LOBYTE(v880) = BYTE2(v1146) + v180 + v880;
										v1224 = v1223 - 1;
										if (!v1224)
											break;
										LOBYTE(v843) = *(x_BYTE*)(v844 + v852);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v879);
										LOWORD(v879) = v1124 + v879;
										BYTE1(v843) = v880;
										LOBYTE(v844) = BYTE2(v1124) + v180 + v844;
										LOBYTE(v843) = m_ptrX_BYTE_F6EE0_tablesx[v843];
										v180 = __CFADD__(v1177, v879);
										v881 = v1177 + v879;
										BYTE1(v843) = v846[14];
										BYTE1(v844) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1186, v880);
										v882 = v1186 + v880;
										v846[14] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v843];
										LOBYTE(v882) = BYTE2(v1146) + v180 + v882;
										v1225 = v1224 - 1;
										if (!v1225)
											break;
										LOBYTE(v843) = *(x_BYTE*)(v844 + v852);
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v881);
										LOWORD(v881) = v1124 + v881;
										BYTE1(v843) = v882;
										LOBYTE(v844) = BYTE2(v1124) + v180 + v844;
										LOBYTE(v843) = m_ptrX_BYTE_F6EE0_tablesx[v843];
										v180 = __CFADD__(v1177, v881);
										v850 = v1177 + v881;
										BYTE1(v843) = v846[15];
										BYTE1(v844) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1186, v882);
										v851 = v1186 + v882;
										v846[15] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v843];
										LOBYTE(v851) = BYTE2(v1146) + v180 + v851;
										v1210 = v1225 - 1;
										if (!v1210)
											break;
										v846 += 16;
									}
									v842 = v1286;
								}
							}
						LABEL_1030:
							v842 += 10;
							if (!--triLn_v1123)
								return;
						}
						if (v845 > viewPortWidth)
							v845 = viewPortWidth;
						v18 = __OFSUB__((x_WORD)v845, (x_WORD)v843);
						LOWORD(v845) = v845 - v843;
						if ((unsigned __int8)(((v845 & 0x8000u) != 0) ^ v18) | ((x_WORD)v845 == 0))
							goto LABEL_1030;
						v846 += v843;
						v850 = __ROL4__(*((x_DWORD*)v842 + 3), 16);
						BYTE1(v844) = v850;
						LOWORD(v850) = v842[4];
						LOBYTE(v844) = *((x_BYTE*)v842 + 10);
						v1210 = v845;
						v851 = __ROL4__(*((x_DWORD*)v842 + 4), 16);
						goto LABEL_1011;
					case 0x16:
						v883 = (unsigned __int16*)unk_DE56Cx;
						v1178 = v1135 << 16;
						HIWORD(v884) = 0;
						HIWORD(v885) = 0;
						while (1)
						{
							LOWORD(v884) = v883[1];
							v886 = v883[3];
							v887 = (x_BYTE*)(pitch + v1102);
							v1102 += pitch;
							line21++;
							if (line21 >= drawEveryNthLine)
							{
								line21 = 0;
								if ((v884 & 0x8000u) == 0)
									break;
								if ((signed __int16)v886 > 0)
								{
									v888 = (unsigned __int16)-(signed __int16)v884;
									v890 = __ROL4__(*((x_DWORD*)v883 + 3) + v1135 * v888, 16);
									BYTE1(v885) = v890;
									v889 = *((x_DWORD*)v883 + 2) + v1124 * v888;
									LOWORD(v890) = v889;
									v891 = v889 >> 8;
									LOBYTE(v885) = BYTE1(v891);
									if (v886 > viewPortWidth)
										LOWORD(v886) = viewPortWidth;
									v884 = (unsigned __int16)v891;
								LABEL_1042:
									v1287 = v883;
									v892 = pTexture;
									while (1)
									{
										BYTE1(v884) = *(x_BYTE*)(v885 + v892);
										if (BYTE1(v884))
										{
											LOBYTE(v884) = *v887;
											*v887 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v884];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v890);
										LOWORD(v890) = v1124 + v890;
										LOBYTE(v885) = BYTE2(v1124) + v180 + v885;
										v180 = __CFADD__(v1178, v890);
										v893 = v1178 + v890;
										BYTE1(v885) += BYTE2(v1135) + v180;
										v894 = v886 - 1;
										if (!v894)
											break;
										BYTE1(v884) = *(x_BYTE*)(v885 + v892);
										if (BYTE1(v884))
										{
											LOBYTE(v884) = v887[1];
											v887[1] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v884];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v893);
										LOWORD(v893) = v1124 + v893;
										LOBYTE(v885) = BYTE2(v1124) + v180 + v885;
										v180 = __CFADD__(v1178, v893);
										v895 = v1178 + v893;
										BYTE1(v885) += BYTE2(v1135) + v180;
										v896 = v894 - 1;
										if (!v896)
											break;
										BYTE1(v884) = *(x_BYTE*)(v885 + v892);
										if (BYTE1(v884))
										{
											LOBYTE(v884) = v887[2];
											v887[2] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v884];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v895);
										LOWORD(v895) = v1124 + v895;
										LOBYTE(v885) = BYTE2(v1124) + v180 + v885;
										v180 = __CFADD__(v1178, v895);
										v897 = v1178 + v895;
										BYTE1(v885) += BYTE2(v1135) + v180;
										v898 = v896 - 1;
										if (!v898)
											break;
										BYTE1(v884) = *(x_BYTE*)(v885 + v892);
										if (BYTE1(v884))
										{
											LOBYTE(v884) = v887[3];
											v887[3] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v884];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v897);
										LOWORD(v897) = v1124 + v897;
										LOBYTE(v885) = BYTE2(v1124) + v180 + v885;
										v180 = __CFADD__(v1178, v897);
										v899 = v1178 + v897;
										BYTE1(v885) += BYTE2(v1135) + v180;
										v900 = v898 - 1;
										if (!v900)
											break;
										BYTE1(v884) = *(x_BYTE*)(v885 + v892);
										if (BYTE1(v884))
										{
											LOBYTE(v884) = v887[4];
											v887[4] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v884];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v899);
										LOWORD(v899) = v1124 + v899;
										LOBYTE(v885) = BYTE2(v1124) + v180 + v885;
										v180 = __CFADD__(v1178, v899);
										v901 = v1178 + v899;
										BYTE1(v885) += BYTE2(v1135) + v180;
										v902 = v900 - 1;
										if (!v902)
											break;
										BYTE1(v884) = *(x_BYTE*)(v885 + v892);
										if (BYTE1(v884))
										{
											LOBYTE(v884) = v887[5];
											v887[5] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v884];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v901);
										LOWORD(v901) = v1124 + v901;
										LOBYTE(v885) = BYTE2(v1124) + v180 + v885;
										v180 = __CFADD__(v1178, v901);
										v903 = v1178 + v901;
										BYTE1(v885) += BYTE2(v1135) + v180;
										v904 = v902 - 1;
										if (!v904)
											break;
										BYTE1(v884) = *(x_BYTE*)(v885 + v892);
										if (BYTE1(v884))
										{
											LOBYTE(v884) = v887[6];
											v887[6] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v884];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v903);
										LOWORD(v903) = v1124 + v903;
										LOBYTE(v885) = BYTE2(v1124) + v180 + v885;
										v180 = __CFADD__(v1178, v903);
										v905 = v1178 + v903;
										BYTE1(v885) += BYTE2(v1135) + v180;
										v906 = v904 - 1;
										if (!v906)
											break;
										BYTE1(v884) = *(x_BYTE*)(v885 + v892);
										if (BYTE1(v884))
										{
											LOBYTE(v884) = v887[7];
											v887[7] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v884];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v905);
										LOWORD(v905) = v1124 + v905;
										LOBYTE(v885) = BYTE2(v1124) + v180 + v885;
										v180 = __CFADD__(v1178, v905);
										v907 = v1178 + v905;
										BYTE1(v885) += BYTE2(v1135) + v180;
										v908 = v906 - 1;
										if (!v908)
											break;
										BYTE1(v884) = *(x_BYTE*)(v885 + v892);
										if (BYTE1(v884))
										{
											LOBYTE(v884) = v887[8];
											v887[8] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v884];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v907);
										LOWORD(v907) = v1124 + v907;
										LOBYTE(v885) = BYTE2(v1124) + v180 + v885;
										v180 = __CFADD__(v1178, v907);
										v909 = v1178 + v907;
										BYTE1(v885) += BYTE2(v1135) + v180;
										v910 = v908 - 1;
										if (!v910)
											break;
										BYTE1(v884) = *(x_BYTE*)(v885 + v892);
										if (BYTE1(v884))
										{
											LOBYTE(v884) = v887[9];
											v887[9] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v884];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v909);
										LOWORD(v909) = v1124 + v909;
										LOBYTE(v885) = BYTE2(v1124) + v180 + v885;
										v180 = __CFADD__(v1178, v909);
										v911 = v1178 + v909;
										BYTE1(v885) += BYTE2(v1135) + v180;
										v912 = v910 - 1;
										if (!v912)
											break;
										BYTE1(v884) = *(x_BYTE*)(v885 + v892);
										if (BYTE1(v884))
										{
											LOBYTE(v884) = v887[10];
											v887[10] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v884];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v911);
										LOWORD(v911) = v1124 + v911;
										LOBYTE(v885) = BYTE2(v1124) + v180 + v885;
										v180 = __CFADD__(v1178, v911);
										v913 = v1178 + v911;
										BYTE1(v885) += BYTE2(v1135) + v180;
										v914 = v912 - 1;
										if (!v914)
											break;
										BYTE1(v884) = *(x_BYTE*)(v885 + v892);
										if (BYTE1(v884))
										{
											LOBYTE(v884) = v887[11];
											v887[11] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v884];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v913);
										LOWORD(v913) = v1124 + v913;
										LOBYTE(v885) = BYTE2(v1124) + v180 + v885;
										v180 = __CFADD__(v1178, v913);
										v915 = v1178 + v913;
										BYTE1(v885) += BYTE2(v1135) + v180;
										v916 = v914 - 1;
										if (!v916)
											break;
										BYTE1(v884) = *(x_BYTE*)(v885 + v892);
										if (BYTE1(v884))
										{
											LOBYTE(v884) = v887[12];
											v887[12] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v884];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v915);
										LOWORD(v915) = v1124 + v915;
										LOBYTE(v885) = BYTE2(v1124) + v180 + v885;
										v180 = __CFADD__(v1178, v915);
										v917 = v1178 + v915;
										BYTE1(v885) += BYTE2(v1135) + v180;
										v918 = v916 - 1;
										if (!v918)
											break;
										BYTE1(v884) = *(x_BYTE*)(v885 + v892);
										if (BYTE1(v884))
										{
											LOBYTE(v884) = v887[13];
											v887[13] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v884];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v917);
										LOWORD(v917) = v1124 + v917;
										LOBYTE(v885) = BYTE2(v1124) + v180 + v885;
										v180 = __CFADD__(v1178, v917);
										v919 = v1178 + v917;
										BYTE1(v885) += BYTE2(v1135) + v180;
										v920 = v918 - 1;
										if (!v920)
											break;
										BYTE1(v884) = *(x_BYTE*)(v885 + v892);
										if (BYTE1(v884))
										{
											LOBYTE(v884) = v887[14];
											v887[14] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v884];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v919);
										LOWORD(v919) = v1124 + v919;
										LOBYTE(v885) = BYTE2(v1124) + v180 + v885;
										v180 = __CFADD__(v1178, v919);
										v921 = v1178 + v919;
										BYTE1(v885) += BYTE2(v1135) + v180;
										v922 = v920 - 1;
										if (!v922)
											break;
										BYTE1(v884) = *(x_BYTE*)(v885 + v892);
										if (BYTE1(v884))
										{
											LOBYTE(v884) = v887[15];
											v887[15] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v884];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v921);
										LOWORD(v921) = v1124 + v921;
										LOBYTE(v885) = BYTE2(v1124) + v180 + v885;
										v180 = __CFADD__(v1178, v921);
										v890 = v1178 + v921;
										BYTE1(v885) += BYTE2(v1135) + v180;
										LOWORD(v886) = v922 - 1;
										if (!(x_WORD)v886)
											break;
										v887 += 16;
									}
									v883 = v1287;
								}
							}
						LABEL_1093:
							v883 += 10;
							if (!--triLn_v1123)
								return;
						}
						if (v886 > viewPortWidth)
							LOWORD(v886) = viewPortWidth;
						v18 = __OFSUB__((x_WORD)v886, (x_WORD)v884);
						LOWORD(v886) = v886 - v884;
						if ((unsigned __int8)(((v886 & 0x8000u) != 0) ^ v18) | ((x_WORD)v886 == 0))
							goto LABEL_1093;
						v887 += v884;
						v890 = __ROL4__(*((x_DWORD*)v883 + 3), 16);
						BYTE1(v885) = v890;
						LOWORD(v890) = v883[4];
						LOBYTE(v885) = *((x_BYTE*)v883 + 10);
						goto LABEL_1042;
					case 0x17:
						v923 = (unsigned __int16*)unk_DE56Cx;
						v1179 = v1135 << 16;
						HIWORD(v924) = 0;
						HIWORD(v925) = 0;
						while (1)
						{
							LOWORD(v924) = v923[1];
							v926 = v923[3];
							v927 = (x_BYTE*)(pitch + v1102);
							v1102 += pitch;
							line22++;
							if (line22 >= drawEveryNthLine)
							{
								line22 = 0;
								if ((v924 & 0x8000u) == 0)
									break;
								if ((signed __int16)v926 > 0)
								{
									v928 = (unsigned __int16)-(signed __int16)v924;
									v930 = __ROL4__(*((x_DWORD*)v923 + 3) + v1135 * v928, 16);
									BYTE1(v925) = v930;
									v929 = *((x_DWORD*)v923 + 2) + v1124 * v928;
									LOWORD(v930) = v929;
									v931 = v929 >> 8;
									LOBYTE(v925) = BYTE1(v931);
									if (v926 > viewPortWidth)
										LOWORD(v926) = viewPortWidth;
									v924 = (unsigned __int16)v931;
								LABEL_1105:
									v1288 = v923;
									v932 = pTexture;
									while (1)
									{
										LOBYTE(v924) = *(x_BYTE*)(v925 + v932);
										if ((x_BYTE)v924)
										{
											BYTE1(v924) = *v927;
											*v927 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v924];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v930);
										LOWORD(v930) = v1124 + v930;
										LOBYTE(v925) = BYTE2(v1124) + v180 + v925;
										v180 = __CFADD__(v1179, v930);
										v933 = v1179 + v930;
										BYTE1(v925) += BYTE2(v1135) + v180;
										v934 = v926 - 1;
										if (!v934)
											break;
										LOBYTE(v924) = *(x_BYTE*)(v925 + v932);
										if ((x_BYTE)v924)
										{
											BYTE1(v924) = v927[1];
											v927[1] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v924];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v933);
										LOWORD(v933) = v1124 + v933;
										LOBYTE(v925) = BYTE2(v1124) + v180 + v925;
										v180 = __CFADD__(v1179, v933);
										v935 = v1179 + v933;
										BYTE1(v925) += BYTE2(v1135) + v180;
										v936 = v934 - 1;
										if (!v936)
											break;
										LOBYTE(v924) = *(x_BYTE*)(v925 + v932);
										if ((x_BYTE)v924)
										{
											BYTE1(v924) = v927[2];
											v927[2] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v924];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v935);
										LOWORD(v935) = v1124 + v935;
										LOBYTE(v925) = BYTE2(v1124) + v180 + v925;
										v180 = __CFADD__(v1179, v935);
										v937 = v1179 + v935;
										BYTE1(v925) += BYTE2(v1135) + v180;
										v938 = v936 - 1;
										if (!v938)
											break;
										LOBYTE(v924) = *(x_BYTE*)(v925 + v932);
										if ((x_BYTE)v924)
										{
											BYTE1(v924) = v927[3];
											v927[3] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v924];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v937);
										LOWORD(v937) = v1124 + v937;
										LOBYTE(v925) = BYTE2(v1124) + v180 + v925;
										v180 = __CFADD__(v1179, v937);
										v939 = v1179 + v937;
										BYTE1(v925) += BYTE2(v1135) + v180;
										v940 = v938 - 1;
										if (!v940)
											break;
										LOBYTE(v924) = *(x_BYTE*)(v925 + v932);
										if ((x_BYTE)v924)
										{
											BYTE1(v924) = v927[4];
											v927[4] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v924];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v939);
										LOWORD(v939) = v1124 + v939;
										LOBYTE(v925) = BYTE2(v1124) + v180 + v925;
										v180 = __CFADD__(v1179, v939);
										v941 = v1179 + v939;
										BYTE1(v925) += BYTE2(v1135) + v180;
										v942 = v940 - 1;
										if (!v942)
											break;
										LOBYTE(v924) = *(x_BYTE*)(v925 + v932);
										if ((x_BYTE)v924)
										{
											BYTE1(v924) = v927[5];
											v927[5] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v924];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v941);
										LOWORD(v941) = v1124 + v941;
										LOBYTE(v925) = BYTE2(v1124) + v180 + v925;
										v180 = __CFADD__(v1179, v941);
										v943 = v1179 + v941;
										BYTE1(v925) += BYTE2(v1135) + v180;
										v944 = v942 - 1;
										if (!v944)
											break;
										LOBYTE(v924) = *(x_BYTE*)(v925 + v932);
										if ((x_BYTE)v924)
										{
											BYTE1(v924) = v927[6];
											v927[6] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v924];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v943);
										LOWORD(v943) = v1124 + v943;
										LOBYTE(v925) = BYTE2(v1124) + v180 + v925;
										v180 = __CFADD__(v1179, v943);
										v945 = v1179 + v943;
										BYTE1(v925) += BYTE2(v1135) + v180;
										v946 = v944 - 1;
										if (!v946)
											break;
										LOBYTE(v924) = *(x_BYTE*)(v925 + v932);
										if ((x_BYTE)v924)
										{
											BYTE1(v924) = v927[7];
											v927[7] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v924];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v945);
										LOWORD(v945) = v1124 + v945;
										LOBYTE(v925) = BYTE2(v1124) + v180 + v925;
										v180 = __CFADD__(v1179, v945);
										v947 = v1179 + v945;
										BYTE1(v925) += BYTE2(v1135) + v180;
										v948 = v946 - 1;
										if (!v948)
											break;
										LOBYTE(v924) = *(x_BYTE*)(v925 + v932);
										if ((x_BYTE)v924)
										{
											BYTE1(v924) = v927[8];
											v927[8] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v924];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v947);
										LOWORD(v947) = v1124 + v947;
										LOBYTE(v925) = BYTE2(v1124) + v180 + v925;
										v180 = __CFADD__(v1179, v947);
										v949 = v1179 + v947;
										BYTE1(v925) += BYTE2(v1135) + v180;
										v950 = v948 - 1;
										if (!v950)
											break;
										LOBYTE(v924) = *(x_BYTE*)(v925 + v932);
										if ((x_BYTE)v924)
										{
											BYTE1(v924) = v927[9];
											v927[9] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v924];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v949);
										LOWORD(v949) = v1124 + v949;
										LOBYTE(v925) = BYTE2(v1124) + v180 + v925;
										v180 = __CFADD__(v1179, v949);
										v951 = v1179 + v949;
										BYTE1(v925) += BYTE2(v1135) + v180;
										v952 = v950 - 1;
										if (!v952)
											break;
										LOBYTE(v924) = *(x_BYTE*)(v925 + v932);
										if ((x_BYTE)v924)
										{
											BYTE1(v924) = v927[10];
											v927[10] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v924];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v951);
										LOWORD(v951) = v1124 + v951;
										LOBYTE(v925) = BYTE2(v1124) + v180 + v925;
										v180 = __CFADD__(v1179, v951);
										v953 = v1179 + v951;
										BYTE1(v925) += BYTE2(v1135) + v180;
										v954 = v952 - 1;
										if (!v954)
											break;
										LOBYTE(v924) = *(x_BYTE*)(v925 + v932);
										if ((x_BYTE)v924)
										{
											BYTE1(v924) = v927[11];
											v927[11] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v924];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v953);
										LOWORD(v953) = v1124 + v953;
										LOBYTE(v925) = BYTE2(v1124) + v180 + v925;
										v180 = __CFADD__(v1179, v953);
										v955 = v1179 + v953;
										BYTE1(v925) += BYTE2(v1135) + v180;
										v956 = v954 - 1;
										if (!v956)
											break;
										LOBYTE(v924) = *(x_BYTE*)(v925 + v932);
										if ((x_BYTE)v924)
										{
											BYTE1(v924) = v927[12];
											v927[12] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v924];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v955);
										LOWORD(v955) = v1124 + v955;
										LOBYTE(v925) = BYTE2(v1124) + v180 + v925;
										v180 = __CFADD__(v1179, v955);
										v957 = v1179 + v955;
										BYTE1(v925) += BYTE2(v1135) + v180;
										v958 = v956 - 1;
										if (!v958)
											break;
										LOBYTE(v924) = *(x_BYTE*)(v925 + v932);
										if ((x_BYTE)v924)
										{
											BYTE1(v924) = v927[13];
											v927[13] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v924];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v957);
										LOWORD(v957) = v1124 + v957;
										LOBYTE(v925) = BYTE2(v1124) + v180 + v925;
										v180 = __CFADD__(v1179, v957);
										v959 = v1179 + v957;
										BYTE1(v925) += BYTE2(v1135) + v180;
										v960 = v958 - 1;
										if (!v960)
											break;
										LOBYTE(v924) = *(x_BYTE*)(v925 + v932);
										if ((x_BYTE)v924)
										{
											BYTE1(v924) = v927[14];
											v927[14] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v924];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v959);
										LOWORD(v959) = v1124 + v959;
										LOBYTE(v925) = BYTE2(v1124) + v180 + v925;
										v180 = __CFADD__(v1179, v959);
										v961 = v1179 + v959;
										BYTE1(v925) += BYTE2(v1135) + v180;
										v962 = v960 - 1;
										if (!v962)
											break;
										LOBYTE(v924) = *(x_BYTE*)(v925 + v932);
										if ((x_BYTE)v924)
										{
											BYTE1(v924) = v927[15];
											v927[15] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v924];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v961);
										LOWORD(v961) = v1124 + v961;
										LOBYTE(v925) = BYTE2(v1124) + v180 + v925;
										v180 = __CFADD__(v1179, v961);
										v930 = v1179 + v961;
										BYTE1(v925) += BYTE2(v1135) + v180;
										LOWORD(v926) = v962 - 1;
										if (!(x_WORD)v926)
											break;
										v927 += 16;
									}
									v923 = v1288;
								}
							}
						LABEL_1156:
							v923 += 10;
							if (!--triLn_v1123)
								return;
						}
						if (v926 > viewPortWidth)
							LOWORD(v926) = viewPortWidth;
						v18 = __OFSUB__((x_WORD)v926, (x_WORD)v924);
						LOWORD(v926) = v926 - v924;
						if ((unsigned __int8)(((v926 & 0x8000u) != 0) ^ v18) | ((x_WORD)v926 == 0))
							goto LABEL_1156;
						v927 += v924;
						v930 = __ROL4__(*((x_DWORD*)v923 + 3), 16);
						BYTE1(v925) = v930;
						LOWORD(v930) = v923[4];
						LOBYTE(v925) = *((x_BYTE*)v923 + 10);
						goto LABEL_1105;
					case 0x18:
						v963 = (unsigned __int16*)unk_DE56Cx;
						v1180 = v1135 << 16;
						v1187 = v1146 << 16;
						HIWORD(v964) = 0;
						HIWORD(v965) = 0;
						while (1)
						{
							LOWORD(v964) = v963[1];
							v966 = v963[3];
							v967 = (x_BYTE*)(pitch + v1102);
							v1102 += pitch;
							line23++;
							if (line23 >= drawEveryNthLine)
							{
								line23 = 0;
								if ((v964 & 0x8000u) == 0)
									break;
								if ((signed __int16)v966 > 0)
								{
									if (v966 > viewPortWidth)
										v966 = viewPortWidth;
									v1226 = v966;
									v968 = (unsigned __int16)-(signed __int16)v964;
									v969 = v968;
									v971 = __ROL4__(*((x_DWORD*)v963 + 3) + v1135 * v968, 16);
									BYTE1(v965) = v971;
									v970 = *((x_DWORD*)v963 + 2) + v1124 * v968;
									LOWORD(v971) = v970;
									v964 = v970 >> 8;
									LOBYTE(v965) = BYTE1(v964);
									v972 = __ROL4__(*((x_DWORD*)v963 + 4) + v1146 * v969, 16);
									v964 = (unsigned __int16)v964;
								LABEL_1168:
									v1289 = v963;
									v973 = pTexture;
									while (1)
									{
										LOBYTE(v964) = *(x_BYTE*)(v965 + v973);
										if ((x_BYTE)v964)
										{
											BYTE1(v964) = v972;
											BYTE1(v964) = m_ptrX_BYTE_F6EE0_tablesx[v964];
											LOBYTE(v964) = *v967;
											*v967 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v964];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v971);
										LOWORD(v971) = v1124 + v971;
										LOBYTE(v965) = BYTE2(v1124) + v180 + v965;
										v180 = __CFADD__(v1180, v971);
										v974 = v1180 + v971;
										BYTE1(v965) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1187, v972);
										v975 = v1187 + v972;
										LOBYTE(v975) = BYTE2(v1146) + v180 + v975;
										v1227 = v1226 - 1;
										if (!v1227)
											break;
										LOBYTE(v964) = *(x_BYTE*)(v965 + v973);
										if ((x_BYTE)v964)
										{
											BYTE1(v964) = v975;
											BYTE1(v964) = m_ptrX_BYTE_F6EE0_tablesx[v964];
											LOBYTE(v964) = v967[1];
											v967[1] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v964];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v974);
										LOWORD(v974) = v1124 + v974;
										LOBYTE(v965) = BYTE2(v1124) + v180 + v965;
										v180 = __CFADD__(v1180, v974);
										v976 = v1180 + v974;
										BYTE1(v965) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1187, v975);
										v977 = v1187 + v975;
										LOBYTE(v977) = BYTE2(v1146) + v180 + v977;
										v1228 = v1227 - 1;
										if (!v1228)
											break;
										LOBYTE(v964) = *(x_BYTE*)(v965 + v973);
										if ((x_BYTE)v964)
										{
											BYTE1(v964) = v977;
											BYTE1(v964) = m_ptrX_BYTE_F6EE0_tablesx[v964];
											LOBYTE(v964) = v967[2];
											v967[2] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v964];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v976);
										LOWORD(v976) = v1124 + v976;
										LOBYTE(v965) = BYTE2(v1124) + v180 + v965;
										v180 = __CFADD__(v1180, v976);
										v978 = v1180 + v976;
										BYTE1(v965) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1187, v977);
										v979 = v1187 + v977;
										LOBYTE(v979) = BYTE2(v1146) + v180 + v979;
										v1229 = v1228 - 1;
										if (!v1229)
											break;
										LOBYTE(v964) = *(x_BYTE*)(v965 + v973);
										if ((x_BYTE)v964)
										{
											BYTE1(v964) = v979;
											BYTE1(v964) = m_ptrX_BYTE_F6EE0_tablesx[v964];
											LOBYTE(v964) = v967[3];
											v967[3] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v964];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v978);
										LOWORD(v978) = v1124 + v978;
										LOBYTE(v965) = BYTE2(v1124) + v180 + v965;
										v180 = __CFADD__(v1180, v978);
										v980 = v1180 + v978;
										BYTE1(v965) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1187, v979);
										v981 = v1187 + v979;
										LOBYTE(v981) = BYTE2(v1146) + v180 + v981;
										v1230 = v1229 - 1;
										if (!v1230)
											break;
										LOBYTE(v964) = *(x_BYTE*)(v965 + v973);
										if ((x_BYTE)v964)
										{
											BYTE1(v964) = v981;
											BYTE1(v964) = m_ptrX_BYTE_F6EE0_tablesx[v964];
											LOBYTE(v964) = v967[4];
											v967[4] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v964];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v980);
										LOWORD(v980) = v1124 + v980;
										LOBYTE(v965) = BYTE2(v1124) + v180 + v965;
										v180 = __CFADD__(v1180, v980);
										v982 = v1180 + v980;
										BYTE1(v965) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1187, v981);
										v983 = v1187 + v981;
										LOBYTE(v983) = BYTE2(v1146) + v180 + v983;
										v1231 = v1230 - 1;
										if (!v1231)
											break;
										LOBYTE(v964) = *(x_BYTE*)(v965 + v973);
										if ((x_BYTE)v964)
										{
											BYTE1(v964) = v983;
											BYTE1(v964) = m_ptrX_BYTE_F6EE0_tablesx[v964];
											LOBYTE(v964) = v967[5];
											v967[5] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v964];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v982);
										LOWORD(v982) = v1124 + v982;
										LOBYTE(v965) = BYTE2(v1124) + v180 + v965;
										v180 = __CFADD__(v1180, v982);
										v984 = v1180 + v982;
										BYTE1(v965) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1187, v983);
										v985 = v1187 + v983;
										LOBYTE(v985) = BYTE2(v1146) + v180 + v985;
										v1232 = v1231 - 1;
										if (!v1232)
											break;
										LOBYTE(v964) = *(x_BYTE*)(v965 + v973);
										if ((x_BYTE)v964)
										{
											BYTE1(v964) = v985;
											BYTE1(v964) = m_ptrX_BYTE_F6EE0_tablesx[v964];
											LOBYTE(v964) = v967[6];
											v967[6] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v964];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v984);
										LOWORD(v984) = v1124 + v984;
										LOBYTE(v965) = BYTE2(v1124) + v180 + v965;
										v180 = __CFADD__(v1180, v984);
										v986 = v1180 + v984;
										BYTE1(v965) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1187, v985);
										v987 = v1187 + v985;
										LOBYTE(v987) = BYTE2(v1146) + v180 + v987;
										v1233 = v1232 - 1;
										if (!v1233)
											break;
										LOBYTE(v964) = *(x_BYTE*)(v965 + v973);
										if ((x_BYTE)v964)
										{
											BYTE1(v964) = v987;
											BYTE1(v964) = m_ptrX_BYTE_F6EE0_tablesx[v964];
											LOBYTE(v964) = v967[7];
											v967[7] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v964];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v986);
										LOWORD(v986) = v1124 + v986;
										LOBYTE(v965) = BYTE2(v1124) + v180 + v965;
										v180 = __CFADD__(v1180, v986);
										v988 = v1180 + v986;
										BYTE1(v965) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1187, v987);
										v989 = v1187 + v987;
										LOBYTE(v989) = BYTE2(v1146) + v180 + v989;
										v1234 = v1233 - 1;
										if (!v1234)
											break;
										LOBYTE(v964) = *(x_BYTE*)(v965 + v973);
										if ((x_BYTE)v964)
										{
											BYTE1(v964) = v989;
											BYTE1(v964) = m_ptrX_BYTE_F6EE0_tablesx[v964];
											LOBYTE(v964) = v967[8];
											v967[8] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v964];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v988);
										LOWORD(v988) = v1124 + v988;
										LOBYTE(v965) = BYTE2(v1124) + v180 + v965;
										v180 = __CFADD__(v1180, v988);
										v990 = v1180 + v988;
										BYTE1(v965) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1187, v989);
										v991 = v1187 + v989;
										LOBYTE(v991) = BYTE2(v1146) + v180 + v991;
										v1235 = v1234 - 1;
										if (!v1235)
											break;
										LOBYTE(v964) = *(x_BYTE*)(v965 + v973);
										if ((x_BYTE)v964)
										{
											BYTE1(v964) = v991;
											BYTE1(v964) = m_ptrX_BYTE_F6EE0_tablesx[v964];
											LOBYTE(v964) = v967[9];
											v967[9] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v964];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v990);
										LOWORD(v990) = v1124 + v990;
										LOBYTE(v965) = BYTE2(v1124) + v180 + v965;
										v180 = __CFADD__(v1180, v990);
										v992 = v1180 + v990;
										BYTE1(v965) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1187, v991);
										v993 = v1187 + v991;
										LOBYTE(v993) = BYTE2(v1146) + v180 + v993;
										v1236 = v1235 - 1;
										if (!v1236)
											break;
										LOBYTE(v964) = *(x_BYTE*)(v965 + v973);
										if ((x_BYTE)v964)
										{
											BYTE1(v964) = v993;
											BYTE1(v964) = m_ptrX_BYTE_F6EE0_tablesx[v964];
											LOBYTE(v964) = v967[10];
											v967[10] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v964];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v992);
										LOWORD(v992) = v1124 + v992;
										LOBYTE(v965) = BYTE2(v1124) + v180 + v965;
										v180 = __CFADD__(v1180, v992);
										v994 = v1180 + v992;
										BYTE1(v965) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1187, v993);
										v995 = v1187 + v993;
										LOBYTE(v995) = BYTE2(v1146) + v180 + v995;
										v1237 = v1236 - 1;
										if (!v1237)
											break;
										LOBYTE(v964) = *(x_BYTE*)(v965 + v973);
										if ((x_BYTE)v964)
										{
											BYTE1(v964) = v995;
											BYTE1(v964) = m_ptrX_BYTE_F6EE0_tablesx[v964];
											LOBYTE(v964) = v967[11];
											v967[11] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v964];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v994);
										LOWORD(v994) = v1124 + v994;
										LOBYTE(v965) = BYTE2(v1124) + v180 + v965;
										v180 = __CFADD__(v1180, v994);
										v996 = v1180 + v994;
										BYTE1(v965) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1187, v995);
										v997 = v1187 + v995;
										LOBYTE(v997) = BYTE2(v1146) + v180 + v997;
										v1238 = v1237 - 1;
										if (!v1238)
											break;
										LOBYTE(v964) = *(x_BYTE*)(v965 + v973);
										if ((x_BYTE)v964)
										{
											BYTE1(v964) = v997;
											BYTE1(v964) = m_ptrX_BYTE_F6EE0_tablesx[v964];
											LOBYTE(v964) = v967[12];
											v967[12] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v964];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v996);
										LOWORD(v996) = v1124 + v996;
										LOBYTE(v965) = BYTE2(v1124) + v180 + v965;
										v180 = __CFADD__(v1180, v996);
										v998 = v1180 + v996;
										BYTE1(v965) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1187, v997);
										v999 = v1187 + v997;
										LOBYTE(v999) = BYTE2(v1146) + v180 + v999;
										v1239 = v1238 - 1;
										if (!v1239)
											break;
										LOBYTE(v964) = *(x_BYTE*)(v965 + v973);
										if ((x_BYTE)v964)
										{
											BYTE1(v964) = v999;
											BYTE1(v964) = m_ptrX_BYTE_F6EE0_tablesx[v964];
											LOBYTE(v964) = v967[13];
											v967[13] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v964];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v998);
										LOWORD(v998) = v1124 + v998;
										LOBYTE(v965) = BYTE2(v1124) + v180 + v965;
										v180 = __CFADD__(v1180, v998);
										v1000 = v1180 + v998;
										BYTE1(v965) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1187, v999);
										v1001 = v1187 + v999;
										LOBYTE(v1001) = BYTE2(v1146) + v180 + v1001;
										v1240 = v1239 - 1;
										if (!v1240)
											break;
										LOBYTE(v964) = *(x_BYTE*)(v965 + v973);
										if ((x_BYTE)v964)
										{
											BYTE1(v964) = v1001;
											BYTE1(v964) = m_ptrX_BYTE_F6EE0_tablesx[v964];
											LOBYTE(v964) = v967[14];
											v967[14] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v964];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1000);
										LOWORD(v1000) = v1124 + v1000;
										LOBYTE(v965) = BYTE2(v1124) + v180 + v965;
										v180 = __CFADD__(v1180, v1000);
										v1002 = v1180 + v1000;
										BYTE1(v965) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1187, v1001);
										v1003 = v1187 + v1001;
										LOBYTE(v1003) = BYTE2(v1146) + v180 + v1003;
										v1241 = v1240 - 1;
										if (!v1241)
											break;
										LOBYTE(v964) = *(x_BYTE*)(v965 + v973);
										if ((x_BYTE)v964)
										{
											BYTE1(v964) = v1003;
											BYTE1(v964) = m_ptrX_BYTE_F6EE0_tablesx[v964];
											LOBYTE(v964) = v967[15];
											v967[15] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v964];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1002);
										LOWORD(v1002) = v1124 + v1002;
										LOBYTE(v965) = BYTE2(v1124) + v180 + v965;
										v180 = __CFADD__(v1180, v1002);
										v971 = v1180 + v1002;
										BYTE1(v965) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1187, v1003);
										v972 = v1187 + v1003;
										LOBYTE(v972) = BYTE2(v1146) + v180 + v972;
										v1226 = v1241 - 1;
										if (!v1226)
											break;
										v967 += 16;
									}
									v963 = v1289;
								}
							}
						LABEL_1219:
							v963 += 10;
							if (!--triLn_v1123)
								return;
						}
						if (v966 > viewPortWidth)
							v966 = viewPortWidth;
						v18 = __OFSUB__((x_WORD)v966, (x_WORD)v964);
						LOWORD(v966) = v966 - v964;
						if ((unsigned __int8)(((v966 & 0x8000u) != 0) ^ v18) | ((x_WORD)v966 == 0))
							goto LABEL_1219;
						v967 += v964;
						v971 = __ROL4__(*((x_DWORD*)v963 + 3), 16);
						BYTE1(v965) = v971;
						LOWORD(v971) = v963[4];
						LOBYTE(v965) = *((x_BYTE*)v963 + 10);
						v1226 = v966;
						v972 = __ROL4__(*((x_DWORD*)v963 + 4), 16);
						goto LABEL_1168;
					case 0x19:
						v1004 = (unsigned __int16*)unk_DE56Cx;
						v1181 = v1135 << 16;
						v1188 = v1146 << 16;
						HIWORD(v1005) = 0;
						HIWORD(v1006) = 0;
						while (1)
						{
							LOWORD(v1005) = v1004[1];
							v1007 = v1004[3];
							v1008 = (x_BYTE*)(pitch + v1102);
							v1102 += pitch;
							line24++;
							if (line24 >= drawEveryNthLine)
							{
								line24 = 0;
								if ((v1005 & 0x8000u) == 0)
									break;
								if ((signed __int16)v1007 > 0)
								{
									if (v1007 > viewPortWidth)
										v1007 = viewPortWidth;
									v1242 = v1007;
									v1009 = (unsigned __int16)-(signed __int16)v1005;
									v1010 = v1009;
									v1012 = __ROL4__(*((x_DWORD*)v1004 + 3) + v1135 * v1009, 16);
									BYTE1(v1006) = v1012;
									v1011 = *((x_DWORD*)v1004 + 2) + v1124 * v1009;
									LOWORD(v1012) = v1011;
									v1005 = v1011 >> 8;
									LOBYTE(v1006) = BYTE1(v1005);
									v1013 = __ROL4__(*((x_DWORD*)v1004 + 4) + v1146 * v1010, 16);
									v1005 = (unsigned __int16)v1005;
								LABEL_1231:
									v1290 = v1004;
									v1014 = pTexture;
									while (1)
									{
										LOBYTE(v1005) = *(x_BYTE*)(v1006 + v1014);
										if ((x_BYTE)v1005)
										{
											BYTE1(v1005) = v1013;
											LOBYTE(v1005) = m_ptrX_BYTE_F6EE0_tablesx[v1005];
											BYTE1(v1005) = *v1008;
											*v1008 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1005];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1012);
										LOWORD(v1012) = v1124 + v1012;
										LOBYTE(v1006) = BYTE2(v1124) + v180 + v1006;
										v180 = __CFADD__(v1181, v1012);
										v1015 = v1181 + v1012;
										BYTE1(v1006) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1188, v1013);
										v1016 = v1188 + v1013;
										LOBYTE(v1016) = BYTE2(v1146) + v180 + v1016;
										v1243 = v1242 - 1;
										if (!v1243)
											break;
										LOBYTE(v1005) = *(x_BYTE*)(v1006 + v1014);
										if ((x_BYTE)v1005)
										{
											BYTE1(v1005) = v1016;
											LOBYTE(v1005) = m_ptrX_BYTE_F6EE0_tablesx[v1005];
											BYTE1(v1005) = v1008[1];
											v1008[1] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1005];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1015);
										LOWORD(v1015) = v1124 + v1015;
										LOBYTE(v1006) = BYTE2(v1124) + v180 + v1006;
										v180 = __CFADD__(v1181, v1015);
										v1017 = v1181 + v1015;
										BYTE1(v1006) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1188, v1016);
										v1018 = v1188 + v1016;
										LOBYTE(v1018) = BYTE2(v1146) + v180 + v1018;
										v1244 = v1243 - 1;
										if (!v1244)
											break;
										LOBYTE(v1005) = *(x_BYTE*)(v1006 + v1014);
										if ((x_BYTE)v1005)
										{
											BYTE1(v1005) = v1018;
											LOBYTE(v1005) = m_ptrX_BYTE_F6EE0_tablesx[v1005];
											BYTE1(v1005) = v1008[2];
											v1008[2] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1005];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1017);
										LOWORD(v1017) = v1124 + v1017;
										LOBYTE(v1006) = BYTE2(v1124) + v180 + v1006;
										v180 = __CFADD__(v1181, v1017);
										v1019 = v1181 + v1017;
										BYTE1(v1006) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1188, v1018);
										v1020 = v1188 + v1018;
										LOBYTE(v1020) = BYTE2(v1146) + v180 + v1020;
										v1245 = v1244 - 1;
										if (!v1245)
											break;
										LOBYTE(v1005) = *(x_BYTE*)(v1006 + v1014);
										if ((x_BYTE)v1005)
										{
											BYTE1(v1005) = v1020;
											LOBYTE(v1005) = m_ptrX_BYTE_F6EE0_tablesx[v1005];
											BYTE1(v1005) = v1008[3];
											v1008[3] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1005];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1019);
										LOWORD(v1019) = v1124 + v1019;
										LOBYTE(v1006) = BYTE2(v1124) + v180 + v1006;
										v180 = __CFADD__(v1181, v1019);
										v1021 = v1181 + v1019;
										BYTE1(v1006) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1188, v1020);
										v1022 = v1188 + v1020;
										LOBYTE(v1022) = BYTE2(v1146) + v180 + v1022;
										v1246 = v1245 - 1;
										if (!v1246)
											break;
										LOBYTE(v1005) = *(x_BYTE*)(v1006 + v1014);
										if ((x_BYTE)v1005)
										{
											BYTE1(v1005) = v1022;
											LOBYTE(v1005) = m_ptrX_BYTE_F6EE0_tablesx[v1005];
											BYTE1(v1005) = v1008[4];
											v1008[4] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1005];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1021);
										LOWORD(v1021) = v1124 + v1021;
										LOBYTE(v1006) = BYTE2(v1124) + v180 + v1006;
										v180 = __CFADD__(v1181, v1021);
										v1023 = v1181 + v1021;
										BYTE1(v1006) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1188, v1022);
										v1024 = v1188 + v1022;
										LOBYTE(v1024) = BYTE2(v1146) + v180 + v1024;
										v1247 = v1246 - 1;
										if (!v1247)
											break;
										LOBYTE(v1005) = *(x_BYTE*)(v1006 + v1014);
										if ((x_BYTE)v1005)
										{
											BYTE1(v1005) = v1024;
											LOBYTE(v1005) = m_ptrX_BYTE_F6EE0_tablesx[v1005];
											BYTE1(v1005) = v1008[5];
											v1008[5] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1005];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1023);
										LOWORD(v1023) = v1124 + v1023;
										LOBYTE(v1006) = BYTE2(v1124) + v180 + v1006;
										v180 = __CFADD__(v1181, v1023);
										v1025 = v1181 + v1023;
										BYTE1(v1006) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1188, v1024);
										v1026 = v1188 + v1024;
										LOBYTE(v1026) = BYTE2(v1146) + v180 + v1026;
										v1248 = v1247 - 1;
										if (!v1248)
											break;
										LOBYTE(v1005) = *(x_BYTE*)(v1006 + v1014);
										if ((x_BYTE)v1005)
										{
											BYTE1(v1005) = v1026;
											LOBYTE(v1005) = m_ptrX_BYTE_F6EE0_tablesx[v1005];
											BYTE1(v1005) = v1008[6];
											v1008[6] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1005];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1025);
										LOWORD(v1025) = v1124 + v1025;
										LOBYTE(v1006) = BYTE2(v1124) + v180 + v1006;
										v180 = __CFADD__(v1181, v1025);
										v1027 = v1181 + v1025;
										BYTE1(v1006) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1188, v1026);
										v1028 = v1188 + v1026;
										LOBYTE(v1028) = BYTE2(v1146) + v180 + v1028;
										v1249 = v1248 - 1;
										if (!v1249)
											break;
										LOBYTE(v1005) = *(x_BYTE*)(v1006 + v1014);
										if ((x_BYTE)v1005)
										{
											BYTE1(v1005) = v1028;
											LOBYTE(v1005) = m_ptrX_BYTE_F6EE0_tablesx[v1005];
											BYTE1(v1005) = v1008[7];
											v1008[7] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1005];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1027);
										LOWORD(v1027) = v1124 + v1027;
										LOBYTE(v1006) = BYTE2(v1124) + v180 + v1006;
										v180 = __CFADD__(v1181, v1027);
										v1029 = v1181 + v1027;
										BYTE1(v1006) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1188, v1028);
										v1030 = v1188 + v1028;
										LOBYTE(v1030) = BYTE2(v1146) + v180 + v1030;
										v1250 = v1249 - 1;
										if (!v1250)
											break;
										LOBYTE(v1005) = *(x_BYTE*)(v1006 + v1014);
										if ((x_BYTE)v1005)
										{
											BYTE1(v1005) = v1030;
											LOBYTE(v1005) = m_ptrX_BYTE_F6EE0_tablesx[v1005];
											BYTE1(v1005) = v1008[8];
											v1008[8] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1005];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1029);
										LOWORD(v1029) = v1124 + v1029;
										LOBYTE(v1006) = BYTE2(v1124) + v180 + v1006;
										v180 = __CFADD__(v1181, v1029);
										v1031 = v1181 + v1029;
										BYTE1(v1006) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1188, v1030);
										v1032 = v1188 + v1030;
										LOBYTE(v1032) = BYTE2(v1146) + v180 + v1032;
										v1251 = v1250 - 1;
										if (!v1251)
											break;
										LOBYTE(v1005) = *(x_BYTE*)(v1006 + v1014);
										if ((x_BYTE)v1005)
										{
											BYTE1(v1005) = v1032;
											LOBYTE(v1005) = m_ptrX_BYTE_F6EE0_tablesx[v1005];
											BYTE1(v1005) = v1008[9];
											v1008[9] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1005];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1031);
										LOWORD(v1031) = v1124 + v1031;
										LOBYTE(v1006) = BYTE2(v1124) + v180 + v1006;
										v180 = __CFADD__(v1181, v1031);
										v1033 = v1181 + v1031;
										BYTE1(v1006) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1188, v1032);
										v1034 = v1188 + v1032;
										LOBYTE(v1034) = BYTE2(v1146) + v180 + v1034;
										v1252 = v1251 - 1;
										if (!v1252)
											break;
										LOBYTE(v1005) = *(x_BYTE*)(v1006 + v1014);
										if ((x_BYTE)v1005)
										{
											BYTE1(v1005) = v1034;
											LOBYTE(v1005) = m_ptrX_BYTE_F6EE0_tablesx[v1005];
											BYTE1(v1005) = v1008[10];
											v1008[10] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1005];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1033);
										LOWORD(v1033) = v1124 + v1033;
										LOBYTE(v1006) = BYTE2(v1124) + v180 + v1006;
										v180 = __CFADD__(v1181, v1033);
										v1035 = v1181 + v1033;
										BYTE1(v1006) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1188, v1034);
										v1036 = v1188 + v1034;
										LOBYTE(v1036) = BYTE2(v1146) + v180 + v1036;
										v1253 = v1252 - 1;
										if (!v1253)
											break;
										LOBYTE(v1005) = *(x_BYTE*)(v1006 + v1014);
										if ((x_BYTE)v1005)
										{
											BYTE1(v1005) = v1036;
											LOBYTE(v1005) = m_ptrX_BYTE_F6EE0_tablesx[v1005];
											BYTE1(v1005) = v1008[11];
											v1008[11] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1005];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1035);
										LOWORD(v1035) = v1124 + v1035;
										LOBYTE(v1006) = BYTE2(v1124) + v180 + v1006;
										v180 = __CFADD__(v1181, v1035);
										v1037 = v1181 + v1035;
										BYTE1(v1006) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1188, v1036);
										v1038 = v1188 + v1036;
										LOBYTE(v1038) = BYTE2(v1146) + v180 + v1038;
										v1254 = v1253 - 1;
										if (!v1254)
											break;
										LOBYTE(v1005) = *(x_BYTE*)(v1006 + v1014);
										if ((x_BYTE)v1005)
										{
											BYTE1(v1005) = v1038;
											LOBYTE(v1005) = m_ptrX_BYTE_F6EE0_tablesx[v1005];
											BYTE1(v1005) = v1008[12];
											v1008[12] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1005];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1037);
										LOWORD(v1037) = v1124 + v1037;
										LOBYTE(v1006) = BYTE2(v1124) + v180 + v1006;
										v180 = __CFADD__(v1181, v1037);
										v1039 = v1181 + v1037;
										BYTE1(v1006) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1188, v1038);
										v1040 = v1188 + v1038;
										LOBYTE(v1040) = BYTE2(v1146) + v180 + v1040;
										v1255 = v1254 - 1;
										if (!v1255)
											break;
										LOBYTE(v1005) = *(x_BYTE*)(v1006 + v1014);
										if ((x_BYTE)v1005)
										{
											BYTE1(v1005) = v1040;
											LOBYTE(v1005) = m_ptrX_BYTE_F6EE0_tablesx[v1005];
											BYTE1(v1005) = v1008[13];
											v1008[13] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1005];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1039);
										LOWORD(v1039) = v1124 + v1039;
										LOBYTE(v1006) = BYTE2(v1124) + v180 + v1006;
										v180 = __CFADD__(v1181, v1039);
										v1041 = v1181 + v1039;
										BYTE1(v1006) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1188, v1040);
										v1042 = v1188 + v1040;
										LOBYTE(v1042) = BYTE2(v1146) + v180 + v1042;
										v1256 = v1255 - 1;
										if (!v1256)
											break;
										LOBYTE(v1005) = *(x_BYTE*)(v1006 + v1014);
										if ((x_BYTE)v1005)
										{
											BYTE1(v1005) = v1042;
											LOBYTE(v1005) = m_ptrX_BYTE_F6EE0_tablesx[v1005];
											BYTE1(v1005) = v1008[14];
											v1008[14] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1005];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1041);
										LOWORD(v1041) = v1124 + v1041;
										LOBYTE(v1006) = BYTE2(v1124) + v180 + v1006;
										v180 = __CFADD__(v1181, v1041);
										v1043 = v1181 + v1041;
										BYTE1(v1006) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1188, v1042);
										v1044 = v1188 + v1042;
										LOBYTE(v1044) = BYTE2(v1146) + v180 + v1044;
										v1257 = v1256 - 1;
										if (!v1257)
											break;
										LOBYTE(v1005) = *(x_BYTE*)(v1006 + v1014);
										if ((x_BYTE)v1005)
										{
											BYTE1(v1005) = v1044;
											LOBYTE(v1005) = m_ptrX_BYTE_F6EE0_tablesx[v1005];
											BYTE1(v1005) = v1008[15];
											v1008[15] = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1005];
										}
										v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1043);
										LOWORD(v1043) = v1124 + v1043;
										LOBYTE(v1006) = BYTE2(v1124) + v180 + v1006;
										v180 = __CFADD__(v1181, v1043);
										v1012 = v1181 + v1043;
										BYTE1(v1006) += BYTE2(v1135) + v180;
										v180 = __CFADD__(v1188, v1044);
										v1013 = v1188 + v1044;
										LOBYTE(v1013) = BYTE2(v1146) + v180 + v1013;
										v1242 = v1257 - 1;
										if (!v1242)
											break;
										v1008 += 16;
									}
									v1004 = v1290;
								}
							}
						LABEL_1282:
							v1004 += 10;
							if (!--triLn_v1123)
								return;
						}
						if (v1007 > viewPortWidth)
							v1007 = viewPortWidth;
						v18 = __OFSUB__((x_WORD)v1007, (x_WORD)v1005);
						LOWORD(v1007) = v1007 - v1005;
						if ((unsigned __int8)(((v1007 & 0x8000u) != 0) ^ v18) | ((x_WORD)v1007 == 0))
							goto LABEL_1282;
						v1008 += v1005;
						v1012 = __ROL4__(*((x_DWORD*)v1004 + 3), 16);
						BYTE1(v1006) = v1012;
						LOWORD(v1012) = v1004[4];
						LOBYTE(v1006) = *((x_BYTE*)v1004 + 10);
						v1242 = v1007;
						v1013 = __ROL4__(*((x_DWORD*)v1004 + 4), 16);
						goto LABEL_1231;
					case 0x1A:
						v1045 = (unsigned __int16*)unk_DE56Cx;
						v1182 = v1135 << 16;
						v1189 = v1146 << 16;
						HIWORD(v1046) = 0;
						HIWORD(v1047) = 0;
						break;
					}
					while (1)
					{
						LOWORD(v1046) = v1045[1];
						v1048 = v1045[3];
						v1049 = (char*)(pitch + v1102);
						v1102 += pitch;
						line25++;
						if (line25 >= drawEveryNthLine)
						{
							line25 = 0;
							if ((v1046 & 0x8000u) == 0)
								break;
							if ((signed __int16)v1048 > 0)
							{
								if (v1048 > viewPortWidth)
									v1048 = viewPortWidth;
								v1258 = v1048;
								v1050 = (unsigned __int16)-(signed __int16)v1046;
								v1051 = v1050;
								v1053 = __ROL4__(*((x_DWORD*)v1045 + 3) + v1135 * v1050, 16);
								BYTE1(v1047) = v1053;
								v1052 = *((x_DWORD*)v1045 + 2) + v1124 * v1050;
								LOWORD(v1053) = v1052;
								v1046 = v1052 >> 8;
								LOBYTE(v1047) = BYTE1(v1046);
								v1054 = __ROL4__(*((x_DWORD*)v1045 + 4) + v1146 * v1051, 16);
								v1046 = (unsigned __int16)v1046;
							LABEL_1294:
								v1291 = v1045;
								v1055 = pTexture;
								while (1)
								{
									LOBYTE(v1046) = *(x_BYTE*)(v1047 + v1055);
									v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1053);
									LOWORD(v1053) = v1124 + v1053;
									BYTE1(v1046) = v1054;
									LOBYTE(v1047) = BYTE2(v1124) + v180 + v1047;
									if ((unsigned __int8)v1046 >= 0xCu)
									{
										v1056 = m_ptrX_BYTE_F6EE0_tablesx[v1046];
									}
									else
									{
										LOBYTE(v1046) = m_ptrX_BYTE_F6EE0_tablesx[v1046];
										BYTE1(v1046) = *v1049;
										v1056 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1046];
									}
									v180 = __CFADD__(v1182, v1053);
									v1057 = v1182 + v1053;
									BYTE1(v1047) += BYTE2(v1135) + v180;
									v180 = __CFADD__(v1189, v1054);
									v1058 = v1189 + v1054;
									*v1049 = v1056;
									LOBYTE(v1058) = BYTE2(v1146) + v180 + v1058;
									v1259 = v1258 - 1;
									if (!v1259)
										break;
									LOBYTE(v1046) = *(x_BYTE*)(v1047 + v1055);
									v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1057);
									LOWORD(v1057) = v1124 + v1057;
									BYTE1(v1046) = v1058;
									LOBYTE(v1047) = BYTE2(v1124) + v180 + v1047;
									if ((unsigned __int8)v1046 >= 0xCu)
									{
										v1059 = m_ptrX_BYTE_F6EE0_tablesx[v1046];
									}
									else
									{
										LOBYTE(v1046) = m_ptrX_BYTE_F6EE0_tablesx[v1046];
										BYTE1(v1046) = v1049[1];
										v1059 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1046];
									}
									v180 = __CFADD__(v1182, v1057);
									v1060 = v1182 + v1057;
									BYTE1(v1047) += BYTE2(v1135) + v180;
									v180 = __CFADD__(v1189, v1058);
									v1061 = v1189 + v1058;
									v1049[1] = v1059;
									LOBYTE(v1061) = BYTE2(v1146) + v180 + v1061;
									v1260 = v1259 - 1;
									if (!v1260)
										break;
									LOBYTE(v1046) = *(x_BYTE*)(v1047 + v1055);
									v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1060);
									LOWORD(v1060) = v1124 + v1060;
									BYTE1(v1046) = v1061;
									LOBYTE(v1047) = BYTE2(v1124) + v180 + v1047;
									if ((unsigned __int8)v1046 >= 0xCu)
									{
										v1062 = m_ptrX_BYTE_F6EE0_tablesx[v1046];
									}
									else
									{
										LOBYTE(v1046) = m_ptrX_BYTE_F6EE0_tablesx[v1046];
										BYTE1(v1046) = v1049[2];
										v1062 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1046];
									}
									v180 = __CFADD__(v1182, v1060);
									v1063 = v1182 + v1060;
									BYTE1(v1047) += BYTE2(v1135) + v180;
									v180 = __CFADD__(v1189, v1061);
									v1064 = v1189 + v1061;
									v1049[2] = v1062;
									LOBYTE(v1064) = BYTE2(v1146) + v180 + v1064;
									v1261 = v1260 - 1;
									if (!v1261)
										break;
									LOBYTE(v1046) = *(x_BYTE*)(v1047 + v1055);
									v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1063);
									LOWORD(v1063) = v1124 + v1063;
									BYTE1(v1046) = v1064;
									LOBYTE(v1047) = BYTE2(v1124) + v180 + v1047;
									if ((unsigned __int8)v1046 >= 0xCu)
									{
										v1065 = m_ptrX_BYTE_F6EE0_tablesx[v1046];
									}
									else
									{
										LOBYTE(v1046) = m_ptrX_BYTE_F6EE0_tablesx[v1046];
										BYTE1(v1046) = v1049[3];
										v1065 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1046];
									}
									v180 = __CFADD__(v1182, v1063);
									v1066 = v1182 + v1063;
									BYTE1(v1047) += BYTE2(v1135) + v180;
									v180 = __CFADD__(v1189, v1064);
									v1067 = v1189 + v1064;
									v1049[3] = v1065;
									LOBYTE(v1067) = BYTE2(v1146) + v180 + v1067;
									v1262 = v1261 - 1;
									if (!v1262)
										break;
									LOBYTE(v1046) = *(x_BYTE*)(v1047 + v1055);
									v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1066);
									LOWORD(v1066) = v1124 + v1066;
									BYTE1(v1046) = v1067;
									LOBYTE(v1047) = BYTE2(v1124) + v180 + v1047;
									if ((unsigned __int8)v1046 >= 0xCu)
									{
										v1068 = m_ptrX_BYTE_F6EE0_tablesx[v1046];
									}
									else
									{
										LOBYTE(v1046) = m_ptrX_BYTE_F6EE0_tablesx[v1046];
										BYTE1(v1046) = v1049[4];
										v1068 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1046];
									}
									v180 = __CFADD__(v1182, v1066);
									v1069 = v1182 + v1066;
									BYTE1(v1047) += BYTE2(v1135) + v180;
									v180 = __CFADD__(v1189, v1067);
									v1070 = v1189 + v1067;
									v1049[4] = v1068;
									LOBYTE(v1070) = BYTE2(v1146) + v180 + v1070;
									v1263 = v1262 - 1;
									if (!v1263)
										break;
									LOBYTE(v1046) = *(x_BYTE*)(v1047 + v1055);
									v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1069);
									LOWORD(v1069) = v1124 + v1069;
									BYTE1(v1046) = v1070;
									LOBYTE(v1047) = BYTE2(v1124) + v180 + v1047;
									if ((unsigned __int8)v1046 >= 0xCu)
									{
										v1071 = m_ptrX_BYTE_F6EE0_tablesx[v1046];
									}
									else
									{
										LOBYTE(v1046) = m_ptrX_BYTE_F6EE0_tablesx[v1046];
										BYTE1(v1046) = v1049[5];
										v1071 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1046];
									}
									v180 = __CFADD__(v1182, v1069);
									v1072 = v1182 + v1069;
									BYTE1(v1047) += BYTE2(v1135) + v180;
									v180 = __CFADD__(v1189, v1070);
									v1073 = v1189 + v1070;
									v1049[5] = v1071;
									LOBYTE(v1073) = BYTE2(v1146) + v180 + v1073;
									v1264 = v1263 - 1;
									if (!v1264)
										break;
									LOBYTE(v1046) = *(x_BYTE*)(v1047 + v1055);
									v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1072);
									LOWORD(v1072) = v1124 + v1072;
									BYTE1(v1046) = v1073;
									LOBYTE(v1047) = BYTE2(v1124) + v180 + v1047;
									if ((unsigned __int8)v1046 >= 0xCu)
									{
										v1074 = m_ptrX_BYTE_F6EE0_tablesx[v1046];
									}
									else
									{
										LOBYTE(v1046) = m_ptrX_BYTE_F6EE0_tablesx[v1046];
										BYTE1(v1046) = v1049[6];
										v1074 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1046];
									}
									v180 = __CFADD__(v1182, v1072);
									v1075 = v1182 + v1072;
									BYTE1(v1047) += BYTE2(v1135) + v180;
									v180 = __CFADD__(v1189, v1073);
									v1076 = v1189 + v1073;
									v1049[6] = v1074;
									LOBYTE(v1076) = BYTE2(v1146) + v180 + v1076;
									v1265 = v1264 - 1;
									if (!v1265)
										break;
									LOBYTE(v1046) = *(x_BYTE*)(v1047 + v1055);
									v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1075);
									LOWORD(v1075) = v1124 + v1075;
									BYTE1(v1046) = v1076;
									LOBYTE(v1047) = BYTE2(v1124) + v180 + v1047;
									if ((unsigned __int8)v1046 >= 0xCu)
									{
										v1077 = m_ptrX_BYTE_F6EE0_tablesx[v1046];
									}
									else
									{
										LOBYTE(v1046) = m_ptrX_BYTE_F6EE0_tablesx[v1046];
										BYTE1(v1046) = v1049[7];
										v1077 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1046];
									}
									v180 = __CFADD__(v1182, v1075);
									v1078 = v1182 + v1075;
									BYTE1(v1047) += BYTE2(v1135) + v180;
									v180 = __CFADD__(v1189, v1076);
									v1079 = v1189 + v1076;
									v1049[7] = v1077;
									LOBYTE(v1079) = BYTE2(v1146) + v180 + v1079;
									v1266 = v1265 - 1;
									if (!v1266)
										break;
									LOBYTE(v1046) = *(x_BYTE*)(v1047 + v1055);
									v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1078);
									LOWORD(v1078) = v1124 + v1078;
									BYTE1(v1046) = v1079;
									LOBYTE(v1047) = BYTE2(v1124) + v180 + v1047;
									if ((unsigned __int8)v1046 >= 0xCu)
									{
										v1080 = m_ptrX_BYTE_F6EE0_tablesx[v1046];
									}
									else
									{
										LOBYTE(v1046) = m_ptrX_BYTE_F6EE0_tablesx[v1046];
										BYTE1(v1046) = v1049[8];
										v1080 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1046];
									}
									v180 = __CFADD__(v1182, v1078);
									v1081 = v1182 + v1078;
									BYTE1(v1047) += BYTE2(v1135) + v180;
									v180 = __CFADD__(v1189, v1079);
									v1082 = v1189 + v1079;
									v1049[8] = v1080;
									LOBYTE(v1082) = BYTE2(v1146) + v180 + v1082;
									v1267 = v1266 - 1;
									if (!v1267)
										break;
									LOBYTE(v1046) = *(x_BYTE*)(v1047 + v1055);
									v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1081);
									LOWORD(v1081) = v1124 + v1081;
									BYTE1(v1046) = v1082;
									LOBYTE(v1047) = BYTE2(v1124) + v180 + v1047;
									if ((unsigned __int8)v1046 >= 0xCu)
									{
										v1083 = m_ptrX_BYTE_F6EE0_tablesx[v1046];
									}
									else
									{
										LOBYTE(v1046) = m_ptrX_BYTE_F6EE0_tablesx[v1046];
										BYTE1(v1046) = v1049[9];
										v1083 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1046];
									}
									v180 = __CFADD__(v1182, v1081);
									v1084 = v1182 + v1081;
									BYTE1(v1047) += BYTE2(v1135) + v180;
									v180 = __CFADD__(v1189, v1082);
									v1085 = v1189 + v1082;
									v1049[9] = v1083;
									LOBYTE(v1085) = BYTE2(v1146) + v180 + v1085;
									v1268 = v1267 - 1;
									if (!v1268)
										break;
									LOBYTE(v1046) = *(x_BYTE*)(v1047 + v1055);
									v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1084);
									LOWORD(v1084) = v1124 + v1084;
									BYTE1(v1046) = v1085;
									LOBYTE(v1047) = BYTE2(v1124) + v180 + v1047;
									if ((unsigned __int8)v1046 >= 0xCu)
									{
										v1086 = m_ptrX_BYTE_F6EE0_tablesx[v1046];
									}
									else
									{
										LOBYTE(v1046) = m_ptrX_BYTE_F6EE0_tablesx[v1046];
										BYTE1(v1046) = v1049[10];
										v1086 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1046];
									}
									v180 = __CFADD__(v1182, v1084);
									v1087 = v1182 + v1084;
									BYTE1(v1047) += BYTE2(v1135) + v180;
									v180 = __CFADD__(v1189, v1085);
									v1088 = v1189 + v1085;
									v1049[10] = v1086;
									LOBYTE(v1088) = BYTE2(v1146) + v180 + v1088;
									v1269 = v1268 - 1;
									if (!v1269)
										break;
									LOBYTE(v1046) = *(x_BYTE*)(v1047 + v1055);
									v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1087);
									LOWORD(v1087) = v1124 + v1087;
									BYTE1(v1046) = v1088;
									LOBYTE(v1047) = BYTE2(v1124) + v180 + v1047;
									if ((unsigned __int8)v1046 >= 0xCu)
									{
										v1089 = m_ptrX_BYTE_F6EE0_tablesx[v1046];
									}
									else
									{
										LOBYTE(v1046) = m_ptrX_BYTE_F6EE0_tablesx[v1046];
										BYTE1(v1046) = v1049[11];
										v1089 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1046];
									}
									v180 = __CFADD__(v1182, v1087);
									v1090 = v1182 + v1087;
									BYTE1(v1047) += BYTE2(v1135) + v180;
									v180 = __CFADD__(v1189, v1088);
									v1091 = v1189 + v1088;
									v1049[11] = v1089;
									LOBYTE(v1091) = BYTE2(v1146) + v180 + v1091;
									v1270 = v1269 - 1;
									if (!v1270)
										break;
									LOBYTE(v1046) = *(x_BYTE*)(v1047 + v1055);
									v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1090);
									LOWORD(v1090) = v1124 + v1090;
									BYTE1(v1046) = v1091;
									LOBYTE(v1047) = BYTE2(v1124) + v180 + v1047;
									if ((unsigned __int8)v1046 >= 0xCu)
									{
										v1092 = m_ptrX_BYTE_F6EE0_tablesx[v1046];
									}
									else
									{
										LOBYTE(v1046) = m_ptrX_BYTE_F6EE0_tablesx[v1046];
										BYTE1(v1046) = v1049[12];
										v1092 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1046];
									}
									v180 = __CFADD__(v1182, v1090);
									v1093 = v1182 + v1090;
									BYTE1(v1047) += BYTE2(v1135) + v180;
									v180 = __CFADD__(v1189, v1091);
									v1094 = v1189 + v1091;
									v1049[12] = v1092;
									LOBYTE(v1094) = BYTE2(v1146) + v180 + v1094;
									v1271 = v1270 - 1;
									if (!v1271)
										break;
									LOBYTE(v1046) = *(x_BYTE*)(v1047 + v1055);
									v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1093);
									LOWORD(v1093) = v1124 + v1093;
									BYTE1(v1046) = v1094;
									LOBYTE(v1047) = BYTE2(v1124) + v180 + v1047;
									if ((unsigned __int8)v1046 >= 0xCu)
									{
										v1095 = m_ptrX_BYTE_F6EE0_tablesx[v1046];
									}
									else
									{
										LOBYTE(v1046) = m_ptrX_BYTE_F6EE0_tablesx[v1046];
										BYTE1(v1046) = v1049[13];
										v1095 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1046];
									}
									v180 = __CFADD__(v1182, v1093);
									v1096 = v1182 + v1093;
									BYTE1(v1047) += BYTE2(v1135) + v180;
									v180 = __CFADD__(v1189, v1094);
									v1097 = v1189 + v1094;
									v1049[13] = v1095;
									LOBYTE(v1097) = BYTE2(v1146) + v180 + v1097;
									v1272 = v1271 - 1;
									if (!v1272)
										break;
									LOBYTE(v1046) = *(x_BYTE*)(v1047 + v1055);
									v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1096);
									LOWORD(v1096) = v1124 + v1096;
									BYTE1(v1046) = v1097;
									LOBYTE(v1047) = BYTE2(v1124) + v180 + v1047;
									if ((unsigned __int8)v1046 >= 0xCu)
									{
										v1098 = m_ptrX_BYTE_F6EE0_tablesx[v1046];
									}
									else
									{
										LOBYTE(v1046) = m_ptrX_BYTE_F6EE0_tablesx[v1046];
										BYTE1(v1046) = v1049[14];
										v1098 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1046];
									}
									v180 = __CFADD__(v1182, v1096);
									v1099 = v1182 + v1096;
									BYTE1(v1047) += BYTE2(v1135) + v180;
									v180 = __CFADD__(v1189, v1097);
									v1100 = v1189 + v1097;
									v1049[14] = v1098;
									LOBYTE(v1100) = BYTE2(v1146) + v180 + v1100;
									v1273 = v1272 - 1;
									if (!v1273)
										break;
									LOBYTE(v1046) = *(x_BYTE*)(v1047 + v1055);
									v180 = __CFADD__((x_WORD)v1124, (x_WORD)v1099);
									LOWORD(v1099) = v1124 + v1099;
									BYTE1(v1046) = v1100;
									LOBYTE(v1047) = BYTE2(v1124) + v180 + v1047;
									if ((unsigned __int8)v1046 >= 0xCu)
									{
										v1101 = m_ptrX_BYTE_F6EE0_tablesx[v1046];
									}
									else
									{
										LOBYTE(v1046) = m_ptrX_BYTE_F6EE0_tablesx[v1046];
										BYTE1(v1046) = v1049[15];
										v1101 = m_ptrX_BYTE_F6EE0_tablesx[0x4000 + v1046];
									}
									v180 = __CFADD__(v1182, v1099);
									v1053 = v1182 + v1099;
									BYTE1(v1047) += BYTE2(v1135) + v180;
									v180 = __CFADD__(v1189, v1100);
									v1054 = v1189 + v1100;
									v1049[15] = v1101;
									LOBYTE(v1054) = BYTE2(v1146) + v180 + v1054;
									v1258 = v1273 - 1;
									if (!v1258)
										break;
									v1049 += 16;
								}
								v1045 = v1291;
							}
						}
					LABEL_1361:
						v1045 += 10;
						if (!--triLn_v1123)
							return;
					}
					if (v1048 > viewPortWidth)
						v1048 = viewPortWidth;
					v18 = __OFSUB__((x_WORD)v1048, (x_WORD)v1046);
					LOWORD(v1048) = v1048 - v1046;
					if ((unsigned __int8)(((v1048 & 0x8000u) != 0) ^ v18) | ((x_WORD)v1048 == 0))
						goto LABEL_1361;
					v1049 += v1046;
					v1053 = __ROL4__(*((x_DWORD*)v1045 + 3), 16);
					BYTE1(v1047) = v1053;
					LOWORD(v1053) = v1045[4];
					LOBYTE(v1047) = *((x_BYTE*)v1045 + 10);
					v1258 = v1048;
					v1054 = __ROL4__(*((x_DWORD*)v1045 + 4), 16);
					goto LABEL_1294;
				}
				v1114 += v1191;
				v74 += v1104 * v1161;
				v75 += v1161 * v1108;
				v76 += v1161 * v1127;
				v77 += v1161 * v1138;
				v78 += v1161 * v1149;
				if (v1301)
				{
					triLn_v1123 = viewPortHeight;
					if (v1297)
					{
						v1114 = viewPortHeight;
					}
					else
					{
						v1297 = viewPortHeight <= v1114;
						v1120 = viewPortHeight - v1114;
					}
				}
			}
			else if (v1301)
			{
			v82 = viewPortHeight - v1191;
			triLn_v1123 = viewPortHeight - v1191;
			if (v1297)
			{
				v1114 = viewPortHeight - v1191;
			}
			else
			{
				v18 = __OFSUB__(v82, v1114);
				v83 = v82 - v1114;
				v1297 = (v83 < 0) ^ v18 | (v83 == 0);
				v1120 = v83;
			}
			}
			v81 = LoadPolygon((x_DWORD*)unk_DE56Cx, &v74, &v75, &v76, &v77, &v78, v1104, v1108, v1127, v1138, v1149, &v1114);
			v80 = v1122;
			goto LABEL_156;
		}
	}
}

x_DWORD* GameRender::LoadPolygon(x_DWORD* ptrPolys, int* v0, int* v1, int s0, int s1, int* line)
{
	do
	{
		ptrPolys[0] = *v0;
		*v0 += s0;
		ptrPolys[1] = *v1;
		*v1 += s1;
		ptrPolys += 5;
		*line = *line - 1;
	} while (*line);

	return ptrPolys;
}

x_DWORD* GameRender::LoadPolygon(x_DWORD* ptrPolys, int* v0, int* v1, int* v4, int s0, int s1, int s4, int* line)
{
	do
	{
		ptrPolys[0] = *v0;
		*v0 += s0;
		ptrPolys[1] = *v1;
		*v1 += s1;
		ptrPolys[4] = *v4;
		*v4 += s4;
		ptrPolys += 5;
		*line = *line - 1;
	} while (*line);

	return ptrPolys;
}

x_DWORD* GameRender::LoadPolygon(x_DWORD* ptrPolys, int* v0, int* v1, int* v2, int* v3, int s0, int s1, int s2, int s3, int* line)
{
	do
	{
		ptrPolys[0] = *v0;
		*v0 += s0;
		ptrPolys[1] = *v1;
		*v1 += s1;
		ptrPolys[2] = *v2;
		*v2 += s2;
		ptrPolys[3] = *v3;
		*v3 += s3;
		ptrPolys += 5;
		*line = *line - 1;
	} while (*line);

	return ptrPolys;
}

x_DWORD* GameRender::LoadPolygon(x_DWORD* ptrPolys, int* v0, int* v1, int* v2, int* v3, int* v4, int s0, int s1, int s2, int s3, int s4, int* line)
{
	do
	{
		ptrPolys[0] = *v0;
		*v0 += s0;
		ptrPolys[1] = *v1;
		*v1 += s1;
		ptrPolys[2] = *v2;
		*v2 += s2;
		ptrPolys[3] = *v3;
		*v3 += s3;
		ptrPolys[4] = *v4;
		*v4 += s4;
		ptrPolys += 5;
		*line = *line - 1;
	} while (*line);

	return ptrPolys;
}

void GameRender::SetTextures(std::array<uint8_t*, 256>& textureAdresses)
{
	m_textureAddresses = textureAdresses;
}

void GameRender::SetX_BYTE_F6EE0_tablesx(uint8_t* pX_BYTE_F6EE0_tablesx)
{
	m_ptrX_BYTE_F6EE0_tablesx = pX_BYTE_F6EE0_tablesx;
}

void GameRender::SetRenderViewPortSize_BCD45(uint8_t viewPortSizeSetting, uint16_t screenWidth, uint16_t screenHeight)
{
	ViewPort viewPort;

	uint16_t v1 = 40 - viewPortSizeSetting;

	if (v1 == 0)
	{
		viewPort.PosX = 0;
		viewPort.Width = screenWidth;
		viewPort.PosY = 0;
		viewPort.Height = screenHeight;
	}
	else
	{
		uint16_t viewPortWidthMultipler = screenWidth / 40;
		uint16_t viewPortHeightMultipler = screenHeight / 40;

		viewPort.PosX = (viewPortWidthMultipler * v1) / 2;
		viewPort.PosY = (viewPortHeightMultipler * v1) / 2;
		viewPort.Width = viewPortWidthMultipler * viewPortSizeSetting;
		viewPort.Height = viewPortHeightMultipler * viewPortSizeSetting;
	}

	SetRenderViewPortSize_BCD45(viewPort, screenWidth, screenHeight);
}

void GameRender::SetRenderViewPortSize_BCD45(ViewPort viewPort, uint16_t screenWidth, uint16_t screenHeight)
{
	m_uiScreenWidth = screenWidth;
	m_uiScreenHeight = screenHeight;

	m_viewPort = viewPort;

	if (viewPort.Width > screenWidth)
	{
		m_viewPort.Width = screenWidth;
	}

	if (viewPort.Height > screenHeight)
	{
		m_viewPort.Height = screenHeight;
	}

	if (viewPort.PosX > screenWidth)
	{
		m_viewPort.PosX = screenWidth - 1;
	}

	if (viewPort.PosY > screenHeight)
	{
		m_viewPort.PosY = screenHeight - 1;
	}

	int32_t ptrScreenRenderBufferStart = m_viewPort.PosX + m_uiScreenWidth * m_viewPort.PosY;
	SetRenderViewPortSize_BCD45(ptrScreenRenderBufferStart + m_ptrScreenBuffer, m_viewPort.Width, m_viewPort.Height, m_uiScreenWidth);
}

void GameRender::SetRenderViewPortSize_BCD45(uint8_t* ptrScreenBufferStart, uint16_t viewPortWidth, uint16_t viewPortHeight, uint16_t screenWidth)
{
	if (screenWidth)
	{
		m_uiScreenWidth = screenWidth;
	}

	if (ptrScreenBufferStart)
	{
		m_ptrViewPortRenderBufferStart = ptrScreenBufferStart;
		m_ptrViewPortRenderBufferAltStart = ptrScreenBufferStart - m_uiScreenWidth;
	}

	if (viewPortWidth)
	{
		m_viewPort.Width = viewPortWidth;
	}

	if (viewPortHeight)
	{
		m_viewPort.Height = viewPortHeight;
	}
}

void GameRender::SetRenderThreads(uint8_t renderThreads)
{
	StopWorkerThreads();

	if (renderThreads < 0)
	{
		renderThreads = 0;
	}

	if (renderThreads > 0)
	{
		StartWorkerThreads(renderThreads, m_assignToSpecificCores);
	}
}

uint8_t GameRender::GetRenderThreads()
{
	return m_renderThreads.size();
}

void GameRender::WaitForRenderFinish()
{
	int taskCount = 0;

	do
	{
		uint8_t i = 0;
		taskCount = 0;
		for (i = 0; i < m_renderThreads.size(); i++)
		{
			taskCount += m_renderThreads[i]->GetIsTaskRunning();
		}
		//std::this_thread::sleep_for(std::chrono::milliseconds(50));
	} while (taskCount > 0);
}
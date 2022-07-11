#include "GameRender_old.h"

GameRender_old::~GameRender_old()
{
}

#define _BITS_PER_PIXEL	8

LPBYTE m_lpZBuffer;
long m_iWidth;
long m_iHeight;
DWORD m_dwSize;
BITMAPINFOHEADER m_bih;
LPBYTE m_lpData;

/* ---- */void GameRender_old::DrawWorld_411A0(int posX, int posY, int16_t yaw, int16_t posZ, int16_t pitch, int16_t roll, int16_t fov)
{
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
	int v28; // ebx
	uint32_t v29; // edx
	int v30; // ebx
	int v31; // edi
	uint8_t* v32; // ST2C_4
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

	m_lpZBuffer = pdwScreenBuffer_351628;
	m_iWidth = screenWidth_18062C;
	m_iHeight = screenHeight_180624;

	//m_hWnd = hWnd;
	//m_iWidth = iWindowWidth;
	//m_iHeight = iWindowHeight;
	//m_iNomFPS = iFPS;
	//m_fFrameNomTime = 1000.0 / m_iNomFPS;
	m_dwSize = m_iWidth * (_BITS_PER_PIXEL >> 3) * m_iHeight;
	m_lpData = (LPBYTE)malloc(m_dwSize * sizeof(BYTE));
	//m_iShadingMode = _SHADING_MODE_NONE;
	//m_iTextureFiltering = _TEXTURE_FILTERING_NONE;
	//m_lpZBuffer = (LPBYTE)malloc(m_dwSize * sizeof(BYTE));

	memset(&m_bih, 0, sizeof(BITMAPINFOHEADER));
	m_bih.biSize = sizeof(BITMAPINFOHEADER);
	m_bih.biWidth = m_iWidth;
	m_bih.biHeight = m_iHeight;
	m_bih.biPlanes = 1;
	m_bih.biBitCount = _BITS_PER_PIXEL;
	m_bih.biCompression = BI_RGB;
	m_bih.biSizeImage = m_dwSize;

	//m_hDD = DrawDibOpen();

	//m_ptMouse.x = (m_iWidth + 1) / 2;
	//m_ptMouse.y = (m_iHeight + 1) / 2;
	//ClientToScreen(m_hWnd, &m_ptMouse);
	//SetCursorPos(m_ptMouse.x, m_ptMouse.y);
	//ShowCursor(FALSE);

	//InitTimer();

	//m_fRefTime = GetTime();
	//m_fFrameRefTime = m_fRefTime;
	//m_iRefFPS = 0;
	//m_iCurFPS = 0;


	LOBYTE(v8) = HIBYTE(posX);
	HIBYTE(v8) = HIBYTE(posY);
	if ((signed int)(unsigned __int8)posX < 128)
		LOBYTE(v8) = HIBYTE(posX) - 1;
	if ((signed int)(unsigned __int8)posY < 128)
		HIBYTE(v8) = HIBYTE(posY) - 1;
	v9 = mapHeightmap_11B4E0[v8];
	LOBYTE(v8) += 2;
	v10 = v9;
	v11 = v9;
	v12 = mapHeightmap_11B4E0[v8];
	HIBYTE(v8) += 2;
	v13 = v10 - v12;
	v14 = v12 + v11;
	v15 = mapHeightmap_11B4E0[v8];
	LOBYTE(v8) -= 2;
	v16 = v13 - v15;
	v17 = v14 - v15;
	v18 = mapHeightmap_11B4E0[v8];
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

	if (D41A0_0.m_GameSettings.str_0x2192.xxxx_0x2193 && D41A0_0.m_GameSettings.m_Display.m_uiScreenSize && screenWidth_18062C == 640)
	{
		//VR interlaced render
		SetRenderViewPortSize_BCD45(
			pdwScreenBuffer_351628,
			2 * screenWidth_18062C,
			screenWidth_18062C / 2 - 8,
			screenHeight_180624 / 2 - 40);
		v22 = Maths::x_DWORD_DB750[vYaw];
		x_DWORD_D4790 = 20;
		v23 = 5 * v22;
		v24 = Maths::x_DWORD_DB750[0x200 + vYaw];
		x_DWORD_D4324 = -5;
		v25 = 4 * v23 >> 16;
		v26 = 20 * (signed int)v24 >> 16;
		DrawTerrainAndParticles_3C080(vPosX - v26, vPosY - v25, vYaw, posZ, pitch, roll, fov);
		SetRenderViewPortSize_BCD45(pdwScreenBuffer_351628 + (screenWidth_18062C / 2), 0, 0, 0);
		x_DWORD_D4324 = 5;
		DrawTerrainAndParticles_3C080(vPosX + v26, vPosY + v25, vYaw, posZ, pitch, roll, fov);
		x_DWORD_D4324 = 0;
		SetRenderViewPortSize_BCD45(pdwScreenBuffer_351628, screenWidth_18062C, screenWidth_18062C, screenHeight_180624);
	}
	else if (D41A0_0.m_GameSettings.m_Display.m_uiScreenSize != 1 || D41A0_0.m_GameSettings.str_0x2192.xxxx_0x2193)
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
			v35 = ViewPortRenderBufferStart_DE558;
			SetRenderViewPortSize_BCD45(x_DWORD_E9C3C, 0, 0, 0);
			DrawTerrainAndParticles_3C080(vPosX, vPosY, vYaw, posZ, pitch, roll, fov);
			//Apply Blur
			SetRenderViewPortSize_BCD45(v35, 0, 0, 0);
			v51 = (signed int)(unsigned __int16)viewPort.Width_DE564 >> 2;
			v49 = iScreenWidth_DE560 - (unsigned __int16)viewPort.Width_DE564;
			v50 = (unsigned __int16)viewPort.Height_DE568;
			
			if (D41A0_0.m_GameSettings.m_Display.xxxx_0x2191 != 1)
			{
				v37 = (x_BYTE*)x_DWORD_E9C3C;
				goto LABEL_33;
			}
			v37 = (x_BYTE*)x_DWORD_E9C3C;
			v38 = (signed int)(unsigned __int16)viewPort.Width_DE564 >> 2;
			LOBYTE(v39) = *(x_BYTE*)(x_DWORD_E9C3C + 2);
			HIBYTE(v39) = v35[2];
			LOBYTE(v40) = *(x_BYTE*)(x_DWORD_E9C3C + 3);
			LOBYTE(v36) = x_BYTE_F6EE0_tablesx[0x4000 + v39];
			HIBYTE(v40) = v35[3];
			HIBYTE(v36) = x_BYTE_F6EE0_tablesx[0x4000 + v40];
			v36 <<= 16;
			LOBYTE(v39) = *(x_BYTE*)x_DWORD_E9C3C;
			HIBYTE(v39) = *v35;
			LOBYTE(v40) = *(x_BYTE*)(x_DWORD_E9C3C + 1);
			LOBYTE(v36) = x_BYTE_F6EE0_tablesx[0x4000 + v39];
			HIBYTE(v40) = v35[1];
			for (BYTE1(v36) = x_BYTE_F6EE0_tablesx[0x4000 + v40]; ; BYTE1(v36) = x_BYTE_F6EE0_tablesx[0x4000 + v42])
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
				LOBYTE(v36) = x_BYTE_F6EE0_tablesx[0x4000 + v41];
				LOBYTE(v42) = v35[3];
				HIBYTE(v36) = x_BYTE_F6EE0_tablesx[0x4000 + v42];
				v36 <<= 16;
				HIBYTE(v41) = *v37;
				LOBYTE(v41) = *v35;
				HIBYTE(v42) = v37[1];
				LOBYTE(v36) = x_BYTE_F6EE0_tablesx[0x4000 + v41];
				LOBYTE(v42) = v35[1];
			}
		}

		DrawTerrainAndParticles_3C080(vPosX, vPosY, vYaw, posZ, pitch, roll, fov);

		//22270D
		//add_compare(0x22270D, debugafterload);

		if (D41A0_0.m_GameSettings.str_0x2192.xxxx_0x2192)
		{
			v53 = ViewPortRenderBufferStart_DE558;
			for (i = (unsigned __int16)viewPort.Height_DE568 - 1; i; i--)
			{
				v43 = (x_BYTE*)v53;
				v44 = iScreenWidth_DE560;
				v45 = (unsigned __int16)viewPort.Width_DE564 - 1;
				HIWORD(v46) = 0;
				HIWORD(v47) = 0;
				HIWORD(v48) = 0;
				do
				{
					LOBYTE(v46) = v43[0];
					LOBYTE(v47) = v43[1];
					BYTE1(v46) = v43[v44];
					LOBYTE(v48) = x_BYTE_F6EE0_tablesx[0x4000 + v46];
					BYTE1(v47) = v43[v44 + 1];
					BYTE1(v48) = x_BYTE_F6EE0_tablesx[0x4000 + v47];
					*v43++ = x_BYTE_F6EE0_tablesx[0x4000 + v48];
					v45--;
				} while (v45);
				v53 += iScreenWidth_DE560;
			}
		}
	LABEL_44:
		D41A0_0.m_GameSettings.m_Display.xxxx_0x2191 = v52;
	}
	else
	{
		v28 = 5 * Maths::x_DWORD_DB750[vYaw];
		v29 = Maths::x_DWORD_DB750[0x200 + vYaw];
		x_DWORD_D4790 = 20;
		x_DWORD_D4324 = iScreenWidth_DE560 / 40;
		v30 = 4 * v28 >> 16;
		v31 = 20 * (signed int)v29 >> 16;
		DrawTerrainAndParticles_3C080(v31 + vPosX, v30 + vPosY, vYaw, posZ, pitch, roll, fov);
		v32 = ViewPortRenderBufferStart_DE558;
		SetRenderViewPortSize_BCD45(x_DWORD_E9C3C, 0, 0, 0);
		x_DWORD_D4324 = 0 - (iScreenWidth_DE560 / 40);
		DrawTerrainAndParticles_3C080(vPosX - v31, vPosY - v30, vYaw, posZ, pitch, roll, fov);
		SetRenderViewPortSize_BCD45(v32, 0, 0, 0);
		x_DWORD_D4324 = 0;
	}
}

void GameRender_old::DrawSky_40950(int16_t roll/*, uint8_t startLine, uint8_t drawEveryNthLine*/)
{
	int v1; // ebx
	int v2; // edx
	int v3; // esi
	int v4; // ebx
	char* v5; // edx
	int v7; // edx
	int v8; // eax
	int v9; // ecx
	int v10; // edx
	__int16 result; // ax
	int v12; // eax
	char* v13; // esi
	uint8_t* v14; // edi
	uint8_t* v15; // edx
	int v16; // ecx
	int v17; // ebx
	int v18; // eax
	char v19ar[0x500]; // [esp+0h] [ebp-52Ch]
	int v20; // [esp+500h] [ebp-2Ch]
	int v21; // [esp+504h] [ebp-28h]
	//int* viewPortRenderBufferStart; // [esp+508h] [ebp-24h]
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
	v26 = (Maths::x_DWORD_DB750[v1] << 8) / viewPort.Width_DE564;
	v3 = 0;
	v25 = v2 / viewPort.Width_DE564;
	v4 = 0;
	v29 = 0;
	v5 = v19ar;
	v30 = 0;
	uint16_t width = viewPort.Width_DE564;
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
	v7 = (-(str_F2C20ar.dword0x0d * str_F2C20ar.dword0x22) >> 16) + str_F2C20ar.dword0x24;
	v8 = str_F2C20ar.dword0x10 - (str_F2C20ar.dword0x11 * str_F2C20ar.dword0x22 >> 16);
	v9 = v7 * v25 - v8 * v26;
	v10 = v25 * v8 + v26 * v7;
	v23 = ((unsigned __int16)x_WORD_F2CC0 << 15) - v9;
	uint8_t* viewPortRenderBufferStart = ViewPortRenderBufferStart_DE558;
	result = viewPort.Height_DE568;
	v27 = -v10;
	v24 = (unsigned __int16)viewPort.Height_DE568;
	if ((x_WORD)viewPort.Height_DE568)
	{
		do
		{
			v12 = ((unsigned __int16)viewPort.Width_DE564
				- (__CFSHL__((signed int)(unsigned __int16)viewPort.Width_DE564 >> 31, 2)
					+ 4 * ((signed int)(unsigned __int16)viewPort.Width_DE564 >> 31))) >> 2;
			v13 = v19ar;
			v20 = v27 >> 16;
			v14 = viewPortRenderBufferStart;
			v21 = v23 >> 16;
			v15 = off_D41A8_sky;
			BYTE1(v17) = BYTE2(v27);
			v16 = ((unsigned __int16)viewPort.Width_DE564
				- (__CFSHL__((signed int)(unsigned __int16)viewPort.Width_DE564 >> 31, 2)
					+ 4 * ((signed int)(unsigned __int16)viewPort.Width_DE564 >> 31))) >> 2;
			LOBYTE(v17) = BYTE2(v23);
			v17 = (unsigned __int16)v17;
			do
			{
				LOBYTE(v12) = v15[v17];
				LOBYTE(v17) = *v13 + v17;
				BYTE1(v17) += v13[1];
				BYTE1(v12) = v15[v17];
				LOBYTE(v17) = v13[2] + v17;
				BYTE1(v17) += v13[3];
				v18 = v12 << 16;
				LOBYTE(v18) = v15[v17];
				LOBYTE(v17) = v13[4] + v17;
				BYTE1(v17) += v13[5];
				BYTE1(v18) = v15[v17];
				LOBYTE(v17) = v13[6] + v17;
				BYTE1(v17) += v13[7];
				v12 = __ROL4_16__(v18);
				*v14 = v12;
				v14++;
				v13 += 8;
				v16--;
			} while (v16);
			viewPortRenderBufferStart += iScreenWidth_DE560;
			result = v25;
			v24--;
			v23 -= v26;
			v27 += v25;
		} while (v24);
	}
}

void GameRender_old::DrawTerrainAndParticles_3C080(__int16 posX, __int16 posY, __int16 yaw, signed int posZ, int pitch, int16_t roll, int fov)
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
	unsigned __int16 v42; // bx
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
	char v62; // ch
	char v63; // ah
	char v64; // dl
	char v65; // dh
	char v66; // ch
	char v67; // dl
	char v71; // dl
	char v73; // ch
	char v74; // ah
	char v75; // dl
	char v76; // dh
	char v77; // ch
	char v78; // dl
	char v79; // dh
	int v82x;
	int v83x;
	char v84; // dl
	char v85; // cl
	char v87; // al
	char v88; // dl
	char v89; // dh
	char v92; // cl
	char v93; // dl
	char v96; // al
	char v97; // dl
	char v99; // ah
	char v100; // dl
	char v101; // dh
	char v102; // ch
	char v105; // dl
	char v106; // dh
	signed int v109; // esi
	int v110; // ebx
	unsigned __int16 v111; // dx
	__int16 v112; // ax
	int v113; // eax
	int v116; // eax
	unsigned __int16 v118; // bx
	unsigned __int8 v119; // al
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
	char v136; // dl
	char v137; // ch
	char v138; // dl
	char v142; // ch
	char v144; // dl
	int v147x;
	char v148; // dl
	char v149; // dl
	char v153; // cl
	char v156; // dl
	int v160x;
	int v161x;
	char v163; // dl
	char v165; // ah
	char v166; // dl
	char v167; // dh
	char v170; // ch
	char v173; // dl
	char v174; // dh
	int v177x;
	int v178x;
	char v179; // dl
	char v180; // ch
	char v182; // ah
	char v183; // dl
	char v184; // dh
	char v189; // ch
	char v191; // dl
	char v192; // dh
	char v194; // ch
	int v197; // ecx
	signed int v198; // esi
	int v199; // ebx
	unsigned __int16 v200; // di
	__int16 v201; // ax
	int v202; // eax
	int v203; // eax
	unsigned __int16 v204; // bx
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
	int v217x;
	int v218x;
	char v219; // dl
	char v221; // al
	char v222; // dl
	char v223; // dh
	char v226; // cl
	char v228; // dl
	char v229; // dh
	int v231x;
	int v232x;
	char v233; // dl
	char v234; // ch
	char v236; // ah
	char v237; // dl
	char v238; // dh
	char v242; // ch
	char v244; // dl
	char v245; // dh
	int v248x[33]; // [esp+0h] [ebp-62h]//v248x[0]
	uint8_t* v277; // [esp+84h] [ebp+22h]
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

	/*
	#ifdef DEBUG_SEQUENCES
		uint8_t origbyte20 = 0;
		uint8_t remakebyte20 = 0;
		int comp20;
		if (debugafterload)
		{
			if (debugcounter_21d080 > 0)
			comp20 = compare_with_sequence((char*)"0021D080-003F52A4", (uint8_t*)x_DWORD_E9C38_smalltit, 0x3f52a4, debugcounter_21d080, 36916 * 2, 36916 * 2, &origbyte20, &remakebyte20);
			else
			{
				*(x_DWORD_E9C38_smalltit + 0x2271) = 0x3b;
				*(x_DWORD_E9C38_smalltit + 0x2271+4) = 0x64;
			}
			debugcounter_21d080++;
		}
	#endif DEBUG_SEQUENCES
	*/

	shadows_F2CC7 = D41A0_0.m_GameSettings.m_Graphics.m_wShadows;//21d080
	notDay_D4320 = D41A0_0.terrain_2FECE.MapType != MapType_t::Day;
	str_F2C20ar.dword0x10 = (signed int)(unsigned __int16)viewPort.Height_DE568 >> 1;
	x_WORD_F2CC4 = posX;
	x_WORD_F2CC0 = yaw & 0x7FF;
	x_WORD_F2CC2 = posY;
	v9 = (yaw & 0x7FF) + 256;
	str_F2C20ar.dword0x20 = posZ;
	v10 = Maths::x_DWORD_DB750[0x100 + v9];
	str_F2C20ar.dword0x24 = x_DWORD_D4324 + ((signed int)(unsigned __int16)viewPort.Width_DE564 >> 1);
	str_F2C20ar.dword0x0f = v10;
	v11 = Maths::x_DWORD_DB750[v9 - 0x100];
	v12 = ((((yaw & 0x7FF) + 256) & 0x1FF) - 256) & 0x7FF;
	v248x[32] = (v9 >> 9) & 3;
	v248x[30] = Maths::x_DWORD_DB750[0x200 + v12];
	str_F2C20ar.dword0x17 = v11;
	v13 = Maths::x_DWORD_DB750[v12];
	SetBillboards_3B560(-roll & 0x7FF);//21d1aa
	str_F2C20ar.dword0x18 = 7
		* Maths::sub_7277A_radix_3d(
			(unsigned __int16)viewPort.Width_DE564 * (unsigned __int16)viewPort.Width_DE564
			+ (unsigned __int16)viewPort.Height_DE568 * (unsigned __int16)viewPort.Height_DE568)
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
			Str_E9C38_smalltit[v15x].dword0_rot = v248x[28];
			Str_E9C38_smalltit[v15x].dword12 = v248x[29];
			if (a1 < 0)
				Str_E9C38_smalltit[v15x].word38 = 0;
			else
				Str_E9C38_smalltit[v15x].word38 = 4;
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
			Str_E9C38_smalltit[v18x].dword0_rot -= v248x[27];
			Str_E9C38_smalltit[v18x].dword12 += v20;
			v19--;
			v18x++;
		}
		a2 += 256;
		v17--;
	}
	str_F2C20ar.dword0x15 = 26214400;//21d2df not drawing
	v278x = 0;
	str_F2C20ar.dword0x12 = 8912896;
	v22 = v277[0];
	str_F2C20ar.dword0x22 = pitch * (unsigned __int16)viewPort.Width_DE564 >> 8;
	LOBYTE(v279) = v22 + HIBYTE(posX);
	HIBYTE(v279) = v277[1] + HIBYTE(posY);
	v23 = roll & 0x7FF;
	str_F2C20ar.dword0x11 = Maths::x_DWORD_DB750[0x200 + v23];
	str_F2C20ar.dword0x16 = 23658496;
	str_F2C20ar.dword0x0d = Maths::x_DWORD_DB750[v23];
	str_F2C20ar.dword0x13 = 14745600;
	if (!D41A0_0.m_GameSettings.m_Graphics.m_wSky || isCaveLevel_D41B6)
	{
		v26 = viewPort.Width_DE564;
		v27 = iScreenWidth_DE560 - viewPort.Width_DE564;
		v28 = (v26 - (__CFSHL__((signed int)v26 >> 31, 2) + 4 * ((signed int)v26 >> 31))) >> 2;
		v29 = viewPort.Height_DE568;
		v25x = ViewPortRenderBufferStart_DE558;
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
		DrawSky_40950(roll);
	}
	if (isCaveLevel_D41B6)//21d3e3 cleaned screen
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
					v48 = ((Str_E9C38_smalltit[v47x].dword16 * str_F2C20ar.dword0x11 - str_F2C20ar.dword0x0d * Str_E9C38_smalltit[v47x].dword20) >> 16) + str_F2C20ar.dword0x24;
					v248x[25] = ((Str_E9C38_smalltit[v47x].dword16 * str_F2C20ar.dword0x11 - str_F2C20ar.dword0x0d * Str_E9C38_smalltit[v47x].dword28) >> 16) + str_F2C20ar.dword0x24;
					v49 = Str_E9C38_smalltit[v47x].dword16 * str_F2C20ar.dword0x0d;
					v248x[24] = str_F2C20ar.dword0x10 - ((v49 + str_F2C20ar.dword0x11 * Str_E9C38_smalltit[v47x].dword20) >> 16);
					v50 = str_F2C20ar.dword0x10 - ((v49 + str_F2C20ar.dword0x11 * Str_E9C38_smalltit[v47x].dword28) >> 16);
					Str_E9C38_smalltit[v47x].dword16 = v48;
					v51 = v50;
					v52 = v248x[24];
					Str_E9C38_smalltit[v47x].dword28 = v51;
					Str_E9C38_smalltit[v47x].dword20 = v52;
					v53 = Str_E9C38_smalltit[v47x].dword16;
					Str_E9C38_smalltit[v47x].dword24 = v248x[25];
					if (v53 >= 0)
					{
						if ((signed int)(unsigned __int16)viewPort.Width_DE564 <= Str_E9C38_smalltit[v47x].dword16)
							Str_E9C38_smalltit[v47x].word38 |= 0x10u;
					}
					else
					{
						Str_E9C38_smalltit[v47x].word38 |= 8u;
					}
					v54 = Str_E9C38_smalltit[v47x].dword20;
					if (v54 >= 0)
					{
						if ((unsigned __int16)viewPort.Height_DE568 <= v54)
							Str_E9C38_smalltit[v47x].word38 |= 0x40u;
					}
					else
					{
						Str_E9C38_smalltit[v47x].word38 |= 0x20u;
					}
					if (Str_E9C38_smalltit[v47x].dword24 >= 0)
					{
						if ((signed int)(unsigned __int16)viewPort.Width_DE564 <= Str_E9C38_smalltit[v47x].dword24)
							Str_E9C38_smalltit[v47x].word38 |= 0x200u;
					}
					else
					{
						Str_E9C38_smalltit[v47x].word38 |= 0x100u;
					}
					v55 = Str_E9C38_smalltit[v47x].dword28;
					if (v55 >= 0)
					{
						if ((unsigned __int16)viewPort.Height_DE568 <= v55)
							Str_E9C38_smalltit[v47x].word38 |= 0x800u;
					}
					else
					{
						Str_E9C38_smalltit[v47x].word38 |= 0x400u;
					}
					v47x++;
					v46--;
				}
				v57x = 800;
				v281 = 20;
				do
				{
					v58 = 39;
					for (jx = v57x; ; jx++)
					{
						v293 = v58;
						if (!v58)
							break;
						v248x[18] = Str_E9C38_smalltit[jx].dword24;//*(x_DWORD*)(j + 24);
						v248x[19] = Str_E9C38_smalltit[jx].dword28;//*(x_DWORD*)(j + 28);
						v248x[22] = Str_E9C38_smalltit[jx].dword32;//*(x_DWORD*)(j + 32);
						v60 = Str_E9C38_smalltit[jx].word38 & 0xff;//*(x_BYTE*)(j - 6);

						if (Str_E9C38_smalltit[jx + 1].word38 & 4)
							break;
						v248x[12] = Str_E9C38_smalltit[jx + 1].dword24;//*(x_DWORD*)(j + 24);
						v248x[13] = Str_E9C38_smalltit[jx + 1].dword28;//*(x_DWORD*)(j + 28);
						v248x[16] = Str_E9C38_smalltit[jx + 1].dword32;//*(x_DWORD*)(j + 32);
						v62 = Str_E9C38_smalltit[jx + 1].word38 & 0xff;//*(x_BYTE*)(j + 38);

						v248x[6] = Str_E9C38_smalltit[jx - 39].dword24;//*(x_DWORD*)(j - 1736);
						v248x[7] = Str_E9C38_smalltit[jx - 39].dword28;//*(x_DWORD*)(j - 1732);
						v248x[10] = Str_E9C38_smalltit[jx - 39].dword32;//*(x_DWORD*)(j - 1728);
						v63 = Str_E9C38_smalltit[jx - 39].word38 & 0xff;//*(x_BYTE*)(j - 1722);
						v64 = v63 | v62 | v60;
						v65 = v63 & v62 & v60;

						v248x[0] = Str_E9C38_smalltit[jx - 40].dword24;//*(x_DWORD*)(j - 1780);
						v248x[1] = Str_E9C38_smalltit[jx - 40].dword28;//*(x_DWORD*)(j - 1776);
						v248x[4] = Str_E9C38_smalltit[jx - 40].dword32;//*(x_DWORD*)(j - 1772);
						v66 = Str_E9C38_smalltit[jx - 40].word38 & 0xff;//*(x_BYTE*)(j - 1766);
						v67 = v66 | v64;

						if ((v66 & v65 & 0x80u) == 0)
						{
							if (Str_E9C38_smalltit[jx].word38 & 0x1000)
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
								{
									v248x[20] = xunk_D4350[Str_E9C38_smalltit[jx].byte42_std][0];
									v248x[21] = xunk_D4350[Str_E9C38_smalltit[jx].byte42_std][1];
									v248x[14] = xunk_D4350[Str_E9C38_smalltit[jx].byte42_std][2];
									v248x[15] = xunk_D4350[Str_E9C38_smalltit[jx].byte42_std][3];
									v248x[8] = xunk_D4350[Str_E9C38_smalltit[jx].byte42_std][4];
									v248x[9] = xunk_D4350[Str_E9C38_smalltit[jx].byte42_std][5];
									v248x[2] = xunk_D4350[Str_E9C38_smalltit[jx].byte42_std][6];
									v248x[3] = xunk_D4350[Str_E9C38_smalltit[jx].byte42_std][7];
									x_DWORD_DE55C_ActTexture = x_DWORD_DDF50_texture_adresses[1];
									x_BYTE_E126D = 5;
									if (Str_E9C38_smalltit[jx].word38 & 1)
									{
										DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[0], &v248x[12]);
										DrawTriangleInProjectionSpace_B6253(&v248x[0], &v248x[6], &v248x[12]);
									}
									else
									{
										DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[6], &v248x[12]);
										DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[0], &v248x[6]);
									}
								}
							}
						}
						v248x[18] = Str_E9C38_smalltit[jx].dword16;//*(x_DWORD*)(v68 + 16);
						v248x[19] = Str_E9C38_smalltit[jx].dword20;//*(x_DWORD*)(v68 + 20);
						v248x[22] = Str_E9C38_smalltit[jx].dword32;//*(x_DWORD*)(v68 + 32);
						v71 = Str_E9C38_smalltit[jx].word38 & 0xff;//*(x_BYTE*)(j - 6);

						if (Str_E9C38_smalltit[jx + 1].word38 & 4)
							break;
						v248x[12] = Str_E9C38_smalltit[jx + 1].dword16;//*(x_DWORD*)(j + 16);
						v248x[13] = Str_E9C38_smalltit[jx + 1].dword20;//*(x_DWORD*)(j + 20);
						v248x[16] = Str_E9C38_smalltit[jx + 1].dword32;//*(x_DWORD*)(j + 32);
						v73 = Str_E9C38_smalltit[jx + 1].word38 & 0xff;

						v248x[6] = Str_E9C38_smalltit[jx - 39].dword16;//*(x_DWORD*)(j - 1744);
						v248x[7] = Str_E9C38_smalltit[jx - 39].dword20;//*(x_DWORD*)(j - 1740);
						v248x[10] = Str_E9C38_smalltit[jx - 39].dword32;//*(x_DWORD*)(j - 1728);
						v74 = Str_E9C38_smalltit[jx - 39].word38 & 0xff;//*(x_BYTE*)(j - 1722);
						v75 = v74 | v73 | v71;
						v76 = v74 & v73 & v71;

						v248x[0] = Str_E9C38_smalltit[jx - 40].dword16;//*(x_DWORD*)(j - 1788);
						v248x[1] = Str_E9C38_smalltit[jx - 40].dword20;//*(x_DWORD*)(j - 1784);
						v248x[4] = Str_E9C38_smalltit[jx - 40].dword32;//*(x_DWORD*)(j - 1772);
						v77 = Str_E9C38_smalltit[jx - 40].word38 & 0xff;//*(x_BYTE*)(j - 1766);
						v78 = v77 | v75;
						v79 = v77 & v76;

						if (v79 >= 0)
						{
							if (Str_E9C38_smalltit[jx].word38 & 0x1000)
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
								v248x[20] = xunk_D4350[Str_E9C38_smalltit[jx].byte42_std][0];
								v248x[21] = xunk_D4350[Str_E9C38_smalltit[jx].byte42_std][1];
								v248x[14] = xunk_D4350[Str_E9C38_smalltit[jx].byte42_std][2];
								v248x[15] = xunk_D4350[Str_E9C38_smalltit[jx].byte42_std][3];
								v248x[8] = xunk_D4350[Str_E9C38_smalltit[jx].byte42_std][4];
								v248x[9] = xunk_D4350[Str_E9C38_smalltit[jx].byte42_std][5];
								v248x[2] = xunk_D4350[Str_E9C38_smalltit[jx].byte42_std][6];
								v248x[3] = xunk_D4350[Str_E9C38_smalltit[jx].byte42_std][7];
								x_DWORD_DE55C_ActTexture = x_DWORD_DDF50_texture_adresses[Str_E9C38_smalltit[jx].byte41];
								if (Str_E9C38_smalltit[jx].word38 & 1)
								{
									DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[12], &v248x[0]);
									DrawTriangleInProjectionSpace_B6253(&v248x[0], &v248x[12], &v248x[6]);
								}
								else
								{
									DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[12], &v248x[6]);
									DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[6], &v248x[0]);
								}
							}
							if (Str_E9C38_smalltit[jx].word36)
								DrawParticles_3E360(jx);
						}
						v58 = v293 - 1;
					}
					if (v293)
					{
						v82x = jx;// -1;
						v83x = v57x + 38;
						do
						{
							v248x[18] = Str_E9C38_smalltit[v83x].dword24;//*(x_DWORD*)(v83 + 24);
							v248x[19] = Str_E9C38_smalltit[v83x].dword28;//*(x_DWORD*)(v83 + 28);
							v248x[22] = Str_E9C38_smalltit[v83x].dword32;//*(x_DWORD*)(v83 + 32);
							v84 = Str_E9C38_smalltit[v83x].word38 & 0xff;//*(x_BYTE*)(v83 + 38);

							v248x[12] = Str_E9C38_smalltit[v83x + 1].dword24;//*(x_DWORD*)(v83 + 68);
							v248x[13] = Str_E9C38_smalltit[v83x + 1].dword28;//*(x_DWORD*)(v83 + 72);
							v248x[16] = Str_E9C38_smalltit[v83x + 1].dword32;//*(x_DWORD*)(v83 + 76);
							v85 = Str_E9C38_smalltit[v83x + 1].word38 & 0xff;//*(x_BYTE*)(v83 + 82);

							v248x[6] = Str_E9C38_smalltit[v83x - 39].dword24;//*(x_DWORD*)(v83 - 1692);
							v248x[7] = Str_E9C38_smalltit[v83x - 39].dword28;//*(x_DWORD*)(v83 - 1688);
							v248x[10] = Str_E9C38_smalltit[v83x - 39].dword32;//*(x_DWORD*)(v83 - 1684);
							v87 = Str_E9C38_smalltit[v83x - 39].word38 & 0xff;//*(x_BYTE*)(v83 - 1678);
							v88 = v87 | v85 | v84;
							v89 = v87 & v85 & v84;

							v248x[0] = Str_E9C38_smalltit[v83x - 40].dword24;//*(x_DWORD*)(v83 - 1736);
							v248x[1] = Str_E9C38_smalltit[v83x - 40].dword28;//*(x_DWORD*)(v83 - 1732);
							v248x[4] = Str_E9C38_smalltit[v83x - 40].dword32;//*(x_DWORD*)(v83 - 1728);
							v92 = Str_E9C38_smalltit[v83x - 40].word38 & 0xff;//*(x_BYTE*)(v91 - 1766);
							v93 = v92 | v88;

							if ((v92 & v89 & 0x80u) == 0)
							{
								if (Str_E9C38_smalltit[v83x].word38 & 0x1000)
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
									v248x[20] = xunk_D4350[Str_E9C38_smalltit[v83x].byte42_std][0];
									v248x[21] = xunk_D4350[Str_E9C38_smalltit[v83x].byte42_std][1];
									v248x[14] = xunk_D4350[Str_E9C38_smalltit[v83x].byte42_std][2];
									v248x[15] = xunk_D4350[Str_E9C38_smalltit[v83x].byte42_std][3];
									v248x[8] = xunk_D4350[Str_E9C38_smalltit[v83x].byte42_std][4];
									v248x[9] = xunk_D4350[Str_E9C38_smalltit[v83x].byte42_std][5];
									v248x[2] = xunk_D4350[Str_E9C38_smalltit[v83x].byte42_std][6];
									v248x[3] = xunk_D4350[Str_E9C38_smalltit[v83x].byte42_std][7];
									x_DWORD_DE55C_ActTexture = x_DWORD_DDF50_texture_adresses[1];
									v96 = Str_E9C38_smalltit[v83x].word38 & 0xff;
									x_BYTE_E126D = 5;
									if (v96 & 1)
									{
										DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[0], &v248x[12]);
										DrawTriangleInProjectionSpace_B6253(&v248x[0], &v248x[6], &v248x[12]);
									}
									else
									{
										DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[6], &v248x[12]);
										DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[0], &v248x[6]);
									}
								}
							}
							v248x[18] = Str_E9C38_smalltit[v83x].dword16;//*(x_DWORD*)(v94 + 16);
							v248x[19] = Str_E9C38_smalltit[v83x].dword20;//*(x_DWORD*)(v94 + 20);
							v248x[22] = Str_E9C38_smalltit[v83x].dword32;//*(x_DWORD*)(v94 + 32);
							v97 = Str_E9C38_smalltit[v83x].word38 & 0xff;//*(x_BYTE*)(v94 + 38);

							v248x[12] = Str_E9C38_smalltit[v83x + 1].dword16;//*(x_DWORD*)(v94 + 60);
							v248x[13] = Str_E9C38_smalltit[v83x + 1].dword20;//*(x_DWORD*)(v94 + 64);
							v248x[16] = Str_E9C38_smalltit[v83x + 1].dword32;//*(x_DWORD*)(v94 + 76);
							v99 = Str_E9C38_smalltit[v83x + 1].word38 & 0xff;//*(x_BYTE*)(v94 + 82);
							v100 = v99 | v97;
							v101 = v99 & v97;

							v248x[6] = Str_E9C38_smalltit[v83x - 39].dword16;//*(x_DWORD*)(v94 - 1700);
							v248x[7] = Str_E9C38_smalltit[v83x - 39].dword20;//*(x_DWORD*)(v94 - 1696);
							v248x[10] = Str_E9C38_smalltit[v83x - 39].dword32;//*(x_DWORD*)(v94 - 1684);
							v102 = Str_E9C38_smalltit[v83x - 39].word38 & 0xff;//*(x_BYTE*)(v94 - 1678);

							v248x[0] = Str_E9C38_smalltit[v83x - 40].dword16;//*(x_DWORD*)(v94 - 1744);
							v248x[1] = Str_E9C38_smalltit[v83x - 40].dword20;//*(x_DWORD*)(v94 - 1740);
							v248x[4] = Str_E9C38_smalltit[v83x - 40].dword32;//*(x_DWORD*)(v104 - 1772);
							v105 = (Str_E9C38_smalltit[v83x - 40].word38 & 0xff) | v102 | v100;
							v106 = (Str_E9C38_smalltit[v83x - 40].word38 & 0xff) & v102 & v101;

							if (v106 >= 0)
							{
								if (Str_E9C38_smalltit[v83x].word38 & 0x1000)
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
									v248x[20] = xunk_D4350[Str_E9C38_smalltit[v83x].byte42_std][0];
									v248x[21] = xunk_D4350[Str_E9C38_smalltit[v83x].byte42_std][1];
									v248x[14] = xunk_D4350[Str_E9C38_smalltit[v83x].byte42_std][2];
									v248x[15] = xunk_D4350[Str_E9C38_smalltit[v83x].byte42_std][3];
									v248x[8] = xunk_D4350[Str_E9C38_smalltit[v83x].byte42_std][4];
									v248x[9] = xunk_D4350[Str_E9C38_smalltit[v83x].byte42_std][5];
									v248x[2] = xunk_D4350[Str_E9C38_smalltit[v83x].byte42_std][6];
									v248x[3] = xunk_D4350[Str_E9C38_smalltit[v83x].byte42_std][7];
									x_DWORD_DE55C_ActTexture = x_DWORD_DDF50_texture_adresses[Str_E9C38_smalltit[v83x].byte41];
									if (Str_E9C38_smalltit[v83x].word38 & 1)
									{
										DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[12], &v248x[0]);
										DrawTriangleInProjectionSpace_B6253(&v248x[0], &v248x[12], &v248x[6]);
									}
									else
									{
										DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[12], &v248x[6]);
										DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[6], &v248x[0]);
									}
								}
								if (Str_E9C38_smalltit[v83x].word36)
									DrawParticles_3E360(v83x);
					}
							v83x--;
				} while (v83x >= v82x);
			}
					v57x -= 40;
					v281--;
		} while (v281);
		return;
	}
			for (k = 40; k; k--)
			{
				v33 = ((unsigned __int8)mapShading_12B4E0[v279] << 8) + 128;
				v34 = Str_E9C38_smalltit[v278x].dword12;
				v35 = v34 * v34 + Str_E9C38_smalltit[v278x].dword0_rot * Str_E9C38_smalltit[v278x].dword0_rot;
				Str_E9C38_smalltit[v278x].word36 = 0;
				if (v34 <= -256 || v35 >= str_F2C20ar.dword0x15)
				{
					Str_E9C38_smalltit[v278x].word38 |= 2u;
					goto LABEL_46;
				}
				if (v34 < 128)
					v34 = 128;
				Str_E9C38_smalltit[v278x].dword16 = str_F2C20ar.dword0x18 * Str_E9C38_smalltit[v278x].dword0_rot / v34;
				v36 = v279;
				Str_E9C38_smalltit[v278x].dword4_height = 32 * mapHeightmap_11B4E0[v279] - posZ;
				Str_E9C38_smalltit[v278x].dword8 = ((unsigned __int8)x_BYTE_14B4E0_second_heightmap[v36] << 15 >> 10) - posZ;
				v37 = 0;
				if (!mapTerrainType_10B4E0[v36])
				{
					v38 = 32 * D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dword_0x012_2BE0_11248;
					v37 = (Maths::x_DWORD_DB750[(v38 + (HIBYTE(v279) << 7)) & 0x7FF] >> 8)
						* (Maths::x_DWORD_DB750[(((unsigned __int8)v279 << 7) + v38) & 0x7FF] >> 8);
					Str_E9C38_smalltit[v278x].dword4_height -= v37 >> 13;
					if (v33 >= 14464)
						v37 = 0;
				}
				v39 = (v33 << 8) + 8 * v37;
				if (v35 <= str_F2C20ar.dword0x13)
					goto LABEL_39;
				if (v35 < str_F2C20ar.dword0x16)
				{
					v39 = v39 * (signed __int64)(str_F2C20ar.dword0x16 - v35) / str_F2C20ar.dword0x12;
				LABEL_39:
					Str_E9C38_smalltit[v278x].dword32 = v39;
					goto LABEL_40;
				}
				Str_E9C38_smalltit[v278x].dword32 = 0;
			LABEL_40:
				if (mapAngle_13B4E0[v279] & 8)
					Str_E9C38_smalltit[v278x].word38 |= 0x80u;
				v40 = str_F2C20ar.dword0x18;
				//v41 = v278;
				Str_E9C38_smalltit[v278x].dword20 = str_F2C20ar.dword0x22 + str_F2C20ar.dword0x18 * Str_E9C38_smalltit[v278x].dword4_height / v34;
				Str_E9C38_smalltit[v278x].dword28 = str_F2C20ar.dword0x22 + v40 * Str_E9C38_smalltit[v278x].dword8 / v34;
				LOBYTE(v42) = v277[2] + v279;
				HIBYTE(v42) = v277[3] + HIBYTE(v279);
				//v43 = v278;
				Str_E9C38_smalltit[v278x].byte41 = mapTerrainType_10B4E0[v42];
				if (D41A0_0.m_GameSettings.str_0x2196.flat_0x2199)
					Str_E9C38_smalltit[v278x].word38 |= 0x1000u;
				Str_E9C38_smalltit[v278x].byte43 = Maths::x_BYTE_D41D8[Str_E9C38_smalltit[v278x].byte41];
				Str_E9C38_smalltit[v278x].byte42_std = v248x[32] + (((signed int)(unsigned __int8)mapAngle_13B4E0[v42] >> 2) & 0x1C);
				LOBYTE(v42) = v277[4] + v42;
				HIBYTE(v42) += v277[5];
				Str_E9C38_smalltit[v278x].word36 = mapEntityIndex_15B4E0[v42];
			LABEL_46:
				v44 = v277;
				Str_E9C38_smalltit[v278x].word38 |= ((x_BYTE)v279 + HIBYTE(v279)) & 1;
				LOBYTE(v279) = v44[8] + v279;
				HIBYTE(v279) += v277[9];
				v278x++;
			}
			v45 = v277[6] + v279;
			HIBYTE(v279) += v277[7];
			LOBYTE(v279) = v45;
}
	}
	if (D41A0_0.m_GameSettings.m_Graphics.m_wReflections)//21de79 nothing changed
	{
		for (l = 21; ; l--)
		{
			if (!l)
			{
				v123 = 840;
				v124x = 0;
				while (v123)
				{
					v125 = ((str_F2C20ar.dword0x11 * Str_E9C38_smalltit[v124x].dword16 - str_F2C20ar.dword0x0d * Str_E9C38_smalltit[v124x].dword20) >> 16) + str_F2C20ar.dword0x24;
					v248x[25] = ((str_F2C20ar.dword0x11 * Str_E9C38_smalltit[v124x].dword16 - str_F2C20ar.dword0x0d * Str_E9C38_smalltit[v124x].dword28) >> 16) + str_F2C20ar.dword0x24;
					v126 = Str_E9C38_smalltit[v124x].dword16 * str_F2C20ar.dword0x0d;
					v248x[24] = str_F2C20ar.dword0x10 - ((v126 + str_F2C20ar.dword0x11 * Str_E9C38_smalltit[v124x].dword20) >> 16);
					v127 = str_F2C20ar.dword0x10 - ((v126 + str_F2C20ar.dword0x11 * Str_E9C38_smalltit[v124x].dword28) >> 16);
					Str_E9C38_smalltit[v124x].dword16 = v125;
					v128 = v127;
					v129 = v248x[24];
					Str_E9C38_smalltit[v124x].dword28 = v128;
					Str_E9C38_smalltit[v124x].dword20 = v129;
					v130 = Str_E9C38_smalltit[v124x].dword16;
					Str_E9C38_smalltit[v124x].dword24 = v248x[25];
					if (v130 >= 0)
					{
						if ((signed int)(unsigned __int16)viewPort.Width_DE564 <= Str_E9C38_smalltit[v124x].dword16)
							Str_E9C38_smalltit[v124x].word38 |= 0x10u;
					}
					else
					{
						Str_E9C38_smalltit[v124x].word38 |= 8u;
					}
					v131 = Str_E9C38_smalltit[v124x].dword20;
					if (v131 >= 0)
					{
						if ((unsigned __int16)viewPort.Height_DE568 <= v131)
							Str_E9C38_smalltit[v124x].word38 |= 0x40u;
					}
					else
					{
						Str_E9C38_smalltit[v124x].word38 |= 0x20u;
					}
					if (Str_E9C38_smalltit[v124x].dword24 >= 0)
					{
						if ((signed int)(unsigned __int16)viewPort.Width_DE564 <= Str_E9C38_smalltit[v124x].dword24)
							Str_E9C38_smalltit[v124x].word38 |= 0x200u;
					}
					else
					{
						Str_E9C38_smalltit[v124x].word38 |= 0x100u;
					}
					v132 = Str_E9C38_smalltit[v124x].dword28;
					if (v132 >= 0)
					{
						if ((unsigned __int16)viewPort.Height_DE568 <= v132)
							Str_E9C38_smalltit[v124x].word38 |= 0x800u;
					}
					else
					{
						Str_E9C38_smalltit[v124x].word38 |= 0x400u;
					}
					v124x++;
					v123--;
				}
				if (posZ < 4096)
				{
					v133x = 800;//35200/sizeof(type_E9C38_smalltit);
					for (m = 20; m; --m)
					{
						v134x = v133x;
						for (n = 39; n; --n)
						{
							v248x[18] = Str_E9C38_smalltit[v134x].dword24;
							v248x[19] = Str_E9C38_smalltit[v134x].dword28;
							v248x[22] = Str_E9C38_smalltit[v134x].dword32;
							v136 = Str_E9C38_smalltit[v134x].word38 & 0xff;

							if (Str_E9C38_smalltit[v134x + 1].word38 & 4)
								break;
							v248x[12] = Str_E9C38_smalltit[v134x + 1].dword24;
							v248x[13] = Str_E9C38_smalltit[v134x + 1].dword28;
							v248x[16] = Str_E9C38_smalltit[v134x + 1].dword32;
							v137 = Str_E9C38_smalltit[v134x + 1].word38 & 0xff;

							v248x[6] = Str_E9C38_smalltit[v134x - 39].dword24;//1716 //*(x_DWORD*)(v134 - 1736);
							v248x[7] = Str_E9C38_smalltit[v134x - 39].dword28;//*(x_DWORD*)(v134 - 1732);
							v248x[10] = Str_E9C38_smalltit[v134x - 39].dword32;//*(x_DWORD*)(v134 - 1728);
							v138 = (Str_E9C38_smalltit[v134x - 39].word38 & 0xff) | v137 | v136;

							v248x[0] = Str_E9C38_smalltit[v134x - 40].dword24;//*(x_DWORD*)(v134 - 1780);
							v248x[1] = Str_E9C38_smalltit[v134x - 40].dword28;//*(x_DWORD*)(v134 - 1776);
							v248x[4] = Str_E9C38_smalltit[v134x - 40].dword32;//*(x_DWORD*)(v140 - 12);
							v142 = Str_E9C38_smalltit[v134x - 40].word38 & 0xff;
							v144 = v142 | v138;

							/*
							uint8_t origbyte20 = 0;
							uint8_t remakebyte20 = 0;
							int comp20;
							//if (debug_counter_1f3e70 >= 0xb5)
							{
								comp20 = compare_with_sequence((char*)"0021E378-003F52A4", (uint8_t*)Str_E9C38_smalltit, 0x2dc4e0, debug_sub_3C080, 44*40*21, 44 * 40 * 21, &origbyte20, &remakebyte20);
							}

							if (debug_sub_3C080 >= 0x12d3e)
							{
								debug_sub_3C080++;
								debug_sub_3C080--;
							}
							debug_sub_3C080++;*/

							//0x21e378

							if (Str_E9C38_smalltit[v134x].byte41)//(*(x_BYTE*)(v143 + 41))
							{
								if (Str_E9C38_smalltit[v134x].word38 & 0x1000)
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
									{
										v248x[20] = xunk_D4350[Str_E9C38_smalltit[v134x].byte42_std][0];
										v248x[21] = xunk_D4350[Str_E9C38_smalltit[v134x].byte42_std][1];
										v248x[14] = xunk_D4350[Str_E9C38_smalltit[v134x].byte42_std][2];
										v248x[15] = xunk_D4350[Str_E9C38_smalltit[v134x].byte42_std][3];
										v248x[8] = xunk_D4350[Str_E9C38_smalltit[v134x].byte42_std][4];
										v248x[9] = xunk_D4350[Str_E9C38_smalltit[v134x].byte42_std][5];
										v248x[2] = xunk_D4350[Str_E9C38_smalltit[v134x].byte42_std][6];
										v248x[3] = xunk_D4350[Str_E9C38_smalltit[v134x].byte42_std][7];
										x_DWORD_DE55C_ActTexture = x_DWORD_DDF50_texture_adresses[Str_E9C38_smalltit[v134x].byte41];
										x_BYTE_E126D = 5;
										//allert_error();
										if (Str_E9C38_smalltit[v134x].word38 & 1)
										{
											DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[0], &v248x[12]);
											DrawTriangleInProjectionSpace_B6253(&v248x[0], &v248x[6], &v248x[12]);
										}
										else
										{
											DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[6], &v248x[12]);
											DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[0], &v248x[6]);
										}
									}
								}
							}
							/*if (v143 - &x_DWORD_E9C38_smalltit[35200] == 0xffffffffffff85f8)
							{
								v143++;
								v143--;
							}*/
							if (Str_E9C38_smalltit[v134x].word36)
								sub_3FD60(v134x);
							v134x++;
						}
						if (n)
						{
							v25z = v134x;// -1;
							v147x = v133x + 38;
							do
							{
								v248x[18] = Str_E9C38_smalltit[v147x].dword24;//*(x_DWORD*)(v147 + 24);
								v248x[19] = Str_E9C38_smalltit[v147x].dword28;//*(x_DWORD*)(v147 + 28);
								v248x[22] = Str_E9C38_smalltit[v147x].dword32;//*(x_DWORD*)(v147 + 32);
								v148 = Str_E9C38_smalltit[v147x].word38 & 0xff;//*(x_BYTE*)(v147 + 38);

								v248x[12] = Str_E9C38_smalltit[v147x + 1].dword24;//*(x_DWORD*)(v147 + 68);
								v248x[13] = Str_E9C38_smalltit[v147x + 1].dword28;//*(x_DWORD*)(v147 + 72);
								v248x[16] = Str_E9C38_smalltit[v147x + 1].dword32;//*(x_DWORD*)(v147 + 76);
								v149 = (Str_E9C38_smalltit[v147x + 1].word38 & 0xff) | v148;//*(x_BYTE*)(v147 + 82) | v148;

								v248x[6] = Str_E9C38_smalltit[v147x - 39].dword24;//*(x_DWORD*)(v147 - 1692);
								v248x[7] = Str_E9C38_smalltit[v147x - 39].dword28;//*(x_DWORD*)(v147 - 1688);
								v248x[10] = Str_E9C38_smalltit[v147x - 39].dword32;//*(x_DWORD*)(v151 - 1728);
								v153 = Str_E9C38_smalltit[v147x - 39].word38 & 0xff;//*(x_BYTE*)(v151 + 38);

								v248x[0] = Str_E9C38_smalltit[v147x - 40].dword24;//*(x_DWORD*)(v151 - 20);
								v248x[1] = Str_E9C38_smalltit[v147x - 40].dword28;//*(x_DWORD*)(v151 - 16);
								v248x[4] = Str_E9C38_smalltit[v147x - 40].dword32;//*(x_DWORD*)(v151 + 32);
								v156 = (Str_E9C38_smalltit[v147x - 40].word38 & 0xff) | v153 | v149;
								if (Str_E9C38_smalltit[v147x].byte41)
								{
									if (Str_E9C38_smalltit[v147x].word38 & 0x1000)
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
										{
											v248x[20] = xunk_D4350[Str_E9C38_smalltit[v147x].byte42_std][0];
											v248x[21] = xunk_D4350[Str_E9C38_smalltit[v147x].byte42_std][1];
											v248x[14] = xunk_D4350[Str_E9C38_smalltit[v147x].byte42_std][2];
											v248x[15] = xunk_D4350[Str_E9C38_smalltit[v147x].byte42_std][3];
											v248x[8] = xunk_D4350[Str_E9C38_smalltit[v147x].byte42_std][4];
											v248x[9] = xunk_D4350[Str_E9C38_smalltit[v147x].byte42_std][5];
											v248x[2] = xunk_D4350[Str_E9C38_smalltit[v147x].byte42_std][6];
											v248x[3] = xunk_D4350[Str_E9C38_smalltit[v147x].byte42_std][7];
											x_BYTE_E126D = 5;
											x_DWORD_DE55C_ActTexture = x_DWORD_DDF50_texture_adresses[Str_E9C38_smalltit[v147x].byte41];
											//allert_error();
											if (Str_E9C38_smalltit[v147x].word38 & 1)
											{
												DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[0], &v248x[12]);
												DrawTriangleInProjectionSpace_B6253(&v248x[0], &v248x[6], &v248x[12]);
											}
											else
											{
												DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[6], &v248x[12]);
												DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[0], &v248x[6]);
											}
										}
									}
								}
								if (Str_E9C38_smalltit[v147x].word36)
									sub_3FD60(v147x);
								v147x--;
							} while (v147x >= v25z);
						}
						v133x -= 40;
					}
				}
				v160x = 800;
				v282 = 20;
				do
				{
					v161x = v160x;
					for (ii = 39; ii; --ii)
					{
						v248x[18] = Str_E9C38_smalltit[v161x].dword16;//*(x_DWORD*)(v161 + 16);
						v248x[19] = Str_E9C38_smalltit[v161x].dword20;//*(x_DWORD*)(v161 + 20);
						v248x[22] = Str_E9C38_smalltit[v161x].dword32;//*(x_DWORD*)(v161 + 32);
						v163 = Str_E9C38_smalltit[v161x].word38 & 0xff;//*(x_BYTE*)(v161 - 6);

						/*if (debugcounter_sub_particle >= 0xf)
						{
							debugcounter_sub_particle++;
							debugcounter_sub_particle--;
						}

						debugcounter_sub_particle++;*/

						if (Str_E9C38_smalltit[v161x + 1].word38 & 4)
							break;

						v248x[12] = Str_E9C38_smalltit[v161x + 1].dword16;//*(x_DWORD*)(v161 + 16);
						v248x[13] = Str_E9C38_smalltit[v161x + 1].dword20;//*(x_DWORD*)(v161 + 20);
						v248x[16] = Str_E9C38_smalltit[v161x + 1].dword32;//*(x_DWORD*)(v161 + 32);
						v165 = Str_E9C38_smalltit[v161x + 1].word38 & 0xff;
						v166 = v165 | v163;
						v167 = v165 & v163;

						v248x[6] = Str_E9C38_smalltit[v161x - 39].dword16;//*(x_DWORD*)(v161 - 1744);
						v248x[7] = Str_E9C38_smalltit[v161x - 39].dword20;//*(x_DWORD*)(v161 - 1740);
						v248x[10] = Str_E9C38_smalltit[v161x - 39].dword32;//*(x_DWORD*)(v161 - 1728);
						v170 = Str_E9C38_smalltit[v161x - 39].word38 & 0xff;//*(x_BYTE*)(v169 + 38);

						v248x[0] = Str_E9C38_smalltit[v161x - 40].dword16;//*(x_DWORD*)(v169 - 28);
						v248x[1] = Str_E9C38_smalltit[v161x - 40].dword20;//*(x_DWORD*)(v169 - 24);
						v248x[4] = Str_E9C38_smalltit[v161x - 40].dword32;
						v173 = (Str_E9C38_smalltit[v161x - 40].word38 & 0xff) | v170 | v166;
						v174 = (Str_E9C38_smalltit[v161x - 40].word38 & 0xff) & v170 & v167;

						if (!(Str_E9C38_smalltit[v161x].word38 & 0x80))
						{
							if (Str_E9C38_smalltit[v161x].word38 & 0x1000)
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
								v248x[20] = xunk_D4350[Str_E9C38_smalltit[v161x].byte42_std][0];
								v248x[21] = xunk_D4350[Str_E9C38_smalltit[v161x].byte42_std][1];
								v248x[14] = xunk_D4350[Str_E9C38_smalltit[v161x].byte42_std][2];
								v248x[15] = xunk_D4350[Str_E9C38_smalltit[v161x].byte42_std][3];
								v248x[8] = xunk_D4350[Str_E9C38_smalltit[v161x].byte42_std][4];
								v248x[9] = xunk_D4350[Str_E9C38_smalltit[v161x].byte42_std][5];
								v248x[2] = xunk_D4350[Str_E9C38_smalltit[v161x].byte42_std][6];
								v248x[3] = xunk_D4350[Str_E9C38_smalltit[v161x].byte42_std][7];
								x_DWORD_DE55C_ActTexture = x_DWORD_DDF50_texture_adresses[Str_E9C38_smalltit[v161x].byte41];
								if (Str_E9C38_smalltit[v161x].word38 & 1)
								{
									DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[12], &v248x[0]);
									DrawTriangleInProjectionSpace_B6253(&v248x[0], &v248x[12], &v248x[6]);
								}
								else
								{
									DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[12], &v248x[6]);
									DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[6], &v248x[0]);
								}
							}
						}
						else
						{
							x_BYTE_E126D = 26;
							if (!(v173 & 2) && !(v174 & 0x78))
							{
								v248x[20] = xunk_D4350[Str_E9C38_smalltit[v161x].byte42_std][0];
								v248x[21] = xunk_D4350[Str_E9C38_smalltit[v161x].byte42_std][1];
								v248x[14] = xunk_D4350[Str_E9C38_smalltit[v161x].byte42_std][2];
								v248x[15] = xunk_D4350[Str_E9C38_smalltit[v161x].byte42_std][3];
								v248x[8] = xunk_D4350[Str_E9C38_smalltit[v161x].byte42_std][4];
								v248x[9] = xunk_D4350[Str_E9C38_smalltit[v161x].byte42_std][5];
								v248x[2] = xunk_D4350[Str_E9C38_smalltit[v161x].byte42_std][6];
								v248x[3] = xunk_D4350[Str_E9C38_smalltit[v161x].byte42_std][7];
								x_DWORD_DE55C_ActTexture = x_DWORD_DDF50_texture_adresses[Str_E9C38_smalltit[v161x].byte41];
								if (Str_E9C38_smalltit[v161x].word38 & 1)
								{
									DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[12], &v248x[0]);
									DrawTriangleInProjectionSpace_B6253(&v248x[0], &v248x[12], &v248x[6]);
								}
								else
								{
									DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[12], &v248x[6]);
									DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[6], &v248x[0]);
								}
						}
					}
						if (Str_E9C38_smalltit[v161x].word36)
							DrawParticles_3E360(/*v160,*/ v161x);
						v161x++;
				}
					if (ii)
					{
						v177x = v161x;// -1;
						v178x = v160x + 38;
						do
						{
							v248x[18] = Str_E9C38_smalltit[v178x].dword16;//*(x_DWORD*)(v178 + 16);
							v248x[19] = Str_E9C38_smalltit[v178x].dword20;//*(x_DWORD*)(v178 + 20);
							v248x[22] = Str_E9C38_smalltit[v178x].dword32;//*(x_DWORD*)(v178 + 32);
							v179 = Str_E9C38_smalltit[v178x].word38 & 0xff;//*(x_BYTE*)(v178 + 38);

							v248x[12] = Str_E9C38_smalltit[v178x + 1].dword16;//*(x_DWORD*)(v178 + 60);
							v248x[13] = Str_E9C38_smalltit[v178x + 1].dword20;//*(x_DWORD*)(v178 + 64);
							v248x[16] = Str_E9C38_smalltit[v178x + 1].dword32;//*(x_DWORD*)(v178 + 76);
							v180 = Str_E9C38_smalltit[v178x + 1].word38 & 0xff;//*(x_BYTE*)(v178 + 82);

							v248x[6] = Str_E9C38_smalltit[v178x - 39].dword16;//*(x_DWORD*)(v178 - 1700);
							v248x[7] = Str_E9C38_smalltit[v178x - 39].dword20;//*(x_DWORD*)(v178 - 1696);
							v248x[10] = Str_E9C38_smalltit[v178x - 39].dword32;//*(x_DWORD*)(v178 - 1684);
							v182 = Str_E9C38_smalltit[v178x - 39].word38 & 0xff;//*(x_BYTE*)(v178 - 1678);
							v183 = v182 | v180 | v179;
							v184 = v182 & v180 & v179;

							v248x[0] = Str_E9C38_smalltit[v178x - 40].dword16;//*(x_DWORD*)(v178 - 1744);
							v248x[1] = Str_E9C38_smalltit[v178x - 40].dword20;//*(x_DWORD*)(v186 - 1784);
							v248x[4] = Str_E9C38_smalltit[v178x - 40].dword32;//*(x_DWORD*)(v186 - 12);
							v189 = Str_E9C38_smalltit[v178x - 40].word38 & 0xff;//*(x_BYTE*)(v186 + 38);
							v191 = v189 | v183;
							v192 = v189 & v184;
							if (!(Str_E9C38_smalltit[v178x].word38 & 0x80))
							{
								if (Str_E9C38_smalltit[v178x].word38 & 0x1000)
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
									v248x[20] = xunk_D4350[Str_E9C38_smalltit[v178x].byte42_std][0];
									v248x[21] = xunk_D4350[Str_E9C38_smalltit[v178x].byte42_std][1];
									v248x[14] = xunk_D4350[Str_E9C38_smalltit[v178x].byte42_std][2];
									v248x[15] = xunk_D4350[Str_E9C38_smalltit[v178x].byte42_std][3];
									v248x[8] = xunk_D4350[Str_E9C38_smalltit[v178x].byte42_std][4];
									v248x[9] = xunk_D4350[Str_E9C38_smalltit[v178x].byte42_std][5];
									v248x[2] = xunk_D4350[Str_E9C38_smalltit[v178x].byte42_std][6];
									v248x[3] = xunk_D4350[Str_E9C38_smalltit[v178x].byte42_std][7];
									x_DWORD_DE55C_ActTexture = x_DWORD_DDF50_texture_adresses[Str_E9C38_smalltit[v178x].byte41];
									if (Str_E9C38_smalltit[v178x].word38 & 1)
									{
										DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[12], &v248x[0]);
										DrawTriangleInProjectionSpace_B6253(&v248x[0], &v248x[12], &v248x[6]);
									}
									else
									{
										DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[12], &v248x[6]);
										DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[6], &v248x[0]);
									}
								}
							}
							else
							{
								x_BYTE_E126D = 26;
								if (!(v191 & 2) && !(v192 & 0x78))
								{
									v248x[20] = xunk_D4350[Str_E9C38_smalltit[v178x].byte42_std][0];
									v248x[21] = xunk_D4350[Str_E9C38_smalltit[v178x].byte42_std][1];
									v248x[14] = xunk_D4350[Str_E9C38_smalltit[v178x].byte42_std][2];
									v248x[15] = xunk_D4350[Str_E9C38_smalltit[v178x].byte42_std][3];
									v248x[8] = xunk_D4350[Str_E9C38_smalltit[v178x].byte42_std][4];
									v248x[9] = xunk_D4350[Str_E9C38_smalltit[v178x].byte42_std][5];
									v248x[2] = xunk_D4350[Str_E9C38_smalltit[v178x].byte42_std][6];
									v248x[3] = xunk_D4350[Str_E9C38_smalltit[v178x].byte42_std][7];
									v194 = Str_E9C38_smalltit[v178x].word38 & 0xff;
									x_DWORD_DE55C_ActTexture = x_DWORD_DDF50_texture_adresses[Str_E9C38_smalltit[v178x].byte41];
									if (v194 & 1)
									{
										DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[12], &v248x[0]);
										DrawTriangleInProjectionSpace_B6253(&v248x[0], &v248x[12], &v248x[6]);
									}
									else
									{
										DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[12], &v248x[6]);
										DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[6], &v248x[0]);
									}
								}
							}
							if (Str_E9C38_smalltit[v178x].word36)
								DrawParticles_3E360(v178x);
							v178x--;
						} while (v178x >= v177x);
					}
					v160x -= 40;
					v282--;
			} while (v282);
			return;
		}
			for (jj = 40; jj; --jj)
			{
				v248x[31] = ((unsigned __int8)mapShading_12B4E0[v279] << 8) + 128;
				v109 = Str_E9C38_smalltit[v278x].dword12;
				v110 = v109 * v109 + Str_E9C38_smalltit[v278x].dword0_rot * Str_E9C38_smalltit[v278x].dword0_rot;
				Str_E9C38_smalltit[v278x].word36 = 0;
				if (v109 <= -256 || v110 >= str_F2C20ar.dword0x15)
				{
					Str_E9C38_smalltit[v278x].word38 |= 2u;
					goto LABEL_140;
				}
				if (v109 < 128)
					v109 = 128;
				Str_E9C38_smalltit[v278x].dword16 = str_F2C20ar.dword0x18 * Str_E9C38_smalltit[v278x].dword0_rot / v109;
				v111 = v279;
				Str_E9C38_smalltit[v278x].dword4_height = 32 * mapHeightmap_11B4E0[v279] - posZ;
				v112 = (unsigned __int16)D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dword_0x012_2BE0_11248 << 6;
				v248x[26] = Maths::x_DWORD_DB750[(v112 + (HIBYTE(v279) << 7)) & 0x7FF] >> 8;
				v113 = v248x[26] * (Maths::x_DWORD_DB750[(((unsigned __int8)v279 << 7) + v112) & 0x7FF] >> 8);
				v248x[26] = mapHeightmap_11B4E0[v111];
				Str_E9C38_smalltit[v278x].dword8 = -(v248x[26] * ((v113 >> 4) + 0x8000) >> 10) - posZ;
				if (!(mapAngle_13B4E0[v111] & 8)
					|| (//v114 = (int32_t*)v278,
						//v25y = (uint32_t*)(((uint32_t*)v278)[1] - (v113 >> 10)),
						//v115 = v248x[31],
						(Str_E9C38_smalltit[v278x].dword4_height -= (v113 >> 10)),
						v248x[31] >= 14464))
					/*if (!(x_BYTE_13B4E0_angle[v111] & 8)
						|| (v114 = (int32_t*)v25y, v25y = (uint32_t*)(v25y[1] - (v113 >> 10)), v115 = v248x[31], v114[1] = (x_DWORD)v25y, v115 >= 14464))*/
				{
					v113 = 0;
				}
				v116 = (v248x[31] << 8) + 8 * v113;
				if (v110 <= str_F2C20ar.dword0x13)
					goto LABEL_133;
				if (v110 < str_F2C20ar.dword0x16)
				{
					v116 = v116 * (signed __int64)(str_F2C20ar.dword0x16 - v110) / str_F2C20ar.dword0x12;
				LABEL_133:
					Str_E9C38_smalltit[v278x].dword32 = v116;
					goto LABEL_134;
				}
				Str_E9C38_smalltit[v278x].dword32 = 0;
			LABEL_134:
				Str_E9C38_smalltit[v278x].dword20 = str_F2C20ar.dword0x22 + str_F2C20ar.dword0x18 * Str_E9C38_smalltit[v278x].dword4_height / v109;
				Str_E9C38_smalltit[v278x].dword28 = str_F2C20ar.dword0x22 + str_F2C20ar.dword0x18 * Str_E9C38_smalltit[v278x].dword8 / v109;
				LOBYTE(v118) = v277[2] + v279;
				HIBYTE(v118) = v277[3] + HIBYTE(v279);
				v119 = mapTerrainType_10B4E0[v118];
				Str_E9C38_smalltit[v278x].byte41 = v119;
				if (Maths::x_BYTE_D41D8[0xa4 + v119])
					Str_E9C38_smalltit[v278x].word38 |= 0x80u;
				if (D41A0_0.m_GameSettings.str_0x2196.flat_0x2199)
					Str_E9C38_smalltit[v278x].word38 |= 0x1000u;
				Str_E9C38_smalltit[v278x].byte43 = Maths::x_BYTE_D41D8[Str_E9C38_smalltit[v278x].byte41];
				Str_E9C38_smalltit[v278x].byte42_std = v248x[32] + (((signed int)(unsigned __int8)mapAngle_13B4E0[v118] >> 2) & 0x1C);
				LOBYTE(v118) = v277[4] + v118;
				HIBYTE(v118) += v277[5];
				Str_E9C38_smalltit[v278x].word36 = mapEntityIndex_15B4E0[v118];
			LABEL_140:
				v121 = v277;
				Str_E9C38_smalltit[v278x].word38 |= ((x_BYTE)v279 + HIBYTE(v279)) & 1;
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
			v197 = ((unsigned __int8)mapShading_12B4E0[v279] << 8) + 128;
			v198 = Str_E9C38_smalltit[v278x].dword12;
			v199 = v198 * v198 + Str_E9C38_smalltit[v278x].dword0_rot * Str_E9C38_smalltit[v278x].dword0_rot;
			Str_E9C38_smalltit[v278x].word36 = 0;
			if (v198 > -256 && v199 < str_F2C20ar.dword0x15)
				break;
			Str_E9C38_smalltit[v278x].word38 |= 2u;
		LABEL_256:
			v207 = v277;
			Str_E9C38_smalltit[v278x].word38 |= ((x_BYTE)v279 + HIBYTE(v279)) & 1;
			LOBYTE(v279) = v207[8] + v279;
			HIBYTE(v279) += v277[9];
			v285--;
			v278x++;
		}
		if (v198 < 128)
			v198 = 128;
		v200 = v279;
		Str_E9C38_smalltit[v278x].dword16 = str_F2C20ar.dword0x18 * Str_E9C38_smalltit[v278x].dword0_rot / v198;
		Str_E9C38_smalltit[v278x].dword4_height = 32 * mapHeightmap_11B4E0[v200] - posZ;
		v201 = (unsigned __int16)D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].dword_0x012_2BE0_11248 << 6;
		v248x[26] = Maths::x_DWORD_DB750[(v201 + (HIBYTE(v279) << 7)) & 0x7FF] >> 8;
		v202 = v248x[26] * (Maths::x_DWORD_DB750[(((unsigned __int8)v279 << 7) + v201) & 0x7FF] >> 8);
		if (!(mapAngle_13B4E0[v200] & 8) || (Str_E9C38_smalltit[v278x].dword4_height -= v202 >> 10, v197 >= 14464))
			v202 = 0;
		v203 = (v197 << 8) + 8 * v202;
		if (v199 > str_F2C20ar.dword0x13)
		{
			if (v199 >= str_F2C20ar.dword0x16)
			{
				Str_E9C38_smalltit[v278x].dword32 = 0;
			LABEL_254:
				Str_E9C38_smalltit[v278x].dword20 = str_F2C20ar.dword0x22 + str_F2C20ar.dword0x18 * Str_E9C38_smalltit[v278x].dword4_height / v198;
				LOBYTE(v204) = v277[2] + v279;
				HIBYTE(v204) = v277[3] + HIBYTE(v279);
				Str_E9C38_smalltit[v278x].byte41 = mapTerrainType_10B4E0[v204];
				Str_E9C38_smalltit[v278x].byte43 = Maths::x_BYTE_D41D8[Str_E9C38_smalltit[v278x].byte41];
				Str_E9C38_smalltit[v278x].byte42_std = v248x[32] + (((signed int)(unsigned __int8)mapAngle_13B4E0[v204] >> 2) & 0x1C);
				LOBYTE(v204) = v277[4] + v204;
				HIBYTE(v204) += v277[5];
				Str_E9C38_smalltit[v278x].word36 = mapEntityIndex_15B4E0[v204];
				goto LABEL_256;
			}
			v203 = v203 * (signed __int64)(str_F2C20ar.dword0x16 - v199) / str_F2C20ar.dword0x12;
		}
		Str_E9C38_smalltit[v278x].dword32 = v203;
		goto LABEL_254;
	}//21edb7 nothing changed
	v208 = roll & 0x7FF;//21edb7
	v209 = 840;
	v210 = Maths::x_DWORD_DB750[v208];
	v211 = Maths::x_DWORD_DB750[0x200 + v208];
	str_F2C20ar.dword0x0d = v210;
	str_F2C20ar.dword0x11 = v211;
	v56x = 0;
	while (v209)
	{
		v212 = str_F2C20ar.dword0x10;
		v213 = (str_F2C20ar.dword0x0d * Str_E9C38_smalltit[v56x].dword16 + str_F2C20ar.dword0x11 * Str_E9C38_smalltit[v56x].dword20) >> 16;
		Str_E9C38_smalltit[v56x].dword16 = ((Str_E9C38_smalltit[v56x].dword16 * str_F2C20ar.dword0x11 - str_F2C20ar.dword0x0d * Str_E9C38_smalltit[v56x].dword20) >> 16)
			+ str_F2C20ar.dword0x24;
		v214 = v212 - v213;
		v215 = Str_E9C38_smalltit[v56x].dword16;
		Str_E9C38_smalltit[v56x].dword20 = v214;
		if (v215 >= 0)
		{
			if ((signed int)(unsigned __int16)viewPort.Width_DE564 <= Str_E9C38_smalltit[v56x].dword16)
				Str_E9C38_smalltit[v56x].word38 |= 0x10u;
		}
		else
		{
			Str_E9C38_smalltit[v56x].word38 |= 8u;
		}
		v216 = Str_E9C38_smalltit[v56x].dword20;
		if (v216 >= 0)
		{
			if ((unsigned __int16)viewPort.Height_DE568 <= v216)
				Str_E9C38_smalltit[v56x].word38 |= 0x40u;
		}
		else
		{
			Str_E9C38_smalltit[v56x].word38 |= 0x20u;
		}
		v56x++;
		v209--;
	}
	//adress 3de7d
	//v217 = &x_DWORD_E9C38_smalltit[35200];//21ee85
	v217x = 800;
	v289 = 20;
	do
	{
		v218x = v217x;
		for (kk = 39; kk; kk--)
		{
			v248x[18] = Str_E9C38_smalltit[v218x].dword16;//*(x_DWORD*)(v218 + 16);
			v248x[19] = Str_E9C38_smalltit[v218x].dword20;//*(x_DWORD*)(v218 + 20);
			v248x[22] = Str_E9C38_smalltit[v218x].dword32;//*(x_DWORD*)(v218 + 32);
			v219 = Str_E9C38_smalltit[v218x].word38 & 0xff;//*(x_BYTE*)(v218 - 6);

			if (Str_E9C38_smalltit[v218x + 1].word38 & 4)
				break;
			v248x[12] = Str_E9C38_smalltit[v218x + 1].dword16;//*(x_DWORD*)(v218 + 16);
			v248x[13] = Str_E9C38_smalltit[v218x + 1].dword20;//*(x_DWORD*)(v218 + 20);
			v248x[16] = Str_E9C38_smalltit[v218x + 1].dword32;//*(x_DWORD*)(v218 + 32);
			v221 = Str_E9C38_smalltit[v218x + 1].word38 & 0xff;//*(x_BYTE*)(v218 + 38);
			v222 = v221 | v219;
			v223 = v221 & v219;

			v248x[6] = Str_E9C38_smalltit[v218x - 39].dword16;//*(x_DWORD*)(v218 - 1744);
			v248x[7] = Str_E9C38_smalltit[v218x - 39].dword20;//*(x_DWORD*)(v218 - 1740);
			v248x[10] = Str_E9C38_smalltit[v218x - 39].dword32;//*(x_DWORD*)(v218 - 1728);
			v226 = Str_E9C38_smalltit[v218x - 39].word38 & 0xff;//*(x_BYTE*)(v225 + 38);

			v248x[0] = Str_E9C38_smalltit[v218x - 40].dword16;//*(x_DWORD*)(v225 - 28);
			v248x[1] = Str_E9C38_smalltit[v218x - 40].dword20;//*(x_DWORD*)(v225 - 24);
			v248x[4] = Str_E9C38_smalltit[v218x - 40].dword32;//*(x_DWORD*)(v225 + 32);
			v228 = (Str_E9C38_smalltit[v218x - 40].word38 & 0xff) | v226 | v222;//(*(x_BYTE*)(v225 + 38)) | v226 | v222;
			v229 = (Str_E9C38_smalltit[v218x - 40].word38 & 0xff) & v226 & v223;//(*(x_BYTE*)(v225 + 38)) & v226 & v223;
			if (Str_E9C38_smalltit[v218x].word38 & 0x1000)
			{
				x_BYTE_E126D = 7;
				x_BYTE_E126C = ((signed int)v248x[10] + v248x[16] + v248x[22] + v248x[4]) >> 18;
			}
			else
			{
				x_BYTE_E126D = 5;
			}
			if (!(v228 & 2) && !(v229 & 0x78))
			{//21ef76
				v248x[20] = xunk_D4350[Str_E9C38_smalltit[v218x].byte42_std][0];
				v248x[21] = xunk_D4350[Str_E9C38_smalltit[v218x].byte42_std][1];
				v248x[14] = xunk_D4350[Str_E9C38_smalltit[v218x].byte42_std][2];
				v248x[15] = xunk_D4350[Str_E9C38_smalltit[v218x].byte42_std][3];
				v248x[8] = xunk_D4350[Str_E9C38_smalltit[v218x].byte42_std][4];
				v248x[9] = xunk_D4350[Str_E9C38_smalltit[v218x].byte42_std][5];
				v248x[2] = xunk_D4350[Str_E9C38_smalltit[v218x].byte42_std][6];
				v248x[3] = xunk_D4350[Str_E9C38_smalltit[v218x].byte42_std][7];
				x_DWORD_DE55C_ActTexture = x_DWORD_DDF50_texture_adresses[Str_E9C38_smalltit[v218x].byte41];
				if ((Str_E9C38_smalltit[v218x].word38 & 0xff) & 1)
				{//adress 21ddf0
					DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[12], &v248x[0]);
					DrawTriangleInProjectionSpace_B6253(&v248x[0], &v248x[12], &v248x[6]);
				}
				else
				{
					DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[12], &v248x[6]);
					DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[6], &v248x[0]);
				}
			}
			if (Str_E9C38_smalltit[v218x].word36)
				DrawParticles_3E360(v218x);//21f01b
			v218x++;
		}
		//VGA_Debug_Blit(640, 480, x_DWORD_180628b_screen_buffer);
		if (kk)
		{
			v231x = v218x;// -1;
			v232x = v217x + 38;
			do
			{
				v248x[18] = Str_E9C38_smalltit[v232x].dword16;//*(x_DWORD*)(v232 + 16);
				v248x[19] = Str_E9C38_smalltit[v232x].dword20;//*(x_DWORD*)(v232 + 20);
				v248x[22] = Str_E9C38_smalltit[v232x].dword32;//*(x_DWORD*)(v232 + 32);
				v233 = Str_E9C38_smalltit[v232x].word38 & 0xff;//*(x_BYTE*)(v232 + 38);

				v248x[12] = Str_E9C38_smalltit[v232x + 1].dword16;//*(x_DWORD*)(v232 + 60);
				v248x[13] = Str_E9C38_smalltit[v232x + 1].dword20;//*(x_DWORD*)(v232 + 64);
				v248x[16] = Str_E9C38_smalltit[v232x + 1].dword32;//*(x_DWORD*)(v232 + 76);
				v234 = Str_E9C38_smalltit[v232x + 1].word38 & 0xff;//*(x_BYTE*)(v232 + 82);

				v248x[6] = Str_E9C38_smalltit[v232x - 39].dword16;//*(x_DWORD*)(v232 - 1700);
				v248x[7] = Str_E9C38_smalltit[v232x - 39].dword20;//*(x_DWORD*)(v232 - 1696);
				v248x[10] = Str_E9C38_smalltit[v232x - 39].dword32;//*(x_DWORD*)(v232 - 1684);
				v236 = Str_E9C38_smalltit[v232x - 39].word38 & 0xff;//*(x_BYTE*)(v232 - 1678);
				v237 = v236 | v234 | v233;
				v238 = v236 & v234 & v233;

				v248x[0] = Str_E9C38_smalltit[v232x - 40].dword16;//*(x_DWORD*)(v232 - 1744);
				v248x[1] = Str_E9C38_smalltit[v232x - 40].dword20;//*(x_DWORD*)(v240 - 1784);
				v248x[4] = Str_E9C38_smalltit[v232x - 40].dword32;//*(x_DWORD*)(v240 - 12);
				v242 = Str_E9C38_smalltit[v232x - 40].word38 & 0xff;//*(x_BYTE*)(v240 + 38);
				v244 = v242 | v237;
				v245 = v242 & v238;

				if (Str_E9C38_smalltit[v232x].word38 & 0x1000)
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
					v248x[20] = xunk_D4350[Str_E9C38_smalltit[v232x].byte42_std][0];
					v248x[21] = xunk_D4350[Str_E9C38_smalltit[v232x].byte42_std][1];
					v248x[14] = xunk_D4350[Str_E9C38_smalltit[v232x].byte42_std][2];
					v248x[15] = xunk_D4350[Str_E9C38_smalltit[v232x].byte42_std][3];
					v248x[8] = xunk_D4350[Str_E9C38_smalltit[v232x].byte42_std][4];
					v248x[9] = xunk_D4350[Str_E9C38_smalltit[v232x].byte42_std][5];
					v248x[2] = xunk_D4350[Str_E9C38_smalltit[v232x].byte42_std][6];
					v248x[3] = xunk_D4350[Str_E9C38_smalltit[v232x].byte42_std][7];
					x_DWORD_DE55C_ActTexture = x_DWORD_DDF50_texture_adresses[Str_E9C38_smalltit[v232x].byte41];
					if (Str_E9C38_smalltit[v232x].word38 & 1)
					{
						DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[12], &v248x[0]);
						DrawTriangleInProjectionSpace_B6253(&v248x[0], &v248x[12], &v248x[6]);
					}
					else
					{
						DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[12], &v248x[6]);
						DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[6], &v248x[0]);
					}
				}
				if (Str_E9C38_smalltit[v232x].word36)//adress 21f1b5 aex 360000 ebx 3f78a0 ecx 0 edx 414eb0
					DrawParticles_3E360(/*v217,*/ v232x);
				v232x--;
			} while (v232x >= v231x);
		}
		v217x -= 40;
		v289--;
	} while (v289);
}

int32_t* GameRender_old::x_DWORD_DB350_ret(uint32_t adress) {
	if (adress >= 0x100) return &Maths::x_DWORD_DB750[adress - 0x100];
	allert_error();
}

uint16_t GameRender_old::sub_3FD60(int a2x)
{
	unsigned __int16 result; // ax
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
	int v38; // eax
	unsigned __int8 v39; // al
	int v40; // [esp+0h] [ebp-Ch]
	type_event_0x6E8E* v41x; // [esp+4h] [ebp-8h]
	int v42; // [esp+8h] [ebp-4h]

	//fix
	v41x = 0;
	type_particle_str* a1x = 0;
	//fix


	uint8_t origbyte20 = 0;
	uint8_t remakebyte20 = 0;
	int comp20;
	//if (debug_counter_1f3e70 >= 0xb5)
	/*{
		comp20 = compare_with_sequence((char*)"00220D6C-FFFFFF04", (uint8_t*)&Str_E9C38_smalltit[a2x], 0x2dc4e0, debug_sub_3FD60B, 44, 44, &origbyte20, &remakebyte20);
	}
	if (debug_sub_3FD60B >= 0xa2b)
	{
		debug_sub_3FD60B++;
		debug_sub_3FD60B--;
	}
	debug_sub_3FD60B++;*/
	//add_compare(0x297272, debugafterload);


	result = Str_E9C38_smalltit[a2x].word36;
	do
	{
		//debug
		//0x220d7a
		/*if (debug_sub_3FD60B2 == 0xa7e)
		{
			result++;
			result--;
		}
		debug_sub_3FD60B2++;*/
		//debug

		//add_compare(0x220d70, debugafterload);

		if (result < 0x3E8u)
		{
			v3x = x_DWORD_EA3E4[result];
			v41x = v3x;
			if (!(v3x->struct_byte_0xc_12_15.byte[0] & 0x21))
			{
				v4 = (signed __int16)(v3x->axis_0x4C_76.x - x_WORD_F2CC4);
				v5 = (signed __int16)(x_WORD_F2CC2 - v3x->axis_0x4C_76.y);
				v42 = -v3x->axis_0x4C_76.z - str_F2C20ar.dword0x20;
				v6 = (v4 * str_F2C20ar.dword0x0f - v5 * str_F2C20ar.dword0x17) >> 16;
				v40 = (str_F2C20ar.dword0x17 * v4 + str_F2C20ar.dword0x0f * v5) >> 16;
				v7 = (str_F2C20ar.dword0x17 * v4 + str_F2C20ar.dword0x0f * v5) >> 16;
				v8 = v40 * v40 + v6 * v6;
				if (v7 > 64 && v8 < str_F2C20ar.dword0x15)
				{
					if (v8 <= str_F2C20ar.dword0x13)
					{
						str_F2C20ar.dword0x00 = 0x2000;
					}
					else if (v8 < str_F2C20ar.dword0x16)
					{
						str_F2C20ar.dword0x00 = 32 * (str_F2C20ar.dword0x16 - (v40 * v40 + v6 * v6)) / str_F2C20ar.dword0x12 << 8;
					}
					else
					{
						str_F2C20ar.dword0x00 = 0;
					}
					v9x = &str_WORD_D951C[v41x->word_0x5A_90];
					v10 = v6 * str_F2C20ar.dword0x18 / v40;
					v11 = str_F2C20ar.dword0x18 * v42 / v40 + str_F2C20ar.dword0x22;
					str_F2C20ar.dword0x04_screenY = ((v10 * str_F2C20ar.dword0x11 - str_F2C20ar.dword0x0d * v11) >> 16) + str_F2C20ar.dword0x24;
					str_F2C20ar.dword0x03_screenX = str_F2C20ar.dword0x10 - ((str_F2C20ar.dword0x0d * v10 + v11 * str_F2C20ar.dword0x11) >> 16);
					v12 = v9x->byte_12;
					x_BYTE_F2CC6 = 0;
					switch (v12)
					{
					case 0:
						if (str_DWORD_F66F0x[v9x->word_0])
						{
							goto LABEL_16;
						}
						if (MainInitTmaps_71520(v9x->word_0))
						{
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
						str_F2C20ar.dword0x08_width = a1x->width;
						str_F2C20ar.dword0x06_height = a1x->height;
						v28 = (signed __int64)(str_F2C20ar.dword0x18 * v9x->rotSpeed_8) / v40;
						str_F2C20ar.dword0x0c_realHeight = v28;
						str_F2C20ar.dword0x09_realWidth = v28 * str_F2C20ar.dword0x08_width / str_F2C20ar.dword0x06_height;
						v23 = -str_F2C20ar.dword0x08_width;
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
						str_F2C20ar.dword0x08_width = a1x->width;
						str_F2C20ar.dword0x06_height = a1x->height;
						v31 = (signed __int64)(str_F2C20ar.dword0x18 * v9x->rotSpeed_8) / v40;
						str_F2C20ar.dword0x0c_realHeight = v31;
						str_F2C20ar.dword0x09_realWidth = v31 * str_F2C20ar.dword0x08_width / str_F2C20ar.dword0x06_height;
						v23 = str_F2C20ar.dword0x08_width;
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
							str_F2C20ar.dword0x08_width = a1x->width;
							str_F2C20ar.dword0x06_height = a1x->height;
							v25 = (signed __int64)(str_F2C20ar.dword0x18 * v9x->rotSpeed_8) / v40;
							str_F2C20ar.dword0x0c_realHeight = v25;
							str_F2C20ar.dword0x09_realWidth = v25 * str_F2C20ar.dword0x08_width / str_F2C20ar.dword0x06_height;
							v23 = -str_F2C20ar.dword0x08_width;
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
							str_F2C20ar.dword0x08_width = a1x->width;
							str_F2C20ar.dword0x06_height = a1x->height;
							v22 = (signed __int64)(str_F2C20ar.dword0x18 * v9x->rotSpeed_8) / v40;
							str_F2C20ar.dword0x0c_realHeight = v22;
							str_F2C20ar.dword0x09_realWidth = v22 * str_F2C20ar.dword0x08_width / str_F2C20ar.dword0x06_height;
							v23 = str_F2C20ar.dword0x08_width;
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
							str_F2C20ar.dword0x08_width = a1x->width;
							str_F2C20ar.dword0x06_height = a1x->height;
							v36 = (signed __int64)(str_F2C20ar.dword0x18 * v9x->rotSpeed_8) / v40;
							str_F2C20ar.dword0x0c_realHeight = v36;
							str_F2C20ar.dword0x09_realWidth = v36 * str_F2C20ar.dword0x08_width / str_F2C20ar.dword0x06_height;
							v23 = -str_F2C20ar.dword0x08_width;
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
							str_F2C20ar.dword0x08_width = a1x->width;
							str_F2C20ar.dword0x06_height = a1x->height;
							v34 = (signed __int64)(str_F2C20ar.dword0x18 * v9x->rotSpeed_8) / v40;
							str_F2C20ar.dword0x0c_realHeight = v34;
							str_F2C20ar.dword0x09_realWidth = v34 * str_F2C20ar.dword0x08_width / str_F2C20ar.dword0x06_height;
							v23 = str_F2C20ar.dword0x08_width;
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
						str_F2C20ar.dword0x08_width = a1x->width;
						str_F2C20ar.dword0x06_height = a1x->height;
						v27 = (signed __int64)(str_F2C20ar.dword0x18 * v9x->rotSpeed_8) / v40;
						str_F2C20ar.dword0x0c_realHeight = v27;
						str_F2C20ar.dword0x09_realWidth = v27 * str_F2C20ar.dword0x08_width / str_F2C20ar.dword0x06_height;
						v23 = str_F2C20ar.dword0x08_width;
					LABEL_69:
						str_F2C20ar.dword0x05 = v23;
					LABEL_70:
						str_F2C20ar.dword0x02_data = a1x->textureBuffer;
						v38 = str_F2C20ar.dword0x00;
						a1x->word_0 |= 8;
						if (v38 == 0x2000)
							v39 = x_BYTE_D4750[v9x->byte_10];
						else
							v39 = x_BYTE_D4750[6 + v9x->byte_10];
						str_F2C20ar.dword0x01_rotIdx = v39;
						str_F2C20ar.dword0x09_realWidth++;
						str_F2C20ar.dword0x0c_realHeight++;
						DrawSprite_41BD3(2u);
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

void GameRender_old::sub_88740(type_event_0x6E8E* a1x, int a2, int a3)
{
	int v3; // esi
	type_event_0x6E8E* v4x; // edx
	unsigned __int8 v5; // al
	unsigned __int8 v6; // al
	unsigned __int8 v7; // al
	unsigned __int8 v8; // al
	signed int v10; // eax
	unsigned __int8 v11; // al
	signed int v13; // eax
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
		if (x_WORD_180660_VGA_type_resolution & 1)
		{
			a2 *= 2;
			a3 *= 2;
		}
		if (str_E2A74[v3].axis_2[0] & 2)
		{
			if (a1x == str_E2A74[v3].dword_12)
			{
				if (!(str_unk_1804B0ar.byte_0x9f & 0x1))
				{
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
				str_E2A74[v3].dword_20 = v13;
				str_E2A74[v3].dword_12 = a1x;
				str_E2A74[v3].axis_2[0] |= 8;
			}
		}
	}
}

void GameRender_old::SetBillboards_3B560(int16_t roll)
{
	int v1; // edx
	type_unk_F0E20x* v2x; // edi
	signed int* v3; // esi
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
	str_F2C20ar.dword0x1e = v1 >> 8;
	switch (v1 >> 8)
	{
	case 0:
		str_F2C20ar.dword0x27 = Maths::x_DWORD_DB750[v1];
		str_F2C20ar.dword0x1b = Maths::x_DWORD_DB750[0x200 + v1];

		v88 = iScreenWidth_DE560;
		v92 = (unsigned __int16)viewPort.Width_DE564;
		str_F2C20ar.dword0x1f = (str_F2C20ar.dword0x27 << 8) / (str_F2C20ar.dword0x1b >> 8);
		v101 = (str_F2C20ar.dword0x27 << 8) / (str_F2C20ar.dword0x1b >> 8);
		v2x = m_str_F0E20x;
		v3 = (signed int*)&x_DWORD_E9C38_smalltit[59360];
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
			v2x++;
			++v4;
			++v6;
			--v92;
		} while (v92);
		str_F2C20ar.dword0x1d = v7;
		str_F2C20ar.dword0x21 = -v7;
		str_F2C20ar.width0x25 = (unsigned __int16)viewPort.Height_DE568;
		str_F2C20ar.height0x26 = (unsigned __int16)viewPort.Width_DE564;
		str_F2C20ar.dword0x1c = (unsigned __int16)viewPort.Height_DE568 + (unsigned __int16)viewPort.Width_DE564;
		str_F2C20ar.pbyte0x1a = (4 * (v7 - 1) + &x_DWORD_E9C38_smalltit[59360]);
		v9 = iScreenWidth_DE560;
		str_F2C20ar.Height_0x19 = viewPort.Height_DE568 - v7;
		goto LABEL_66;
	case 1:
		v10 = Maths::x_DWORD_DB750[v1];
		v11 = Maths::x_DWORD_DB750[0x200 + v1];

		str_F2C20ar.dword0x27 = v10;
		str_F2C20ar.dword0x1b = (int)v11;
		v84 = iScreenWidth_DE560;
		v93 = (unsigned __int16)viewPort.Height_DE568;
		if (v1 == 256)
		{
			str_F2C20ar.dword0x1f = 0x10000;
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
				v12x++;
				v14 += v84 + 1;
				++v15;
				--v93;
			} while (v93);
			str_F2C20ar.dword0x1d = (unsigned __int16)viewPort.Height_DE568;
			str_F2C20ar.dword0x21 = -(unsigned __int16)viewPort.Height_DE568;
		}
		else
		{
			str_F2C20ar.dword0x1f = (str_F2C20ar.dword0x1b << 8) / (v10 >> 8);
			v102 = (str_F2C20ar.dword0x1b << 8) / (v10 >> 8);
			v17x = m_str_F0E20x;
			v18 = (signed int*)&x_DWORD_E9C38_smalltit[59360];
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
				v17x++;
				v19 += v84;
				v21++;
				v93--;
			} while (v93);
			str_F2C20ar.dword0x1d = v22;
			str_F2C20ar.dword0x21 = -v22;
		}
		str_F2C20ar.width0x25 = (unsigned __int16)viewPort.Width_DE564;
		str_F2C20ar.height0x26 = (unsigned __int16)viewPort.Height_DE568;
		str_F2C20ar.dword0x1c = (unsigned __int16)viewPort.Height_DE568 + (unsigned __int16)viewPort.Width_DE564;
		str_F2C20ar.pbyte0x1a = (4 * (-1 - str_F2C20ar.dword0x21) + &x_DWORD_E9C38_smalltit[59360]);
		v9 = -1;
		str_F2C20ar.Height_0x19 = (unsigned __int16)viewPort.Width_DE564 + str_F2C20ar.dword0x21;
		goto LABEL_66;
	case 2:
		str_F2C20ar.dword0x27 = Maths::x_DWORD_DB750[v1 - 0x200];//copy to other
		str_F2C20ar.dword0x1b = Maths::x_DWORD_DB750[v1];

		v85 = iScreenWidth_DE560;
		v94 = (unsigned __int16)viewPort.Height_DE568;
		str_F2C20ar.dword0x1f = (str_F2C20ar.dword0x27 << 8) / (str_F2C20ar.dword0x1b >> 8);
		v103 = (str_F2C20ar.dword0x27 << 8) / (str_F2C20ar.dword0x1b >> 8);
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
			v23x++;
			v25 += v85;
			v27++;
			v94--;
		} while (v94);
		str_F2C20ar.dword0x1d = v28;
		str_F2C20ar.dword0x21 = -v28;
		str_F2C20ar.width0x25 = (unsigned __int16)viewPort.Width_DE564;
		str_F2C20ar.dword0x1c = (unsigned __int16)viewPort.Height_DE568 + (unsigned __int16)viewPort.Width_DE564;
		str_F2C20ar.height0x26 = (unsigned __int16)viewPort.Height_DE568;
		str_F2C20ar.dword0x23 = -1;
		str_F2C20ar.Height_0x19 = (unsigned __int16)viewPort.Width_DE564 - v28;
		str_F2C20ar.pbyte0x1a = (4 * (v28 - 1) + &x_DWORD_E9C38_smalltit[59360]);
		v95 = (unsigned __int16)viewPort.Height_DE568 - 1;
		resultx = m_str_F0E20x;
		if (v95 < 0)
			return;
		goto LABEL_68;
	case 3:
		str_F2C20ar.dword0x27 = Maths::x_DWORD_DB750[v1 - 0x200];//copy to other
		str_F2C20ar.dword0x1b = Maths::x_DWORD_DB750[v1];

		v89 = iScreenWidth_DE560;
		v96 = (unsigned __int16)viewPort.Width_DE564;
		if (v1 == 768)
		{
			str_F2C20ar.dword0x1f = 0x10000;
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
				v32x++;
				v33 = v89 + v33 - 1;
				v34++;
				v96--;
			} while (v96);
			str_F2C20ar.dword0x1d = (unsigned __int16)viewPort.Width_DE564;
			str_F2C20ar.dword0x21 = -(unsigned __int16)viewPort.Width_DE564;
		}
		else
		{
			str_F2C20ar.dword0x1f = (str_F2C20ar.dword0x1b << 8) / (str_F2C20ar.dword0x27 >> 8);
			v104 = (str_F2C20ar.dword0x1b << 8) / (str_F2C20ar.dword0x27 >> 8);
			v36x = m_str_F0E20x;
			v37 = (signed int*)&x_DWORD_E9C38_smalltit[59360];
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
				v36x++;
				v38--;
				v40++;
				v96--;
			} while (v96);
			str_F2C20ar.dword0x1d = v41;
			str_F2C20ar.dword0x21 = -v41;
		}
		str_F2C20ar.width0x25 = (unsigned __int16)viewPort.Height_DE568;
		str_F2C20ar.height0x26 = (unsigned __int16)viewPort.Width_DE564;
		str_F2C20ar.dword0x1c = (unsigned __int16)viewPort.Height_DE568 + (unsigned __int16)viewPort.Width_DE564;
		v42x = (4 * (-1 - str_F2C20ar.dword0x21) + &x_DWORD_E9C38_smalltit[59360]);
		str_F2C20ar.Height_0x19 = (unsigned __int16)viewPort.Height_DE568 + str_F2C20ar.dword0x21;
		v9 = -iScreenWidth_DE560;
		goto LABEL_65;
	case 4:
		str_F2C20ar.dword0x27 = Maths::x_DWORD_DB750[v1 - 0x400];//copy to other
		str_F2C20ar.dword0x1b = Maths::x_DWORD_DB750[v1 - 0x200];//copy to other

		v90 = -iScreenWidth_DE560;
		v97 = (unsigned __int16)viewPort.Width_DE564;
		str_F2C20ar.dword0x1f = (str_F2C20ar.dword0x27 << 8) / (str_F2C20ar.dword0x1b >> 8);
		v105 = (str_F2C20ar.dword0x27 << 8) / (str_F2C20ar.dword0x1b >> 8);
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
			v43x++;
			v45--;
			v47++;
			v97--;
		} while (v97);
		str_F2C20ar.dword0x1d = v48;
		v49 = -v48;
		str_F2C20ar.dword0x21 = v49;
		v50 = (unsigned __int16)viewPort.Height_DE568 + v49;
		str_F2C20ar.width0x25 = (unsigned __int16)viewPort.Height_DE568;
		str_F2C20ar.height0x26 = (unsigned __int16)viewPort.Width_DE564;
		str_F2C20ar.dword0x1c = (unsigned __int16)viewPort.Height_DE568 + (unsigned __int16)viewPort.Width_DE564;
		v42x = (4 * (-1 - v49) + &x_DWORD_E9C38_smalltit[59360]);
		str_F2C20ar.Height_0x19 = v50;
		v9 = -iScreenWidth_DE560;
		goto LABEL_65;
	case 5:
		str_F2C20ar.dword0x1b = Maths::x_DWORD_DB750[v1 - 0x200];//copy to other
		str_F2C20ar.dword0x27 = Maths::x_DWORD_DB750[v1 - 0x400];//copy to other

		v86 = -iScreenWidth_DE560;
		v98 = (unsigned __int16)viewPort.Height_DE568;
		if (v1 == 1280)
		{
			str_F2C20ar.dword0x1f = 0x10000;
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
				v53x++;
				v54 = v86 + v54 - 1;
				++v55;
				--v98;
			} while (v98);
			str_F2C20ar.dword0x1d = (unsigned __int16)viewPort.Height_DE568;
			str_F2C20ar.dword0x21 = -(unsigned __int16)viewPort.Height_DE568;
		}
		else
		{
			str_F2C20ar.dword0x1f = (str_F2C20ar.dword0x1b << 8) / (str_F2C20ar.dword0x27 >> 8);
			v106 = (str_F2C20ar.dword0x1b << 8) / (str_F2C20ar.dword0x27 >> 8);
			v57x = m_str_F0E20x;
			v58 = (signed int*)&x_DWORD_E9C38_smalltit[59360];
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
				v57x++;
				v59 += v86;
				v61++;
				v98--;
			} while (v98);
			str_F2C20ar.dword0x1d = v62;
			str_F2C20ar.dword0x21 = -v62;
		}
		str_F2C20ar.width0x25 = (unsigned __int16)viewPort.Width_DE564;
		str_F2C20ar.height0x26 = (unsigned __int16)viewPort.Height_DE568;
		str_F2C20ar.dword0x1c = (unsigned __int16)viewPort.Height_DE568 + (unsigned __int16)viewPort.Width_DE564;
		str_F2C20ar.dword0x23 = 1;
		str_F2C20ar.Height_0x19 = (unsigned __int16)viewPort.Width_DE564 + str_F2C20ar.dword0x21;
		str_F2C20ar.pbyte0x1a = (4 * (-1 - str_F2C20ar.dword0x21) + &x_DWORD_E9C38_smalltit[59360]);
		v95 = (unsigned __int16)viewPort.Height_DE568 - 1;
		resultx = m_str_F0E20x;
		if (v95 < 0)
			return;
		goto LABEL_68;
	case 6:
		str_F2C20ar.dword0x27 = Maths::x_DWORD_DB750[v1 - 0x600];//copy to other
		str_F2C20ar.dword0x1b = Maths::x_DWORD_DB750[v1 - 0x400];//copy to other

		v87 = -iScreenWidth_DE560;
		v99 = (unsigned __int16)viewPort.Height_DE568;
		str_F2C20ar.dword0x1f = (str_F2C20ar.dword0x27 << 8) / (str_F2C20ar.dword0x1b >> 8);
		v107 = (str_F2C20ar.dword0x27 << 8) / (str_F2C20ar.dword0x1b >> 8);
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
			v63x++;
			v65 += v87;
			v67++;
			v99--;
		} while (v99);
		str_F2C20ar.dword0x1d = v68;
		str_F2C20ar.dword0x21 = -v68;
		str_F2C20ar.Height_0x19 = (unsigned __int16)viewPort.Width_DE564 - v68;
		str_F2C20ar.width0x25 = (unsigned __int16)viewPort.Width_DE564;
		str_F2C20ar.dword0x1c = (unsigned __int16)viewPort.Height_DE568 + (unsigned __int16)viewPort.Width_DE564;
		str_F2C20ar.height0x26 = (unsigned __int16)viewPort.Height_DE568;
		str_F2C20ar.dword0x23 = 1;
		str_F2C20ar.pbyte0x1a = (4 * (v68 - 1) + &x_DWORD_E9C38_smalltit[59360]);
		v95 = (unsigned __int16)viewPort.Height_DE568 - 1;
		resultx = m_str_F0E20x;
		if (v95 < 0)
			return;
		goto LABEL_68;
	case 7:
		v69 = Maths::x_DWORD_DB750[v1 - 0x600];//copy to other
		v70 = Maths::x_DWORD_DB750[v1 - 0x400];//copy to other

		str_F2C20ar.dword0x27 = v69;
		str_F2C20ar.dword0x1b = v70;
		v91 = -iScreenWidth_DE560;
		v100 = (unsigned __int16)viewPort.Width_DE564;
		if (v1 == 1792)
		{
			v71x = m_str_F0E20x;
			v72 = (signed int*)&x_DWORD_E9C38_smalltit[59360];
			str_F2C20ar.dword0x1f = 0x10000;
			v73 = 0;
			v74 = 1;
			v75 = 0;
			do
			{
				v71x->dword_1 = v73;
				v71x->dword_2 = v75++;
				v72[0] = v74;
				v72++;
				v71x++;
				v73 += v91 + 1;
				v74++;
				v100--;
			} while (v100);
			str_F2C20ar.dword0x1d = (unsigned __int16)viewPort.Width_DE564;
			str_F2C20ar.dword0x21 = -(unsigned __int16)viewPort.Width_DE564;
		}
		else
		{
			str_F2C20ar.dword0x1f = (str_F2C20ar.dword0x1b << 8) / (v69 >> 8);
			v108 = (str_F2C20ar.dword0x1b << 8) / (v69 >> 8);
			v76x = m_str_F0E20x;
			v77 = (signed int*)&x_DWORD_E9C38_smalltit[59360];
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
				v76x++;
				v78++;
				v80++;
				v100--;
			} while (v100);
			str_F2C20ar.dword0x1d = v81;
			str_F2C20ar.dword0x21 = -v81;
		}
		str_F2C20ar.width0x25 = (unsigned __int16)viewPort.Height_DE568;
		str_F2C20ar.height0x26 = (unsigned __int16)viewPort.Width_DE564;
		str_F2C20ar.dword0x1c = (unsigned __int16)viewPort.Height_DE568 + (unsigned __int16)viewPort.Width_DE564;
		str_F2C20ar.Height_0x19 = (unsigned __int16)viewPort.Height_DE568 + str_F2C20ar.dword0x21;
		v42x = (4 * (-1 - str_F2C20ar.dword0x21) + &x_DWORD_E9C38_smalltit[59360]);
		v9 = iScreenWidth_DE560;
	LABEL_65:
		str_F2C20ar.pbyte0x1a = v42x;
	LABEL_66:
		str_F2C20ar.dword0x23 = v9;
		break;
	default:
		break;
	}
	v95 = str_F2C20ar.height0x26 - 1;
	for (resultx = m_str_F0E20x; v95 >= 0; --v95)
	{
	LABEL_68:
		v82 = resultx->dword_1;
		// FIXME: This is what would have happend in the original memory layout in which 
		//        the array unk_F0A20x is directly located before unk_F0E20x.
		//        But not sure if this is intended. Maybe it becomes clearer when the logic get refactored.
		if (resultx == m_str_F0E20x) {
			v83 = *(x_DWORD*)&unk_F0A20x[0x3f8];
		}
		else {
			v83 = resultx[-1].dword_1;
		}
		resultx->dword_0 = v82 - v83;
		resultx++;
	}
}

void GameRender_old::DrawSorcererNameAndHealthBar_2CB30(type_event_0x6E8E* a1x, __int16 a2, int a3, __int16 a4)//20db30 //maybe draw sorcerer name
{
	char* v5; // esi
	int v9x; // eax
	int v9y; // eax
	__int16 v10; // bx
	__int16 v11; // bx
	int v12; // edi
	int v13; // esi
	unsigned __int8 v15; // ST10_1
	unsigned __int8 v16; // ST10_1
	unsigned __int8 v17; // ST10_1
	unsigned __int8 v18; // ST10_1
	unsigned __int8 v19; // ST10_1*/
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

	v31 = viewPort.PreWidth_EA3C4 + viewPort.PosX_EA3D0 - 4;
	v29 = viewPort.PreHeight_EA3C0 + viewPort.PosY_EA3CC - 22;
	v25 = a1x->dword_0xA4_164x->word_0x38_56;
	v5 = D41A0_0.array_0x2BDE[v25].array_0x39f_2BFA_12157;//wizard name
	strcpy(v24, v5);
	v36 = x_BYTE_E88E0x[3 * sub_61790(v25)];//c
	v35 = (*xadataclrd0dat.colorPallette_var28)[0];//10 //v19
	v34 = x_BYTE_E88E0x[3 * sub_61790(v25)];	//14 //v18
	v33 = str_D94F0_bldgprmbuffer[static_cast<std::underlying_type<MapType_t>::type>(D41A0_0.terrain_2FECE.MapType)][2];//18 v14
	v38 = str_D94F0_bldgprmbuffer[static_cast<std::underlying_type<MapType_t>::type>(D41A0_0.terrain_2FECE.MapType)][3];//4 v15
	v37 = str_D94F0_bldgprmbuffer[static_cast<std::underlying_type<MapType_t>::type>(D41A0_0.terrain_2FECE.MapType)][0];//?v22
	v10 = (a4 >> 1) + a2;
	if (x_WORD_180660_VGA_type_resolution & 1)
	{
		v10 *= 2;
		a3 *= 2;
	}
	v11 = viewPort.PosX_EA3D0 + v10;
	v12 = viewPort.PosY_EA3CC + a3 - 20;
	v39 = viewPort.PosY_EA3CC + a3 - 20;
	if (v11 >= viewPort.PosX_EA3D0)
	{
		if ((signed __int16)v12 >= viewPort.PosY_EA3CC && v11 < v31 && (signed __int16)v12 < v29)
		{
			v9x = strlen(v24);
			v13 = 8 * v9x + 4;
			if (v11 + v13 > v31)
			{
				v13 = v31 - v11;
				v9x = ((v31 - v11 - 4) - (my_sign32(v31 - v11 - 4) << 3) + my_sign32(v31 - v11 - 4)) >> 3;
			}
			if (v9x > 0)
			{
				v24[v9x] = 0;
				v32 = v13 + 2;
				v26 = (signed __int16)(v13 + 2);
				v30 = v11;
				DrawLine_2BC80(v11, v39, v13 + 2, 18, v37);//8
				v27 = v33;//30// v16
				DrawLine_2BC80(v30, v39, v26, 2, v33);//18
				v28 = v38;//2c//v17
				DrawLine_2BC80(v30, v39 + 16, v26, 2, v38);//4
				DrawLine_2BC80(v30, v39, 2, 16, v33);//30,tj.18
				DrawLine_2BC80(v11 + v32 - 2, v39, 2, 18, v38);//2c tj. 4
				DrawText_2BC10(v24, v11 + 4, v39, v34);//14
				DrawLine_2BC80(v11 + 2, v39 + 14, v13 - 2, 2, v35);//10
				if (a1x->dword_0x4)
				{
					v9y = a1x->dword_0x8 * (v13 - 2) / a1x->dword_0x4;
					if (v30 + 2 + v9y > v31 - 2)
						v9y = v31 - 2 - (v30 + 2);
					if (v9y > 0)
						DrawLine_2BC80(v11 + 2, v39 + 14, v9y, 2, v36);
				}
			}
		}
	}
}

void GameRender_old::DrawParticles_3E360(int a2x)//21f360
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
	int v17; // ebx
	int v18; // ebx
	int v19; // eax
	int v20; // eax
	int v21; // eax
	int v22; // edx
	int v23; // eax
	int v24; // eax
	int v25; // ebx
	unsigned __int16 v27; // ax
	int v28; // eax
	unsigned __int16 v30; // ax
	int v31; // eax
	int v32; // ebx
	int v33; // eax
	int v35; // eax
	int v36; // eax
	int v37; // ebx
	int v38; // edx
	int v39; // eax
	int v40; // eax
	int v41; // eax
	int v42; // eax
	int v43; // ebx
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
	int v59; // ebx
	int v61; // ebx
	int v62; // ebx
	int v63; // eax
	unsigned __int16 v65; // ax
	int v66; // eax
	int v67; // eax
	int v68; // eax
	int v70; // eax
	int v71; // eax
	int v72; // ebx
	int v73; // eax
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
	/*if(debugafterload)
		VGA_Debug_Blit(640, 480, pdwScreenBuffer);*/

#ifdef DEBUG_SEQUENCES
		/*uint8_t origbyte20 = 0;
		uint8_t remakebyte20 = 0;
		int comp20;
		if (debugafterload)
		{
			if(debugcountersub_3E360>0x470)
				comp20 = compare_with_sequence_D41A0((char*)"0021f360-00356038", (uint8_t*)&D41A0_BYTESTR_0, 0x356038, debugcountersub_3E360, 224790, &origbyte20, &remakebyte20);
			debugcountersub_3E360++;
		}*/
#endif //DEBUG_SEQUENCES

	result = Str_E9C38_smalltit[a2x].word36;
	do
	{
		//adress 21f370

		v3x = x_DWORD_EA3E4[result];
		str_F2C20ar.dword0x14x = v3x;
		if (!(v3x->struct_byte_0xc_12_15.byte[0] & 0x21))
		{
			v4 = v3x->axis_0x4C_76.y;
			v96 = (signed __int16)(v3x->axis_0x4C_76.x - x_WORD_F2CC4);
			v97 = (signed __int16)(x_WORD_F2CC2 - v4);
			if (shadows_F2CC7)
			{
				if (!Str_E9C38_smalltit[a2x].byte43 && !(v3x->struct_byte_0xc_12_15.word[1] & 0x808))
				{
					//adress 21f40c
					v98 = sub_B5C60_getTerrainAlt2(v3x->axis_0x4C_76.x, v4) - str_F2C20ar.dword0x20;
					v5 = (str_F2C20ar.dword0x0f * v96 - str_F2C20ar.dword0x17 * v97) >> 16;
					v99 = (str_F2C20ar.dword0x17 * v96 + str_F2C20ar.dword0x0f * v97) >> 16;
					v6 = v99 * v99 + v5 * v5;
					if (v99 > 64 && v6 < str_F2C20ar.dword0x15)
					{
						if (v6 <= str_F2C20ar.dword0x13)
							str_F2C20ar.dword0x00 = 0x2000;
						else
							str_F2C20ar.dword0x00 = v6 < str_F2C20ar.dword0x16 ? 32 * (str_F2C20ar.dword0x16 - (v99 * v99 + v5 * v5)) / str_F2C20ar.dword0x12 << 8 : 0;
						v7x = &str_WORD_D951C[str_F2C20ar.dword0x14x->word_0x5A_90];
						if (!v7x->byte_10)
						{
							v8 = v5 * str_F2C20ar.dword0x18 / v99;
							v9 = str_F2C20ar.dword0x18 * v98 / v99 + str_F2C20ar.dword0x22;
							str_F2C20ar.dword0x04_screenY = ((v8 * str_F2C20ar.dword0x11 - str_F2C20ar.dword0x0d * v9) >> 16) + str_F2C20ar.dword0x24;
							str_F2C20ar.dword0x03_screenX = str_F2C20ar.dword0x10 - ((str_F2C20ar.dword0x0d * v8 + v9 * str_F2C20ar.dword0x11) >> 16);
							v10 = v7x->byte_12;
							x_BYTE_F2CC6 = 0;
							switch (v10)
							{
							case 0:
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
								v25 = (((str_F2C20ar.dword0x14x->word_0x1C_28 - (unsigned __int16)x_WORD_F2CC0) >> 3) & 0xF0) >> 4;
								if (v25 < 8)
								{
									if (str_DWORD_F66F0x[v25 + v7x->word_0])
									{
										v27 = str_TMAPS00TAB_BEGIN_BUFFER[v25 + v7x->word_0].word_8;
									}
									else
									{
										if (!MainInitTmaps_71520(v25 + v7x->word_0))
											goto LABEL_178;
										v27 = str_TMAPS00TAB_BEGIN_BUFFER[v25 + v7x->word_0].word_8;
									}
									x_DWORD_F5730[v27] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
									a1y = *str_DWORD_F66F0x[v25 + v7x->word_0];
									goto LABEL_51;
								}
								if (str_DWORD_F66F0x[v7x->word_0 + 15 - v25])
								{
									v30 = str_TMAPS00TAB_BEGIN_BUFFER[v7x->word_0 + 15 - v25].word_8;
								}
								else
								{
									if (!MainInitTmaps_71520(v7x->word_0 + 15 - v25))
										goto LABEL_178;
									v30 = str_TMAPS00TAB_BEGIN_BUFFER[v7x->word_0 + 15 - v25].word_8;
								}
								x_DWORD_F5730[v30] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
								a1y = *str_DWORD_F66F0x[v7x->word_0 + 15 - v25];
								str_F2C20ar.dword0x08_width = a1y->width;
								str_F2C20ar.dword0x06_height = a1y->height;
								v31 = (signed __int64)(str_F2C20ar.dword0x18 * v7x->rotSpeed_8) / v99;
								str_F2C20ar.dword0x0c_realHeight = v31;
								str_F2C20ar.dword0x09_realWidth = v31 * str_F2C20ar.dword0x08_width / str_F2C20ar.dword0x06_height;
								v21 = -str_F2C20ar.dword0x08_width;
								goto LABEL_72;
							case 18:
								v32 = (((str_F2C20ar.dword0x14x->word_0x1C_28 - (unsigned __int16)x_WORD_F2CC0) >> 3) & 0xF0) >> 4;
								v33 = v32 + v7x->word_0;
								if (str_DWORD_F66F0x[v33])
								{
									v35 = str_TMAPS00TAB_BEGIN_BUFFER[v33].word_8;
								}
								else
								{
									if (!MainInitTmaps_71520(v32 + v7x->word_0))
										goto LABEL_178;
									v35 = str_TMAPS00TAB_BEGIN_BUFFER[v32 + v7x->word_0].word_8;
								}
								x_DWORD_F5730[v35] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
								a1y = *str_DWORD_F66F0x[v32 + v7x->word_0];
								str_F2C20ar.dword0x08_width = a1y->width;
								str_F2C20ar.dword0x06_height = a1y->height;
								v36 = (signed __int64)(str_F2C20ar.dword0x18 * v7x->rotSpeed_8) / v99;
								str_F2C20ar.dword0x0c_realHeight = v36;
								str_F2C20ar.dword0x09_realWidth = v36 * str_F2C20ar.dword0x08_width / str_F2C20ar.dword0x06_height;
								v21 = str_F2C20ar.dword0x08_width;
								goto LABEL_72;
							case 19:
								v18 = (((str_F2C20ar.dword0x14x->word_0x1C_28 - (unsigned __int16)x_WORD_F2CC0) >> 3) & 0xF0) >> 4;
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
									str_F2C20ar.dword0x08_width = a1y->width;
									str_F2C20ar.dword0x06_height = a1y->height;
									v24 = (signed __int64)(str_F2C20ar.dword0x18 * v7x->rotSpeed_8) / v99;
									str_F2C20ar.dword0x0c_realHeight = v24;
									str_F2C20ar.dword0x09_realWidth = v24 * str_F2C20ar.dword0x08_width / str_F2C20ar.dword0x06_height;
									v21 = -str_F2C20ar.dword0x08_width;
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
									str_F2C20ar.dword0x08_width = a1y->width;
									str_F2C20ar.dword0x06_height = a1y->height;
									v20 = (signed __int64)(str_F2C20ar.dword0x18 * v7x->rotSpeed_8) / v99;
									str_F2C20ar.dword0x0c_realHeight = v20;
									str_F2C20ar.dword0x09_realWidth = v20 * str_F2C20ar.dword0x08_width / str_F2C20ar.dword0x06_height;
									v21 = str_F2C20ar.dword0x08_width;
								}
								goto LABEL_72;
							case 20:
								v37 = (((str_F2C20ar.dword0x14x->word_0x1C_28 - (unsigned __int16)x_WORD_F2CC0) >> 3) & 0xF0) >> 4;
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
									str_F2C20ar.dword0x08_width = a1y->width;
									str_F2C20ar.dword0x06_height = a1y->height;
									v42 = (signed __int64)(str_F2C20ar.dword0x18 * v7x->rotSpeed_8) / v99;
									str_F2C20ar.dword0x0c_realHeight = v42;
									str_F2C20ar.dword0x09_realWidth = v42 * str_F2C20ar.dword0x08_width / str_F2C20ar.dword0x06_height;
									v21 = -str_F2C20ar.dword0x08_width;
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
									str_F2C20ar.dword0x08_width = a1y->width;
									str_F2C20ar.dword0x06_height = a1y->height;
									v40 = (signed __int64)(str_F2C20ar.dword0x18 * v7x->rotSpeed_8) / v99;
									str_F2C20ar.dword0x0c_realHeight = v40;
									str_F2C20ar.dword0x09_realWidth = v40 * str_F2C20ar.dword0x08_width / str_F2C20ar.dword0x06_height;
									v21 = str_F2C20ar.dword0x08_width;
								}
								goto LABEL_72;
							case 21:
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
								v17 = v7x->word_0 + str_F2C20ar.dword0x14x->byte_0x5C_92;
								if (str_DWORD_F66F0x[v17])
								{
									x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v17].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
								}
								else
								{
									if (!MainInitTmaps_71520(v7x->word_0 + str_F2C20ar.dword0x14x->byte_0x5C_92))
										goto LABEL_178;
									x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v7x->word_0 + str_F2C20ar.dword0x14x->byte_0x5C_92].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
								}
								a1y = *str_DWORD_F66F0x[v7x->word_0 + str_F2C20ar.dword0x14x->byte_0x5C_92];
							LABEL_51:
								str_F2C20ar.dword0x08_width = a1y->width;
								str_F2C20ar.dword0x06_height = a1y->height;
								v28 = (signed __int64)(str_F2C20ar.dword0x18 * v7x->rotSpeed_8) / v99;
								str_F2C20ar.dword0x0c_realHeight = v28;
								str_F2C20ar.dword0x09_realWidth = v28 * str_F2C20ar.dword0x08_width / str_F2C20ar.dword0x06_height;
								v21 = str_F2C20ar.dword0x08_width;
							LABEL_72:
								str_F2C20ar.dword0x05 = v21;
							LABEL_73:
								v43 = str_F2C20ar.dword0x00;
								str_F2C20ar.dword0x02_data = a1y->textureBuffer;
								a1y->word_0 |= 8;

								if (v43 == 0x2000)
									v45 = x_BYTE_D4750[v7x->byte_10];
								else
									v45 = x_BYTE_D4750[6 + v7x->byte_10];
								str_F2C20ar.dword0x01_rotIdx = v45;
								v46 = str_F2C20ar.dword0x0c_realHeight >> 2;
								str_F2C20ar.dword0x0c_realHeight >>= 2;
								if (str_F2C20ar.dword0x09_realWidth > 0 && v46 > 0)
								{
									v47 = str_F2C20ar.dword0x00 >> 2;
									if (notDay_D4320)
										str_F2C20ar.dword0x00 = 0x2000 - v47;
									else
										str_F2C20ar.dword0x00 = v47 + 0x2000;
									str_F2C20ar.dword0x01_rotIdx = 8;
									DrawSprite_41BD3(0);
								}
								break;
							default:
								goto LABEL_73;
							}
						}
					}
				}
			}
			if (str_F2C20ar.dword0x14x->struct_byte_0xc_12_15.byte[3] >= 0)
				v48 = str_F2C20ar.dword0x14x->axis_0x4C_76.z;
			else
				v48 = str_F2C20ar.dword0x14x->axis_0x4C_76.z - 160;
			v100 = (str_F2C20ar.dword0x17 * v96 + str_F2C20ar.dword0x0f * v97) >> 16;
			v49 = (str_F2C20ar.dword0x0f * v96 - str_F2C20ar.dword0x17 * v97) >> 16;
			if (str_F2C20ar.dword0x14x->struct_byte_0xc_12_15.byte[3] & 0x20)
			{
				v50x = &str_D404C[str_F2C20ar.dword0x14x->byte_0x3B_59];
				switch ((((x_DWORD_EA3E4[str_F2C20ar.dword0x14x->word_0x32_50]->word_0x1C_28
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
			if (v100 > 64 && v51 < str_F2C20ar.dword0x15)
			{
				if (v51 <= str_F2C20ar.dword0x13)
				{
					str_F2C20ar.dword0x00 = 0x2000;
				}
				else if (v51 < str_F2C20ar.dword0x16)
				{
					str_F2C20ar.dword0x00 = 32 * (str_F2C20ar.dword0x16 - (v100 * v100 + v49 * v49)) / str_F2C20ar.dword0x12 << 8;
				}
				else
				{
					str_F2C20ar.dword0x00 = 0;
				}
				v52x = &str_WORD_D951C[str_F2C20ar.dword0x14x->word_0x5A_90];
				v53 = v49 * str_F2C20ar.dword0x18 / v100;
				v54 = str_F2C20ar.dword0x18 * (v48 - str_F2C20ar.dword0x20) / v100 + str_F2C20ar.dword0x22;
				str_F2C20ar.dword0x04_screenY = ((v53 * str_F2C20ar.dword0x11 - str_F2C20ar.dword0x0d * v54) >> 16) + str_F2C20ar.dword0x24;
				str_F2C20ar.dword0x03_screenX = str_F2C20ar.dword0x10 - ((str_F2C20ar.dword0x0d * v53 + v54 * str_F2C20ar.dword0x11) >> 16);
				v55 = v52x->byte_12;
				x_BYTE_F2CC6 = 0;
				switch (v55)
				{
				case 0:
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
					v72 = (((str_F2C20ar.dword0x14x->word_0x1C_28 - (unsigned __int16)x_WORD_F2CC0) >> 3) & 0xF0) >> 4;
					if (str_F2C20ar.dword0x14x->struct_byte_0xc_12_15.byte[3] & 0x40)
						v72 = (unsigned __int8)x_BYTE_D4750[0x2c + v72];
					if (v72 < 8)
					{
						v73 = v72 + v52x->word_0;
						if (str_DWORD_F66F0x[v73])
						{
							v75 = str_TMAPS00TAB_BEGIN_BUFFER[v73].word_8;
						}
						else
						{
							if (!MainInitTmaps_71520(v72 + v52x->word_0))
								break;
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
					str_F2C20ar.dword0x08_width = a1y->width;
					str_F2C20ar.dword0x06_height = a1y->height;
					v78 = (signed __int64)(str_F2C20ar.dword0x18 * v52x->rotSpeed_8) / v100;
					str_F2C20ar.dword0x0c_realHeight = v78;
					str_F2C20ar.dword0x09_realWidth = v78 * str_F2C20ar.dword0x08_width / str_F2C20ar.dword0x06_height;
					v67 = -str_F2C20ar.dword0x08_width;
					goto LABEL_163;
				case 18:
					v79 = (((str_F2C20ar.dword0x14x->word_0x1C_28 - (unsigned __int16)x_WORD_F2CC0) >> 3) & 0xF0) >> 4;
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
					str_F2C20ar.dword0x08_width = a1y->width;
					str_F2C20ar.dword0x06_height = a1y->height;
					v81 = (signed __int64)(str_F2C20ar.dword0x18 * v52x->rotSpeed_8) / v100;
					str_F2C20ar.dword0x0c_realHeight = v81;
					str_F2C20ar.dword0x09_realWidth = v81 * str_F2C20ar.dword0x08_width / str_F2C20ar.dword0x06_height;
					v67 = str_F2C20ar.dword0x08_width;
					goto LABEL_163;
				case 19:
					v62 = (((str_F2C20ar.dword0x14x->word_0x1C_28 - (unsigned __int16)x_WORD_F2CC0) >> 3) & 0xF0) >> 4;
					if (v62 >= 8)
					{
						v68 = v52x->word_0 + (unsigned __int8)x_BYTE_D4750[0xc + v62];
						if (str_DWORD_F66F0x[v68])
						{
							v70 = str_TMAPS00TAB_BEGIN_BUFFER[v68].word_8;
						}
						else
						{
							if (!MainInitTmaps_71520(v52x->word_0 + (unsigned __int8)x_BYTE_D4750[0xc + v62]))
								break;
							v70 = str_TMAPS00TAB_BEGIN_BUFFER[v52x->word_0 + (unsigned __int8)x_BYTE_D4750[0xc + v62]].word_8;
						}
						x_DWORD_F5730[v70] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
						a1y = *str_DWORD_F66F0x[v52x->word_0 + (unsigned __int8)x_BYTE_D4750[0xc + v62]];
						str_F2C20ar.dword0x08_width = a1y->width;
						str_F2C20ar.dword0x06_height = a1y->height;
						v71 = (signed __int64)(str_F2C20ar.dword0x18 * v52x->rotSpeed_8) / v100;
						str_F2C20ar.dword0x0c_realHeight = v71;
						str_F2C20ar.dword0x09_realWidth = v71 * str_F2C20ar.dword0x08_width / str_F2C20ar.dword0x06_height;
						v67 = -str_F2C20ar.dword0x08_width;
					}
					else
					{
						v63 = v52x->word_0 + (unsigned __int8)x_BYTE_D4750[0xc + v62];
						if (str_DWORD_F66F0x[v63])
						{
							v65 = str_TMAPS00TAB_BEGIN_BUFFER[v63].word_8;
						}
						else
						{
							if (!MainInitTmaps_71520(v52x->word_0 + (unsigned __int8)x_BYTE_D4750[0xc + v62]))
								break;
							v65 = str_TMAPS00TAB_BEGIN_BUFFER[(unsigned __int8)x_BYTE_D4750[0xc + v62] + v52x->word_0].word_8;
						}
						x_DWORD_F5730[v65] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
						a1y = *str_DWORD_F66F0x[v52x->word_0 + (unsigned __int8)x_BYTE_D4750[0xc + v62]];
						str_F2C20ar.dword0x08_width = a1y->width;
						str_F2C20ar.dword0x06_height = a1y->height;
						v66 = (signed __int64)(str_F2C20ar.dword0x18 * v52x->rotSpeed_8) / v100;
						str_F2C20ar.dword0x0c_realHeight = v66;
						str_F2C20ar.dword0x09_realWidth = v66 * str_F2C20ar.dword0x08_width / str_F2C20ar.dword0x06_height;
						v67 = str_F2C20ar.dword0x08_width;
					}
					goto LABEL_163;
				case 20:
					v82 = (((str_F2C20ar.dword0x14x->word_0x1C_28 - (unsigned __int16)x_WORD_F2CC0) >> 3) & 0xF0) >> 4;
					if (v82 >= 8)
					{
						v86 = (unsigned __int8)x_BYTE_D4750[0x1c + v82] + v52x->word_0;
						if (str_DWORD_F66F0x[v86])
						{
							v88 = str_TMAPS00TAB_BEGIN_BUFFER[v86].word_8;
						}
						else
						{
							if (!MainInitTmaps_71520(v52x->word_0 + (unsigned __int8)x_BYTE_D4750[0x1c + v82]))
								break;
							v88 = str_TMAPS00TAB_BEGIN_BUFFER[v52x->word_0 + (unsigned __int8)x_BYTE_D4750[0x1c + v82]].word_8;
						}
						x_DWORD_F5730[v88] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
						a1y = *str_DWORD_F66F0x[v52x->word_0 + (unsigned __int8)x_BYTE_D4750[0x1c + v82]];
						str_F2C20ar.dword0x08_width = a1y->width;
						str_F2C20ar.dword0x06_height = a1y->height;
						v89 = (signed __int64)(str_F2C20ar.dword0x18 * v52x->rotSpeed_8) / v100;
						str_F2C20ar.dword0x0c_realHeight = v89;
						str_F2C20ar.dword0x09_realWidth = v89 * str_F2C20ar.dword0x08_width / str_F2C20ar.dword0x06_height;
						v67 = -str_F2C20ar.dword0x08_width;
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
						str_F2C20ar.dword0x08_width = a1y->width;
						str_F2C20ar.dword0x06_height = a1y->height;
						v85 = (signed __int64)(str_F2C20ar.dword0x18 * v52x->rotSpeed_8) / v100;
						str_F2C20ar.dword0x0c_realHeight = v85;
						str_F2C20ar.dword0x09_realWidth = v85 * str_F2C20ar.dword0x08_width / str_F2C20ar.dword0x06_height;
						v67 = str_F2C20ar.dword0x08_width;
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
					v61 = v52x->word_0 + str_F2C20ar.dword0x14x->byte_0x5C_92;
					if (str_DWORD_F66F0x[v61])
					{
						x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v61].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
					}
					else
					{
						if (!MainInitTmaps_71520(v52x->word_0 + str_F2C20ar.dword0x14x->byte_0x5C_92))
							break;
						x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[v52x->word_0 + str_F2C20ar.dword0x14x->byte_0x5C_92].word_8] = x_D41A0_BYTEARRAY_4_struct.byteindex_26;
					}
					a1y = *str_DWORD_F66F0x[v52x->word_0 + str_F2C20ar.dword0x14x->byte_0x5C_92];
				LABEL_141:
					str_F2C20ar.dword0x08_width = a1y->width;
					str_F2C20ar.dword0x06_height = a1y->height;
					v76 = (signed __int64)(str_F2C20ar.dword0x18 * v52x->rotSpeed_8) / v100;
					str_F2C20ar.dword0x0c_realHeight = v76;
					str_F2C20ar.dword0x09_realWidth = v76 * str_F2C20ar.dword0x08_width / str_F2C20ar.dword0x06_height;
					v67 = str_F2C20ar.dword0x08_width;
				LABEL_163:
					str_F2C20ar.dword0x05 = v67;
				LABEL_164:
					v90x = str_F2C20ar.dword0x14x;
					a1y->word_0 |= 8u;
					v91 = v90x->struct_byte_0xc_12_15.byte[2];
					str_F2C20ar.dword0x02_data = a1y->textureBuffer;
					if (v91 & 0x380)
					{
						v93 = v90x->struct_byte_0xc_12_15.byte[3];
						if (v93 & 2)
						{
							v94 = (unsigned __int8)x_BYTE_E88E0x[0x2 + 3
								* x_DWORD_EA3E4[v90x->word_0x28_40]->dword_0xA4_164x->word_0x38_56];
							str_F2C20ar.dword0x01_rotIdx = 4;
							str_F2C20ar.dword0x07 = v94;
						}
						else if (v93 & 4)
						{
							v95 = (unsigned __int8)x_BYTE_E88E0x[0x2 + 3
								* x_DWORD_EA3E4[v90x->word_0x28_40]->dword_0xA4_164x->word_0x38_56];
							str_F2C20ar.dword0x01_rotIdx = 5;
							str_F2C20ar.dword0x07 = v95;
						}
						else if (v90x->struct_byte_0xc_12_15.byte[2] >= 0)
						{
							if (v93 & 1)
								str_F2C20ar.dword0x01_rotIdx = 3;
						}
						else
						{
							str_F2C20ar.dword0x01_rotIdx = 2;
						}
					}
					else
					{
						if (str_F2C20ar.dword0x00 == 0x2000)
							v92 = x_BYTE_D4750[v52x->byte_10];
						else
							v92 = x_BYTE_D4750[6 + v52x->byte_10];
						str_F2C20ar.dword0x01_rotIdx = v92;
					}
					str_F2C20ar.dword0x09_realWidth++;
					str_F2C20ar.dword0x0c_realHeight++;
					DrawSprite_41BD3(1u);
					break;
				default:
					goto LABEL_164;
				}
			}
		}
	LABEL_178:
		result = str_F2C20ar.dword0x14x->oldMapEntity_0x16_22;
	} while (result);
	/*VGA_Debug_Blit(640, 480, pdwScreenBuffer);
	pdwScreenBuffer = pdwScreenBuffer;//debug*/
}

void GameRender_old::DrawSprite_41BD3(uint32 a1)
{
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
	int comp22a = compare_with_sequence_array_222BD3((char*)"00222BD3", (uint8_t*)x_DWORD_F2C20ar, 0x222bd3, debugcounter_DrawSprite_41BD3, 0x28 * 4, origbyte2y, remakebyte2y, remakepos2y);

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
				str_F2C20ar.dword0x04_screenY -= ((str_F2C20ar.dword0x11 * str_F2C20ar.dword0x09_realWidth >> 1) + str_F2C20ar.dword0x0d * str_F2C20ar.dword0x0c_realHeight) >> 16;
				str_F2C20ar.dword0x03_screenX -= (str_F2C20ar.dword0x11 * str_F2C20ar.dword0x0c_realHeight - (str_F2C20ar.dword0x0d * str_F2C20ar.dword0x09_realWidth >> 1)) >> 16;
				goto LABEL_126;
			}
			if (a1 != 2)//a1 == 0,1
			{
			LABEL_126:
				if ((unsigned int)str_F2C20ar.dword0x1e <= 7)
				{
					switch (str_F2C20ar.dword0x1e)//mirroring
					{
					case 0:
						v156 = str_F2C20ar.dword0x1b * str_F2C20ar.dword0x09_realWidth >> 16;
						if (v156 <= 0)
							break;
						v159 = (str_F2C20ar.dword0x0c_realHeight << 16) / str_F2C20ar.dword0x1b;
						if (v159 <= 0)
							break;
						v135 = (str_F2C20ar.dword0x06_height << 16) / v159;
						if (a1 == 1)
						{
							str_F2C20ar.dword0x0a_actIdx = 0;
						}
						else
						{
							str_F2C20ar.dword0x0a_actIdx = (v159 - 1) * v135;
							v135 = -v135;
						}
						v160 = str_F2C20ar.dword0x27 * str_F2C20ar.dword0x0c_realHeight / v159;
						v162 = (int*)(&x_DWORD_E9C38_smalltit[45920]);
						v157 = str_F2C20ar.dword0x04_screenY << 16;
						v170 = str_F2C20ar.dword0x03_screenX - (str_F2C20ar.dword0x1f * str_F2C20ar.dword0x04_screenY >> 16);
						if (str_F2C20ar.dword0x03_screenX - (str_F2C20ar.dword0x1f * str_F2C20ar.dword0x04_screenY >> 16) >= str_F2C20ar.dword0x21)
							goto LABEL_136;
						v139 = str_F2C20ar.dword0x21 - v170;
						v159 -= str_F2C20ar.dword0x21 - v170;
						if (v159 > 0)
						{
							str_F2C20ar.dword0x0a_actIdx += v139 * v135;
							v157 -= v139 * v160;
							v170 = str_F2C20ar.dword0x21;
						LABEL_136:
							v123 = iScreenWidth_DE560 * v170 + ViewPortRenderBufferStart_DE558;
							goto LABEL_137;
						}
						break;
					case 1:
						v156 = str_F2C20ar.dword0x27 * str_F2C20ar.dword0x09_realWidth >> 16;
						if (v156 <= 0)
							break;
						v159 = (str_F2C20ar.dword0x0c_realHeight << 16) / str_F2C20ar.dword0x27;
						if (v159 <= 0)
							break;
						v135 = (str_F2C20ar.dword0x06_height << 16) / v159;
						if (a1 == 1)
						{
							str_F2C20ar.dword0x0a_actIdx = 0;
						}
						else
						{
							str_F2C20ar.dword0x0a_actIdx = (v159 - 1) * v135;
							v135 = -v135;
						}
						v161 = str_F2C20ar.dword0x1b * str_F2C20ar.dword0x0c_realHeight / v159;
						v166 = (int*)(&x_DWORD_E9C38_smalltit[45920]);
						v158 = str_F2C20ar.dword0x03_screenX << 16;
						v171 = str_F2C20ar.dword0x04_screenY - (str_F2C20ar.dword0x1f * str_F2C20ar.dword0x03_screenX >> 16);
						if (str_F2C20ar.dword0x04_screenY - (str_F2C20ar.dword0x1f * str_F2C20ar.dword0x03_screenX >> 16) < str_F2C20ar.width0x25)
						{
							if (v171 >= str_F2C20ar.dword0x21)
								goto LABEL_284;
						}
						else
						{
							v141 = v171 - str_F2C20ar.width0x25;
							v159 -= v171 - str_F2C20ar.width0x25;
							if (v159 > 0)
							{
								str_F2C20ar.dword0x0a_actIdx += v141 * v135;
								v158 += v141 * v161;
								v171 = str_F2C20ar.width0x25;
							LABEL_284:
								v123 = v171 + ViewPortRenderBufferStart_DE558;
								goto LABEL_285;
							}
						}
						break;
					case 2:
						v156 = str_F2C20ar.dword0x1b * str_F2C20ar.dword0x09_realWidth >> 16;
						if (v156 <= 0)
							break;
						v159 = (str_F2C20ar.dword0x0c_realHeight << 16) / str_F2C20ar.dword0x1b;
						if (v159 <= 0)
							break;
						v135 = (str_F2C20ar.dword0x06_height << 16) / v159;
						if (a1 == 1)
						{
							str_F2C20ar.dword0x0a_actIdx = 0;
						}
						else
						{
							str_F2C20ar.dword0x0a_actIdx = (v159 - 1) * v135;
							v135 = -v135;
						}
						v160 = str_F2C20ar.dword0x27 * str_F2C20ar.dword0x0c_realHeight / v159;
						v162 = (int*)(&x_DWORD_E9C38_smalltit[45920]);
						v157 = str_F2C20ar.dword0x03_screenX << 16;
						v170 = str_F2C20ar.width0x25 - str_F2C20ar.dword0x04_screenY - (str_F2C20ar.dword0x1f * str_F2C20ar.dword0x03_screenX >> 16);
						if (v170 >= str_F2C20ar.dword0x21)
							goto LABEL_329;
						v147 = str_F2C20ar.dword0x21 - v170;
						v159 -= str_F2C20ar.dword0x21 - v170;
						if (v159 <= 0)
							break;
						str_F2C20ar.dword0x0a_actIdx += v147 * v135;
						v157 -= v147 * v160;
						v170 = str_F2C20ar.dword0x21;
					LABEL_329:
						v123 = str_F2C20ar.width0x25 + ViewPortRenderBufferStart_DE558 - 1 - v170;
						goto LABEL_137;
					case 3:
						v156 = str_F2C20ar.dword0x27 * str_F2C20ar.dword0x09_realWidth >> 16;
						if (v156 <= 0)
							break;
						v159 = (str_F2C20ar.dword0x0c_realHeight << 16) / str_F2C20ar.dword0x27;
						if (v159 <= 0)
							break;
						v135 = (str_F2C20ar.dword0x06_height << 16) / v159;
						if (a1 == 1)
						{
							str_F2C20ar.dword0x0a_actIdx = 0;
						}
						else
						{
							str_F2C20ar.dword0x0a_actIdx = (v159 - 1) * v135;
							v135 = -v135;
						}
						v161 = str_F2C20ar.dword0x1b * str_F2C20ar.dword0x0c_realHeight / v159;
						v166 = (int*)(&x_DWORD_E9C38_smalltit[45920]);
						v158 = (str_F2C20ar.height0x26 - str_F2C20ar.dword0x04_screenY) << 16;
						v171 = str_F2C20ar.dword0x03_screenX - (str_F2C20ar.dword0x1f * (str_F2C20ar.height0x26 - str_F2C20ar.dword0x04_screenY) >> 16);
						if (str_F2C20ar.dword0x03_screenX - (str_F2C20ar.dword0x1f * (str_F2C20ar.height0x26 - str_F2C20ar.dword0x04_screenY) >> 16) < str_F2C20ar.width0x25)
						{
							if (v171 < str_F2C20ar.dword0x21)
								break;
						}
						else
						{
							v148 = v171 - str_F2C20ar.width0x25;
							v159 -= v171 - str_F2C20ar.width0x25;
							if (v159 <= 0)
								break;
							str_F2C20ar.dword0x0a_actIdx += v148 * v135;
							v158 += v148 * v161;
							v171 = str_F2C20ar.width0x25;
						}
						v123 = str_F2C20ar.height0x26 + iScreenWidth_DE560 * v171 - 1 + ViewPortRenderBufferStart_DE558;
						goto LABEL_285;
					case 4:
						v156 = str_F2C20ar.dword0x1b * str_F2C20ar.dword0x09_realWidth >> 16;
						if (v156 <= 0)
							break;
						v159 = (str_F2C20ar.dword0x0c_realHeight << 16) / str_F2C20ar.dword0x1b;
						if (v159 <= 0)
							break;
						v135 = (str_F2C20ar.dword0x06_height << 16) / v159;
						if (a1 == 1)
						{
							str_F2C20ar.dword0x0a_actIdx = 0;
						}
						else
						{
							str_F2C20ar.dword0x0a_actIdx = (v159 - 1) * v135;
							v135 = -v135;
						}
						v160 = str_F2C20ar.dword0x27 * str_F2C20ar.dword0x0c_realHeight / v159;
						v162 = (int*)(&x_DWORD_E9C38_smalltit[45920]);
						v157 = (str_F2C20ar.height0x26 - str_F2C20ar.dword0x04_screenY) << 16;
						v170 = str_F2C20ar.width0x25 - str_F2C20ar.dword0x03_screenX - (str_F2C20ar.dword0x1f * (str_F2C20ar.height0x26 - str_F2C20ar.dword0x04_screenY) >> 16);
						if (v170 >= str_F2C20ar.dword0x21)
							goto LABEL_348;
						v149 = str_F2C20ar.dword0x21 - v170;
						v159 -= str_F2C20ar.dword0x21 - v170;
						if (v159 <= 0)
							break;
						str_F2C20ar.dword0x0a_actIdx += v149 * v135;
						v157 -= v149 * v160;
						v170 = str_F2C20ar.dword0x21;
					LABEL_348:
						v123 = str_F2C20ar.height0x26 + ViewPortRenderBufferStart_DE558 + iScreenWidth_DE560 * (str_F2C20ar.width0x25 - v170 - 1) - 1;
						goto LABEL_137;
					case 5:
						v156 = str_F2C20ar.dword0x27 * str_F2C20ar.dword0x09_realWidth >> 16;
						if (v156 <= 0)
							break;
						v159 = (str_F2C20ar.dword0x0c_realHeight << 16) / str_F2C20ar.dword0x27;
						if (v159 <= 0)
							break;
						v135 = (str_F2C20ar.dword0x06_height << 16) / v159;
						if (a1 == 1)
						{
							str_F2C20ar.dword0x0a_actIdx = 0;
						}
						else
						{
							str_F2C20ar.dword0x0a_actIdx = (v159 - 1) * v135;
							v135 = -v135;
						}
						v161 = str_F2C20ar.dword0x1b * str_F2C20ar.dword0x0c_realHeight / v159;
						v166 = (int*)(&x_DWORD_E9C38_smalltit[45920]);
						v158 = (str_F2C20ar.height0x26 - str_F2C20ar.dword0x03_screenX) << 16;
						v113 = str_F2C20ar.dword0x1f * (str_F2C20ar.height0x26 - str_F2C20ar.dword0x03_screenX) >> 16;
						v171 = str_F2C20ar.width0x25 - str_F2C20ar.dword0x04_screenY - v113;
						if (v171 < str_F2C20ar.width0x25)
						{
							if (v171 < str_F2C20ar.dword0x21)
								break;
						}
						else
						{
							v150 = v171 - str_F2C20ar.width0x25;
							v159 -= v171 - str_F2C20ar.width0x25;
							if (v159 <= 0)
								break;
							str_F2C20ar.dword0x0a_actIdx += v150 * v135;
							v158 += v150 * v161;
							v171 = str_F2C20ar.width0x25;
						}
						v123 = (str_F2C20ar.height0x26 - 1) * iScreenWidth_DE560 + str_F2C20ar.width0x25 + ViewPortRenderBufferStart_DE558 - 1 - v171;
						goto LABEL_285;
					case 6:
						v156 = str_F2C20ar.dword0x1b * str_F2C20ar.dword0x09_realWidth >> 16;
						if (v156 <= 0)
							break;
						v159 = (str_F2C20ar.dword0x0c_realHeight << 16) / str_F2C20ar.dword0x1b;
						if (v159 <= 0)
							break;
						v135 = (str_F2C20ar.dword0x06_height << 16) / v159;
						if (a1 == 1)
						{
							str_F2C20ar.dword0x0a_actIdx = 0;
						}
						else
						{
							str_F2C20ar.dword0x0a_actIdx = (v159 - 1) * v135;
							v135 = -v135;
						}
						v160 = str_F2C20ar.dword0x27 * str_F2C20ar.dword0x0c_realHeight / v159;
						v162 = (int*)(&x_DWORD_E9C38_smalltit[45920]);
						v157 = (str_F2C20ar.height0x26 - str_F2C20ar.dword0x03_screenX) << 16;
						v170 = str_F2C20ar.dword0x04_screenY - (str_F2C20ar.dword0x1f * (str_F2C20ar.height0x26 - str_F2C20ar.dword0x03_screenX) >> 16);
						if (str_F2C20ar.dword0x04_screenY - (str_F2C20ar.dword0x1f * (str_F2C20ar.height0x26 - str_F2C20ar.dword0x03_screenX) >> 16) >= str_F2C20ar.dword0x21)
							goto LABEL_367;
						v151 = str_F2C20ar.dword0x21 - v170;
						v159 -= str_F2C20ar.dword0x21 - v170;
						if (v159 <= 0)
							break;
						str_F2C20ar.dword0x0a_actIdx += v151 * v135;
						v157 -= v151 * v160;
						v170 = str_F2C20ar.dword0x21;
					LABEL_367:
						v123 = ViewPortRenderBufferStart_DE558 + iScreenWidth_DE560 * (str_F2C20ar.height0x26 - 1) + v170;
					LABEL_137:
						v140 = str_F2C20ar.width0x25 - str_F2C20ar.dword0x21;
						if (v170 <= 0)
						{
							if (v159 > v140)
								v159 = str_F2C20ar.width0x25 - str_F2C20ar.dword0x21;
						}
						else if (v170 + v159 > v140)
						{
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
								if (v162[1] + *v162 > str_F2C20ar.height0x26)
									v162[1] = str_F2C20ar.height0x26 - *v162;
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
						if (v170 + v159 > str_F2C20ar.Height_0x19)
						{
							if (str_F2C20ar.Height_0x19 - v170 <= 0)
							{
								v163 = (x_DWORD*)(&x_DWORD_E9C38_smalltit[45920]);
								v117 = (x_DWORD*)(4 * (str_F2C20ar.Height_0x19 - v170) + str_F2C20ar.pbyte0x1a);
								v125 = v159;
							}
							else
							{
								v163 = (x_DWORD*)(12 * (str_F2C20ar.Height_0x19 - v170) + &x_DWORD_E9C38_smalltit[45920]);
								v117 = (x_DWORD*)str_F2C20ar.pbyte0x1a;
								v125 = v170 + v159 - str_F2C20ar.Height_0x19;
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
						v137 = (str_F2C20ar.dword0x05 << 16) / v156;
						str_F2C20ar.dword0x0b = 0;
						if (str_F2C20ar.dword0x05 < 0)
							str_F2C20ar.dword0x0b -= v137 * (v156 - 1);
						v132 = v156 - v116;
						if (v156 - v116 > 0)
						{
							if (v132 > str_F2C20ar.dword0x1c)
								v132 = str_F2C20ar.dword0x1c;
							str_F2C20ar.dword0x0b += v137 * v116;
							v154 = (x_DWORD*)(&x_DWORD_E9C38_smalltit[36960]);
							while (v132 >= 0)
							{
								v154[1] = str_F2C20ar.dword0x0b >> 16;
								*v154 = v154[1] - *(v154 - 1);
								str_F2C20ar.dword0x0b += v137;
								v154 += 2;
								v132--;
							}
							v165 = (x_DWORD*)(&x_DWORD_E9C38_smalltit[45920]);
							for (i = v159; i; i--)
							{
								v133 = v165[1];
								if (v133 > 0)
								{
									//adress 2237d3

									v169x = &m_str_F0E20x[*v165];
									v155 = 8 * (v165[2] - v116) + &x_DWORD_E9C38_smalltit[36960];
									v121x = &str_F2C20ar.dword0x02_data[str_F2C20ar.dword0x08_width * (str_F2C20ar.dword0x0a_actIdx >> 16)];
									if ((unsigned int)str_F2C20ar.dword0x01_rotIdx <= 8)
									{
										switch (str_F2C20ar.dword0x01_rotIdx)
										{
										case 0:
											v51x = &v121x[*(x_DWORD*)(v155 + 4)];
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
											v62x = &v121x[*(x_DWORD*)(v155 + 4)];
											v63 = str_F2C20ar.dword0x00;
											v64 = (int8_t*)(v169x->dword_1 + v123);
											v65 = (x_DWORD*)(8 * (v165[2] - v116) + &x_DWORD_E9C38_smalltit[36960]);
											v66x = &m_str_F0E20x[*v165];
											v67 = v133 >> 1;
											if (!(v133 & 1))
											{
												v65 = (x_DWORD*)(v155 + 8);
												v66x = &v169x[1];
												goto LABEL_207;
											}
											v67++;
											while (1)
											{
												LOBYTE(v63) = v62x[0];
												v62x += v65[2];
												if ((x_BYTE)v63)
													*v64 = x_BYTE_F6EE0_tablesx[v63];
												v64 += v66x[1].dword_0;
												v65 += 4;
												v66x += 2;
												if (!--v67)
													break;
											LABEL_207:
												LOBYTE(v63) = v62x[0];
												v62x += *v65;
												if ((x_BYTE)v63)
													*v64 = x_BYTE_F6EE0_tablesx[v63];
												v64 += v66x->dword_0;
											}
											break;
										case 2:
											v68x = &v121x[*(x_DWORD*)(v155 + 4)];
											v69 = (x_BYTE*)(v169x->dword_1 + v123);
											v70 = (x_DWORD*)(8 * (v165[2] - v116) + &x_DWORD_E9C38_smalltit[36960]);
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
													*v69 = x_BYTE_F6EE0_tablesx[0x4000 + v72];
												}
												v69 += v71x[1].dword_0;
												v70 += 4;
												v71x += 2;
												if (!--v73)
													break;
											LABEL_217:
												HIBYTE(v72) = v68x[0];
												v68x += *v70;
												if (HIBYTE(v72))
												{
													LOBYTE(v72) = *v69;
													*v69 = x_BYTE_F6EE0_tablesx[0x4000 + v72];
												}
												v69 += v71x->dword_0;
											}
											break;
										case 3:
											v74x = &v121x[*(x_DWORD*)(v155 + 4)];
											v75 = (x_BYTE*)(v169x->dword_1 + v123);
											v76 = (x_DWORD*)(8 * (v165[2] - v116) + &x_DWORD_E9C38_smalltit[36960]);//to position
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
													*v75 = x_BYTE_F6EE0_tablesx[0x4000 + v78];
												}
												v75 += v77x[1].dword_0;
												v76 += 4;
												v77x += 2;
												if (!--v79)
													break;
											LABEL_227:
												LOBYTE(v78) = v74x[0];
												v74x += *v76;
												if ((x_BYTE)v78)
												{
													HIBYTE(v78) = *v75;
													*v75 = x_BYTE_F6EE0_tablesx[0x4000 + v78];
												}
												v75 += v77x->dword_0;
											}
											break;
										case 4:
											v80x = &v121x[*(x_DWORD*)(v155 + 4)];
											HIWORD(v84) = HIWORD(str_F2C20ar.dword0x07);
											v81 = (x_BYTE*)(v169x->dword_1 + v123);
											v82 = (x_DWORD*)(8 * (v165[2] - v116) + &x_DWORD_E9C38_smalltit[36960]);
											v83x = &m_str_F0E20x[*v165];
											BYTE1(v84) = str_F2C20ar.dword0x07;
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
													*v81 = x_BYTE_F6EE0_tablesx[0x4000 + v84];
												v81 += v83x[1].dword_0;
												v82 += 4;
												v83x += 2;
												if (!--v85)
													break;
											LABEL_237:
												LOBYTE(v84) = v80x[0];
												v80x += *v82;
												if ((x_BYTE)v84)
													*v81 = x_BYTE_F6EE0_tablesx[0x4000 + v84];
												v81 += v83x->dword_0;
											}
											break;
										case 5:
											v86x = &v121x[*(x_DWORD*)(v155 + 4)];
											v87 = str_F2C20ar.dword0x07;
											v88 = (x_BYTE*)(v169x->dword_1 + v123);
											v89 = (x_DWORD*)(8 * (v165[2] - v116) + &x_DWORD_E9C38_smalltit[36960]);
											v90x = &m_str_F0E20x[*v165];
											v91 = v133 >> 1;
											if (!(v133 & 1))
											{
												v89 = (x_DWORD*)(v155 + 8);
												v90x = &m_str_F0E20x[1];
												goto LABEL_247;
											}
											v91++;
											while (1)
											{
												BYTE1(v87) = v86x[0];
												v86x += v89[2];
												if (BYTE1(v87))
													*v88 = x_BYTE_F6EE0_tablesx[0x4000 + v87];
												v88 += v90x[1].dword_0;
												v89 += 4;
												v90x += 2;
												if (!--v91)
													break;
											LABEL_247:
												BYTE1(v87) = v86x[0];
												v86x += *v89;
												if (BYTE1(v87))
													*v88 = x_BYTE_F6EE0_tablesx[0x4000 + v87];
												v88 += v90x->dword_0;
											}
											break;
										case 6:
											v92 = str_F2C20ar.dword0x00;
											v93x = &v121x[*(x_DWORD*)(v155 + 4)];
											v94 = (x_BYTE*)(v169x->dword_1 + v123);
											HIWORD(v95) = 0;
											v96 = (x_DWORD*)(v155 + 8);
											v97x = &v169x[1];
											do
											{
												BYTE1(v95) = v93x[0];
												v93x += *v96;
												if (BYTE1(v95))
												{
													LOBYTE(v95) = *v94;
													LOBYTE(v92) = x_BYTE_F6EE0_tablesx[0x4000 + v95];
													*v94 = x_BYTE_F6EE0_tablesx[v92];
												}
												v94 += v97x->dword_0;
												v96 += 2;
												v97x++;
												v133--;
											} while (v133);
											break;
										case 7:
											v98 = str_F2C20ar.dword0x00;
											v99x = &v121x[*(x_DWORD*)(v155 + 4)];
											v100 = (x_BYTE*)(v169x->dword_1 + v123);
											HIWORD(v101) = 0;
											v102 = (x_DWORD*)(v155 + 8);
											v103x = &v169x[1];
											do
											{
												LOBYTE(v101) = v99x[0];
												v99x += *v102;
												if ((x_BYTE)v101)
												{
													BYTE1(v101) = *v100;
													LOBYTE(v98) = x_BYTE_F6EE0_tablesx[0x4000 + v101];
													*v100 = x_BYTE_F6EE0_tablesx[v98];
												}
												v100 += v103x->dword_0;
												v102 += 2;
												v103x++;
												v133--;
											} while (v133);
											break;
										case 8:
											v104x = &v121x[*(x_DWORD*)(v155 + 4)];
											v105 = str_F2C20ar.dword0x00;
											v106 = (x_BYTE*)(v169x->dword_1 + v123);
											v107 = (x_DWORD*)(8 * (v165[2] - v116) + &x_DWORD_E9C38_smalltit[36960]);
											v108x = &m_str_F0E20x[*v165];
											v109 = v133 >> 1;
											if (!(v133 & 1))
											{
												v107 = (x_DWORD*)(v155 + 8);
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
													*v106 = x_BYTE_F6EE0_tablesx[v105];
												}
												v106 += v108x[3].dword_0;
												v107 += 4;
												v108x += 2;
												if (!--v109)
													break;
											LABEL_267:
												v110 = v104x[0];
												v104x += *v107;
												if (v110)
												{
													LOBYTE(v105) = *v106;
													*v106 = x_BYTE_F6EE0_tablesx[v105];
												}
												v106 += v108x->dword_0;
											}
											break;
										}
									}
								}
								str_F2C20ar.dword0x0a_actIdx += v135;
								v123 += str_F2C20ar.dword0x23;
								v165 += 3;
							}
						}
						break;
					case 7:
						v156 = str_F2C20ar.dword0x27 * str_F2C20ar.dword0x09_realWidth >> 16;
						if (v156 <= 0)
							break;
						v159 = (str_F2C20ar.dword0x0c_realHeight << 16) / str_F2C20ar.dword0x27;
						if (v159 <= 0)
							break;
						if (str_F2C20ar.dword0x04_screenY >= str_F2C20ar.height0x26)
							break;
						v135 = (str_F2C20ar.dword0x06_height << 16) / v159;
						if (a1 == 1)
						{
							str_F2C20ar.dword0x0a_actIdx = 0;
						}
						else
						{
							str_F2C20ar.dword0x0a_actIdx = (v159 - 1) * v135;
							v135 = -v135;
						}
						v161 = str_F2C20ar.dword0x1b * str_F2C20ar.dword0x0c_realHeight / v159;
						v166 = (int*)(&x_DWORD_E9C38_smalltit[45920]);
						v158 = str_F2C20ar.dword0x04_screenY << 16;
						v114 = str_F2C20ar.dword0x1f * str_F2C20ar.dword0x04_screenY >> 16;
						v171 = str_F2C20ar.width0x25 - str_F2C20ar.dword0x03_screenX - v114;
						if (v171 < str_F2C20ar.width0x25)
						{
							if (v171 < str_F2C20ar.dword0x21)
								break;
						}
						else
						{
							v152 = v171 - str_F2C20ar.width0x25;
							v159 -= v171 - str_F2C20ar.width0x25;
							if (v159 <= 0)
								break;
							str_F2C20ar.dword0x0a_actIdx += v152 * v135;
							v158 += v152 * v161;
							v171 = str_F2C20ar.width0x25;
						}
						v123 = iScreenWidth_DE560 * (str_F2C20ar.width0x25 - 1 - v171) + ViewPortRenderBufferStart_DE558;
					LABEL_285:
						v142 = str_F2C20ar.width0x25 - str_F2C20ar.dword0x21;
						if (v171 < str_F2C20ar.width0x25)
						{
							if (v159 > v142)
								v159 = str_F2C20ar.width0x25 - str_F2C20ar.dword0x21;
						}
						else if (v171 + v159 - str_F2C20ar.width0x25 > v142)
						{
							v159 = v142 - v171 + str_F2C20ar.width0x25;
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
							if (v166[1] + v166[0] > str_F2C20ar.height0x26)
								v166[1] = str_F2C20ar.height0x26 - v166[0];
							v158 += v161;
							v166 += 3;
						}
						v143 = v171 + str_F2C20ar.dword0x1d - str_F2C20ar.width0x25 + 1;
						if (v143 > 0)
						{
							v167 = (x_DWORD*)(12 * v143 + &x_DWORD_E9C38_smalltit[45920]);
							v119 = (int32_t*)str_F2C20ar.pbyte0x1a;
							for (k = v171 + str_F2C20ar.dword0x1d - str_F2C20ar.width0x25 + 2; k; k--)
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
							v145 = v171 - v159 - str_F2C20ar.dword0x21;
							if (v145 < 0)
							{
								v159 = v171 - str_F2C20ar.dword0x21;
								if (v171 - str_F2C20ar.dword0x21 <= 0)
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
						&& str_F2C20ar.dword0x14x->type_0x3F_63 == 3
						&& (!str_F2C20ar.dword0x14x->subtype_0x40_64 || str_F2C20ar.dword0x14x->subtype_0x40_64 == 1))
					{
						DrawSorcererNameAndHealthBar_2CB30(str_F2C20ar.dword0x14x, str_F2C20ar.dword0x04_screenY, (signed __int16)str_F2C20ar.dword0x03_screenX, str_F2C20ar.dword0x09_realWidth);
					}
					if (x_D41A0_BYTEARRAY_4_struct.byteindex_10)
						sub_88740(
							str_F2C20ar.dword0x14x,
							(signed __int16)(str_F2C20ar.dword0x04_screenY + (str_F2C20ar.dword0x09_realWidth >> 1)),
							(signed __int16)(str_F2C20ar.dword0x03_screenX + (str_F2C20ar.dword0x0c_realHeight >> 1)));
					if (str_F2C20ar.dword0x14x->struct_byte_0xc_12_15.byte[3] & 0x40)
					{
						str_F2C20ar.dword0x14x->word_0x2A_42 |= 0x40u;
					}
				}
				return;
			}
		}
		str_F2C20ar.dword0x04_screenY -= str_F2C20ar.dword0x11 * str_F2C20ar.dword0x09_realWidth >> 17;
		str_F2C20ar.dword0x03_screenX -= -(str_F2C20ar.dword0x0d * str_F2C20ar.dword0x09_realWidth) >> 17;
		goto LABEL_126;
	}
	v138 = (str_F2C20ar.dword0x0c_realHeight + str_F2C20ar.dword0x09_realWidth) >> 2;
	if (a1 >= 1)
	{
		if (a1 <= 1)
		{
			str_F2C20ar.dword0x04_screenY += -(str_F2C20ar.dword0x0d * v138 >> 16) - v138;
			str_F2C20ar.dword0x03_screenX += -(str_F2C20ar.dword0x11 * v138 >> 16) - v138;
		}
		else if (a1 == 2)
		{
			str_F2C20ar.dword0x04_screenY += (str_F2C20ar.dword0x0d * v138 >> 16) - v138;
			str_F2C20ar.dword0x03_screenX += (str_F2C20ar.dword0x11 * v138 >> 16) - v138;
		}
	}
	if ((unsigned __int16)viewPort.Width_DE564 > str_F2C20ar.dword0x04_screenY)
	{
		v136 = (str_F2C20ar.dword0x05 << 16) / str_F2C20ar.dword0x09_realWidth;
		if (-str_F2C20ar.dword0x04_screenY < 0 || str_F2C20ar.dword0x04_screenY == 0)
		{
			str_F2C20ar.dword0x0b = 0;
			if (str_F2C20ar.dword0x09_realWidth + str_F2C20ar.dword0x04_screenY - (unsigned __int16)viewPort.Width_DE564 > 0)
				str_F2C20ar.dword0x09_realWidth -= str_F2C20ar.dword0x09_realWidth + str_F2C20ar.dword0x04_screenY - (unsigned __int16)viewPort.Width_DE564;
		}
		else
		{
			str_F2C20ar.dword0x09_realWidth += str_F2C20ar.dword0x04_screenY;
			if (str_F2C20ar.dword0x09_realWidth <= 0)
				return;
			str_F2C20ar.dword0x0b = v136 * -str_F2C20ar.dword0x04_screenY;
			str_F2C20ar.dword0x04_screenY = 0;
			if ((unsigned __int16)viewPort.Width_DE564 <= str_F2C20ar.dword0x09_realWidth)
				str_F2C20ar.dword0x09_realWidth = (unsigned __int16)viewPort.Width_DE564;
		}
		if ((unsigned __int16)viewPort.Height_DE568 > str_F2C20ar.dword0x03_screenX)
		{
			v134 = (str_F2C20ar.dword0x06_height << 16) / str_F2C20ar.dword0x0c_realHeight;
			if (-str_F2C20ar.dword0x03_screenX < 0 || str_F2C20ar.dword0x03_screenX == 0)
			{
				str_F2C20ar.dword0x0a_actIdx = 0;
				if (str_F2C20ar.dword0x0c_realHeight + str_F2C20ar.dword0x03_screenX - (unsigned __int16)viewPort.Height_DE568 > 0)
					str_F2C20ar.dword0x0c_realHeight -= str_F2C20ar.dword0x0c_realHeight + str_F2C20ar.dword0x03_screenX - (unsigned __int16)viewPort.Height_DE568;
			}
			else
			{
				str_F2C20ar.dword0x0c_realHeight += str_F2C20ar.dword0x03_screenX;
				if (str_F2C20ar.dword0x0c_realHeight <= 0)
					return;
				str_F2C20ar.dword0x0a_actIdx = v134 * -str_F2C20ar.dword0x03_screenX;
				str_F2C20ar.dword0x03_screenX = 0;
				if ((unsigned __int16)viewPort.Height_DE568 <= str_F2C20ar.dword0x0c_realHeight)
					str_F2C20ar.dword0x0c_realHeight = (unsigned __int16)viewPort.Height_DE568;
			}
			v153 = (x_DWORD*)(&x_DWORD_E9C38_smalltit[36960]);
			for (l = str_F2C20ar.dword0x09_realWidth; l; l--)
			{
				v153[1] = str_F2C20ar.dword0x0b >> 16;
				if ((x_DWORD*)(&x_DWORD_E9C38_smalltit[36960]) == v153)
					*v153 = 22;
				else
					*v153 = v153[1] - *(v153 - 1);
				str_F2C20ar.dword0x0b += v136;
				v153 += 2;
			}
			if (a1 == 1 && x_D41A0_BYTEARRAY_4_struct.byteindex_10)
				sub_88740(
					str_F2C20ar.dword0x14x,
					(signed __int16)(str_F2C20ar.dword0x04_screenY + (str_F2C20ar.dword0x09_realWidth >> 1)),
					(signed __int16)(str_F2C20ar.dword0x03_screenX + (str_F2C20ar.dword0x0c_realHeight >> 1)));
			//               screen-Y                                            screen-X
			//v122 = (int)(str_F2C20ar.dword0x03_screenX * iScreenWidth_DE560 + str_F2C20ar.dword0x04_screenY + x_DWORD_DE558);
			v122x = &ViewPortRenderBufferStart_DE558[str_F2C20ar.dword0x03_screenX * iScreenWidth_DE560 + str_F2C20ar.dword0x04_screenY];
			//height
			while (str_F2C20ar.dword0x0c_realHeight)
			{
				if ((unsigned int)str_F2C20ar.dword0x01_rotIdx <= 7)
				{
					switch (str_F2C20ar.dword0x01_rotIdx)//mirroring
					{
					case 0:
						//               width                  actual line                   base adress                 add index
						//v2 = (char*)(str_F2C20ar.dword0x08_width * (str_F2C20ar.dword0x0a_actIdx >> 16) + (int)str_F2C20ar.dword0x02_data + *(x_DWORD*)(x_DWORD_E9C38_smalltit + 36964));//sprite
						v2x = &str_F2C20ar.dword0x02_data[str_F2C20ar.dword0x08_width * (str_F2C20ar.dword0x0a_actIdx >> 16) + *(x_DWORD*)(x_DWORD_E9C38_smalltit + 36964)];//sprite
						v3 = (x_DWORD*)&(x_DWORD_E9C38_smalltit[36960]);
						v4 = v122x;//screen
						//   parametres
						v5 = str_F2C20ar.dword0x09_realWidth >> 1;
						if (!(str_F2C20ar.dword0x09_realWidth & 1))//bit 1
						{
							v6 = v5 & 1;//bit 2
							v7 = str_F2C20ar.dword0x09_realWidth >> 2;//bits 3-8 -> 1-6
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
						v13 = str_F2C20ar.dword0x00;
						v14 = (x_BYTE*)(str_F2C20ar.dword0x08_width * (str_F2C20ar.dword0x0a_actIdx >> 16) + str_F2C20ar.dword0x02_data + *(x_DWORD*)(x_DWORD_E9C38_smalltit + 36964));
						v15 = (x_DWORD*)(&x_DWORD_E9C38_smalltit[36960]);
						v16 = (uint8_t*)v122x;
						v17 = str_F2C20ar.dword0x09_realWidth >> 1;
						if (!(str_F2C20ar.dword0x09_realWidth & 1))
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
								v16[1] = x_BYTE_F6EE0_tablesx[v13];
							v16 += 2;
							v15 += 4;
							if (!--v17)
								break;
						LABEL_58:
							LOBYTE(v13) = *v14;
							v14 += *v15;
							if ((x_BYTE)v13)
								*v16 = x_BYTE_F6EE0_tablesx[v13];
						}
						break;
					case 2:
						v18 = (x_BYTE*)(str_F2C20ar.dword0x08_width * (str_F2C20ar.dword0x0a_actIdx >> 16) + str_F2C20ar.dword0x02_data + *(x_DWORD*)(x_DWORD_E9C38_smalltit + 36964));
						v19 = (x_DWORD*)(&x_DWORD_E9C38_smalltit[36960]);
						v20 = (uint8_t*)v122x;
						HIWORD(v21) = 0;
						v22 = str_F2C20ar.dword0x09_realWidth >> 1;
						if (!(str_F2C20ar.dword0x09_realWidth & 1))
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
								v20[1] = x_BYTE_F6EE0_tablesx[0x4000 + v21];
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
								v20[0] = x_BYTE_F6EE0_tablesx[0x4000 + v21];
							}
						}
						break;
					case 3:
						v23 = (x_BYTE*)(str_F2C20ar.dword0x08_width * (str_F2C20ar.dword0x0a_actIdx >> 16) + str_F2C20ar.dword0x02_data + *(x_DWORD*)(x_DWORD_E9C38_smalltit + 36964));//from mask
						v24 = (x_DWORD*)(&x_DWORD_E9C38_smalltit[36960]);//from image
						v25 = v122x;//to adress
						HIWORD(v26) = 0;
						v27 = str_F2C20ar.dword0x09_realWidth >> 1;
						if (!(str_F2C20ar.dword0x09_realWidth & 1))
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
								v25[1] = x_BYTE_F6EE0_tablesx[0x4000 + v26];
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
								v25[0] = x_BYTE_F6EE0_tablesx[0x4000 + v26];
							}
						}
						break;
					case 4:
						HIWORD(v31) = HIWORD(str_F2C20ar.dword0x07);
						v28 = (x_BYTE*)(str_F2C20ar.dword0x08_width * (str_F2C20ar.dword0x0a_actIdx >> 16) + str_F2C20ar.dword0x02_data + *(x_DWORD*)(x_DWORD_E9C38_smalltit + 36964));
						v29 = (x_DWORD*)(&x_DWORD_E9C38_smalltit[36960]);
						v30 = v122x;
						HIBYTE(v31) = str_F2C20ar.dword0x07;
						v32 = str_F2C20ar.dword0x09_realWidth >> 1;
						if (!(str_F2C20ar.dword0x09_realWidth & 1))
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
								v30[1] = x_BYTE_F6EE0_tablesx[0x4000 + v31];
							v30 += 2;
							v29 += 4;
							if (!--v32)
								break;
						LABEL_85:
							LOBYTE(v31) = *v28;
							v28 += *v29;
							if ((x_BYTE)v31)
								*v30 = x_BYTE_F6EE0_tablesx[0x4000 + v31];
						}
						break;
					case 5:
						v33 = str_F2C20ar.dword0x07;
						v34 = (x_BYTE*)(str_F2C20ar.dword0x08_width * (str_F2C20ar.dword0x0a_actIdx >> 16) + str_F2C20ar.dword0x02_data + *(x_DWORD*)(x_DWORD_E9C38_smalltit + 36964));
						v35 = (x_DWORD*)(&x_DWORD_E9C38_smalltit[36960]);
						v36 = v122x;
						v37 = str_F2C20ar.dword0x09_realWidth >> 1;
						if (!(str_F2C20ar.dword0x09_realWidth & 1))
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
								v36[1] = x_BYTE_F6EE0_tablesx[0x4000 + v33];
							v36 += 2;
							v35 += 4;
							if (!--v37)
								break;
						LABEL_94:
							BYTE1(v33) = *v34;
							v34 += *v35;
							if (BYTE1(v33))
								*v36 = x_BYTE_F6EE0_tablesx[0x4000 + v33];
						}
						break;
					case 6:
						v38 = str_F2C20ar.dword0x00;
						v39 = (x_BYTE*)(str_F2C20ar.dword0x08_width * (str_F2C20ar.dword0x0a_actIdx >> 16) + str_F2C20ar.dword0x02_data + *(x_DWORD*)(x_DWORD_E9C38_smalltit + 36964));
						v40 = (x_DWORD*)(&x_DWORD_E9C38_smalltit[36960]);
						v41 = v122x;
						HIWORD(v42) = 0;
						v43 = str_F2C20ar.dword0x09_realWidth >> 1;
						if (!(str_F2C20ar.dword0x09_realWidth & 1))
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
								LOBYTE(v38) = x_BYTE_F6EE0_tablesx[0x4000 + v42];
								v41[1] = x_BYTE_F6EE0_tablesx[v38];
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
								LOBYTE(v38) = x_BYTE_F6EE0_tablesx[0x4000 + v42];
								*v41 = x_BYTE_F6EE0_tablesx[v38];
							}
						}
						break;
					case 7:
						v44 = str_F2C20ar.dword0x00;
						v45 = (x_BYTE*)(str_F2C20ar.dword0x08_width * (str_F2C20ar.dword0x0a_actIdx >> 16) + str_F2C20ar.dword0x02_data + *(x_DWORD*)(x_DWORD_E9C38_smalltit + 36964));
						v46 = (x_DWORD*)(&x_DWORD_E9C38_smalltit[36960]);
						v47 = v122x;
						HIWORD(v48) = 0;
						v49 = str_F2C20ar.dword0x09_realWidth >> 1;
						if (!(str_F2C20ar.dword0x09_realWidth & 1))
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
								LOBYTE(v44) = x_BYTE_F6EE0_tablesx[0x4000 + v48];
								v47[1] = x_BYTE_F6EE0_tablesx[v44];
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
								LOBYTE(v44) = x_BYTE_F6EE0_tablesx[0x4000 + v48];
								*v47 = x_BYTE_F6EE0_tablesx[v44];
							}
						}
						break;
					}
				}
				str_F2C20ar.dword0x0a_actIdx += v134;
				v122x += iScreenWidth_DE560;
				str_F2C20ar.dword0x0c_realHeight--;
			}
		}
	}

	/*if (debugafterload)
		VGA_Debug_Blit(640, 480, pdwScreenBuffer);*/
}

#define _PIXEL	BYTE

#define _GetRValue(c)	((BYTE)((c) & 0xFF))		// Red color component
#define _GetGValue(c)	((BYTE)((c) & 0xFF))		// Green color component
#define _GetBValue(c)	((BYTE)((c) & 0xFF))		// Blue color component

typedef long fixed;
#define itofx(x) ((x) << 8)										// Integer to fixed point
#define ftofx(x) (long)((x) * 256)								// Float to fixed point
#define dtofx(x) (long)((x) * 256)								// Double to fixed point
#define fxtoi(x) ((x) >> 8)										// Fixed point to integer
#define fxtof(x) ((float) (x) / 256)							// Fixed point to float
#define fxtod(x) ((double)(x) / 256)							// Fixed point to double
#define Mulfx(x,y) (((x) * (y)) >> 8)							// Multiply a fixed by a fixed
#define Divfx(x,y) (((x) << 8) / (y))							// Divide a fixed by a fixed

typedef struct __POINT2D
{
	long x;
	long y;

} _POINT2D, * _LPPOINT2D;

typedef struct __POINT3DI
{
	long x;
	long y;
	long z;
	long w;

} _POINT3DI, * _LPPOINT3DI;

typedef struct __POINT3D
{
	float x;
	float y;
	float z;
	float w;

} _POINT3D, * _LPPOINT3D;

typedef enum __TEXTURE_FILTERING
{
	_TEXTURE_FILTERING_NONE = 0x0000,
	_TEXTURE_FILTERING_BILINEAR = 0x0001

} _TEXTURE_FILTERING;

typedef struct __TEXTUREINFO
{
	LPBYTE lpTexture;
	DWORD dwSize;
	DWORD dwPitch;
	DWORD dwTextureOffset;
	_TEXTURE_FILTERING filter;

} _TEXTUREINFO, * _LPTEXTUREINFO;

typedef struct _TRIANGLE
{
	_POINT3DI p1;
	_POINT3DI p2;
	_POINT3DI p3;
	_PIXEL color1;
	_PIXEL color2;
	_PIXEL color3;
	_PIXEL color;
	_POINT2D t1;
	_POINT2D t2;
	_POINT2D t3;
	BYTE shadingMode;
	_TEXTUREINFO texture;

} TRIANGLE, * LPTRIANGLE;

typedef struct __VERTEX3D
{
	_POINT3D p;
	_POINT3D n;
	_PIXEL color;
	_POINT2D uv;

} _VERTEX3D, * _LPVERTEX3D;

BOOL m_bFollowInvert;
BOOL m_bInvert;
int m_iFillRate;
int m_iSkipRate;

typedef enum __SHADING_MODE
{
	_SHADING_MODE_NONE = 0x00,
	_SHADING_MODE_CONSTANT = 0x01,
	_SHADING_MODE_FLAT = 0x02,
	_SHADING_MODE_GOURAUD = 0x04,
	_SHADING_MODE_TEXTURE = 0x08

} _SHADING_MODE;

void RasterizeLine(_POINT3DI p1, _POINT3DI p2, _PIXEL color)
{
	_POINT3DI top, bottom;

	// Order points
	if (p1.y < p2.y)
	{
		top = p1;
		bottom = p2;
	}
	else
	{
		top = p2;
		bottom = p1;
	}

	// Line invisible
	if ((top.y > m_iHeight - 1) || (bottom.y < 0) ||
		((top.x > m_iWidth - 1) && (bottom.x > m_iWidth - 1)) ||
		((top.x < 0) && (bottom.x < 0)))
	{
		return;
	}

	// Clip top point
	long tx = top.x;
	long ty = top.y;
	if (top.y < 0)
	{
		ty = 0;
		tx = (long)(top.x + (-top.y) * (float)(bottom.x - top.x) / (float)(bottom.y - top.y));
		if (top.x < 0)
		{
			if (tx < 0)
			{
				tx = 0;
				ty = (long)(top.y + (-top.x) * (float)(bottom.y - top.y) / (float)(bottom.x - top.x));
			}
		}
		else if (top.x > m_iWidth - 1)
		{
			if (tx > m_iWidth - 1)
			{
				tx = m_iWidth - 1;
				ty = (long)(top.y + (m_iWidth - 1 - top.x) * (float)(bottom.y - top.y) / (float)(bottom.x - top.x));
			}
		}
	}
	else
	{
		if (top.x < 0)
		{
			tx = 0;
			ty = (long)(top.y + (-top.x) * (float)(bottom.y - top.y) / (float)(bottom.x - top.x));
		}
		else if (top.x > m_iWidth - 1)
		{
			tx = m_iWidth - 1;
			ty = (long)(top.y + (m_iWidth - 1 - top.x) * (float)(bottom.y - top.y) / (float)(bottom.x - top.x));
		}
	}

	// Clip bottom point
	long bx = bottom.x;
	long by = bottom.y;
	if (bottom.y > m_iHeight - 1)
	{
		by = m_iHeight - 1;
		bx = (long)(bottom.x + (m_iHeight - 1 - bottom.y) * (float)(top.x - bottom.x) / (float)(top.y - bottom.y));
		if (bottom.x < 0)
		{
			if (bx < 0)
			{
				bx = 0;
				by = (long)(bottom.y + (-bottom.x) * (float)(top.y - bottom.y) / (float)(top.x - bottom.x));
			}
		}
		else if (bottom.x > m_iWidth - 1)
		{
			if (bx > m_iWidth - 1)
			{
				bx = m_iWidth - 1;
				by = (long)(bottom.y + (m_iWidth - 1 - bottom.x) * (float)(top.y - bottom.y) / (float)(top.x - bottom.x));
			}
		}
	}
	else
	{
		if (bottom.x < 0)
		{
			bx = 0;
			by = (long)(bottom.y + (-bottom.x) * (float)(top.y - bottom.y) / (float)(top.x - bottom.x));
		}
		else if (bottom.x > m_iWidth - 1)
		{
			bx = m_iWidth - 1;
			by = (long)(bottom.y + (m_iWidth - 1 - bottom.x) * (float)(top.y - bottom.y) / (float)(top.x - bottom.x));
		}
	}
	if ((tx < 0) || (tx > m_iWidth - 1) || (ty < 0) || (ty > m_iHeight - 1) ||
		(bx < 0) || (bx > m_iWidth - 1) || (by < 0) || (by > m_iHeight - 1))
	{
		return;
	}
	else
	{
		top.x = tx;
		top.y = ty;
		bottom.x = bx;
		bottom.y = by;
	}

	LPBYTE lpData = m_lpData;
	DWORD dwTotalOffset = (m_iHeight - 1 - top.y) * m_iWidth;
	lpData += (dwTotalOffset + top.x);
	*lpData = color;
	LPBYTE lpTempData = lpData;

	float dx = (float)abs(bottom.x - top.x);
	float dy = (float)abs(bottom.y - top.y);
	float d = dy / dx;
	long width = abs(bottom.x - top.x);
	long height = abs(bottom.y - top.y);

	// Horizontal line
	if (dy == 0.0)
	{
		if (width == 0)
		{
			return;
		}

		if (bottom.x < top.x)
		{
			lpTempData += (bottom.x - top.x);
		}

		__asm {
			mov edi, lpTempData
			mov ecx, width
			mov al, color
			loop1 : mov[edi], al
			add edi, 1
			dec cx
			jnz loop1
		}
	}
	// Vertical line
	else if (dx == 0.0)
	{
		if (height == 0)
		{
			return;
		}

		DWORD dwOffset = m_iWidth * (_BITS_PER_PIXEL >> 3);
		__asm {
			mov edi, lpTempData
			mov ecx, height
			mov al, color
			loop2 : mov[edi], al
			sub edi, dwOffset
			dec cx
			jnz loop2
		}
	}
	// General line
	else
	{
		// X-dominant line
		if (dx > dy)
		{
			float y = (float)top.y;
			long oldy = top.y;
			long offsetX = 1;
			if (bottom.x < top.x)
				offsetX = -1;
			fixed f_y = ftofx(y);
			fixed f_d = ftofx(d);
			fixed f_oldy = itofx(oldy);
			fixed f_1 = itofx(1);
			DWORD dwOffsetX = offsetX * 4;
			DWORD dwOffsetY = m_iWidth * (_BITS_PER_PIXEL >> 3);
			__asm {
				mov edi, lpTempData
				mov ecx, width
				mov al, color
				mov ebx, f_y
				loop3 : add ebx, f_d
				mov edx, ebx
				sub edx, f_oldy
				cmp edx, f_1
				jl next1
				mov f_oldy, ebx
				and f_oldy, 0xFFFFFF00
				sub edi, dwOffsetY
				next1 : add edi, dwOffsetX
				mov[edi], al
				dec cx
				jnz loop3
			}
			lpData = m_lpData;
			dwTotalOffset = (m_iHeight - 1 - bottom.y) * m_iWidth;
			lpData += (dwTotalOffset + bottom.x);
			*lpData = color;
		}
		// Y-dominant line
		else
		{
			d = 1.0f / d;
			float x = (float)top.x;
			long oldx = top.x;
			long offsetX = 1;
			if (bottom.x < top.x)
				offsetX = -1;
			fixed f_x = ftofx(x);
			fixed f_d = ftofx(d);
			fixed f_oldx = itofx(oldx);
			fixed f_1 = itofx(1);
			DWORD dwOffsetX = offsetX * 4;
			DWORD dwOffsetY = m_iWidth * (_BITS_PER_PIXEL >> 3);
			__asm {
				mov edi, lpTempData
				mov ecx, height
				mov al, color
				mov ebx, f_x
				loop4 : add ebx, f_d
				mov edx, ebx
				sub edx, f_oldx
				cmp edx, f_1
				jl next2
				mov f_oldx, ebx
				and f_oldx, 0xFFFFFF00
				add edi, dwOffsetX
				next2 : sub edi, dwOffsetY
				mov[edi], al
				dec cx
				jnz loop4
			}
			lpData = m_lpData;
			dwTotalOffset = (m_iHeight - 1 - bottom.y) * m_iWidth;
			lpData += (dwTotalOffset + bottom.x);
			*lpData = color;
		}
	}
}

void Draw_Bottom_Triangle_Color(_VERTEX3D top, _VERTEX3D middle, _VERTEX3D bottom)
{
	_PIXEL color = top.color;
	long minY = (long)middle.p.y;
	long maxY = (long)bottom.p.y;
	long bottomX = (long)bottom.p.x;

	if ((minY > m_iHeight - 1) || (maxY < 0))
	{
		return;
	}

	LPBYTE lpData = m_lpData;
	LPBYTE lpZBufferData = m_lpZBuffer;
	DWORD dwTotalOffset = 0;
	if (maxY <= m_iHeight - 1)
	{
		dwTotalOffset = (m_iHeight - 1 - maxY) * m_iWidth;
	}
	else
	{
		dwTotalOffset = 0;
	}
	lpData += dwTotalOffset;
	lpZBufferData += dwTotalOffset;

	float xm, xb;
	long x, xmin, xmax, oldxmin, oldxmax;
	float dxm = -(float)(bottom.p.x - middle.p.x) / (float)(bottom.p.y - middle.p.y);
	float dxb = -(float)(bottom.p.x - top.p.x) / (float)(bottom.p.y - top.p.y);
	float dzm = -(1.0f / bottom.p.z - 1.0f / middle.p.z) / (bottom.p.y - middle.p.y);
	float dzb = -(1.0f / bottom.p.z - 1.0f / top.p.z) / (bottom.p.y - top.p.y);
	x = bottomX;
	xm = bottom.p.x;
	xb = bottom.p.x;
	xmin = x;
	xmax = x;
	oldxmin = x;
	oldxmax = x;
	float zm = 1.0f / bottom.p.z;
	float zb = 1.0f / bottom.p.z;
	for (long y1 = maxY; y1 >= minY; y1--)
	{
		xmin = (long)xm;
		xmax = (long)xb;
		if (xmin > xmax)
		{
			long temp = xmin;
			xmin = xmax;
			xmax = temp;
		}

		if (xmin > oldxmax)
		{
			xmin = oldxmax + 1;
		}
		else if (xmax < oldxmin)
		{
			xmax = oldxmin - 1;
		}

		if ((y1 >= 0) && (y1 <= m_iHeight - 1))
		{
			DWORD dwSize = xmax - xmin + 1;
			LPBYTE lpTempData = lpData;
			LPBYTE lpTempZBufferData = lpZBufferData;

			if ((xmin <= m_iWidth - 1) && (xmax >= 0))
			{
				float fSize = (float)dwSize;
				float dz = (zb - zm) / fSize;
				float z = zm;

				if (xmin < 0)
				{
					dwSize += xmin;
					z -= (xmin * dz);
				}
				else
				{
					lpTempData += xmin;
					lpTempZBufferData += xmin;
				}

				if (xmax > m_iWidth - 1)
				{
					dwSize -= (xmax - m_iWidth + 1);
				}

				/*				for (int x=0; x<(int)dwSize; x++)
								{
									float z_inv = 1.0f / z;
									if (z_inv < (*lpTempZBufferData))
									{
										*lpTempZBufferData = z_inv;
										*lpTempData++ = color;
									}
								}*/

				long dwFillRate = 0;
				byte zi;
				__asm {
					mov edi, lpTempData
					mov esi, lpTempZBufferData
					mov ecx, dwSize
					mov bl, color
					loop1 : fld1
					fdiv z
					frndint
					fistp zi
					mov al, zi
					cmp al, [esi]
					jge next1
					mov[esi], al
					mov[edi], bl
					inc dwFillRate
					next1 : fld z
					fadd dz
					fstp z
					add edi, 1
					add esi, 1
					dec ecx
					jnz loop1
				}
				m_iFillRate += dwFillRate;
				m_iSkipRate += (dwSize - dwFillRate);
			}
			lpData += m_iWidth;
			lpZBufferData += m_iWidth;
		}

		oldxmin = xmin;
		oldxmax = xmax;

		xm += dxm;
		xb += dxb;
		zm += dzm;
		zb += dzb;
	}
}

void Draw_Bottom_Triangle_Gouraud(_VERTEX3D top, _VERTEX3D middle, _VERTEX3D bottom)
{
	_PIXEL c1 = top.color;
	_PIXEL c2 = middle.color;
	_PIXEL c3 = bottom.color;
	long minY = (long)middle.p.y;
	long maxY = (long)bottom.p.y;
	long bottomX = (long)bottom.p.x;

	if ((minY > m_iHeight - 1) || (maxY < 0))
	{
		return;
	}

	LPBYTE lpData = m_lpData;
	LPBYTE lpZBufferData = m_lpZBuffer;
	DWORD dwTotalOffset = 0;
	if (maxY <= m_iHeight - 1)
	{
		dwTotalOffset = (m_iHeight - 1 - maxY) * m_iWidth;
	}
	else
	{
		dwTotalOffset = 0;
	}
	lpData += dwTotalOffset;
	lpZBufferData += dwTotalOffset;

	float cxm, cxb, red1, green1, blue1, red2, green2, blue2;
	float xm, xb, reds, greens, blues, rede, greene, bluee;
	long x, xmin, xmax, oldxmin, oldxmax;
	float dxm = -(bottom.p.x - middle.p.x) / (bottom.p.y - middle.p.y);
	float dxb = -(bottom.p.x - top.p.x) / (bottom.p.y - top.p.y);
	float dzm = -(1.0f / bottom.p.z - 1.0f / middle.p.z) / (bottom.p.y - middle.p.y);
	float dzb = -(1.0f / bottom.p.z - 1.0f / top.p.z) / (bottom.p.y - top.p.y);
	x = bottomX;
	xm = bottom.p.x;
	xb = bottom.p.x;
	xmin = x;
	xmax = x;
	oldxmin = x;
	oldxmax = x;
	float zm = 1.0f / bottom.p.z;
	float zb = 1.0f / bottom.p.z;
	long midY = (long)middle.p.y;
	long midX = (long)(top.p.x + (middle.p.y - top.p.y) * dxb);
	cxm = bottom.p.y - middle.p.y;
	cxb = bottom.p.y - top.p.y;
	if (cxm == 0.0f)
	{
		red1 = green1 = blue1 = 0.0f;
	}
	else
	{
		red1 = ((float)_GetRValue(c2) / middle.p.z - (float)_GetRValue(c3) / bottom.p.z) / cxm;
		green1 = ((float)_GetGValue(c2) / middle.p.z - (float)_GetGValue(c3) / bottom.p.z) / cxm;
		blue1 = ((float)_GetBValue(c2) / middle.p.z - (float)_GetBValue(c3) / bottom.p.z) / cxm;
	}
	if (cxb == 0.0f)
	{
		red2 = green2 = blue2 = 0.0f;
	}
	else
	{
		red2 = ((float)_GetRValue(c1) / top.p.z - (float)_GetRValue(c3) / bottom.p.z) / cxb;
		green2 = ((float)_GetGValue(c1) / top.p.z - (float)_GetGValue(c3) / bottom.p.z) / cxb;
		blue2 = ((float)_GetBValue(c1) / top.p.z - (float)_GetBValue(c3) / bottom.p.z) / cxb;
	}
	if (m_bFollowInvert == TRUE)
	{
		if (m_bInvert == TRUE)
		{
			float rt = red2;
			float gt = green2;
			float bt = blue2;
			red2 = red1;
			green2 = green1;
			blue2 = blue1;
			red1 = rt;
			green1 = gt;
			blue1 = bt;
			float zmt = dzm;
			dzm = dzb;
			dzb = zmt;
			m_bInvert = FALSE;
		}
	}
	else if (midX < (long)middle.p.x)
	{
		float rt = red2;
		float gt = green2;
		float bt = blue2;
		red2 = red1;
		green2 = green1;
		blue2 = blue1;
		red1 = rt;
		green1 = gt;
		blue1 = bt;
		float zmt = dzm;
		dzm = dzb;
		dzb = zmt;
		m_bInvert = FALSE;
	}
	reds = (float)_GetRValue(c3) / bottom.p.z;
	greens = (float)_GetGValue(c3) / bottom.p.z;
	blues = (float)_GetBValue(c3) / bottom.p.z;
	rede = reds;
	greene = greens;
	bluee = blues;

	for (long y1 = maxY; y1 >= minY; y1--)
	{
		xmin = (long)xm;
		xmax = (long)xb;
		if (xmin > xmax)
		{
			long temp = xmin;
			xmin = xmax;
			xmax = temp;
		}

		if (xmin > oldxmax)
		{
			xmin = oldxmax + 1;
		}
		else if (xmax < oldxmin)
		{
			xmax = oldxmin - 1;
		}

		if ((y1 >= 0) && (y1 <= m_iHeight - 1))
		{
			DWORD dwSize = xmax - xmin + 1;
			LPBYTE lpTempData = lpData;
			LPBYTE lpTempZBufferData = lpZBufferData;

			if ((xmin <= m_iWidth - 1) && (xmax >= 0))
			{
				float fSize = (float)dwSize;
				float dr = (rede - reds) / fSize;
				float dg = (greene - greens) / fSize;
				float db = (bluee - blues) / fSize;
				float dz = (zb - zm) / fSize;
				float r = reds;
				float g = greens;
				float b = blues;
				float z = zm;

				if (xmin < 0)
				{
					dwSize += xmin;
					r -= (xmin * dr);
					g -= (xmin * dg);
					b -= (xmin * db);
					z -= (xmin * dz);
				}
				else
				{
					lpTempData += xmin;
					lpTempZBufferData += xmin;
				}

				if (xmax > m_iWidth - 1)
				{
					dwSize -= (xmax - m_iWidth + 1);
				}

				/*				for (int x=0; x<(int)dwSize; x++)
								{
									float z_inv = 1.0f / z;
									if (z_inv < (*lpTempZBufferData))
									{
										*lpTempZBufferData = z_inv;
										long ri = max(0, min(255, (long)(r * z_inv)));
										long gi = max(0, min(255, (long)(g * z_inv)));
										long bi = max(0, min(255, (long)(b * z_inv)));
										*lpTempData++ = _RGB(ri, gi, bi);
										r += dr;
										g += dg;
										b += db;
										z += dz;
									}
								}*/

				float r_dr1 = r + dr;
				float r_dr2 = r_dr1 + dr;
				float r_dr3 = r_dr2 + dr;
				float r_dr4 = 4 * dr;
				float g_dg1 = g + dg;
				float g_dg2 = g_dg1 + dg;
				float g_dg3 = g_dg2 + dg;
				float g_dg4 = 4 * dg;
				float b_db1 = b + db;
				float b_db2 = b_db1 + db;
				float b_db3 = b_db2 + db;
				float b_db4 = 4 * db;
				float z_dz1 = z + dz;
				float z_dz2 = z_dz1 + dz;
				float z_dz3 = z_dz2 + dz;
				float z_dz4 = 4 * dz;
				__m128 simd_r = { r, r_dr1, r_dr2, r_dr3 };
				__m128 simd_g = { g, g_dg1, g_dg2, g_dg3 };
				__m128 simd_b = { b, b_db1, b_db2, b_db3 };
				__m128 simd_z = { z, z_dz1, z_dz2, z_dz3 };
				__m128 simd_dr = { r_dr4, r_dr4, r_dr4, r_dr4 };
				__m128 simd_dg = { g_dg4, g_dg4, g_dg4, g_dg4 };
				__m128 simd_db = { b_db4, b_db4, b_db4, b_db4 };
				__m128 simd_dz = { z_dz4, z_dz4, z_dz4, z_dz4 };
				long ri[4] = { 0, 0, 0, 0 };
				long gi[4] = { 0, 0, 0, 0 };
				long bi[4] = { 0, 0, 0, 0 };
				long zi[4] = { 0, 0, 0, 0 };

				long dwFillRate = 0;
				_asm {
					mov edi, lpTempData
					mov esi, lpTempZBufferData
					mov ecx, dwSize
					movaps xmm0, simd_r
					movaps xmm1, simd_g
					movaps xmm2, simd_b
					movaps xmm3, simd_z
					loop1 : movaps xmm4, xmm0
					movaps xmm5, xmm1
					movaps xmm6, xmm2
					rcpps xmm7, xmm3
					mulps xmm4, xmm7
					mulps xmm5, xmm7
					mulps xmm6, xmm7
					movaps simd_r, xmm4
					movaps simd_g, xmm5
					movaps simd_b, xmm6
					movaps simd_z, xmm7
					addps xmm0, simd_dr
					addps xmm1, simd_dg
					addps xmm2, simd_db
					addps xmm3, simd_dz
					fld simd_z[0]
					frndint
					fistp zi[0]
					mov eax, zi[0]
					cmp eax, [esi]
					jge next26
					mov[esi], eax
					fld simd_r[0]
					frndint
					fistp ri[0]
					cmp ri[0], 0
					jge next1
					mov ri[0], 0
					next1:				cmp ri[0], 255
					jle next2
					mov ri[0], 255
					next2 : fld simd_g[0]
					frndint
					fistp gi[0]
					cmp gi[0], 0
					jge next3
					mov gi[0], 0
					next3 : cmp gi[0], 255
					jle next4
					mov gi[0], 255
					next4 : fld simd_b[0]
					frndint
					fistp bi[0]
					cmp bi[0], 0
					jge next5
					mov bi[0], 0
					next5 : cmp bi[0], 255
					jle next6
					mov bi[0], 255
					next6 : mov eax, ri[0]
					shl eax, 8
					or eax, gi[0]
					shl eax, 8
					or eax, bi[0]
					mov[edi], eax
					inc dwFillRate
					next26 : add edi, 4
					add esi, 4
					dec ecx
					jz next25
					fld simd_z[4]
					frndint
					fistp zi[4]
					mov eax, zi[4]
					cmp eax, [esi]
					jge next27
					mov[esi], eax
					fld simd_r[4]
					frndint
					fistp ri[4]
					cmp ri[4], 0
					jge next7
					mov ri[4], 0
					next7:				cmp ri[4], 255
					jle next8
					mov ri[4], 255
					next8 : fld simd_g[4]
					frndint
					fistp gi[4]
					cmp gi[4], 0
					jge next9
					mov gi[4], 0
					next9 : cmp gi[4], 255
					jle next10
					mov gi[4], 255
					next10 : fld simd_b[4]
					frndint
					fistp bi[4]
					cmp bi[4], 0
					jge next11
					mov bi[4], 0
					next11 : cmp bi[4], 255
					jle next12
					mov bi[4], 255
					next12 : mov eax, ri[4]
					shl eax, 8
					or eax, gi[4]
					shl eax, 8
					or eax, bi[4]
					mov[edi], eax
					inc dwFillRate
					next27 : add edi, 4
					add esi, 4
					dec ecx
					jz next25
					fld simd_z[8]
					frndint
					fistp zi[8]
					mov eax, zi[8]
					cmp eax, [esi]
					jge next28
					mov[esi], eax
					fld simd_r[8]
					frndint
					fistp ri[8]
					cmp ri[8], 0
					jge next13
					mov ri[8], 0
					next13:				cmp ri[8], 255
					jle next14
					mov ri[8], 255
					next14 : fld simd_g[8]
					frndint
					fistp gi[8]
					cmp gi[8], 0
					jge next15
					mov gi[8], 0
					next15 : cmp gi[8], 255
					jle next16
					mov gi[8], 255
					next16 : fld simd_b[8]
					frndint
					fistp bi[8]
					cmp bi[8], 0
					jge next17
					mov bi[8], 0
					next17 : cmp bi[8], 255
					jle next18
					mov bi[8], 255
					next18 : mov eax, ri[8]
					shl eax, 8
					or eax, gi[8]
					shl eax, 8
					or eax, bi[8]
					mov[edi], eax
					inc dwFillRate
					next28 : add edi, 4
					add esi, 4
					dec ecx
					jz next25
					fld simd_z[12]
					frndint
					fistp zi[12]
					mov eax, zi[12]
					cmp eax, [esi]
					jge next29
					mov[esi], eax
					fld simd_r[12]
					frndint
					fistp ri[12]
					cmp ri[12], 0
					jge next19
					mov ri[12], 0
					next19:				cmp ri[12], 255
					jle next20
					mov ri[12], 255
					next20 : fld simd_g[12]
					frndint
					fistp gi[12]
					cmp gi[12], 0
					jge next21
					mov gi[12], 0
					next21 : cmp gi[12], 255
					jle next22
					mov gi[12], 255
					next22 : fld simd_b[12]
					frndint
					fistp bi[12]
					cmp bi[12], 0
					jge next23
					mov bi[12], 0
					next23 : cmp bi[12], 255
					jle next24
					mov bi[12], 255
					next24 : mov eax, ri[12]
					shl eax, 8
					or eax, gi[12]
					shl eax, 8
					or eax, bi[12]
					mov[edi], eax
					inc dwFillRate
					next29 : add edi, 4
					add esi, 4
					dec ecx
					jnz loop1
					next25 : nop
				}
				m_iFillRate += dwFillRate;
				m_iSkipRate += (dwSize - dwFillRate);
			}
			lpData += m_iWidth;
			lpZBufferData += m_iWidth;
		}

		oldxmin = xmin;
		oldxmax = xmax;

		xm += dxm;
		xb += dxb;
		zm += dzm;
		zb += dzb;
		reds += red1;
		greens += green1;
		blues += blue1;
		rede += red2;
		greene += green2;
		bluee += blue2;
	}
}

void Draw_Bottom_Triangle_Texture(_VERTEX3D top, _VERTEX3D middle, _VERTEX3D bottom, _TEXTUREINFO textureInfo)
{
	_POINT2D uvt = top.uv;
	_POINT2D uvm = middle.uv;
	_POINT2D uvb = bottom.uv;
	long minY = (long)middle.p.y;
	long maxY = (long)bottom.p.y;
	long bottomX = (long)bottom.p.x;

	if ((minY > m_iHeight - 1) || (maxY < 0))
	{
		return;
	}

	LPBYTE lpData = m_lpData;
	LPBYTE lpZBufferData = m_lpZBuffer;
	DWORD dwTotalOffset = 0;
	if (maxY <= m_iHeight - 1)
	{
		dwTotalOffset = (m_iHeight - 1 - maxY) * m_iWidth;
	}
	else
	{
		dwTotalOffset = 0;
	}
	lpData += dwTotalOffset;
	lpZBufferData += dwTotalOffset;

	float xm, xb;
	long x, xmin, xmax, oldxmin, oldxmax;
	float dxm = -(bottom.p.x - middle.p.x) / (bottom.p.y - middle.p.y);
	float dxb = -(bottom.p.x - top.p.x) / (bottom.p.y - top.p.y);
	float dzm = -(1.0f / bottom.p.z - 1.0f / middle.p.z) / (bottom.p.y - middle.p.y);
	float dzb = -(1.0f / bottom.p.z - 1.0f / top.p.z) / (bottom.p.y - top.p.y);
	x = bottomX;
	xm = bottom.p.x;
	xb = bottom.p.x;
	xmin = x;
	xmax = x;
	oldxmin = x;
	oldxmax = x;
	float zm = 1.0f / bottom.p.z;
	float zb = 1.0f / bottom.p.z;
	long midY = (long)middle.p.y;
	long midX = (long)(top.p.x + (middle.p.y - top.p.y) * dxb);
	float dum = -(float)((float)uvb.x / (float)bottom.p.z - (float)uvm.x / (float)middle.p.z) / (float)(bottom.p.y - middle.p.y);
	float dvm = -(float)((float)uvb.y / (float)bottom.p.z - (float)uvm.y / (float)middle.p.z) / (float)(bottom.p.y - middle.p.y);
	float dub = -(float)((float)uvb.x / (float)bottom.p.z - (float)uvt.x / (float)top.p.z) / (float)(bottom.p.y - top.p.y);
	float dvb = -(float)((float)uvb.y / (float)bottom.p.z - (float)uvt.y / (float)top.p.z) / (float)(bottom.p.y - top.p.y);
	float um = (float)uvb.x / (float)bottom.p.z;
	float vm = (float)uvb.y / (float)bottom.p.z;
	float ub = (float)uvb.x / (float)bottom.p.z;
	float vb = (float)uvb.y / (float)bottom.p.z;
	if (m_bFollowInvert == TRUE)
	{
		if (m_bInvert == TRUE)
		{
			float umt = dum;
			float uvt = dvm;
			dum = dub;
			dvm = dvb;
			dub = umt;
			dvb = uvt;
			float zmt = dzm;
			dzm = dzb;
			dzb = zmt;
			m_bInvert = FALSE;
		}
	}
	else if (midX < (long)middle.p.x)
	{
		float umt = dum;
		float uvt = dvm;
		dum = dub;
		dvm = dvb;
		dub = umt;
		dvb = uvt;
		float zmt = dzm;
		dzm = dzb;
		dzb = zmt;
		m_bInvert = FALSE;
	}

	for (long y1 = maxY; y1 >= minY; y1--)
	{
		xmin = (long)xm;
		xmax = (long)xb;
		if (xmin > xmax)
		{
			long temp = xmin;
			xmin = xmax;
			xmax = temp;
		}

		if (xmin > oldxmax)
		{
			xmin = oldxmax + 1;
		}
		else if (xmax < oldxmin)
		{
			xmax = oldxmin - 1;
		}

		if ((y1 >= 0) && (y1 <= m_iHeight - 1))
		{
			DWORD dwSize = xmax - xmin + 1;
			LPBYTE lpTempData = lpData;
			LPBYTE lpTempZBufferData = lpZBufferData;

			if ((xmin <= m_iWidth - 1) && (xmax >= 0))
			{
				float fSize = (float)dwSize;
				float du = (ub - um) / fSize;
				float dv = (vb - vm) / fSize;
				float dz = (zb - zm) / fSize;
				float u = um;
				float v = vm;
				float z = zm;

				if (xmin < 0)
				{
					dwSize += xmin;
					u -= (xmin * du);
					v -= (xmin * dv);
					z -= (xmin * dz);
				}
				else
				{
					lpTempData += xmin;
					lpTempZBufferData += xmin;
				}

				if (xmax > m_iWidth - 1)
				{
					dwSize -= (xmax - m_iWidth + 1);
				}

				DWORD dwTextureSize = textureInfo.dwSize - 1;
				LPBYTE lpTextureData = textureInfo.lpTexture;
				DWORD dwShift = (dwTextureSize + 1) << 2;
				/*				for (int x=0; x<(int)dwSize; x++)
								{
									float z_inv = 1.0f / z;
									if (z_inv < (*lpTempZBufferData))
									{
										*lpTempZBufferData = z_inv;
										long ui = max(0, min(dwTextureSize, (long)(u * z_inv)));
										long vi = max(0, min(dwTextureSize, (long)(v * z_inv)));
										*lpTempData++ = lpTextureData[ui + vi*textureInfo.dwSize];
										u += du;
										v += dv;
										z += dz;
									}
								}*/

				float u_du1 = u + du;
				float u_du2 = u_du1 + du;
				float u_du3 = u_du2 + du;
				float u_du4 = 4 * du;
				float v_dv1 = v + dv;
				float v_dv2 = v_dv1 + dv;
				float v_dv3 = v_dv2 + dv;
				float v_dv4 = 4 * dv;
				float z_dz1 = z + dz;
				float z_dz2 = z_dz1 + dz;
				float z_dz3 = z_dz2 + dz;
				float z_dz4 = 4 * dz;
				__m128 simd_u = { u, u_du1, u_du2, u_du3 };
				__m128 simd_v = { v, v_dv1, v_dv2, v_dv3 };
				__m128 simd_z = { z, z_dz1, z_dz2, z_dz3 };
				__m128 simd_du = { u_du4, u_du4, u_du4, u_du4 };
				__m128 simd_dv = { v_dv4, v_dv4, v_dv4, v_dv4 };
				__m128 simd_dz = { z_dz4, z_dz4, z_dz4, z_dz4 };
				long ui[4] = { 0, 0, 0, 0 };
				long vi[4] = { 0, 0, 0, 0 };
				long zi[4] = { 0, 0, 0, 0 };

				long dwFillRate = 0;
				_asm {
					mov edi, lpTempData
					mov esi, lpTempZBufferData
					mov ecx, dwSize
					movaps xmm0, simd_u
					movaps xmm1, simd_v
					movaps xmm2, simd_z
					loop1 : movaps xmm3, xmm0
					movaps xmm4, xmm1
					rcpps xmm5, xmm2
					mulps xmm3, xmm5
					mulps xmm4, xmm5
					movaps simd_u, xmm3
					movaps simd_v, xmm4
					movaps simd_z, xmm5
					addps xmm0, simd_du
					addps xmm1, simd_dv
					addps xmm2, simd_dz
					fld simd_z[0]
					frndint
					fistp zi[0]
					mov eax, zi[0]
					cmp eax, [esi]
					jge next18
					mov[esi], eax
					mov eax, dwTextureSize
					fld simd_u[0]
					frndint
					fistp ui[0]
					cmp ui[0], 0
					jge next1
					mov ui[0], 0
					next1:				cmp ui[0], eax
					jle next2
					mov ui[0], eax
					next2 : fld simd_v[0]
					frndint
					fistp vi[0]
					cmp vi[0], 0
					jge next3
					mov vi[0], 0
					next3 : cmp vi[0], eax
					jle next4
					mov vi[0], eax
					next4 : mov eax, vi[0]
					mul dwShift
					shr eax, 2
					add eax, ui[0]
					shl eax, 2
					push esi
					mov esi, lpTextureData
					add esi, eax
					mov eax, [esi]
					mov[edi], eax
					inc dwFillRate
					pop esi
					next18 : add edi, 4
					add esi, 4
					dec ecx
					jz next17
					fld simd_z[4]
					frndint
					fistp zi[4]
					mov eax, zi[4]
					cmp eax, [esi]
					jge next19
					mov[esi], eax
					mov eax, dwTextureSize
					fld simd_u[4]
					frndint
					fistp ui[4]
					cmp ui[4], 0
					jge next5
					mov ui[4], 0
					next5:				cmp ui[4], eax
					jle next6
					mov ui[4], eax
					next6 : fld simd_v[4]
					frndint
					fistp vi[4]
					cmp vi[4], 0
					jge next7
					mov vi[4], 0
					next7 : cmp vi[4], eax
					jle next8
					mov vi[4], eax
					next8 : mov eax, vi[4]
					mul dwShift
					shr eax, 2
					add eax, ui[4]
					shl eax, 2
					push esi
					mov esi, lpTextureData
					add esi, eax
					mov eax, [esi]
					mov[edi], eax
					inc dwFillRate
					pop esi
					next19 : add edi, 4
					add esi, 4
					dec ecx
					jz next17
					fld simd_z[8]
					frndint
					fistp zi[8]
					mov eax, zi[8]
					cmp eax, [esi]
					jge next20
					mov[esi], eax
					mov eax, dwTextureSize
					fld simd_u[8]
					frndint
					fistp ui[8]
					cmp ui[8], 0
					jge next9
					mov ui[8], 0
					next9:				cmp ui[8], eax
					jle next10
					mov ui[8], eax
					next10 : fld simd_v[8]
					frndint
					fistp vi[8]
					cmp vi[8], 0
					jge next11
					mov vi[8], 0
					next11 : cmp vi[8], eax
					jle next12
					mov vi[8], eax
					next12 : mov eax, vi[8]
					mul dwShift
					shr eax, 2
					add eax, ui[8]
					shl eax, 2
					push esi
					mov esi, lpTextureData
					add esi, eax
					mov eax, [esi]
					mov[edi], eax
					inc dwFillRate
					pop esi
					next20 : add edi, 4
					add esi, 4
					dec ecx
					jz next17
					fld simd_z[12]
					frndint
					fistp zi[12]
					mov eax, zi[12]
					cmp eax, [esi]
					jge next21
					mov[esi], eax
					mov eax, dwTextureSize
					fld simd_u[12]
					frndint
					fistp ui[12]
					cmp ui[12], 0
					jge next13
					mov ui[12], 0
					next13:				cmp ui[12], eax
					jle next14
					mov ui[12], eax
					next14 : fld simd_v[12]
					frndint
					fistp vi[12]
					cmp vi[12], 0
					jge next15
					mov vi[12], 0
					next15 : cmp vi[12], eax
					jle next16
					mov vi[12], eax
					next16 : mov eax, vi[12]
					mul dwShift
					shr eax, 2
					add eax, ui[12]
					shl eax, 2
					push esi
					mov esi, lpTextureData
					add esi, eax
					mov eax, [esi]
					mov[edi], eax
					inc dwFillRate
					pop esi
					next21 : add edi, 4
					add esi, 4
					dec ecx
					jnz loop1
					next17 : nop
				}
				m_iFillRate += dwFillRate;
				m_iSkipRate += (dwSize - dwFillRate);
			}
			lpData += m_iWidth;
			lpZBufferData += m_iWidth;
		}

		oldxmin = xmin;
		oldxmax = xmax;

		xm += dxm;
		xb += dxb;
		zm += dzm;
		zb += dzb;
		um += dum;
		vm += dvm;
		ub += dub;
		vb += dvb;
	}
}

void Draw_Bottom_Triangle_Texture_Color(_VERTEX3D top, _VERTEX3D middle, _VERTEX3D bottom, _TEXTUREINFO textureInfo)
{
	float f_red = (float)_GetRValue(top.color) / 255.0f;
	float f_green = (float)_GetGValue(top.color) / 255.0f;
	float f_blue = (float)_GetBValue(top.color) / 255.0f;
	_POINT2D uvt = top.uv;
	_POINT2D uvm = middle.uv;
	_POINT2D uvb = bottom.uv;
	long minY = (long)middle.p.y;
	long maxY = (long)bottom.p.y;
	long bottomX = (long)bottom.p.x;

	if ((minY > m_iHeight - 1) || (maxY < 0))
	{
		return;
	}

	LPBYTE lpData = m_lpData;
	LPBYTE lpZBufferData = m_lpZBuffer;
	DWORD dwTotalOffset = 0;
	if (maxY <= m_iHeight - 1)
	{
		dwTotalOffset = (m_iHeight - 1 - maxY) * m_iWidth;
	}
	else
	{
		dwTotalOffset = 0;
	}
	lpData += dwTotalOffset;
	lpZBufferData += dwTotalOffset;

	float xm, xb;
	long x, xmin, xmax, oldxmin, oldxmax;
	float dxm = -(bottom.p.x - middle.p.x) / (bottom.p.y - middle.p.y);
	float dxb = -(bottom.p.x - top.p.x) / (bottom.p.y - top.p.y);
	float dzm = -(1.0f / bottom.p.z - 1.0f / middle.p.z) / (bottom.p.y - middle.p.y);
	float dzb = -(1.0f / bottom.p.z - 1.0f / top.p.z) / (bottom.p.y - top.p.y);
	x = bottomX;
	xm = bottom.p.x;
	xb = bottom.p.x;
	xmin = x;
	xmax = x;
	oldxmin = x;
	oldxmax = x;
	float zm = 1.0f / bottom.p.z;
	float zb = 1.0f / bottom.p.z;
	long midY = (long)middle.p.y;
	long midX = (long)(top.p.x + (middle.p.y - top.p.y) * dxb);
	float dum = -(float)((float)uvb.x / (float)bottom.p.z - (float)uvm.x / (float)middle.p.z) / (float)(bottom.p.y - middle.p.y);
	float dvm = -(float)((float)uvb.y / (float)bottom.p.z - (float)uvm.y / (float)middle.p.z) / (float)(bottom.p.y - middle.p.y);
	float dub = -(float)((float)uvb.x / (float)bottom.p.z - (float)uvt.x / (float)top.p.z) / (float)(bottom.p.y - top.p.y);
	float dvb = -(float)((float)uvb.y / (float)bottom.p.z - (float)uvt.y / (float)top.p.z) / (float)(bottom.p.y - top.p.y);
	float um = (float)uvb.x / (float)bottom.p.z;
	float vm = (float)uvb.y / (float)bottom.p.z;
	float ub = (float)uvb.x / (float)bottom.p.z;
	float vb = (float)uvb.y / (float)bottom.p.z;
	if (m_bFollowInvert == TRUE)
	{
		if (m_bInvert == TRUE)
		{
			float umt = dum;
			float uvt = dvm;
			dum = dub;
			dvm = dvb;
			dub = umt;
			dvb = uvt;
			float zmt = dzm;
			dzm = dzb;
			dzb = zmt;
			m_bInvert = FALSE;
		}
	}
	else if (midX < (long)middle.p.x)
	{
		float umt = dum;
		float uvt = dvm;
		dum = dub;
		dvm = dvb;
		dub = umt;
		dvb = uvt;
		float zmt = dzm;
		dzm = dzb;
		dzb = zmt;
		m_bInvert = FALSE;
	}

	for (long y1 = maxY; y1 >= minY; y1--)
	{
		xmin = (long)xm;
		xmax = (long)xb;
		if (xmin > xmax)
		{
			long temp = xmin;
			xmin = xmax;
			xmax = temp;
		}

		if (xmin > oldxmax)
		{
			xmin = oldxmax + 1;
		}
		else if (xmax < oldxmin)
		{
			xmax = oldxmin - 1;
		}

		if ((y1 >= 0) && (y1 <= m_iHeight - 1))
		{
			DWORD dwSize = xmax - xmin + 1;
			LPBYTE lpTempData = lpData;
			LPBYTE lpTempZBufferData = lpZBufferData;

			if ((xmin <= m_iWidth - 1) && (xmax >= 0))
			{
				float fSize = (float)dwSize;
				float du = (ub - um) / fSize;
				float dv = (vb - vm) / fSize;
				float dz = (zb - zm) / fSize;
				float u = um;
				float v = vm;
				float z = zm;

				if (xmin < 0)
				{
					dwSize += xmin;
					u -= (xmin * du);
					v -= (xmin * dv);
					z -= (xmin * dz);
				}
				else
				{
					lpTempData += xmin;
					lpTempZBufferData += xmin;
				}

				if (xmax > m_iWidth - 1)
				{
					dwSize -= (xmax - m_iWidth + 1);
				}

				DWORD dwTextureSize = textureInfo.dwSize - 1;
				LPBYTE lpTextureData = textureInfo.lpTexture;
				DWORD dwShift = (dwTextureSize + 1) << 2;
				/*				for (int x=0; x<(int)dwSize; x++)
								{
									float z_inv = 1.0f / z;
									if (z_inv < (*lpTempZBufferData))
									{
										*lpTempZBufferData = z_inv;
										long ui = max(0, min((long)textureInfo.dwSize-1, (long)(u * z_inv)));
										long vi = max(0, min((long)textureInfo.dwSize-1, (long)(v * z_inv)));
										_PIXEL srcPixel = lpTextureData[ui + vi*textureInfo.dwSize];
										BYTE red = (BYTE)((float)_GetRValue(srcPixel) * f_red);
										BYTE green = (BYTE)((float)_GetGValue(srcPixel) * f_green);
										BYTE blue = (BYTE)((float)_GetBValue(srcPixel) * f_blue);
										*lpTempData++ = _RGB(red, green, blue);
										u += du;
										v += dv;
										z += dz;
									}
								}*/

				float u_du1 = u + du;
				float u_du2 = u_du1 + du;
				float u_du3 = u_du2 + du;
				float u_du4 = 4 * du;
				float v_dv1 = v + dv;
				float v_dv2 = v_dv1 + dv;
				float v_dv3 = v_dv2 + dv;
				float v_dv4 = 4 * dv;
				float z_dz1 = z + dz;
				float z_dz2 = z_dz1 + dz;
				float z_dz3 = z_dz2 + dz;
				float z_dz4 = 4 * dz;
				__m128 simd_u = { u, u_du1, u_du2, u_du3 };
				__m128 simd_v = { v, v_dv1, v_dv2, v_dv3 };
				__m128 simd_z = { z, z_dz1, z_dz2, z_dz3 };
				__m128 simd_du = { u_du4, u_du4, u_du4, u_du4 };
				__m128 simd_dv = { v_dv4, v_dv4, v_dv4, v_dv4 };
				__m128 simd_dz = { z_dz4, z_dz4, z_dz4, z_dz4 };
				long ui[4] = { 0, 0, 0, 0 };
				long vi[4] = { 0, 0, 0, 0 };
				long zi[4] = { 0, 0, 0, 0 };

				long dwFillRate = 0;
				long red, green, blue;
				_asm {
					mov edi, lpTempData
					mov esi, lpTempZBufferData
					mov ecx, dwSize
					movaps xmm0, simd_u
					movaps xmm1, simd_v
					movaps xmm2, simd_z
					loop1 : movaps xmm3, xmm0
					movaps xmm4, xmm1
					rcpps xmm5, xmm2
					mulps xmm3, xmm5
					mulps xmm4, xmm5
					movaps simd_u, xmm3
					movaps simd_v, xmm4
					movaps simd_z, xmm5
					addps xmm0, simd_du
					addps xmm1, simd_dv
					addps xmm2, simd_dz
					fld simd_z[0]
					frndint
					fistp zi[0]
					mov eax, zi[0]
					cmp eax, [esi]
					jge next18
					mov[esi], eax
					mov eax, dwTextureSize
					fld simd_u[0]
					frndint
					fistp ui[0]
					cmp ui[0], 0
					jge next1
					mov ui[0], 0
					next1:				cmp ui[0], eax
					jle next2
					mov ui[0], eax
					next2 : fld simd_v[0]
					frndint
					fistp vi[0]
					cmp vi[0], 0
					jge next3
					mov vi[0], 0
					next3 : cmp vi[0], eax
					jle next4
					mov vi[0], eax
					next4 : mov eax, vi[0]
					mul dwShift
					shr eax, 2
					add eax, ui[0]
					shl eax, 2
					push esi
					mov esi, lpTextureData
					add esi, eax
					mov eax, [esi]
					and eax, 0x000000FF
					mov blue, eax
					fild blue
					fmul f_blue
					fistp blue
					mov eax, [esi]
					and eax, 0x0000FF00
					shr eax, 8
					mov green, eax
					fild green
					fmul f_green
					fistp green
					mov eax, [esi]
					and eax, 0x00FF0000
					shr eax, 16
					mov red, eax
					fild red
					fmul f_red
					fistp red
					mov eax, red
					shl eax, 8
					or eax, green
					shl eax, 8
					or eax, blue
					mov[edi], eax
					inc dwFillRate
					pop esi
					next18 : add edi, 4
					add esi, 4
					dec ecx
					jz next17
					fld simd_z[4]
					frndint
					fistp zi[4]
					mov eax, zi[4]
					cmp eax, [esi]
					jge next19
					mov[esi], eax
					mov eax, dwTextureSize
					fld simd_u[4]
					frndint
					fistp ui[4]
					cmp ui[4], 0
					jge next5
					mov ui[4], 0
					next5:				cmp ui[4], eax
					jle next6
					mov ui[4], eax
					next6 : fld simd_v[4]
					frndint
					fistp vi[4]
					cmp vi[4], 0
					jge next7
					mov vi[4], 0
					next7 : cmp vi[4], eax
					jle next8
					mov vi[4], eax
					next8 : mov eax, vi[4]
					mul dwShift
					shr eax, 2
					add eax, ui[4]
					shl eax, 2
					push esi
					mov esi, lpTextureData
					add esi, eax
					mov eax, [esi]
					and eax, 0x000000FF
					mov blue, eax
					fild blue
					fmul f_blue
					fistp blue
					mov eax, [esi]
					and eax, 0x0000FF00
					shr eax, 8
					mov green, eax
					fild green
					fmul f_green
					fistp green
					mov eax, [esi]
					and eax, 0x00FF0000
					shr eax, 16
					mov red, eax
					fild red
					fmul f_red
					fistp red
					mov eax, red
					shl eax, 8
					or eax, green
					shl eax, 8
					or eax, blue
					mov[edi], eax
					inc dwFillRate
					pop esi
					next19 : add edi, 4
					add esi, 4
					dec ecx
					jz next17
					fld simd_z[8]
					frndint
					fistp zi[8]
					mov eax, zi[8]
					cmp eax, [esi]
					jge next20
					mov[esi], eax
					mov eax, dwTextureSize
					fld simd_u[8]
					frndint
					fistp ui[8]
					cmp ui[8], 0
					jge next9
					mov ui[8], 0
					next9:				cmp ui[8], eax
					jle next10
					mov ui[8], eax
					next10 : fld simd_v[8]
					frndint
					fistp vi[8]
					cmp vi[8], 0
					jge next11
					mov vi[8], 0
					next11 : cmp vi[8], eax
					jle next12
					mov vi[8], eax
					next12 : mov eax, vi[8]
					mul dwShift
					shr eax, 2
					add eax, ui[8]
					shl eax, 2
					push esi
					mov esi, lpTextureData
					add esi, eax
					mov eax, [esi]
					and eax, 0x000000FF
					mov blue, eax
					fild blue
					fmul f_blue
					fistp blue
					mov eax, [esi]
					and eax, 0x0000FF00
					shr eax, 8
					mov green, eax
					fild green
					fmul f_green
					fistp green
					mov eax, [esi]
					and eax, 0x00FF0000
					shr eax, 16
					mov red, eax
					fild red
					fmul f_red
					fistp red
					mov eax, red
					shl eax, 8
					or eax, green
					shl eax, 8
					or eax, blue
					mov[edi], eax
					inc dwFillRate
					pop esi
					next20 : add edi, 4
					add esi, 4
					dec ecx
					jz next17
					fld simd_z[12]
					frndint
					fistp zi[12]
					mov eax, zi[12]
					cmp eax, [esi]
					jge next21
					mov[esi], eax
					mov eax, dwTextureSize
					fld simd_u[12]
					frndint
					fistp ui[12]
					cmp ui[12], 0
					jge next13
					mov ui[12], 0
					next13:				cmp ui[12], eax
					jle next14
					mov ui[12], eax
					next14 : fld simd_v[12]
					frndint
					fistp vi[12]
					cmp vi[12], 0
					jge next15
					mov vi[12], 0
					next15 : cmp vi[12], eax
					jle next16
					mov vi[12], eax
					next16 : mov eax, vi[12]
					mul dwShift
					shr eax, 2
					add eax, ui[12]
					shl eax, 2
					push esi
					mov esi, lpTextureData
					add esi, eax
					mov eax, [esi]
					and eax, 0x000000FF
					mov blue, eax
					fild blue
					fmul f_blue
					fistp blue
					mov eax, [esi]
					and eax, 0x0000FF00
					shr eax, 8
					mov green, eax
					fild green
					fmul f_green
					fistp green
					mov eax, [esi]
					and eax, 0x00FF0000
					shr eax, 16
					mov red, eax
					fild red
					fmul f_red
					fistp red
					mov eax, red
					shl eax, 8
					or eax, green
					shl eax, 8
					or eax, blue
					mov[edi], eax
					inc dwFillRate
					pop esi
					next21 : add edi, 4
					add esi, 4
					dec ecx
					jnz loop1
					next17 : nop
				}
				m_iFillRate += dwFillRate;
				m_iSkipRate += (dwSize - dwFillRate);
			}
			lpData += m_iWidth;
			lpZBufferData += m_iWidth;
		}

		oldxmin = xmin;
		oldxmax = xmax;

		xm += dxm;
		xb += dxb;
		zm += dzm;
		zb += dzb;
		um += dum;
		vm += dvm;
		ub += dub;
		vb += dvb;
	}
}

void Draw_Bottom_Triangle_Texture_Gouraud(_VERTEX3D top, _VERTEX3D middle, _VERTEX3D bottom, _TEXTUREINFO textureInfo)
{
	_POINT2D uvt = top.uv;
	_POINT2D uvm = middle.uv;
	_POINT2D uvb = bottom.uv;
	long minY = (long)middle.p.y;
	long maxY = (long)bottom.p.y;
	long bottomX = (long)bottom.p.x;
	_PIXEL c1 = top.color;
	_PIXEL c2 = middle.color;
	_PIXEL c3 = bottom.color;
	float tred = (float)_GetRValue(c1) / 255.0f;
	float tgreen = (float)_GetGValue(c1) / 255.0f;
	float tblue = (float)_GetBValue(c1) / 255.0f;
	float mred = (float)_GetRValue(c2) / 255.0f;
	float mgreen = (float)_GetGValue(c2) / 255.0f;
	float mblue = (float)_GetBValue(c2) / 255.0f;
	float bred = (float)_GetRValue(c3) / 255.0f;
	float bgreen = (float)_GetGValue(c3) / 255.0f;
	float bblue = (float)_GetBValue(c3) / 255.0f;

	if ((minY > m_iHeight - 1) || (maxY < 0))
	{
		return;
	}

	LPBYTE lpData = m_lpData;
	LPBYTE lpZBufferData = m_lpZBuffer;
	DWORD dwTotalOffset = 0;
	if (maxY <= m_iHeight - 1)
	{
		dwTotalOffset = (m_iHeight - 1 - maxY) * m_iWidth;
	}
	else
	{
		dwTotalOffset = 0;
	}
	lpData += dwTotalOffset;
	lpZBufferData += dwTotalOffset;

	float cxm, cxb, red1, green1, blue1, red2, green2, blue2;
	float xm, xb, reds, greens, blues, rede, greene, bluee;
	long x, xmin, xmax, oldxmin, oldxmax;
	float dxm = -(bottom.p.x - middle.p.x) / (bottom.p.y - middle.p.y);
	float dxb = -(bottom.p.x - top.p.x) / (bottom.p.y - top.p.y);
	float dzm = -(1.0f / bottom.p.z - 1.0f / middle.p.z) / (bottom.p.y - middle.p.y);
	float dzb = -(1.0f / bottom.p.z - 1.0f / top.p.z) / (bottom.p.y - top.p.y);
	x = bottomX;
	xm = bottom.p.x;
	xb = bottom.p.x;
	xmin = x;
	xmax = x;
	oldxmin = x;
	oldxmax = x;
	float zm = 1.0f / bottom.p.z;
	float zb = 1.0f / bottom.p.z;
	long midY = (long)middle.p.y;
	long midX = (long)(top.p.x + (middle.p.y - top.p.y) * dxb);
	float dum = -(float)((float)uvb.x / (float)bottom.p.z - (float)uvm.x / (float)middle.p.z) / (float)(bottom.p.y - middle.p.y);
	float dvm = -(float)((float)uvb.y / (float)bottom.p.z - (float)uvm.y / (float)middle.p.z) / (float)(bottom.p.y - middle.p.y);
	float dub = -(float)((float)uvb.x / (float)bottom.p.z - (float)uvt.x / (float)top.p.z) / (float)(bottom.p.y - top.p.y);
	float dvb = -(float)((float)uvb.y / (float)bottom.p.z - (float)uvt.y / (float)top.p.z) / (float)(bottom.p.y - top.p.y);
	float um = (float)uvb.x / (float)bottom.p.z;
	float vm = (float)uvb.y / (float)bottom.p.z;
	float ub = (float)uvb.x / (float)bottom.p.z;
	float vb = (float)uvb.y / (float)bottom.p.z;
	cxm = bottom.p.y - middle.p.y;
	cxb = bottom.p.y - top.p.y;
	if (cxm == 0.0f)
	{
		red1 = green1 = blue1 = 0.0f;
	}
	else
	{
		red1 = (mred / middle.p.z - bred / bottom.p.z) / cxm;
		green1 = (mgreen / middle.p.z - bgreen / bottom.p.z) / cxm;
		blue1 = (mblue / middle.p.z - bblue / bottom.p.z) / cxm;
	}
	if (cxb == 0.0f)
	{
		red2 = green2 = blue2 = 0.0f;
	}
	else
	{
		red2 = (tred / top.p.z - bred / bottom.p.z) / cxb;
		green2 = (tgreen / top.p.z - bgreen / bottom.p.z) / cxb;
		blue2 = (tblue / top.p.z - bblue / bottom.p.z) / cxb;
	}
	if (m_bFollowInvert == TRUE)
	{
		if (m_bInvert == TRUE)
		{
			float umt = dum;
			float uvt = dvm;
			dum = dub;
			dvm = dvb;
			dub = umt;
			dvb = uvt;
			float rt = red2;
			float gt = green2;
			float bt = blue2;
			red2 = red1;
			green2 = green1;
			blue2 = blue1;
			red1 = rt;
			green1 = gt;
			blue1 = bt;
			float zmt = dzm;
			dzm = dzb;
			dzb = zmt;
			m_bInvert = FALSE;
		}
	}
	else if (midX < (long)middle.p.x)
	{
		float umt = dum;
		float uvt = dvm;
		dum = dub;
		dvm = dvb;
		dub = umt;
		dvb = uvt;
		float rt = red2;
		float gt = green2;
		float bt = blue2;
		red2 = red1;
		green2 = green1;
		blue2 = blue1;
		red1 = rt;
		green1 = gt;
		blue1 = bt;
		float zmt = dzm;
		dzm = dzb;
		dzb = zmt;
		m_bInvert = FALSE;
	}
	reds = bred / bottom.p.z;
	greens = bgreen / bottom.p.z;
	blues = bblue / bottom.p.z;
	rede = reds;
	greene = greens;
	bluee = blues;

	for (long y1 = maxY; y1 >= minY; y1--)
	{
		xmin = (long)xm;
		xmax = (long)xb;
		if (xmin > xmax)
		{
			long temp = xmin;
			xmin = xmax;
			xmax = temp;
		}

		if (xmin > oldxmax)
		{
			xmin = oldxmax + 1;
		}
		else if (xmax < oldxmin)
		{
			xmax = oldxmin - 1;
		}

		if ((y1 >= 0) && (y1 <= m_iHeight - 1))
		{
			DWORD dwSize = xmax - xmin + 1;
			LPBYTE lpTempData = lpData;
			LPBYTE lpTempZBufferData = lpZBufferData;

			if ((xmin <= m_iWidth - 1) && (xmax >= 0))
			{
				float fSize = (float)dwSize;
				float du = (ub - um) / fSize;
				float dv = (vb - vm) / fSize;
				float dz = (zb - zm) / fSize;
				float dr = (rede - reds) / fSize;
				float dg = (greene - greens) / fSize;
				float db = (bluee - blues) / fSize;
				float u = um;
				float v = vm;
				float z = zm;
				float r = reds;
				float g = greens;
				float b = blues;

				if (xmin < 0)
				{
					dwSize += xmin;
					u -= (xmin * du);
					v -= (xmin * dv);
					z -= (xmin * dz);
					r -= (xmin * dr);
					g -= (xmin * dg);
					b -= (xmin * db);
				}
				else
				{
					lpTempData += xmin;
					lpTempZBufferData += xmin;
				}

				if (xmax > m_iWidth - 1)
				{
					dwSize -= (xmax - m_iWidth + 1);
				}

				DWORD dwTextureSize = textureInfo.dwSize - 1;
				LPBYTE lpTextureData = textureInfo.lpTexture;
				DWORD dwShift = (dwTextureSize + 1) << 2;
				/*				for (int x=0; x<(int)dwSize; x++)
								{
									float z_inv = 1.0f / z;
									if (z_inv < (*lpTempZBufferData))
									{
										*lpTempZBufferData = z_inv;
										long ui = max(0, min((long)textureInfo.dwSize-1, (long)(u * z_inv)));
										long vi = max(0, min((long)textureInfo.dwSize-1, (long)(v * z_inv)));
										float f_red = r * z_inv;
										float f_green = g * z_inv;
										float f_blue = b * z_inv;
										_PIXEL srcPixel = lpTextureData[ui + vi*textureInfo.dwSize];
										BYTE red = (BYTE)((float)_GetRValue(srcPixel) * f_red);
										BYTE green = (BYTE)((float)_GetGValue(srcPixel) * f_green);
										BYTE blue = (BYTE)((float)_GetBValue(srcPixel) * f_blue);
										*lpTempData++ = _RGB(red, green, blue);
										u += du;
										v += dv;
										z += dz;
										r += dr;
										g += dg;
										b += db;
									}
								}*/

				float u_du1 = u + du;
				float u_du2 = u_du1 + du;
				float u_du3 = u_du2 + du;
				float u_du4 = 4 * du;
				float v_dv1 = v + dv;
				float v_dv2 = v_dv1 + dv;
				float v_dv3 = v_dv2 + dv;
				float v_dv4 = 4 * dv;
				float z_dz1 = z + dz;
				float z_dz2 = z_dz1 + dz;
				float z_dz3 = z_dz2 + dz;
				float z_dz4 = 4 * dz;
				float r_dr1 = r + dr;
				float r_dr2 = r_dr1 + dr;
				float r_dr3 = r_dr2 + dr;
				float r_dr4 = 4 * dr;
				float g_dg1 = g + dg;
				float g_dg2 = g_dg1 + dg;
				float g_dg3 = g_dg2 + dg;
				float g_dg4 = 4 * dg;
				float b_db1 = b + db;
				float b_db2 = b_db1 + db;
				float b_db3 = b_db2 + db;
				float b_db4 = 4 * db;
				__m128 simd_u = { u, u_du1, u_du2, u_du3 };
				__m128 simd_v = { v, v_dv1, v_dv2, v_dv3 };
				__m128 simd_z = { z, z_dz1, z_dz2, z_dz3 };
				__m128 simd_r = { r, r_dr1, r_dr2, r_dr3 };
				__m128 simd_g = { g, g_dg1, g_dg2, g_dg3 };
				__m128 simd_b = { b, b_db1, b_db2, b_db3 };
				__m128 simd_tu = { u, u_du1, u_du2, u_du3 };
				__m128 simd_tv = { v, v_dv1, v_dv2, v_dv3 };
				__m128 simd_tz = { z, z_dz1, z_dz2, z_dz3 };
				__m128 simd_tr = { r, r_dr1, r_dr2, r_dr3 };
				__m128 simd_tg = { g, g_dg1, g_dg2, g_dg3 };
				__m128 simd_tb = { b, b_db1, b_db2, b_db3 };
				__m128 simd_du = { u_du4, u_du4, u_du4, u_du4 };
				__m128 simd_dv = { v_dv4, v_dv4, v_dv4, v_dv4 };
				__m128 simd_dz = { z_dz4, z_dz4, z_dz4, z_dz4 };
				__m128 simd_dr = { r_dr4, r_dr4, r_dr4, r_dr4 };
				__m128 simd_dg = { g_dg4, g_dg4, g_dg4, g_dg4 };
				__m128 simd_db = { b_db4, b_db4, b_db4, b_db4 };
				long ui[4] = { 0, 0, 0, 0 };
				long vi[4] = { 0, 0, 0, 0 };
				long zi[4] = { 0, 0, 0, 0 };

				long dwFillRate = 0;
				long red, green, blue;
				_asm {
					mov edi, lpTempData
					mov esi, lpTempZBufferData
					mov ecx, dwSize
					movaps xmm0, simd_u
					movaps xmm1, simd_v
					movaps xmm2, simd_z
					movaps xmm3, simd_r
					movaps xmm4, simd_g
					movaps xmm5, simd_b
					loop1 : rcpps xmm6, xmm2
					mulps xmm0, xmm6
					mulps xmm1, xmm6
					mulps xmm3, xmm6
					mulps xmm4, xmm6
					mulps xmm5, xmm6
					movaps simd_u, xmm0
					movaps simd_v, xmm1
					movaps simd_z, xmm6
					movaps simd_r, xmm3
					movaps simd_g, xmm4
					movaps simd_b, xmm5
					movaps xmm0, simd_tu
					movaps xmm1, simd_tv
					movaps xmm2, simd_tz
					movaps xmm3, simd_tr
					movaps xmm4, simd_tg
					movaps xmm5, simd_tb
					addps xmm0, simd_du
					addps xmm1, simd_dv
					addps xmm2, simd_dz
					addps xmm3, simd_dr
					addps xmm4, simd_dg
					addps xmm5, simd_db
					movaps simd_tu, xmm0
					movaps simd_tv, xmm1
					movaps simd_tz, xmm2
					movaps simd_tr, xmm3
					movaps simd_tg, xmm4
					movaps simd_tb, xmm5
					fld simd_z[0]
					frndint
					fistp zi[0]
					mov eax, zi[0]
					cmp eax, [esi]
					jge next18
					mov[esi], eax
					mov eax, dwTextureSize
					fld simd_u[0]
					frndint
					fistp ui[0]
					cmp ui[0], 0
					jge next1
					mov ui[0], 0
					next1:				cmp ui[0], eax
					jle next2
					mov ui[0], eax
					next2 : fld simd_v[0]
					frndint
					fistp vi[0]
					cmp vi[0], 0
					jge next3
					mov vi[0], 0
					next3 : cmp vi[0], eax
					jle next4
					mov vi[0], eax
					next4 : mov eax, vi[0]
					mul dwShift
					shr eax, 2
					add eax, ui[0]
					shl eax, 2
					push esi
					mov esi, lpTextureData
					add esi, eax
					mov eax, [esi]
					and eax, 0x000000FF
					mov blue, eax
					fild blue
					fmul simd_b[0]
					fistp blue
					cmp blue, 0
					jge next22
					mov blue, 0
					next22:				cmp blue, 255
					jle next23
					mov blue, 255
					next23 : mov eax, [esi]
					and eax, 0x0000FF00
					shr eax, 8
					mov green, eax
					fild green
					fmul simd_g[0]
					fistp green
					cmp green, 0
					jge next24
					mov green, 0
					next24:				cmp green, 255
					jle next25
					mov green, 255
					next25 : mov eax, [esi]
					and eax, 0x00FF0000
					shr eax, 16
					mov red, eax
					fild red
					fmul simd_r[0]
					fistp red
					cmp red, 0
					jge next26
					mov red, 0
					next26:				cmp red, 255
					jle next27
					mov red, 255
					next27 : mov eax, red
					shl eax, 8
					or eax, green
					shl eax, 8
					or eax, blue
					mov[edi], eax
					inc dwFillRate
					pop esi
					next18 : add edi, 4
					add esi, 4
					dec ecx
					jz next17
					fld simd_z[4]
					frndint
					fistp zi[4]
					mov eax, zi[4]
					cmp eax, [esi]
					jge next19
					mov[esi], eax
					mov eax, dwTextureSize
					fld simd_u[4]
					frndint
					fistp ui[4]
					cmp ui[4], 0
					jge next5
					mov ui[4], 0
					next5:				cmp ui[4], eax
					jle next6
					mov ui[4], eax
					next6 : fld simd_v[4]
					frndint
					fistp vi[4]
					cmp vi[4], 0
					jge next7
					mov vi[4], 0
					next7 : cmp vi[4], eax
					jle next8
					mov vi[4], eax
					next8 : mov eax, vi[4]
					mul dwShift
					shr eax, 2
					add eax, ui[4]
					shl eax, 2
					push esi
					mov esi, lpTextureData
					add esi, eax
					mov eax, [esi]
					and eax, 0x000000FF
					mov blue, eax
					fild blue
					fmul simd_b[4]
					fistp blue
					cmp blue, 0
					jge next28
					mov blue, 0
					next28:				cmp blue, 255
					jle next29
					mov blue, 255
					next29 : mov eax, [esi]
					and eax, 0x0000FF00
					shr eax, 8
					mov green, eax
					fild green
					fmul simd_g[4]
					fistp green
					cmp green, 0
					jge next30
					mov green, 0
					next30:				cmp green, 255
					jle next31
					mov green, 255
					next31 : mov eax, [esi]
					and eax, 0x00FF0000
					shr eax, 16
					mov red, eax
					fild red
					fmul simd_r[4]
					fistp red
					cmp red, 0
					jge next32
					mov red, 0
					next32:				cmp red, 255
					jle next33
					mov red, 255
					next33 : mov eax, red
					shl eax, 8
					or eax, green
					shl eax, 8
					or eax, blue
					mov[edi], eax
					inc dwFillRate
					pop esi
					next19 : add edi, 4
					add esi, 4
					dec ecx
					jz next17
					fld simd_z[8]
					frndint
					fistp zi[8]
					mov eax, zi[8]
					cmp eax, [esi]
					jge next20
					mov[esi], eax
					mov eax, dwTextureSize
					fld simd_u[8]
					frndint
					fistp ui[8]
					cmp ui[8], 0
					jge next9
					mov ui[8], 0
					next9:				cmp ui[8], eax
					jle next10
					mov ui[8], eax
					next10 : fld simd_v[8]
					frndint
					fistp vi[8]
					cmp vi[8], 0
					jge next11
					mov vi[8], 0
					next11 : cmp vi[8], eax
					jle next12
					mov vi[8], eax
					next12 : mov eax, vi[8]
					mul dwShift
					shr eax, 2
					add eax, ui[8]
					shl eax, 2
					push esi
					mov esi, lpTextureData
					add esi, eax
					mov eax, [esi]
					and eax, 0x000000FF
					mov blue, eax
					fild blue
					fmul simd_b[8]
					fistp blue
					cmp blue, 0
					jge next34
					mov blue, 0
					next34:				cmp blue, 255
					jle next35
					mov blue, 255
					next35 : mov eax, [esi]
					and eax, 0x0000FF00
					shr eax, 8
					mov green, eax
					fild green
					fmul simd_g[8]
					fistp green
					cmp green, 0
					jge next36
					mov green, 0
					next36:				cmp green, 255
					jle next37
					mov green, 255
					next37 : mov eax, [esi]
					and eax, 0x00FF0000
					shr eax, 16
					mov red, eax
					fild red
					fmul simd_r[8]
					fistp red
					cmp red, 0
					jge next38
					mov red, 0
					next38:				cmp red, 255
					jle next39
					mov red, 255
					next39 : mov eax, red
					shl eax, 8
					or eax, green
					shl eax, 8
					or eax, blue
					mov[edi], eax
					inc dwFillRate
					pop esi
					next20 : add edi, 4
					add esi, 4
					dec ecx
					jz next17
					fld simd_z[12]
					frndint
					fistp zi[12]
					mov eax, zi[12]
					cmp eax, [esi]
					jge next21
					mov[esi], eax
					mov eax, dwTextureSize
					fld simd_u[12]
					frndint
					fistp ui[12]
					cmp ui[12], 0
					jge next13
					mov ui[12], 0
					next13:				cmp ui[12], eax
					jle next14
					mov ui[12], eax
					next14 : fld simd_v[12]
					frndint
					fistp vi[12]
					cmp vi[12], 0
					jge next15
					mov vi[12], 0
					next15 : cmp vi[12], eax
					jle next16
					mov vi[12], eax
					next16 : mov eax, vi[12]
					mul dwShift
					shr eax, 2
					add eax, ui[12]
					shl eax, 2
					push esi
					mov esi, lpTextureData
					add esi, eax
					mov eax, [esi]
					and eax, 0x000000FF
					mov blue, eax
					fild blue
					fmul simd_b[12]
					fistp blue
					cmp blue, 0
					jge next40
					mov blue, 0
					next40:				cmp blue, 255
					jle next41
					mov blue, 255
					next41 : mov eax, [esi]
					and eax, 0x0000FF00
					shr eax, 8
					mov green, eax
					fild green
					fmul simd_g[12]
					fistp green
					cmp green, 0
					jge next42
					mov green, 0
					next42:				cmp green, 255
					jle next43
					mov green, 255
					next43 : mov eax, [esi]
					and eax, 0x00FF0000
					shr eax, 16
					mov red, eax
					fild red
					fmul simd_r[12]
					fistp red
					cmp red, 0
					jge next44
					mov red, 0
					next44:				cmp red, 255
					jle next45
					mov red, 255
					next45 : mov eax, red
					shl eax, 8
					or eax, green
					shl eax, 8
					or eax, blue
					mov[edi], eax
					inc dwFillRate
					pop esi
					next21 : add edi, 4
					add esi, 4
					dec ecx
					jnz loop1
					next17 : nop
				}
				m_iFillRate += dwFillRate;
				m_iSkipRate += (dwSize - dwFillRate);
			}
			lpData += m_iWidth;
			lpZBufferData += m_iWidth;
		}

		oldxmin = xmin;
		oldxmax = xmax;

		xm += dxm;
		xb += dxb;
		zm += dzm;
		zb += dzb;
		um += dum;
		vm += dvm;
		ub += dub;
		vb += dvb;
		reds += red1;
		greens += green1;
		blues += blue1;
		rede += red2;
		greene += green2;
		bluee += blue2;
	}
}

void Draw_Top_Triangle_Color(_VERTEX3D top, _VERTEX3D middle, _VERTEX3D bottom)
{
	_PIXEL color = top.color;
	long minY = (long)top.p.y;
	long maxY = (long)middle.p.y;
	long topX = (long)top.p.x;

	LPBYTE lpData = m_lpData;
	LPBYTE lpZBufferData = m_lpZBuffer;
	DWORD dwTotalOffset = 0;
	if (minY >= 0)
	{
		dwTotalOffset = (m_iHeight - 1 - minY) * m_iWidth;
	}
	else
	{
		dwTotalOffset = (m_iHeight - 1) * m_iWidth;
	}
	lpData += dwTotalOffset;
	lpZBufferData += dwTotalOffset;

	if ((minY > m_iHeight - 1) || (maxY < 0))
	{
		return;
	}

	float xm, xb;
	long x, xmin, xmax, oldxmin, oldxmax;
	float dxm = (float)(middle.p.x - top.p.x) / (float)(middle.p.y - top.p.y);
	float dxb = (float)(bottom.p.x - top.p.x) / (float)(bottom.p.y - top.p.y);
	float dzm = (1.0f / middle.p.z - 1.0f / top.p.z) / (middle.p.y - top.p.y);
	float dzb = (1.0f / bottom.p.z - 1.0f / top.p.z) / (bottom.p.y - top.p.y);
	x = topX;
	xm = top.p.x;
	xb = top.p.x;
	xmin = x;
	xmax = x;
	oldxmin = x;
	oldxmax = x;
	float zm = 1.0f / top.p.z;
	float zb = 1.0f / top.p.z;
	for (long y1 = minY; y1 <= maxY; y1++)
	{
		xmin = (long)xm;
		xmax = (long)xb;
		if (xmin > xmax)
		{
			long temp = xmin;
			xmin = xmax;
			xmax = temp;
		}

		if (xmin > oldxmax)
		{
			xmin = oldxmax + 1;
		}
		else if (xmax < oldxmin)
		{
			xmax = oldxmin - 1;
		}

		if ((y1 >= 0) && (y1 <= m_iHeight - 1))
		{
			DWORD dwSize = xmax - xmin + 1;
			LPBYTE lpTempData = lpData;
			LPBYTE lpTempZBufferData = lpZBufferData;

			if ((xmin <= m_iWidth - 1) && (xmax >= 0))
			{
				float fSize = (float)dwSize;
				float dz = (zb - zm) / fSize;
				float z = zm;

				if (xmin < 0)
				{
					dwSize += xmin;
					z -= (xmin * dz);
				}
				else
				{
					lpTempData += xmin;
					lpTempZBufferData += xmin;
				}

				if (xmax > m_iWidth - 1)
				{
					dwSize -= (xmax - m_iWidth + 1);
				}

				/*				for (int x=0; x<(int)dwSize; x++)
								{
									float z_inv = 1.0f / z;
									if (z_inv < (*lpTempZBufferData))
									{
										*lpTempZBufferData = z_inv;
										*lpTempData++ = color;
									}
								}*/

				long dwFillRate = 0;
				byte zi;
				__asm {
					mov edi, lpTempData
					mov esi, lpTempZBufferData
					mov ecx, dwSize
					mov bl, color
					loop1 : fld1
					fdiv z
					frndint
					fistp zi
					mov al, zi
					cmp al, [esi]
					jge next1
					mov[esi], al
					mov[edi], bl
					inc dwFillRate
					next1 : fld z
					fadd dz
					fstp z
					add edi, 1
					add esi, 1
					dec ecx
					jnz loop1
				}
				m_iFillRate += dwFillRate;
				m_iSkipRate += (dwSize - dwFillRate);
			}
			lpData -= m_iWidth;
			lpZBufferData -= m_iWidth;
		}

		oldxmin = xmin;
		oldxmax = xmax;

		xm += dxm;
		xb += dxb;
		zm += dzm;
		zb += dzb;
	}
}

void Draw_Top_Triangle_Gouraud(_VERTEX3D top, _VERTEX3D middle, _VERTEX3D bottom)
{
	_PIXEL c1 = top.color;
	_PIXEL c2 = middle.color;
	_PIXEL c3 = bottom.color;
	long minY = (long)top.p.y;
	long maxY = (long)middle.p.y;
	long topX = (long)top.p.x;

	LPBYTE lpData = m_lpData;
	LPBYTE lpZBufferData = m_lpZBuffer;
	DWORD dwTotalOffset = 0;
	if (minY >= 0)
	{
		dwTotalOffset = (m_iHeight - 1 - minY) * m_iWidth;
	}
	else
	{
		dwTotalOffset = (m_iHeight - 1) * m_iWidth;
	}
	lpData += dwTotalOffset;
	lpZBufferData += dwTotalOffset;

	float cxm, cxb, red1, green1, blue1, red2, green2, blue2;
	float xm, xb, reds, greens, blues, rede, greene, bluee;
	long x, xmin, xmax, oldxmin, oldxmax;
	float dxm = (middle.p.x - top.p.x) / (middle.p.y - top.p.y);
	float dxb = (bottom.p.x - top.p.x) / (bottom.p.y - top.p.y);
	float dzm = (1.0f / middle.p.z - 1.0f / top.p.z) / (middle.p.y - top.p.y);
	float dzb = (1.0f / bottom.p.z - 1.0f / top.p.z) / (bottom.p.y - top.p.y);
	x = topX;
	xm = top.p.x;
	xb = top.p.x;
	xmin = x;
	xmax = x;
	oldxmin = x;
	oldxmax = x;
	float zm = 1.0f / top.p.z;
	float zb = 1.0f / top.p.z;
	long midY = (long)middle.p.y;
	long midX = (long)(top.p.x + (middle.p.y - top.p.y) * dxb);
	cxm = middle.p.y - top.p.y;
	cxb = bottom.p.y - top.p.y;
	if (cxm == 0.0f)
	{
		red1 = green1 = blue1 = 0.0f;
	}
	else
	{
		red1 = ((float)_GetRValue(c2) / middle.p.z - (float)_GetRValue(c1) / top.p.z) / cxm;
		green1 = ((float)_GetGValue(c2) / middle.p.z - (float)_GetGValue(c1) / top.p.z) / cxm;
		blue1 = ((float)_GetBValue(c2) / middle.p.z - (float)_GetBValue(c1) / top.p.z) / cxm;
	}
	if (cxb == 0.0f)
	{
		red2 = green2 = blue2 = 0.0f;
	}
	else
	{
		red2 = ((float)_GetRValue(c3) / bottom.p.z - (float)_GetRValue(c1) / top.p.z) / cxb;
		green2 = ((float)_GetGValue(c3) / bottom.p.z - (float)_GetGValue(c1) / top.p.z) / cxb;
		blue2 = ((float)_GetBValue(c3) / bottom.p.z - (float)_GetBValue(c1) / top.p.z) / cxb;
	}
	m_bInvert = FALSE;
	if (midX < (long)middle.p.x)
	{
		float rt = red2;
		float gt = green2;
		float bt = blue2;
		red2 = red1;
		green2 = green1;
		blue2 = blue1;
		red1 = rt;
		green1 = gt;
		blue1 = bt;
		float zmt = dzm;
		dzm = dzb;
		dzb = zmt;
		m_bInvert = TRUE;
	}
	reds = (float)_GetRValue(c1) / top.p.z;
	greens = (float)_GetGValue(c1) / top.p.z;
	blues = (float)_GetBValue(c1) / top.p.z;
	rede = reds;
	greene = greens;
	bluee = blues;

	if ((minY > m_iHeight - 1) || (maxY < 0))
	{
		return;
	}

	for (long y1 = minY; y1 <= maxY; y1++)
	{
		xmin = (long)xm;
		xmax = (long)xb;
		if (xmin > xmax)
		{
			long temp = xmin;
			xmin = xmax;
			xmax = temp;
		}

		if (xmin > oldxmax)
		{
			xmin = oldxmax + 1;
		}
		else if (xmax < oldxmin)
		{
			xmax = oldxmin - 1;
		}

		if ((y1 >= 0) && (y1 <= m_iHeight - 1))
		{
			DWORD dwSize = xmax - xmin + 1;
			LPBYTE lpTempData = lpData;
			LPBYTE lpTempZBufferData = lpZBufferData;

			if ((xmin <= m_iWidth - 1) && (xmax >= 0))
			{
				float fSize = (float)dwSize;
				float dr = (rede - reds) / fSize;
				float dg = (greene - greens) / fSize;
				float db = (bluee - blues) / fSize;
				float dz = (zb - zm) / fSize;
				float r = reds;
				float g = greens;
				float b = blues;
				float z = zm;

				if (xmin < 0)
				{
					dwSize += xmin;
					r -= (xmin * dr);
					g -= (xmin * dg);
					b -= (xmin * db);
					z -= (xmin * dz);
				}
				else
				{
					lpTempData += xmin;
					lpTempZBufferData += xmin;
				}

				if (xmax > m_iWidth - 1)
				{
					dwSize -= (xmax - m_iWidth + 1);
				}

				/*				for (int x=0; x<(int)dwSize; x++)
								{
									float z_inv = 1.0f / z;
									if (z_inv < (*lpTempZBufferData))
									{
										*lpTempZBufferData = z_inv;
										long ri = max(0, min(255, (long)(r * z_inv)));
										long gi = max(0, min(255, (long)(g * z_inv)));
										long bi = max(0, min(255, (long)(b * z_inv)));
										*lpTempData++ = _RGB(ri, gi, bi);
										r += dr;
										g += dg;
										b += db;
										z += dz;
									}
								}*/

				float r_dr1 = r + dr;
				float r_dr2 = r_dr1 + dr;
				float r_dr3 = r_dr2 + dr;
				float r_dr4 = 4 * dr;
				float g_dg1 = g + dg;
				float g_dg2 = g_dg1 + dg;
				float g_dg3 = g_dg2 + dg;
				float g_dg4 = 4 * dg;
				float b_db1 = b + db;
				float b_db2 = b_db1 + db;
				float b_db3 = b_db2 + db;
				float b_db4 = 4 * db;
				float z_dz1 = z + dz;
				float z_dz2 = z_dz1 + dz;
				float z_dz3 = z_dz2 + dz;
				float z_dz4 = 4 * dz;
				__m128 simd_r = { r, r_dr1, r_dr2, r_dr3 };
				__m128 simd_g = { g, g_dg1, g_dg2, g_dg3 };
				__m128 simd_b = { b, b_db1, b_db2, b_db3 };
				__m128 simd_z = { z, z_dz1, z_dz2, z_dz3 };
				__m128 simd_dr = { r_dr4, r_dr4, r_dr4, r_dr4 };
				__m128 simd_dg = { g_dg4, g_dg4, g_dg4, g_dg4 };
				__m128 simd_db = { b_db4, b_db4, b_db4, b_db4 };
				__m128 simd_dz = { z_dz4, z_dz4, z_dz4, z_dz4 };
				long ri[4] = { 0, 0, 0, 0 };
				long gi[4] = { 0, 0, 0, 0 };
				long bi[4] = { 0, 0, 0, 0 };
				long zi[4] = { 0, 0, 0, 0 };

				long dwFillRate = 0;
				_asm {
					mov edi, lpTempData
					mov esi, lpTempZBufferData
					mov ecx, dwSize
					movaps xmm0, simd_r
					movaps xmm1, simd_g
					movaps xmm2, simd_b
					movaps xmm3, simd_z
					loop1 : movaps xmm4, xmm0
					movaps xmm5, xmm1
					movaps xmm6, xmm2
					rcpps xmm7, xmm3
					mulps xmm4, xmm7
					mulps xmm5, xmm7
					mulps xmm6, xmm7
					movaps simd_r, xmm4
					movaps simd_g, xmm5
					movaps simd_b, xmm6
					movaps simd_z, xmm7
					addps xmm0, simd_dr
					addps xmm1, simd_dg
					addps xmm2, simd_db
					addps xmm3, simd_dz
					fld simd_z[0]
					frndint
					fistp zi[0]
					mov eax, zi[0]
					cmp eax, [esi]
					jge next26
					mov[esi], eax
					fld simd_r[0]
					frndint
					fistp ri[0]
					cmp ri[0], 0
					jge next1
					mov ri[0], 0
					next1:				cmp ri[0], 255
					jle next2
					mov ri[0], 255
					next2 : fld simd_g[0]
					frndint
					fistp gi[0]
					cmp gi[0], 0
					jge next3
					mov gi[0], 0
					next3 : cmp gi[0], 255
					jle next4
					mov gi[0], 255
					next4 : fld simd_b[0]
					frndint
					fistp bi[0]
					cmp bi[0], 0
					jge next5
					mov bi[0], 0
					next5 : cmp bi[0], 255
					jle next6
					mov bi[0], 255
					next6 : mov eax, ri[0]
					shl eax, 8
					or eax, gi[0]
					shl eax, 8
					or eax, bi[0]
					mov[edi], eax
					inc dwFillRate
					next26 : add edi, 4
					add esi, 4
					dec ecx
					jz next25
					fld simd_z[4]
					frndint
					fistp zi[4]
					mov eax, zi[4]
					cmp eax, [esi]
					jge next27
					mov[esi], eax
					fld simd_r[4]
					frndint
					fistp ri[4]
					cmp ri[4], 0
					jge next7
					mov ri[4], 0
					next7:				cmp ri[4], 255
					jle next8
					mov ri[4], 255
					next8 : fld simd_g[4]
					frndint
					fistp gi[4]
					cmp gi[4], 0
					jge next9
					mov gi[4], 0
					next9 : cmp gi[4], 255
					jle next10
					mov gi[4], 255
					next10 : fld simd_b[4]
					frndint
					fistp bi[4]
					cmp bi[4], 0
					jge next11
					mov bi[4], 0
					next11 : cmp bi[4], 255
					jle next12
					mov bi[4], 255
					next12 : mov eax, ri[4]
					shl eax, 8
					or eax, gi[4]
					shl eax, 8
					or eax, bi[4]
					mov[edi], eax
					inc dwFillRate
					next27 : add edi, 4
					add esi, 4
					dec ecx
					jz next25
					fld simd_z[8]
					frndint
					fistp zi[8]
					mov eax, zi[8]
					cmp eax, [esi]
					jge next28
					mov[esi], eax
					fld simd_r[8]
					frndint
					fistp ri[8]
					cmp ri[8], 0
					jge next13
					mov ri[8], 0
					next13:				cmp ri[8], 255
					jle next14
					mov ri[8], 255
					next14 : fld simd_g[8]
					frndint
					fistp gi[8]
					cmp gi[8], 0
					jge next15
					mov gi[8], 0
					next15 : cmp gi[8], 255
					jle next16
					mov gi[8], 255
					next16 : fld simd_b[8]
					frndint
					fistp bi[8]
					cmp bi[8], 0
					jge next17
					mov bi[8], 0
					next17 : cmp bi[8], 255
					jle next18
					mov bi[8], 255
					next18 : mov eax, ri[8]
					shl eax, 8
					or eax, gi[8]
					shl eax, 8
					or eax, bi[8]
					mov[edi], eax
					inc dwFillRate
					next28 : add edi, 4
					add esi, 4
					dec ecx
					jz next25
					fld simd_z[12]
					frndint
					fistp zi[12]
					mov eax, zi[12]
					cmp eax, [esi]
					jge next29
					mov[esi], eax
					fld simd_r[12]
					frndint
					fistp ri[12]
					cmp ri[12], 0
					jge next19
					mov ri[12], 0
					next19:				cmp ri[12], 255
					jle next20
					mov ri[12], 255
					next20 : fld simd_g[12]
					frndint
					fistp gi[12]
					cmp gi[12], 0
					jge next21
					mov gi[12], 0
					next21 : cmp gi[12], 255
					jle next22
					mov gi[12], 255
					next22 : fld simd_b[12]
					frndint
					fistp bi[12]
					cmp bi[12], 0
					jge next23
					mov bi[12], 0
					next23 : cmp bi[12], 255
					jle next24
					mov bi[12], 255
					next24 : mov eax, ri[12]
					shl eax, 8
					or eax, gi[12]
					shl eax, 8
					or eax, bi[12]
					mov[edi], eax
					inc dwFillRate
					next29 : add edi, 4
					add esi, 4
					dec ecx
					jnz loop1
					next25 : nop
				}
				m_iFillRate += dwFillRate;
				m_iSkipRate += (dwSize - dwFillRate);
			}
			lpData -= m_iWidth;
			lpZBufferData -= m_iWidth;
		}

		oldxmin = xmin;
		oldxmax = xmax;

		xm += dxm;
		xb += dxb;
		zm += dzm;
		zb += dzb;
		reds += red1;
		greens += green1;
		blues += blue1;
		rede += red2;
		greene += green2;
		bluee += blue2;
	}
}

void Draw_Top_Triangle_Texture(_VERTEX3D top, _VERTEX3D middle, _VERTEX3D bottom, _TEXTUREINFO textureInfo)
{
	_POINT2D uvt = top.uv;
	_POINT2D uvm = middle.uv;
	_POINT2D uvb = bottom.uv;
	long minY = (long)top.p.y;
	long maxY = (long)middle.p.y;
	long topX = (long)top.p.x;

	LPBYTE lpData = m_lpData;
	LPBYTE lpZBufferData = m_lpZBuffer;
	DWORD dwTotalOffset = 0;
	if (minY >= 0)
	{
		dwTotalOffset = (m_iHeight - 1 - minY) * m_iWidth;
	}
	else
	{
		dwTotalOffset = (m_iHeight - 1) * m_iWidth;
	}
	lpData += dwTotalOffset;
	lpZBufferData += dwTotalOffset;

	float xm, xb;
	long x, xmin, xmax, oldxmin, oldxmax;
	float dxm = (middle.p.x - top.p.x) / (middle.p.y - top.p.y);
	float dxb = (bottom.p.x - top.p.x) / (bottom.p.y - top.p.y);
	float dzm = (1.0f / middle.p.z - 1.0f / top.p.z) / (middle.p.y - top.p.y);
	float dzb = (1.0f / bottom.p.z - 1.0f / top.p.z) / (bottom.p.y - top.p.y);
	x = topX;
	xm = top.p.x;
	xb = top.p.x;
	xmin = x;
	xmax = x;
	oldxmin = x;
	oldxmax = x;
	float zm = 1.0f / top.p.z;
	float zb = 1.0f / top.p.z;
	long midY = (long)middle.p.y;
	long midX = (long)(top.p.x + (middle.p.y - top.p.y) * dxb);
	float dum = (float)((float)uvm.x / (float)middle.p.z - (float)uvt.x / (float)top.p.z) / (float)(middle.p.y - top.p.y);
	float dvm = (float)((float)uvm.y / (float)middle.p.z - (float)uvt.y / (float)top.p.z) / (float)(middle.p.y - top.p.y);
	float dub = (float)((float)uvb.x / (float)bottom.p.z - (float)uvt.x / (float)top.p.z) / (float)(bottom.p.y - top.p.y);
	float dvb = (float)((float)uvb.y / (float)bottom.p.z - (float)uvt.y / (float)top.p.z) / (float)(bottom.p.y - top.p.y);
	float um = (float)uvt.x / (float)top.p.z;
	float vm = (float)uvt.y / (float)top.p.z;
	float ub = (float)uvt.x / (float)top.p.z;
	float vb = (float)uvt.y / (float)top.p.z;
	m_bInvert = FALSE;
	if (midX < (long)middle.p.x)
	{
		float umt = dum;
		float uvt = dvm;
		dum = dub;
		dvm = dvb;
		dub = umt;
		dvb = uvt;
		float zmt = dzm;
		dzm = dzb;
		dzb = zmt;
		m_bInvert = TRUE;
	}

	if ((minY > m_iHeight - 1) || (maxY < 0))
	{
		return;
	}

	for (long y1 = minY; y1 <= maxY; y1++)
	{
		xmin = (long)xm;
		xmax = (long)xb;
		if (xmin > xmax)
		{
			long temp = xmin;
			xmin = xmax;
			xmax = temp;
		}

		if (xmin > oldxmax)
		{
			xmin = oldxmax + 1;
		}
		else if (xmax < oldxmin)
		{
			xmax = oldxmin - 1;
		}

		if ((y1 >= 0) && (y1 <= m_iHeight - 1))
		{
			DWORD dwSize = xmax - xmin + 1;
			LPBYTE lpTempData = lpData;
			LPBYTE lpTempZBufferData = lpZBufferData;

			if ((xmin <= m_iWidth - 1) && (xmax >= 0))
			{
				float fSize = (float)dwSize;
				float du = (ub - um) / fSize;
				float dv = (vb - vm) / fSize;
				float dz = (zb - zm) / fSize;
				float u = um;
				float v = vm;
				float z = zm;

				if (xmin < 0)
				{
					dwSize += xmin;
					u -= (xmin * du);
					v -= (xmin * dv);
					z -= (xmin * dz);
				}
				else
				{
					lpTempData += xmin;
					lpTempZBufferData += xmin;
				}

				if (xmax > m_iWidth - 1)
				{
					dwSize -= (xmax - m_iWidth + 1);
				}

				DWORD dwTextureSize = textureInfo.dwSize - 1;
				LPBYTE lpTextureData = textureInfo.lpTexture;
				DWORD dwShift = (dwTextureSize + 1) << 2;
				/*				for (int x=0; x<(int)dwSize; x++)
								{
									float z_inv = 1.0f / z;
									if (z_inv < (*lpTempZBufferData))
									{
										*lpTempZBufferData = z_inv;
										long ui = max(0, min(dwTextureSize, (long)(u * z_inv)));
										long vi = max(0, min(dwTextureSize, (long)(v * z_inv)));
										*lpTempData++ = lpTextureData[ui + vi*textureInfo.dwSize];
										u += du;
										v += dv;
										z += dz;
									}
								}*/

				float u_du1 = u + du;
				float u_du2 = u_du1 + du;
				float u_du3 = u_du2 + du;
				float u_du4 = 4 * du;
				float v_dv1 = v + dv;
				float v_dv2 = v_dv1 + dv;
				float v_dv3 = v_dv2 + dv;
				float v_dv4 = 4 * dv;
				float z_dz1 = z + dz;
				float z_dz2 = z_dz1 + dz;
				float z_dz3 = z_dz2 + dz;
				float z_dz4 = 4 * dz;
				__m128 simd_u = { u, u_du1, u_du2, u_du3 };
				__m128 simd_v = { v, v_dv1, v_dv2, v_dv3 };
				__m128 simd_z = { z, z_dz1, z_dz2, z_dz3 };
				__m128 simd_du = { u_du4, u_du4, u_du4, u_du4 };
				__m128 simd_dv = { v_dv4, v_dv4, v_dv4, v_dv4 };
				__m128 simd_dz = { z_dz4, z_dz4, z_dz4, z_dz4 };
				long ui[4] = { 0, 0, 0, 0 };
				long vi[4] = { 0, 0, 0, 0 };
				long zi[4] = { 0, 0, 0, 0 };

				long dwFillRate = 0;
				_asm {
					mov edi, lpTempData
					mov esi, lpTempZBufferData
					mov ecx, dwSize
					movaps xmm0, simd_u
					movaps xmm1, simd_v
					movaps xmm2, simd_z
					loop1 : movaps xmm3, xmm0
					movaps xmm4, xmm1
					rcpps xmm5, xmm2
					mulps xmm3, xmm5
					mulps xmm4, xmm5
					movaps simd_u, xmm3
					movaps simd_v, xmm4
					movaps simd_z, xmm5
					addps xmm0, simd_du
					addps xmm1, simd_dv
					addps xmm2, simd_dz
					fld simd_z[0]
					frndint
					fistp zi[0]
					mov eax, zi[0]
					cmp eax, [esi]
					jge next18
					mov[esi], eax
					mov eax, dwTextureSize
					fld simd_u[0]
					frndint
					fistp ui[0]
					cmp ui[0], 0
					jge next1
					mov ui[0], 0
					next1:				cmp ui[0], eax
					jle next2
					mov ui[0], eax
					next2 : fld simd_v[0]
					frndint
					fistp vi[0]
					cmp vi[0], 0
					jge next3
					mov vi[0], 0
					next3 : cmp vi[0], eax
					jle next4
					mov vi[0], eax
					next4 : mov eax, vi[0]
					mul dwShift
					shr eax, 2
					add eax, ui[0]
					shl eax, 2
					push esi
					mov esi, lpTextureData
					add esi, eax
					mov eax, [esi]
					mov[edi], eax
					inc dwFillRate
					pop esi
					next18 : add edi, 4
					add esi, 4
					dec ecx
					jz next17
					fld simd_z[4]
					frndint
					fistp zi[4]
					mov eax, zi[4]
					cmp eax, [esi]
					jge next19
					mov[esi], eax
					mov eax, dwTextureSize
					fld simd_u[4]
					frndint
					fistp ui[4]
					cmp ui[4], 0
					jge next5
					mov ui[4], 0
					next5:				cmp ui[4], eax
					jle next6
					mov ui[4], eax
					next6 : fld simd_v[4]
					frndint
					fistp vi[4]
					cmp vi[4], 0
					jge next7
					mov vi[4], 0
					next7 : cmp vi[4], eax
					jle next8
					mov vi[4], eax
					next8 : mov eax, vi[4]
					mul dwShift
					shr eax, 2
					add eax, ui[4]
					shl eax, 2
					push esi
					mov esi, lpTextureData
					add esi, eax
					mov eax, [esi]
					mov[edi], eax
					inc dwFillRate
					pop esi
					next19 : add edi, 4
					add esi, 4
					dec ecx
					jz next17
					fld simd_z[8]
					frndint
					fistp zi[8]
					mov eax, zi[8]
					cmp eax, [esi]
					jge next20
					mov[esi], eax
					mov eax, dwTextureSize
					fld simd_u[8]
					frndint
					fistp ui[8]
					cmp ui[8], 0
					jge next9
					mov ui[8], 0
					next9:				cmp ui[8], eax
					jle next10
					mov ui[8], eax
					next10 : fld simd_v[8]
					frndint
					fistp vi[8]
					cmp vi[8], 0
					jge next11
					mov vi[8], 0
					next11 : cmp vi[8], eax
					jle next12
					mov vi[8], eax
					next12 : mov eax, vi[8]
					mul dwShift
					shr eax, 2
					add eax, ui[8]
					shl eax, 2
					push esi
					mov esi, lpTextureData
					add esi, eax
					mov eax, [esi]
					mov[edi], eax
					inc dwFillRate
					pop esi
					next20 : add edi, 4
					add esi, 4
					dec ecx
					jz next17
					fld simd_z[12]
					frndint
					fistp zi[12]
					mov eax, zi[12]
					cmp eax, [esi]
					jge next21
					mov[esi], eax
					mov eax, dwTextureSize
					fld simd_u[12]
					frndint
					fistp ui[12]
					cmp ui[12], 0
					jge next13
					mov ui[12], 0
					next13:				cmp ui[12], eax
					jle next14
					mov ui[12], eax
					next14 : fld simd_v[12]
					frndint
					fistp vi[12]
					cmp vi[12], 0
					jge next15
					mov vi[12], 0
					next15 : cmp vi[12], eax
					jle next16
					mov vi[12], eax
					next16 : mov eax, vi[12]
					mul dwShift
					shr eax, 2
					add eax, ui[12]
					shl eax, 2
					push esi
					mov esi, lpTextureData
					add esi, eax
					mov eax, [esi]
					mov[edi], eax
					inc dwFillRate
					pop esi
					next21 : add edi, 4
					add esi, 4
					dec ecx
					jnz loop1
					next17 : nop
				}
				m_iFillRate += dwFillRate;
				m_iSkipRate += (dwSize - dwFillRate);
			}
			lpData -= m_iWidth;
			lpZBufferData -= m_iWidth;
		}

		oldxmin = xmin;
		oldxmax = xmax;

		xm += dxm;
		xb += dxb;
		zm += dzm;
		zb += dzb;
		um += dum;
		vm += dvm;
		ub += dub;
		vb += dvb;
	}
}


void Draw_Top_Triangle_Texture_Color(_VERTEX3D top, _VERTEX3D middle, _VERTEX3D bottom, _TEXTUREINFO textureInfo)
{
	_POINT2D uvt = top.uv;
	_POINT2D uvm = middle.uv;
	_POINT2D uvb = bottom.uv;
	long minY = (long)top.p.y;
	long maxY = (long)middle.p.y;
	long topX = (long)top.p.x;
	float f_red = (float)_GetRValue(top.color) / 255.0f;
	float f_green = (float)_GetGValue(top.color) / 255.0f;
	float f_blue = (float)_GetBValue(top.color) / 255.0f;

	LPBYTE lpData = m_lpData;
	LPBYTE lpZBufferData = m_lpZBuffer;
	DWORD dwTotalOffset = 0;
	if (minY >= 0)
	{
		dwTotalOffset = (m_iHeight - 1 - minY) * m_iWidth;
	}
	else
	{
		dwTotalOffset = (m_iHeight - 1) * m_iWidth;
	}
	lpData += dwTotalOffset;
	lpZBufferData += dwTotalOffset;

	float xm, xb;
	long x, xmin, xmax, oldxmin, oldxmax;
	float dxm = (middle.p.x - top.p.x) / (middle.p.y - top.p.y);
	float dxb = (bottom.p.x - top.p.x) / (bottom.p.y - top.p.y);
	float dzm = (1.0f / middle.p.z - 1.0f / top.p.z) / (middle.p.y - top.p.y);
	float dzb = (1.0f / bottom.p.z - 1.0f / top.p.z) / (bottom.p.y - top.p.y);
	x = topX;
	xm = top.p.x;
	xb = top.p.x;
	xmin = x;
	xmax = x;
	oldxmin = x;
	oldxmax = x;
	float zm = 1.0f / top.p.z;
	float zb = 1.0f / top.p.z;
	long midY = (long)middle.p.y;
	long midX = (long)(top.p.x + (middle.p.y - top.p.y) * dxb);
	float dum = (float)((float)uvm.x / (float)middle.p.z - (float)uvt.x / (float)top.p.z) / (float)(middle.p.y - top.p.y);
	float dvm = (float)((float)uvm.y / (float)middle.p.z - (float)uvt.y / (float)top.p.z) / (float)(middle.p.y - top.p.y);
	float dub = (float)((float)uvb.x / (float)bottom.p.z - (float)uvt.x / (float)top.p.z) / (float)(bottom.p.y - top.p.y);
	float dvb = (float)((float)uvb.y / (float)bottom.p.z - (float)uvt.y / (float)top.p.z) / (float)(bottom.p.y - top.p.y);
	float um = (float)uvt.x / (float)top.p.z;
	float vm = (float)uvt.y / (float)top.p.z;
	float ub = (float)uvt.x / (float)top.p.z;
	float vb = (float)uvt.y / (float)top.p.z;
	m_bInvert = FALSE;
	if (midX < (long)middle.p.x)
	{
		float umt = dum;
		float uvt = dvm;
		dum = dub;
		dvm = dvb;
		dub = umt;
		dvb = uvt;
		float zmt = dzm;
		dzm = dzb;
		dzb = zmt;
		m_bInvert = TRUE;
	}

	if ((minY > m_iHeight - 1) || (maxY < 0))
	{
		return;
	}

	for (long y1 = minY; y1 <= maxY; y1++)
	{
		xmin = (long)xm;
		xmax = (long)xb;
		if (xmin > xmax)
		{
			long temp = xmin;
			xmin = xmax;
			xmax = temp;
		}

		if (xmin > oldxmax)
		{
			xmin = oldxmax + 1;
		}
		else if (xmax < oldxmin)
		{
			xmax = oldxmin - 1;
		}

		if ((y1 >= 0) && (y1 <= m_iHeight - 1))
		{
			DWORD dwSize = xmax - xmin + 1;
			LPBYTE lpTempData = lpData;
			LPBYTE lpTempZBufferData = lpZBufferData;

			if ((xmin <= m_iWidth - 1) && (xmax >= 0))
			{
				float fSize = (float)dwSize;
				float du = (ub - um) / fSize;
				float dv = (vb - vm) / fSize;
				float dz = (zb - zm) / fSize;
				float u = um;
				float v = vm;
				float z = zm;

				if (xmin < 0)
				{
					dwSize += xmin;
					u -= (xmin * du);
					v -= (xmin * dv);
					z -= (xmin * dz);
				}
				else
				{
					lpTempData += xmin;
					lpTempZBufferData += xmin;
				}

				if (xmax > m_iWidth - 1)
				{
					dwSize -= (xmax - m_iWidth + 1);
				}

				DWORD dwTextureSize = textureInfo.dwSize - 1;
				LPBYTE lpTextureData = textureInfo.lpTexture;
				DWORD dwShift = (dwTextureSize + 1) << 2;
				/*				for (int x=0; x<(int)dwSize; x++)
								{
									float z_inv = 1.0f / z;
									if (z_inv < (*lpTempZBufferData))
									{
										*lpTempZBufferData = z_inv;
										long ui = max(0, min((long)textureInfo.dwSize-1, (long)(u * z_inv)));
										long vi = max(0, min((long)textureInfo.dwSize-1, (long)(v * z_inv)));
										_PIXEL srcPixel = lpTextureData[ui + vi*textureInfo.dwSize];
										BYTE red = (BYTE)((float)_GetRValue(srcPixel) * f_red);
										BYTE green = (BYTE)((float)_GetGValue(srcPixel) * f_green);
										BYTE blue = (BYTE)((float)_GetBValue(srcPixel) * f_blue);
										*lpTempData++ = _RGB(red, green, blue);
										u += du;
										v += dv;
										z += dz;
									}
								}*/

				float u_du1 = u + du;
				float u_du2 = u_du1 + du;
				float u_du3 = u_du2 + du;
				float u_du4 = 4 * du;
				float v_dv1 = v + dv;
				float v_dv2 = v_dv1 + dv;
				float v_dv3 = v_dv2 + dv;
				float v_dv4 = 4 * dv;
				float z_dz1 = z + dz;
				float z_dz2 = z_dz1 + dz;
				float z_dz3 = z_dz2 + dz;
				float z_dz4 = 4 * dz;
				__m128 simd_u = { u, u_du1, u_du2, u_du3 };
				__m128 simd_v = { v, v_dv1, v_dv2, v_dv3 };
				__m128 simd_z = { z, z_dz1, z_dz2, z_dz3 };
				__m128 simd_du = { u_du4, u_du4, u_du4, u_du4 };
				__m128 simd_dv = { v_dv4, v_dv4, v_dv4, v_dv4 };
				__m128 simd_dz = { z_dz4, z_dz4, z_dz4, z_dz4 };
				long ui[4] = { 0, 0, 0, 0 };
				long vi[4] = { 0, 0, 0, 0 };
				long zi[4] = { 0, 0, 0, 0 };

				long dwFillRate = 0;
				long red, green, blue;
				_asm {
					mov edi, lpTempData
					mov esi, lpTempZBufferData
					mov ecx, dwSize
					movaps xmm0, simd_u
					movaps xmm1, simd_v
					movaps xmm2, simd_z
					loop1 : movaps xmm3, xmm0
					movaps xmm4, xmm1
					rcpps xmm5, xmm2
					mulps xmm3, xmm5
					mulps xmm4, xmm5
					movaps simd_u, xmm3
					movaps simd_v, xmm4
					movaps simd_z, xmm5
					addps xmm0, simd_du
					addps xmm1, simd_dv
					addps xmm2, simd_dz
					fld simd_z[0]
					frndint
					fistp zi[0]
					mov eax, zi[0]
					cmp eax, [esi]
					jge next18
					mov[esi], eax
					mov eax, dwTextureSize
					fld simd_u[0]
					frndint
					fistp ui[0]
					cmp ui[0], 0
					jge next1
					mov ui[0], 0
					next1:				cmp ui[0], eax
					jle next2
					mov ui[0], eax
					next2 : fld simd_v[0]
					frndint
					fistp vi[0]
					cmp vi[0], 0
					jge next3
					mov vi[0], 0
					next3 : cmp vi[0], eax
					jle next4
					mov vi[0], eax
					next4 : mov eax, vi[0]
					mul dwShift
					shr eax, 2
					add eax, ui[0]
					shl eax, 2
					push esi
					mov esi, lpTextureData
					add esi, eax
					mov eax, [esi]
					and eax, 0x000000FF
					mov blue, eax
					fild blue
					fmul f_blue
					fistp blue
					mov eax, [esi]
					and eax, 0x0000FF00
					shr eax, 8
					mov green, eax
					fild green
					fmul f_green
					fistp green
					mov eax, [esi]
					and eax, 0x00FF0000
					shr eax, 16
					mov red, eax
					fild red
					fmul f_red
					fistp red
					mov eax, red
					shl eax, 8
					or eax, green
					shl eax, 8
					or eax, blue
					mov[edi], eax
					inc dwFillRate
					pop esi
					next18 : add edi, 4
					add esi, 4
					dec ecx
					jz next17
					fld simd_z[4]
					frndint
					fistp zi[4]
					mov eax, zi[4]
					cmp eax, [esi]
					jge next19
					mov[esi], eax
					mov eax, dwTextureSize
					fld simd_u[4]
					frndint
					fistp ui[4]
					cmp ui[4], 0
					jge next5
					mov ui[4], 0
					next5:				cmp ui[4], eax
					jle next6
					mov ui[4], eax
					next6 : fld simd_v[4]
					frndint
					fistp vi[4]
					cmp vi[4], 0
					jge next7
					mov vi[4], 0
					next7 : cmp vi[4], eax
					jle next8
					mov vi[4], eax
					next8 : mov eax, vi[4]
					mul dwShift
					shr eax, 2
					add eax, ui[4]
					shl eax, 2
					push esi
					mov esi, lpTextureData
					add esi, eax
					mov eax, [esi]
					and eax, 0x000000FF
					mov blue, eax
					fild blue
					fmul f_blue
					fistp blue
					mov eax, [esi]
					and eax, 0x0000FF00
					shr eax, 8
					mov green, eax
					fild green
					fmul f_green
					fistp green
					mov eax, [esi]
					and eax, 0x00FF0000
					shr eax, 16
					mov red, eax
					fild red
					fmul f_red
					fistp red
					mov eax, red
					shl eax, 8
					or eax, green
					shl eax, 8
					or eax, blue
					mov[edi], eax
					inc dwFillRate
					pop esi
					next19 : add edi, 4
					add esi, 4
					dec ecx
					jz next17
					fld simd_z[8]
					frndint
					fistp zi[8]
					mov eax, zi[8]
					cmp eax, [esi]
					jge next20
					mov[esi], eax
					mov eax, dwTextureSize
					fld simd_u[8]
					frndint
					fistp ui[8]
					cmp ui[8], 0
					jge next9
					mov ui[8], 0
					next9:				cmp ui[8], eax
					jle next10
					mov ui[8], eax
					next10 : fld simd_v[8]
					frndint
					fistp vi[8]
					cmp vi[8], 0
					jge next11
					mov vi[8], 0
					next11 : cmp vi[8], eax
					jle next12
					mov vi[8], eax
					next12 : mov eax, vi[8]
					mul dwShift
					shr eax, 2
					add eax, ui[8]
					shl eax, 2
					push esi
					mov esi, lpTextureData
					add esi, eax
					mov eax, [esi]
					and eax, 0x000000FF
					mov blue, eax
					fild blue
					fmul f_blue
					fistp blue
					mov eax, [esi]
					and eax, 0x0000FF00
					shr eax, 8
					mov green, eax
					fild green
					fmul f_green
					fistp green
					mov eax, [esi]
					and eax, 0x00FF0000
					shr eax, 16
					mov red, eax
					fild red
					fmul f_red
					fistp red
					mov eax, red
					shl eax, 8
					or eax, green
					shl eax, 8
					or eax, blue
					mov[edi], eax
					inc dwFillRate
					pop esi
					next20 : add edi, 4
					add esi, 4
					dec ecx
					jz next17
					fld simd_z[12]
					frndint
					fistp zi[12]
					mov eax, zi[12]
					cmp eax, [esi]
					jge next21
					mov[esi], eax
					mov eax, dwTextureSize
					fld simd_u[12]
					frndint
					fistp ui[12]
					cmp ui[12], 0
					jge next13
					mov ui[12], 0
					next13:				cmp ui[12], eax
					jle next14
					mov ui[12], eax
					next14 : fld simd_v[12]
					frndint
					fistp vi[12]
					cmp vi[12], 0
					jge next15
					mov vi[12], 0
					next15 : cmp vi[12], eax
					jle next16
					mov vi[12], eax
					next16 : mov eax, vi[12]
					mul dwShift
					shr eax, 2
					add eax, ui[12]
					shl eax, 2
					push esi
					mov esi, lpTextureData
					add esi, eax
					mov eax, [esi]
					and eax, 0x000000FF
					mov blue, eax
					fild blue
					fmul f_blue
					fistp blue
					mov eax, [esi]
					and eax, 0x0000FF00
					shr eax, 8
					mov green, eax
					fild green
					fmul f_green
					fistp green
					mov eax, [esi]
					and eax, 0x00FF0000
					shr eax, 16
					mov red, eax
					fild red
					fmul f_red
					fistp red
					mov eax, red
					shl eax, 8
					or eax, green
					shl eax, 8
					or eax, blue
					mov[edi], eax
					inc dwFillRate
					pop esi
					next21 : add edi, 4
					add esi, 4
					dec ecx
					jnz loop1
					next17 : nop
				}
				m_iFillRate += dwFillRate;
				m_iSkipRate += (dwSize - dwFillRate);
			}
			lpData -= m_iWidth;
			lpZBufferData -= m_iWidth;
		}

		oldxmin = xmin;
		oldxmax = xmax;

		xm += dxm;
		xb += dxb;
		zm += dzm;
		zb += dzb;
		um += dum;
		vm += dvm;
		ub += dub;
		vb += dvb;
	}
}

void Draw_Top_Triangle_Texture_Gouraud(_VERTEX3D top, _VERTEX3D middle, _VERTEX3D bottom, _TEXTUREINFO textureInfo)
{
	_POINT2D uvt = top.uv;
	_POINT2D uvm = middle.uv;
	_POINT2D uvb = bottom.uv;
	long minY = (long)top.p.y;
	long maxY = (long)middle.p.y;
	long topX = (long)top.p.x;
	_PIXEL c1 = top.color;
	_PIXEL c2 = middle.color;
	_PIXEL c3 = bottom.color;
	float tred = (float)_GetRValue(c1) / 255.0f;
	float tgreen = (float)_GetGValue(c1) / 255.0f;
	float tblue = (float)_GetBValue(c1) / 255.0f;
	float mred = (float)_GetRValue(c2) / 255.0f;
	float mgreen = (float)_GetGValue(c2) / 255.0f;
	float mblue = (float)_GetBValue(c2) / 255.0f;
	float bred = (float)_GetRValue(c3) / 255.0f;
	float bgreen = (float)_GetGValue(c3) / 255.0f;
	float bblue = (float)_GetBValue(c3) / 255.0f;

	LPBYTE lpData = m_lpData;
	LPBYTE lpZBufferData = m_lpZBuffer;
	DWORD dwTotalOffset = 0;
	if (minY >= 0)
	{
		dwTotalOffset = (m_iHeight - 1 - minY) * m_iWidth;
	}
	else
	{
		dwTotalOffset = (m_iHeight - 1) * m_iWidth;
	}
	lpData += dwTotalOffset;
	lpZBufferData += dwTotalOffset;

	float cxm, cxb, red1, green1, blue1, red2, green2, blue2;
	float xm, xb, reds, greens, blues, rede, greene, bluee;
	long x, xmin, xmax, oldxmin, oldxmax;
	float dxm = (middle.p.x - top.p.x) / (middle.p.y - top.p.y);
	float dxb = (bottom.p.x - top.p.x) / (bottom.p.y - top.p.y);
	float dzm = (1.0f / middle.p.z - 1.0f / top.p.z) / (middle.p.y - top.p.y);
	float dzb = (1.0f / bottom.p.z - 1.0f / top.p.z) / (bottom.p.y - top.p.y);
	x = topX;
	xm = top.p.x;
	xb = top.p.x;
	xmin = x;
	xmax = x;
	oldxmin = x;
	oldxmax = x;
	float zm = 1.0f / top.p.z;
	float zb = 1.0f / top.p.z;
	long midY = (long)middle.p.y;
	long midX = (long)(top.p.x + (middle.p.y - top.p.y) * dxb);
	float dum = (float)((float)uvm.x / (float)middle.p.z - (float)uvt.x / (float)top.p.z) / (float)(middle.p.y - top.p.y);
	float dvm = (float)((float)uvm.y / (float)middle.p.z - (float)uvt.y / (float)top.p.z) / (float)(middle.p.y - top.p.y);
	float dub = (float)((float)uvb.x / (float)bottom.p.z - (float)uvt.x / (float)top.p.z) / (float)(bottom.p.y - top.p.y);
	float dvb = (float)((float)uvb.y / (float)bottom.p.z - (float)uvt.y / (float)top.p.z) / (float)(bottom.p.y - top.p.y);
	float um = (float)uvt.x / (float)top.p.z;
	float vm = (float)uvt.y / (float)top.p.z;
	float ub = (float)uvt.x / (float)top.p.z;
	float vb = (float)uvt.y / (float)top.p.z;
	cxm = middle.p.y - top.p.y;
	cxb = bottom.p.y - top.p.y;
	if (cxm == 0.0f)
	{
		red1 = green1 = blue1 = 0.0f;
	}
	else
	{
		red1 = (mred / middle.p.z - tred / top.p.z) / cxm;
		green1 = (mgreen / middle.p.z - tgreen / top.p.z) / cxm;
		blue1 = (mblue / middle.p.z - tblue / top.p.z) / cxm;
	}
	if (cxb == 0.0f)
	{
		red2 = green2 = blue2 = 0.0f;
	}
	else
	{
		red2 = (bred / bottom.p.z - tred / top.p.z) / cxb;
		green2 = (bgreen / bottom.p.z - tgreen / top.p.z) / cxb;
		blue2 = (bblue / bottom.p.z - tblue / top.p.z) / cxb;
	}
	m_bInvert = FALSE;
	if (midX < (long)middle.p.x)
	{
		float umt = dum;
		float uvt = dvm;
		dum = dub;
		dvm = dvb;
		dub = umt;
		dvb = uvt;
		float rt = red2;
		float gt = green2;
		float bt = blue2;
		red2 = red1;
		green2 = green1;
		blue2 = blue1;
		red1 = rt;
		green1 = gt;
		blue1 = bt;
		float zmt = dzm;
		dzm = dzb;
		dzb = zmt;
		m_bInvert = TRUE;
	}
	reds = tred / top.p.z;
	greens = tgreen / top.p.z;
	blues = tblue / top.p.z;
	rede = reds;
	greene = greens;
	bluee = blues;

	if ((minY > m_iHeight - 1) || (maxY < 0))
	{
		return;
	}

	for (long y1 = minY; y1 <= maxY; y1++)
	{
		xmin = (long)xm;
		xmax = (long)xb;
		if (xmin > xmax)
		{
			long temp = xmin;
			xmin = xmax;
			xmax = temp;
		}

		if (xmin > oldxmax)
		{
			xmin = oldxmax + 1;
		}
		else if (xmax < oldxmin)
		{
			xmax = oldxmin - 1;
		}

		if ((y1 >= 0) && (y1 <= m_iHeight - 1))
		{
			DWORD dwSize = xmax - xmin + 1;
			LPBYTE lpTempData = lpData;
			LPBYTE lpTempZBufferData = lpZBufferData;

			if ((xmin <= m_iWidth - 1) && (xmax >= 0))
			{
				float fSize = (float)dwSize;
				float du = (ub - um) / fSize;
				float dv = (vb - vm) / fSize;
				float dz = (zb - zm) / fSize;
				float dr = (rede - reds) / fSize;
				float dg = (greene - greens) / fSize;
				float db = (bluee - blues) / fSize;
				float u = um;
				float v = vm;
				float z = zm;
				float r = reds;
				float g = greens;
				float b = blues;

				if (xmin < 0)
				{
					dwSize += xmin;
					u -= (xmin * du);
					v -= (xmin * dv);
					z -= (xmin * dz);
					r -= (xmin * dr);
					g -= (xmin * dg);
					b -= (xmin * db);
				}
				else
				{
					lpTempData += xmin;
					lpTempZBufferData += xmin;
				}

				if (xmax > m_iWidth - 1)
				{
					dwSize -= (xmax - m_iWidth + 1);
				}

				DWORD dwTextureSize = textureInfo.dwSize - 1;
				LPBYTE lpTextureData = textureInfo.lpTexture;
				DWORD dwShift = (dwTextureSize + 1) << 2;
				/*				for (int x=0; x<(int)dwSize; x++)
								{
									float z_inv = 1.0f / z;
									if (z_inv < (*lpTempZBufferData))
									{
										*lpTempZBufferData = z_inv;
										long ui = max(0, min((long)textureInfo.dwSize-1, (long)(u * z_inv)));
										long vi = max(0, min((long)textureInfo.dwSize-1, (long)(v * z_inv)));
										float f_red = r * z_inv;
										float f_green = g * z_inv;
										float f_blue = b * z_inv;
										_PIXEL srcPixel = lpTextureData[ui + vi*textureInfo.dwSize];
										BYTE red = (BYTE)((float)_GetRValue(srcPixel) * f_red);
										BYTE green = (BYTE)((float)_GetGValue(srcPixel) * f_green);
										BYTE blue = (BYTE)((float)_GetBValue(srcPixel) * f_blue);
										*lpTempData++ = _RGB(red, green, blue);
										u += du;
										v += dv;
										z += dz;
										r += dr;
										g += dg;
										b += db;
									}
								}*/

				float u_du1 = u + du;
				float u_du2 = u_du1 + du;
				float u_du3 = u_du2 + du;
				float u_du4 = 4 * du;
				float v_dv1 = v + dv;
				float v_dv2 = v_dv1 + dv;
				float v_dv3 = v_dv2 + dv;
				float v_dv4 = 4 * dv;
				float z_dz1 = z + dz;
				float z_dz2 = z_dz1 + dz;
				float z_dz3 = z_dz2 + dz;
				float z_dz4 = 4 * dz;
				float r_dr1 = r + dr;
				float r_dr2 = r_dr1 + dr;
				float r_dr3 = r_dr2 + dr;
				float r_dr4 = 4 * dr;
				float g_dg1 = g + dg;
				float g_dg2 = g_dg1 + dg;
				float g_dg3 = g_dg2 + dg;
				float g_dg4 = 4 * dg;
				float b_db1 = b + db;
				float b_db2 = b_db1 + db;
				float b_db3 = b_db2 + db;
				float b_db4 = 4 * db;
				__m128 simd_u = { u, u_du1, u_du2, u_du3 };
				__m128 simd_v = { v, v_dv1, v_dv2, v_dv3 };
				__m128 simd_z = { z, z_dz1, z_dz2, z_dz3 };
				__m128 simd_r = { r, r_dr1, r_dr2, r_dr3 };
				__m128 simd_g = { g, g_dg1, g_dg2, g_dg3 };
				__m128 simd_b = { b, b_db1, b_db2, b_db3 };
				__m128 simd_tu = { u, u_du1, u_du2, u_du3 };
				__m128 simd_tv = { v, v_dv1, v_dv2, v_dv3 };
				__m128 simd_tz = { z, z_dz1, z_dz2, z_dz3 };
				__m128 simd_tr = { r, r_dr1, r_dr2, r_dr3 };
				__m128 simd_tg = { g, g_dg1, g_dg2, g_dg3 };
				__m128 simd_tb = { b, b_db1, b_db2, b_db3 };
				__m128 simd_du = { u_du4, u_du4, u_du4, u_du4 };
				__m128 simd_dv = { v_dv4, v_dv4, v_dv4, v_dv4 };
				__m128 simd_dz = { z_dz4, z_dz4, z_dz4, z_dz4 };
				__m128 simd_dr = { r_dr4, r_dr4, r_dr4, r_dr4 };
				__m128 simd_dg = { g_dg4, g_dg4, g_dg4, g_dg4 };
				__m128 simd_db = { b_db4, b_db4, b_db4, b_db4 };
				long ui[4] = { 0, 0, 0, 0 };
				long vi[4] = { 0, 0, 0, 0 };
				long zi[4] = { 0, 0, 0, 0 };

				long dwFillRate = 0;
				long red, green, blue;
				_asm {
					mov edi, lpTempData
					mov esi, lpTempZBufferData
					mov ecx, dwSize
					movaps xmm0, simd_u
					movaps xmm1, simd_v
					movaps xmm2, simd_z
					movaps xmm3, simd_r
					movaps xmm4, simd_g
					movaps xmm5, simd_b
					loop1 : rcpps xmm6, xmm2
					mulps xmm0, xmm6
					mulps xmm1, xmm6
					mulps xmm3, xmm6
					mulps xmm4, xmm6
					mulps xmm5, xmm6
					movaps simd_u, xmm0
					movaps simd_v, xmm1
					movaps simd_z, xmm6
					movaps simd_r, xmm3
					movaps simd_g, xmm4
					movaps simd_b, xmm5
					movaps xmm0, simd_tu
					movaps xmm1, simd_tv
					movaps xmm2, simd_tz
					movaps xmm3, simd_tr
					movaps xmm4, simd_tg
					movaps xmm5, simd_tb
					addps xmm0, simd_du
					addps xmm1, simd_dv
					addps xmm2, simd_dz
					addps xmm3, simd_dr
					addps xmm4, simd_dg
					addps xmm5, simd_db
					movaps simd_tu, xmm0
					movaps simd_tv, xmm1
					movaps simd_tz, xmm2
					movaps simd_tr, xmm3
					movaps simd_tg, xmm4
					movaps simd_tb, xmm5
					fld simd_z[0]
					frndint
					fistp zi[0]
					mov eax, zi[0]
					cmp eax, [esi]
					jge next18
					mov[esi], eax
					mov eax, dwTextureSize
					fld simd_u[0]
					frndint
					fistp ui[0]
					cmp ui[0], 0
					jge next1
					mov ui[0], 0
					next1:				cmp ui[0], eax
					jle next2
					mov ui[0], eax
					next2 : fld simd_v[0]
					frndint
					fistp vi[0]
					cmp vi[0], 0
					jge next3
					mov vi[0], 0
					next3 : cmp vi[0], eax
					jle next4
					mov vi[0], eax
					next4 : mov eax, vi[0]
					mul dwShift
					shr eax, 2
					add eax, ui[0]
					shl eax, 2
					push esi
					mov esi, lpTextureData
					add esi, eax
					mov eax, [esi]
					and eax, 0x000000FF
					mov blue, eax
					fild blue
					fmul simd_b[0]
					fistp blue
					cmp blue, 0
					jge next22
					mov blue, 0
					next22:				cmp blue, 255
					jle next23
					mov blue, 255
					next23 : mov eax, [esi]
					and eax, 0x0000FF00
					shr eax, 8
					mov green, eax
					fild green
					fmul simd_g[0]
					fistp green
					cmp green, 0
					jge next24
					mov green, 0
					next24:				cmp green, 255
					jle next25
					mov green, 255
					next25 : mov eax, [esi]
					and eax, 0x00FF0000
					shr eax, 16
					mov red, eax
					fild red
					fmul simd_r[0]
					fistp red
					cmp red, 0
					jge next26
					mov red, 0
					next26:				cmp red, 255
					jle next27
					mov red, 255
					next27 : mov eax, red
					shl eax, 8
					or eax, green
					shl eax, 8
					or eax, blue
					mov[edi], eax
					inc dwFillRate
					pop esi
					next18 : add edi, 4
					add esi, 4
					dec ecx
					jz next17
					fld simd_z[4]
					frndint
					fistp zi[4]
					mov eax, zi[4]
					cmp eax, [esi]
					jge next19
					mov[esi], eax
					mov eax, dwTextureSize
					fld simd_u[4]
					frndint
					fistp ui[4]
					cmp ui[4], 0
					jge next5
					mov ui[4], 0
					next5:				cmp ui[4], eax
					jle next6
					mov ui[4], eax
					next6 : fld simd_v[4]
					frndint
					fistp vi[4]
					cmp vi[4], 0
					jge next7
					mov vi[4], 0
					next7 : cmp vi[4], eax
					jle next8
					mov vi[4], eax
					next8 : mov eax, vi[4]
					mul dwShift
					shr eax, 2
					add eax, ui[4]
					shl eax, 2
					push esi
					mov esi, lpTextureData
					add esi, eax
					mov eax, [esi]
					and eax, 0x000000FF
					mov blue, eax
					fild blue
					fmul simd_b[4]
					fistp blue
					cmp blue, 0
					jge next28
					mov blue, 0
					next28:				cmp blue, 255
					jle next29
					mov blue, 255
					next29 : mov eax, [esi]
					and eax, 0x0000FF00
					shr eax, 8
					mov green, eax
					fild green
					fmul simd_g[4]
					fistp green
					cmp green, 0
					jge next30
					mov green, 0
					next30:				cmp green, 255
					jle next31
					mov green, 255
					next31 : mov eax, [esi]
					and eax, 0x00FF0000
					shr eax, 16
					mov red, eax
					fild red
					fmul simd_r[4]
					fistp red
					cmp red, 0
					jge next32
					mov red, 0
					next32:				cmp red, 255
					jle next33
					mov red, 255
					next33 : mov eax, red
					shl eax, 8
					or eax, green
					shl eax, 8
					or eax, blue
					mov[edi], eax
					inc dwFillRate
					pop esi
					next19 : add edi, 4
					add esi, 4
					dec ecx
					jz next17
					fld simd_z[8]
					frndint
					fistp zi[8]
					mov eax, zi[8]
					cmp eax, [esi]
					jge next20
					mov[esi], eax
					mov eax, dwTextureSize
					fld simd_u[8]
					frndint
					fistp ui[8]
					cmp ui[8], 0
					jge next9
					mov ui[8], 0
					next9:				cmp ui[8], eax
					jle next10
					mov ui[8], eax
					next10 : fld simd_v[8]
					frndint
					fistp vi[8]
					cmp vi[8], 0
					jge next11
					mov vi[8], 0
					next11 : cmp vi[8], eax
					jle next12
					mov vi[8], eax
					next12 : mov eax, vi[8]
					mul dwShift
					shr eax, 2
					add eax, ui[8]
					shl eax, 2
					push esi
					mov esi, lpTextureData
					add esi, eax
					mov eax, [esi]
					and eax, 0x000000FF
					mov blue, eax
					fild blue
					fmul simd_b[8]
					fistp blue
					cmp blue, 0
					jge next34
					mov blue, 0
					next34:				cmp blue, 255
					jle next35
					mov blue, 255
					next35 : mov eax, [esi]
					and eax, 0x0000FF00
					shr eax, 8
					mov green, eax
					fild green
					fmul simd_g[8]
					fistp green
					cmp green, 0
					jge next36
					mov green, 0
					next36:				cmp green, 255
					jle next37
					mov green, 255
					next37 : mov eax, [esi]
					and eax, 0x00FF0000
					shr eax, 16
					mov red, eax
					fild red
					fmul simd_r[8]
					fistp red
					cmp red, 0
					jge next38
					mov red, 0
					next38:				cmp red, 255
					jle next39
					mov red, 255
					next39 : mov eax, red
					shl eax, 8
					or eax, green
					shl eax, 8
					or eax, blue
					mov[edi], eax
					inc dwFillRate
					pop esi
					next20 : add edi, 4
					add esi, 4
					dec ecx
					jz next17
					fld simd_z[12]
					frndint
					fistp zi[12]
					mov eax, zi[12]
					cmp eax, [esi]
					jge next21
					mov[esi], eax
					mov eax, dwTextureSize
					fld simd_u[12]
					frndint
					fistp ui[12]
					cmp ui[12], 0
					jge next13
					mov ui[12], 0
					next13:				cmp ui[12], eax
					jle next14
					mov ui[12], eax
					next14 : fld simd_v[12]
					frndint
					fistp vi[12]
					cmp vi[12], 0
					jge next15
					mov vi[12], 0
					next15 : cmp vi[12], eax
					jle next16
					mov vi[12], eax
					next16 : mov eax, vi[12]
					mul dwShift
					shr eax, 2
					add eax, ui[12]
					shl eax, 2
					push esi
					mov esi, lpTextureData
					add esi, eax
					mov eax, [esi]
					and eax, 0x000000FF
					mov blue, eax
					fild blue
					fmul simd_b[12]
					fistp blue
					cmp blue, 0
					jge next40
					mov blue, 0
					next40:				cmp blue, 255
					jle next41
					mov blue, 255
					next41 : mov eax, [esi]
					and eax, 0x0000FF00
					shr eax, 8
					mov green, eax
					fild green
					fmul simd_g[12]
					fistp green
					cmp green, 0
					jge next42
					mov green, 0
					next42:				cmp green, 255
					jle next43
					mov green, 255
					next43 : mov eax, [esi]
					and eax, 0x00FF0000
					shr eax, 16
					mov red, eax
					fild red
					fmul simd_r[12]
					fistp red
					cmp red, 0
					jge next44
					mov red, 0
					next44:				cmp red, 255
					jle next45
					mov red, 255
					next45 : mov eax, red
					shl eax, 8
					or eax, green
					shl eax, 8
					or eax, blue
					mov[edi], eax
					inc dwFillRate
					pop esi
					next21 : add edi, 4
					add esi, 4
					dec ecx
					jnz loop1
					next17 : nop
				}
				m_iFillRate += dwFillRate;
				m_iSkipRate += (dwSize - dwFillRate);
			}
			lpData -= m_iWidth;
			lpZBufferData -= m_iWidth;
		}

		oldxmin = xmin;
		oldxmax = xmax;

		xm += dxm;
		xb += dxb;
		zm += dzm;
		zb += dzb;
		um += dum;
		vm += dvm;
		ub += dub;
		vb += dvb;
		reds += red1;
		greens += green1;
		blues += blue1;
		rede += red2;
		greene += green2;
		bluee += blue2;
	}
}

void RasterizeTriangle(LPTRIANGLE lpTriangle)
{
	_POINT3DI p1, p2, p3;
	_PIXEL color, color1, color2, color3;
	_POINT3DI top, middle, bottom;
	_PIXEL c1, c2, c3;
	_POINT2D t1, t2, t3;
	_POINT2D uvt, uvm, uvb;

	if (lpTriangle == NULL)
	{
		return;
	}

	p1 = lpTriangle->p1;
	p2 = lpTriangle->p2;
	p3 = lpTriangle->p3;
	color1 = lpTriangle->color1;
	color2 = lpTriangle->color2;
	color3 = lpTriangle->color3;
	color = lpTriangle->color;
	t1 = lpTriangle->t1;
	t2 = lpTriangle->t2;
	t3 = lpTriangle->t3;
	uvt = lpTriangle->t1;
	uvm = lpTriangle->t2;
	uvb = lpTriangle->t3;

	if ((p1.y == p2.y) && (p2.y == p3.y))
	{
		return;
	}

	if (p1.y <= p2.y)
	{
		if (p2.y <= p3.y)
		{
			top = p1;
			c1 = color1;
			uvt = t1;

			middle = p2;
			c2 = color2;
			uvm = t2;

			bottom = p3;
			c3 = color3;
			uvb = t3;
		}
		else
		{
			if (p1.y <= p3.y)
			{
				top = p1;
				c1 = color1;
				uvt = t1;

				middle = p3;
				c2 = color3;
				uvm = t3;

				bottom = p2;
				c3 = color2;
				uvb = t2;
			}
			else
			{
				top = p3;
				c1 = color3;
				uvt = t3;

				middle = p1;
				c2 = color1;
				uvm = t1;

				bottom = p2;
				c3 = color2;
				uvb = t2;
			}
		}
	}
	else
	{
		if (p2.y >= p3.y)
		{
			top = p3;
			c1 = color3;
			uvt = t3;

			middle = p2;
			c2 = color2;
			uvm = t2;

			bottom = p1;
			c3 = color1;
			uvb = t1;
		}
		else
		{
			if (p1.y >= p3.y)
			{
				top = p2;
				c1 = color2;
				uvt = t2;

				middle = p3;
				c2 = color3;
				uvm = t3;

				bottom = p1;
				c3 = color1;
				uvb = t1;
			}
			else
			{
				top = p2;
				c1 = color2;
				uvt = t2;

				middle = p1;
				c2 = color1;
				uvm = t1;

				bottom = p3;
				c3 = color3;
				uvb = t3;
			}
		}
	}

	// Triangle invisible
	if ((top.y > m_iHeight - 1) || (bottom.y < 0) ||
		((top.x < 0) && (middle.x < 0) && (bottom.x < 0)) ||
		((top.x > m_iWidth - 1) && (middle.x > m_iWidth - 1) && (bottom.x > m_iWidth - 1)))
	{
		return;
	}

	// Bottom triangle
	if (top.y == middle.y)
	{
		// Wireframe triangle
		if (lpTriangle->shadingMode == _SHADING_MODE_NONE)
		{
			RasterizeLine(top, middle, lpTriangle->color);
			RasterizeLine(top, bottom, lpTriangle->color);
			RasterizeLine(middle, bottom, lpTriangle->color);
		}
		// Shaded triangle
		else
		{
			if ((lpTriangle->shadingMode == _SHADING_MODE_CONSTANT) || (lpTriangle->shadingMode == _SHADING_MODE_FLAT))
			{
				_VERTEX3D vt, vm, vb;
				vt.p.x = (float)top.x;
				vt.p.y = (float)top.y;
				vt.p.z = (float)top.z;
				vt.color = color;
				vm.p.x = (float)middle.x;
				vm.p.y = (float)middle.y;
				vm.p.z = (float)middle.z;
				vm.color = color;
				vb.p.x = (float)bottom.x;
				vb.p.y = (float)bottom.y;
				vb.p.z = (float)bottom.z;
				vb.color = color;
				Draw_Bottom_Triangle_Color(vt, vm, vb);
				return;
			}

			if (lpTriangle->shadingMode == _SHADING_MODE_GOURAUD)
			{
				_VERTEX3D vt, vm, vb;
				vt.p.x = (float)top.x;
				vt.p.y = (float)top.y;
				vt.p.z = (float)top.z;
				vt.color = c1;
				vm.p.x = (float)middle.x;
				vm.p.y = (float)middle.y;
				vm.p.z = (float)middle.z;
				vm.color = c2;
				vb.p.x = (float)bottom.x;
				vb.p.y = (float)bottom.y;
				vb.p.z = (float)bottom.z;
				vb.color = c3;
				m_bFollowInvert = FALSE;
				Draw_Bottom_Triangle_Gouraud(vt, vm, vb);
				return;
			}

			if ((lpTriangle->shadingMode == _SHADING_MODE_TEXTURE) || (lpTriangle->shadingMode == (_SHADING_MODE_TEXTURE | _SHADING_MODE_CONSTANT)))
			{
				_VERTEX3D vt, vm, vb;
				vt.p.x = (float)top.x;
				vt.p.y = (float)top.y;
				vt.p.z = (float)top.z;
				vt.uv = uvt;
				vm.p.x = (float)middle.x;
				vm.p.y = (float)middle.y;
				vm.p.z = (float)middle.z;
				vm.uv = uvm;
				vb.p.x = (float)bottom.x;
				vb.p.y = (float)bottom.y;
				vb.p.z = (float)bottom.z;
				vb.uv = uvb;
				m_bFollowInvert = FALSE;
				Draw_Bottom_Triangle_Texture(vt, vm, vb, lpTriangle->texture);
				return;
			}

			if (lpTriangle->shadingMode == (_SHADING_MODE_TEXTURE | _SHADING_MODE_FLAT))
			{
				_VERTEX3D vt, vm, vb;
				vt.p.x = (float)top.x;
				vt.p.y = (float)top.y;
				vt.p.z = (float)top.z;
				vt.uv = uvt;
				vt.color = color;
				vm.p.x = (float)middle.x;
				vm.p.y = (float)middle.y;
				vm.p.z = (float)middle.z;
				vm.uv = uvm;
				vm.color = color;
				vb.p.x = (float)bottom.x;
				vb.p.y = (float)bottom.y;
				vb.p.z = (float)bottom.z;
				vb.uv = uvb;
				vb.color = color;
				m_bFollowInvert = FALSE;
				Draw_Bottom_Triangle_Texture_Color(vt, vm, vb, lpTriangle->texture);
				return;
			}

			if (lpTriangle->shadingMode == (_SHADING_MODE_TEXTURE | _SHADING_MODE_GOURAUD))
			{
				_VERTEX3D vt, vm, vb;
				vt.p.x = (float)top.x;
				vt.p.y = (float)top.y;
				vt.p.z = (float)top.z;
				vt.uv = uvt;
				vt.color = c1;
				vm.p.x = (float)middle.x;
				vm.p.y = (float)middle.y;
				vm.p.z = (float)middle.z;
				vm.uv = uvm;
				vm.color = c2;
				vb.p.x = (float)bottom.x;
				vb.p.y = (float)bottom.y;
				vb.p.z = (float)bottom.z;
				vb.uv = uvb;
				vb.color = c3;
				m_bFollowInvert = FALSE;
				Draw_Bottom_Triangle_Texture_Gouraud(vt, vm, vb, lpTriangle->texture);
				return;
			}
		}
	}
	// Top triangle
	else if (middle.y == bottom.y)
	{
		// Wireframe triangle
		if (lpTriangle->shadingMode == _SHADING_MODE_NONE)
		{
			RasterizeLine(top, middle, lpTriangle->color);
			RasterizeLine(top, bottom, lpTriangle->color);
			RasterizeLine(middle, bottom, lpTriangle->color);
		}
		// Shaded triangle
		else
		{
			if ((lpTriangle->shadingMode == _SHADING_MODE_CONSTANT) || (lpTriangle->shadingMode == _SHADING_MODE_FLAT))
			{
				_VERTEX3D vt, vm, vb;
				vt.p.x = (float)top.x;
				vt.p.y = (float)top.y;
				vt.p.z = (float)top.z;
				vt.color = color;
				vm.p.x = (float)middle.x;
				vm.p.y = (float)middle.y;
				vm.p.z = (float)middle.z;
				vm.color = color;
				vb.p.x = (float)bottom.x;
				vb.p.y = (float)bottom.y;
				vb.p.z = (float)bottom.z;
				vb.color = color;
				Draw_Top_Triangle_Color(vt, vm, vb);
				return;
			}

			if (lpTriangle->shadingMode == _SHADING_MODE_GOURAUD)
			{
				_VERTEX3D vt, vm, vb;
				vt.p.x = (float)top.x;
				vt.p.y = (float)top.y;
				vt.p.z = (float)top.z;
				vt.color = c1;
				vm.p.x = (float)middle.x;
				vm.p.y = (float)middle.y;
				vm.p.z = (float)middle.z;
				vm.color = c2;
				vb.p.x = (float)bottom.x;
				vb.p.y = (float)bottom.y;
				vb.p.z = (float)bottom.z;
				vb.color = c3;
				Draw_Top_Triangle_Gouraud(vt, vm, vb);
				m_bFollowInvert = FALSE;
				return;
			}

			if ((lpTriangle->shadingMode == _SHADING_MODE_TEXTURE) || (lpTriangle->shadingMode == (_SHADING_MODE_TEXTURE | _SHADING_MODE_CONSTANT)))
			{
				_VERTEX3D vt, vm, vb;
				vt.p.x = (float)top.x;
				vt.p.y = (float)top.y;
				vt.p.z = (float)top.z;
				vt.uv = uvt;
				vm.p.x = (float)middle.x;
				vm.p.y = (float)middle.y;
				vm.p.z = (float)middle.z;
				vm.uv = uvm;
				vb.p.x = (float)bottom.x;
				vb.p.y = (float)bottom.y;
				vb.p.z = (float)bottom.z;
				vb.uv = uvb;
				Draw_Top_Triangle_Texture(vt, vm, vb, lpTriangle->texture);
				m_bFollowInvert = FALSE;
				return;
			}

			if (lpTriangle->shadingMode == (_SHADING_MODE_TEXTURE | _SHADING_MODE_FLAT))
			{
				_VERTEX3D vt, vm, vb;
				vt.p.x = (float)top.x;
				vt.p.y = (float)top.y;
				vt.p.z = (float)top.z;
				vt.uv = uvt;
				vt.color = color;
				vm.p.x = (float)middle.x;
				vm.p.y = (float)middle.y;
				vm.p.z = (float)middle.z;
				vm.uv = uvm;
				vm.color = color;
				vb.p.x = (float)bottom.x;
				vb.p.y = (float)bottom.y;
				vb.p.z = (float)bottom.z;
				vb.uv = uvb;
				vb.color = color;
				Draw_Top_Triangle_Texture_Color(vt, vm, vb, lpTriangle->texture);
				m_bFollowInvert = FALSE;
				return;
			}

			if (lpTriangle->shadingMode == (_SHADING_MODE_TEXTURE | _SHADING_MODE_GOURAUD))
			{
				_VERTEX3D vt, vm, vb;
				vt.p.x = (float)top.x;
				vt.p.y = (float)top.y;
				vt.p.z = (float)top.z;
				vt.uv = uvt;
				vt.color = c1;
				vm.p.x = (float)middle.x;
				vm.p.y = (float)middle.y;
				vm.p.z = (float)middle.z;
				vm.uv = uvm;
				vm.color = c2;
				vb.p.x = (float)bottom.x;
				vb.p.y = (float)bottom.y;
				vb.p.z = (float)bottom.z;
				vb.uv = uvb;
				vb.color = c3;
				Draw_Top_Triangle_Texture_Gouraud(vt, vm, vb, lpTriangle->texture);
				m_bFollowInvert = FALSE;
				return;
			}
		}
	}
	// General triangle
	else
	{
		// Wireframe triangle
		if (lpTriangle->shadingMode == _SHADING_MODE_NONE)
		{
			RasterizeLine(top, middle, lpTriangle->color);
			RasterizeLine(top, bottom, lpTriangle->color);
			RasterizeLine(middle, bottom, lpTriangle->color);
		}
		// Shaded triangle
		else
		{
			if ((lpTriangle->shadingMode == _SHADING_MODE_CONSTANT) || (lpTriangle->shadingMode == _SHADING_MODE_FLAT))
			{
				_VERTEX3D vt, vm, vb;
				vt.p.x = (float)top.x;
				vt.p.y = (float)top.y;
				vt.p.z = (float)top.z;
				vt.color = color;
				vm.p.x = (float)middle.x;
				vm.p.y = (float)middle.y;
				vm.p.z = (float)middle.z;
				vm.color = color;
				vb.p.x = (float)bottom.x;
				vb.p.y = (float)bottom.y;
				vb.p.z = (float)bottom.z;
				vb.color = color;
				Draw_Top_Triangle_Color(vt, vm, vb);
				Draw_Bottom_Triangle_Color(vt, vm, vb);
				return;
			}

			if (lpTriangle->shadingMode == _SHADING_MODE_GOURAUD)
			{
				_VERTEX3D vt, vm, vb;
				vt.p.x = (float)top.x;
				vt.p.y = (float)top.y;
				vt.p.z = (float)top.z;
				vt.color = c1;
				vm.p.x = (float)middle.x;
				vm.p.y = (float)middle.y;
				vm.p.z = (float)middle.z;
				vm.color = c2;
				vb.p.x = (float)bottom.x;
				vb.p.y = (float)bottom.y;
				vb.p.z = (float)bottom.z;
				vb.color = c3;
				Draw_Top_Triangle_Gouraud(vt, vm, vb);
				m_bFollowInvert = TRUE;
				Draw_Bottom_Triangle_Gouraud(vt, vm, vb);
				m_bFollowInvert = FALSE;
				return;
			}

			if ((lpTriangle->shadingMode == _SHADING_MODE_TEXTURE) || (lpTriangle->shadingMode == (_SHADING_MODE_TEXTURE | _SHADING_MODE_CONSTANT)))
			{
				_VERTEX3D vt, vm, vb;
				vt.p.x = (float)top.x;
				vt.p.y = (float)top.y;
				vt.p.z = (float)top.z;
				vt.uv = uvt;
				vm.p.x = (float)middle.x;
				vm.p.y = (float)middle.y;
				vm.p.z = (float)middle.z;
				vm.uv = uvm;
				vb.p.x = (float)bottom.x;
				vb.p.y = (float)bottom.y;
				vb.p.z = (float)bottom.z;
				vb.uv = uvb;
				Draw_Top_Triangle_Texture(vt, vm, vb, lpTriangle->texture);
				m_bFollowInvert = TRUE;
				Draw_Bottom_Triangle_Texture(vt, vm, vb, lpTriangle->texture);
				m_bFollowInvert = FALSE;
				return;
			}

			if (lpTriangle->shadingMode == (_SHADING_MODE_TEXTURE | _SHADING_MODE_FLAT))
			{
				_VERTEX3D vt, vm, vb;
				vt.p.x = (float)top.x;
				vt.p.y = (float)top.y;
				vt.p.z = (float)top.z;
				vt.uv = uvt;
				vt.color = color;
				vm.p.x = (float)middle.x;
				vm.p.y = (float)middle.y;
				vm.p.z = (float)middle.z;
				vm.uv = uvm;
				vm.color = color;
				vb.p.x = (float)bottom.x;
				vb.p.y = (float)bottom.y;
				vb.p.z = (float)bottom.z;
				vb.uv = uvb;
				vb.color = color;
				Draw_Top_Triangle_Texture_Color(vt, vm, vb, lpTriangle->texture);
				m_bFollowInvert = TRUE;
				Draw_Bottom_Triangle_Texture_Color(vt, vm, vb, lpTriangle->texture);
				m_bFollowInvert = FALSE;
				return;
			}

			if (lpTriangle->shadingMode == (_SHADING_MODE_TEXTURE | _SHADING_MODE_GOURAUD))
			{
				_VERTEX3D vt, vm, vb;
				vt.p.x = (float)top.x;
				vt.p.y = (float)top.y;
				vt.p.z = (float)top.z;
				vt.uv = uvt;
				vt.color = c1;
				vm.p.x = (float)middle.x;
				vm.p.y = (float)middle.y;
				vm.p.z = (float)middle.z;
				vm.uv = uvm;
				vm.color = c2;
				vb.p.x = (float)bottom.x;
				vb.p.y = (float)bottom.y;
				vb.p.z = (float)bottom.z;
				vb.uv = uvb;
				vb.color = c3;
				Draw_Top_Triangle_Texture_Gouraud(vt, vm, vb, lpTriangle->texture);
				m_bFollowInvert = TRUE;
				Draw_Bottom_Triangle_Texture_Gouraud(vt, vm, vb, lpTriangle->texture);
				m_bFollowInvert = FALSE;
				return;
			}
		}
	}
}

void GameRender_old::DrawTriangleInProjectionSpace_B6253(x_DWORD* vertex1, x_DWORD* vertex2, x_DWORD* vertex3)//sub_B6253 - 297253
{
	TRIANGLE lpTriangle;
	//lpTriangle->shadingMode = _SHADING_MODE_TEXTURE;
	lpTriangle.shadingMode = _SHADING_MODE_TEXTURE;
	//lpTriangle.shadingMode = _SHADING_MODE_NONE;
	lpTriangle.p1.x = vertex1[0];
	lpTriangle.p1.y = vertex1[1];
	lpTriangle.p1.z = 1;
	lpTriangle.t1.x = vertex1[2] / 8192;
	lpTriangle.t1.y = vertex1[3] / 8192;

	lpTriangle.p2.x = vertex2[0];
	lpTriangle.p2.y = vertex2[1];
	lpTriangle.p2.z = 1;
	lpTriangle.t2.x = vertex2[2] / 8192;
	lpTriangle.t2.y = vertex2[3] / 8192;

	lpTriangle.p3.x = vertex3[0];
	lpTriangle.p3.y = vertex3[1];
	lpTriangle.p3.z = 1;
	lpTriangle.t3.x = vertex3[2] / 8192;
	lpTriangle.t3.y = vertex3[3] / 8192;

	BYTE* textdata = (BYTE*)malloc(16*16+1000000);
	memset(textdata,50, 16 * 16 + 1000000);

	/*lpTriangle.p1.x = 0;
	lpTriangle.p1.y = 0;
	lpTriangle.p2.x = 100;
	lpTriangle.p2.y = 0;
	lpTriangle.p3.x = 0;
	lpTriangle.p3.y = 100;*/

	_TEXTUREINFO texture;
	texture.dwTextureOffset = 0;
	texture.dwPitch = 16;
	texture.dwSize = 16;
	texture.filter = _TEXTURE_FILTERING_NONE;
	texture.lpTexture = textdata;
	lpTriangle.texture = texture;

	RasterizeTriangle(&lpTriangle);
	free(textdata);
}
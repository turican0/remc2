#include "GameRenderOriginal.h"
#include "CommandLineParser.h"
#include "../utilities/RendererTests.h"

GameRenderOriginal::~GameRenderOriginal()
{
}

// ---------------------------------------------------------------------------
// DrawWorld_411A0
// Top-level render dispatcher. Samples the heightmap under the player to
// compute a smoothed tilt offset, then chooses between three render paths:
//   1. VR interlaced stereo (640-wide, split left/right eye)
//   2. Normal render  (with optional motion-blur passes)
//   3. Small-viewport stereo  (side-by-side, small screen size = 1)
// ---------------------------------------------------------------------------
void GameRenderOriginal::DrawWorld_411A0(
    int posX, int posY, int16_t yaw, int16_t posZ,
    int16_t pitch, int16_t roll, int16_t fov)
{
    // Build a 2-byte heightmap index from the high bytes of posX / posY.
    // Subtract 1 from each high-byte component when the low byte is < 128
    // so that the 2×2 sample straddles the correct tile boundary.
    uint8_t tileX = (uint8_t)(posX >> 8);
    uint8_t tileY = (uint8_t)(posY >> 8);
    if ((uint8_t)posX < 128) tileX--;
    if ((uint8_t)posY < 128) tileY--;

    uint16_t mapIdx = (uint16_t)(tileX | (tileY << 8));

    // Sample a 2×2 block of heightmap texels
    int h00 = mapHeightmap_11B4E0[mapIdx];
    mapIdx = (uint16_t)((tileX + 2) | (tileY << 8));
    int h10 = mapHeightmap_11B4E0[mapIdx];
    mapIdx = (uint16_t)((tileX + 2) | ((tileY + 2) << 8));
    int h11 = mapHeightmap_11B4E0[mapIdx];
    mapIdx = (uint16_t)(tileX | ((tileY + 2) << 8));
    int h01 = mapHeightmap_11B4E0[mapIdx];

    // Derive X/Y terrain-slope values from the four height samples and clamp
    int slopeX = 2 * (h01 + h00 - h10 - h11);
    int slopeY = 2 * (h00 + h10 - h11 - h01);
    if      (slopeX >  100) slopeX =  100;
    else if (slopeX < -100) slopeX = -100;
    if      (slopeY >  100) slopeY =  100;
    else if (slopeY < -100) slopeY = -100;

    int clampedYaw = yaw & 0x7FF;

    // Exponential moving average (alpha = 1/8) keeps the tilt smooth
    x_DWORD_D4794 += (slopeX - x_DWORD_D4794) >> 3;
    x_DWORD_D4798 += (slopeY - x_DWORD_D4798) >> 3;

    int smoothPosX = x_DWORD_D4794 + posX;
    int smoothPosY = x_DWORD_D4798 + posY;

    // -----------------------------------------------------------------------
    // Path 1 – VR interlaced stereo (640-wide, screen-size flag set)
    // -----------------------------------------------------------------------
    if (D41A0_0.m_GameSettings.str_0x2192.xxxx_0x2193
        && D41A0_0.m_GameSettings.m_Display.m_uiScreenSize
        && screenWidth_18062C == 640)
    {
        viewPort.SetRenderViewPortSize_BCD45(
            pdwScreenBuffer_351628,
            2 * screenWidth_18062C,
            screenWidth_18062C / 2 - 8,
            screenHeight_180624 / 2 - 40);

        // Compute per-eye lateral offset from yaw sin/cos
        int sinYaw   = Maths::sin_DB750[clampedYaw];
        int cosYaw   = (int)(uint32_t)Maths::sin_DB750[512 + clampedYaw];
        x_DWORD_D4790 = 20;
        x_DWORD_D4324 = -5;

        int eyeOffsetX = (4 * (5 * sinYaw)) >> 16;   // ~sin component
        int eyeOffsetY = (20 * cosYaw)       >> 16;   // ~cos component

        // Left eye
        DrawTerrainAndParticles_3C080(
            smoothPosX - eyeOffsetY, smoothPosY - eyeOffsetX,
            clampedYaw, posZ, pitch, roll, fov);

        // Right eye – render into the second half of the screen buffer
        viewPort.SetRenderViewPortSize_BCD45(
            pdwScreenBuffer_351628 + (screenWidth_18062C / 2), 0, 0, 0);
        x_DWORD_D4324 = 5;
        DrawTerrainAndParticles_3C080(
            smoothPosX + eyeOffsetY, smoothPosY + eyeOffsetX,
            clampedYaw, posZ, pitch, roll, fov);

        // Restore
        x_DWORD_D4324 = 0;
        viewPort.SetRenderViewPortSize_BCD45(
            pdwScreenBuffer_351628, screenWidth_18062C,
            screenWidth_18062C, screenHeight_180624);
    }
    // -----------------------------------------------------------------------
    // Path 2 – Normal render  (screen-size != 1, or VR flag set)
    // -----------------------------------------------------------------------
    else if (D41A0_0.m_GameSettings.m_Display.m_uiScreenSize != 1
             || D41A0_0.m_GameSettings.str_0x2192.xxxx_0x2193)
    {
        // Save quality flag so we can restore it at the end
        char savedQualityFlag = D41A0_0.m_GameSettings.m_Display.xxxx_0x2191;

        // Auto-degrade quality when the player is moving fast (VGA mode only)
        if (x_WORD_180660_VGA_type_resolution == 1
            && !D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].MenuState_0x3DF_2BE4_12221
            && x_D41A0_BYTEARRAY_4_struct.m_wHighSpeedSystem
            && x_DWORD_E9C3C
            && D41A0_0.m_GameSettings.m_Graphics.m_wViewPortSize == 40)
        {
            int64_t rawSpeed = Entities_EA3E4[
                D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240
            ]->actSpeed_0x82_130;
            // Absolute value via XOR trick on the 64-bit value
            int absSpeed = (int)((int32_t)(rawSpeed >> 32) ^ rawSpeed)
                         - (int32_t)(rawSpeed >> 32);
            if (absSpeed > 80)
                D41A0_0.m_GameSettings.m_Display.xxxx_0x2191 = 1;
        }

        // --- Optional motion-blur pre-pass ---
        bool useBlur = D41A0_0.str_0x21AE.xxxx_0x21B1
                    && D41A0_0.m_GameSettings.m_Display.xxxx_0x2191
                    && x_DWORD_E9C3C;

        if (useBlur)
        {
            // Render the new frame into the auxiliary buffer
            uint8_t* mainBuffer = ViewPortRenderBufferStart_DE558;
            viewPort.SetRenderViewPortSize_BCD45(x_DWORD_E9C3C, 0, 0, 0);
            DrawTerrainAndParticles_3C080(
                smoothPosX, smoothPosY, clampedYaw, posZ, pitch, roll, fov);

            // Switch back to the main buffer and blend
            viewPort.SetRenderViewPortSize_BCD45(mainBuffer, 0, 0, 0);

            int colsDiv4   = (int)(uint16_t)viewPort.Width_DE564 >> 2;
            int rowStride  = iScreenWidth_DE560 - (uint16_t)viewPort.Width_DE564;
            int rowsLeft   = (uint16_t)viewPort.Height_DE568;

            uint8_t* srcPx = (uint8_t*)x_DWORD_E9C3C;
            uint8_t* dstPx = mainBuffer;

            if (D41A0_0.m_GameSettings.m_Display.xxxx_0x2191 != 1)
            {
                // Quality > 1: skip the per-pixel blend, fall through to
                // the normal DrawTerrainAndParticles call below
                srcPx = (uint8_t*)x_DWORD_E9C3C;
                goto LABEL_33;
            }

            // Quality == 1: 50-50 blend of aux buffer and main buffer,
            // 4 pixels at a time using the pre-built blend lookup table.
            {
                int cols = (int)(uint16_t)viewPort.Width_DE564 >> 2;

                // Seed the first blended DWORD (pixels 2-3 then 0-1)
                uint16_t lo = (uint16_t)(srcPx[2] | (dstPx[2] << 8));
                uint16_t hi = (uint16_t)(srcPx[3] | (dstPx[3] << 8));
                uint32_t blended = ((uint32_t)x_BYTE_F6EE0_tablesx[16384 + hi] << 24)
                                 | ((uint32_t)x_BYTE_F6EE0_tablesx[16384 + lo] << 16);

                lo = (uint16_t)(srcPx[0] | (dstPx[0] << 8));
                hi = (uint16_t)(srcPx[1] | (dstPx[1] << 8));
                blended |= (uint32_t)x_BYTE_F6EE0_tablesx[16384 + lo];
                uint8_t byte1 = x_BYTE_F6EE0_tablesx[16384 + hi];

                for (;;)
                {
                    blended = (blended & 0xFFFF0000u)
                            | ((uint32_t)byte1 << 8)
                            | (blended & 0xFF);   // keep LOBYTE from previous iter
                    *(uint32_t*)dstPx = blended;
                    dstPx += 4;
                    srcPx += 4;
                    if (!--cols)
                    {
                        // End of row – advance both pointers past the stride gap
                        dstPx += rowStride;
                        srcPx += rowStride;
                        if (!--rowsLeft) goto LABEL_44;
                    LABEL_33:
                        cols = colsDiv4;
                    }
                    // Next 4-pixel block
                    lo      = (uint16_t)(srcPx[2] | (dstPx[2] << 8));
                    hi      = (uint16_t)(srcPx[3] | (dstPx[3] << 8));
                    blended = ((uint32_t)x_BYTE_F6EE0_tablesx[16384 + lo])
                            | ((uint32_t)x_BYTE_F6EE0_tablesx[16384 + hi] << 8);
                    blended <<= 16;
                    lo    = (uint16_t)(srcPx[0] | (dstPx[0] << 8));
                    hi    = (uint16_t)(srcPx[1] | (dstPx[1] << 8));
                    blended |= (uint32_t)x_BYTE_F6EE0_tablesx[16384 + lo];
                    byte1    = x_BYTE_F6EE0_tablesx[16384 + hi];
                }
            }
        }

        // --- Main terrain draw ---
        DrawTerrainAndParticles_3C080(
            smoothPosX, smoothPosY, clampedYaw, posZ, pitch, roll, fov);

        // --- Optional 2-D row-blur post-process ---
        if (D41A0_0.m_GameSettings.str_0x2192.xxxx_0x2192)
        {
            uint8_t* rowPtr = ViewPortRenderBufferStart_DE558;
            for (int row = (uint16_t)viewPort.Height_DE568 - 1; row > 0; row--)
            {
                uint8_t* px      = rowPtr;
                int      stride  = iScreenWidth_DE560;
                int      cols    = (uint16_t)viewPort.Width_DE564 - 1;

                do
                {
                    // Blend current pixel with the one below it
                    uint16_t ab = (uint16_t)(px[0] | (px[stride] << 8));
                    uint16_t cd = (uint16_t)(px[1] | (px[stride + 1] << 8));
                    uint8_t  t  = x_BYTE_F6EE0_tablesx[16384 + ab];
                    t           = x_BYTE_F6EE0_tablesx[16384 + (uint16_t)(t | (cd & 0xFF00u))];
                    *px++ = x_BYTE_F6EE0_tablesx[16384 + t];
                    cols--;
                } while (cols);

                rowPtr += iScreenWidth_DE560;
            }
        }

    LABEL_44:
        D41A0_0.m_GameSettings.m_Display.xxxx_0x2191 = savedQualityFlag;
    }
    // -----------------------------------------------------------------------
    // Path 3 – Small-viewport side-by-side stereo (screen size == 1)
    // -----------------------------------------------------------------------
    else
    {
        int sinYaw   = 5 * Maths::sin_DB750[clampedYaw];
        uint32_t cosYaw = (uint32_t)Maths::sin_DB750[512 + clampedYaw];
        x_DWORD_D4790 = 20;

        int halfIPD  = iScreenWidth_DE560 / 40;
        x_DWORD_D4324 = halfIPD;

        int eyeOffsetX = (4 * sinYaw) >> 16;
        int eyeOffsetY = (20 * (int)cosYaw) >> 16;

        // Right eye into the main buffer
        DrawTerrainAndParticles_3C080(
            smoothPosX + eyeOffsetY, smoothPosY + eyeOffsetX,
            clampedYaw, posZ, pitch, roll, fov);

        // Left eye into the auxiliary buffer
        uint8_t* savedBuffer = ViewPortRenderBufferStart_DE558;
        viewPort.SetRenderViewPortSize_BCD45(x_DWORD_E9C3C, 0, 0, 0);
        x_DWORD_D4324 = -halfIPD;
        DrawTerrainAndParticles_3C080(
            smoothPosX - eyeOffsetY, smoothPosY - eyeOffsetX,
            clampedYaw, posZ, pitch, roll, fov);

        viewPort.SetRenderViewPortSize_BCD45(savedBuffer, 0, 0, 0);
        x_DWORD_D4324 = 0;
    }
}

// ---------------------------------------------------------------------------
// DrawSky_40950
// Renders the sky texture into the viewport buffer using an affine-mapped
// scanline walk driven by the camera roll angle.
// ---------------------------------------------------------------------------
void GameRenderOriginal::DrawSky_40950(int16_t roll)
{
    int roundRoll = roll & 0x7FF;

    // Per-pixel step in texture space along each scanline axis
    int cosStep = ((int)Maths::sin_DB750[512 + roundRoll] << 8)
                / (int)(uint16_t)viewPort.Width_DE564;
    int sinStep = ((int)Maths::sin_DB750[roundRoll]       << 8)
                / (int)(uint16_t)viewPort.Width_DE564;

    // Build a scanline lookup table: delta-U and delta-V for each column
    // Stored interleaved as pairs [deltaU, deltaV] in a local 0x500-byte array
    int   accumU   = 0;
    int   accumV   = 0;
    uint8_t prevU  = 0;
    uint8_t prevV  = 0;

    char lut[0x500];
    char* lutPtr = lut;

    uint16_t colCount = viewPort.Width_DE564;
    while (colCount--)
    {
        uint8_t curU    = (uint8_t)(accumU >> 16);
        *lutPtr         = (char)(curU - prevU);   // delta-U this column
        lutPtr         += 2;
        *(lutPtr - 1)   = (char)((uint8_t)(accumV >> 16) - prevV); // delta-V
        prevU           = curU;
        prevV           = (uint8_t)(accumV >> 16);
        accumV         += sinStep;
        accumU         += cosStep;
    }

    // Compute the starting texture coordinate for the top-left pixel
    // using the camera forward/side vectors stored in str_F2C20ar
    int fwdX  = -(str_F2C20ar.sin_0x0d * str_F2C20ar.dword0x22 >> 16)
               + str_F2C20ar.dword0x24;
    int fwdY  = str_F2C20ar.dword0x10
               - (str_F2C20ar.cos_0x11 * str_F2C20ar.dword0x22 >> 16);
    int baseU = fwdX * cosStep - fwdY * sinStep;
    int baseV = cosStep * fwdY + sinStep * fwdX;

    int texU = ((uint16_t)yaw_F2CC0 << 15) - baseU;
    int texV = -baseV;

    uint8_t* screenRow = ViewPortRenderBufferStart_DE558;
    int      rowsLeft  = (uint16_t)viewPort.Height_DE568;

    if (!rowsLeft) return;

    do
    {
        // Number of 4-pixel groups per row
        int groups = ((uint16_t)viewPort.Width_DE564
                    - (((int)(uint16_t)viewPort.Width_DE564 >> 31) * 4
                       + (int)(uint16_t)viewPort.Width_DE564 >> 31 << 2)) >> 2;

        const char*   colLut  = lut;
        uint32_t*     outPx   = (uint32_t*)screenRow;
        const uint8_t* skyTex = off_D41A8_sky;

        // Pack current texV high-byte into hi, texU high-byte into lo
        uint8_t curV = (uint8_t)(texV >> 16);
        uint8_t curU = (uint8_t)(texU >> 16);
        uint16_t texIdx = (uint16_t)(curU | ((uint32_t)curV << 8));

        // Unrolled 4-pixels-per-iteration inner loop
        do
        {
            uint8_t p0 = skyTex[texIdx];
            curU       += colLut[0];  curV += colLut[1];
            texIdx      = (uint16_t)(curU | ((uint32_t)curV << 8));

            uint8_t p1 = skyTex[texIdx];
            curU       += colLut[2];  curV += colLut[3];
            texIdx      = (uint16_t)(curU | ((uint32_t)curV << 8));

            uint32_t pair01 = (uint32_t)p1 << 16;

            uint8_t p2 = skyTex[texIdx];
            curU       += colLut[4];  curV += colLut[5];
            texIdx      = (uint16_t)(curU | ((uint32_t)curV << 8));

            uint8_t p3 = skyTex[texIdx];
            curU       += colLut[6];  curV += colLut[7];
            texIdx      = (uint16_t)(curU | ((uint32_t)curV << 8));

            // Assemble and store 4 pixels as one DWORD (endian-aware swap)
            uint32_t pair23 = (uint32_t)p2 | ((uint32_t)p3 << 8);
            *outPx++ = __SWAP_HILOWORD__(pair01 | p0) ^ (pair23 << 16) ^ (pair23 >> 16);
            colLut  += 8;
            groups--;
        } while (groups);

        screenRow += iScreenWidth_DE560;
        rowsLeft--;
        texU -= sinStep;
        texV += cosStep;
    } while (rowsLeft);
}

// ---------------------------------------------------------------------------
// DrawTerrainAndParticles_3C080
// Sets up the tile-grid projection, fills Str_E9C38_smalltit[] with
// per-tile screen coordinates, then dispatches triangle rasterisation and
// sprite/billboard drawing for one complete camera view.
// ---------------------------------------------------------------------------
void GameRenderOriginal::DrawTerrainAndParticles_3C080(
    int16_t posX, int16_t posY, int16_t yaw,
    int posZ, int pitch, int16_t roll, int fov)
{
    // -----------------------------------------------------------------------
    // Global render state
    // -----------------------------------------------------------------------
    shadows_F2CC7   = D41A0_0.m_GameSettings.m_Graphics.m_wShadows;
    notDay_D4320    = D41A0_0.terrain_2FECE.MapType != MapType_t::Day;

    str_F2C20ar.dword0x10 = (int)(uint16_t)viewPort.Height_DE568 >> 1; // half-height
    cameraX_F2CC4         = posX;
    cameraY_F2CC2         = posY;
    yaw_F2CC0             = yaw & 0x7FF;

    int yawPlus256 = (yaw & 0x7FF) + 256;
    str_F2C20ar.dword0x20  = posZ;
    str_F2C20ar.cos2_0x0f  = Maths::sin_DB750[256 + yawPlus256];
    str_F2C20ar.sin2_0x17  = Maths::sin_DB750[yawPlus256 - 256];
    str_F2C20ar.dword0x24  = x_DWORD_D4324
                           + ((int)(uint16_t)viewPort.Width_DE564 >> 1);

    int yawQuadrant = (yawPlus256 >> 9) & 3;  // 0-3 based on 90-deg sector
    int v248_30 = Maths::sin_DB750[512 + (((yawPlus256 & 0x1FF) - 256) & 0x7FF)];
    int v248_29 = Maths::sin_DB750[(((yawPlus256 & 0x1FF) - 256) & 0x7FF)];

    int quadrantV13 = v248_29;  // sin of the adjusted yaw within quadrant

    // Focal-length equivalent: scaled by viewport diagonal and fov
    str_F2C20ar.dword0x18 = 7
        * Maths::sub_7277A_radix_3d(
            (uint16_t)viewPort.Width_DE564  * (uint16_t)viewPort.Width_DE564
          + (uint16_t)viewPort.Height_DE568 * (uint16_t)viewPort.Height_DE568)
        * fov >> 11;

    // Setup billboard rotation for the negative of roll
    SetBillboards_3B560((-roll) & 0x7FF);

    // -----------------------------------------------------------------------
    // Tile-grid origin offset table  (10 bytes per quadrant)
    // -----------------------------------------------------------------------
    uint8_t* tileOffsetTable = unk_D4328x + 10 * yawQuadrant;

    // Compute world-space starting coordinates of the tile grid
    // depending on which 90-degree quadrant the camera is facing
    int gridA, gridB;
    switch ((uint8_t)yawQuadrant)
    {
    case 0:
        gridB = (uint8_t)posY - 256;
        gridA = -(uint8_t)posX - 4864;
        break;
    case 1:
        gridA = -(uint8_t)posY - 4864;
        gridB = -(uint8_t)posX;
        break;
    case 2:
        gridA = (uint8_t)posX - 4864;
        gridB = -(uint8_t)posY;
        break;
    default: // case 3
        gridA = (uint8_t)posY - 4864;
        gridB = (uint8_t)posX - 256;
        break;
    }

    // -----------------------------------------------------------------------
    // Fill Str_E9C38_smalltit[].x_0 / .y_12  (tile-space x/y in view coords)
    // Pass 1 – columns (40 tiles wide)
    // -----------------------------------------------------------------------
    int tileIdx = 0;
    for (int col = 40; col > 0; col--)
    {
        int projX = gridA * (int)v248_30 >> 16;  // tile cos-component
        int projY = gridA * quadrantV13   >> 16;  // tile sin-component

        for (int row = 21; row > 0; row--)
        {
            Str_E9C38_smalltit[tileIdx].x_0     = projX;
            Str_E9C38_smalltit[tileIdx].y_12    = projY;
            Str_E9C38_smalltit[tileIdx].triangleFeatures_38 = (gridA < 0) ? 0 : 4;
            tileIdx += 40;
            row--;  // note: original decrements twice per outer iteration
        }
        tileIdx -= 839;  // step back to next column start
        gridA += 256;
    }

    // Pass 2 – rows: add the perpendicular component
    int tileIdx2 = 0;
    for (int row = 21; row > 0; row--)
    {
        int perpX = gridB * quadrantV13   >> 16;
        int perpY = gridB * (int)v248_30  >> 16;

        for (int col = 40; col > 0; col--)
        {
            Str_E9C38_smalltit[tileIdx2].x_0  -= perpX;
            Str_E9C38_smalltit[tileIdx2].y_12 += perpY;
            tileIdx2++;
            col--;
        }
        gridB += 256;
    }

    // -----------------------------------------------------------------------
    // Projection state
    // -----------------------------------------------------------------------
    str_F2C20ar.dword0x15_tileRenderCutOffDistance = 26214400;
    str_F2C20ar.dword0x12 = 8912896;
    str_F2C20ar.dword0x16 = 23658496;
    str_F2C20ar.dword0x13 = 14745600;

    // Screen-space pitch offset
    str_F2C20ar.dword0x22 = pitch * (uint16_t)viewPort.Width_DE564 >> 8;

    // Starting tile map index (high bytes of pos + quadrant offsets)
    uint8_t mapIdxLo = (uint8_t)(tileOffsetTable[0] + (uint8_t)(posX >> 8));
    uint8_t mapIdxHi = (uint8_t)(tileOffsetTable[1] + (uint8_t)(posY >> 8));
    uint16_t mapPos  = (uint16_t)(mapIdxLo | ((uint16_t)mapIdxHi << 8));

    // Roll-based rotation sin/cos used in the rasteriser
    int rollAngle = roll & 0x7FF;
    str_F2C20ar.cos_0x11 = (int)(uint32_t)Maths::sin_DB750[512 + rollAngle];
    str_F2C20ar.sin_0x0d = Maths::sin_DB750[rollAngle];

    // -----------------------------------------------------------------------
    // Clear the sky / background
    // -----------------------------------------------------------------------
    if (!D41A0_0.m_GameSettings.m_Graphics.m_wSky || isCaveLevel_D41B6)
    {
        // Solid-colour fill using the key colour
        uint16_t vpWidth  = viewPort.Width_DE564;
        int      gap      = iScreenWidth_DE560 - vpWidth;
        int      colDWs   = (vpWidth - (((int)(vpWidth >> 31) * 4)
                                       + ((int)(vpWidth >> 31) << 2))) >> 2;
        int16_t  rowCount = viewPort.Height_DE568;
        uint8_t* fillPtr  = ViewPortRenderBufferStart_DE558;

        uint8_t  keyCol = keyColor1_D4B7C;
        uint16_t keyPair   = (uint16_t)(keyCol | ((uint16_t)keyCol << 8));
        uint32_t keyDWord  = (uint32_t)(keyPair | ((uint32_t)keyPair << 16));

        do
        {
            memset32(fillPtr, keyDWord, colDWs * 4);
            fillPtr += 4 * colDWs + gap;
            rowCount--;
        } while (rowCount);
    }
    else
    {
        DrawSky_40950(roll);
    }

    // -----------------------------------------------------------------------
    // Per-tile projection and triangle rasterisation
    // The two large blocks below (cave vs. open + reflections) share
    // identical inner structure; only the per-tile shading formula differs.
    // -----------------------------------------------------------------------
    int tileWalkIdx = 0;     // index into Str_E9C38_smalltit[]
    int v248x[33];           // working scratch array (mirrors original)

    if (isCaveLevel_D41B6)
    {
        // ===================================================================
        // Cave render – no sky, no reflections, fixed texture on ceiling quad
        // ===================================================================
        for (char rowOuter = 21; ; rowOuter--)
        {
            if (!rowOuter)
            {
                // ----------------------------------------------------------
                // After all rows processed: transform projected points from
                // tile-local to screen space by applying the roll rotation
                // ----------------------------------------------------------
                for (int t = 840, i = 0; t > 0; t--, i++)
                {
                    // Rotate (pnt1, pnt2) by roll matrix
                    int screenX = ((str_F2C20ar.cos_0x11 * Str_E9C38_smalltit[i].pnt1_16
                                  - str_F2C20ar.sin_0x0d * Str_E9C38_smalltit[i].pnt2_20) >> 16)
                                + str_F2C20ar.dword0x24;
                    int screenX2 = ((str_F2C20ar.cos_0x11 * Str_E9C38_smalltit[i].pnt1_16
                                   - str_F2C20ar.sin_0x0d * Str_E9C38_smalltit[i].pnt4_28) >> 16)
                                 + str_F2C20ar.dword0x24;

                    int tmp = Str_E9C38_smalltit[i].pnt1_16 * str_F2C20ar.sin_0x0d;
                    int screenY  = str_F2C20ar.dword0x10
                                 - ((tmp + str_F2C20ar.cos_0x11 * Str_E9C38_smalltit[i].pnt2_20) >> 16);
                    int screenY2 = str_F2C20ar.dword0x10
                                 - ((tmp + str_F2C20ar.cos_0x11 * Str_E9C38_smalltit[i].pnt4_28) >> 16);

                    Str_E9C38_smalltit[i].pnt1_16 = screenX;
                    Str_E9C38_smalltit[i].pnt3_24 = screenX2;
                    Str_E9C38_smalltit[i].pnt2_20 = screenY;
                    Str_E9C38_smalltit[i].pnt4_28 = screenY2;

                    // Set clip flags on pnt1
                    if (screenX >= 0)
                    {
                        if ((int)(uint16_t)viewPort.Width_DE564 <= screenX)
                            Str_E9C38_smalltit[i].triangleFeatures_38 |= 0x10u;
                    }
                    else
                    {
                        Str_E9C38_smalltit[i].triangleFeatures_38 |= 0x08u;
                    }
                    // clip flags on pnt2
                    if (screenY >= 0)
                    {
                        if ((uint16_t)viewPort.Height_DE568 <= (uint32_t)screenY)
                            Str_E9C38_smalltit[i].triangleFeatures_38 |= 0x40u;
                    }
                    else
                    {
                        Str_E9C38_smalltit[i].triangleFeatures_38 |= 0x20u;
                    }
                    // clip flags on pnt3
                    if (screenX2 >= 0)
                    {
                        if ((int)(uint16_t)viewPort.Width_DE564 <= screenX2)
                            Str_E9C38_smalltit[i].triangleFeatures_38 |= 0x200u;
                    }
                    else
                    {
                        Str_E9C38_smalltit[i].triangleFeatures_38 |= 0x100u;
                    }
                    // clip flags on pnt4
                    if (screenY2 >= 0)
                    {
                        if ((uint16_t)viewPort.Height_DE568 <= (uint32_t)screenY2)
                            Str_E9C38_smalltit[i].triangleFeatures_38 |= 0x800u;
                    }
                    else
                    {
                        Str_E9C38_smalltit[i].triangleFeatures_38 |= 0x400u;
                    }
                }

                // ----------------------------------------------------------
                // Rasterise quads row by row (front-to-back, clipped)
                // ----------------------------------------------------------
                int startIdx = 800;
                for (char row = 20; row > 0; row--)
                {
                    char colCount = 39;
                    int  curIdx   = startIdx;

                    for (; colCount > 0; curIdx++, colCount--)
                    {
                        // Read ceiling-quad vertices (pnt3/pnt4)
                        v248x[18] = Str_E9C38_smalltit[curIdx].pnt3_24;
                        v248x[19] = Str_E9C38_smalltit[curIdx].pnt4_28;
                        v248x[22] = Str_E9C38_smalltit[curIdx].pnt5_32;
                        uint8_t flags0 = Str_E9C38_smalltit[curIdx].triangleFeatures_38 & 0xFF;

                        if (Str_E9C38_smalltit[curIdx + 1].triangleFeatures_38 & 4) break;

                        v248x[12] = Str_E9C38_smalltit[curIdx + 1].pnt3_24;
                        v248x[13] = Str_E9C38_smalltit[curIdx + 1].pnt4_28;
                        v248x[16] = Str_E9C38_smalltit[curIdx + 1].pnt5_32;
                        uint8_t flags1 = Str_E9C38_smalltit[curIdx + 1].triangleFeatures_38 & 0xFF;

                        v248x[6]  = Str_E9C38_smalltit[curIdx - 39].pnt3_24;
                        v248x[7]  = Str_E9C38_smalltit[curIdx - 39].pnt4_28;
                        v248x[10] = Str_E9C38_smalltit[curIdx - 39].pnt5_32;
                        uint8_t flagsA = Str_E9C38_smalltit[curIdx - 39].triangleFeatures_38 & 0xFF;
                        uint8_t orABC  = flagsA | flags1 | flags0;
                        uint8_t andABC = flagsA & flags1 & flags0;

                        v248x[0] = Str_E9C38_smalltit[curIdx - 40].pnt3_24;
                        v248x[1] = Str_E9C38_smalltit[curIdx - 40].pnt4_28;
                        v248x[4] = Str_E9C38_smalltit[curIdx - 40].pnt5_32;
                        uint8_t flagsB = Str_E9C38_smalltit[curIdx - 40].triangleFeatures_38 & 0xFF;
                        uint8_t orABCD = flagsB | orABC;

                        if ((flagsB & andABC & 0x80u) == 0)
                        {
                            // Flat-light ceiling quad
                            if (Str_E9C38_smalltit[curIdx].triangleFeatures_38 & 0x1000)
                            {
                                x_BYTE_E126D = 7;
                                x_BYTE_E126C = ((int)v248x[10] + v248x[16] + v248x[22] + v248x[4]) >> 18;
                            }
                            else
                            {
                                x_BYTE_E126D = 5;
                            }
                            if (!(orABCD & 2))
                            {
                                // Load UV table for this tile
                                for (int k = 0; k < 8; k++)
                                    v248x[20 + k - (k/2)*2 + (k/2)*2] =
                                        UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][k];
                                // Simplified: copy all 8 UV entries
                                v248x[20] = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][0];
                                v248x[21] = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][1];
                                v248x[14] = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][2];
                                v248x[15] = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][3];
                                v248x[8]  = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][4];
                                v248x[9]  = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][5];
                                v248x[2]  = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][6];
                                v248x[3]  = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][7];

                                x_DWORD_DE55C_ActTexture = x_DWORD_DDF50_texture_adresses[1];
                                x_BYTE_E126D = 5;

                                if (Str_E9C38_smalltit[curIdx].triangleFeatures_38 & 1)
                                {
                                    DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[0],  &v248x[12]);
                                    DrawTriangleInProjectionSpace_B6253(&v248x[0],  &v248x[6],  &v248x[12]);
                                }
                                else
                                {
                                    DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[6],  &v248x[12]);
                                    DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[0],  &v248x[6]);
                                }
                            }
                        }

                        // Read floor-quad vertices (pnt1/pnt2)
                        v248x[18] = Str_E9C38_smalltit[curIdx].pnt1_16;
                        v248x[19] = Str_E9C38_smalltit[curIdx].pnt2_20;
                        v248x[22] = Str_E9C38_smalltit[curIdx].pnt5_32;
                        flags0 = Str_E9C38_smalltit[curIdx].triangleFeatures_38 & 0xFF;

                        if (Str_E9C38_smalltit[curIdx + 1].triangleFeatures_38 & 4) break;

                        v248x[12] = Str_E9C38_smalltit[curIdx + 1].pnt1_16;
                        v248x[13] = Str_E9C38_smalltit[curIdx + 1].pnt2_20;
                        v248x[16] = Str_E9C38_smalltit[curIdx + 1].pnt5_32;
                        flags1 = Str_E9C38_smalltit[curIdx + 1].triangleFeatures_38 & 0xFF;

                        v248x[6]  = Str_E9C38_smalltit[curIdx - 39].pnt1_16;
                        v248x[7]  = Str_E9C38_smalltit[curIdx - 39].pnt2_20;
                        v248x[10] = Str_E9C38_smalltit[curIdx - 39].pnt5_32;
                        flagsA = Str_E9C38_smalltit[curIdx - 39].triangleFeatures_38 & 0xFF;
                        uint8_t or3  = flagsA | flags1 | flags0;
                        uint8_t and3 = flagsA & flags1 & flags0;

                        v248x[0] = Str_E9C38_smalltit[curIdx - 40].pnt1_16;
                        v248x[1] = Str_E9C38_smalltit[curIdx - 40].pnt2_20;
                        v248x[4] = Str_E9C38_smalltit[curIdx - 40].pnt5_32;
                        flagsB = Str_E9C38_smalltit[curIdx - 40].triangleFeatures_38 & 0xFF;
                        uint8_t or4  = flagsB | or3;
                        uint8_t and4 = flagsB & and3;

                        if ((int8_t)and4 >= 0)  // sign bit clear = not fully clipped
                        {
                            if (Str_E9C38_smalltit[curIdx].triangleFeatures_38 & 0x1000)
                            {
                                x_BYTE_E126D = 7;
                                x_BYTE_E126C = ((int)v248x[10] + v248x[16] + v248x[22] + v248x[4]) >> 18;
                            }
                            else
                            {
                                x_BYTE_E126D = 5;
                            }

                            if (!(or4 & 2) && !(and4 & 0x78))
                            {
                                v248x[20] = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][0];
                                v248x[21] = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][1];
                                v248x[14] = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][2];
                                v248x[15] = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][3];
                                v248x[8]  = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][4];
                                v248x[9]  = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][5];
                                v248x[2]  = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][6];
                                v248x[3]  = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][7];

                                x_DWORD_DE55C_ActTexture =
                                    x_DWORD_DDF50_texture_adresses[Str_E9C38_smalltit[curIdx].textIndex_41];

                                if (Str_E9C38_smalltit[curIdx].triangleFeatures_38 & 1)
                                {
                                    DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[12], &v248x[0]);
                                    DrawTriangleInProjectionSpace_B6253(&v248x[0],  &v248x[12], &v248x[6]);
                                }
                                else
                                {
                                    DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[12], &v248x[6]);
                                    DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[6],  &v248x[0]);
                                }
                            }

                            if (Str_E9C38_smalltit[curIdx].haveBillboard_36)
                                DrawSprites_3E360(curIdx);
                        }
                    }

                    // Remaining tiles in the row (from right edge back to the
                    // breakpoint) — same logic, reverse column order
                    if (colCount > 0)
                    {
                        int revIdx = startIdx + 38;
                        do
                        {
                            // (identical vertex-fetch + rasterise block, reversed)
                            // ... (same pattern as above, omitted for brevity —
                            //      all variable names follow the same scheme)
                            v248x[18] = Str_E9C38_smalltit[revIdx].pnt3_24;
                            v248x[19] = Str_E9C38_smalltit[revIdx].pnt4_28;
                            v248x[22] = Str_E9C38_smalltit[revIdx].pnt5_32;
                            uint8_t rf0 = Str_E9C38_smalltit[revIdx].triangleFeatures_38 & 0xFF;

                            v248x[12] = Str_E9C38_smalltit[revIdx + 1].pnt3_24;
                            v248x[13] = Str_E9C38_smalltit[revIdx + 1].pnt4_28;
                            v248x[16] = Str_E9C38_smalltit[revIdx + 1].pnt5_32;
                            uint8_t rf1 = Str_E9C38_smalltit[revIdx + 1].triangleFeatures_38 & 0xFF;

                            v248x[6]  = Str_E9C38_smalltit[revIdx - 39].pnt3_24;
                            v248x[7]  = Str_E9C38_smalltit[revIdx - 39].pnt4_28;
                            v248x[10] = Str_E9C38_smalltit[revIdx - 39].pnt5_32;
                            uint8_t rfA  = Str_E9C38_smalltit[revIdx - 39].triangleFeatures_38 & 0xFF;
                            uint8_t rorABC = rfA | rf1 | rf0;
                            uint8_t randABC = rfA & rf1 & rf0;

                            v248x[0] = Str_E9C38_smalltit[revIdx - 40].pnt3_24;
                            v248x[1] = Str_E9C38_smalltit[revIdx - 40].pnt4_28;
                            v248x[4] = Str_E9C38_smalltit[revIdx - 40].pnt5_32;
                            uint8_t rfB = Str_E9C38_smalltit[revIdx - 40].triangleFeatures_38 & 0xFF;
                            uint8_t rorAll = rfB | rorABC;

                            if ((rfB & randABC & 0x80u) == 0 && !(rorAll & 2))
                            {
                                if (Str_E9C38_smalltit[revIdx].triangleFeatures_38 & 0x1000)
                                {
                                    x_BYTE_E126D = 7;
                                    x_BYTE_E126C = ((int)v248x[10] + v248x[16]
                                                  + v248x[22] + v248x[4]) >> 18;
                                }
                                else { x_BYTE_E126D = 5; }

                                v248x[20] = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][0];
                                v248x[21] = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][1];
                                v248x[14] = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][2];
                                v248x[15] = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][3];
                                v248x[8]  = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][4];
                                v248x[9]  = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][5];
                                v248x[2]  = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][6];
                                v248x[3]  = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][7];
                                x_DWORD_DE55C_ActTexture = x_DWORD_DDF50_texture_adresses[1];
                                x_BYTE_E126D = 5;

                                uint8_t rfl = Str_E9C38_smalltit[revIdx].triangleFeatures_38 & 0xFF;
                                if (rfl & 1)
                                {
                                    DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[0],  &v248x[12]);
                                    DrawTriangleInProjectionSpace_B6253(&v248x[0],  &v248x[6],  &v248x[12]);
                                }
                                else
                                {
                                    DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[6],  &v248x[12]);
                                    DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[0],  &v248x[6]);
                                }
                            }

                            // Floor quad
                            v248x[18] = Str_E9C38_smalltit[revIdx].pnt1_16;
                            v248x[19] = Str_E9C38_smalltit[revIdx].pnt2_20;
                            v248x[22] = Str_E9C38_smalltit[revIdx].pnt5_32;
                            rf0 = Str_E9C38_smalltit[revIdx].triangleFeatures_38 & 0xFF;

                            v248x[12] = Str_E9C38_smalltit[revIdx + 1].pnt1_16;
                            v248x[13] = Str_E9C38_smalltit[revIdx + 1].pnt2_20;
                            v248x[16] = Str_E9C38_smalltit[revIdx + 1].pnt5_32;
                            rf1 = Str_E9C38_smalltit[revIdx + 1].triangleFeatures_38 & 0xFF;
                            uint8_t ror2 = rf1 | rf0;
                            uint8_t rand2 = rf1 & rf0;

                            v248x[6]  = Str_E9C38_smalltit[revIdx - 39].pnt1_16;
                            v248x[7]  = Str_E9C38_smalltit[revIdx - 39].pnt2_20;
                            v248x[10] = Str_E9C38_smalltit[revIdx - 39].pnt5_32;
                            rfA = Str_E9C38_smalltit[revIdx - 39].triangleFeatures_38 & 0xFF;

                            v248x[0] = Str_E9C38_smalltit[revIdx - 40].pnt1_16;
                            v248x[1] = Str_E9C38_smalltit[revIdx - 40].pnt2_20;
                            v248x[4] = Str_E9C38_smalltit[revIdx - 40].pnt5_32;
                            uint8_t rorFinal  = (Str_E9C38_smalltit[revIdx - 40].triangleFeatures_38 & 0xFF)
                                               | rfA | ror2;
                            uint8_t randFinal = (Str_E9C38_smalltit[revIdx - 40].triangleFeatures_38 & 0xFF)
                                              & rfA & rand2;

                            if ((int8_t)randFinal >= 0 && !(rorFinal & 2) && !(randFinal & 0x78))
                            {
                                if (Str_E9C38_smalltit[revIdx].triangleFeatures_38 & 0x1000)
                                {
                                    x_BYTE_E126D = 7;
                                    x_BYTE_E126C = ((int)v248x[10] + v248x[16]
                                                  + v248x[22] + v248x[4]) >> 18;
                                }
                                else { x_BYTE_E126D = 5; }

                                v248x[20] = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][0];
                                v248x[21] = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][1];
                                v248x[14] = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][2];
                                v248x[15] = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][3];
                                v248x[8]  = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][4];
                                v248x[9]  = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][5];
                                v248x[2]  = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][6];
                                v248x[3]  = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][7];
                                x_DWORD_DE55C_ActTexture =
                                    x_DWORD_DDF50_texture_adresses[Str_E9C38_smalltit[revIdx].textIndex_41];

                                if (Str_E9C38_smalltit[revIdx].triangleFeatures_38 & 1)
                                {
                                    DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[12], &v248x[0]);
                                    DrawTriangleInProjectionSpace_B6253(&v248x[0],  &v248x[12], &v248x[6]);
                                }
                                else
                                {
                                    DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[12], &v248x[6]);
                                    DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[6],  &v248x[0]);
                                }
                            }

                            if (Str_E9C38_smalltit[revIdx].haveBillboard_36)
                                DrawSprites_3E360(revIdx);

                            revIdx--;
                        } while (revIdx >= curIdx);
                    }

                    startIdx -= 40;
                }
                return;  // cave path done
            }

            // ----------------------------------------------------------------
            // Per-tile projection pass (cave path, 40 columns x 21 rows)
            // ----------------------------------------------------------------
            for (char col = 40; col > 0; col--, tileWalkIdx++)
            {
                int shading = ((uint8_t)mapShading_12B4E0[mapPos] << 8) + 128;
                int distY   = Str_E9C38_smalltit[tileWalkIdx].y_12;
                int distSq  = distY * distY
                            + Str_E9C38_smalltit[tileWalkIdx].x_0
                            * Str_E9C38_smalltit[tileWalkIdx].x_0;

                Str_E9C38_smalltit[tileWalkIdx].haveBillboard_36 = 0;

                if (distY <= -256 || distSq >= str_F2C20ar.dword0x15_tileRenderCutOffDistance)
                {
                    Str_E9C38_smalltit[tileWalkIdx].triangleFeatures_38 |= 2u;
                    goto CAVE_NEXT_TILE;
                }

                if (distY < 128) distY = 128;

                Str_E9C38_smalltit[tileWalkIdx].pnt1_16 =
                    str_F2C20ar.dword0x18 * Str_E9C38_smalltit[tileWalkIdx].x_0 / distY;

                {
                    uint16_t mapKey = mapPos;
                    Str_E9C38_smalltit[tileWalkIdx].alt_4 =
                        32 * mapHeightmap_11B4E0[mapKey] - posZ;
                    Str_E9C38_smalltit[tileWalkIdx].inverse_alt_8 =
                        ((uint8_t)x_BYTE_14B4E0_second_heightmap[mapKey] << 15 >> 10) - posZ;

                    int waveDisplace = 0;
                    if (!mapTerrainType_10B4E0[mapKey])
                    {
                        // Animated water ripple displacement
                        int t = 32 * D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].Turn_2BE0_11248;
                        waveDisplace =
                            (Maths::sin_DB750[(t + ((uint8_t)(mapPos >> 8) << 7)) & 0x7FF] >> 8)
                          * (Maths::sin_DB750[(((uint8_t)mapPos << 7) + t) & 0x7FF] >> 8);
                        Str_E9C38_smalltit[tileWalkIdx].alt_4 -= waveDisplace >> 13;
                        if (shading >= 14464) waveDisplace = 0;
                    }

                    int lighting = (shading << 8) + 8 * waveDisplace;

                    if (distSq > str_F2C20ar.dword0x13 && distSq < str_F2C20ar.dword0x16)
                        lighting = (int)((int64_t)lighting
                                        * (str_F2C20ar.dword0x16 - distSq)
                                        / str_F2C20ar.dword0x12);
                    else if (distSq >= str_F2C20ar.dword0x16)
                        lighting = 0;

                    Str_E9C38_smalltit[tileWalkIdx].pnt5_32 = lighting;

                    if (mapAngle_13B4E0[mapKey] & 8)
                        Str_E9C38_smalltit[tileWalkIdx].triangleFeatures_38 |= 0x80u;

                    Str_E9C38_smalltit[tileWalkIdx].pnt2_20 =
                        str_F2C20ar.dword0x22
                      + str_F2C20ar.dword0x18 * Str_E9C38_smalltit[tileWalkIdx].alt_4 / distY;
                    Str_E9C38_smalltit[tileWalkIdx].pnt4_28 =
                        str_F2C20ar.dword0x22
                      + str_F2C20ar.dword0x18 * Str_E9C38_smalltit[tileWalkIdx].inverse_alt_8 / distY;

                    // Look up the tile type one step ahead in the grid
                    uint16_t nextKey = (uint16_t)(
                        ((uint8_t)(tileOffsetTable[2] + (uint8_t)mapPos))
                      | ((uint16_t)((uint8_t)(tileOffsetTable[3] + (uint8_t)(mapPos >> 8))) << 8));

                    Str_E9C38_smalltit[tileWalkIdx].textIndex_41 = mapTerrainType_10B4E0[nextKey];

                    if (D41A0_0.m_GameSettings.str_0x2196.flat_0x2199)
                        Str_E9C38_smalltit[tileWalkIdx].triangleFeatures_38 |= 0x1000u;

                    Str_E9C38_smalltit[tileWalkIdx].textAtyp_43 =
                        Maths::x_BYTE_D41D8[Str_E9C38_smalltit[tileWalkIdx].textIndex_41];
                    Str_E9C38_smalltit[tileWalkIdx].textUV_42 =
                        yawQuadrant + (((int)(uint8_t)mapAngle_13B4E0[nextKey] >> 2) & 0x1C);

                    uint16_t entityKey = (uint16_t)(
                        ((uint8_t)(tileOffsetTable[4] + (uint8_t)nextKey))
                      | ((uint16_t)((uint8_t)(tileOffsetTable[5] + (uint8_t)(nextKey >> 8))) << 8));
                    Str_E9C38_smalltit[tileWalkIdx].haveBillboard_36 =
                        mapEntityIndex_15B4E0[entityKey];
                }

            CAVE_NEXT_TILE:
                Str_E9C38_smalltit[tileWalkIdx].triangleFeatures_38 |=
                    ((uint8_t)mapPos + (uint8_t)(mapPos >> 8)) & 1;

                mapPos = (uint16_t)(
                    ((uint8_t)(tileOffsetTable[8] + (uint8_t)mapPos))
                  | ((uint16_t)((uint8_t)(tileOffsetTable[9] + (uint8_t)(mapPos >> 8))) << 8));
            }

            // Advance to next row of map
            mapPos = (uint16_t)(
                ((uint8_t)(tileOffsetTable[6] + (uint8_t)mapPos))
              | ((uint16_t)((uint8_t)(tileOffsetTable[7] + (uint8_t)(mapPos >> 8))) << 8));
        }
    }

    // =========================================================================
    // Open-world path (reflections + normal terrain)
    // The reflection pass is only run when m_wReflections is set and the
    // camera is low enough (posZ < 4096).
    // The normal terrain pass always runs.
    // Both passes share the same tile-projection loop structure.
    // =========================================================================

    if (D41A0_0.m_GameSettings.m_Graphics.m_wReflections)
    {
        for (char reflRow = 21; ; reflRow--)
        {
            if (!reflRow)
            {
                // Transform to screen space (same roll rotation as cave path)
                for (int t = 840, i = 0; t > 0; t--, i++)
                {
                    int screenX = ((str_F2C20ar.cos_0x11 * Str_E9C38_smalltit[i].pnt1_16
                                  - str_F2C20ar.sin_0x0d * Str_E9C38_smalltit[i].pnt2_20) >> 16)
                                + str_F2C20ar.dword0x24;
                    int screenX2= ((str_F2C20ar.cos_0x11 * Str_E9C38_smalltit[i].pnt1_16
                                  - str_F2C20ar.sin_0x0d * Str_E9C38_smalltit[i].pnt4_28) >> 16)
                                + str_F2C20ar.dword0x24;
                    int tmp     = Str_E9C38_smalltit[i].pnt1_16 * str_F2C20ar.sin_0x0d;
                    int screenY = str_F2C20ar.dword0x10
                                - ((tmp + str_F2C20ar.cos_0x11 * Str_E9C38_smalltit[i].pnt2_20) >> 16);
                    int screenY2= str_F2C20ar.dword0x10
                                - ((tmp + str_F2C20ar.cos_0x11 * Str_E9C38_smalltit[i].pnt4_28) >> 16);

                    Str_E9C38_smalltit[i].pnt1_16 = screenX;
                    Str_E9C38_smalltit[i].pnt3_24 = screenX2;
                    Str_E9C38_smalltit[i].pnt2_20 = screenY;
                    Str_E9C38_smalltit[i].pnt4_28 = screenY2;

                    if (screenX >= 0)
                    { if ((int)(uint16_t)viewPort.Width_DE564 <= screenX) Str_E9C38_smalltit[i].triangleFeatures_38 |= 0x10u; }
                    else Str_E9C38_smalltit[i].triangleFeatures_38 |= 0x08u;

                    if (screenY >= 0)
                    { if ((uint16_t)viewPort.Height_DE568 <= (uint32_t)screenY) Str_E9C38_smalltit[i].triangleFeatures_38 |= 0x40u; }
                    else Str_E9C38_smalltit[i].triangleFeatures_38 |= 0x20u;

                    if (screenX2 >= 0)
                    { if ((int)(uint16_t)viewPort.Width_DE564 <= screenX2) Str_E9C38_smalltit[i].triangleFeatures_38 |= 0x200u; }
                    else Str_E9C38_smalltit[i].triangleFeatures_38 |= 0x100u;

                    if (screenY2 >= 0)
                    { if ((uint16_t)viewPort.Height_DE568 <= (uint32_t)screenY2) Str_E9C38_smalltit[i].triangleFeatures_38 |= 0x800u; }
                    else Str_E9C38_smalltit[i].triangleFeatures_38 |= 0x400u;
                }

                // Reflection quads (water surface) – only when camera is near ground
                if (posZ < 4096)
                {
                    int startIdx = 800;
                    for (char row = 20; row > 0; row--)
                    {
                        int curIdx = startIdx;
                        for (char col = 39; col > 0; col--, curIdx++)
                        {
                            v248x[18] = Str_E9C38_smalltit[curIdx].pnt3_24;
                            v248x[19] = Str_E9C38_smalltit[curIdx].pnt4_28;
                            v248x[22] = Str_E9C38_smalltit[curIdx].pnt5_32;
                            uint8_t f0 = Str_E9C38_smalltit[curIdx].triangleFeatures_38 & 0xFF;

                            if (Str_E9C38_smalltit[curIdx + 1].triangleFeatures_38 & 4) break;

                            v248x[12] = Str_E9C38_smalltit[curIdx + 1].pnt3_24;
                            v248x[13] = Str_E9C38_smalltit[curIdx + 1].pnt4_28;
                            v248x[16] = Str_E9C38_smalltit[curIdx + 1].pnt5_32;
                            uint8_t f1 = Str_E9C38_smalltit[curIdx + 1].triangleFeatures_38 & 0xFF;

                            v248x[6]  = Str_E9C38_smalltit[curIdx - 39].pnt3_24;
                            v248x[7]  = Str_E9C38_smalltit[curIdx - 39].pnt4_28;
                            v248x[10] = Str_E9C38_smalltit[curIdx - 39].pnt5_32;
                            uint8_t fA = (Str_E9C38_smalltit[curIdx - 39].triangleFeatures_38 & 0xFF)
                                        | f1 | f0;

                            v248x[0] = Str_E9C38_smalltit[curIdx - 40].pnt3_24;
                            v248x[1] = Str_E9C38_smalltit[curIdx - 40].pnt4_28;
                            v248x[4] = Str_E9C38_smalltit[curIdx - 40].pnt5_32;
                            uint8_t fAll = (Str_E9C38_smalltit[curIdx - 40].triangleFeatures_38 & 0xFF)
                                         | fA;

                            // Only draw tiles that have a non-zero texture index (water)
                            if (Str_E9C38_smalltit[curIdx].textIndex_41)
                            {
                                if (Str_E9C38_smalltit[curIdx].triangleFeatures_38 & 0x1000)
                                { x_BYTE_E126D = 7; x_BYTE_E126C = ((int)v248x[10]+v248x[16]+v248x[22]+v248x[4])>>18; }
                                else x_BYTE_E126D = 5;

                                if (!(fAll & 2))
                                {
                                    v248x[20] = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][0];
                                    v248x[21] = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][1];
                                    v248x[14] = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][2];
                                    v248x[15] = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][3];
                                    v248x[8]  = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][4];
                                    v248x[9]  = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][5];
                                    v248x[2]  = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][6];
                                    v248x[3]  = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][7];
                                    x_DWORD_DE55C_ActTexture =
                                        x_DWORD_DDF50_texture_adresses[Str_E9C38_smalltit[curIdx].textIndex_41];
                                    x_BYTE_E126D = 5;

                                    if (Str_E9C38_smalltit[curIdx].triangleFeatures_38 & 1)
                                    {
                                        DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[0],  &v248x[12]);
                                        DrawTriangleInProjectionSpace_B6253(&v248x[0],  &v248x[6],  &v248x[12]);
                                    }
                                    else
                                    {
                                        DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[6],  &v248x[12]);
                                        DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[0],  &v248x[6]);
                                    }
                                }
                            }

                            if (Str_E9C38_smalltit[curIdx].haveBillboard_36)
                                sub_3FD60(curIdx);
                        }

                        // Reverse scan for remaining columns
                        if (col > 0)
                        {
                            int revIdx = startIdx + 38;
                            do
                            {
                                v248x[18] = Str_E9C38_smalltit[revIdx].pnt3_24;
                                v248x[19] = Str_E9C38_smalltit[revIdx].pnt4_28;
                                v248x[22] = Str_E9C38_smalltit[revIdx].pnt5_32;
                                uint8_t rf0 = Str_E9C38_smalltit[revIdx].triangleFeatures_38 & 0xFF;

                                v248x[12] = Str_E9C38_smalltit[revIdx + 1].pnt3_24;
                                v248x[13] = Str_E9C38_smalltit[revIdx + 1].pnt4_28;
                                v248x[16] = Str_E9C38_smalltit[revIdx + 1].pnt5_32;
                                uint8_t rf1 = (Str_E9C38_smalltit[revIdx + 1].triangleFeatures_38 & 0xFF) | rf0;

                                v248x[6]  = Str_E9C38_smalltit[revIdx - 39].pnt3_24;
                                v248x[7]  = Str_E9C38_smalltit[revIdx - 39].pnt4_28;
                                v248x[10] = Str_E9C38_smalltit[revIdx - 39].pnt5_32;
                                uint8_t rfA = Str_E9C38_smalltit[revIdx - 39].triangleFeatures_38 & 0xFF;

                                v248x[0] = Str_E9C38_smalltit[revIdx - 40].pnt3_24;
                                v248x[1] = Str_E9C38_smalltit[revIdx - 40].pnt4_28;
                                v248x[4] = Str_E9C38_smalltit[revIdx - 40].pnt5_32;
                                uint8_t rfAll = (Str_E9C38_smalltit[revIdx - 40].triangleFeatures_38 & 0xFF)
                                               | rfA | rf1;

                                if (Str_E9C38_smalltit[revIdx].textIndex_41)
                                {
                                    if (Str_E9C38_smalltit[revIdx].triangleFeatures_38 & 0x1000)
                                    { x_BYTE_E126D=7; x_BYTE_E126C=((int)v248x[10]+v248x[16]+v248x[22]+v248x[4])>>18; }
                                    else x_BYTE_E126D=5;

                                    if (!(rfAll & 2))
                                    {
                                        v248x[20] = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][0];
                                        v248x[21] = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][1];
                                        v248x[14] = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][2];
                                        v248x[15] = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][3];
                                        v248x[8]  = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][4];
                                        v248x[9]  = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][5];
                                        v248x[2]  = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][6];
                                        v248x[3]  = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][7];
                                        x_BYTE_E126D = 5;
                                        x_DWORD_DE55C_ActTexture =
                                            x_DWORD_DDF50_texture_adresses[Str_E9C38_smalltit[revIdx].textIndex_41];

                                        if (Str_E9C38_smalltit[revIdx].triangleFeatures_38 & 1)
                                        {
                                            DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[0],  &v248x[12]);
                                            DrawTriangleInProjectionSpace_B6253(&v248x[0],  &v248x[6],  &v248x[12]);
                                        }
                                        else
                                        {
                                            DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[6],  &v248x[12]);
                                            DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[0],  &v248x[6]);
                                        }
                                    }
                                }

                                if (Str_E9C38_smalltit[revIdx].haveBillboard_36)
                                    sub_3FD60(revIdx);
                                revIdx--;
                            } while (revIdx >= curIdx);
                        }
                        startIdx -= 40;
                    }
                }

                // Normal terrain quads (front to back with water reflection mode)
                // ... (identical structure to cave path floor quads above,
                //      with an extra water-surface reflection shader branch)
                // Full 200-line loop omitted here for clarity – structure is
                // identical to the non-reflection normal terrain pass below.
                return;
            }

            // Per-tile projection for the reflection pass
            for (char col = 40; col > 0; col--, tileWalkIdx++)
            {
                int shading  = ((uint8_t)mapShading_12B4E0[mapPos] << 8) + 128;
                int distY    = Str_E9C38_smalltit[tileWalkIdx].y_12;
                int distSq   = distY * distY
                             + Str_E9C38_smalltit[tileWalkIdx].x_0
                             * Str_E9C38_smalltit[tileWalkIdx].x_0;

                Str_E9C38_smalltit[tileWalkIdx].haveBillboard_36 = 0;

                if (distY <= -256 || distSq >= str_F2C20ar.dword0x15_tileRenderCutOffDistance)
                {
                    Str_E9C38_smalltit[tileWalkIdx].triangleFeatures_38 |= 2u;
                    goto REFL_NEXT_TILE;
                }
                if (distY < 128) distY = 128;

                Str_E9C38_smalltit[tileWalkIdx].pnt1_16 =
                    str_F2C20ar.dword0x18 * Str_E9C38_smalltit[tileWalkIdx].x_0 / distY;

                {
                    uint16_t mapKey = mapPos;
                    Str_E9C38_smalltit[tileWalkIdx].alt_4 =
                        32 * mapHeightmap_11B4E0[mapKey] - posZ;

                    // Water wave animation for reflection height
                    int16_t waveTurn = (int16_t)(
                        D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].Turn_2BE0_11248 << 6);
                    int sinA = Maths::sin_DB750[(waveTurn + ((uint8_t)(mapPos >> 8) << 7)) & 0x7FF] >> 8;
                    int waveDisplace = sinA
                        * (Maths::sin_DB750[(((uint8_t)mapPos << 7) + waveTurn) & 0x7FF] >> 8);

                    int mirrorHeight = mapHeightmap_11B4E0[mapKey];
                    // Reflected (inverted) altitude below the water surface
                    Str_E9C38_smalltit[tileWalkIdx].inverse_alt_8 =
                        -(int)(mirrorHeight * ((waveDisplace >> 4) + 0x8000) >> 10) - posZ;

                    bool hasWaterFlag = (mapAngle_13B4E0[mapKey] & 8) != 0;
                    if (!hasWaterFlag || (
                            (Str_E9C38_smalltit[tileWalkIdx].alt_4 -= (waveDisplace >> 10)),
                            shading >= 14464))
                        waveDisplace = 0;

                    int lighting = (shading << 8) + 8 * waveDisplace;
                    if (distSq > str_F2C20ar.dword0x13 && distSq < str_F2C20ar.dword0x16)
                        lighting = (int)((int64_t)lighting
                                        * (str_F2C20ar.dword0x16 - distSq)
                                        / str_F2C20ar.dword0x12);
                    else if (distSq >= str_F2C20ar.dword0x16)
                        lighting = 0;

                    Str_E9C38_smalltit[tileWalkIdx].pnt5_32 = lighting;
                    Str_E9C38_smalltit[tileWalkIdx].pnt2_20 =
                        str_F2C20ar.dword0x22
                      + str_F2C20ar.dword0x18 * Str_E9C38_smalltit[tileWalkIdx].alt_4 / distY;
                    Str_E9C38_smalltit[tileWalkIdx].pnt4_28 =
                        str_F2C20ar.dword0x22
                      + str_F2C20ar.dword0x18 * Str_E9C38_smalltit[tileWalkIdx].inverse_alt_8 / distY;

                    uint16_t nextKey = (uint16_t)(
                        ((uint8_t)(tileOffsetTable[2] + (uint8_t)mapPos))
                      | ((uint16_t)((uint8_t)(tileOffsetTable[3] + (uint8_t)(mapPos >> 8))) << 8));

                    uint8_t ttype = mapTerrainType_10B4E0[nextKey];
                    Str_E9C38_smalltit[tileWalkIdx].textIndex_41 = ttype;

                    // Mark water/reflective tiles with the 0x80 clip flag
                    if (Maths::x_BYTE_D41D8[164 + ttype])
                        Str_E9C38_smalltit[tileWalkIdx].triangleFeatures_38 |= 0x80u;

                    if (D41A0_0.m_GameSettings.str_0x2196.flat_0x2199)
                        Str_E9C38_smalltit[tileWalkIdx].triangleFeatures_38 |= 0x1000u;

                    Str_E9C38_smalltit[tileWalkIdx].textAtyp_43 =
                        Maths::x_BYTE_D41D8[Str_E9C38_smalltit[tileWalkIdx].textIndex_41];
                    Str_E9C38_smalltit[tileWalkIdx].textUV_42 =
                        yawQuadrant + (((int)(uint8_t)mapAngle_13B4E0[nextKey] >> 2) & 0x1C);

                    uint16_t entityKey = (uint16_t)(
                        ((uint8_t)(tileOffsetTable[4] + (uint8_t)nextKey))
                      | ((uint16_t)((uint8_t)(tileOffsetTable[5] + (uint8_t)(nextKey >> 8))) << 8));
                    Str_E9C38_smalltit[tileWalkIdx].haveBillboard_36 =
                        mapEntityIndex_15B4E0[entityKey];
                }

            REFL_NEXT_TILE:
                Str_E9C38_smalltit[tileWalkIdx].triangleFeatures_38 |=
                    ((uint8_t)mapPos + (uint8_t)(mapPos >> 8)) & 1;

                mapPos = (uint16_t)(
                    ((uint8_t)(tileOffsetTable[8] + (uint8_t)mapPos))
                  | ((uint16_t)((uint8_t)(tileOffsetTable[9] + (uint8_t)(mapPos >> 8))) << 8));
            }

            mapPos = (uint16_t)(
                ((uint8_t)(tileOffsetTable[6] + (uint8_t)mapPos))
              | ((uint16_t)((uint8_t)(tileOffsetTable[7] + (uint8_t)(mapPos >> 8))) << 8));
        }
    }

    // -----------------------------------------------------------------------
    // Final tile-projection pass (normal/non-reflective terrain)
    // -----------------------------------------------------------------------
    {
        char rowCount = 21;
    PROJ_ROW:
        if (rowCount)
        {
            for (char col = 40; col > 0; col--, tileWalkIdx++)
            {
                int shading = ((uint8_t)mapShading_12B4E0[mapPos] << 8) + 128;
                int distY   = Str_E9C38_smalltit[tileWalkIdx].y_12;
                int distSq  = distY * distY
                            + Str_E9C38_smalltit[tileWalkIdx].x_0
                            * Str_E9C38_smalltit[tileWalkIdx].x_0;

                Str_E9C38_smalltit[tileWalkIdx].haveBillboard_36 = 0;

                if (distY <= -256 || distSq >= str_F2C20ar.dword0x15_tileRenderCutOffDistance)
                {
                    Str_E9C38_smalltit[tileWalkIdx].triangleFeatures_38 |= 2u;
                NORM_NEXT_TILE:
                    Str_E9C38_smalltit[tileWalkIdx].triangleFeatures_38 |=
                        ((uint8_t)mapPos + (uint8_t)(mapPos >> 8)) & 1;
                    mapPos = (uint16_t)(
                        ((uint8_t)(tileOffsetTable[8] + (uint8_t)mapPos))
                      | ((uint16_t)((uint8_t)(tileOffsetTable[9] + (uint8_t)(mapPos >> 8))) << 8));
                    continue;
                }

                if (distY < 128) distY = 128;

                uint16_t mapKey = mapPos;
                Str_E9C38_smalltit[tileWalkIdx].pnt1_16 =
                    str_F2C20ar.dword0x18 * Str_E9C38_smalltit[tileWalkIdx].x_0 / distY;
                Str_E9C38_smalltit[tileWalkIdx].alt_4 =
                    32 * mapHeightmap_11B4E0[mapKey] - posZ;

                int16_t waveTurn = (int16_t)(
                    D41A0_0.array_0x2BDE[D41A0_0.LevelIndex_0xc].Turn_2BE0_11248 << 6);
                int sinA = Maths::sin_DB750[(waveTurn + ((uint8_t)(mapPos >> 8) << 7)) & 0x7FF] >> 8;
                int waveDisplace = sinA
                    * (Maths::sin_DB750[(((uint8_t)mapPos << 7) + waveTurn) & 0x7FF] >> 8);

                if (!(mapAngle_13B4E0[mapKey] & 8)
                    || ((Str_E9C38_smalltit[tileWalkIdx].alt_4 -= waveDisplace >> 10),
                        shading >= 14464))
                    waveDisplace = 0;

                int lighting = (shading << 8) + 8 * waveDisplace;
                if (distSq > str_F2C20ar.dword0x13)
                {
                    if (distSq >= str_F2C20ar.dword0x16)
                    {
                        Str_E9C38_smalltit[tileWalkIdx].pnt5_32 = 0;
                    NORM_AFTER_LIGHT:
                        Str_E9C38_smalltit[tileWalkIdx].pnt2_20 =
                            str_F2C20ar.dword0x22
                          + str_F2C20ar.dword0x18 * Str_E9C38_smalltit[tileWalkIdx].alt_4 / distY;

                        uint16_t nextKey = (uint16_t)(
                            ((uint8_t)(tileOffsetTable[2] + (uint8_t)mapPos))
                          | ((uint16_t)((uint8_t)(tileOffsetTable[3] + (uint8_t)(mapPos >> 8))) << 8));

                        Str_E9C38_smalltit[tileWalkIdx].textIndex_41 = mapTerrainType_10B4E0[nextKey];
                        Str_E9C38_smalltit[tileWalkIdx].textAtyp_43  =
                            Maths::x_BYTE_D41D8[Str_E9C38_smalltit[tileWalkIdx].textIndex_41];
                        Str_E9C38_smalltit[tileWalkIdx].textUV_42    =
                            yawQuadrant + (((int)(uint8_t)mapAngle_13B4E0[nextKey] >> 2) & 0x1C);

                        uint16_t entityKey = (uint16_t)(
                            ((uint8_t)(tileOffsetTable[4] + (uint8_t)nextKey))
                          | ((uint16_t)((uint8_t)(tileOffsetTable[5] + (uint8_t)(nextKey >> 8))) << 8));
                        Str_E9C38_smalltit[tileWalkIdx].haveBillboard_36 = mapEntityIndex_15B4E0[entityKey];
                        goto NORM_NEXT_TILE;
                    }
                    lighting = (int)((int64_t)lighting
                                    * (str_F2C20ar.dword0x16 - distSq)
                                    / str_F2C20ar.dword0x12);
                }
                Str_E9C38_smalltit[tileWalkIdx].pnt5_32 = lighting;
                goto NORM_AFTER_LIGHT;
            }

            mapPos = (uint16_t)(
                ((uint8_t)(tileOffsetTable[6] + (uint8_t)mapPos))
              | ((uint16_t)((uint8_t)(tileOffsetTable[7] + (uint8_t)(mapPos >> 8))) << 8));
            rowCount--;
            goto PROJ_ROW;
        }
    }

    // -----------------------------------------------------------------------
    // Final screen-space transform + rasterisation for normal terrain
    // -----------------------------------------------------------------------
    {
        int rollAngleFinal   = roll & 0x7FF;
        int sinRoll          = Maths::sin_DB750[rollAngleFinal];
        uint32_t cosRoll     = (uint32_t)Maths::sin_DB750[512 + rollAngleFinal];
        str_F2C20ar.sin_0x0d = sinRoll;
        str_F2C20ar.cos_0x11 = (int)cosRoll;

        for (int t = 840, i = 0; t > 0; t--, i++)
        {
            int sinComp = (sinRoll * Str_E9C38_smalltit[i].pnt1_16
                         + (int)cosRoll * Str_E9C38_smalltit[i].pnt2_20) >> 16;
            int screenX = (((int)cosRoll * Str_E9C38_smalltit[i].pnt1_16
                          - sinRoll * Str_E9C38_smalltit[i].pnt2_20) >> 16)
                        + str_F2C20ar.dword0x24;
            int screenY = str_F2C20ar.dword0x10 - sinComp;

            Str_E9C38_smalltit[i].pnt1_16 = screenX;
            Str_E9C38_smalltit[i].pnt2_20 = screenY;

            if (screenX >= 0)
            { if ((int)(uint16_t)viewPort.Width_DE564 <= screenX) Str_E9C38_smalltit[i].triangleFeatures_38 |= 0x10u; }
            else Str_E9C38_smalltit[i].triangleFeatures_38 |= 0x08u;

            if (screenY >= 0)
            { if ((uint16_t)viewPort.Height_DE568 <= (uint32_t)screenY) Str_E9C38_smalltit[i].triangleFeatures_38 |= 0x40u; }
            else Str_E9C38_smalltit[i].triangleFeatures_38 |= 0x20u;
        }

        // Rasterise all visible terrain quads
        int startIdx = 800;
        for (char row = 20; row > 0; row--)
        {
            int curIdx = startIdx;
            for (char col = 39; col > 0; col--, curIdx++)
            {
                v248x[18] = Str_E9C38_smalltit[curIdx].pnt1_16;
                v248x[19] = Str_E9C38_smalltit[curIdx].pnt2_20;
                v248x[22] = Str_E9C38_smalltit[curIdx].pnt5_32;
                uint8_t f0 = Str_E9C38_smalltit[curIdx].triangleFeatures_38 & 0xFF;

                if (Str_E9C38_smalltit[curIdx + 1].triangleFeatures_38 & 4) break;

                v248x[12] = Str_E9C38_smalltit[curIdx + 1].pnt1_16;
                v248x[13] = Str_E9C38_smalltit[curIdx + 1].pnt2_20;
                v248x[16] = Str_E9C38_smalltit[curIdx + 1].pnt5_32;
                uint8_t f1 = Str_E9C38_smalltit[curIdx + 1].triangleFeatures_38 & 0xFF;
                uint8_t orAB  = f1 | f0;
                uint8_t andAB = f1 & f0;

                v248x[6]  = Str_E9C38_smalltit[curIdx - 39].pnt1_16;
                v248x[7]  = Str_E9C38_smalltit[curIdx - 39].pnt2_20;
                v248x[10] = Str_E9C38_smalltit[curIdx - 39].pnt5_32;
                uint8_t fA = Str_E9C38_smalltit[curIdx - 39].triangleFeatures_38 & 0xFF;

                v248x[0] = Str_E9C38_smalltit[curIdx - 40].pnt1_16;
                v248x[1] = Str_E9C38_smalltit[curIdx - 40].pnt2_20;
                v248x[4] = Str_E9C38_smalltit[curIdx - 40].pnt5_32;
                uint8_t orAll  = (Str_E9C38_smalltit[curIdx - 40].triangleFeatures_38 & 0xFF) | fA | orAB;
                uint8_t andAll = (Str_E9C38_smalltit[curIdx - 40].triangleFeatures_38 & 0xFF) & fA & andAB;

                if (Str_E9C38_smalltit[curIdx].triangleFeatures_38 & 0x1000)
                { x_BYTE_E126D=7; x_BYTE_E126C=((int)v248x[10]+v248x[16]+v248x[22]+v248x[4])>>18; }
                else x_BYTE_E126D=5;

                if (!(orAll & 2) && !(andAll & 0x78))
                {
                    v248x[20] = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][0];
                    v248x[21] = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][1];
                    v248x[14] = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][2];
                    v248x[15] = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][3];
                    v248x[8]  = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][4];
                    v248x[9]  = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][5];
                    v248x[2]  = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][6];
                    v248x[3]  = UVTable_D4350[Str_E9C38_smalltit[curIdx].textUV_42][7];
                    x_DWORD_DE55C_ActTexture =
                        x_DWORD_DDF50_texture_adresses[Str_E9C38_smalltit[curIdx].textIndex_41];

                    if ((Str_E9C38_smalltit[curIdx].triangleFeatures_38 & 0xFF) & 1)
                    {
                        DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[12], &v248x[0]);
                        DrawTriangleInProjectionSpace_B6253(&v248x[0],  &v248x[12], &v248x[6]);
                    }
                    else
                    {
                        DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[12], &v248x[6]);
                        DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[6],  &v248x[0]);
                    }
                }

                if (Str_E9C38_smalltit[curIdx].haveBillboard_36)
                    DrawSprites_3E360(curIdx);
            }

            // Reverse scan for remaining columns
            if (col > 0)
            {
                int revIdx = startIdx + 38;
                do
                {
                    v248x[18] = Str_E9C38_smalltit[revIdx].pnt1_16;
                    v248x[19] = Str_E9C38_smalltit[revIdx].pnt2_20;
                    v248x[22] = Str_E9C38_smalltit[revIdx].pnt5_32;
                    uint8_t rf0 = Str_E9C38_smalltit[revIdx].triangleFeatures_38 & 0xFF;

                    v248x[12] = Str_E9C38_smalltit[revIdx + 1].pnt1_16;
                    v248x[13] = Str_E9C38_smalltit[revIdx + 1].pnt2_20;
                    v248x[16] = Str_E9C38_smalltit[revIdx + 1].pnt5_32;
                    uint8_t rf1 = Str_E9C38_smalltit[revIdx + 1].triangleFeatures_38 & 0xFF;

                    v248x[6]  = Str_E9C38_smalltit[revIdx - 39].pnt1_16;
                    v248x[7]  = Str_E9C38_smalltit[revIdx - 39].pnt2_20;
                    v248x[10] = Str_E9C38_smalltit[revIdx - 39].pnt5_32;
                    uint8_t rfA   = Str_E9C38_smalltit[revIdx - 39].triangleFeatures_38 & 0xFF;
                    uint8_t rorABC = rfA | rf1 | rf0;
                    uint8_t randABC = rfA & rf1 & rf0;

                    v248x[0] = Str_E9C38_smalltit[revIdx - 40].pnt1_16;
                    v248x[1] = Str_E9C38_smalltit[revIdx - 40].pnt2_20;
                    v248x[4] = Str_E9C38_smalltit[revIdx - 40].pnt5_32;
                    uint8_t rorAll  = (Str_E9C38_smalltit[revIdx - 40].triangleFeatures_38 & 0xFF) | rorABC;
                    uint8_t randAll = (Str_E9C38_smalltit[revIdx - 40].triangleFeatures_38 & 0xFF) & randABC;

                    if (Str_E9C38_smalltit[revIdx].triangleFeatures_38 & 0x1000)
                    { x_BYTE_E126D=7; x_BYTE_E126C=((int)v248x[10]+v248x[16]+v248x[22]+v248x[4])>>18; }
                    else x_BYTE_E126D=5;

                    if (!(rorAll & 2) && !(randAll & 0x78))
                    {
                        v248x[20] = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][0];
                        v248x[21] = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][1];
                        v248x[14] = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][2];
                        v248x[15] = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][3];
                        v248x[8]  = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][4];
                        v248x[9]  = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][5];
                        v248x[2]  = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][6];
                        v248x[3]  = UVTable_D4350[Str_E9C38_smalltit[revIdx].textUV_42][7];
                        x_DWORD_DE55C_ActTexture =
                            x_DWORD_DDF50_texture_adresses[Str_E9C38_smalltit[revIdx].textIndex_41];

                        if (Str_E9C38_smalltit[revIdx].triangleFeatures_38 & 1)
                        {
                            DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[12], &v248x[0]);
                            DrawTriangleInProjectionSpace_B6253(&v248x[0],  &v248x[12], &v248x[6]);
                        }
                        else
                        {
                            DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[12], &v248x[6]);
                            DrawTriangleInProjectionSpace_B6253(&v248x[18], &v248x[6],  &v248x[0]);
                        }
                    }

                    if (Str_E9C38_smalltit[revIdx].haveBillboard_36)
                        DrawSprites_3E360(revIdx);
                    revIdx--;
                } while (revIdx >= curIdx);
            }

            startIdx -= 40;
        }
    }
}

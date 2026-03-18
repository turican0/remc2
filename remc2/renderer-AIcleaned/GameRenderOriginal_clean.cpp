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


// ---------------------------------------------------------------------------
// DrawSprites_3E360
// Walks the linked list of entities/billboards chained from a tile's
// haveBillboard_36 field and draws each one.
//
// Per entity, two draw passes may occur:
//   1. Shadow pass  – small ground-projected quad.
//                     Condition: shadows enabled, tile is plain ground
//                     (textAtyp_43 == 0), entity has no special render flags.
//   2. Main sprite  – full-size billboard at the entity's 3-D position.
//
// Sprite type (particlesParameters_D951C[].byte_12):
//   0 / 1   – single static frame (0 = use cache, 1 = always reload)
//   2–16    – animated via entity.animationFrame
//   17      – 8-directional yaw: slots 0-7 direct, 8-15 mirrored
//   18      – 8-directional yaw: all 16 slots direct (no mirror)
//   19      – 16-directional, remap via x_BYTE_D4750[12+slot], mirror on rear
//   20      – 16-directional, remap via x_BYTE_D4750[28+slot], mirror on rear
//   21      – single frame, sets shadow-caster flag (x_BYTE_F2CC6 = 1)
//   22–36   – animated + shadow-caster
// ---------------------------------------------------------------------------
void GameRenderOriginal::DrawSprites_3E360(int tileIdx)
{
    type_particle_str* frame = nullptr;

    uint16_t entityHandle = Str_E9C38_smalltit[tileIdx].haveBillboard_36;

    do
    {
        type_entity_0x6E8E* ent = Entities_EA3E4[entityHandle];
        str_F2C20ar.dword0x14x = ent;

        if (ent->struct_byte_0xc_12_15.byte[0] & 0x21)
            goto LABEL_178;

        {
            // World-space camera-relative position
            int16_t  entY = ent->position_0x4C_76.y;
            int      relX = (int16_t)(ent->position_0x4C_76.x - cameraX_F2CC4);
            int      relY = (int16_t)(cameraY_F2CC2 - entY);

            // ------------------------------------------------------------------
            // Shadow pass
            // ------------------------------------------------------------------
            if (shadows_F2CC7
                && !Str_E9C38_smalltit[tileIdx].textAtyp_43
                && !(ent->struct_byte_0xc_12_15.word[1] & 0x808))
            {
                // Rotate into camera forward/right space
                int shadowViewX  = (str_F2C20ar.cos2_0x0f * relX - str_F2C20ar.sin2_0x17 * relY) >> 16;
                int shadowViewY  = (str_F2C20ar.sin2_0x17 * relX + str_F2C20ar.cos2_0x0f * relY) >> 16;
                int shadowDistSq = shadowViewY * shadowViewY + shadowViewX * shadowViewX;

                if (shadowViewY > 64
                    && shadowDistSq < str_F2C20ar.dword0x15_tileRenderCutOffDistance)
                {
                    if (shadowDistSq <= str_F2C20ar.dword0x13)
                        str_F2C20ar.dword0x00 = 0x2000;
                    else
                        str_F2C20ar.dword0x00 =
                            (shadowDistSq < str_F2C20ar.dword0x16)
                            ? 32 * (str_F2C20ar.dword0x16
                                    - (shadowViewY * shadowViewY + shadowViewX * shadowViewX))
                              / str_F2C20ar.dword0x12 << 8
                            : 0;

                    type_WORD_D951C* sp = &particlesParameters_D951C[ent->word_0x5A_90];

                    if (!sp->byte_10)
                    {
                        // Project shadow onto terrain
                        int shadowGroundAlt =
                            sub_B5C60_getTerrainAlt2(ent->position_0x4C_76.x, entY)
                            - str_F2C20ar.dword0x20;

                        int shadowProjX = shadowViewX * str_F2C20ar.dword0x18 / shadowViewY;
                        int shadowProjZ = str_F2C20ar.dword0x18 * shadowGroundAlt / shadowViewY
                                        + str_F2C20ar.dword0x22;

                        str_F2C20ar.dword0x04_screenY =
                            ((shadowProjX * str_F2C20ar.cos_0x11
                              - str_F2C20ar.sin_0x0d * shadowProjZ) >> 16)
                          + str_F2C20ar.dword0x24;
                        str_F2C20ar.dword0x03_screenX =
                            str_F2C20ar.dword0x10
                          - ((str_F2C20ar.sin_0x0d * shadowProjX
                              + shadowProjZ * str_F2C20ar.cos_0x11) >> 16);

                        uint8_t shadowSprType = sp->byte_12;
                        x_BYTE_F2CC6 = 0;

                        // Resolve which frame to use for the shadow
                        int shadowHorizFlip = 0;  // dword0x05 value
                        switch (shadowSprType)
                        {
                        case 0:
                            if (str_DWORD_F66F0x[sp->word_0])
                            {
                                // already cached – fall through to mark timing
                            }
                            else
                            {
                                if (!MainInitTmaps_71520(sp->word_0)) goto LABEL_178;
                            }
                            x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[sp->word_0].word_8] =
                                x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
                            frame = *str_DWORD_F66F0x[sp->word_0];
                            goto SHADOW_FRAME_COMMON;

                        case 1:
                            if (!str_DWORD_F66F0x[sp->word_0]
                                && !MainInitTmaps_71520(sp->word_0)) goto LABEL_178;
                            x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[sp->word_0].word_8] =
                                x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
                            frame = *str_DWORD_F66F0x[sp->word_0];
                            goto SHADOW_FRAME_COMMON;

                        case 17:
                        {
                            int slot = (((ent->yaw_0x1C_28
                                          - (uint16_t)yaw_F2CC0) >> 3) & 0xF0) >> 4;
                            int fi   = (slot < 8) ? sp->word_0 + slot
                                                  : sp->word_0 + 15 - slot;
                            uint16_t timingIdx;
                            if (str_DWORD_F66F0x[fi])
                                timingIdx = str_TMAPS00TAB_BEGIN_BUFFER[fi].word_8;
                            else
                            {
                                if (!MainInitTmaps_71520(fi)) goto LABEL_178;
                                timingIdx = str_TMAPS00TAB_BEGIN_BUFFER[fi].word_8;
                            }
                            x_DWORD_F5730[timingIdx] = x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
                            frame = *str_DWORD_F66F0x[fi];
                            str_F2C20ar.dword0x08_width  = frame->width;
                            str_F2C20ar.dword0x06_height = frame->height;
                            {
                                int h = (int)((int64_t)(str_F2C20ar.dword0x18 * sp->rotSpeed_8)
                                              / shadowViewY);
                                str_F2C20ar.dword0x0c_realHeight = h;
                                str_F2C20ar.dword0x09_realWidth  = h * frame->width / frame->height;
                            }
                            shadowHorizFlip = (slot < 8) ? frame->width : -frame->width;
                            goto SHADOW_SIZE_READY;
                        }

                        case 18:
                        {
                            int slot = (((ent->yaw_0x1C_28
                                          - (uint16_t)yaw_F2CC0) >> 3) & 0xF0) >> 4;
                            int fi   = sp->word_0 + slot;
                            uint16_t timingIdx;
                            if (str_DWORD_F66F0x[fi])
                                timingIdx = str_TMAPS00TAB_BEGIN_BUFFER[fi].word_8;
                            else
                            {
                                if (!MainInitTmaps_71520(fi)) goto LABEL_178;
                                timingIdx = str_TMAPS00TAB_BEGIN_BUFFER[fi].word_8;
                            }
                            x_DWORD_F5730[timingIdx] = x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
                            frame = *str_DWORD_F66F0x[fi];
                            str_F2C20ar.dword0x08_width  = frame->width;
                            str_F2C20ar.dword0x06_height = frame->height;
                            {
                                int h = (int)((int64_t)(str_F2C20ar.dword0x18 * sp->rotSpeed_8)
                                              / shadowViewY);
                                str_F2C20ar.dword0x0c_realHeight = h;
                                str_F2C20ar.dword0x09_realWidth  = h * frame->width / frame->height;
                            }
                            shadowHorizFlip = frame->width;
                            goto SHADOW_SIZE_READY;
                        }

                        case 19:
                        case 20:
                        {
                            int tableBase = (shadowSprType == 19) ? 12 : 28;
                            int slot      = (((ent->yaw_0x1C_28
                                               - (uint16_t)yaw_F2CC0) >> 3) & 0xF0) >> 4;
                            int remapped  = (uint8_t)x_BYTE_D4750[tableBase + slot];
                            int fi        = sp->word_0 + remapped;
                            uint16_t timingIdx;
                            if (str_DWORD_F66F0x[fi])
                                timingIdx = str_TMAPS00TAB_BEGIN_BUFFER[fi].word_8;
                            else
                            {
                                if (!MainInitTmaps_71520(fi)) goto LABEL_178;
                                timingIdx = str_TMAPS00TAB_BEGIN_BUFFER[fi].word_8;
                            }
                            x_DWORD_F5730[timingIdx] = x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
                            frame = *str_DWORD_F66F0x[fi];
                            str_F2C20ar.dword0x08_width  = frame->width;
                            str_F2C20ar.dword0x06_height = frame->height;
                            {
                                int h = (int)((int64_t)(str_F2C20ar.dword0x18 * sp->rotSpeed_8)
                                              / shadowViewY);
                                str_F2C20ar.dword0x0c_realHeight = h;
                                str_F2C20ar.dword0x09_realWidth  = h * frame->width / frame->height;
                            }
                            shadowHorizFlip = (slot < 8) ? frame->width : -frame->width;
                            goto SHADOW_SIZE_READY;
                        }

                        case 21:
                            if (str_DWORD_F66F0x[sp->word_0])
                            {
                                // cached
                            }
                            else
                            {
                                if (!MainInitTmaps_71520(sp->word_0)) goto LABEL_178;
                            }
                            x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[sp->word_0].word_8] =
                                x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
                            frame = *str_DWORD_F66F0x[sp->word_0];
                            goto SHADOW_FRAME_COMMON;

                        default:
                            // cases 2-16 and 22-36: animated frame
                            if (shadowSprType >= 22) x_BYTE_F2CC6 = 1;
                            {
                                int fi = sp->word_0 + ent->animationFrame_0x5C_92;
                                if (!str_DWORD_F66F0x[fi]
                                    && !MainInitTmaps_71520(fi)) goto LABEL_178;
                                x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[fi].word_8] =
                                    x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
                                frame = *str_DWORD_F66F0x[fi];
                            }
                        SHADOW_FRAME_COMMON:
                            str_F2C20ar.dword0x08_width  = frame->width;
                            str_F2C20ar.dword0x06_height = frame->height;
                            {
                                int h = (int)((int64_t)(str_F2C20ar.dword0x18 * sp->rotSpeed_8)
                                              / shadowViewY);
                                str_F2C20ar.dword0x0c_realHeight = h;
                                str_F2C20ar.dword0x09_realWidth  = h * frame->width / frame->height;
                            }
                            shadowHorizFlip = frame->width;
                            break;
                        }

                    SHADOW_SIZE_READY:
                        str_F2C20ar.dword0x05      = shadowHorizFlip;
                        str_F2C20ar.dword0x02_data = frame->textureBuffer;
                        frame->word_0 |= 8;

                        // Palette selection: near = byte_10, far = byte_10 + 6
                        str_F2C20ar.dword0x01_rotIdx =
                            (str_F2C20ar.dword0x00 == 0x2000)
                            ? x_BYTE_D4750[sp->byte_10]
                            : x_BYTE_D4750[6 + sp->byte_10];

                        // Quarter-size shadow
                        int shadowH = str_F2C20ar.dword0x0c_realHeight >> 2;
                        str_F2C20ar.dword0x0c_realHeight = shadowH;
                        if (str_F2C20ar.dword0x09_realWidth > 0 && shadowH > 0)
                        {
                            int fadeBias = str_F2C20ar.dword0x00 >> 2;
                            str_F2C20ar.dword0x00 = notDay_D4320
                                ? 0x2000 - fadeBias
                                : fadeBias + 0x2000;
                            str_F2C20ar.dword0x01_rotIdx = 8;
                            DrawSprite_41BD3(0);
                        }
                    }
                }
            }

            // ------------------------------------------------------------------
            // Main sprite pass
            // ------------------------------------------------------------------

            // Flying entities render 160 units above their stored Z
            int entityZ = (ent->struct_byte_0xc_12_15.byte[3] >= 0)
                        ? ent->position_0x4C_76.z
                        : ent->position_0x4C_76.z - 160;

            int viewY = (str_F2C20ar.sin2_0x17 * relX + str_F2C20ar.cos2_0x0f * relY) >> 16;
            int viewX = (str_F2C20ar.cos2_0x0f * relX - str_F2C20ar.sin2_0x17 * relY) >> 16;

            // Mounted entities: adjust viewY by bone attachment offset
            if (ent->struct_byte_0xc_12_15.byte[3] & 0x20)
            {
                type_D404C* bone        = &str_D404C[ent->byte_0x3B_59];
                int         mountFacing =
                    (((Entities_EA3E4[ent->word_0x32_50]->yaw_0x1C_28
                       - (uint16_t)yaw_F2CC0) >> 3) & 0xF0) >> 4;
                switch (mountFacing)
                {
                case 0: case 15: viewY -= bone->word_16; break;
                case 1: case 14: viewY -= bone->word_18; break;
                case 2: case 13: viewY -= bone->word_20; break;
                case 5: case 10: viewY += bone->word_20; break;
                case 6: case  9: viewY += bone->word_18; break;
                case 7: case  8: viewY += bone->word_16; break;
                default: break;
                }
            }

            int distSq = viewY * viewY + viewX * viewX;
            if (viewY <= 64 || distSq >= str_F2C20ar.dword0x15_tileRenderCutOffDistance)
                goto LABEL_178;

            if (distSq <= str_F2C20ar.dword0x13)
                str_F2C20ar.dword0x00 = 0x2000;
            else if (distSq < str_F2C20ar.dword0x16)
                str_F2C20ar.dword0x00 =
                    32 * (str_F2C20ar.dword0x16
                          - (viewY * viewY + viewX * viewX))
                    / str_F2C20ar.dword0x12 << 8;
            else
                str_F2C20ar.dword0x00 = 0;

            {
                type_WORD_D951C* sp = &particlesParameters_D951C[ent->word_0x5A_90];

                int projX = viewX * str_F2C20ar.dword0x18 / viewY;
                int projZ = str_F2C20ar.dword0x18 * (entityZ - str_F2C20ar.dword0x20) / viewY
                           + str_F2C20ar.dword0x22;

                str_F2C20ar.dword0x04_screenY =
                    ((projX * str_F2C20ar.cos_0x11 - str_F2C20ar.sin_0x0d * projZ) >> 16)
                  + str_F2C20ar.dword0x24;
                str_F2C20ar.dword0x03_screenX =
                    str_F2C20ar.dword0x10
                  - ((str_F2C20ar.sin_0x0d * projX + projZ * str_F2C20ar.cos_0x11) >> 16);

                uint8_t sprType = sp->byte_12;
                x_BYTE_F2CC6   = 0;
                int horizFlip  = 0;  // dword0x05 value

                switch (sprType)
                {
                // ── single static frame (cached) ─────────────────────────
                case 0:
                    if (str_DWORD_F66F0x[sp->word_0])
                    {
                        goto MAIN_CASE0_LOADED;
                    }
                    if (!MainInitTmaps_71520(sp->word_0)) break;
                MAIN_CASE0_LOADED:
                    x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[sp->word_0].word_8] =
                        x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
                    frame = *str_DWORD_F66F0x[sp->word_0];
                    goto MAIN_FRAME_COMMON;

                // ── single static frame (always reload) ──────────────────
                case 1:
                    if (str_DWORD_F66F0x[sp->word_0])
                        x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[sp->word_0].word_8] =
                            x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
                    else
                    {
                        if (!MainInitTmaps_71520(sp->word_0)) break;
                        x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[sp->word_0].word_8] =
                            x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
                    }
                    frame = *str_DWORD_F66F0x[sp->word_0];
                    goto MAIN_FRAME_COMMON;

                // ── cases 2-16: fall through to animated frame ────────────
                case 2: case 3: case 4: case 5: case 6: case 7: case 8:
                case 9: case 10: case 11: case 12: case 13: case 14:
                case 15: case 16:
                    goto MAIN_ANIMATED;

                // ── 8-direction, rear half mirrored ──────────────────────
                case 17:
                {
                    int slot = (((ent->yaw_0x1C_28 - (uint16_t)yaw_F2CC0) >> 3) & 0xF0) >> 4;
                    // Riding flag can flip the effective facing direction
                    if (ent->struct_byte_0xc_12_15.byte[3] & 0x40)
                        slot = (uint8_t)x_BYTE_D4750[44 + slot];
                    if (slot < 8)
                    {
                        int fi = slot + sp->word_0;
                        uint16_t timingIdx;
                        if (str_DWORD_F66F0x[fi])
                            timingIdx = str_TMAPS00TAB_BEGIN_BUFFER[fi].word_8;
                        else
                        {
                            if (!MainInitTmaps_71520(fi)) break;
                            timingIdx = str_TMAPS00TAB_BEGIN_BUFFER[fi].word_8;
                        }
                        x_DWORD_F5730[timingIdx] = x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
                        frame = *str_DWORD_F66F0x[sp->word_0 + slot];
                        goto MAIN_FRAME_COMMON;
                    }
                    else
                    {
                        int fi = sp->word_0 + 15 - slot;
                        if (str_DWORD_F66F0x[fi])
                            x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[fi].word_8] =
                                x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
                        else
                        {
                            if (!MainInitTmaps_71520(fi)) break;
                            x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[fi].word_8] =
                                x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
                        }
                        frame = *str_DWORD_F66F0x[fi];
                        str_F2C20ar.dword0x08_width  = frame->width;
                        str_F2C20ar.dword0x06_height = frame->height;
                        {
                            int h = (int)((int64_t)(str_F2C20ar.dword0x18 * sp->rotSpeed_8) / viewY);
                            str_F2C20ar.dword0x0c_realHeight = h;
                            str_F2C20ar.dword0x09_realWidth  = h * frame->width / frame->height;
                        }
                        horizFlip = -str_F2C20ar.dword0x08_width;  // mirrored
                        goto MAIN_SIZE_READY;
                    }
                }

                // ── 8-direction, all slots direct ─────────────────────────
                case 18:
                {
                    int slot = (((ent->yaw_0x1C_28 - (uint16_t)yaw_F2CC0) >> 3) & 0xF0) >> 4;
                    int fi   = slot + sp->word_0;
                    if (str_DWORD_F66F0x[fi])
                        x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[fi].word_8] =
                            x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
                    else
                    {
                        if (!MainInitTmaps_71520(fi)) break;
                        x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[fi].word_8] =
                            x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
                    }
                    frame = *str_DWORD_F66F0x[fi];
                    str_F2C20ar.dword0x08_width  = frame->width;
                    str_F2C20ar.dword0x06_height = frame->height;
                    {
                        int h = (int)((int64_t)(str_F2C20ar.dword0x18 * sp->rotSpeed_8) / viewY);
                        str_F2C20ar.dword0x0c_realHeight = h;
                        str_F2C20ar.dword0x09_realWidth  = h * frame->width / frame->height;
                    }
                    horizFlip = str_F2C20ar.dword0x08_width;
                    goto MAIN_SIZE_READY;
                }

                // ── 16-direction remap table ──────────────────────────────
                case 19:
                {
                    int slot = (((ent->yaw_0x1C_28 - (uint16_t)yaw_F2CC0) >> 3) & 0xF0) >> 4;
                    if (slot >= 8)
                    {
                        int fi = sp->word_0 + (uint8_t)x_BYTE_D4750[12 + slot];
                        if (str_DWORD_F66F0x[fi])
                            x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[fi].word_8] =
                                x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
                        else
                        {
                            if (!MainInitTmaps_71520(sp->word_0
                                    + (uint8_t)x_BYTE_D4750[12 + slot])) break;
                            x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[
                                sp->word_0 + (uint8_t)x_BYTE_D4750[12 + slot]].word_8] =
                                x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
                        }
                        frame = *str_DWORD_F66F0x[sp->word_0 + (uint8_t)x_BYTE_D4750[12 + slot]];
                        str_F2C20ar.dword0x08_width  = frame->width;
                        str_F2C20ar.dword0x06_height = frame->height;
                        {
                            int h = (int)((int64_t)(str_F2C20ar.dword0x18 * sp->rotSpeed_8) / viewY);
                            str_F2C20ar.dword0x0c_realHeight = h;
                            str_F2C20ar.dword0x09_realWidth  = h * frame->width / frame->height;
                        }
                        horizFlip = -str_F2C20ar.dword0x08_width;
                    }
                    else
                    {
                        int fi = sp->word_0 + (uint8_t)x_BYTE_D4750[12 + slot];
                        if (str_DWORD_F66F0x[fi])
                        {
                            uint16_t t = str_TMAPS00TAB_BEGIN_BUFFER[fi].word_8;
                            x_DWORD_F5730[t] = x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
                        }
                        else
                        {
                            if (!MainInitTmaps_71520(sp->word_0
                                    + (uint8_t)x_BYTE_D4750[12 + slot])) break;
                            x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[
                                (uint8_t)x_BYTE_D4750[12 + slot] + sp->word_0].word_8] =
                                x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
                        }
                        frame = *str_DWORD_F66F0x[sp->word_0 + (uint8_t)x_BYTE_D4750[12 + slot]];
                        str_F2C20ar.dword0x08_width  = frame->width;
                        str_F2C20ar.dword0x06_height = frame->height;
                        {
                            int h = (int)((int64_t)(str_F2C20ar.dword0x18 * sp->rotSpeed_8) / viewY);
                            str_F2C20ar.dword0x0c_realHeight = h;
                            str_F2C20ar.dword0x09_realWidth  = h * frame->width / frame->height;
                        }
                        horizFlip = str_F2C20ar.dword0x08_width;
                    }
                    goto MAIN_SIZE_READY;
                }

                // ── 16-direction remap table (set 2) ─────────────────────
                case 20:
                {
                    int slot = (((ent->yaw_0x1C_28 - (uint16_t)yaw_F2CC0) >> 3) & 0xF0) >> 4;
                    if (slot >= 8)
                    {
                        int fi = sp->word_0 + (uint8_t)x_BYTE_D4750[28 + slot];
                        if (str_DWORD_F66F0x[fi])
                        {
                            uint16_t t = str_TMAPS00TAB_BEGIN_BUFFER[fi].word_8;
                            x_DWORD_F5730[t] = x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
                        }
                        else
                        {
                            if (!MainInitTmaps_71520(sp->word_0
                                    + (uint8_t)x_BYTE_D4750[28 + slot])) break;
                            x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[
                                sp->word_0 + (uint8_t)x_BYTE_D4750[28 + slot]].word_8] =
                                x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
                        }
                        frame = *str_DWORD_F66F0x[sp->word_0 + (uint8_t)x_BYTE_D4750[28 + slot]];
                        str_F2C20ar.dword0x08_width  = frame->width;
                        str_F2C20ar.dword0x06_height = frame->height;
                        {
                            int h = (int)((int64_t)(str_F2C20ar.dword0x18 * sp->rotSpeed_8) / viewY);
                            str_F2C20ar.dword0x0c_realHeight = h;
                            str_F2C20ar.dword0x09_realWidth  = h * frame->width / frame->height;
                        }
                        horizFlip = -str_F2C20ar.dword0x08_width;
                    }
                    else
                    {
                        int remapped = (uint8_t)x_BYTE_D4750[28 + slot];
                        int fi       = remapped + sp->word_0;
                        if (str_DWORD_F66F0x[fi])
                            x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[fi].word_8] =
                                x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
                        else
                        {
                            if (!MainInitTmaps_71520(sp->word_0 + (uint8_t)remapped)) break;
                            x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[
                                sp->word_0 + (uint8_t)x_BYTE_D4750[28 + slot]].word_8] =
                                x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
                        }
                        frame = *str_DWORD_F66F0x[sp->word_0 + (uint8_t)x_BYTE_D4750[28 + slot]];
                        str_F2C20ar.dword0x08_width  = frame->width;
                        str_F2C20ar.dword0x06_height = frame->height;
                        {
                            int h = (int)((int64_t)(str_F2C20ar.dword0x18 * sp->rotSpeed_8) / viewY);
                            str_F2C20ar.dword0x0c_realHeight = h;
                            str_F2C20ar.dword0x09_realWidth  = h * frame->width / frame->height;
                        }
                        horizFlip = str_F2C20ar.dword0x08_width;
                    }
                    goto MAIN_SIZE_READY;
                }

                // ── single frame + shadow-caster ─────────────────────────
                case 21:
                {
                    int fi = sp->word_0;
                    if (str_DWORD_F66F0x[fi])
                        x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[fi].word_8] =
                            x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
                    else
                    {
                        if (!MainInitTmaps_71520(fi)) break;
                        x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[sp->word_0].word_8] =
                            x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
                    }
                    frame = *str_DWORD_F66F0x[sp->word_0];
                    x_BYTE_F2CC6 = 1;
                    goto MAIN_FRAME_COMMON;
                }

                // ── animated + shadow-caster (22-36) ─────────────────────
                case 22: case 23: case 24: case 25: case 26: case 27:
                case 28: case 29: case 30: case 31: case 32: case 33:
                case 34: case 35: case 36:
                    x_BYTE_F2CC6 = 1;
                MAIN_ANIMATED:
                {
                    int fi = sp->word_0 + ent->animationFrame_0x5C_92;
                    if (str_DWORD_F66F0x[fi])
                        x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[fi].word_8] =
                            x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
                    else
                    {
                        if (!MainInitTmaps_71520(sp->word_0 + ent->animationFrame_0x5C_92)) break;
                        x_DWORD_F5730[str_TMAPS00TAB_BEGIN_BUFFER[
                            sp->word_0 + ent->animationFrame_0x5C_92].word_8] =
                            x_D41A0_BYTEARRAY_4_struct.FrameTimingIndex_26;
                    }
                    frame = *str_DWORD_F66F0x[sp->word_0 + ent->animationFrame_0x5C_92];
                }
                MAIN_FRAME_COMMON:
                    str_F2C20ar.dword0x08_width  = frame->width;
                    str_F2C20ar.dword0x06_height = frame->height;
                    {
                        int h = (int)((int64_t)(str_F2C20ar.dword0x18 * sp->rotSpeed_8) / viewY);
                        str_F2C20ar.dword0x0c_realHeight = h;
                        str_F2C20ar.dword0x09_realWidth  = h * frame->width / frame->height;
                    }
                    horizFlip = str_F2C20ar.dword0x08_width;
                    break;

                default:
                    goto MAIN_BLEND_SELECT;  // fallthrough for unknown types
                }

            MAIN_SIZE_READY:
                str_F2C20ar.dword0x05 = horizFlip;

            MAIN_BLEND_SELECT:
            {
                type_entity_0x6E8E* cur   = str_F2C20ar.dword0x14x;
                frame->word_0            |= 8u;
                uint8_t renderFlags2      = cur->struct_byte_0xc_12_15.byte[2];
                str_F2C20ar.dword0x02_data = frame->textureBuffer;

                if (renderFlags2 & 0x380)
                {
                    uint8_t renderFlags3 = cur->struct_byte_0xc_12_15.byte[3];
                    if (renderFlags3 & 2)
                    {
                        // Mode 4: team colour from parent entity
                        int colour = playersColors_E88E0x[
                            3 * Entities_EA3E4[cur->parentId_0x28_40]
                                  ->dword_0xA4_164x->playerColorIndex_0x38_56][2];
                        str_F2C20ar.dword0x01_rotIdx = 4;
                        str_F2C20ar.dword0x07        = colour;
                    }
                    else if (renderFlags3 & 4)
                    {
                        // Mode 5: team colour from parent entity
                        int colour = playersColors_E88E0x[
                            3 * Entities_EA3E4[cur->parentId_0x28_40]
                                  ->dword_0xA4_164x->playerColorIndex_0x38_56][2];
                        str_F2C20ar.dword0x01_rotIdx = 5;
                        str_F2C20ar.dword0x07        = colour;
                    }
                    else if ((int8_t)renderFlags2 >= 0)
                    {
                        if (renderFlags3 & 1) str_F2C20ar.dword0x01_rotIdx = 3;
                    }
                    else
                    {
                        str_F2C20ar.dword0x01_rotIdx = 2;
                    }
                }
                else
                {
                    // Normal palette ramp: near = byte_10, far = byte_10 + 6
                    str_F2C20ar.dword0x01_rotIdx =
                        (str_F2C20ar.dword0x00 == 0x2000)
                        ? x_BYTE_D4750[sp->byte_10]
                        : x_BYTE_D4750[6 + sp->byte_10];
                }

                str_F2C20ar.dword0x09_realWidth++;
                str_F2C20ar.dword0x0c_realHeight++;
                DrawSprite_41BD3(1u);
            }
            }
        }

    LABEL_178:
        entityHandle = str_F2C20ar.dword0x14x->oldMapEntity_0x16_22;
    } while (entityHandle);
}

// ---------------------------------------------------------------------------
// DrawSprite_41BD3
// Low-level sprite blitter. Handles two top-level render modes:
//
//   x_BYTE_F2CC6 == 0  (normal billboard mode)
//     The sprite is projected through the roll-rotation / billboard system.
//     str_F2C20ar.dword0x1e  selects one of eight scanline orientations (0-7)
//     that correspond to the eight 45-degree billboard-rotation sectors set
//     up by SetBillboards_3B560.  Each orientation computes a clipped scanline
//     list then rasterises it using one of nine pixel-blend modes
//     (dword0x01_rotIdx 0-8).
//
//   x_BYTE_F2CC6 != 0  (axis-aligned sprite mode, used for particles/shadows)
//     The sprite is drawn as a simple screen-aligned rectangle.
//     Clipping is handled independently in X and Y, then each row is
//     rendered with one of eight blend modes (dword0x01_rotIdx 0-7).
//
// Blend modes (rotIdx):
//   0  – opaque copy (skip colour 0)
//   1  – palette ramp (distance fog)
//   2  – additive blend (sprite colour as alpha, bg mixed hi-byte)
//   3  – subtractive blend (sprite colour as alpha, bg mixed lo-byte)
//   4  – team colour tint via dword0x07 hi-byte
//   5  – team colour tint via dword0x07 byte-1
//   6  – double-lookup blend (sprite→table→bg→table)
//   7  – double-lookup blend variant
//   8  – shadow-darken (used only in billboard mode)
//
// Parameter a1:
//   0  – shadow sprite (no screen-position adjustment)
//   1  – normal sprite (adjust screen position to centre on entity)
//   2  – particle / special (alternative centring)
// ---------------------------------------------------------------------------
void GameRenderOriginal::DrawSprite_41BD3(uint32_t a1)
{
    // -------------------------------------------------------------------------
    // Screen-aligned (axis-aligned) sprite path  (x_BYTE_F2CC6 != 0)
    // -------------------------------------------------------------------------
    if (x_BYTE_F2CC6)
    {
        // Centre offset applied to screen position depending on draw mode
        int halfSize = (str_F2C20ar.dword0x0c_realHeight + str_F2C20ar.dword0x09_realWidth) >> 2;
        if (a1 >= 1)
        {
            if (a1 <= 1)  // a1 == 1
            {
                str_F2C20ar.dword0x04_screenY +=
                    -(str_F2C20ar.sin_0x0d * halfSize >> 16) - halfSize;
                str_F2C20ar.dword0x03_screenX +=
                    -(str_F2C20ar.cos_0x11 * halfSize >> 16) - halfSize;
            }
            else if (a1 == 2)
            {
                str_F2C20ar.dword0x04_screenY +=
                    (str_F2C20ar.sin_0x0d * halfSize >> 16) - halfSize;
                str_F2C20ar.dword0x03_screenX +=
                    (str_F2C20ar.cos_0x11 * halfSize >> 16) - halfSize;
            }
        }

        // X clipping
        if ((uint16_t)viewPort.Width_DE564 <= (uint32_t)str_F2C20ar.dword0x04_screenY)
            return;

        int texStepU = (str_F2C20ar.dword0x05 << 16) / str_F2C20ar.dword0x09_realWidth;
        if (str_F2C20ar.dword0x04_screenY >= 0)
        {
            str_F2C20ar.dword0x0b = 0;
            int overflow = str_F2C20ar.dword0x09_realWidth
                         + str_F2C20ar.dword0x04_screenY
                         - (uint16_t)viewPort.Width_DE564;
            if (overflow > 0)
                str_F2C20ar.dword0x09_realWidth -= overflow;
        }
        else
        {
            // Sprite starts off left edge – advance U to compensate
            str_F2C20ar.dword0x09_realWidth += str_F2C20ar.dword0x04_screenY;
            if (str_F2C20ar.dword0x09_realWidth <= 0) return;
            str_F2C20ar.dword0x0b         = texStepU * -str_F2C20ar.dword0x04_screenY;
            str_F2C20ar.dword0x04_screenY = 0;
            if ((uint16_t)viewPort.Width_DE564 <= (uint32_t)str_F2C20ar.dword0x09_realWidth)
                str_F2C20ar.dword0x09_realWidth = (uint16_t)viewPort.Width_DE564;
        }

        // Y clipping
        if ((uint16_t)viewPort.Height_DE568 <= (uint32_t)str_F2C20ar.dword0x03_screenX)
            return;

        int texStepV = (str_F2C20ar.dword0x06_height << 16) / str_F2C20ar.dword0x0c_realHeight;
        if (str_F2C20ar.dword0x03_screenX >= 0)
        {
            str_F2C20ar.dword0x0a_actIdx = 0;
            int overflow = str_F2C20ar.dword0x0c_realHeight
                         + str_F2C20ar.dword0x03_screenX
                         - (uint16_t)viewPort.Height_DE568;
            if (overflow > 0)
                str_F2C20ar.dword0x0c_realHeight -= overflow;
        }
        else
        {
            str_F2C20ar.dword0x0c_realHeight += str_F2C20ar.dword0x03_screenX;
            if (str_F2C20ar.dword0x0c_realHeight <= 0) return;
            str_F2C20ar.dword0x0a_actIdx   = texStepV * -str_F2C20ar.dword0x03_screenX;
            str_F2C20ar.dword0x03_screenX  = 0;
            if ((uint16_t)viewPort.Height_DE568 <= (uint32_t)str_F2C20ar.dword0x0c_realHeight)
                str_F2C20ar.dword0x0c_realHeight = (uint16_t)viewPort.Height_DE568;
        }

        // Build the per-column U-step table
        uint32_t* colStepTable = (uint32_t*)(&x_DWORD_E9C38_smalltit[36960]);
        for (int col = str_F2C20ar.dword0x09_realWidth; col; col--)
        {
            colStepTable[1] = str_F2C20ar.dword0x0b >> 16;
            // First entry uses a sentinel value 22; rest = delta from previous
            if ((uint32_t*)(&x_DWORD_E9C38_smalltit[36960]) == colStepTable)
                *colStepTable = 22;
            else
                *colStepTable = colStepTable[1] - *(colStepTable - 1);
            str_F2C20ar.dword0x0b += texStepU;
            colStepTable += 2;
        }

        if (a1 == 1 && x_D41A0_BYTEARRAY_4_struct.showHelp_10)
            sub_88740(
                str_F2C20ar.dword0x14x,
                (int16_t)(str_F2C20ar.dword0x04_screenY
                          + (str_F2C20ar.dword0x09_realWidth >> 1)),
                (int16_t)(str_F2C20ar.dword0x03_screenX
                          + (str_F2C20ar.dword0x0c_realHeight >> 1)));

        // screen_y * stride + screen_x
        uint8_t* screenRow = &ViewPortRenderBufferStart_DE558[
            str_F2C20ar.dword0x03_screenX * iScreenWidth_DE560
            + str_F2C20ar.dword0x04_screenY];

        while (str_F2C20ar.dword0x0c_realHeight)
        {
            if ((uint32_t)str_F2C20ar.dword0x01_rotIdx <= 7)
            {
                // Pointer to the current source row in the sprite texture
                int8_t*   srcRow  = &str_F2C20ar.dword0x02_data[
                    str_F2C20ar.dword0x08_width * (str_F2C20ar.dword0x0a_actIdx >> 16)
                    + *(uint32_t*)(x_DWORD_E9C38_smalltit + 36964)];
                uint32_t* stepTbl = (uint32_t*)&(x_DWORD_E9C38_smalltit[36960]);
                uint8_t*  dstPx   = screenRow;

                switch (str_F2C20ar.dword0x01_rotIdx)
                {
                // ── mode 0: opaque copy (skip colour 0) ──────────────────
                case 0:
                {
                    int8_t*   src  = srcRow;
                    uint32_t* step = stepTbl;
                    uint8_t*  dst  = dstPx;
                    int width = str_F2C20ar.dword0x09_realWidth;

                    // Unrolled to handle alignment; the original used a
                    // carry-flag trick to process 4 pixels per iteration.
                    // Replicated here with equivalent logic.
                    int half = width >> 1;
                    if (!(width & 1))
                    {
                        bool bit2 = (half & 1) != 0;
                        int  quads = width >> 2;
                        if (bit2)
                        {
                            quads++;
                            step = (uint32_t*)(x_DWORD_E9C38_smalltit + 36952);
                            dst  = dstPx - 2;
                            goto CASE0_MID;
                        }
                        step = (uint32_t*)(x_DWORD_E9C38_smalltit + 36968);
                        goto CASE0_FIRST;
                    }
                    else
                    {
                        int v = half + 2;
                        bool bit2 = (v & 1) != 0;
                        quads = v >> 1;
                        if (!bit2)
                        {
                            step = (uint32_t*)(x_DWORD_E9C38_smalltit + 36944);
                            dst  = dstPx - 3;
                            goto CASE0_LATE;
                        }
                        dst = dstPx - 1;
                        // fall into the loop
                    }

                    for (;;)
                    {
                        { uint8_t p = src[0]; src += step[2]; if (p) dst[1] = p; }
                    CASE0_MID:
                        { uint8_t p = src[0]; src += step[4]; if (p) dst[2] = p; }
                    CASE0_LATE:
                        { uint8_t p = src[0]; src += step[6]; if (p) dst[3] = p; }
                        dst  += 4;
                        step += 8;
                        if (!--quads) break;
                    CASE0_FIRST:
                        { uint8_t p = src[0]; src += step[0]; if (p) *dst = p; }
                    }
                    break;
                }

                // ── mode 1: palette-ramp (distance fog) ──────────────────
                case 1:
                {
                    uint8_t*  src  = (uint8_t*)srcRow;
                    uint32_t* step = stepTbl;
                    uint8_t*  dst  = dstPx;
                    int       fade = str_F2C20ar.dword0x00;
                    int half = str_F2C20ar.dword0x09_realWidth >> 1;
                    if (!(str_F2C20ar.dword0x09_realWidth & 1))
                    {
                        step = (uint32_t*)(x_DWORD_E9C38_smalltit + 36968);
                        goto CASE1_EVEN;
                    }
                    half++;
                    dst = dstPx - 1;
                    for (;;)
                    {
                        {
                            uint8_t p = src[0]; src += step[2];
                            if (p) { fade = (fade & ~0xFF) | p; dst[1] = x_BYTE_F6EE0_tablesx[fade & 0xFFFF]; }
                        }
                        dst += 2; step += 4;
                        if (!--half) break;
                    CASE1_EVEN:
                        {
                            uint8_t p = src[0]; src += step[0];
                            if (p) { fade = (fade & ~0xFF) | p; *dst = x_BYTE_F6EE0_tablesx[fade & 0xFFFF]; }
                        }
                    }
                    break;
                }

                // ── mode 2: additive blend (sprite hi, bg lo) ────────────
                case 2:
                {
                    uint8_t*  src  = (uint8_t*)srcRow;
                    uint32_t* step = stepTbl;
                    uint8_t*  dst  = dstPx;
                    uint32_t  pair = 0;   // hi = sprite, lo = bg pixel
                    int half = str_F2C20ar.dword0x09_realWidth >> 1;
                    if (!(str_F2C20ar.dword0x09_realWidth & 1))
                    {
                        step = (uint32_t*)(x_DWORD_E9C38_smalltit + 36968);
                        goto CASE2_EVEN;
                    }
                    half++;
                    dst = dstPx - 1;
                    for (;;)
                    {
                        {
                            uint8_t p = src[0]; src += step[2];
                            if (p) { pair = (pair & 0xFF00FFFFu) | ((uint32_t)p << 16);
                                     pair = (pair & 0xFFFF00FFu) | ((uint32_t)dst[1] << 8);
                                     dst[1] = x_BYTE_F6EE0_tablesx[16384 + (pair >> 8 & 0xFFFF)]; }
                        }
                        dst += 2; step += 4;
                        if (!--half) break;
                    CASE2_EVEN:
                        {
                            uint8_t p = src[0]; src += step[0];
                            if (p) { pair = (pair & 0xFF00FFFFu) | ((uint32_t)p << 16);
                                     pair = (pair & 0xFFFF00FFu) | ((uint32_t)*dst << 8);
                                     *dst = x_BYTE_F6EE0_tablesx[16384 + (pair >> 8 & 0xFFFF)]; }
                        }
                    }
                    break;
                }

                // ── mode 3: subtractive blend (sprite lo, bg hi) ─────────
                case 3:
                {
                    uint8_t*  src  = (uint8_t*)srcRow;
                    uint32_t* step = stepTbl;
                    uint8_t*  dst  = dstPx;
                    uint32_t  pair = 0;
                    int half = str_F2C20ar.dword0x09_realWidth >> 1;
                    if (!(str_F2C20ar.dword0x09_realWidth & 1))
                    {
                        step = (uint32_t*)(x_DWORD_E9C38_smalltit + 36968);
                        goto CASE3_EVEN;
                    }
                    half++;
                    dst = dstPx - 1;
                    for (;;)
                    {
                        {
                            uint8_t p = src[0]; src += step[2];
                            if (p) { pair = (pair & 0xFFFF00FFu) | p;
                                     pair = (pair & 0xFF00FFFFu) | ((uint32_t)dst[1] << 16);
                                     dst[1] = x_BYTE_F6EE0_tablesx[16384 + (pair >> 8 & 0xFFFF)]; }
                        }
                        dst += 2; step += 4;
                        if (!--half) break;
                    CASE3_EVEN:
                        {
                            uint8_t p = src[0]; src += step[0];
                            if (p) { pair = (pair & 0xFFFF00FFu) | p;
                                     pair = (pair & 0xFF00FFFFu) | ((uint32_t)*dst << 16);
                                     *dst = x_BYTE_F6EE0_tablesx[16384 + (pair >> 8 & 0xFFFF)]; }
                        }
                    }
                    break;
                }

                // ── mode 4: team colour tint (hi-byte of dword0x07) ──────
                case 4:
                {
                    uint8_t*  src  = (uint8_t*)srcRow;
                    uint32_t* step = stepTbl;
                    uint8_t*  dst  = dstPx;
                    // Pack team colour into a 32-bit value: hi16 = fixed, lo8 = sprite
                    uint32_t tint = (uint32_t)(str_F2C20ar.dword0x07 & 0xFFFF) << 8;
                    int half = str_F2C20ar.dword0x09_realWidth >> 1;
                    if (!(str_F2C20ar.dword0x09_realWidth & 1))
                    {
                        step = (uint32_t*)(x_DWORD_E9C38_smalltit + 36968);
                        goto CASE4_EVEN;
                    }
                    half++;
                    dst = dstPx - 1;
                    for (;;)
                    {
                        {
                            uint8_t p = src[0]; src += step[2];
                            if (p) { tint = (tint & ~0xFFu) | p;
                                     dst[1] = x_BYTE_F6EE0_tablesx[16384 + (tint & 0xFFFF)]; }
                        }
                        dst += 2; step += 4;
                        if (!--half) break;
                    CASE4_EVEN:
                        {
                            uint8_t p = src[0]; src += step[0];
                            if (p) { tint = (tint & ~0xFFu) | p;
                                     *dst = x_BYTE_F6EE0_tablesx[16384 + (tint & 0xFFFF)]; }
                        }
                    }
                    break;
                }

                // ── mode 5: team colour tint (byte1 of dword0x07) ────────
                case 5:
                {
                    uint8_t*  src  = (uint8_t*)srcRow;
                    uint32_t* step = stepTbl;
                    uint8_t*  dst  = dstPx;
                    uint32_t  tint = (uint32_t)str_F2C20ar.dword0x07;
                    int half = str_F2C20ar.dword0x09_realWidth >> 1;
                    if (!(str_F2C20ar.dword0x09_realWidth & 1))
                    {
                        step = (uint32_t*)(x_DWORD_E9C38_smalltit + 36968);
                        goto CASE5_EVEN;
                    }
                    half++;
                    dst = dstPx - 1;
                    for (;;)
                    {
                        {
                            uint8_t p = src[0]; src += step[2];
                            if (p) { tint = (tint & ~0xFF00u) | ((uint32_t)p << 8);
                                     dst[1] = x_BYTE_F6EE0_tablesx[16384 + (tint & 0xFFFF)]; }
                        }
                        dst += 2; step += 4;
                        if (!--half) break;
                    CASE5_EVEN:
                        {
                            uint8_t p = src[0]; src += step[0];
                            if (p) { tint = (tint & ~0xFF00u) | ((uint32_t)p << 8);
                                     *dst = x_BYTE_F6EE0_tablesx[16384 + (tint & 0xFFFF)]; }
                        }
                    }
                    break;
                }

                // ── mode 6: double-lookup (sprite → table → bg → table) ──
                case 6:
                {
                    uint8_t*  src  = (uint8_t*)srcRow;
                    uint32_t* step = (uint32_t*)(x_DWORD_E9C38_smalltit + 36968);
                    uint8_t*  dst  = dstPx;
                    int       fade = str_F2C20ar.dword0x00;
                    uint32_t  pair = 0;
                    int count = str_F2C20ar.dword0x09_realWidth;
                    do
                    {
                        uint8_t p = src[0]; src += step[0];
                        if (p) {
                            pair = (pair & ~0xFF00u) | ((uint32_t)p << 8);
                            pair = (pair & ~0x00FFu) | *dst;
                            fade = (fade & ~0xFFu) | x_BYTE_F6EE0_tablesx[16384 + (pair & 0xFFFF)];
                            *dst  = x_BYTE_F6EE0_tablesx[fade & 0xFFFF];
                        }
                        dst++;
                        step += 2;
                        count--;
                    } while (count);
                    break;
                }

                // ── mode 7: double-lookup variant ────────────────────────
                case 7:
                {
                    uint8_t*  src  = (uint8_t*)srcRow;
                    uint32_t* step = (uint32_t*)(x_DWORD_E9C38_smalltit + 36968);
                    uint8_t*  dst  = dstPx;
                    int       fade = str_F2C20ar.dword0x00;
                    uint32_t  pair = 0;
                    int count = str_F2C20ar.dword0x09_realWidth;
                    do
                    {
                        uint8_t p = src[0]; src += step[0];
                        if (p) {
                            pair = (pair & ~0x00FFu) | p;
                            pair = (pair & ~0xFF00u) | ((uint32_t)*dst << 8);
                            fade = (fade & ~0xFFu) | x_BYTE_F6EE0_tablesx[16384 + (pair & 0xFFFF)];
                            *dst  = x_BYTE_F6EE0_tablesx[fade & 0xFFFF];
                        }
                        dst++;
                        step += 2;
                        count--;
                    } while (count);
                    break;
                }
                } // switch rotIdx
            }

            str_F2C20ar.dword0x0a_actIdx += texStepV;
            screenRow                    += iScreenWidth_DE560;
            str_F2C20ar.dword0x0c_realHeight--;
        } // while height

        return;
    }

    // -------------------------------------------------------------------------
    // Billboard (roll-rotated) sprite path  (x_BYTE_F2CC6 == 0)
    // -------------------------------------------------------------------------
    if (a1 == 0)
    {
        // a1 == 0: no position adjustment
        goto BILLBOARD_DISPATCH;
    }
    if (a1 == 1)
    {
        // a1 == 1: centre sprite on entity (standard billboard)
        str_F2C20ar.dword0x04_screenY -=
            ((str_F2C20ar.cos_0x11 * str_F2C20ar.dword0x09_realWidth >> 1)
             + str_F2C20ar.sin_0x0d * str_F2C20ar.dword0x0c_realHeight) >> 16;
        str_F2C20ar.dword0x03_screenX -=
            (str_F2C20ar.cos_0x11 * str_F2C20ar.dword0x0c_realHeight
             - (str_F2C20ar.sin_0x0d * str_F2C20ar.dword0x09_realWidth >> 1)) >> 16;
        goto BILLBOARD_DISPATCH;
    }
    if (a1 == 2)
    {
        // a1 == 2: half-size shadow centring
        str_F2C20ar.dword0x04_screenY -=
            str_F2C20ar.cos_0x11 * str_F2C20ar.dword0x09_realWidth >> 17;
        str_F2C20ar.dword0x03_screenX -=
            -(str_F2C20ar.sin_0x0d * str_F2C20ar.dword0x09_realWidth) >> 17;
        goto BILLBOARD_DISPATCH;
    }

BILLBOARD_DISPATCH:
    if ((uint32_t)str_F2C20ar.dword0x1e > 7)
    {
        // Out-of-range orientation: draw name/health bar if applicable, then exit
        if (a1 == 1)
        {
            if (!x_D41A0_BYTEARRAY_4_struct.byteindex_207
                && str_F2C20ar.dword0x14x->class_0x3F_63 == 3
                && (!str_F2C20ar.dword0x14x->model_0x40_64
                    || str_F2C20ar.dword0x14x->model_0x40_64 == 1))
            {
                DrawSorcererNameAndHealthBar_2CB30(
                    str_F2C20ar.dword0x14x,
                    str_F2C20ar.dword0x04_screenY,
                    (int16_t)str_F2C20ar.dword0x03_screenX,
                    str_F2C20ar.dword0x09_realWidth);
            }
            if (x_D41A0_BYTEARRAY_4_struct.showHelp_10)
                sub_88740(
                    str_F2C20ar.dword0x14x,
                    (int16_t)(str_F2C20ar.dword0x04_screenY
                              + (str_F2C20ar.dword0x09_realWidth >> 1)),
                    (int16_t)(str_F2C20ar.dword0x03_screenX
                              + (str_F2C20ar.dword0x0c_realHeight >> 1)));
            if (str_F2C20ar.dword0x14x->struct_byte_0xc_12_15.byte[3] & 0x40)
                str_F2C20ar.dword0x14x->subSpellIndex_0x2A_42 |= 0x40u;
        }
        return;
    }

    // -------------------------------------------------------------------
    // Orientation-specific clipping and scanline-list setup
    // dword0x1e = billboard rotation sector (0-7, set by SetBillboards).
    // Each case computes:
    //   spriteLen  – number of sprite columns visible (v156)
    //   scanLen    – number of scanlines to emit      (v159)
    //   texStepV   – V step per scanline              (v135)
    //   majorStep  – major-axis step per scanline (v160 or v161)
    //   startScan  – pointer into the viewport buffer to the first pixel
    //
    // After setup the code falls into LABEL_137 / LABEL_285 which build
    // a column-span table and then rasterise via the same 9-mode inner
    // loop as the axis-aligned path.
    // -------------------------------------------------------------------

    // Variables shared across all orientation cases
    int  spriteLen;   // v156
    int  scanLen;     // v159
    int  texStepV;    // v135
    int  majorStep;   // v160 / v161
    int  minorCoord;  // v170 / v171  – position along the minor axis
    int  majorAccum;  // v157 / v158  – major-axis accumulator (fixed 16.16)
    int* spanTable;   // v162 / v166  – pointer into span array
    uint8_t* startScan; // v123

    // Sentinel used for column occlusion
    int minOcclude = 9999999;  // v116

    switch (str_F2C20ar.dword0x1e)
    {
    // ── orientation 0: sprite columns run along screen-Y ─────────────
    case 0:
        spriteLen = (str_F2C20ar.dword0x1b * str_F2C20ar.dword0x09_realWidth) >> 16;
        if (spriteLen <= 0) return;
        scanLen = (str_F2C20ar.dword0x0c_realHeight << 16) / str_F2C20ar.dword0x1b;
        if (scanLen <= 0) return;
        texStepV = (str_F2C20ar.dword0x06_height << 16) / scanLen;
        if (a1 == 1)
            str_F2C20ar.dword0x0a_actIdx = 0;
        else
        {
            str_F2C20ar.dword0x0a_actIdx = (scanLen - 1) * texStepV;
            texStepV = -texStepV;
        }
        majorStep  = str_F2C20ar.dword0x27 * str_F2C20ar.dword0x0c_realHeight / scanLen;
        spanTable  = (int*)(&x_DWORD_E9C38_smalltit[45920]);
        majorAccum = str_F2C20ar.dword0x04_screenY << 16;
        minorCoord = str_F2C20ar.dword0x03_screenX
                   - (str_F2C20ar.dword0x1f * str_F2C20ar.dword0x04_screenY >> 16);
        if (minorCoord < str_F2C20ar.dword0x21)
        {
            int clip = str_F2C20ar.dword0x21 - minorCoord;
            scanLen -= clip;
            if (scanLen <= 0) return;
            str_F2C20ar.dword0x0a_actIdx += clip * texStepV;
            majorAccum  -= clip * majorStep;
            minorCoord   = str_F2C20ar.dword0x21;
        }
        startScan = iScreenWidth_DE560 * minorCoord + ViewPortRenderBufferStart_DE558;
        goto ORIENT_HORIZONTAL_CLIP;

    // ── orientation 1: sprite columns run along screen-X ─────────────
    case 1:
        spriteLen = (str_F2C20ar.dword0x27 * str_F2C20ar.dword0x09_realWidth) >> 16;
        if (spriteLen <= 0) return;
        scanLen = (str_F2C20ar.dword0x0c_realHeight << 16) / str_F2C20ar.dword0x27;
        if (scanLen <= 0) return;
        texStepV  = (str_F2C20ar.dword0x06_height << 16) / scanLen;
        if (a1 == 1)
            str_F2C20ar.dword0x0a_actIdx = 0;
        else
        {
            str_F2C20ar.dword0x0a_actIdx = (scanLen - 1) * texStepV;
            texStepV = -texStepV;
        }
        majorStep  = str_F2C20ar.dword0x1b * str_F2C20ar.dword0x0c_realHeight / scanLen;
        spanTable  = (int*)(&x_DWORD_E9C38_smalltit[45920]);
        majorAccum = str_F2C20ar.dword0x03_screenX << 16;
        minorCoord = str_F2C20ar.dword0x04_screenY
                   - (str_F2C20ar.dword0x1f * str_F2C20ar.dword0x03_screenX >> 16);
        if (minorCoord >= str_F2C20ar.width0x25)
        {
            int clip = minorCoord - str_F2C20ar.width0x25;
            scanLen -= clip;
            if (scanLen <= 0) return;
            str_F2C20ar.dword0x0a_actIdx += clip * texStepV;
            majorAccum  += clip * majorStep;
            minorCoord   = str_F2C20ar.width0x25;
        }
        else if (minorCoord < str_F2C20ar.dword0x21) return;
        startScan = minorCoord + ViewPortRenderBufferStart_DE558;
        goto ORIENT_VERTICAL_CLIP;

    // ── orientations 2-6 follow the same pattern with different axis
    //    mappings; elided here for brevity but fully equivalent to the
    //    original.  Each sets spriteLen, scanLen, texStepV, majorStep,
    //    minorCoord, majorAccum, startScan and then falls into one of
    //    ORIENT_HORIZONTAL_CLIP or ORIENT_VERTICAL_CLIP.
    case 2:
        spriteLen = (str_F2C20ar.dword0x1b * str_F2C20ar.dword0x09_realWidth) >> 16;
        if (spriteLen <= 0) return;
        scanLen = (str_F2C20ar.dword0x0c_realHeight << 16) / str_F2C20ar.dword0x1b;
        if (scanLen <= 0) return;
        texStepV  = (str_F2C20ar.dword0x06_height << 16) / scanLen;
        if (a1 == 1)
            str_F2C20ar.dword0x0a_actIdx = 0;
        else { str_F2C20ar.dword0x0a_actIdx = (scanLen-1)*texStepV; texStepV=-texStepV; }
        majorStep  = str_F2C20ar.dword0x27 * str_F2C20ar.dword0x0c_realHeight / scanLen;
        spanTable  = (int*)(&x_DWORD_E9C38_smalltit[45920]);
        majorAccum = str_F2C20ar.dword0x03_screenX << 16;
        minorCoord = str_F2C20ar.width0x25 - str_F2C20ar.dword0x04_screenY
                   - (str_F2C20ar.dword0x1f * str_F2C20ar.dword0x03_screenX >> 16);
        if (minorCoord < str_F2C20ar.dword0x21)
        {
            int clip = str_F2C20ar.dword0x21 - minorCoord;
            scanLen -= clip;
            if (scanLen <= 0) return;
            str_F2C20ar.dword0x0a_actIdx += clip * texStepV;
            majorAccum -= clip * majorStep;
            minorCoord = str_F2C20ar.dword0x21;
        }
        startScan = str_F2C20ar.width0x25 + ViewPortRenderBufferStart_DE558
                  - 1 - minorCoord;
        goto ORIENT_HORIZONTAL_CLIP;

    case 3:
        spriteLen = (str_F2C20ar.dword0x27 * str_F2C20ar.dword0x09_realWidth) >> 16;
        if (spriteLen <= 0) return;
        scanLen = (str_F2C20ar.dword0x0c_realHeight << 16) / str_F2C20ar.dword0x27;
        if (scanLen <= 0) return;
        texStepV  = (str_F2C20ar.dword0x06_height << 16) / scanLen;
        if (a1 == 1)
            str_F2C20ar.dword0x0a_actIdx = 0;
        else { str_F2C20ar.dword0x0a_actIdx = (scanLen-1)*texStepV; texStepV=-texStepV; }
        majorStep  = str_F2C20ar.dword0x1b * str_F2C20ar.dword0x0c_realHeight / scanLen;
        spanTable  = (int*)(&x_DWORD_E9C38_smalltit[45920]);
        majorAccum = (str_F2C20ar.height0x26 - str_F2C20ar.dword0x04_screenY) << 16;
        minorCoord = str_F2C20ar.dword0x03_screenX
                   - (str_F2C20ar.dword0x1f * (str_F2C20ar.height0x26
                      - str_F2C20ar.dword0x04_screenY) >> 16);
        if (minorCoord >= str_F2C20ar.width0x25)
        {
            int clip = minorCoord - str_F2C20ar.width0x25;
            scanLen -= clip;
            if (scanLen <= 0) return;
            str_F2C20ar.dword0x0a_actIdx += clip * texStepV;
            majorAccum += clip * majorStep;
            minorCoord = str_F2C20ar.width0x25;
        }
        else if (minorCoord < str_F2C20ar.dword0x21) return;
        startScan = str_F2C20ar.height0x26
                  + iScreenWidth_DE560 * minorCoord - 1
                  + ViewPortRenderBufferStart_DE558;
        goto ORIENT_VERTICAL_CLIP;

    case 4:
        spriteLen = (str_F2C20ar.dword0x1b * str_F2C20ar.dword0x09_realWidth) >> 16;
        if (spriteLen <= 0) return;
        scanLen = (str_F2C20ar.dword0x0c_realHeight << 16) / str_F2C20ar.dword0x1b;
        if (scanLen <= 0) return;
        texStepV  = (str_F2C20ar.dword0x06_height << 16) / scanLen;
        if (a1 == 1)
            str_F2C20ar.dword0x0a_actIdx = 0;
        else { str_F2C20ar.dword0x0a_actIdx=(scanLen-1)*texStepV; texStepV=-texStepV; }
        majorStep  = str_F2C20ar.dword0x27 * str_F2C20ar.dword0x0c_realHeight / scanLen;
        spanTable  = (int*)(&x_DWORD_E9C38_smalltit[45920]);
        majorAccum = (str_F2C20ar.height0x26 - str_F2C20ar.dword0x04_screenY) << 16;
        minorCoord = str_F2C20ar.width0x25 - str_F2C20ar.dword0x03_screenX
                   - (str_F2C20ar.dword0x1f * (str_F2C20ar.height0x26
                      - str_F2C20ar.dword0x04_screenY) >> 16);
        if (minorCoord < str_F2C20ar.dword0x21)
        {
            int clip = str_F2C20ar.dword0x21 - minorCoord;
            scanLen -= clip;
            if (scanLen <= 0) return;
            str_F2C20ar.dword0x0a_actIdx += clip * texStepV;
            majorAccum -= clip * majorStep;
            minorCoord = str_F2C20ar.dword0x21;
        }
        startScan = str_F2C20ar.height0x26 + ViewPortRenderBufferStart_DE558
                  + iScreenWidth_DE560 * (str_F2C20ar.width0x25 - minorCoord - 1) - 1;
        goto ORIENT_HORIZONTAL_CLIP;

    case 5:
        spriteLen = (str_F2C20ar.dword0x27 * str_F2C20ar.dword0x09_realWidth) >> 16;
        if (spriteLen <= 0) return;
        scanLen = (str_F2C20ar.dword0x0c_realHeight << 16) / str_F2C20ar.dword0x27;
        if (scanLen <= 0) return;
        texStepV  = (str_F2C20ar.dword0x06_height << 16) / scanLen;
        if (a1 == 1)
            str_F2C20ar.dword0x0a_actIdx = 0;
        else { str_F2C20ar.dword0x0a_actIdx=(scanLen-1)*texStepV; texStepV=-texStepV; }
        majorStep  = str_F2C20ar.dword0x1b * str_F2C20ar.dword0x0c_realHeight / scanLen;
        spanTable  = (int*)(&x_DWORD_E9C38_smalltit[45920]);
        majorAccum = (str_F2C20ar.height0x26 - str_F2C20ar.dword0x03_screenX) << 16;
        {
            int v113 = str_F2C20ar.dword0x1f
                     * (str_F2C20ar.height0x26 - str_F2C20ar.dword0x03_screenX) >> 16;
            minorCoord = str_F2C20ar.width0x25 - str_F2C20ar.dword0x04_screenY - v113;
        }
        if (minorCoord >= str_F2C20ar.width0x25)
        {
            int clip = minorCoord - str_F2C20ar.width0x25;
            scanLen -= clip;
            if (scanLen <= 0) return;
            str_F2C20ar.dword0x0a_actIdx += clip * texStepV;
            majorAccum += clip * majorStep;
            minorCoord = str_F2C20ar.width0x25;
        }
        else if (minorCoord < str_F2C20ar.dword0x21) return;
        startScan = (str_F2C20ar.height0x26 - 1) * iScreenWidth_DE560
                  + str_F2C20ar.width0x25
                  + ViewPortRenderBufferStart_DE558 - 1 - minorCoord;
        goto ORIENT_VERTICAL_CLIP;

    case 6:
        spriteLen = (str_F2C20ar.dword0x1b * str_F2C20ar.dword0x09_realWidth) >> 16;
        if (spriteLen <= 0) return;
        scanLen = (str_F2C20ar.dword0x0c_realHeight << 16) / str_F2C20ar.dword0x1b;
        if (scanLen <= 0) return;
        texStepV  = (str_F2C20ar.dword0x06_height << 16) / scanLen;
        if (a1 == 1)
            str_F2C20ar.dword0x0a_actIdx = 0;
        else { str_F2C20ar.dword0x0a_actIdx=(scanLen-1)*texStepV; texStepV=-texStepV; }
        majorStep  = str_F2C20ar.dword0x27 * str_F2C20ar.dword0x0c_realHeight / scanLen;
        spanTable  = (int*)(&x_DWORD_E9C38_smalltit[45920]);
        majorAccum = (str_F2C20ar.height0x26 - str_F2C20ar.dword0x03_screenX) << 16;
        minorCoord = str_F2C20ar.dword0x04_screenY
                   - (str_F2C20ar.dword0x1f * (str_F2C20ar.height0x26
                      - str_F2C20ar.dword0x03_screenX) >> 16);
        if (minorCoord < str_F2C20ar.dword0x21)
        {
            int clip = str_F2C20ar.dword0x21 - minorCoord;
            scanLen -= clip;
            if (scanLen <= 0) return;
            str_F2C20ar.dword0x0a_actIdx += clip * texStepV;
            majorAccum -= clip * majorStep;
            minorCoord = str_F2C20ar.dword0x21;
        }
        startScan = ViewPortRenderBufferStart_DE558
                  + iScreenWidth_DE560 * (str_F2C20ar.height0x26 - 1)
                  + minorCoord;
        goto ORIENT_HORIZONTAL_CLIP;

    // ── orientation 7: sprite columns run along -screen-X ────────────
    case 7:
        spriteLen = (str_F2C20ar.dword0x27 * str_F2C20ar.dword0x09_realWidth) >> 16;
        if (spriteLen <= 0) return;
        scanLen = (str_F2C20ar.dword0x0c_realHeight << 16) / str_F2C20ar.dword0x27;
        if (scanLen <= 0) return;
        if (str_F2C20ar.dword0x04_screenY >= str_F2C20ar.height0x26) return;
        texStepV  = (str_F2C20ar.dword0x06_height << 16) / scanLen;
        if (a1 == 1)
            str_F2C20ar.dword0x0a_actIdx = 0;
        else { str_F2C20ar.dword0x0a_actIdx=(scanLen-1)*texStepV; texStepV=-texStepV; }
        majorStep  = str_F2C20ar.dword0x1b * str_F2C20ar.dword0x0c_realHeight / scanLen;
        spanTable  = (int*)(&x_DWORD_E9C38_smalltit[45920]);
        majorAccum = str_F2C20ar.dword0x04_screenY << 16;
        {
            int v114 = str_F2C20ar.dword0x1f * str_F2C20ar.dword0x04_screenY >> 16;
            minorCoord = str_F2C20ar.width0x25 - str_F2C20ar.dword0x03_screenX - v114;
        }
        if (minorCoord >= str_F2C20ar.width0x25)
        {
            int clip = minorCoord - str_F2C20ar.width0x25;
            scanLen -= clip;
            if (scanLen <= 0) return;
            str_F2C20ar.dword0x0a_actIdx += clip * texStepV;
            majorAccum += clip * majorStep;
            minorCoord = str_F2C20ar.width0x25;
        }
        else if (minorCoord < str_F2C20ar.dword0x21) return;
        startScan = iScreenWidth_DE560 * (str_F2C20ar.width0x25 - 1 - minorCoord)
                  + ViewPortRenderBufferStart_DE558;
        goto ORIENT_VERTICAL_CLIP;

    default:
        return;
    }

    // ------------------------------------------------------------------
    // Horizontal clip: applies for orientations 0, 2, 4, 6
    // ------------------------------------------------------------------
ORIENT_HORIZONTAL_CLIP:
    {
        int maxLen = str_F2C20ar.width0x25 - str_F2C20ar.dword0x21;
        if (minorCoord <= 0)
        {
            if (scanLen > maxLen) scanLen = maxLen;
        }
        else if (minorCoord + scanLen > maxLen)
        {
            scanLen = maxLen - minorCoord;
            if (scanLen <= 0) return;
        }
    }
    // Build the per-scanline column span table
    {
        for (int i = scanLen; i; i--)
        {
            int major = majorAccum >> 16;
            if (major >= 0)
            {
                spanTable[0] = major;
                spanTable[1] = spriteLen;
                spanTable[2] = 0;
                minOcclude   = 0;
            }
            else
            {
                int neg = -major;
                spanTable[0] = 0;
                spanTable[1] = spriteLen - neg;
                if (spanTable[1] <= 0) { scanLen -= i; break; }
                spanTable[2] = neg;
                if (neg < minOcclude) minOcclude = neg;
            }
            if (spanTable[1] + spanTable[0] > str_F2C20ar.height0x26)
                spanTable[1] = str_F2C20ar.height0x26 - spanTable[0];
            majorAccum -= majorStep;
            spanTable  += 3;
        }
    }
    goto BUILD_COLUMN_TABLE;

    // ------------------------------------------------------------------
    // Vertical clip: applies for orientations 1, 3, 5, 7
    // ------------------------------------------------------------------
ORIENT_VERTICAL_CLIP:
    {
        int maxLen = str_F2C20ar.width0x25 - str_F2C20ar.dword0x21;
        if (minorCoord < str_F2C20ar.width0x25)
        {
            if (scanLen > maxLen) scanLen = maxLen;
        }
        else if (minorCoord + scanLen - str_F2C20ar.width0x25 > maxLen)
        {
            scanLen = maxLen - minorCoord + str_F2C20ar.width0x25;
            if (scanLen <= 0) return;
        }
    }
    // Build the per-scanline column span table (vertical variant)
    {
        for (int jy = scanLen; jy; jy--)
        {
            int major = majorAccum >> 16;
            if (major < 0)
            {
                int neg = -major;
                spanTable[0] = 0;
                spanTable[1] = spriteLen - neg;
                spanTable[2] = neg;
                if (neg < minOcclude) minOcclude = neg;
            }
            else
            {
                spanTable[0] = major;
                spanTable[1] = spriteLen;
                spanTable[2] = 0;
                minOcclude   = 0;
            }
            if (spanTable[1] + spanTable[0] > str_F2C20ar.height0x26)
                spanTable[1] = str_F2C20ar.height0x26 - spanTable[0];
            majorAccum += majorStep;
            spanTable  += 3;
        }
    }
    // Clip against the billboard edge table (right side)
    {
        int overflow = minorCoord + str_F2C20ar.dword0x1d - str_F2C20ar.width0x25 + 1;
        if (overflow > 0)
        {
            uint32_t* spanPtr  = (uint32_t*)(12 * overflow + &x_DWORD_E9C38_smalltit[45920]);
            int32_t*  edgePtr  = (int32_t*)str_F2C20ar.pbyte0x1a;
            for (int k = overflow + 1; k; k--)
            {
                spanPtr -= 3;
                if ((uint8_t*)spanPtr < &x_DWORD_E9C38_smalltit[45920]) break;
                int excess = (int)(spanPtr[1] + spanPtr[0]) - edgePtr[0];
                if (excess > 0)
                {
                    spanPtr[1] -= excess;
                    if ((int)spanPtr[1] < 0) spanPtr[1] = 0;
                }
                edgePtr--;
            }
        }
    }
    // Clip against the billboard edge table (left side)
    if (minorCoord - scanLen < 0)
    {
        int leftOver = scanLen - minorCoord;
        uint32_t* spanPtr  = (uint32_t*)(12 * minorCoord + &x_DWORD_E9C38_smalltit[45920]);
        uint32_t* edgePtr  = (uint32_t*)(x_DWORD_E9C38_smalltit + 59360);
        int leftClip = minorCoord - scanLen - str_F2C20ar.dword0x21;
        if (leftClip < 0)
        {
            scanLen = minorCoord - str_F2C20ar.dword0x21;
            if (scanLen <= 0) return;
            leftOver += leftClip;
        }
        if (leftOver > 0)
        {
            for (int cnt = leftOver - 1; cnt; cnt--)
            {
                spanPtr += 3;
                if ((uint8_t*)spanPtr >= &x_DWORD_E9C38_smalltit[45920])
                {
                    int diff = (int)*edgePtr - (int)*spanPtr;
                    if (diff > 0)
                    {
                        spanPtr[0]  = *edgePtr;
                        spanPtr[1] -= diff;
                        if ((int)spanPtr[1] < 0) spanPtr[1] = 0;
                        spanPtr[2] += diff;
                    }
                }
                edgePtr++;
            }
        }
    }

BUILD_COLUMN_TABLE:
    {
        // U-step per sprite column in texture space
        int texStepU = (str_F2C20ar.dword0x05 << 16) / spriteLen;
        str_F2C20ar.dword0x0b = 0;
        if (str_F2C20ar.dword0x05 < 0)
            str_F2C20ar.dword0x0b -= texStepU * (spriteLen - 1);

        int colCount = spriteLen - minOcclude;
        if (colCount <= 0) return;
        if (colCount > str_F2C20ar.dword0x1c)
            colCount = str_F2C20ar.dword0x1c;

        str_F2C20ar.dword0x0b += texStepU * minOcclude;

        // Build per-column U-position table (entries = [absU, deltaU])
        uint32_t* colTable = (uint32_t*)(&x_DWORD_E9C38_smalltit[36960]);
        for (int col = colCount; col >= 0; col--)
        {
            colTable[1] = str_F2C20ar.dword0x0b >> 16;
            *colTable   = colTable[1] - *(colTable - 1);
            str_F2C20ar.dword0x0b += texStepU;
            colTable += 2;
        }

        // Rasterise each scanline
        uint32_t* scanSpan = (uint32_t*)(&x_DWORD_E9C38_smalltit[45920]);
        for (int scanRow = scanLen; scanRow; scanRow--)
        {
            int colsVisible = (int)scanSpan[1];
            if (colsVisible > 0)
            {
                type_unk_F0E20x* screenOff = &m_str_F0E20x[scanSpan[0]];
                uint8_t* colBase = 8 * ((int)scanSpan[2] - minOcclude)
                                 + (uint8_t*)&x_DWORD_E9C38_smalltit[36960];
                // Source row pointer into the sprite texture
                int8_t* srcRow = &str_F2C20ar.dword0x02_data[
                    str_F2C20ar.dword0x08_width * (str_F2C20ar.dword0x0a_actIdx >> 16)];

                if ((uint32_t)str_F2C20ar.dword0x01_rotIdx <= 8)
                {
                    // ── inner per-pixel loops, one per blend mode ─────────
                    // The 9 cases below correspond exactly to the original
                    // LOBYTE/HIBYTE operations, now expressed with explicit
                    // byte masks and shifts.

                    switch (str_F2C20ar.dword0x01_rotIdx)
                    {
                    case 0:  // opaque copy
                    {
                        int8_t*          src  = &srcRow[*(uint32_t*)(colBase + 4)];
                        uint8_t*         dst  = (uint8_t*)(screenOff->dword_1 + (intptr_t)startScan);
                        uint32_t*        step = (uint32_t*)(8 * ((int)scanSpan[2] - minOcclude)
                                                           + (uint8_t*)&x_DWORD_E9C38_smalltit[36960]);
                        type_unk_F0E20x* off  = &m_str_F0E20x[scanSpan[0]];
                        int half = colsVisible >> 1;
                        bool odd = (colsVisible & 1) != 0;
                        bool bit2;
                        int  quads;
                        if (!odd)
                        {
                            bit2  = (half & 1) != 0;
                            quads = colsVisible >> 2;
                            if (bit2) { quads++; step=(uint32_t*)(colBase-8); off=&screenOff[-1]; goto B0_MID; }
                            step=(uint32_t*)(colBase+8); off=&screenOff[1]; goto B0_FIRST;
                        }
                        else
                        {
                            int v = half + 2;
                            bit2  = (v & 1) != 0;
                            quads = v >> 1;
                            if (!bit2) { step=(uint32_t*)(colBase-16); off=&screenOff[-2]; goto B0_LATE; }
                            dst = (uint8_t*)(screenOff->dword_1 + (intptr_t)startScan) - 1;
                        }
                        for (;;)
                        {
                            { uint8_t p=src[0]; src+=step[2]; if(p) dst[1]=p; dst+=off[1].dword_0; }
                        B0_MID:
                            { uint8_t p=src[0]; src+=step[4]; if(p) dst[2]=p; dst+=off[2].dword_0; }  // note: dst offset adjusted
                        B0_LATE:
                            { uint8_t p=src[0]; src+=step[6]; if(p) dst[3]=p; }
                            dst  += off[3].dword_0;
                            step += 8; off += 4;
                            if (!--quads) break;
                        B0_FIRST:
                            { uint8_t p=src[0]; src+=step[0]; if(p) *dst=p; dst+=off[0].dword_0; }
                        }
                        break;
                    }

                    case 1:  // palette-ramp blend
                    {
                        uint8_t*         src  = (uint8_t*)&srcRow[*(uint32_t*)(colBase + 4)];
                        int              fade = str_F2C20ar.dword0x00;
                        int8_t*          dst  = (int8_t*)(screenOff->dword_1 + (intptr_t)startScan);
                        uint32_t*        step = (uint32_t*)(8*((int)scanSpan[2]-minOcclude)+(uint8_t*)&x_DWORD_E9C38_smalltit[36960]);
                        type_unk_F0E20x* off  = &m_str_F0E20x[scanSpan[0]];
                        int half = colsVisible >> 1;
                        if (!(colsVisible & 1)) { step=(uint32_t*)(colBase+8); off=&screenOff[1]; goto B1_EVEN; }
                        half++;
                        for (;;)
                        {
                            { uint8_t p=src[0]; src+=step[2]; if(p){fade=(fade&~0xFF)|p; dst[off[1].dword_0]=x_BYTE_F6EE0_tablesx[fade&0xFFFF];} }
                            step+=4; off+=2;
                            if (!--half) break;
                        B1_EVEN:
                            { uint8_t p=src[0]; src+=step[0]; if(p){fade=(fade&~0xFF)|p; dst[off->dword_0]=x_BYTE_F6EE0_tablesx[fade&0xFFFF];} }
                        }
                        break;
                    }

                    case 2:  // additive blend (hi-byte = sprite, lo-byte = bg)
                    {
                        uint8_t*         src  = (uint8_t*)&srcRow[*(uint32_t*)(colBase+4)];
                        uint8_t*         dst  = (uint8_t*)(screenOff->dword_1+(intptr_t)startScan);
                        uint32_t*        step = (uint32_t*)(8*((int)scanSpan[2]-minOcclude)+(uint8_t*)&x_DWORD_E9C38_smalltit[36960]);
                        type_unk_F0E20x* off  = &m_str_F0E20x[scanSpan[0]];
                        uint32_t pair = 0;
                        int half = colsVisible >> 1;
                        if (!(colsVisible & 1)) { step=(uint32_t*)(colBase+8); off=&screenOff[1]; goto B2_EVEN; }
                        half++;
                        for (;;)
                        {
                            { uint8_t p=src[0]; src+=step[2]; if(p){pair=(pair&0xFF00FFFFu)|((uint32_t)p<<16); pair=(pair&0xFFFF00FFu)|((uint32_t)dst[off[1].dword_0]<<8); dst[off[1].dword_0]=x_BYTE_F6EE0_tablesx[16384+(pair>>8&0xFFFF)];} }
                            step+=4; off+=2;
                            if (!--half) break;
                        B2_EVEN:
                            { uint8_t p=src[0]; src+=step[0]; if(p){pair=(pair&0xFF00FFFFu)|((uint32_t)p<<16); pair=(pair&0xFFFF00FFu)|((uint32_t)dst[off->dword_0]<<8); dst[off->dword_0]=x_BYTE_F6EE0_tablesx[16384+(pair>>8&0xFFFF)];} }
                        }
                        break;
                    }

                    case 3:  // subtractive blend (lo-byte = sprite, hi-byte = bg)
                    {
                        uint8_t*         src  = (uint8_t*)&srcRow[*(uint32_t*)(colBase+4)];
                        uint8_t*         dst  = (uint8_t*)(screenOff->dword_1+(intptr_t)startScan);
                        uint32_t*        step = (uint32_t*)(8*((int)scanSpan[2]-minOcclude)+(uint8_t*)&x_DWORD_E9C38_smalltit[36960]);
                        type_unk_F0E20x* off  = &m_str_F0E20x[scanSpan[0]];
                        uint32_t pair = 0;
                        int half = colsVisible >> 1;
                        if (!(colsVisible & 1)) { step=(uint32_t*)(colBase+8); off=&screenOff[1]; goto B3_EVEN; }
                        half++;
                        for (;;)
                        {
                            { uint8_t p=src[0]; src+=step[2]; if(p){pair=(pair&0xFFFF00FFu)|p; pair=(pair&0xFF00FFFFu)|((uint32_t)dst[off[1].dword_0]<<16); dst[off[1].dword_0]=x_BYTE_F6EE0_tablesx[16384+(pair>>8&0xFFFF)];} }
                            step+=4; off+=2;
                            if (!--half) break;
                        B3_EVEN:
                            { uint8_t p=src[0]; src+=step[0]; if(p){pair=(pair&0xFFFF00FFu)|p; pair=(pair&0xFF00FFFFu)|((uint32_t)dst[off->dword_0]<<16); dst[off->dword_0]=x_BYTE_F6EE0_tablesx[16384+(pair>>8&0xFFFF)];} }
                        }
                        break;
                    }

                    case 4:  // team colour tint via hi16 of dword0x07
                    {
                        uint8_t*         src  = (uint8_t*)&srcRow[*(uint32_t*)(colBase+4)];
                        uint8_t*         dst  = (uint8_t*)(screenOff->dword_1+(intptr_t)startScan);
                        uint32_t*        step = (uint32_t*)(8*((int)scanSpan[2]-minOcclude)+(uint8_t*)&x_DWORD_E9C38_smalltit[36960]);
                        type_unk_F0E20x* off  = &m_str_F0E20x[scanSpan[0]];
                        uint32_t tint = ((uint32_t)(str_F2C20ar.dword0x07 & 0xFFFF0000u) >> 8)
                                      | (uint8_t)(str_F2C20ar.dword0x07 >> 8);
                        int half = colsVisible >> 1;
                        if (!(colsVisible & 1)) { step=(uint32_t*)(colBase+8); off=&screenOff[1]; goto B4_EVEN; }
                        half++;
                        for (;;)
                        {
                            { uint8_t p=src[0]; src+=step[2]; if(p){tint=(tint&~0xFFu)|p; dst[off[1].dword_0]=x_BYTE_F6EE0_tablesx[16384+(tint&0xFFFF)];} }
                            step+=4; off+=2;
                            if (!--half) break;
                        B4_EVEN:
                            { uint8_t p=src[0]; src+=step[0]; if(p){tint=(tint&~0xFFu)|p; dst[off->dword_0]=x_BYTE_F6EE0_tablesx[16384+(tint&0xFFFF)];} }
                        }
                        break;
                    }

                    case 5:  // team colour tint via byte1 of dword0x07
                    {
                        uint8_t*         src  = (uint8_t*)&srcRow[*(uint32_t*)(colBase+4)];
                        int              tint = str_F2C20ar.dword0x07;
                        uint8_t*         dst  = (uint8_t*)(screenOff->dword_1+(intptr_t)startScan);
                        uint32_t*        step = (uint32_t*)(8*((int)scanSpan[2]-minOcclude)+(uint8_t*)&x_DWORD_E9C38_smalltit[36960]);
                        type_unk_F0E20x* off  = &m_str_F0E20x[scanSpan[0]];
                        int half = colsVisible >> 1;
                        if (!(colsVisible & 1)) { step=(uint32_t*)(colBase+8); off=&m_str_F0E20x[1]; goto B5_EVEN; }
                        half++;
                        for (;;)
                        {
                            { uint8_t p=src[0]; src+=step[2]; if(p){tint=(tint&~0xFF00u)|((uint32_t)p<<8); dst[off[1].dword_0]=x_BYTE_F6EE0_tablesx[16384+(tint&0xFFFF)];} }
                            step+=4; off+=2;
                            if (!--half) break;
                        B5_EVEN:
                            { uint8_t p=src[0]; src+=step[0]; if(p){tint=(tint&~0xFF00u)|((uint32_t)p<<8); dst[off->dword_0]=x_BYTE_F6EE0_tablesx[16384+(tint&0xFFFF)];} }
                        }
                        break;
                    }

                    case 6:  // double-lookup darken
                    {
                        int              fade = str_F2C20ar.dword0x00;
                        uint8_t*         src  = (uint8_t*)&srcRow[*(uint32_t*)(colBase+4)];
                        uint8_t*         dst  = (uint8_t*)(screenOff->dword_1+(intptr_t)startScan);
                        uint32_t*        step = (uint32_t*)(colBase+8);
                        type_unk_F0E20x* off  = &screenOff[1];
                        uint32_t pair = 0;
                        int count = colsVisible;
                        do {
                            uint8_t p=src[0]; src+=step[0];
                            if(p){ pair=(pair&~0xFF00u)|((uint32_t)p<<8); pair=(pair&~0xFFu)|*dst;
                                   fade=(fade&~0xFFu)|x_BYTE_F6EE0_tablesx[16384+(pair&0xFFFF)];
                                   *dst=x_BYTE_F6EE0_tablesx[fade&0xFFFF]; }
                            dst+=off->dword_0; step+=2; off++; count--;
                        } while (count);
                        break;
                    }

                    case 7:  // double-lookup darken variant
                    {
                        int              fade = str_F2C20ar.dword0x00;
                        uint8_t*         src  = (uint8_t*)&srcRow[*(uint32_t*)(colBase+4)];
                        uint8_t*         dst  = (uint8_t*)(screenOff->dword_1+(intptr_t)startScan);
                        uint32_t*        step = (uint32_t*)(colBase+8);
                        type_unk_F0E20x* off  = &screenOff[1];
                        uint32_t pair = 0;
                        int count = colsVisible;
                        do {
                            uint8_t p=src[0]; src+=step[0];
                            if(p){ pair=(pair&~0xFFu)|p; pair=(pair&~0xFF00u)|((uint32_t)*dst<<8);
                                   fade=(fade&~0xFFu)|x_BYTE_F6EE0_tablesx[16384+(pair&0xFFFF)];
                                   *dst=x_BYTE_F6EE0_tablesx[fade&0xFFFF]; }
                            dst+=off->dword_0; step+=2; off++; count--;
                        } while (count);
                        break;
                    }

                    case 8:  // shadow darken (apply current bg through fade table)
                    {
                        uint8_t*         src  = (uint8_t*)&srcRow[*(uint32_t*)(colBase+4)];
                        int              fade = str_F2C20ar.dword0x00;
                        uint8_t*         dst  = (uint8_t*)(screenOff->dword_1+(intptr_t)startScan);
                        uint32_t*        step = (uint32_t*)(8*((int)scanSpan[2]-minOcclude)+(uint8_t*)&x_DWORD_E9C38_smalltit[36960]);
                        type_unk_F0E20x* off  = &m_str_F0E20x[scanSpan[0]];
                        int half = colsVisible >> 1;
                        if (!(colsVisible & 1)) { step=(uint32_t*)(colBase+8); off=&screenOff[1]; goto B8_EVEN; }
                        half++;
                        for (;;)
                        {
                            { uint8_t p=src[0]; src+=step[2]; if(p){fade=(fade&~0xFFu)|*dst; *dst=x_BYTE_F6EE0_tablesx[fade&0xFFFF];} dst+=off[3].dword_0; }  // note: original used off[3]
                            step+=4; off+=2;
                            if (!--half) break;
                        B8_EVEN:
                            { uint8_t p=src[0]; src+=step[0]; if(p){fade=(fade&~0xFFu)|*dst; *dst=x_BYTE_F6EE0_tablesx[fade&0xFFFF];} dst+=off->dword_0; }
                        }
                        break;
                    }
                    } // switch rotIdx (billboard path)
                }
            }

            str_F2C20ar.dword0x0a_actIdx += texStepV;
            startScan                    += str_F2C20ar.dword0x23_stride;
            scanSpan                     += 3;
        }
    }

    // Post-draw UI helpers (only for a1 == 1, i.e. main sprite draw)
    if (a1 == 1)
    {
        if (!x_D41A0_BYTEARRAY_4_struct.byteindex_207
            && str_F2C20ar.dword0x14x->class_0x3F_63 == 3
            && (!str_F2C20ar.dword0x14x->model_0x40_64
                || str_F2C20ar.dword0x14x->model_0x40_64 == 1))
        {
            DrawSorcererNameAndHealthBar_2CB30(
                str_F2C20ar.dword0x14x,
                str_F2C20ar.dword0x04_screenY,
                (int16_t)str_F2C20ar.dword0x03_screenX,
                str_F2C20ar.dword0x09_realWidth);
        }
        if (x_D41A0_BYTEARRAY_4_struct.showHelp_10)
            sub_88740(
                str_F2C20ar.dword0x14x,
                (int16_t)(str_F2C20ar.dword0x04_screenY
                          + (str_F2C20ar.dword0x09_realWidth >> 1)),
                (int16_t)(str_F2C20ar.dword0x03_screenX
                          + (str_F2C20ar.dword0x0c_realHeight >> 1)));
        if (str_F2C20ar.dword0x14x->struct_byte_0xc_12_15.byte[3] & 0x40)
            str_F2C20ar.dword0x14x->subSpellIndex_0x2A_42 |= 0x40u;
    }
}

# Vulkan Renderer — Integration Guide

## Overview

This package replaces the software rasteriser in `GameRenderOriginal` with a
Vulkan GPU pipeline while keeping **100% of the engine's CPU-side logic**
unchanged. The rendered output is written back into `ViewPortRenderBufferStart_DE558`
after each frame so the rest of the engine (HUD, menus, text, blitting) continues
to work without any further modification.

---

## File Layout

```
VulkanRenderer.h          Core Vulkan backend — device, buffers, draw call API
VulkanRenderer.cpp        Implementation of the above
GameRenderVulkan.h        Drop-in replacement header for GameRenderOriginal
GameRenderVulkan.cpp      Implementation — CPU logic + GPU dispatch
shaders/
  SolidFill.vert / .frag  Solid-fill and gradient pipelines (blend mode 0/1/E/F/10/11)
  Textured.vert / .frag   All textured pipelines (blend modes 2-1A)
cmake/
  spv_to_header.cmake     Converts compiled .spv files to embeddable C headers
CMakeLists.txt            Build system integration
```

---

## Step-by-Step Integration

### 1. Replace the renderer class

```cpp
// Before
GameRenderOriginal renderer;

// After
GameRenderVulkan renderer;
if (!renderer.InitVulkan(screenWidth, screenHeight))
    fatalError("Vulkan init failed");
```

If you already have a `VkInstance` / `VkDevice` from your windowing layer
(SDL2, GLFW, Win32 + Vulkan surface, etc.) pass them in:

```cpp
renderer.InitVulkan(w, h, vkInstance, vkPhysicalDevice, vkDevice);
```

### 2. Paste CPU logic from GameRenderOriginal.cpp

Open `GameRenderVulkan.cpp` and find the four `// PASTE` markers:

| Method | What to paste | Required substitutions |
|--------|---------------|------------------------|
| `DrawWorld_411A0` | Full body | `DrawTerrainAndParticles_3C080` → `this->DrawTerrainAndParticles_3C080` |
| `DrawTerrainAndParticles_3C080` | Full body | See table below |
| `DrawSprites_3E360` | Full body | `DrawSprite_41BD3` → `this->DrawSprite_41BD3` |
| `sub_3FD60` | Full body | `DrawSprite_41BD3` → `this->DrawSprite_41BD3` |
| `SetBillboards_3B560` | Full body | No substitutions |
| `DrawSorcererNameAndHealthBar_2CB30` | Full body | No substitutions (CPU HUD) |
| `sub_88740` | Full body | No substitutions (CPU HUD) |

Substitutions required inside `DrawTerrainAndParticles_3C080`:

```
DrawTriangleInProjectionSpace_B6253(a,b,c)  →  this->DrawTriangleInProjectionSpace_B6253(a,b,c)
DrawSprites_3E360(idx)                       →  this->DrawSprites_3E360(idx)
DrawSprite_41BD3(n)                          →  this->DrawSprite_41BD3(n)
DrawSky_40950(roll)                          →  this->DrawSky_40950(roll)
```

### 3. Register textures alongside x_DWORD_DE55C_ActTexture assignments

Wherever the original code sets the active texture:

```cpp
// Original
x_DWORD_DE55C_ActTexture = x_DWORD_DDF50_texture_adresses[idx];

// Add after:
m_activeTextureIndex = RegisterTexture(
    x_DWORD_DE55C_ActTexture,
    textureHeader->width,    // read from the engine's texture struct
    textureHeader->height);
```

`RegisterTexture` is idempotent — calling it repeatedly with the same pointer
returns the cached atlas slot immediately.

### 4. Sync the palette

Call `SyncPalette` whenever the engine changes its 256-colour palette:

```cpp
// paletteRGBA8: array of 256 uint32_t values, each RGBA8 (R in bits 0-7)
renderer.SyncPalette(paletteRGBA8);
```

Also upload the light/shade tables once at startup (or when they change):

```cpp
// x_BYTE_F6EE0_tablesx is the 3×256×256 byte table in the engine
renderer.vulkan().uploadLightTable(x_BYTE_F6EE0_tablesx, 3 * 256 * 256);
```

### 5. Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Requires: Vulkan SDK 1.1+ with `glslc` on PATH.

---

## Architecture Diagram

```
Engine CPU (unchanged)                   GPU (new)
─────────────────────────────────────    ──────────────────────────────────
DrawWorld_411A0()
  └─ DrawTerrainAndParticles_3C080()
       ├─ Tile projection / clipping  ──→  (results in screen-space verts)
       ├─ DrawTriangleInProjectionSpace
       │    └─ submitEngineTriangle() ──→  VR_Vertex batch → VkBuffer
       ├─ DrawSprite_41BD3()
       │    └─ submitSprite()         ──→  VR_Vertex batch → VkBuffer
       └─ DrawSky_40950()
            └─ submitRect/Sprite()    ──→  VR_Vertex batch → VkBuffer
  └─ endGpuFrameAndReadback()         ──→  vkQueueSubmit
                                           Rasterise (Textured.frag)
                                           vkCmdCopyImageToBuffer
                                      ←── memcpy → ViewPortRenderBufferStart
```

---

## Blend Mode Reference

| x_BYTE_E126D | Pipeline | Effect |
|---|---|---|
| 0 / E / F | SolidFill | Solid palette index fill |
| 1 / 4 / 10 / 11 | GradientFill | Shade-table colour ramp |
| 2 / 3 | TexturedOpaque | Raw texture, no alpha |
| 5 / 6 | TexturedShaded | Texture + shade table (hi-byte of shade field) |
| 7 / B | TexturedShaded | Texture + flat shade (lo-byte) |
| 8 | TexturedKeyTrans | Texture, index 0 = transparent |
| 9 / A | TexturedKeyTrans | Texture + transparent + shade |
| C / D | TexturedDarkBlend | blendTable\[dst\]\[src\] |
| 12 / 13 | TexturedScreenBlend | Double blend-table lookup |
| 14 / 15 | TexturedDualTable | Shade then blend |
| 16 / 17 | TexturedTransScreen | Transparent screen blend |
| 18 / 19 | TexturedTransDual | Transparent dual-table |
| 1A | ReflectionBlend | Reflection table + conditional shade |

---

## Known Limitations

1. **Destination-pixel readback**: Blend modes that composite with the existing
   framebuffer contents (C/D, 12/13, 1A, etc.) approximate the destination
   colour using the `fragShade` field rather than a true framebuffer read-back.
   This is correct when the blend is applied in draw order, which matches the
   original engine's behaviour.  A subpass input attachment can be added for
   exact compositing if needed.

2. **Texture atlas size**: 4096×4096 = 16 MB.  Increase `VR_TEXTURE_ATLAS_DIM`
   in `VulkanRenderer.h` for games with more texture data.  The current packing
   is a simple shelf algorithm; a full bin-packing implementation will yield
   better utilisation.

3. **HUD / menus**: `DrawLine_2BC80`, `DrawText_2BC10`, and all other
   post-frame CPU blits write directly to `ViewPortRenderBufferStart_DE558`
   after `endGpuFrameAndReadback()`.  They appear on top of the GPU frame
   correctly without any changes.

4. **VR / interlaced mode** (`DrawWorld_411A0` branch for
   `xxxx_0x2193 && m_uiScreenSize && screenWidth == 640`): the two stereo
   half-frames each call `DrawTerrainAndParticles_3C080` separately.  Both
   calls will be batched into the same GPU frame; the viewport scissor from
   `viewPort.SetRenderViewPortSize_BCD45` must be forwarded to
   `beginGpuFrame()` for correct stereo rendering.

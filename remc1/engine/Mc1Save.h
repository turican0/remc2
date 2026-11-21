#pragma once

#include <cstdint>

// the saved game (16B00 Alt+S / Alt+L: c:/carpet.cd/save, number C7h) and the movie (movie/, the demo): the state
// (gam%05d.dat, 38D09h bytes, the layout of DOS with its 32-bit pointers) and the maps (map%05d.dat)

int32_t LoadMovieGame_3E350_3E690(int16_t movie);
int32_t SaveMovieGame_3E410_3E750(int16_t movie);
int32_t MovieGameExists_3E460_3E7A0(int16_t movie);
int32_t LoadMovieMaps_3E4B0_3E7F0(int16_t movie);
int32_t SaveMovieMaps_3E580_3E8C0(int16_t movie);
int32_t MovieMapsExist_3E640_3E980(int16_t movie);
int32_t LoadGame_3E690_3E9D0(int16_t number);
int32_t SaveGame_3E750_3EA90(int16_t number);
int32_t GameExists_3E7A0_3EAE0(int16_t number);
int32_t LoadMaps_3E7F0_3EB30(int16_t number);
int32_t SaveMaps_3E8C0_3EC00(int16_t number);
int32_t MapsExist_3E980_3ECC0(int16_t number);

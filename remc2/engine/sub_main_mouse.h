#ifndef ENGINE_SUB_MAIN_MOUSE
#define ENGINE_SUB_MAIN_MOUSE

#include <cstdint>

void MouseEvents(uint32_t buttons, int x, int y);
void MouseEvents(uint32_t buttons, int x, int y, uint16_t width, uint16_t height);

#endif //ENGINE_SUB_MAIN_MOUSE
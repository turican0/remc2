#pragma once

struct KeyboardInputMapping
{
	int Forward;
	int Backwards;
	int Left;
	int Right;
	int Map;
	int SpellMenu;
	int SpellMenuMark;
};

typedef struct KeyboardInputMapping KeyboardInputMapping_t;
extern KeyboardInputMapping_t inputMapping;
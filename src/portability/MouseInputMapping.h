#pragma once

struct MouseInputMapping
{
	int SpellLeft;
	int SpellRight;
	int map;
	int SpellMenu;
	int SpellMenuMark;
};

typedef struct MouseInputMapping MouseInputMapping_t;
extern MouseInputMapping_t mouseMapping;

#pragma once
#ifndef TYPE_BIG_SPRITES_BUFFER_H
#define TYPE_BIG_SPRITES_BUFFER_H

const int max_sprites_frames = 30;

typedef struct type_BIG_SPRITES_BUFFER {
	int count = -1;
	type_particle_str* actdatax;
	uint8_t* frames[max_sprites_frames];
} type_BIG_SPRITES_BUFFER;

#endif //TYPE_BIG_SPRITES_BUFFER_H
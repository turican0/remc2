#include "port_sdl_vga_mouse.h"
#include "port_time.h"

#include "../engine/sub_main_mouse.h"


#ifdef USE_DOSBOX
extern DOS_Device *DOS_CON;
#endif //USE_DOSBOX

#ifdef USE_SDL2
SDL_Window* gWindow = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;
SDL_Surface *helper_surface = NULL;
#endif

SDL_Surface *screen;

int width = 640;
int height = 480;
int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;
const char* default_caption = "Remake Magic Carpet 2";


bool inited = false;
Uint8 temppallettebuffer[768];/* = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x2A, 0x00, 0x2A, 0x00, 0x00, 0x2A, 0x2A, 0x2A, 0x00, 0x00, 0x2A,
0x00, 0x2A, 0x2A, 0x2A, 0x00, 0x2A, 0x2A, 0x2A, 0x00, 0x00, 0x15, 0x00, 0x00, 0x3F, 0x00, 0x2A,
0x15, 0x00, 0x2A, 0x3F, 0x2A, 0x00, 0x15, 0x2A, 0x00, 0x3F, 0x2A, 0x2A, 0x15, 0x2A, 0x2A, 0x3F,
0x00, 0x15, 0x00, 0x00, 0x15, 0x2A, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x2A, 0x2A, 0x15, 0x00, 0x2A,
0x15, 0x2A, 0x2A, 0x3F, 0x00, 0x2A, 0x3F, 0x2A, 0x00, 0x15, 0x15, 0x00, 0x15, 0x3F, 0x00, 0x3F,
0x15, 0x00, 0x3F, 0x3F, 0x2A, 0x15, 0x15, 0x2A, 0x15, 0x3F, 0x2A, 0x3F, 0x15, 0x2A, 0x3F, 0x3F,
0x15, 0x00, 0x00, 0x15, 0x00, 0x2A, 0x15, 0x2A, 0x00, 0x15, 0x2A, 0x2A, 0x3F, 0x00, 0x00, 0x3F,
0x00, 0x2A, 0x3F, 0x2A, 0x00, 0x3F, 0x2A, 0x2A, 0x15, 0x00, 0x15, 0x15, 0x00, 0x3F, 0x15, 0x2A,
0x15, 0x15, 0x2A, 0x3F, 0x3F, 0x00, 0x15, 0x3F, 0x00, 0x3F, 0x3F, 0x2A, 0x15, 0x3F, 0x2A, 0x3F,
0x15, 0x15, 0x00, 0x15, 0x15, 0x2A, 0x15, 0x3F, 0x00, 0x15, 0x3F, 0x2A, 0x3F, 0x15, 0x00, 0x3F,
0x15, 0x2A, 0x3F, 0x3F, 0x00, 0x3F, 0x3F, 0x2A, 0x15, 0x15, 0x15, 0x15, 0x15, 0x3F, 0x15, 0x3F,
0x15, 0x15, 0x3F, 0x3F, 0x3F, 0x15, 0x15, 0x3F, 0x15, 0x3F, 0x3F, 0x3F, 0x15, 0x3F, 0x3F, 0x3F,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };*/


Uint8* VGA_Get_pallette() {
	return temppallettebuffer;
}

void SubBlit() {
#ifdef USE_SDL2
	SDL_BlitSurface(screen, NULL, helper_surface, NULL);

	SDL_UpdateTexture(texture, NULL/*&rect*/, helper_surface->pixels, helper_surface->pitch);

	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);

	//SDL_UpdateWindowSurface(gWindow);
#else
	SDL_UpdateRect(screen, 0, 0, 0, 0);
#endif
}

void SubSet_pallette(SDL_Color* colors) {
#ifdef USE_SDL2
	SDL_SetPaletteColors(screen->format->palette, colors, 0, 256);
	SubBlit();
#else
	SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, colors, 0, 256);
#endif
}

void Set_basic_pallette0() {
	SDL_Color colors[256];
	for (int i = 0;i < 256;i++) {
		temppallettebuffer[i * 3] = i / 4;
		temppallettebuffer[i * 3 + 1] = i / 4;
		temppallettebuffer[i * 3 + 2] = i / 4;
		colors[i].r = temppallettebuffer[i * 3];
		colors[i].g = temppallettebuffer[i * 3 + 1];
		colors[i].b = temppallettebuffer[i * 3 + 2];
	}
	SubSet_pallette(colors);
}
void Set_basic_pallette1() {
	SDL_Color colors[256];
	for (int i = 0;i < 256;i++) {
		if (i != 0)
		{
			colors[i].r = 0;
			colors[i].g = 0;
			colors[i].b = 0;
		}
		else
		{
			colors[i].r = 255;
			colors[i].g = 255;
			colors[i].b = 255;
		}
	}
	SubSet_pallette(colors);
}

void Set_basic_pallette3() {
	SDL_Color colors[256];
	for (int i = 0; i < 256; i++) {
		temppallettebuffer[i * 3] = i;
		temppallettebuffer[i * 3 + 1] = ((int)(i/16))*16;
		temppallettebuffer[i * 3 + 2] = (i%16)*16;
		colors[i].r = temppallettebuffer[i * 3];
		colors[i].g = temppallettebuffer[i * 3 + 1];
		colors[i].b = temppallettebuffer[i * 3 + 2];
	}
	SubSet_pallette(colors);
}

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		*p = pixel;
		break;

	case 2:
		*(Uint16 *)p = pixel;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		}
		else {
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;

	case 4:
		*(Uint32 *)p = pixel;
		break;
	}
}

void Draw_debug_matrix0() {
	SDL_Rect dstrect;
	if (SDL_MUSTLOCK(screen)) {
		if (SDL_LockSurface(screen) < 0) {
			fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
			return;
		}
	}

	for (int i = 0;i < 16;i++)
		for (int j = 0;j < 16;j++)
		{
			dstrect.x = i * screen->w / 16;
			dstrect.y = j * screen->h / 16;
			dstrect.w = screen->w / 16;
			dstrect.h = screen->h / 16;
			SDL_FillRect(screen, &dstrect, i * 16 + j/*SDL_MapRGB(screen->format, i*16+j, 0, 0)*/);
		}

	if (SDL_MUSTLOCK(screen)) {
		SDL_UnlockSurface(screen);
	}
	SubBlit();
};

void Draw_debug_matrix1() {
	SDL_Rect dstrect;
	if (SDL_MUSTLOCK(screen)) {
		if (SDL_LockSurface(screen) < 0) {
			fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
			return;
		}
	}

	for (int i = 0;i < 16;i++)
		for (int j = 0;j < 16;j++)
		{
			dstrect.x = i * screen->w / 16;
			dstrect.y = j * screen->h / 16;
			dstrect.w = screen->w / 16;
			dstrect.h = screen->h / 16;
			SDL_FillRect(screen, &dstrect, 1);
		}

	if (SDL_MUSTLOCK(screen)) {
		SDL_UnlockSurface(screen);
	}
	SubBlit();
};

SDL_Surface* surface_font = NULL;
bool VGA_LoadFont()
{
	//Loading success flag
	bool success = true;

	//Load splash image
	//surface_font = SDL_LoadBMP("c:/prenos/remc2/font/xterm714.bmp");
	surface_font = SDL_LoadBMP("../font/xterm714.bmp");
	if (surface_font == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", "xterm714.bmp", SDL_GetError());
		success = false;
	}

	return success;
}

void Draw_letter(int letter_number, int pozx, int pozy) {
	SDL_Rect srcrect;
	SDL_Rect dstrect;

	/*for (int i = 0;i < 100;i++)
		for (int j = 0;j < 100;j++)
			putpixel(screen, i, j, 127);*/
			//217 x 224//13x14
			//186 x 208//11x13
			/*int letter_numaber = 34;
			int pozx = 15;
			int pozy = 15;*/
			/*srcrect.x = 12*(letter_number%16);
			srcrect.y = 13 *(int) (letter_number / 16);
			srcrect.w = 12;
			srcrect.h = 13;*///for 613
	srcrect.x = 14 * (letter_number % 16);
	srcrect.y = 14 * (int)(letter_number / 16);
	srcrect.w = 14;
	srcrect.h = 14;
	dstrect.x = (screen->w / 40)*pozx;
	dstrect.y = (screen->h / 25)*pozy;
	dstrect.w = screen->w / 40;
	dstrect.h = screen->h / 25;
	//SDL_RenderCopy(screen, surface_font, &srcrect, &dstrect);
	SDL_BlitSurface(surface_font, &srcrect, screen, &dstrect);
};

int lastpoz = 0;
int textwidth = 80;

void VGA_GotoXY(int x, int y) {
	lastpoz = y * textwidth + x;
};
int VGA_WhereX() {
	return(lastpoz%textwidth);
};
int VGA_WhereY() {
	return((int)(lastpoz/textwidth));
};

POSITION VGA_WhereXY() {
	POSITION result;
	result.x=(lastpoz%textwidth);
	result.y=((int)(lastpoz / textwidth));
	return result;
};

void VGA_Draw_string(char* wrstring) {
	SDL_Rect srcrect;
	SDL_Rect dstrect;
	if (SDL_MUSTLOCK(screen)) {
		if (SDL_LockSurface(screen) < 0) {
			fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
			return;
		}
	}

	for (int i = 0;i < strlen(wrstring);i++)
	{
		if (wrstring[i] == '\n')
		{
			lastpoz += textwidth - lastpoz % textwidth;
		}
		else
		{
			Draw_letter(wrstring[i], lastpoz % textwidth, (int)(lastpoz / textwidth));
			lastpoz++;
		}
	}

	if (SDL_MUSTLOCK(screen)) {
		SDL_UnlockSurface(screen);
	}
	SubBlit();
	mydelay(10);
}


void VGA_Init() {
#ifdef USE_SDL2
#define SDL_HWPALETTE 0
#endif
	VGA_Init(SCREEN_WIDTH, SCREEN_HEIGHT, 8, SDL_HWPALETTE | SDL_INIT_AUDIO);
}

SDL_Rect dst;
//SDL_Surface *curs;
/*
void VGA_Set_Cursor(posistruct posstrcursor,int number) {
	int sizex = 64;
	int sizey = 64;

	if (SDL_MUSTLOCK(curs)) {
		if (SDL_LockSurface(curs) < 0) {
			fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
			return;
		}
	}

	SDL_FillRect(curs, NULL, 0x000000);

	Bit32u inindex = 0;
	Bit32u outindex = 0;
	Bit8s shift = 0;
	Bit8s end = 0;
	Bit8s count = 0;

	Bit8s unknownvar = posstrcursor.pointer[inindex++];
	for(int i=0;i<number;i++)
		while (posstrcursor.pointer[inindex++] != 0x7f);

	end = posstrcursor.pointer[inindex++];
	if (end == 0)
	{
		count = posstrcursor.pointer[inindex++];
		outindex++;
		while (count == 0) {
			outindex += curs->w + 1;
			count = posstrcursor.pointer[inindex++];
		}
		if (count < 0) {
			shift = -count;
			count = posstrcursor.pointer[inindex++];
		}
		outindex += curs->w;
	}
	else {
		shift += count;
		count = end;
		if (count < 0) {
			shift -= count;
			count = posstrcursor.pointer[inindex++];
		}
	}
	memcpy(&((Uint8 *)curs->pixels)[outindex], &posstrcursor.pointer[inindex], count);
	for (Bit32u y = 1;count != 0x7f;y++)
	{
		memcpy(&((Uint8 *)curs->pixels)[outindex + shift], &posstrcursor.pointer[inindex], count);
		inindex += count;
		end = posstrcursor.pointer[inindex++];
		if (end == 0)
		{
			count = posstrcursor.pointer[inindex++];
			while (count == 0) {
				outindex += curs->w + 1;
				count = posstrcursor.pointer[inindex++];
			}
			if (count < 0) {
				shift = -count;
				count = posstrcursor.pointer[inindex++];
			}
			outindex += curs->w;
		}
		else {
			shift += count;
			count = end;
			if (count < 0) {
				shift -= count;
				count = posstrcursor.pointer[inindex++];
			}
		}
	}

	//free(v21_buffer_temp_index1);


	//v21_buffer_temp_index1 = (x_DWORD_18062C_resolution_x * tiley + tilex + pixel_buffer_index);

	//v21 = (char *)(dword_18062C * a2 + a3 + v6);

	if (SDL_MUSTLOCK(curs)) {
		SDL_UnlockSurface(curs);
	}

	//Uint32 colorkey = SDL_MapRGB(curs->format, 0xff, 0xFF, 0xFF);
	//SDL_SetColorKey(curs, SDL_SRCCOLORKEY, colorkey);
	//(curs->->pixels)[0] = 0;
};

Bit8u cursor1[10] = {
0x10,0x10,0x10,0,0,0,0,0,0,0
};

void VGA_Set_Cursor2() {
	int sizex = 64;
	int sizey = 64;

	if (SDL_MUSTLOCK(curs)) {
		if (SDL_LockSurface(curs) < 0) {
			fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
			return;
		}
	}

	for (int i = 0;i < 10;i++)
		((Uint8 *)curs->pixels)[i] = cursor1[i];

	if (SDL_MUSTLOCK(curs)) {
		SDL_UnlockSurface(curs);
	}


};*/

#ifdef USE_SDL2


// Initalize Color Masks. 
#if SDL_BYTEORDER == SDL_BIG_ENDIAN 
Uint32 redMask = 0xff000000;
Uint32 greenMask = 0x00ff0000;
Uint32 blueMask = 0x0000ff00;
Uint32 alphaMask = 0x000000ff;
#else 
Uint32 redMask = 0x000000ff;
Uint32 greenMask = 0x0000ff00;
Uint32 blueMask = 0x00ff0000;
Uint32 alphaMask = 0xff000000;
#endif 
void VGA_Init(int width, int height, int bpp, Uint32 flags)
{
	if (!inited)
	{
		//Initialization flag
		//bool success = true;

		//Initialize SDL
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
			exit(0);
			//success = false;
		}
		else
		{
			init_sound();
			//SDL_EnableUNICODE(SDL_DISABLE);
			SDL_ShowCursor(0);
			// Set hint before you create the Renderer!
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
			SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, "1");
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

			gWindow =
				SDL_CreateWindow(default_caption, SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);

			renderer =
				SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED |
					SDL_RENDERER_TARGETTEXTURE);


			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);


			// Now create your surface and convert the pixel format right away!
			// Converting the pixel format to match the texture makes for quicker updates, otherwise
			// It has to do the conversion each time you update the texture manually. This slows everything down.
			// Do it once, and don't have to worry about it again.

			helper_surface =
				SDL_CreateRGBSurface(
					SDL_SWSURFACE, width, height, 32,
					redMask, greenMask, blueMask, alphaMask);

			helper_surface =
				SDL_ConvertSurfaceFormat(
					helper_surface, SDL_PIXELFORMAT_ARGB8888, 0);

			screen =
				SDL_CreateRGBSurface(
					SDL_SWSURFACE, width, height, 32,
					redMask, greenMask, blueMask, alphaMask);

			screen =
				SDL_ConvertSurfaceFormat(
					screen, SDL_PIXELFORMAT_INDEX8, 0);


			texture = SDL_CreateTexture(renderer,
				SDL_PIXELFORMAT_ARGB8888,
				SDL_TEXTUREACCESS_STREAMING,
				helper_surface->w, helper_surface->h);

			SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

			// Sure clear the screen first.. always nice.
			SDL_RenderClear(renderer);
			SDL_RenderPresent(renderer);
			/*//Create window
			//gWindow = SDL_CreateWindow(default_caption, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
			//
			gWindow = SDL_CreateWindow(default_caption,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, width, height,0);

			if (gWindow == NULL)
			{
				printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
				exit(0);
				//success = false;
			}
			else
			{
				//Get window surface
				helper_surface = SDL_GetWindowSurface(gWindow);
				//
				//helper_surface =SDL_CreateRGBSurface(SDL_SWSURFACE, 320, 200, 32,redMask, greenMask, blueMask, alphaMask);

				helper_surface =SDL_ConvertSurfaceFormat(helper_surface, SDL_PIXELFORMAT_ARGB8888, 0);

				screen =SDL_CreateRGBSurface(SDL_SWSURFACE, 320, 200, 32,	redMask, greenMask, blueMask, alphaMask);

				screen =SDL_ConvertSurfaceFormat(screen, SDL_PIXELFORMAT_ARGB8888, 0);
			}*/
		}
		if (!VGA_LoadFont())
		{
			printf("Failed to load font!\n");
			exit(-1);
		}

		Set_basic_pallette1();
		Draw_debug_matrix1();	
		inited = true;
		//mydelay(3000);
	}
	//return success;
}
#else
void VGA_Init(int width, int height, int bpp, Uint32 flags) {
	if (!inited)
	{
		SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
		init_sound();
		SDL_EnableUNICODE(SDL_DISABLE);

		SDL_ShowCursor(0);

		atexit(SDL_Quit);

		screen = SDL_SetVideoMode(width, height, bpp, flags);

		if (!screen) {
			printf("Couldn't set video mode: %s\n", SDL_GetError());
			exit(-1);
		}

		//Set the window caption

		SDL_WM_SetCaption(default_caption, NULL);

		if (!VGA_LoadFont())
		{
			printf("Failed to load font!\n");
			exit(-1);
		}

		//debug
		Set_basic_pallette1();
		Draw_debug_matrix1();
		//Draw_debug_matrix();	
		//Draw_letter(0x42,15,15);
		//VGA_Draw_string((char*)"test textx");
		//degug
		inited = true;
		//mydelay(3000);
	}
}
#endif


int origw = 640;
int origh = 480;

void VGA_Resize(int width, int height) {
	origw = width;
	origh = height;
	//VGA_Resize(width, height, 8, SDL_HWPALETTE);
}
void VGA_Resize(int width, int height, int bpp, Uint32 flags) {
	/*SDL_FreeSurface(screen);

	#ifdef USE_SDL2
		//screen = SDL_SetVideoMode(width, height, bpp, flags);
	#else
	screen = SDL_SetVideoMode(width, height, bpp, flags);
	#endif
	if (!screen) {
		printf("Couldn't set video mode: %s\n", SDL_GetError());
		exit(-1);
	}
	//debug
	//Draw_debug_matrix();
	//VGA_Draw_string((char*)"test textx");
	//degug
	*/
};


void VGA_Set_pallette(Uint8* pallettebuffer) {
	memcpy(temppallettebuffer, pallettebuffer, 768);
	SDL_Color colors[256];
	/* Fill colors with color information */
	for (int i = 0;i < 256;i++) {
		colors[i].r = /*i;*/4 * pallettebuffer[i * 3];
		colors[i].g = /*i;*/4 * pallettebuffer[i * 3 + 1];
		colors[i].b = /*i;*/4 * pallettebuffer[i * 3 + 2];
		//if((colors[i].r==0x8)/*&&(colors[i].g == 0x0)&&(colors[i].b == 0x00)*/)
			//printf("%01X %02X,%02X,%02X\n", i,colors[i].r, colors[i].g, colors[i].b);
	}

	SubSet_pallette(colors);
}

void VGA_Write_basic_pallette(Uint8* pallettebuffer) {
	memcpy(temppallettebuffer, pallettebuffer, 768);
}



void VGA_test() {

	int x = screen->w / 2;
	int y = screen->h / 2;


	/* Lock the screen for direct access to the pixels */
	if (SDL_MUSTLOCK(screen)) {
		if (SDL_LockSurface(screen) < 0) {
			fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
			return;
		}
	}

	for (int i = 0;i < 600;i++)
		for (int j = 0;j < 400;j++)
			putpixel(screen, i, j, 127);

	if (SDL_MUSTLOCK(screen)) {
		SDL_UnlockSurface(screen);
	}
	/* Update just the part of the display that we've changed */

	SubBlit();
}

/*

Keyboard scan code/character code combinations returned in AH/AL by
INT 16h functions 00h/01h (standard) and functions 01h/11h (extended)

Key                        Normal           Shift           Ctrl             Alt
						 Std     Ext     Std     Ext     Std     Ext     Std     Ext
Esc                     01/1B   01/1B   01/1B   01/1B   01/1B   01/1B     -     01/00
1 !                     02/31   02/31   02/21   02/21     -       -     78/00   78/00
2 @                     03/32   03/32   03/40   03/40   03/00   03/00   79/00   79/00
3 #                     04/33   04/33   04/23   04/23     -       -     7A/00   7A/00
4 $                     05/34   05/34   05/24   05/24     -       -     7B/00   7B/00
5 %                     06/35   06/35   06/25   06/25     -       -     7C/00   7C/00
6 ^                     07/36   07/36   07/5E   07/5E   07/1E   07/1E   7D/00   7D/00
7 &                     08/37   08/37   08/26   08/26     -       -     7E/00   7E/00
8 *                     09/38   09/38   09/2A   09/2A     -       -     7F/00   7F/00
9 (                     0A/39   0A/39   0A/28   0A/28     -       -     80/00   80/00
0 )                     0B/30   0B/30   0B/29   0B/29     -       -     81/00   81/00
- _                     0C/2D   0C/2D   0C/5F   0C/5F   0C/1F   0C/1F   82/00   82/00
= +                     0D/3D   0D/3D   0D/2B   0D/2B     -       -     83/00   83/00
Backspace               0E/08   0E/08   0E/08   0E/08   0E/7F   0E/7F     -     0E/00
Tab                     0F/09   0F/09   0F/00   0F/00     -     94/00     -     A5/00
q Q                     10/71   10/71   10/51   10/51   10/11   10/11   10/00   10/00
w W                     11/77   11/77   11/57   11/57   11/17   11/17   11/00   11/00
e E                     12/65   12/65   12/45   12/45   12/05   12/05   12/00   12/00
r R                     13/72   13/72   13/52   13/52   13/12   13/12   13/00   13/00
t T                     14/74   14/74   14/54   14/54   14/14   14/14   14/00   14/00
y Y                     15/79   15/79   15/59   15/59   15/19   15/19   15/00   15/00
u U                     16/75   16/75   16/55   16/55   16/15   16/15   16/00   16/00
i I                     17/69   17/69   17/49   17/49   17/09   17/09   17/00   17/00
o O                     18/6F   18/6F   18/4F   18/4F   18/0F   18/0F   18/00   18/00
p P                     19/70   19/70   19/50   19/50   19/10   19/10   19/00   19/00
[ {                     1A/5B   1A/5B   1A/7B   1A/7B   1A/1B   1A/1B     -     1A/00
] }                     1B/5D   1B/5D   1B/7D   1B/7D   1B/1D   1B/1D     -     1B/00
Enter                   lC/0D   1C/0D   1C/0D   1C/0D   1C/0A   1C/0A     -     1C/00
Ctrl                      *       *       *       *       *       *       *       *
a A                     1E/61   1E/61   1E/41   1E/41   1E/01   1E/01   1E/00   1E/00
s S                     1F/73   1F/73   1F/53   1F/53   1F/13   1F/13   1F/00   1F/00
d D                     20/64   20/64   20/44   20/44   20/04   20/04   20/00   20/00
f F                     21/66   21/66   21/46   21/46   21/06   21/06   21/00   21/00
g G                     22/67   22/67   22/47   22/47   22/07   22/07   22/00   22/00
h H                     23/68   23/68   23/48   23/48   23/08   23/08   23/00   23/00
j J                     24/6A   24/6A   24/4A   24/4A   24/0A   24/0A   24/00   24/00
k K                     25/6B   25/6B   25/4B   25/4B   25/0B   25/0B   25/00   25/00
l L                     26/6C   26/6C   26/4C   26/4C   26/0C   26/0C   26/00   26/00
; :                     27/3B   27/3B   27/3A   27/3A     -       -       -     27/00
' "                     28/27   28/27   28/22   28/22     -       -       -     28/00
` ~                     29/60   29/60   29/7E   29/7E     -       -       -     29/00
Left Shift                *       *       *       *       *       *       *       *
\ |                     2B/5C   2B/5C   2B/7C   2B/7C   2B/1C   2B/1C     -     2B/00
z Z                     2C/7A   2C/7A   2C/5A   2C/5A   2C/1A   2C/1A   2C/00   2C/00
x X                     2D/78   2D/78   2D/58   2D/58   2D/18   2D/18   2D/00   2D/00
c C                     2E/63   2E/63   2E/43   2E/43   2E/03   2E/03   2E/00   2E/00
v V                     2F/76   2F/76   2F/56   2F/56   2F/16   2F/16   2F/00   2F/00
b B                     30/62   30/62   30/42   30/42   30/02   30/02   30/00   30/00
n N                     31/6E   31/6E   31/4E   31/4E   31/0E   31/0E   31/00   31/00
m M                     32/6D   32/6D   32/4D   32/4D   32/0D   32/0D   32/00   32/00
, <                     33/2C   33/2C   33/3C   33/3C     -       -       -     33/00
. >                     34/2E   34/2E   34/3E   34/3E     -       -       -     34/00
/ ?                     35/2F   35/2F   35/3F   35/3F     -       -       -     35/00
Right Shift               *       *       *       *       *       *       *       *
*                       37/2A   37/2A   37/2A   37/2A     -     96/00     -     37/00
Alt                       *       *       *       *       *       *       *       *
Space                   39/20   39/20   39/20   39/20   39/20   39/20   39/20   39/20
Caps Lock                 *       *       *       *       *       *       *       *
F1                      3B/00   3B/00   54/00   54/00   5E/00   5E/00   68/00   68/00
F2                      3C/00   3C/00   55/00   55/00   5F/00   5F/00   69/00   69/00
F3                      3D/00   3D/00   56/00   56/00   60/00   60/00   6A/00   6A/00
F4                      3E/00   3E/00   57/00   57/00   61/00   61/00   6B/00   6B/00
F5                      3F/00   3F/00   58/00   58/00   62/00   62/00   6C/00   6C/00
F6                      40/00   40/00   59/00   59/00   63/00   63/00   6D/00   6D/00
F7                      41/00   41/00   5A/00   5A/00   64/00   64/00   6E/00   6E/00
F8                      42/00   42/00   5B/00   5B/00   65/00   65/00   6F/00   6F/00
F9                      43/00   43/00   5C/00   5C/00   66/00   66/00   70/00   70/00
F10                     44/00   44/00   5D/00   5D/00   67/00   67/00   71/00   71/00
Num Lock                  *       *       *       *       *       *       *       *
Scroll Lock               *       *       *       *       *       *       *       *
Home 7                  47/00   47/00   47/37   47/37   77/00   77/00   00/#    00/#
Up Arrow 8              48/00   48/00   48/38   48/38     -     8D/00   00/#    00/#
PgUp 9                  49/00   49/00   49/39   49/39   84/00   84/00   00/#    00/#
-                       4A/2D   4A/2D   4A/2D   4A/2D     -     8E/00     -     4A/00
Left Arrow 4            4B/00   4B/00   4B/34   4B/34   73/00   73/00   00/#    00/#
Center Key 5              -     4C/00   4C/35   4C/35     -     8F/00   00/#    00/#
Right Arrow 6           4D/00   4D/00   4D/36   4D/36   74/00   74/00   00/#    00/#
+                       4E/2B   4E/2B   4E/2B   4E/2B     -     90/00     -     4E/00
End 1                   4F/00   4F/00   4F/31   4F/31   75/00   75/00   00/#    00/#
Down Arrow 2            50/00   50/00   50/32   50/32     -     91/00   00/#    00/#
PgDn 3                  51/00   51/00   51/33   51/33   76/00   76/00   00/#    00/#
Ins 0                   52/00   52/00   52/30   52/30     -     92/00   00/#    00/#
Del .                   53/00   53/00   53/2E   53/2E     -     93/00     -       -
SysReq (84-key only)      *       *       *       *       *       *       *       *
PrtSc/SysReq (101-key)    *       *       *       *     72/00   72/00     *       *
Pause/Break (101-key)     *       *       *       *     00/00   00/00     *       *
F11 (101-key)             -     85/00     -     87/00     -     89/00     -     8B/00
F12 (101-key)             -     86/00     -     88/00     -     8A/00     -     8C/00
Home (101-key)          47/00   47/E0   47/00   47/E0   77/00   77/E0     -     97/00
Up Arrow (101-key)      48/00   48/E0   48/00   48/E0     -     8D/E0     -     98/00
Page Up (101-key)       49/00   49/E0   49/00   49/E0   84/00   84/E0     -     99/00
Left Arrow (101-key)    4B/00   4B/E0   4B/00   4B/E0   73/00   73/E0     -     9B/00
Right Arrow (101-key)   4D/00   4D/E0   4D/00   4D/E0   74/00   74/E0     -     9D/00
End (101-key)           4F/00   4F/E0   4F/00   4F/E0   75/00   75/E0     -     9F/00
Down Arrow (101-key)    50/00   50/E0   50/00   50/E0     -     91/E0     -     A0/00
Page Down (101-key)     51/00   51/E0   51/00   51/E0   76/00   76/E0     -     A1/00
Insert (101-key)        52/00   52/E0   52/00   52/E0     -     92/E0     -     A2/00
Delete (101-key)        53/00   53/E0   53/00   53/E0     -     93/E0     -     A3/00
/ (101-key)             35/2F   E0/2F   35/2F   E0/2F     -     95/00     -     A4/00
Enter (101-key)         1C/0D   E0/0D   1C/0D   E0/0D   1C/0A   E0/0A     -     A6/00
Right Alt (101-key)       *       *       *       *       *       *       *       *
Right Ctrl (101-key)      *       *       *       *       *       *       *       *
\ | (102-key)           56/5C   56/5C   56/7C   56/7C     -       -       -       -

- Keystroke ignored
* Processed by the BIOS
# Alt-keypad number returns the specified character code and scan code 0
*/

int mousex, mousey;
bool pressed = false;
Bit16u lastchar = 0;
int events()
{
	SDL_Event event;
	Uint8 buttonindex;
	Uint8 buttonstate;
	Bit32u buttonresult;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			pressed = true;
			lastchar = (event.key.keysym.scancode << 8) + event.key.keysym.sym;
			printf("Key press detected\n");//test
			break;

		case SDL_KEYUP:
			//pressed = false;
			//lastchar = 0;
			printf("Key release detected\n");//test
			break;

		case SDL_MOUSEMOTION:
			mousex = event.motion.x;
			mousey = event.motion.y;
			mouse_events(1, event.motion.x, event.motion.y);
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			mousex = event.motion.x;
			mousey = event.motion.y;

			buttonresult = 0;

			buttonindex = event.button.button;
			buttonstate = event.button.state;
			switch (buttonindex) {
			case SDL_BUTTON_LEFT:
			{
				switch (buttonstate) {
				case SDL_PRESSED:
				{
					buttonresult |= 2;
					break;
				}
				case SDL_RELEASED:
				{
					buttonresult |= 4;
					break;}
				}
				break;
			}
			case SDL_BUTTON_MIDDLE:
			{
				switch (buttonstate) {
				case SDL_PRESSED:
				{
					buttonresult |= 8;
					break;
				}
				case SDL_RELEASED:
				{
					buttonresult |= 0x10;
					break;}
				}
				break;
			}
			case SDL_BUTTON_RIGHT:
			{
				switch (buttonstate) {
				case SDL_PRESSED:
				{
					buttonresult |= 0x20;
					break;
				}
				case SDL_RELEASED:
				{
					buttonresult |= 0x40;
					break;}
				}
				break;
			}
			}
			//x, y	The X / Y coordinates of the mouse at press / release time

			mouse_events(buttonresult, event.motion.x, event.motion.y);
			break;

		case SDL_QUIT: return 0;
		}
	}
	return 1;
}

int VGA_Set_mouse(Bit16s a1, Bit16s a2) {
	//mouse set cursor
	return 0;
};

//SDL_Window *window = NULL;
//SDL_Surface *output_surface = NULL;


void VGA_Blit(int width, int height, Uint8* buffer) {
	events();
	if (SDL_MUSTLOCK(screen)) {
		if (SDL_LockSurface(screen) < 0) {
			fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
			return;
		}
	}
	if ((origw == screen->w) && (origh == screen->h))//same resolution
		memcpy(screen->pixels, buffer, width*height);
	else if ((origw * 2 == screen->w) && (origh * 2 == screen->h))//2x resolution
	{
		int k = 0;
		int l = 0;
		for (int i = 0;i < screen->w;i++)
		{
			for (int j = 0;j < screen->h;j++)
			{
				((Bit8u*)screen->pixels)[i + j * screen->w] = buffer[k + l * origw];
				l += j % 2;
			}
			k += i % 2;
			l = 0;
		}
	}
	else//any resolution
	{
		int k = 0;
		int l = 0;
		float xscale = (float)origw / (float)screen->w;
		float yscale = (float)origh / (float)screen->h;
		for (int i = 0; i < screen->w; i++)
		{
			for (int j = 0; j < screen->h; j++)//49+1 - final size
			{
				k = (int)(i * xscale);
				l = (int)(j * yscale);
				((Bit8u*)screen->pixels)[i + j * screen->w] = buffer[k + l * origw];
			}
		}
	}
	if (SDL_MUSTLOCK(screen)) {
		SDL_UnlockSurface(screen);
	}
	SubBlit();

	//Set_basic_pallette();
}

void VGA_Debug_Blit(int width, int height, Uint8* buffer) {
	//Set_basic_pallette3();
	VGA_Blit(width, height, buffer);
}



void VGA_Init_test() {
	/* Create a display surface with a grayscale palette */
	SDL_Color colors[256];
	//neco
			/* Fill colors with color information */
	for (int i = 0;i < 256;i++) {
		colors[i].r = i;
		colors[i].g = i;
		colors[i].b = i;
	}

	/* Create display */

#ifdef USE_SDL2
	//screen = SDL_SetVideoMode(640, 480, 8, SDL_HWPALETTE);
#else
	screen = SDL_SetVideoMode(640, 480, 8, SDL_HWPALETTE);
#endif
	if (!screen) {
		printf("Couldn't set video mode: %s\n", SDL_GetError());
		exit(-1);
	}

	/* Set palette */

#ifdef USE_SDL2
	SDL_SetPaletteColors(screen->format->palette, colors, 0, 256);
#else
	SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, colors, 0, 256);
#endif

	int x = screen->w / 2;
	int y = screen->h / 2;

	/* Lock the screen for direct access to the pixels */
	if (SDL_MUSTLOCK(screen)) {
		if (SDL_LockSurface(screen) < 0) {
			fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
			return;
		}
	}

	for (int i = 0;i < 600;i++)
		for (int j = 0;j < 400;j++)
			putpixel(screen, i, j, 127);

	if (SDL_MUSTLOCK(screen)) {
		SDL_UnlockSurface(screen);
	}
	/* Update just the part of the display that we've changed */

#ifdef USE_SDL2
	SDL_UpdateWindowSurface(gWindow);
#else
	SDL_UpdateRect(screen, 0, 0, 0, 0);
#endif
}
#ifdef USE_SDL2
void VGA_close()
{
	clean_up_sound();
	SDL_FreeSurface(surface_font);
	surface_font = NULL;
	SDL_FreeSurface(screen);
	screen = NULL;
	SDL_DestroyTexture(texture);
	texture = NULL;
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	SDL_Quit();
}
#else
void VGA_close()
{
	clean_up_sound();
	SDL_FreeSurface(surface_font);
	surface_font = NULL;
	SDL_FreeSurface(screen);
	screen = NULL;
	SDL_Quit();
}
#endif
/*
change resolution
#include <SDL.h>

#define WIDTH 800
#define HEIGHT 600
#define BPP 32
#define FLAGS (SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN)

int main(int argc, char *argv[]) {
SDL_Surface *screen;
SDL_Surface *temp;
SDL_Surface *image;

SDL_Init(SDL_INIT_TIMER|SDL_INIT_VIDEO);
atexit(SDL_Quit);
screen = SDL_SetVideoMode(WIDTH, HEIGHT, BPP, FLAGS);

// load something on screen
image = SDL_LoadBMP(�test.bmp�);
SDL_BlitSurface(image, NULL, screen, NULL);
SDL_Flip(screen);
SDL_Delay(5000);

// backup display
temp =
SDL_CreateRGBSurface(SDL_HWSURFACE, screen->w, screen->h, screen->format->BitsPerPixel, screen->format->Rmask, screen->format->Gmask, screen->format->Bmask, screen->format->Amask);
SDL_BlitSurface(screen, NULL, temp, NULL);

// change resolution
screen = SDL_SetVideoMode(WIDTH, HEIGHT, BPP, FLAGS);

// restore old screen
SDL_BlitSurface(temp, NULL, screen, NULL);
SDL_Flip(screen);
SDL_Delay(5000);

SDL_FreeSurface(temp);
SDL_FreeSurface(image);
SDL_FreeSurface(screen);
exit(0);
}

*/

/*
static INLINE void mem_writew_inline(const PhysPt address,const Bit16u val) {
	if ((address & 0xfffu)<0xfffu) {
		const HostPt tlb_addr=get_tlb_write(address);//test zda jde o normalni pamet
		if (tlb_addr) host_writew(tlb_addr+address,val);
		else (get_tlb_writehandler(address))->writew(address,val);
	} else mem_unalignedwritew(address,val);
}

//prvni zapsany byte do vga:
34e434
34e - page
/*

//draw line
static void VGA_DrawSingleLine(Bitu /-blah-/) {
static Bit8u * VGA_Draw_Xlat32_Linear_Line(Bitu vidstart, Bitu /-line-/) {
Bit32u* temps = (Bit32u*)TempLine;

for (Bitu i = 0; i < (vga.draw.line_length >> 2); i++)
	temps[i] = vga.dac.xlat32[vga.draw.linear_base[(vidstart + i)&vga.draw.linear_mask]];

return TempLine;
}


//scalerSourceCache - videocache

CAPTURE_AddImage( render.src.width, render.src.height, render.src.bpp, pitch,
			flags, fps, (Bit8u *)&scalerSourceCache, (Bit8u*)&render.pal.rgb );

			drwacursor je v ints/mouse.cpp - ale nepouziva se

			INLINE void Mouse_AddEvent(Bit8u type) // zpracovani udalosti mysi - aktivuje preruseni

*/
Bit16s VGA_get_shift_status() {
	return 0;
}
bool VGA_check_standart_input_status() {
	bool locpressed = pressed;
	Bit16u loclastchar = lastchar;
	pressed = false;
	return locpressed;
}
Bit16u VGA_read_char_from_buffer() {
	bool locpressed = pressed;
	Bit16u loclastchar = lastchar;
	lastchar = 0;
	switch (loclastchar & 0xff00) {
	case 0x4900://up
		loclastchar = 0x4800;
		break;
	case 0x4e00://right
		loclastchar = 0x4d00;
		break;

	case 0x5100://down
		loclastchar = 0x5000;
		break;
	case 0x4c00://left
		loclastchar = 0x4b00;
		break;

	case 0x1c00://enter
	case 0x1d00://enter
		loclastchar = 0x1c00;
		break;
	case 0x1e00://ctrl
		loclastchar = 0x1d00;
		break;

	case 0x3900://alt
		loclastchar = 0x3800;
		break;
	case 0x2b00://left shift
		loclastchar = 0x2a00;
		break;

	case 0x3700://left shift
		loclastchar = 0x3600;
		break;

	case 0x4000://.
		loclastchar = 0x3400;
		break;

	case 0x3500:// /
	case 0x3600:// /
		loclastchar = 0x352f;
		break;
	}
	return loclastchar;
}
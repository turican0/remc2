#include "port_sdl_vga_mouse.h"
#include "port_time.h"

#include "../engine/sub_main_mouse.h"


#ifdef USE_DOSBOX
extern DOS_Device *DOS_CON;
#endif //USE_DOSBOX

SDL_Surface *screen;

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




void Set_basic_pallette() {	
	SDL_Color colors[256];
	/* Fill colors with color information */
	for (int i = 0;i < 256;i++) {
		temppallettebuffer[i * 3]=i/4;
		temppallettebuffer[i * 3 + 1] = i/4;
		temppallettebuffer[i * 3 + 2] = i/4;
		colors[i].r = temppallettebuffer[i * 3];
		colors[i].g = temppallettebuffer[i * 3 + 1];
		colors[i].b = temppallettebuffer[i * 3 + 2];
	}

	/* Set palette */
	SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, colors, 0, 256);
	//memcpy(temppallettebuffer, pallettebuffer, 768);
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

void Draw_debug_matrix() {
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
			dstrect.x = i* screen->w/16;
			dstrect.y = j* screen->h/16;
			dstrect.w = screen->w / 16;
			dstrect.h = screen->h / 16;
			SDL_FillRect(screen, &dstrect, i * 16 + j/*SDL_MapRGB(screen->format, i*16+j, 0, 0)*/);
		}

	if (SDL_MUSTLOCK(screen)) {
		SDL_UnlockSurface(screen);
	}
	SDL_UpdateRect(screen, 0, 0, 0, 0);	
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

void Draw_letter(int letter_number,int pozx,int pozy) {
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
	return(lastpoz%textwidth);
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
			lastpoz += textwidth -lastpoz% textwidth;
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
	SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void VGA_Init() {
	VGA_Init(640, 480, 8, SDL_HWPALETTE/*|SDL_DOUBLEBUF*/);
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


void VGA_Init(int width,int height,int bpp,Uint32 flags) {
	if (!inited)
	{
		SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
		//curs = SDL_LoadBMP("../cursors/cursor.bmp");
		/*curs = SDL_CreateRGBSurface(SDL_HWPALETTE, 64, 64, 8, 0,0,0,0);
		VGA_Set_Cursor2();*/

		/*Uint32 colorkey = SDL_MapRGB(curs->format, 0x0, 0x0, 0x0);
		SDL_SetColorKey(curs, SDL_SRCCOLORKEY, colorkey);		

		dst.w = curs->w;
		dst.h = curs->h;*/

		SDL_EnableUNICODE(SDL_DISABLE);

		SDL_ShowCursor(0);

		atexit(SDL_Quit);
		screen = SDL_SetVideoMode(width, height, bpp, flags);
		if (!screen) {
			printf("Couldn't set video mode: %s\n", SDL_GetError());
			exit(-1);
		}

		//Set the window caption
		SDL_WM_SetCaption("Remake Magic Carpet 2", NULL);

		if (!VGA_LoadFont())
		{
			printf("Failed to load font!\n");
			exit(-1);
		}

		//debug
		Set_basic_pallette();
		//Draw_debug_matrix();	
		//Draw_letter(0x42,15,15);
		//VGA_Draw_string((char*)"test textx");
		//degug
		inited = true;
		//mydelay(3000);
	}
}

void VGA_Resize(int width, int height) {
	VGA_Resize(width, height, 8, SDL_HWPALETTE);
}
void VGA_Resize(int width, int height, int bpp, Uint32 flags) {
	SDL_FreeSurface(screen);
	screen = SDL_SetVideoMode(width, height, bpp, flags);
	if (!screen) {
		printf("Couldn't set video mode: %s\n", SDL_GetError());
		exit(-1);
	}
	//debug
	//Draw_debug_matrix();
	//VGA_Draw_string((char*)"test textx");
	//degug
};


Uint8* VGA_Get_pallette() {
	return temppallettebuffer;
}

void VGA_Set_pallette(Uint8* pallettebuffer) {
	memcpy(temppallettebuffer, pallettebuffer,768);
	SDL_Color colors[256];
	/* Fill colors with color information */
	for (int i = 0;i < 256;i++) {
		colors[i].r = /*i;*/4 * pallettebuffer[i * 3];
		colors[i].g = /*i;*/4 * pallettebuffer[i * 3 + 1];
		colors[i].b = /*i;*/4 * pallettebuffer[i * 3 + 2];
		//if((colors[i].r==0x8)/*&&(colors[i].g == 0x0)&&(colors[i].b == 0x00)*/)
			//printf("%01X %02X,%02X,%02X\n", i,colors[i].r, colors[i].g, colors[i].b);
	}

	/* Set palette */
	SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, colors, 0, 256);
	//SDL_SetPalette(curs, SDL_LOGPAL | SDL_PHYSPAL, colors, 0, 256);
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
	SDL_UpdateRect(screen, 0, 0, 0, 0);
}

int mousex, mousey;
bool pressed = false;
char lastchar=0;
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
			lastchar = (char)event.key.keysym.sym;
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

void VGA_Blit(int width, int height, Uint8* buffer) {
	events();
	//int x = screen->w / 2;
	//int y = screen->h / 2;
	//int bpp = screen->format->BytesPerPixel;
	//Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

	/* Lock the screen for direct access to the pixels */
	if (SDL_MUSTLOCK(screen)) {
		if (SDL_LockSurface(screen) < 0) {
			fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
			return;
		}
	}

	memcpy(screen->pixels, buffer, width*height);
	/*dst.x = mousex;
	dst.y = mousey;
	// Blit onto main surface
	SDL_BlitSurface(curs, NULL, screen, &dst);*/

	/*Uint8 unused[256];
	for (int k = 0;k < 255;k++)
		unused[k] == 0;
	for (int i = 0;i < 640;i++)
		for (int j = 0;j < 480;j++)
			for (int k = 0;k < 255;k++)
				if (((Uint8 *)screen->pixels)[480 * j + i] == k)
					unused[k] = 1;
	printf("\n,");
	for (int k = 0;k < 255;k++)
		if (unused[k] != 1)
			printf("%04X,",k);
*/
	if (SDL_MUSTLOCK(screen)) {
		SDL_UnlockSurface(screen);
	}
		/*
	//barvy e5 a vice jsou volne
	
	/* Update just the part of the display that we've changed */
	SDL_UpdateRect(screen, 0, 0, 0, 0);	

	//Set_basic_pallette();
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
	screen = SDL_SetVideoMode(640, 480, 8, SDL_HWPALETTE);
	if (!screen) {
		printf("Couldn't set video mode: %s\n", SDL_GetError());
		exit(-1);
	}

	/* Set palette */
	SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, colors, 0, 256);

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
	SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void VGA_close()
{
	//Deallocate surface
	SDL_FreeSurface(surface_font);
	surface_font = NULL;

	//Destroy window
	SDL_FreeSurface(screen);
	screen = NULL;

	//SDL_FreeSurface(curs);
	//curs=NULL;

	//Quit SDL subsystems
	SDL_Quit();
}
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
image = SDL_LoadBMP(“test.bmp”);
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
	char loclastchar = lastchar;
	pressed = false;
	return locpressed;
}
char VGA_read_char_from_buffer() {
	bool locpressed = pressed;
	char loclastchar = lastchar;
	lastchar = 0;
	return loclastchar;
}

#include "graphics_sdl.h"


#ifdef USE_DOSBOX
extern DOS_Device *DOS_CON;
#endif //USE_DOSBOX

SDL_Surface *screen;

bool inited = false;
Uint8 temppallettebuffer[768];

void Set_basic_pallette() {	
	SDL_Color colors[256];
	/* Fill colors with color information */
	for (int i = 0;i < 256;i++) {
		temppallettebuffer[i * 3]=i;
		temppallettebuffer[i * 3 + 1] = i;
		temppallettebuffer[i * 3 + 2] = i;
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
	surface_font = SDL_LoadBMP("../font/xterm613.bmp");
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
	srcrect.x = 12*(letter_number%16);
	srcrect.y = 13 *(int) (letter_number / 16);
	srcrect.w = 12;
	srcrect.h = 13;
	dstrect.x = (screen->w / 80)*pozx;
	dstrect.y = (screen->h / 25)*pozy;
	dstrect.w = screen->w / 80;
	dstrect.h = screen->h / 25;
	//SDL_RenderCopy(screen, surface_font, &srcrect, &dstrect);
	SDL_BlitSurface(surface_font, &srcrect, screen, &dstrect);

};

int lastpoz = 0;

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
			lastpoz += 80-lastpoz%80;
		}
		else
		{
			Draw_letter(wrstring[i], lastpoz % 80, (int)(lastpoz / 80));
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
void VGA_Init(int width,int height,int bpp,Uint32 flags) {
	if (!inited)
	{
		SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
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
		//Set_basic_pallette();
		Draw_debug_matrix();	
		//Draw_letter(0x42,15,15);
		//VGA_Draw_string((char*)"test textx");
		//degug
		inited = true;
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
	Draw_debug_matrix();
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
	}

	/* Set palette */
	SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, colors, 0, 256);
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

void VGA_Blit(int width, int height, Uint8* buffer) {
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
	/*for (int i = 0;i < 600;i++)
		for (int j = 0;j < 400;j++)
			putpixel(screen, i, j, 127);*/

	if (SDL_MUSTLOCK(screen)) {
		SDL_UnlockSurface(screen);
	}
	/* Update just the part of the display that we've changed */
	SDL_UpdateRect(screen, 0, 0, 0, 0);	
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

#include "graphics_sdl.h"


#ifdef USE_DOSBOX
extern DOS_Device *DOS_CON;
#endif //USE_DOSBOX

SDL_Surface *screen;

void VGA_Init() {
	VGA_Init(320, 200, 8, SDL_HWPALETTE/*|SDL_DOUBLEBUF*/);
}
void VGA_Init(int width,int height,int bpp,Uint32 flags) {
	SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
	atexit(SDL_Quit);
	screen = SDL_SetVideoMode(width, height, bpp, flags);
	if (!screen) {
		printf("Couldn't set video mode: %s\n", SDL_GetError());
		exit(-1);
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
};

void VGA_set_pallette() {
	SDL_Color colors[256];
	/* Fill colors with color information */
	for (int i = 0;i < 256;i++) {
		colors[i].r = i;
		colors[i].g = i;
		colors[i].b = i;
	}

	/* Set palette */
	SDL_SetPalette(screen, SDL_LOGPAL | SDL_PHYSPAL, colors, 0, 256);
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

	memcpy(screen->pixels, buffer,0x10000);
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

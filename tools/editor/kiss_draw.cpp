/*
  kiss_sdl widget toolkit
  Copyright (c) 2016 Tarvo Korrovits <tkorrovi@mail.com>

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
	 claim that you wrote the original software. If you use this software
	 in a product, an acknowledgment in the product documentation would
	 be appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not
	 be misrepresented as being the original software.
  3. This notice may not be removed or altered from any source
	 distribution.

  kiss_sdl version 1.2.4
*/

#include "kiss_sdl.h"

kiss_font kiss_textfont, kiss_buttonfont;
kiss_image kiss_normal, kiss_prelight, kiss_active, kiss_bar,
kiss_up, kiss_down, kiss_left, kiss_left_sel, kiss_right, kiss_right_sel, kiss_vslider,
kiss_hslider, kiss_selected, kiss_unselected, kiss_combo;
int kiss_screen_width, kiss_screen_height;
int kiss_textfont_size = 15;
int kiss_buttonfont_size = 20;
int kiss_click_interval = 140;
int kiss_progress_interval = 50;
int kiss_slider_padding = 2;
int kiss_edge = 2;
int kiss_border = 6;
double kiss_spacing = 0.5;
SDL_Color kiss_white = { 255, 255, 255, 255 };
SDL_Color kiss_black = { 0, 0, 0, 255 };
SDL_Color kiss_gray = { 128, 128, 128, 255 };
SDL_Color kiss_yellow = { 255, 255, 0, 255 };
SDL_Color kiss_gryellow = { 192, 192, 64, 255 };
SDL_Color kiss_green = { 0, 150, 0, 255 };
SDL_Color kiss_blue = { 0, 0, 255, 255 };
SDL_Color kiss_grblue = { 64, 64, 128, 255 };
SDL_Color kiss_lightblue = { 200, 225, 255, 255 };
SDL_Color kiss_red = { 255, 0, 0, 255 };
SDL_Color kiss_magenta = { 255, 0, 255, 255 };

unsigned int kiss_getticks(void)
{
	return SDL_GetTicks();
}

/* Can be rewritten for proportional fonts */
int kiss_maxlength(kiss_font font, int width, char* str1, char* str2)
{
	char buf[KISS_MAX_LENGTH];
	int n, i;

	n = 0;
	if (!str1 && !str2) return -1;
	kiss_string_copy(buf, KISS_MAX_LENGTH, str1, str2);
	/* Maximum length + 1 for '\0', by the rule */
	for (i = 0; buf[i]; i += kiss_utf8next(buf, i))
		if (++n * font.advance > width)
			return i + 1;
	return i + 1;
}

/* Works also with proportional fonts */
int kiss_textwidth(kiss_font font, char* str1, char* str2)
{
	char buf[KISS_MAX_LENGTH];
	int width;

	if (!str1 && !str2) return -1;
	kiss_string_copy(buf, KISS_MAX_LENGTH, str1, str2);
	TTF_SizeUTF8(font.font, buf, &width, NULL);
	return width;
}

int kiss_renderimage(SDL_Renderer* renderer, kiss_image image,
	int x, int y, SDL_Rect* clip)
{
	SDL_Rect dst;

	if (!renderer || !image.image) return -1;
	kiss_makerect(&dst, x, y, image.w, image.h);
	if (clip) dst.w = clip->w;
	if (clip) dst.h = clip->h;
	SDL_RenderCopy(renderer, image.image, clip, &dst);
	return 0;
}

int kiss_rendertext(SDL_Renderer* renderer, char* text, int x, int y,
	kiss_font font, SDL_Color color)
{
	SDL_Surface* surface;
	kiss_image image;

	if (!text || !renderer || !font.font) return -1;
	surface = TTF_RenderUTF8_Blended(font.font, text, color);
	image.image = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_QueryTexture(image.image, NULL, NULL, &image.w, &image.h);
	if (surface) SDL_FreeSurface(surface);
	kiss_renderimage(renderer, image, x, y, NULL);
	SDL_DestroyTexture(image.image);
	return 0;
}

/*int kiss_fillrect(SDL_Renderer* renderer, SDL_Rect* rect, SDL_Color color)
{
	if (!renderer || !rect) return -1;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, rect);
	return 0;
}*/

int kiss_fillrect2(SDL_Renderer* renderer, SDL_Rect* rect, SDL_Color color)
{
	Uint32 rmask, gmask, bmask, amask;

	/* SDL interprets each pixel as a 32-bit number, so our masks must depend
	   on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	SDL_Rect dst;
	SDL_Rect highlightrect2;
	//SDL_Texture* texture;
	Uint32* pixels = nullptr;
	int pitch = 0;

	kiss_makerect(&highlightrect2, 0, 0, rect->w, rect->h);
	//texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1, 1);
	
	SDL_Surface* surface = SDL_CreateRGBSurface(0, 1, 1, 32, rmask, gmask, bmask, amask);

	if (surface == NULL) {
		fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
		exit(1);
	}
	((Uint8*)surface->pixels)[0] = color.r;
	((Uint8*)surface->pixels)[1] = color.g;
	((Uint8*)surface->pixels)[2] = color.b;
	((Uint8*)surface->pixels)[3] = color.a;


	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	if (texture == NULL) {
		fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
		exit(1);
	}


	/*
	Uint32 format;
	int w, h;
	SDL_QueryTexture(texture, &format, nullptr, &w, &h);
	if (SDL_LockTexture(texture, nullptr, (void**)&pixels, &pitch))
	{
		// If the locking fails, you might want to handle it somehow. SDL_GetError(); or something here.
	}
	pixels[0] = color.r;
	pixels[1] = color.g;
	pixels[2] = color.b;
	pixels[3] = color.a;
	SDL_UnlockTexture(texture);
	*/
	if (!renderer || !texture) return -1;
	kiss_makerect(&dst, 0, 0, rect->w, rect->h);
	SDL_RenderCopy(renderer, texture, &highlightrect2, rect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	return 0;
}

int kiss_decorate(SDL_Renderer* renderer, SDL_Rect* rect, SDL_Color color,
	int edge)
{
	SDL_Rect outlinerect;
	int d, i;

	d = 2 * edge;
	if (!renderer || !rect || rect->w < d + 6 || rect->h < d + 6)
		return -1;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	for (i = 0; i < 2; i++) {
		kiss_makerect(&outlinerect, rect->x + edge + i,
			rect->y + edge + i, rect->w - d - i - i,
			rect->h - d - i - i);
		SDL_RenderDrawRect(renderer, &outlinerect);
	}
	return 0;
}

int kiss_image_new(kiss_image* image, char* fname, kiss_array* a,
	SDL_Renderer* renderer)
{
	char buf[KISS_MAX_LENGTH];

	if (!image || !fname) return -1;
	kiss_string_copy(buf, KISS_MAX_LENGTH, (char*)RESDIR, fname);
	if (!(image->image = IMG_LoadTexture(renderer, buf))) {
		fprintf(stderr, "Cannot load image %s\n", fname);
		return -1;
	}
	if (a) kiss_array_append(a, TEXTURE_TYPE, image->image);
	SDL_QueryTexture(image->image, NULL, NULL, &image->w, &image->h);
	image->magic = KISS_MAGIC;
	return 0;
}

int kiss_font_new(kiss_font* font, char* fname, kiss_array* a, int size)
{
	char buf[KISS_MAX_LENGTH];

	if (!font || !fname) return -1;
	kiss_string_copy(buf, KISS_MAX_LENGTH, (char*)RESDIR, fname);
	if (!(font->font = TTF_OpenFont(buf, size))) {
		fprintf(stderr, "Cannot load font %s\n", fname);
		return -1;
	}
	if (a) kiss_array_append(a, FONT_TYPE, font->font);
	font->fontheight = TTF_FontHeight(font->font);
	font->spacing = (int)kiss_spacing * font->fontheight;
	font->lineheight = font->fontheight + font->spacing;
	font->ascent = TTF_FontAscent(font->font);
	TTF_GlyphMetrics(font->font, 'W', NULL, NULL, NULL, NULL,
		&(font->advance));
	font->magic = KISS_MAGIC;
	return 0;
}

SDL_Renderer* kiss_init(char* title, kiss_array* a, int w, int h)
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Rect srect;
	int r;

	r = 0;
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GetDisplayBounds(0, &srect);
	if (!a || w > srect.w || h > srect.h) {
		SDL_Quit();
		return NULL;
	}
	kiss_screen_width = w;
	kiss_screen_height = h;
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	kiss_array_new(a);
	window = SDL_CreateWindow(title, srect.w / 2 - w / 2,
		srect.h / 2 - h / 2, w, h, SDL_WINDOW_SHOWN);
	if (window) kiss_array_append(a, WINDOW_TYPE, window);
	renderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer) kiss_array_append(a, RENDERER_TYPE, renderer);
	char path2[512];
	FixDir(path2, (char*)"kiss/kiss_font.ttf");
	r += kiss_font_new(&kiss_textfont, path2, a, kiss_textfont_size);
	r += kiss_font_new(&kiss_buttonfont, path2, a, kiss_buttonfont_size);
	FixDir(path2, (char*)"kiss/kiss_normal.png");
	r += kiss_image_new(&kiss_normal, path2, a, renderer);
	FixDir(path2, (char*)"kiss/kiss_prelight.png");
	r += kiss_image_new(&kiss_prelight, path2, a, renderer);
	FixDir(path2, (char*)"kiss/kiss_active.png");
	r += kiss_image_new(&kiss_active, path2, a, renderer);
	FixDir(path2, (char*)"kiss/kiss_bar.png");
	r += kiss_image_new(&kiss_bar, path2, a, renderer);
	FixDir(path2, (char*)"kiss/kiss_vslider.png");
	r += kiss_image_new(&kiss_vslider, path2, a, renderer);
	FixDir(path2, (char*)"kiss/kiss_hslider.png");
	r += kiss_image_new(&kiss_hslider, path2, a, renderer);
	FixDir(path2, (char*)"kiss/kiss_up.png");
	r += kiss_image_new(&kiss_up, path2, a, renderer);
	FixDir(path2, (char*)"kiss/kiss_down.png");
	r += kiss_image_new(&kiss_down, path2, a, renderer);
	FixDir(path2, (char*)"kiss/kiss_left.png");
	r += kiss_image_new(&kiss_left, path2, a, renderer);
	FixDir(path2, (char*)"kiss/kiss_left_sel.png");
	r += kiss_image_new(&kiss_left_sel, path2, a, renderer);
	FixDir(path2, (char*)"kiss/kiss_right.png");
	r += kiss_image_new(&kiss_right, path2, a, renderer);
	FixDir(path2, (char*)"kiss/kiss_right_sel.png");
	r += kiss_image_new(&kiss_right_sel, path2, a, renderer);
	FixDir(path2, (char*)"kiss/kiss_combo.png");
	r += kiss_image_new(&kiss_combo, path2, a, renderer);
	FixDir(path2, (char*)"kiss/kiss_selected.png");
	r += kiss_image_new(&kiss_selected, path2, a, renderer);
	FixDir(path2, (char*)"kiss/kiss_unselected.png");
	r += kiss_image_new(&kiss_unselected, path2, a, renderer);
	if (r) {
		kiss_clean(a);
		return NULL;
	}
	return renderer;
}

int kiss_clean(kiss_array* a)
{
	int i;

	if (!a) return -1;
	if (a->length)
		for (i = a->length - 1; i >= 0; i--) {
			if (kiss_array_id(a, i) == FONT_TYPE)
				TTF_CloseFont((TTF_Font*)
					kiss_array_data(a, i));
			else if (kiss_array_id(a, i) == TEXTURE_TYPE)
				SDL_DestroyTexture((SDL_Texture*)
					kiss_array_data(a, i));
			else if (kiss_array_id(a, i) == RENDERER_TYPE)
				SDL_DestroyRenderer((SDL_Renderer*)
					kiss_array_data(a, i));
			else if (kiss_array_id(a, i) == WINDOW_TYPE)
				SDL_DestroyWindow((SDL_Window*)
					kiss_array_data(a, i));
			else if (kiss_array_id(a, i) == ARRAY_TYPE)
				kiss_array_free((kiss_array*)
					kiss_array_data(a, i));
			else
				free(a->data[i]);
		}
	a->length = 0;
	kiss_array_free(a);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}

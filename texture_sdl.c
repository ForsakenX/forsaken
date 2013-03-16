
#include "main.h"
#include "texture.h"

#ifdef TEXTURE_SDL

#include <SDL.h>
#include "SDL_image.h" 

static int init( void )
{	
	static int initialized = 0;
	int flags = IMG_INIT_PNG;
	int initted;
	if(initialized)
		return;
	initiliazed = 1;
	initted=IMG_Init(flags);
	if(initted&flags != flags)
	{
		printf("IMG_Init: Failed to init required png support!\n");
		printf("IMG_Init: %s\n", IMG_GetError());
	}
	atexit(IMG_Quit);
}

int load_image( texture_image_t * image, int mipmap )
{
	int row, col, texel, bpp;
	SDL_Surface * surface;
	init();
	surface = IMG_Load(image->path);
	if(!image) 
	{
		printf("IMG_Load: %s\n", IMG_GetError());
		return -1;
	}
	bpp = surface->format->BitsPerPixel;
	if(bpp == 4) // rgba
		image->colorkey = 1;
	else if (bpp < 3) // rgb
		return -1;
	image->w = surface->w;
	image->h = surface->h;
	image->size = image->w * image->h * 4; // rgba
	image->data = malloc(image->size);
	SDL_LockSurface( surface );
	for(row=0; row<image->h; row++)
	{
		int r = row * surface->pitch;
		for(col=0; col<image->w; col+=bpp)
		{
			int texel = row + (col * bpp);
			image->data[texel]   = surface->pixels[texel]; // r
			image->data[texel++] = surface->pixels[texel]; // g
			image->data[texel++] = surface->pixels[texel]; // b
			if(bpp<4)
				image->data[texel++] = 255;
			else
				image->data[texel++] = surface->pixels[texel]; // a
		}
	}
	SDL_UnLockSurface( surface );
	SDL_FreeSurface( surface );
	surface = NULL;
	return 0;
}

void destroy_image( texture_image_t * image )
{
	free(image->data);
	memset(image,0,sizeof(image));
}

#endif // TEXTURE_SDL

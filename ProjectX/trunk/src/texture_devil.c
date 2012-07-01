
#include "main.h"
#include "texture.h"

#ifdef TEXTURE_DEVIL


#define IL_STATIC_LIB

#include "IL/il.h"
#include "IL/ilu.h"

#define IMAGE_COMPONENTS 4 // rgba

static int devil_initialized = 0;
static int image_count = 0;

void texture_init( void )
{
	if(devil_initialized)
		return;
	ilInit();
	devil_initialized = 1;
}

void destroy_image( texture_image_t * image )
{
	ilDeleteImages(1, &image->id);
	free(image->data);
}

int load_image( texture_image_t * image, int mipmaps ) //, float gamma )
{
	int format = 0;
	ILboolean rv;
	texture_init();
	ilGenImages(1, &image->id);
	ilBindImage(image->id);
	rv = ilLoadImage(image->path);
	if (rv == IL_false)
		return -1;
	if(mipmaps==0)
	{
		rv = iluBuildMipmaps();	
		if (rv == IL_false)
			return -1;
	}
	//else
	//{
	//	// TODO - use iluScale to ensure power of two
	//}
	//rv = iluGammaCorrect( gamma );
	//if (rv == IL_false)
	//	return -1;
	image->w = ilGetInteger(IL_IMAGE_WIDTH);
	image->h = ilGetInteger(IL_IMAGE_HEIGHT);
	format	 = ilGetInteger(IL_IMAGE_FORMAT);
	// by projectx convention
	// textures that we want to color key are rbga
	if(format == IL_RGBA)	
		image->colorkey = 1; // true
	else					
		image->colorkey = 0; // false
	image->size = image->w * image->h * IMAGE_COMPONENTS * sizeof(unsigned char);
	image->data = (ILubyte *)malloc(image->size);
	ilCopyPixels(0, 0, 0, image->w, image->h, 1, IL_RGBA, IL_UNSIGNED_BYTE, image->data);
	return 0;
}

#endif

#ifndef TEXTURE_INCLUDED
#define TEXTURE_INCLUDED

typedef struct {
	int id;			// internal id
	int w;			// width
	int h;			// height
	int size;		// bytes
	int colorkey;	// bool - requires colorkeying
	char path[256];	// image path
	char * data;	// image data (bytes)
} texture_image_t;

int load_image( texture_image_t * image, int mipmap ); //, float gamma );
void destroy_image( texture_image_t * image );

#endif

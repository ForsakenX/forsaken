#ifdef TEXTURE_PNG

#include "main.h"
#include <stdio.h>
#include <string.h>
#include <png.h>

#include "texture.h"
#include "file.h"

void destroy_image( texture_image_t * image )
{
	free(image->data);
	memset(image,0,sizeof(image));
}

int load_image( texture_image_t * image, int mipmap )
{
  png_byte magic[8];
  png_structp png_ptr;
  png_infop info_ptr;
  int bit_depth, color_type;
  FILE *fp = NULL;
  png_bytep *row_pointers = NULL;
  int i;

  /* open image file */
  fp = file_open (image->path, "rb");
  if (!fp)
    {
      fprintf (stderr, "error: couldn't open \"%s\"!\n", image->path);
      return -1;
    }

  /* read magic number */
  fread (magic, 1, sizeof (magic), fp);

  /* check for valid magic number */
  if (!png_check_sig (magic, sizeof (magic)))
    {
      fprintf (stderr, "error: \"%s\" is not a valid PNG image!\n",
	       image->path);
      fclose (fp);
      return -1;
    }

  /* create a png read struct */
  png_ptr = png_create_read_struct
    (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr)
    {
      fclose (fp);
      return -1;
    }

  /* create a png info struct */
  info_ptr = png_create_info_struct (png_ptr);
  if (!info_ptr)
    {
      fclose (fp);
      png_destroy_read_struct (&png_ptr, NULL, NULL);
      return -1;
    }

  /* initialize the setjmp for returning properly after a libpng
     error occured */
  if (setjmp (png_jmpbuf (png_ptr)))
    {
      fclose (fp);
      png_destroy_read_struct (&png_ptr, &info_ptr, NULL);

      if (row_pointers)
		free (row_pointers);

      return -1;
    }

  /* setup libpng for using standard C fread() function
     with our FILE pointer */
  png_init_io (png_ptr, fp);

  /* tell libpng that we have already read the magic number */
  png_set_sig_bytes (png_ptr, sizeof (magic));

  /* read png info */
  png_read_info (png_ptr, info_ptr);

  /* get some usefull information from header */
  bit_depth = png_get_bit_depth (png_ptr, info_ptr);
  color_type = png_get_color_type (png_ptr, info_ptr);

  // 8 bits per pixel
  if (bit_depth != 8)
	  return -1;

  /* detect if not rgb */
  if (!(color_type & PNG_COLOR_MASK_COLOR))
      return -1;

  /* detect if has alpha */
  image->colorkey = (color_type & PNG_COLOR_MASK_ALPHA);
  
  // add alpha layer
  if(!image->colorkey)
	png_set_add_alpha(png_ptr, (png_uint_32)255, PNG_FILLER_AFTER);

  /* update info structure to apply transformations */
  png_read_update_info (png_ptr, info_ptr);

  /* retrieve updated information */
  png_get_IHDR (png_ptr, info_ptr,
		(png_uint_32*)(&image->w),
		(png_uint_32*)(&image->h),
		&bit_depth, &color_type,
		NULL, NULL, NULL);

  if(color_type != PNG_COLOR_TYPE_RGB_ALPHA)
	  return -1;

  /* we can now allocate memory for storing pixel data - 4 = rgba */
  image->data = (char*)malloc (sizeof (char) * image->w * image->h * 4);

  /* setup a pointer array.  Each one points at the begening of a row. */
  row_pointers = (png_bytep *)malloc (sizeof (png_bytep) * image->h);

  // TODO - need to detect *row-order* 
  // in case y axis is stored up side down
  for (i = 0; i < image->h; ++i)
      row_pointers[i] = (png_bytep)(image->data + 
	  (/*(image->h - (i + 1))*/ i * image->w * 4));

  /* read pixel data using row pointers */
  png_read_image (png_ptr, row_pointers);

  /* finish decompression and release memory */
  png_read_end (png_ptr, NULL);
  png_destroy_read_struct (&png_ptr, &info_ptr, NULL);

  /* we don't need row pointers anymore */
  free (row_pointers);

  fclose (fp);
  return 0;
}

#endif

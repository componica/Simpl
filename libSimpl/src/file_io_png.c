#include <stdlib.h>

#include <lpng-1.2.49/png.h>

#include "simpl.h"

/** This a callback function to stream in a PNG instead of a file. */
static void StreamReadData(png_structp png_ptr, png_bytep data, png_size_t length)
{
	SimplInStream istrm = (SimplInStream)png_get_io_ptr(png_ptr);
	simpl_istream_read(istrm, data, length);
}


static void StreamWriteData(png_structp png_ptr, png_bytep data, png_size_t length)
{
	SimplOutStream ostrm = (SimplOutStream)png_get_io_ptr(png_ptr);
	simpl_ostream_write(ostrm, data, length);
}


static void StreamWriteFlush(png_structp png_ptr)
{
	/* Maybe a flush function is required in SimplOutStream. */
}


int simpl_image_load_png(SimplImage **image,
                         SimplInStream istream)
{
	png_structp png_ptr;
	png_infop info_ptr;
	png_byte color_type, bitdepth, *row_data=NULL;
	png_bytep* row_ptrs=NULL;
	
	png_uint_32 i, j, width, height, row_size;
	
	SimplColorPixel *iptr;
	SimplPixel *aptr;
	
	int out = SIMPL_INTERNAL;
	
	/* Create a read struct. */
	if (!(png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0))) {
		return SIMPL_INTERNAL;
	}
	
	/* Create an info struct. */
	if (!(info_ptr = png_create_info_struct(png_ptr))) {
		goto error;
	}
	
	/* Handle libpng errors with a magic setjmp. */
	if (setjmp(png_jmpbuf(png_ptr))) {
		goto error;
	}
	
	/* Set the stream-based data source. */
	png_set_read_fn(png_ptr, istream, StreamReadData);
	
	/* Read the info chunk. */
	png_read_info(png_ptr, info_ptr);
	
	/* Get the dimensions and color information. */
	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	bitdepth = png_get_bit_depth(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);
	
	/* If palette, low bit depth gray, transparent w/o alpha, or 16 bit, fix it. */
	if (color_type == PNG_COLOR_TYPE_PALETTE) {
		png_set_palette_to_rgb(png_ptr);
		color_type = png_get_color_type(png_ptr, info_ptr);
	} else if (color_type == PNG_COLOR_TYPE_GRAY) {
		if (bitdepth<8) png_set_expand_gray_1_2_4_to_8(png_ptr);
		bitdepth = 8;
	}
	
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
		png_set_tRNS_to_alpha(png_ptr);
		color_type = png_get_color_type(png_ptr, info_ptr);
	}
	
	png_set_strip_16(png_ptr);
	
	/* Either allocate a row, or load the entire image into a buffer. This is
	 * because single row access uses less memory but doesn't work for
	 * interlaced PNGs.
	 */
	row_size = png_get_rowbytes(png_ptr, info_ptr);
	if (png_get_interlace_type(png_ptr, info_ptr)==PNG_INTERLACE_NONE) {
		row_data = (png_byte *)malloc(sizeof(png_byte) * row_size);
		if (!row_data) {
			out = SIMPL_NOMEM;
			goto error;
		}
	} else {
		row_ptrs = (png_bytep*)calloc(height, sizeof(png_bytep));
		if (!row_ptrs) {
			out = SIMPL_NOMEM;
			goto error;
		}
		
		for (j=0; j<height; ++j) {
			row_ptrs[j] = (png_byte *)malloc(sizeof(png_byte) * row_size);
			if (!row_ptrs[j]) {
				out = SIMPL_NOMEM;
				goto error;
			}
		}
		
		png_read_image(png_ptr, row_ptrs);
	}
		
	/* Allocate an image of the specified size. */
	out = simpl_image(image, width, height);
	if (out != SIMPL_OK) goto error;
		
	/* Store the decoded image into our format. */
	if (color_type == PNG_COLOR_TYPE_RGB) {
		for (j=0; j<height; j++) {
			if (row_ptrs) row_data = row_ptrs[j];
			else png_read_row(png_ptr, row_data, NULL);
			
			iptr = (*image)->image + j * width;
			for (i=0; i<3*width; i+=3) {
				iptr->red   = row_data[i];
				iptr->green = row_data[i+1];
				iptr->blue  = row_data[i+2];
				iptr++;
			}
		}
	} else if (color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
		simpl_alpha_create(*image);
		for (j=0; j<height; j++) {
			if (row_ptrs) row_data = row_ptrs[j];
			else png_read_row(png_ptr, row_data, NULL);
			
			iptr = (*image)->image + j * width;
			aptr = (*image)->alpha + j * width;
			for (i=0; i<4*width; i+=4) {
				iptr->red   = row_data[i];
				iptr->green = row_data[i+1];
				iptr->blue  = row_data[i+2];
				iptr++;
			
				*aptr++ = row_data[i+3];
			}
		}
	} else if (color_type == PNG_COLOR_TYPE_GRAY) {
		for (j=0; j<height; j++) {
			if (row_ptrs) row_data = row_ptrs[j];
			else png_read_row(png_ptr, row_data, NULL);
			
			iptr = (*image)->image + j * width;
			for (i=0; i<width; i++) {
				iptr->red   = row_data[i];
				iptr->green = row_data[i];
				iptr->blue  = row_data[i];
				iptr++;
			}
		}
	} else if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
		simpl_alpha_create(*image);
		for (j=0; j<height; j++) {
			if (row_ptrs) row_data = row_ptrs[j];
			else png_read_row(png_ptr, row_data, NULL);
			
			iptr = (*image)->image + j * width;
			aptr = (*image)->alpha + j * width;
			for (i=0; i<2*width; i+=2) {
				iptr->red   = row_data[i];
				iptr->green = row_data[i];
				iptr->blue  = row_data[i];
				iptr++;
			
				*aptr++ = row_data[i+1];
			}
		}
	} else goto error;
	
error:
	if (row_ptrs) {
		for (j=0; j<height; ++j) {
			if (row_ptrs[j]) free((void *)row_ptrs[j]);
		}
		
		free((void *)row_ptrs);
	} else {
		if (row_data) free((void *)row_data);
	}
	
	png_destroy_read_struct(&png_ptr, &info_ptr, 0);
	return out;
}


int simpl_gray_load_png(SimplGrayImage **image,
                        SimplInStream istream,
                        const SimplColorToGrayMethods method,
                        const SimplPixel bk_value)
{
	png_structp png_ptr;
	png_infop info_ptr;
	png_byte color_type, bitdepth, *row_data=NULL;
	png_bytep* row_ptrs=NULL;
	
	png_uint_32 i, j, x, width, height, row_size;
	
	SimplColorPixel *pixels=NULL;
	SimplPixel *iptr, *alpha=NULL;
	
	int value, out = SIMPL_INTERNAL;
	
	/* Create a read struct. */
	if (!(png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0))) {
		return SIMPL_INTERNAL;
	}
	
	/* Create an info struct. */
	if (!(info_ptr = png_create_info_struct(png_ptr))) {
		goto error;
	}
	
	/* Handle libpng errors with a magic setjmp. */
	if (setjmp(png_jmpbuf(png_ptr))) {
		goto error;
	}
	
	/* Set the stream-based data source. */
	png_set_read_fn(png_ptr, istream, StreamReadData);
	
	/* Read the info chunk. */
	png_read_info(png_ptr, info_ptr);
	
	/* Get the dimensions and color information. */
	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	bitdepth = png_get_bit_depth(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);
	
	/* If palette, low bit depth gray, transparent w/o alpha, or 16 bit, fix it. */
	if (color_type == PNG_COLOR_TYPE_PALETTE) {
		png_set_palette_to_rgb(png_ptr);
		color_type = png_get_color_type(png_ptr, info_ptr);
	} else if (color_type == PNG_COLOR_TYPE_GRAY) {
		if (bitdepth<8) png_set_expand_gray_1_2_4_to_8(png_ptr);
		bitdepth = 8;
	}
	
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
		png_set_tRNS_to_alpha(png_ptr);
		color_type = png_get_color_type(png_ptr, info_ptr);
	}
	
	png_set_strip_16(png_ptr);
	
	/* Either allocate a row, or load the entire image into a buffer. This is
	 * because single row access uses less memory but doesn't work for
	 * interlaced PNGs.
	 */
	row_size = png_get_rowbytes(png_ptr, info_ptr);
	if (png_get_interlace_type(png_ptr, info_ptr)==PNG_INTERLACE_NONE) {
		row_data = (png_byte *)malloc(sizeof(png_byte) * row_size);
		if (!row_data) {
			out = SIMPL_NOMEM;
			goto error;
		}
	} else {
		row_ptrs = (png_bytep*)calloc(height, sizeof(png_bytep));
		if (!row_ptrs) {
			out = SIMPL_NOMEM;
			goto error;
		}
		
		for (j=0; j<height; ++j) {
			row_ptrs[j] = (png_byte *)malloc(sizeof(png_byte) * row_size);
			if (!row_ptrs[j]) {
				out = SIMPL_NOMEM;
				goto error;
			}
		}
		
		png_read_image(png_ptr, row_ptrs);
	}
	
	/* Allocate an image of the specified size. */
	out = simpl_gray(image, width, height);
	if (out != SIMPL_OK) goto error;
	
	/* Decode the image line by line. */
	if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
		pixels = (SimplColorPixel *)malloc(sizeof(SimplColorPixel) * width);
		if (!pixels) {
			out = SIMPL_NOMEM;
			goto error;
		}
		
		alpha = (SimplPixel *)malloc(sizeof(SimplPixel) * width);
		if (!alpha) {
			out = SIMPL_NOMEM;
			goto error;
		}
		memset(alpha, 255, width);
		
		for (j=0; j<height; j++) {
			if (row_ptrs) row_data = row_ptrs[j];
			else png_read_row(png_ptr, row_data, NULL);
			iptr = (*image)->image + j * width;
			
			if (color_type==PNG_COLOR_TYPE_RGB) {
				for (x=i=0; x<width; ++x, i+=3) {
					pixels[x].red   = row_data[i];
					pixels[x].green = row_data[i+1];
					pixels[x].blue  = row_data[i+2];
				}
			} else {
				for (x=i=0; x<width; ++x, i+=4) {
					pixels[x].red   = row_data[i];
					pixels[x].green = row_data[i+1];
					pixels[x].blue  = row_data[i+2];
					alpha[x]        = row_data[i+3];
				}
			}
			
			switch(method) {
			case COLOR_TO_GRAY_RED:
				for (i=0; i<width; ++i) iptr[i] = pixels[i].red;
				break;
				
			case COLOR_TO_GRAY_GREEN:
				for (i=0; i<width; ++i) iptr[i] = pixels[i].green;
				break;
				
			case COLOR_TO_GRAY_BLUE:
				for (i=0; i<width; ++i) iptr[i] = pixels[i].blue;
				break;
				
			case COLOR_TO_GRAY_ALPHA:
				for (i=0; i<width; ++i) iptr[i] = alpha[i];
				break;
				
			case COLOR_TO_GRAY_CIE:
				if (color_type==PNG_COLOR_TYPE_RGB_ALPHA) {
					for (i=0; i<width; ++i) {
						value = (6969*pixels[i].red + 23434*pixels[i].green + 2365*pixels[i].blue)>>15;
						value = (alpha[i]*value + (255-alpha[i])*bk_value) / 255;
						iptr[i] = (value>255)?255:value;
					}
				} else {
					for (i=0; i<width; ++i) {
						value = (6969*pixels[i].red + 23434*pixels[i].green + 2365*pixels[i].blue)>>15;
						iptr[i] = (value>255)?255:value;
					}
				}
				break;
				
			case COLOR_TO_GRAY_MEAN:
			default:
				if (color_type==PNG_COLOR_TYPE_RGB_ALPHA) {
					for (i=0; i<width; ++i) {
						value = (pixels[i].red + pixels[i].green + pixels[i].blue)/3;
						value = (alpha[i]*value + (255-alpha[i])*bk_value) / 255;
						iptr[i] = (value>255)?255:value;
					}
				} else {
					for (i=0; i<width; ++i) {
						value = (pixels[i].red + pixels[i].green + pixels[i].blue)/3;
						iptr[i] = (value>255)?255:value;
					}
				}
				break;
			}
		}
	} else if (color_type == PNG_COLOR_TYPE_GRAY) {
		for (j=0; j<height; j++) {
			if (row_ptrs) row_data = row_ptrs[j];
			else png_read_row(png_ptr, row_data, NULL);
			iptr = (*image)->image + j * width;
			for (i=0; i<width; i++) {
				iptr[i] = row_data[i];
			}
		}
	} else if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
		for (j=0; j<height; j++) {
			if (row_ptrs) row_data = row_ptrs[j];
			else png_read_row(png_ptr, row_data, NULL);
			iptr = (*image)->image + j * width;
			for (i=0; i<2*width; i+=2) {
				value = row_data[i+1];
				value = (value*row_data[i] + (255-value)*bk_value) / 255;
				iptr[i] = (value>255)?255:value;
			}
		}
	} else goto error;
	
error:
	if (row_ptrs) {
		for (j=0; j<height; ++j) {
			if (row_ptrs[j]) free((void *)row_ptrs[j]);
		}
		
		free((void *)row_ptrs);
	} else {
		if (row_data) free((void *)row_data);
	}
	
	if (pixels) free((void *)pixels);
	if (alpha) free((void *)alpha);
	
	png_destroy_read_struct(&png_ptr, &info_ptr, 0);
	return out;
}


int simpl_image_save_png(SimplOutStream ostream,
                         const SimplImage *image)
{
	png_structp png_ptr;
	png_infop info_ptr;
	png_byte *row_data=NULL;
	
	const SimplColorPixel *iptr;
	const SimplPixel *aptr;
	size_t i, j, x;
	
	int out = SIMPL_INTERNAL;
	
	if (!image || !image->image || !image->width || !image->height) return SIMPL_BAD_PARAMS;
	
	/* Create a write struct. */
	if (!(png_ptr=png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0))) {
		return SIMPL_INTERNAL;
	}
	
	/* Create an info struct. */
	if (!(info_ptr = png_create_info_struct(png_ptr))) {
		goto error;
	}
	
	/* Block to handle libpng errors and set the file pointer. */
	if (setjmp(png_jmpbuf(png_ptr))) {
		goto error;
	}
	
	row_data = (png_byte *)malloc(sizeof(png_byte *) * 4 * image->width);
	if (!row_data) {
		out = SIMPL_NOMEM;
		goto error;
	}
	
	png_set_write_fn(png_ptr, ostream, StreamWriteData, StreamWriteFlush);
	
	/* Create a file header. */
	png_set_IHDR(png_ptr, info_ptr, image->width, image->height, 8,
		(image->alpha?PNG_COLOR_TYPE_RGB_ALPHA:PNG_COLOR_TYPE_RGB),
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
		PNG_FILTER_TYPE_BASE
	);
	
	png_write_info(png_ptr, info_ptr);
	
	if (image->alpha) {
		for (j=0; j<image->height; j++) {
			iptr = image->image + j * image->width;
			aptr = image->alpha + j * image->width;
			
			for (x=i=0; x<image->width; ++x, i+=4) {
				row_data[i]   = iptr[x].red;
				row_data[i+1] = iptr[x].green;
				row_data[i+2] = iptr[x].blue;
				row_data[i+3] = aptr[x];
			}
			
			png_write_row(png_ptr, row_data);
		}
	} else {
		for (j=0; j<image->height; j++) {
			iptr = image->image + j * image->width;
			
			for (x=i=0; x<image->width; ++x, i+=3) {
				row_data[i]   = iptr[x].red;
				row_data[i+1] = iptr[x].green;
				row_data[i+2] = iptr[x].blue;
			}
			
			png_write_row(png_ptr, row_data);
		}
	}
	
	png_write_end(png_ptr, info_ptr);
	out = SIMPL_OK;
	
error:
	if (row_data) free((void *)row_data);
	png_destroy_write_struct(&png_ptr, &info_ptr);
	return out;
}


int simpl_gray_save_png(SimplOutStream ostream,
                        const SimplGrayImage *image)
{
	png_structp png_ptr;
	png_infop info_ptr;
	
	const SimplPixel *iptr;
	size_t j;
	
	int out = SIMPL_INTERNAL;
	
	if (!image || !image->image || !image->width || !image->height) return SIMPL_BAD_PARAMS;
	
	/* Create a write struct. */
	if (!(png_ptr=png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0))) {
		return SIMPL_INTERNAL;
	}
	
	/* Create an info struct. */
	if (!(info_ptr = png_create_info_struct(png_ptr))) {
		goto error;
	}
	
	/* Block to handle libpng errors and set the file pointer. */
	if (setjmp(png_jmpbuf(png_ptr))) {
		goto error;
	}
	
	png_set_write_fn(png_ptr, ostream, StreamWriteData, StreamWriteFlush);
	
	/* Create a file header. */
	png_set_IHDR(png_ptr, info_ptr, image->width, image->height, 8,
		PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT
	);
	
	png_write_info(png_ptr, info_ptr);
	
	for (j=0; j<image->height; j++) {
		iptr = image->image + j * image->width;
		png_write_row(png_ptr, (png_byte *)iptr);
	}
	
	png_write_end(png_ptr, info_ptr);
	out = SIMPL_OK;
	
error:
	png_destroy_write_struct(&png_ptr, &info_ptr);
	return out;
}

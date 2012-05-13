#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>

#include "simpl.h"

SimplColorPixel simpl_color(const int red,
                            const int green,
                            const int blue)
{
	SimplColorPixel pixel;
	
	pixel.red   = (red<0)?0:((red>255)?255:red);
	pixel.green = (green<0)?0:((green>255)?255:green);
	pixel.blue  = (blue<0)?0:((blue>255)?255:blue);
	
	return pixel;
}


int simpl_image(SimplImage **image,
                const size_t width,
                const size_t height)
{
	int has_alpha;
	
	if (*image) {
		if ((*image)->width!=width || (*image)->height!=height) {
			if ((*image)->image) {
				free((void *)(*image)->image);
				(*image)->image = NULL;
			}
			
			has_alpha = FALSE;
			if ((*image)->alpha) {
				free((void *)(*image)->alpha);
				(*image)->alpha = NULL;
				has_alpha = TRUE;
			}
			
			if (width && height) {
				(*image)->image = (SimplColorPixel *)calloc(width*height, sizeof(SimplColorPixel));
				if (!(*image)->image) {
					free((void *)(*image));
					*image = NULL;
					return SIMPL_NOMEM;
				}
				
				if (has_alpha) {
					(*image)->alpha = (SimplPixel *)malloc(width*height*sizeof(SimplPixel));
					if (!(*image)->alpha) {
						free((void *)((*image)->image));
						free((void *)(*image));
						*image = NULL;
						return SIMPL_NOMEM;
					}
					
					memset((*image)->alpha, 255, width*height);
				}
				
				(*image)->width = width;
				(*image)->height = height;
			} else {
				(*image)->width = (*image)->height = 0;
				return SIMPL_BAD_PARAMS;
			}
		}
	} else {
		*image = (SimplImage *)malloc(sizeof(SimplImage));
		if (!(*image)) {
			*image = NULL;
			return SIMPL_NOMEM;
		}
		
		(*image)->image = NULL;
		(*image)->alpha = NULL;
		
		if (width && height) {
			(*image)->image = (SimplColorPixel *)calloc(width*height, sizeof(SimplColorPixel));
			if (!(*image)->image) {
				free((void *)(*image));
				*image = NULL;
				return SIMPL_NOMEM;
			}
			
			(*image)->width = width;
			(*image)->height = height;
		} else {
			(*image)->width = (*image)->height = 0;
			return SIMPL_BAD_PARAMS;
		}
	}
	
	return SIMPL_OK;
}


void simpl_image_free(SimplImage **image)
{
	if (*image) {
		if ((*image)->image) {
			free((void *)((*image)->image));
		}
		
		if ((*image)->alpha) {
			free((void *)((*image)->alpha));
		}
		
		free((void *)(*image));
		*image = NULL;
	}
}


int simpl_image_clear(SimplImage *image,
                      const SimplColorPixel bk_color)
{
	size_t i, size;
	
	if (image && image->image && image->width && image->height) {
		size = image->width * image->height;
		
		for (i=0; i<size; ++i) {
			image->image[i] = bk_color;
		}
		
		if (image->alpha) {
			memset(image->alpha, 255, size);
		}
		
		return SIMPL_OK;
	} else {
		return SIMPL_BAD_PARAMS;
	}
}


int simpl_image_copy(SimplImage **dest_img,
                     const SimplImage *src_img)
{
	int result;
	
	if (src_img && src_img->image && src_img->width && src_img->height) {
		if (*dest_img==src_img) return SIMPL_OK;
		
		result = simpl_image(dest_img, src_img->width, src_img->height);
		if (result!=SIMPL_OK) return result;
		
		memcpy((*dest_img)->image, src_img->image, src_img->width * src_img->height * sizeof(SimplColorPixel));
		
		if (src_img->alpha) {
			simpl_alpha_create(*dest_img);
			memcpy((*dest_img)->alpha, src_img->alpha, src_img->width * src_img->height * sizeof(SimplPixel));
		}
	} else return SIMPL_BAD_PARAMS;
	
	return SIMPL_OK;
}


int simpl_image_compare(const SimplImage *image1,
                        const SimplImage *image2)
{
	size_t i, size;
	
	if (image1 && image1->image && image2 && image2->image) {
		if (image1->width==image2->width && image1->height==image2->height) {
			size = image1->width * image1->height;
			
			if ((image1->alpha==NULL && image2->alpha!=NULL) ||
				(image1->alpha!=NULL && image2->alpha==NULL)) return FALSE;
			
			if (image1->alpha && image2->alpha) {
				for (i=0; i<size; ++i) {
					if (image1->alpha[i] != image2->alpha[i]) return FALSE;
				}
			}
			
			for (i=0; i<size; ++i) {
				if ((image1->image[i].red != image2->image[i].red) ||
					(image1->image[i].green != image2->image[i].green) ||
					(image1->image[i].blue != image2->image[i].blue)) return FALSE;
			}
			
			return TRUE;
		}
	}
	
	return FALSE;
}


int simpl_image_crop(SimplImage **dest_img,
                     const SimplImage *src_img,
                     const SimplRect *rect)
{
	return SIMPL_OK;
}


int simpl_image_patch(SimplImage **dest_img,
                      const SimplImage *src_img,
                      const SimplRect *rect,
                      const SimplBoundaryMethods boundary,
                      const SimplColorPixel bk_color,
                      const SimplPixel bk_alpha)
{
	return SIMPL_OK;
}


int simpl_image_flip(SimplImage **dest_img,
                     const SimplImage *src_img,
                     const int flip_horizontal,
                     const int flip_vertical)
{
	return SIMPL_OK;
}


int simpl_image_rotate_ninety(SimplImage **dest_img,
                              const SimplImage *src_img,
                              const int rotations)
{
	return SIMPL_OK;
}


int simpl_image_to_gray(SimplGrayImage **gray_img,
                        const SimplImage *image,
                        const SimplColorToGrayMethods method,
                        const SimplPixel bk_value)
{
	size_t i, size;
	int result, value;
	SimplPixel *out;
	
	if (image && image->image && image->width && image->height) {
		result = simpl_gray(gray_img, image->width, image->height);
		if (result!=SIMPL_OK) return result;
		
		size = image->width * image->height;
		out = (*gray_img)->image;
		
		switch (method) {
		case COLOR_TO_GRAY_RED:
			for (i=0; i<size; ++i) {
				out[i] = image->image[i].red;
			}
			break;
		
		case COLOR_TO_GRAY_GREEN:
			for (i=0; i<size; ++i) {
				out[i] = image->image[i].green;
			}
			break;
		
		case COLOR_TO_GRAY_BLUE:
			for (i=0; i<size; ++i) {
				out[i] = image->image[i].blue;
			}
			break;
		
		case COLOR_TO_GRAY_ALPHA:
			if (image->alpha) {
				for (i=0; i<size; ++i) {
					out[i] = image->alpha[i];
				}
			} else {
				for (i=0; i<size; ++i) {
					out[i] = 255;
				}
			}
			break;
		
		case COLOR_TO_GRAY_CIE:
			if (image->alpha) {
				for (i=0; i<size; ++i) {
					value = (6969*image->image[i].red + 23434*image->image[i].green + 2365*image->image[i].blue)>>15;
					value = (image->alpha[i]*value + (255-image->alpha[i])*bk_value) / 255;
					out[i] = (value>255)?255:value;
				}
			} else {
				for (i=0; i<size; ++i) {
					value = (6969*image->image[i].red + 23434*image->image[i].green + 2365*image->image[i].blue)>>15;
					out[i] = (value>255)?255:value;
				}
			}
			break;
		
		case COLOR_TO_GRAY_MEAN:
		default:
			if (image->alpha) {
				for (i=0; i<size; ++i) {
					value = (image->image[i].red + image->image[i].green + image->image[i].blue) / 3;
					value = (image->alpha[i]*value + (255-image->alpha[i])*bk_value) / 255;
					out[i] = (value>255)?255:value;
				}
			} else {
				for (i=0; i<size; ++i) {
					value = (image->image[i].red + image->image[i].green + image->image[i].blue) / 3;
					out[i] = (value>255)?255:value;
				}
			}
			break;
		}
	} else return SIMPL_BAD_PARAMS;
	
	return SIMPL_OK;
}


int simpl_alpha_create(SimplImage *image)
{
	if (image && image->image && image->width && image->height) {
		if (image->alpha) {
			image->alpha = (SimplPixel *)malloc(image->width*image->height*sizeof(SimplPixel));
			if (!image->alpha) return SIMPL_NOMEM;
			memset(image->alpha, 255, image->width*image->height);
		}
	} else return SIMPL_BAD_PARAMS;
		
	return SIMPL_OK;
}


void simpl_alpha_remove(SimplImage *image)
{
	if (image && image->alpha) {
		free((void *)image->alpha);
		image->alpha = NULL;
	}
}


#ifndef NDEBUG

#include <assert.h>
#include <stdio.h>

void test_image(void)
{
	printf("\tTesting Color Images.\n");
}

#endif

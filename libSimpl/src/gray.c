#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>

#include "simpl.h"


int simpl_gray(SimplGrayImage **image,
               const size_t width,
               const size_t height)
{
	if (*image) {
		if ((*image)->width!=width || (*image)->height!=height) {
			if ((*image)->image) {
				free((void *)(*image)->image);
				(*image)->image = NULL;
			}
			
			if (width && height) {
				(*image)->image = (SimplPixel *)calloc(width*height, sizeof(SimplPixel));
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
	} else {
		*image = (SimplGrayImage *)malloc(sizeof(SimplGrayImage));
		if (!(*image)) {
			*image = NULL;
			return SIMPL_NOMEM;
		}
		
		(*image)->image = NULL;
		
		if (width && height) {
			(*image)->image = (SimplPixel *)calloc(width*height, sizeof(SimplPixel));
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


void simpl_gray_free(SimplGrayImage **image)
{
	if (*image) {
		if ((*image)->image) {
			free((void *)((*image)->image));
		}
		
		free((void *)(*image));
		*image = NULL;
	}
}


int simpl_gray_clear(SimplGrayImage *image,
                     const SimplPixel bk_value)
{
	if (image && image->image && image->width && image->height) {
		memset(image->image, bk_value, image->width*image->height);
		return SIMPL_OK;
	} else {
		return SIMPL_BAD_PARAMS;
	}
}


int simpl_gray_copy(SimplGrayImage **dest_img,
                    const SimplGrayImage *src_img)
{
	int result;
	
	if (src_img && src_img->image && src_img->width && src_img->height) {
		if (*dest_img==src_img) return SIMPL_OK;
		
		result = simpl_gray(dest_img, src_img->width, src_img->height);
		if (result!=SIMPL_OK) return result;
		
		memcpy((*dest_img)->image, src_img->image, src_img->width * src_img->height * sizeof(SimplPixel));
	} else return SIMPL_BAD_PARAMS;
	
	return SIMPL_OK;
}


int simpl_gray_compare(const SimplGrayImage *image1,
                       const SimplGrayImage *image2)
{
	size_t i, size;
	
	if (image1 && image1->image && image2 && image2->image) {
		if (image1->width==image2->width && image1->height==image2->height) {
			size = image1->width * image1->height;
			
			for (i=0; i<size; ++i) {
				if ((image1->image[i] != image2->image[i])) return FALSE;
			}
			
			return TRUE;
		}
	}
	
	return FALSE;
}


int simpl_gray_crop(SimplGrayImage **dest_img,
                    const SimplGrayImage *src_img,
                    const SimplRect *rect)
{
	return SIMPL_OK;
}


int simpl_gray_patch(SimplGrayImage **dest_img,
                     const SimplGrayImage *src_img,
                     const SimplRect *rect,
                     const SimplBoundaryMethods boundary,
                     const SimplColorPixel bk_value)
{
	return SIMPL_OK;
}


int simpl_gray_flip(SimplGrayImage **dest_img,
                    const SimplGrayImage *src_img,
                    const int flip_horizontal,
                    const int flip_vertical)
{
	return SIMPL_OK;
}


int simpl_gray_rotate_ninety(SimplGrayImage **dest_img,
                             const SimplGrayImage *src_img,
                             const int rotations)
{
	return SIMPL_OK;
}


int simpl_gray_to_image(SimplImage **image,
                        const SimplGrayImage *gray_img,
                        const SimplGrayToColorMethods method)
{
	size_t i, size;
	int result;
	
	if (gray_img && gray_img->image && gray_img->width && gray_img->height) {
		result = simpl_image(image, gray_img->width, gray_img->height);
		if (result!=SIMPL_OK) return result;
		
		size = gray_img->width * gray_img->height;
		
		switch (method) {
		case GRAY_TO_COLOR_RED:
			for (i=0; i<size; ++i) {
				(*image)->image[i].red = gray_img->image[i];
			}
			break;
			
		case GRAY_TO_COLOR_GREEN:
			for (i=0; i<size; ++i) {
				(*image)->image[i].green = gray_img->image[i];
			}
			break;
			
		case GRAY_TO_COLOR_BLUE:
			for (i=0; i<size; ++i) {
				(*image)->image[i].blue = gray_img->image[i];
			}
			break;
			
		case GRAY_TO_COLOR_ALPHA:
			simpl_alpha_create(*image);
			for (i=0; i<size; ++i) {
				(*image)->alpha[i] = gray_img->image[i];
			}
			break;
			
		case GRAY_TO_ALL:
		default:
			for (i=0; i<size; ++i) {
				(*image)->image[i].red   = gray_img->image[i];
				(*image)->image[i].green = gray_img->image[i];
				(*image)->image[i].blue  = gray_img->image[i];
			}
			break;
		}
	}  else return SIMPL_BAD_PARAMS;
	
	return SIMPL_OK;
}


#ifndef NDEBUG

#include <assert.h>
#include <stdio.h>

void test_gray(void)
{
	printf("\tTesting Gray Images.\n");
}

#endif

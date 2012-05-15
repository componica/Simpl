#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "simpl.h"

static SimplFileFormat GuessFileFormat(uint8_t header[8])
{
	if (header[0]==0x89 && header[1]==0x50 && header[2]==0x4E && header[3]==0x47 && 
		header[4]==0x0D && header[5]==0x0A && header[6]==0x1A && header[7]==0x0A) return IMAGE_PNG;
	else if (header[0]==0x47 && header[1]==0x49 && header[2]==0x46 && header[3]==0x38) return IMAGE_GIF;
	else if (header[0]==0xFF && header[1]==0xD8) return IMAGE_JPG;
	else if (header[0]==0x42 && header[1]==0x4D) return IMAGE_BMP;

	return IMAGE_NONE;
}


static SimplFileFormat GuessFileExtension(const char *filename)
{
	int i;
	char buffer[6];
	const char *str = filename;
	
	while (str && *str) ++str;
	while (str && str>filename && *str!='.') --str;
	
	for (i=0; i<6; ++i) buffer[i] = '\0';
	
	if (*str=='.') {
		++str;
		for (i=0; i<5 && *str; ++i) buffer[i] = tolower(*str++);
	}
	
	if (strcmp(buffer, "png")==0) {
		return IMAGE_PNG;
	} else if (strcmp(buffer, "jpg")==0 || strcmp(buffer, "jpeg")==0) {
		return IMAGE_JPG;
	} else if (strcmp(buffer, "bmp")==0) {
		return IMAGE_BMP;
	} else if (strcmp(buffer, "gif")==0) {
		return IMAGE_GIF;
	}
	
	return IMAGE_NONE;
}


int simpl_image_load(SimplImage **image,
                     const char *filename)
{
	SimplInStream istr = NULL;
	uint8_t header[8];
	int out = SIMPL_OK;
	
	istr = simpl_istream_from_file(filename);
	if (istr) {
		if (simpl_istream_peek(istr, header, 8)!=8) {
			out = SIMPL_INTERNAL;
			goto finished;
		}
		
		switch(GuessFileFormat(header)) {
		case IMAGE_GIF:
			break;
			
		case IMAGE_BMP:
			break;
			
		case IMAGE_JPG:
			break;
			
		case IMAGE_PNG:
			out = simpl_image_load_png(image, istr);
			break;
			
		default:
			out = SIMPL_INTERNAL;
			break;
		}
	} else out = SIMPL_BAD_PARAMS;

finished:
	if (istr) simpl_istream_free(&istr);
	return out;
}


int simpl_image_load_buffer(SimplImage **image,
                            const uint8_t *data,
                            const size_t length)
{
	SimplInStream istr = NULL;
	uint8_t header[8];
	int out = SIMPL_OK;
	
	istr = simpl_istream_from_buffer(data, length, FALSE);
	if (istr) {
		if (simpl_istream_peek(istr, header, 8)!=8) {
			out = SIMPL_INTERNAL;
			goto finished;
		}
		
		switch(GuessFileFormat(header)) {
		case IMAGE_GIF:
			break;
			
		case IMAGE_BMP:
			break;
			
		case IMAGE_JPG:
			break;
			
		case IMAGE_PNG:
			out = simpl_image_load_png(image, istr);
			break;
			
		default:
			out = SIMPL_INTERNAL;
			break;
		}
	} else out = SIMPL_BAD_PARAMS;

finished:
	if (istr) simpl_istream_free(&istr);
	return out;
}


int simpl_image_save(const SimplImage *image,
                     const char *filename)
{
	SimplOutStream ostr = NULL;
	int out = SIMPL_INTERNAL;
	
	ostr = simpl_ostream_to_file(filename);
	if (ostr) {
		switch (GuessFileExtension(filename)) {
		case IMAGE_GIF:
			break;
			
		case IMAGE_BMP:
			break;
			
		case IMAGE_JPG:
			break;
			
		case IMAGE_PNG:
		default:
			out = simpl_image_save_png(ostr, image);
			break;
		}
	} else out = SIMPL_BAD_PARAMS;
	
	if (ostr) simpl_ostream_free(&ostr);
	return out;
}


int simpl_image_save_buffer(uint8_t **data,
                            size_t *length,
                            const SimplImage *image,
                            const SimplFileFormat format)
{
	SimplOutStream ostr = NULL;
	int out = SIMPL_INTERNAL;
	
	ostr = simpl_ostream_to_buffer();
	if (ostr) {
		switch (format) {
		case IMAGE_GIF:
			break;
			
		case IMAGE_BMP:
			break;
			
		case IMAGE_JPG:
			break;
			
		case IMAGE_PNG:
		default:
			out = simpl_image_save_png(ostr, image);
			break;
		}
	} else out = SIMPL_BAD_PARAMS;
	
	if (ostr) simpl_ostream_free(&ostr);
	return out;
}


int simpl_gray_load(SimplGrayImage **image,
                    const char *filename,
                    const SimplColorToGrayMethods method,
                    const SimplPixel bk_value)
{
	SimplInStream istr = NULL;
	uint8_t header[8];
	int out = SIMPL_OK;
	
	istr = simpl_istream_from_file(filename);
	if (istr) {
		if (simpl_istream_peek(istr, header, 8)!=8) {
			out = SIMPL_INTERNAL;
			goto finished;
		}
		
		switch(GuessFileFormat(header)) {
		case IMAGE_GIF:
			break;
			
		case IMAGE_BMP:
			break;
			
		case IMAGE_JPG:
			break;
			
		case IMAGE_PNG:
			out = simpl_gray_load_png(image, istr, method, bk_value);
			break;
			
		default:
			out = SIMPL_INTERNAL;
			break;
		}
	} else out = SIMPL_BAD_PARAMS;

finished:
	if (istr) simpl_istream_free(&istr);
	return out;
}


int simpl_gray_load_buffer(SimplGrayImage **image,
                           const uint8_t *data,
                           const size_t length,
                           const SimplColorToGrayMethods method,
                           const SimplPixel bk_value)
{
	SimplInStream istr = NULL;
	uint8_t header[8];
	int out = SIMPL_OK;
	
	istr = simpl_istream_from_buffer(data, length, FALSE);
	if (istr) {
		if (simpl_istream_peek(istr, header, 8)!=8) {
			out = SIMPL_INTERNAL;
			goto finished;
		}
		
		switch(GuessFileFormat(header)) {
		case IMAGE_GIF:
			break;
			
		case IMAGE_BMP:
			break;
			
		case IMAGE_JPG:
			break;
			
		case IMAGE_PNG:
			out = simpl_gray_load_png(image, istr, method, bk_value);
			break;
			
		default:
			out = SIMPL_INTERNAL;
			break;
		}
	} else out = SIMPL_BAD_PARAMS;

finished:
	if (istr) simpl_istream_free(&istr);
	return out;
}


int simpl_gray_save(const SimplGrayImage *image,
                    const char *filename)
{
	SimplOutStream ostr = NULL;
	int out = SIMPL_INTERNAL;
	
	ostr = simpl_ostream_to_file(filename);
	if (ostr) {
		switch (GuessFileExtension(filename)) {
		case IMAGE_GIF:
			break;
			
		case IMAGE_BMP:
			break;
			
		case IMAGE_JPG:
			break;
			
		case IMAGE_PNG:
		default:
			out = simpl_gray_save_png(ostr, image);
			break;
		}
	} else out = SIMPL_BAD_PARAMS;
	
	if (ostr) simpl_ostream_free(&ostr);
	return out;
}


int simpl_gray_save_buffer(uint8_t **data,
                           size_t *length,
                           const SimplGrayImage *image,
                           const SimplFileFormat format)
{
	SimplOutStream ostr = NULL;
	int out = SIMPL_INTERNAL;
	
	ostr = simpl_ostream_to_buffer();
	if (ostr) {
		switch (format) {
		case IMAGE_GIF:
			break;
			
		case IMAGE_BMP:
			break;
			
		case IMAGE_JPG:
			break;
			
		case IMAGE_PNG:
		default:
			out = simpl_gray_save_png(ostr, image);
			break;
		}
	} else out = SIMPL_BAD_PARAMS;
	
	if (ostr) simpl_ostream_free(&ostr);
	return out;
}


#ifndef NDEBUG

#include <assert.h>
#include <stdio.h>

void test_image_io(void)
{
	SimplImage *img = NULL;
	
	printf("\tTesting Color Image I/O.\n");
	
	assert(simpl_image_load(&img, "images/lena_interlaced.png")==SIMPL_OK);
	assert(simpl_image_save(img, "tmp/lena2.png")==SIMPL_OK);
	simpl_image_free(&img);
}


void test_gray_io(void)
{
	SimplGrayImage *img = NULL;

	printf("\tTesting Gray Image I/O.\n");

	assert(simpl_gray_load(&img, "images/lena_interlaced.png", COLOR_TO_GRAY_CIE, 0)==SIMPL_OK);
	assert(simpl_gray_save(img, "tmp/lena3.png")==SIMPL_OK);
	simpl_gray_free(&img);
}

#endif

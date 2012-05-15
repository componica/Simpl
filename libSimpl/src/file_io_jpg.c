#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>

#include <jpeg-8b/jpeglib.h>
#include <jpeg-8b/jerror.h>

#include "simpl.h"


typedef struct LibJpegErrorMgr {
	struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
} LibJpegErrorMgr;


#define JLIB_BUFFER_SIZE 4096
typedef struct LibJpegSourceMgr {
	struct jpeg_source_mgr pub;
	SimplInStream          istream;
	JOCTET                 buffer[JLIB_BUFFER_SIZE];
} LibJpegSourceMgr;


static void JpegLoadErrorExit(j_common_ptr cinfo)
{
	LibJpegErrorMgr * myerr = (LibJpegErrorMgr *) cinfo->err;
	
	/* Don't print the error. */
	/* (*cinfo->err->output_message) (cinfo); */
	
	longjmp(myerr->setjmp_buffer, 1);
}


static void InitSource(j_decompress_ptr cinfo)
{
	/* Does nothing. */
}


static boolean FillInputBuffer(j_decompress_ptr cinfo)
{
	LibJpegSourceMgr *src = (LibJpegSourceMgr *)cinfo->src;
	size_t nbytes = 0;
	
	nbytes = simpl_istream_read(src->istream, src->buffer, JLIB_BUFFER_SIZE);
	if (nbytes==0) {
		/* If the stream is finished, insert a fake EOI maker. */
		src->buffer[0] = (JOCTET) 0xFF;
		src->buffer[1] = (JOCTET) JPEG_EOI;
		nbytes = 2;
	}
	
	src->pub.next_input_byte = src->buffer;
	src->pub.bytes_in_buffer = nbytes;
	
	return TRUE;
}



static void SkipInputData(j_decompress_ptr cinfo, long num_bytes)
{
	LibJpegSourceMgr *src = (LibJpegSourceMgr *)cinfo->src;
	
	if(num_bytes <= (long) src->pub.bytes_in_buffer) {
		src->pub.bytes_in_buffer -= num_bytes;
		src->pub.next_input_byte += num_bytes;
	} else {
		num_bytes -= src->pub.bytes_in_buffer;
		src->pub.bytes_in_buffer = 0;
		simpl_istream_skip(src->istream, num_bytes);
	}
}


static void TermSource(j_decompress_ptr cinfo)
{
	/* Does nothing. */
}


static void JpegMyReader(j_decompress_ptr cinfo, SimplInStream istream)
{
	LibJpegSourceMgr *src;
	
	if (cinfo->src == NULL) { 
		cinfo->src = (struct jpeg_source_mgr *)(*cinfo->mem->alloc_small) 
			((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(LibJpegSourceMgr));
		src = (LibJpegSourceMgr *) cinfo->src;
	}
	
	src = (LibJpegSourceMgr *) cinfo->src;
	src->istream = istream;
	src->pub.init_source = InitSource;
	src->pub.fill_input_buffer = FillInputBuffer;
	src->pub.skip_input_data = SkipInputData;
	src->pub.resync_to_restart = jpeg_resync_to_restart;
	src->pub.term_source = TermSource;
	src->pub.bytes_in_buffer = 0;
	src->pub.next_input_byte = NULL;
}


int simpl_image_load_jpg(SimplImage **image,
                         SimplInStream istream)
{
	struct jpeg_decompress_struct cinfo;
	LibJpegErrorMgr jerr;
	
	JDIMENSION width, height, row_stride, i;
	JOCTET *row_data = NULL;
	
	int y, u, v;
	SimplColorPixel *ptr, *row_ptr;
	int out = SIMPL_INTERNAL;
	
	/* Allocate and initialize JPEG decompression object. */
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = JpegLoadErrorExit;
	
	/* Establish the setjmp return context for JpegLoadErrorExit to use. */
	if (setjmp(jerr.setjmp_buffer)) {
		goto error;
	}
	
	/* Now we can initialize the JPEG decompression object and file. */
	jpeg_create_decompress(&cinfo);
	JpegMyReader(&cinfo, istream);
	
	/* Read file parameters with jpeg_read_header(). */
	jpeg_read_header(&cinfo, TRUE);
	
	/* Start decompresser. */
	jpeg_start_decompress(&cinfo);
	
	/* Allocate the image and line buffer. */
	width  = cinfo.image_width;
	height = cinfo.image_height;
	row_stride = cinfo.output_width * cinfo.output_components;
	
	row_data = (JOCTET *)malloc(cinfo.image_width*4*sizeof(JOCTET));
	if (!row_data) {
		out = SIMPL_NOMEM;
		goto error;
	}
	
	out = simpl_image(image, width, height);
	if (out != SIMPL_OK) goto error;
	
	/* Extract the image. */
	switch (cinfo.out_color_space) {
	case JCS_GRAYSCALE:
		row_ptr = (*image)->image;
		while (cinfo.output_scanline < cinfo.output_height) {
			jpeg_read_scanlines(&cinfo, &row_data, 1);
			
			ptr = row_ptr;
			for (i=0; i<row_stride; i++) {
				ptr->red = ptr->green = ptr->blue = row_data[i];
				ptr++;
			}
			row_ptr += width;
		}
		break;
	
	case JCS_RGB:
		row_ptr = (*image)->image;
		while (cinfo.output_scanline < cinfo.output_height) {
			jpeg_read_scanlines(&cinfo, &row_data, 1);
			
			ptr = row_ptr;
			for (i=0; i<row_stride; i+=3) {
				ptr->red   = row_data[i];
				ptr->green = row_data[i+1];
				ptr->blue  = row_data[i+2];
				ptr++;
			}
			row_ptr += width;
		}
		break;
	
	case JCS_YCbCr:
		row_ptr = (*image)->image;
		while (cinfo.output_scanline < cinfo.output_height) {
			jpeg_read_scanlines(&cinfo, &row_data, 1);
			
			ptr = row_ptr;
			for (i=0; i<row_stride; i+=3) {
				y = row_data[i];
				u = row_data[i+1];
				v = row_data[i+2];
				ptr->red   = y + ((360*(v-128))>>8);
				ptr->green = y - ((88*(u-128)+183*(v-128))>>8);
				ptr->blue  = y + ((455 * (u-128))>>8);
				ptr++;
			}
			row_ptr += width;
		}
		break;
	
	case JCS_CMYK:
		row_ptr = (*image)->image;
		while (cinfo.output_scanline < cinfo.output_height) {
			jpeg_read_scanlines(&cinfo, &row_data, 1);
			
			ptr = row_ptr;
			for (i=0; i<row_stride; i+=4) {
				y = 255 - row_data[i+3];
				ptr->red   = y - ((row_data[i]*y)>>8);
				ptr->green = y - ((row_data[i+1]*y)>>8);
				ptr->blue  = y - ((row_data[i+2]*y)>>8);
				ptr++;
			}
			row_ptr += width;
		}
		break;
	
	default:
		simpl_image_clear(*image, simpl_color(0,0,0));
		break;
	}
	
error:
	if (row_data) free((void *)row_data);
	jpeg_destroy_decompress(&cinfo);
	return out;
}


int simpl_gray_load_jpg(SimplGrayImage **image,
                        SimplInStream istream,
                        const SimplColorToGrayMethods method,
                        const SimplPixel bk_value)
{
	struct jpeg_decompress_struct cinfo;
	LibJpegErrorMgr jerr;
	
	JDIMENSION width, height, row_stride, i;
	JOCTET *row_data = NULL;
	
	int y, u, v;
	SimplColorPixel *row=NULL, *row_ptr;
	SimplPixel *ptr;

	int out = SIMPL_INTERNAL;
	
	/* Allocate and initialize JPEG decompression object. */
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = JpegLoadErrorExit;
	
	/* Establish the setjmp return context for JpegLoadErrorExit to use. */
	if (setjmp(jerr.setjmp_buffer)) {
		goto error;
	}
	
	/* Now we can initialize the JPEG decompression object and file. */
	jpeg_create_decompress(&cinfo);
	JpegMyReader(&cinfo, istream);
	
	/* Read file parameters with jpeg_read_header(). */
	jpeg_read_header(&cinfo, TRUE);
	
	/* Start decompresser. */
	jpeg_start_decompress(&cinfo);
	
	/* Allocate the image and line buffer. */
	width  = cinfo.image_width;
	height = cinfo.image_height;
	row_stride = cinfo.output_width * cinfo.output_components;
	
	row_data = (JOCTET *)malloc(cinfo.image_width*4*sizeof(JOCTET));
	if (!row_data) {
		out = SIMPL_NOMEM;
		goto error;
	}

	row = (SimplColorPixel *)malloc(width * sizeof(SimplColorPixel));
	if (!row) {
		out = SIMPL_NOMEM;
		goto error;
	}

	out = simpl_gray(image, width, height);
	if (out != SIMPL_OK) goto error;
	ptr = (*image)->image;

	/* Extract the image. */
	if (cinfo.out_color_space==JCS_GRAYSCALE) {
		while (cinfo.output_scanline < cinfo.output_height) {
			jpeg_read_scanlines(&cinfo, &row_data, 1);
			for (i=0; i<row_stride; i++) ptr[i] = row_data[i];
			ptr += width;
		}
	} else if (cinfo.out_color_space==JCS_RGB || cinfo.out_color_space==JCS_YCbCr || cinfo.out_color_space==JCS_CMYK) {
		while (cinfo.output_scanline < cinfo.output_height) {
			jpeg_read_scanlines(&cinfo, &row_data, 1);
			row_ptr = row;
			
			if (cinfo.out_color_space==JCS_RGB) {
				for (i=0; i<row_stride; i+=3) {
					row_ptr->red   = row_data[i];
					row_ptr->green = row_data[i+1];
					row_ptr->blue  = row_data[i+2];
					++row_ptr;
				}
			} else if (cinfo.out_color_space==JCS_YCbCr) {
				for (i=0; i<row_stride; i+=3) {
					y = row_data[i];
					u = row_data[i+1];
					v = row_data[i+2];
					row_ptr->red   = y + ((360*(v-128))>>8);
					row_ptr->green = y - ((88*(u-128)+183*(v-128))>>8);
					row_ptr->blue  = y + ((455 * (u-128))>>8);
					++row_ptr;
				}
			} else {
				for (i=0; i<row_stride; i+=4) {
					y = 255 - row_data[i+3];
					row_ptr->red   = y - ((row_data[i]*y)>>8);
					row_ptr->green = y - ((row_data[i+1]*y)>>8);
					row_ptr->blue  = y - ((row_data[i+2]*y)>>8);
					++row_ptr;
				}
			}
			
			switch(method) {
			case COLOR_TO_GRAY_RED:
				for (i=0; i<width; ++i) ptr[i] = row[i].red;
				break;
				
			case COLOR_TO_GRAY_GREEN:
				for (i=0; i<width; ++i) ptr[i] = row[i].green;
				break;
				
			case COLOR_TO_GRAY_BLUE:
				for (i=0; i<width; ++i) ptr[i] = row[i].blue;
				break;
				
			case COLOR_TO_GRAY_ALPHA:
				for (i=0; i<width; ++i) ptr[i] = 255;
				break;
				
			case COLOR_TO_GRAY_CIE:
				for (i=0; i<width; ++i) {
					y = (6969*row[i].red + 23434*row[i].green + 2365*row[i].blue)>>15;
					ptr[i] = (y>255)?255:y;
				}
				break;
				
			case COLOR_TO_GRAY_MEAN:
			default:
				for (i=0; i<width; ++i) {
					y = (row[i].red + row[i].green + row[i].blue)/3;
					ptr[i] = (y>255)?255:y;
				}
				break;
			}
			
			ptr += width;
		}
	} else {
		simpl_gray_clear(*image, 0);
	}

error:
	if (row_data) free((void *)row_data);
	if (row) free((void *)row);
	jpeg_destroy_decompress(&cinfo);
	return out;
}


int simpl_image_save_jpg(SimplOutStream ostream,
                         const SimplImage *image,
                         const int quality)
{
	return SIMPL_OK;
}


int simpl_gray_save_jpg(SimplOutStream ostream,
                        const SimplGrayImage *image,
                        const int quality)
{
	return SIMPL_OK;
}

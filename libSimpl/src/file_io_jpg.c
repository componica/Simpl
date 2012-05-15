#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>

#include <jpeg-8b/jpeglib.h>
#include <jpeg-8b/jerror.h>

#include "simpl.h"

static void JpegLoadErrorExit(j_common_ptr cinfo)
{
	struct my_error_mgr {
		struct jpeg_error_mgr pub;
		jmp_buf setjmp_buffer;
	};
	
	struct my_error_mgr * myerr = (struct my_error_mgr *) cinfo->err;
	
	/* Don't print the error. */
	/* (*cinfo->err->output_message) (cinfo); */
	longjmp(myerr->setjmp_buffer, 1);
}


int simpl_image_load_jpg(SimplImage **image,
                         SimplInStream istream)
{
	return SIMPL_OK;
}


int simpl_gray_load_jpg(SimplGrayImage **image,
                        SimplInStream istream,
                        const SimplColorToGrayMethods method,
                        const SimplPixel bk_value)
{
	return SIMPL_OK;
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

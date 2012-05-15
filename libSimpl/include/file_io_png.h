/**
 * Header file for functions related to PNG encoding and decoding.
 * Implementation of these functions is in 'file_io_png.c'.
 */
#ifndef SIMPL_FILE_IO_PNG_H_
#define SIMPL_FILE_IO_PNG_H_


/**
 * Loads a PNG image from an input stream
 *
 * @param image A double pointer to an existing image or NULL.
 * @param istream A SimplInStream pointer.
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM,
 *         SIMPL_BAD_PARAMS, or SIMPL_INTERNAL.
 */
int simpl_image_load_png(SimplImage **image,
                         SimplInStream istream);


/**
 * Loads a PNG gray image from an input stream
 *
 * @param image A double pointer to an existing gray image or NULL.
 * @param istream A SimplInStream pointer.
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM,
 *         SIMPL_BAD_PARAMS, or SIMPL_INTERNAL.
 */
int simpl_gray_load_png(SimplGrayImage **image,
                        SimplInStream istream,
                        const SimplColorToGrayMethods method,
                        const SimplPixel bk_value);


/**
 * Saves an image to a PNG encoded output stream.
 *
 * @param ostream A SimplOutStream pointer.
 * @param image The image to be encoded. Value cannot be NULL.
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM,
 *         SIMPL_BAD_PARAMS, or SIMPL_INTERNAL.
 */
int simpl_image_save_png(SimplOutStream ostream,
                         const SimplImage *image);


/**
 * Saves a gray image to a PNG encoded output stream.
 *
 * @param ostream A SimplOutStream pointer.
 * @param image The gray image to be encoded. Value cannot be NULL.
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM,
 *         SIMPL_BAD_PARAMS, or SIMPL_INTERNAL.
 */
int simpl_gray_save_png(SimplOutStream ostream,
                        const SimplGrayImage *image);


#endif

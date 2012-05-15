/**
 * Header file for functions related to Jpeg encoding and decoding.
 * Implementation of these functions is in 'file_io_jpg.c'.
 */
#ifndef SIMPL_FILE_IO_JPG_H_
#define SIMPL_FILE_IO_JPG_H_


/**
 * Loads a JPEG image from an input stream
 *
 * @param image A double pointer to an existing image or NULL.
 * @param istream A SimplInStream pointer.
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM,
 *         SIMPL_BAD_PARAMS, or SIMPL_INTERNAL.
 */
int simpl_image_load_jpg(SimplImage **image,
                         SimplInStream istream);


/**
 * Loads a JPEG gray image from an input stream
 *
 * @param image A double pointer to an existing gray image or NULL.
 * @param istream A SimplInStream pointer.
 * @param method The method used to convert gray to color.
 * @param bk_value The background color in case of alpha channel.
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM,
 *         SIMPL_BAD_PARAMS, or SIMPL_INTERNAL.
 */
int simpl_gray_load_jpg(SimplGrayImage **image,
                        SimplInStream istream,
                        const SimplColorToGrayMethods method,
                        const SimplPixel bk_value);


/**
 * Saves an image to a JPEG encoded output stream.
 *
 * @param ostream A SimplOutStream pointer.
 * @param image The image to be encoded. Value cannot be NULL.
 * @param quality A quality factor from 0 to 100 to adjust compression.
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM,
 *         SIMPL_BAD_PARAMS, or SIMPL_INTERNAL.
 */
int simpl_image_save_jpg(SimplOutStream ostream,
                         const SimplImage *image,
                         const int quality);


/**
 * Saves a gray image to a JPEG encoded output stream.
 *
 * @param ostream A SimplOutStream pointer.
 * @param image The gray image to be encoded. Value cannot be NULL.
 * @param quality A quality factor from 0 to 100 to adjust compression.
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM,
 *         SIMPL_BAD_PARAMS, or SIMPL_INTERNAL.
 */
int simpl_gray_save_jpg(SimplOutStream ostream,
                        const SimplGrayImage *image,
                        const int quality);


#endif

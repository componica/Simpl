/**
 * Header file for functions related to storing, retrieving, encoding, and
 * decoding of images and gray images. Implementation of these functions are
 * in...
 */
#ifndef SIMPL_FILE_IO_H_
#define SIMPL_FILE_IO_H_


/**
 * Loads an image from a file automatically determining the file format.
 *
 * @param image A double pointer to an existing image or NULL.
 * @param filename The file path to the image file.
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM,
 *         SIMPL_BAD_PARAMS, or SIMPL_INTERNAL.
 */
int simpl_image_load(SimplImage **image,
                     const char *filename);


/**
 * Loads an image from a buffer automatically determing the file format.
 *
 * @param image A double pointer to an existing image or NULL.
 * @param data The buffer to decode.
 * @param length The size of the buffer.
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM,
 *         SIMPL_BAD_PARAMS, or SIMPL_INTERNAL.
 */
int simpl_image_load_buffer(SimplImage **image,
                            const uint8_t *data,
                            const size_t length);


/**
 * Saves an image to a file with file format determined by the dot extension
 * of the file name. Valid extensions are .png, .jpg, and .jpeg.
 *
 * @param image The image to be saved. Value cannot be NULL.
 * @param filename. The file path of the new image file.
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM,
 *         SIMPL_BAD_PARAMS, or SIMPL_INTERNAL.
 */
int simpl_image_save(const SimplImage *image,
                     const char *filename);


/**
 * Save an image to a buffer based on a given file format.
 *
 * @param data A double pointer to the buffer that the output is saved to.
 *             The pointer must eventually be freed.
 * @param length The returned length of the buffer.
 * @param image The image to be encoded. Value cannot be NULL.
 * @param format The encoding format of the output.
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM,
 *         SIMPL_BAD_PARAMS, or SIMPL_INTERNAL.
 */
int simpl_image_save_buffer(uint8_t **data,
                            size_t *length,
                            const SimplImage *image,
                            const SimplFileFormat format);


/**
 * Loads a gray image from a file automatically determining the file format.
 *
 * @param image A double pointer to an existing gray image or NULL.
 * @param filename The file path to the image file.
 * @param method The method used to convert gray to color.
 * @param bk_value The background color in case of alpha channel.
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM,
 *         SIMPL_BAD_PARAMS, or SIMPL_INTERNAL.
 */
int simpl_gray_load(SimplGrayImage **image,
                    const char *filename,
                    const SimplColorToGrayMethods method,
                    const SimplPixel bk_value);


/**
 * Loads a gray image from a buffer automatically determing the file format.
 *
 * @param image A double pointer to an existing gray image or NULL.
 * @param data The buffer to decode.
 * @param length The size of the buffer.
 * @param method The method used to convert gray to color.
 * @param bk_value The background color in case of alpha channel.
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM,
 *         SIMPL_BAD_PARAMS, or SIMPL_INTERNAL.
 */
int simpl_gray_load_buffer(SimplGrayImage **image,
                           const uint8_t *data,
                           const size_t length,
                           const SimplColorToGrayMethods method,
                           const SimplPixel bk_value);


/**
 * Saves a gray image to a file with file format determined by the dot
 * extension of the file name. Valid extensions are .png, .jpg, and .jpeg.
 *
 * @param image The gray image to be saved. Value cannot be NULL.
 * @param filename. The file path of the new image file.
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM,
 *         SIMPL_BAD_PARAMS, or SIMPL_INTERNAL.
 */
int simpl_gray_save(const SimplGrayImage *image,
                    const char *filename);


/**
 * Save a gray image to a buffer based on a given file format.
 *
 * @param data A double pointer to the buffer that the output is saved to.
 *             The pointer must eventually be freed.
 * @param length The returned length of the buffer.
 * @param image The gray image to be encoded. Value cannot be NULL.
 * @param format The encoding format of the output.
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM,
 *         SIMPL_BAD_PARAMS, or SIMPL_INTERNAL.
 */
int simpl_gray_save_buffer(uint8_t **data,
                           size_t *length,
                           const SimplGrayImage *image,
                           const SimplFileFormat format);


/**
 * Public function for unit testing.
 */
#ifndef NDEBUG
void test_image_io(void);
void test_gray_io(void);
#endif

#ifdef __cplusplus
}
#endif

#endif

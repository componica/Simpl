/**
 * Header file for functions related to the creation, deallocation,
 * copying, and converting of color images. Other function headers cover
 * the more complex operations. Implementation of these functions is in
 * "image.c".
 */
#ifndef SIMPL_IMAGE_H_
#define SIMPL_IMAGE_H_

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Returns a SimplColorPixel struct built from red, green, blue components.
 * Values outside 0 and 255 are pegged to 0 and 255.
 *
 * @param red A value from 0 to 255.
 * @param greed A value from 0 to 255.
 * @param blue A value from 0 to 255.
 *
 * @return A SimplColorPixel with RGB values assigned.
 */
SimplColorPixel simpl_color(const int red,
                            const int green,
                            const int blue);


/**
 * Allocates or reallocates an image of a given width and height. If image
 * points to an existing image, the pixel plane is reallocated to the
 * desired size or no operation if the original image's size matches the
 * desired size. If image points to NULL, a new image is allocated and
 * returned.
 *
 * @param image A double pointer to an existing image or NULL.
 * @param width The width of the image, cannot be 0.
 * @param height The height of the image, cannot be 0.
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM or
 *         SIMPL_BAD_PARAMS.
 */
int simpl_image(SimplImage **image,
                const size_t width,
                const size_t height);


/**
 * Deallocates an image and sets pointer to NULL.
 *
 * @param image A double pointer to an image or NULL.
 */
void simpl_image_free(SimplImage **image);


/**
 * Clears an image by setting all the pixels to bk_color.
 *
 * @param image The image to be cleared. Value cannot be NULL.
 * @param bk_color The color of the pixels to clear the image with.
 *
 * @return If successful, returns 0 (SIMPL_OK) or SIMPL_BAD_PARAMS if the
 *         image isn't valid.
 */
int simpl_image_clear(SimplImage *image,
                      const SimplColorPixel bk_color);


/**
 * Copies the source image to a destination image. If dest_img points to
 * NULL, the function allocates a new image and copies to that.
 *
 * @param dest_img A double pointer of image to copy to or NULL.
 * @param src_img The image to be copied.
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM or
 *         SIMPL_BAD_PARAMS.
 */
int simpl_image_copy(SimplImage **dest_img,
                     const SimplImage *src_img);


/**
 * Tests to see if the two images are the same.
 *
 * @param image1 An image to compare. Value cannot be NULL.
 * @param image2 An image to compare. Value cannot be NULL.
 *
 * @return 1 if the dimensions and pixel content are the same, else 0.
 */
int simpl_image_compare(const SimplImage *image1,
                        const SimplImage *image2);


/**
 * Crops a rectangular region from an image producing a new image. If
 * dest_img points to NULL, the function allocates a new image. Both dest_img
 * and src_img may point to the same image to destructively overwrite the
 * original.
 *
 * @param dest_img A double pointer of the image to crop to or NULL.
 * @param src_img The image to be cropped from and may be the dest_img.
 * @param rect The rectangular region to crop from. It's borders will be
 *             bounded to src_img.
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM or
 *         SIMPL_BAD_PARAMS.
 */
int simpl_image_crop(SimplImage **dest_img,
                     const SimplImage *src_img,
                     const SimplRect *rect);


/**
 * Similar to cropping, the function copies a patch defined by rect, but
 * pixels sampled outside the image are defined by SimplBoundaryMethods
 * and a background color and alpha.
 *
 * @param dest_img A double pointer of the image to copy to or NULL.
 * @param src_img The image to be copied from and may be the dest_img.
 * @param rect The rectangular region to copy from src_img.
 * @param boundary How pixels outside the image are sampled.
 * @param bk_color The background color if needed.
 * @param bk_alpha The background alpha if needed.
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM or
 *         SIMPL_BAD_PARAMS.
 */
int simpl_image_patch(SimplImage **dest_img,
                      const SimplImage *src_img,
                      const SimplRect *rect,
                      const SimplBoundaryMethods boundary,
                      const SimplColorPixel bk_color,
                      const SimplPixel bk_alpha);


/**
 * Flips an image horizontally and/or vertically. If dest_img points to NULL,
 * the function allocates a new image. Both dest_img and src_img may point to
 * the same image to overwrite the original.
 *
 * @param dest_img A double pointer of the image to flip or NULL.
 * @param src_img The image to be flipped and may be the dest_img.
 * @param flip_horizontal If not zero, flips horizontally.
 * @param flip_vertical If not zero, flips vertically.
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM or
 *         SIMPL_BAD_PARAMS.
 */
int simpl_image_flip(SimplImage **dest_img,
                     const SimplImage *src_img,
                     const int flip_horizontal,
                     const int flip_vertical);


/**
 * Rotates an image 90, 180, or 270 degrees. If dest_img points to NULL, the
 * function allocates a new image. Both dest_img and src_img may point to the
 * same image to overwrite the original.
 *
 * @param dest_img A double pointer of the image to rotate or NULL.
 * @param src_img The image to be rotated and may be the dest_img.
 * @param rotations The number of times to rotate by 90 (1, 2, or 3).
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM or
 *         SIMPL_BAD_PARAMS.
 */
int simpl_image_rotate_ninety(SimplImage **dest_img,
                              const SimplImage *src_img,
                              const int rotations);


/**
 * Converts an image to a gray image based on a specified method.
 *
 * @param gray_img A double pointer to the gray image or NULL.
 * @param image The image to be converted, cannot be NULL.
 * @param method The method used to convert color to gray.
 * @param bk_value The background color in case of alpha.
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM or
 *         SIMPL_BAD_PARAMS.
 */
int simpl_image_to_gray(SimplGrayImage **gray_img,
                        const SimplImage *image,
                        const SimplColorToGrayMethods method,
                        const SimplPixel bk_value);


/**
 * Adds an alpha channel to an image if it doesn't already have one. If the
 * function fails due to SIMPL_NOMEM, it doesn't destroy the original image
 * data.
 *
 * @param image The image to have an alpha channel added, cannot be NULL.
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM or
 *         SIMPL_BAD_PARAMS.
 */
int simpl_alpha_create(SimplImage *image);


/**
 * Removes the alpha channel from an image if it wasn't already removed.
 *
 * @param image The image to have the alpha channel removed, cannot be NULL.
 */
void simpl_alpha_remove(SimplImage *image);


/**
 * Public function for unit testing.
 */
#ifndef NDEBUG
void test_image(void);
#endif

#ifdef __cplusplus
}
#endif

#endif

/**
 * Header file for functions related to the creation, deallocation,
 * copying, and converting of gray images. Other function headers cover
 * the more complex operations. Implementation of these functions is in
 * "gray.c".
 */
#ifndef SIMPL_GRAY_H_
#define SIMPL_GRAY_H_

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Allocates or reallocates a gray image of a given width and height. If the
 * image points to an existing image, the pixel plane is reallocated to the
 * desired size or no operation if the original image's size matches the
 * desired size. If image points to NULL, a new gray image is allocated and
 * returned.
 *
 * @param image A double pointer to an existing gray image or NULL.
 * @param width The width of the gray image, cannot be 0.
 * @param height The height of the gray image, cannot be 0.
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM or
 *         SIMPL_BAD_PARAMS.
 */
int simpl_gray(SimplGrayImage **image,
               const size_t width,
               const size_t height);


/**
 * Deallocates an image and sets pointer to NULL.
 *
 * @param image A double pointer to an image or NULL.
 */
void simpl_gray_free(SimplGrayImage **image);


/**
 * Clears a gray image by setting all the pixels to bk_value.
 *
 * @param image The image to be cleared. Value cannot be NULL.
 * @param bk_value The color of the pixels to clear the image with.
 *
 * @return If successful, returns 0 (SIMPL_OK) or SIMPL_BAD_PARAMS if the
 *         image isn't valid.
 */
int simpl_gray_clear(SimplGrayImage *image,
                     const SimplPixel bk_value);


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
int simpl_gray_copy(SimplGrayImage **dest_img,
                    const SimplGrayImage *src_img);


/**
 * Tests to see if the two gray images are the same.
 *
 * @param image1 An image to compare. Value cannot be NULL.
 * @param image2 An image to compare. Value cannot be NULL.
 *
 * @return 1 if the dimensions and pixel content are the same, else 0.
 */
int simpl_gray_compare(const SimplGrayImage *image1,
                       const SimplGrayImage *image2);


/**
 * Crops a rectangular region from a gray image producing a new image. If
 * dest_img points to NULL, the function allocates a new gray image. Both
 * dest_img and src_img may point to the same image to destructively
 * overwrite the original.
 *
 * @param dest_img A double pointer of the gray image to crop to or NULL.
 * @param src_img The gray image to be cropped from and may be the dest_img.
 * @param rect The rectangular region to crop from. It's borders will be
 *             bounded to src_img.
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM or
 *         SIMPL_BAD_PARAMS.
 */
int simpl_gray_crop(SimplGrayImage **dest_img,
                    const SimplGrayImage *src_img,
                    const SimplRect *rect);


/**
 * Similar to cropping, the function copies a patch defined by rect, but
 * pixels sampled outside the image are defined by SimplBoundaryMethods
 * and a background value.
 *
 * @param dest_img A double pointer of the image to copy to or NULL.
 * @param src_img The image to be copied from and may be the dest_img.
 * @param rect The rectangular region to copy from src_img.
 * @param boundary How pixels outside the image are sampled.
 * @param bk_value The background value if needed.
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM or
 *         SIMPL_BAD_PARAMS.
 */
int simpl_gray_patch(SimplGrayImage **dest_img,
                     const SimplGrayImage *src_img,
                     const SimplRect *rect,
                     const SimplBoundaryMethods boundary,
                     const SimplColorPixel bk_value);


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
int simpl_gray_flip(SimplGrayImage **dest_img,
                    const SimplGrayImage *src_img,
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
int simpl_gray_rotate_ninety(SimplGrayImage **dest_img,
                             const SimplGrayImage *src_img,
                             const int rotations);


/**
 * Copies a gray image to a color image based on a specified method.
 *
 * @param image A double pointer to the image or NULL.
 * @param gray_img The gray image to be copied, cannot be NULL.
 * @param method The method used to convert gray to color.
 *
 * @return If successful, returns 0 (SIMPL_OK) else SIMPL_NOMEM or
 *         SIMPL_BAD_PARAMS.
 */
int simpl_gray_to_image(SimplImage **image,
                        const SimplGrayImage *gray_img,
                        const SimplGrayToColorMethods method);


/**
 * Public function for unit testing.
 */
#ifndef NDEBUG
void test_gray(void);
#endif

#ifdef __cplusplus
}
#endif

#endif

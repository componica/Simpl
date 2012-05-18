/**
 * Header file for functions related to rectangle operations. Implementation
 * of these functions is in "rect.c".
 */
#ifndef SIMPL_RECT_H_
#define SIMPL_RECT_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Macro that initializes a rect to an empty nonsensical size.
 *
 * @param rect The rectangle to initialize.
 */
#define simpl_rect_INIT(rect) do { (rect).min_x = (rect).min_y = 0; (rect).max_x = (rect).max_y = -1;} while(0)


 /**
  * Macro that initializes a rect to a width and height with the min_x
  * and min_y at 0, 0.
  *
  * @param rect The rectangle to initialize.
  * @param width The new width of the rectangle.
  * @param height The new height of the rectangle.
  */
#define simpl_rect_RESIZE(rect, width, height) do { (rect).min_x = (rect).min_y = 0; (rect).max_x = (width)-1; (rect).max_y = (height)-1;} while(0)


/**
 * Macro that returns the width of the rectangle.
 *
 * @param rect The rectangle.
 *
 * @return The width of the rectangle.
 */
#define simpl_rect_WIDTH(rect) ((rect).max_x - (rect).min_x + 1)


/**
 * Macro that returns the height of the rectangle.
 *
 * @param rect The rectangle.
 *
 * @return The height of the rectangle.
 */
#define simpl_rect_HEIGHT(rect) ((rect).max_y - (rect).min_y + 1)


/**
 * Macro that returns the size of the rectangle.
 *
 * @param rect The rectangle.
 *
 * @return The size of the rectangle.
 */
#define simpl_rect_SIZE(rect) (((rect).max_x-(rect).min_x+1) * ((rect).max_y-(rect).min_y+1))


/**
 * Macro that's true if an x, y falls within the bounds of the rectangle.
 *
 * @param rect The rectangle to test.
 * @param x The x coordinate.
 * @param y The y coordinate.
 *
 * @return truthy if point in rectangle.
 */
#define simpl_rect_PT_TEST(rect, x, y) ((x)>=(rect).min_x && (x)<=(rect).max_x && (y)>=(rect).min_y && (y)<=(rect).max_y)


/**
 * Macro that's true if a rectangle is empty (no width or height).
 *
 * @param rect The rectangle to test.
 *
 * @return truthy if rectangle is empty.
 */
#define simpl_rect_EMPTY(rect) ((rect).min_x>(rect).max_x || (rect).min_y>(rect).max_y)


/**
 * Macro that's true if the two rectangles are the same.
 *
 * @param rect1 A rectangle to compare.
 * @param rect2 A rectangle to compare.
 *
 * @return truthy if the dimensions are the same.
 */
#define simpl_rect_COMPARE(rect1, rect2) ((rect1).min_x==(rect2).min_x && (rect1).min_y==(rect2).min_y && (rect1).max_x==(rect2).max_x && (rect1).max_y==(rect2).max_y)


/**
 * Initializes a rect to an empty nonsensical size.
 *
 * @param rect The rectangle to initialize.
 */
void simpl_rect_init(SimplRect *rect);


/**
 * Initializes a rect to a width and height with the min_x and min_y at 0, 0.
 *
 * @param rect The rectangle to initialize.
 * @param width The new width of the rectangle.
 * @param height The new height of the rectangle.
 */
void simpl_rect_resize(SimplRect *rect, int width, int height);


/**
 * Returns the width of the rectangle.
 *
 * @return The width of the rectangle.
 */
int simpl_rect_width(const SimplRect *rect);


/**
 * Returns the height of the rectangle.
 *
 * @return The height of the rectangle.
 */
int simpl_rect_height(const SimplRect *rect);


/**
 * Returns the size of the rectangle.
 *
 * @return The size of the rectangle.
 */
int simpl_rect_size(const SimplRect *rect);


/**
 * Copies a rectangle to a new rectangle.
 *
 * @param dest_rect The destination rectangle.
 * @param src_rect The source rectangle.
 */
void simpl_rect_copy(SimplRect *dest_rect, const SimplRect *src_rect);


/**
 * Returns 1 if an x, y falls within the bounds of the rectangle.
 *
 * @param rect The rectangle to test.
 * @param x The x coordinate.
 * @param y The y coordinate.
 *
 * @return 1 if point inside rectangle, 0 otherwise.
 */
int simpl_rect_pt_test(const SimplRect *rect, const int x, const int y);


/**
 * Tests if a rectangle is empty.
 *
 * @param rect The rectangle to test.
 *
 * @return 1 if empty, 0 otherwise
 */
int simpl_rect_empty(const SimplRect *rect);


/**
 * Tests to see if the two rectangles are the same.
 *
 * @param rect1 A rectangle to compare.
 * @param rect2 A rectangle to compare.
 *
 * @return 1 if the dimensions are the same, else 0.
 */
int simpl_rect_compare(const SimplRect *rect1, const SimplRect *rect2);


/**
 * Expands the rectangle to contain the x, y point.
 *
 * @param rect The rectangle to expand.
 * @param x The x coordinate.
 * @param y The y coordinate.
 */
void simpl_rect_contain(SimplRect *rect, const int x, const int y);


/**
 * Expands the rectangle to contain a different rectangle.
 *
 * @param dest_rect The destination rectangle to expand
 * @param src_rect The source rectangle.
 */
void simpl_rect_contain_rect(SimplRect *dest_rect, const SimplRect *src_rect);


/**
 * Copies a rectangle centering it on a given point. Both dest_rect and
 * src_rect may point to the same rectangle.
 *
 * @param dest_rect The destination rectangle to be of the same width and
 *                  height as src_rect but centered on x/y.
 * @param src_rect The source rectangle.
 * @param x The x coordinate.
 * @param y The y coordinate.
 */
void simpl_rect_center_on(SimplRect *dest_rect, const SimplRect *src_rect, const int x, const int y);


/**
 * Copies a rectangle that fits within the bounds of 0/0 and width/height so
 * that it doesn't fall outside an image. Returns 0 if rectangle is empty
 * because it completely is outside of the boundary or width or height is
 * zero, otherwise 1. Both dest_rect and src_rect may point to the same
 * rectangle.
 *
 * @param dest_rect The destination rectangle to constraint inside 0/0 and
 *                  width/height.
 * @param src_rect The source rectangle.
 * @param width The exclusive upper bounds for x, [0,width).
 * @param height The exclusive upper bounds for y, [0,height).
 */
int simpl_rect_boundary(SimplRect *dest_rect, const SimplRect *src_rect, size_t width, size_t height);


/**
 * Copies a rectangle and adds an x/y offset. Both dest_rect and src_rect may
 * point to the same rectangle.
 *
 * @param dest_rect The destination rectangle to be offset.
 * @param src_rect The source rectangle.
 * @param x The x offset.
 * @param y The y offset.
 */
void simpl_rect_offset(SimplRect *dest_rect, const SimplRect *src_rect, int x, int y);


/**
 * Public function for unit testing.
 */
#ifndef NDEBUG
void test_rectangle(void);
#endif

#ifdef __cplusplus
}
#endif

#endif

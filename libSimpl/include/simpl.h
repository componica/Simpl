#ifndef SIMPL_H_
#define SIMPL_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


/** Windows-style integers. */
#ifdef WIN32
typedef __int8 int8_t;
typedef unsigned __int8 uint8_t;
typedef __int16 int16_t;
typedef unsigned __int16 uint16_t;
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
#include <stdint.h>
#endif


/** Constants that should exist. */
#ifndef M_PI
#define M_PI  3.14159265358979323846264338327950288 
#endif

#ifndef M_2PI
#define M_2PI 6.283185307179586476925286766559
#endif

#ifndef M_SQRT2
#define M_SQRT2 1.4142135623730950488016887
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


/** Result codes from functions. */
#define SIMPL_OK         0 /* Successful result */
#define SIMPL_NOMEM      1 /* A malloc() failed. */
#define SIMPL_BAD_PARAMS 2 /* Provided parameters where invalid. */
#define SIMPL_INTERNAL   3 /* Function failed due internal computations. */


/** The possible types of supported image file formats. */
typedef enum SimplFileFormat {
    IMAGE_NONE, /* Either bad image or not specified flag. */
    IMAGE_GIF,
    IMAGE_BMP,
    IMAGE_JPG,
    IMAGE_PNG
} SimplFileFormat;


/** The possible ways of converting a gray image to an image. */
typedef enum SimplGrayToColorMethods {
    GRAY_TO_ALL=0,        /* Just load the value to red, green, and blue. */
    GRAY_TO_COLOR_RED,    /* Just load the red channel. */
    GRAY_TO_COLOR_GREEN,  /* The green channel. */
    GRAY_TO_COLOR_BLUE,   /* The blue channel. */
    GRAY_TO_COLOR_ALPHA,  /* Loads the alpha. */
} SimplGrayToColorMethods;


/** The possible ways of converting an image to a gray image. */
typedef enum SimplColorToGrayMethods {
    COLOR_TO_GRAY_MEAN=0, /* Just compute the average value. */
    COLOR_TO_GRAY_RED,    /* Just load the red channel. */
    COLOR_TO_GRAY_GREEN,  /* The green channel. */
    COLOR_TO_GRAY_BLUE,   /* The blue channel. */
    COLOR_TO_GRAY_ALPHA,  /* Loads the alpha or black if not present. */
    COLOR_TO_GRAY_CIE     /* The CIE method of converting color to gray scale. */
} SimplColorToGrayMethods;


/** The possible ways to continuously sample pixels. */
typedef enum SimplSamplingMethods {
    SAMPLE_NEAREST_NEIGHBOR,
    SAMPLE_BILINEAR,
    SAMPLE_HIGH_ORDER
} SimplSamplingMethods;


/** Specifies how to handle boundaries of images. */
typedef enum SimplBoundaryMethods {
    BOUNDARY_USE_BKGND,   /* Use a background color */
    BOUNDARY_STRETCH,     /* The border pixels are infinitely replicated. */
    BOUNDARY_WRAP,        /* Wrap around like a torus. */
    BOUNDARY_MIRROR       /* Mirror the border. */
} SimplBoundaryMethods;


/** Definition of pixel and color pixel values. */
typedef uint8_t SimplPixel;

typedef struct SimplColorPixel {
    SimplPixel blue;
    SimplPixel green;
    SimplPixel red;
} SimplColorPixel;


/** Definition of a colored image. */
typedef struct SimplImage {
    SimplColorPixel *image;
    SimplPixel      *alpha;
    
    size_t           width;
    size_t           height;
} SimplImage;


/** Definition of a gray-level or binary image. */
typedef struct SimplGrayImage {
    SimplPixel      *image;
    
    size_t           width;
    size_t           height;
} SimplGrayImage;


/** Definition of an integer 2D point. */
typedef struct SimplPoint {
    int x, y;
} SimplPoint;


/** Definition of a rectangle. */
typedef struct SimplRect {
    int min_x, min_y, max_x, max_y;
} SimplRect;


/**
 * Definition of the stream opaque pointer. The full definition is found in
 * "stream.c".
 */
typedef struct SimplStream_t * SimplInStream;
typedef struct SimplStream_t * SimplOutStream;


#include "gray.h"
#include "file_io.h"
#include "file_io_jpg.h"
#include "file_io_png.h"
#include "image.h"
#include "rect.h"
#include "stream.h"


/** Public function for unit testing. */
#ifndef NDEBUG
void unit_test(void);
#endif

#ifdef __cplusplus
}
#endif

#endif

#include "simpl.h"

#ifndef NDEBUG

#include <stdio.h>
#include <assert.h>

static void test_constants_and_sizes(void)
{
    /* Test the sizings of platform independent integer types. */
    int8_t   a;
    uint8_t  b;
    int16_t  c;
    uint16_t d;
    int32_t  e;
    uint32_t f;
    int64_t  g;
    uint64_t h;
    
    printf("\tTesting Constants and Sizes.\n");
    
    /* Asserting the byte sizes of integer types. */
    assert(sizeof(int8_t)   == 1);
    assert(sizeof(uint8_t)  == 1);
    assert(sizeof(int16_t)  == 2);
    assert(sizeof(uint16_t) == 2);
    assert(sizeof(int32_t)  == 4);
    assert(sizeof(uint32_t) == 4);
    assert(sizeof(int64_t)  == 8);
    assert(sizeof(uint64_t) == 8);
    
    /* Asserting the signess and unsigness of integer types. */
    a = 0; a--; assert(a<0);
    b = 0; b--; assert(b>0);
    c = 0; c--; assert(c<0);
    d = 0; d--; assert(d>0);
    e = 0; e--; assert(e<0);
    f = 0; f--; assert(f>0);
    g = 0; g--; assert(g<0);
    h = 0; h--; assert(h>0);
    
    /* Asserting TRUE and FALSE work. */
    assert(TRUE);
    assert(!FALSE);
}


void unit_test(void)
{
    test_constants_and_sizes();
    test_rectangle();
    test_image();
    test_gray();
    test_streams();
    test_image_io();
    test_gray_io();
}

#endif

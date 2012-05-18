#include "simpl.h"


void simpl_rect_init(SimplRect *rect)
{
    rect->min_x = rect->min_y = 0;
    rect->max_x = rect->max_y = -1;
}


void simpl_rect_resize(SimplRect *rect, int width, int height)
{
    rect->min_x = rect->min_y = 0;
    rect->max_x = width-1;
    rect->max_y = height-1;
}


int simpl_rect_width(const SimplRect *rect)
{
    return rect->max_x-rect->min_x+1;
}


int simpl_rect_height(const SimplRect *rect)
{
    return rect->max_y-rect->min_y+1;
}


int simpl_rect_size(const SimplRect *rect)
{
    return (rect->max_x-rect->min_x+1) * (rect->max_y-rect->min_y+1);
}


void simpl_rect_copy(SimplRect *dest_rect, const SimplRect *src_rect)
{
    dest_rect->min_x = src_rect->min_x;
    dest_rect->min_y = src_rect->min_y;
    dest_rect->max_x = src_rect->max_x;
    dest_rect->max_y = src_rect->max_y;
}


int simpl_rect_pt_test(const SimplRect *rect, const int x, const int y)
{
    return (x>=rect->min_x && x<=rect->max_x && y>=rect->min_y && y<=rect->max_y) ? TRUE : FALSE;
}


int simpl_rect_empty(const SimplRect *rect)
{
    return (rect->min_x>rect->max_x || rect->min_y>rect->max_y) ? TRUE : FALSE;
}


int simpl_rect_compare(const SimplRect *rect1, const SimplRect *rect2)
{
    return (rect1->min_x==rect2->min_x &&
            rect1->min_y==rect2->min_y &&
            rect1->max_x==rect2->max_x &&
            rect1->max_y==rect2->max_y) ? TRUE : FALSE;
}


void simpl_rect_contain(SimplRect *rect, const int x, const int y)
{
    if (rect->min_x<=rect->max_x) {
        if (x<rect->min_x) rect->min_x=x;
        if (x>rect->max_x) rect->max_x=x;
    } else {
        rect->min_x = rect->max_x = x;
    }
    
    if (rect->min_y<=rect->max_y) {
        if (y<rect->min_y) rect->min_y=y;
        if (y>rect->max_y) rect->max_y=y;
    } else {
        rect->min_y = rect->max_y = y;
    }
}


void simpl_rect_contain_rect(SimplRect *dest_rect, const SimplRect *src_rect)
{
    if (dest_rect->min_x<=dest_rect->max_x) {
        if (src_rect->min_x<dest_rect->min_x) dest_rect->min_x=src_rect->min_x;
        if (src_rect->max_x>dest_rect->max_x) dest_rect->max_x=src_rect->max_x;
    } else {
        dest_rect->min_x = src_rect->min_x;
        dest_rect->max_x = src_rect->max_x;
    }
    
    if (dest_rect->min_y<=dest_rect->max_y) {
        if (src_rect->min_y<dest_rect->min_y) dest_rect->min_y=src_rect->min_y;
        if (src_rect->max_y>dest_rect->max_y) dest_rect->max_y=src_rect->max_y;
    } else {
        dest_rect->min_y = src_rect->min_y;
        dest_rect->max_y = src_rect->max_y;
    }
}


void simpl_rect_center_on(SimplRect *dest_rect, const SimplRect *src_rect, const int x, const int y)
{
    int length;
    
    length = src_rect->max_x - src_rect->min_x + 1;
    dest_rect->min_x = x - (length-1) / 2;
    dest_rect->max_x = length + dest_rect->min_x - 1;

    length = src_rect->max_y - src_rect->min_y + 1;
    dest_rect->min_y = y - (length-1) / 2;
    dest_rect->max_y = length + dest_rect->min_y - 1;
}


int simpl_rect_boundary(SimplRect *dest_rect, const SimplRect *src_rect, size_t width, size_t height)
{
    if (width>0 && height>0) {
        if (src_rect->min_x<(int)width && src_rect->max_x>=0 && src_rect->min_y<(int)height && src_rect->max_y>=0) {
            dest_rect->min_x = (src_rect->min_x<0) ? 0 : src_rect->min_x;
            dest_rect->max_x = (src_rect->max_x>=(int)width) ? (int)(width-1) : src_rect->max_x;
            dest_rect->min_y = (src_rect->min_y<0) ? 0 : src_rect->min_y;
            dest_rect->max_y = (src_rect->max_y>=(int)height) ? (int)(height-1) : src_rect->max_y;
            
            return TRUE;
        }
    }
    
    simpl_rect_INIT(*dest_rect);
    
    return FALSE;
}


void simpl_rect_offset(SimplRect *dest_rect, const SimplRect *src_rect, int x, int y)
{
    dest_rect->min_x = src_rect->min_x + x;
    dest_rect->max_x = src_rect->max_x + x;
    dest_rect->min_y = src_rect->min_y + y;
    dest_rect->max_y = src_rect->max_y + y;
}


#ifndef NDEBUG

#include <assert.h>
#include <stdio.h>

void test_rectangle(void)
{
    SimplRect r1, r2, r3;
    
    printf("\tTesting Rectangles.\n");
    
    /* First test the functions. */
    simpl_rect_init(&r1);
    simpl_rect_resize(&r2, 5, 9);
    simpl_rect_resize(&r3, 2, 3);
    
    assert(simpl_rect_width(&r1)==0);
    assert(simpl_rect_height(&r1)==0);
    assert(simpl_rect_size(&r1)==0);
    assert(simpl_rect_pt_test(&r1, 0, 0)==FALSE);
    assert(simpl_rect_empty(&r1)==TRUE);
    
    assert(simpl_rect_width(&r2)==5);
    assert(simpl_rect_height(&r2)==9);
    assert(simpl_rect_size(&r2)==45);
    assert(simpl_rect_pt_test(&r2, -1, 0)==FALSE);
    assert(simpl_rect_pt_test(&r2, 0, 0)==TRUE);
    assert(simpl_rect_pt_test(&r2, 4, 8)==TRUE);
    assert(simpl_rect_pt_test(&r2, 3, 4)==TRUE);
    assert(simpl_rect_empty(&r2)==FALSE);
    
    assert(simpl_rect_width(&r3)==2);
    assert(simpl_rect_height(&r3)==3);
    assert(simpl_rect_size(&r3)==6);
    assert(simpl_rect_empty(&r3)==FALSE);
    
    simpl_rect_contain_rect(&r1, &r2);
    simpl_rect_contain_rect(&r1, &r3);
    assert(simpl_rect_width(&r1)==5);
    assert(simpl_rect_height(&r1)==9);
    assert(simpl_rect_size(&r1)==45);
    assert(simpl_rect_empty(&r1)==FALSE);
    simpl_rect_contain(&r1, -3, -6);
    assert(simpl_rect_width(&r1)==8);
    assert(simpl_rect_height(&r1)==15);
    
    simpl_rect_copy(&r1, &r2);
    assert(simpl_rect_width(&r2)==5);
    assert(simpl_rect_height(&r2)==9);
    assert(simpl_rect_size(&r2)==45);
    assert(simpl_rect_compare(&r1, &r2)==TRUE);
    assert(simpl_rect_compare(&r1, &r3)==FALSE);
    
    simpl_rect_resize(&r1, 3, 5);
    simpl_rect_center_on(&r2, &r1, 1, 2);
    assert(simpl_rect_width(&r2)==3 && simpl_rect_height(&r2)==5);
    assert(r2.min_x==0 && r2.min_y==0);
    simpl_rect_center_on(&r2, &r2, 0, 0);
    assert(simpl_rect_width(&r2)==3 && simpl_rect_height(&r2)==5);
    assert(r2.min_x==-1 && r2.min_y==-2);
    
    simpl_rect_resize(&r1, 3, 5);
    simpl_rect_offset(&r2, &r1, 4, 3);
    assert(simpl_rect_width(&r1)==3 && simpl_rect_width(&r2)==3);
    assert(simpl_rect_height(&r1)==5 && simpl_rect_height(&r2)==5);
    assert(r1.min_x==0 && r1.min_y==0 && r2.min_x==4 && r2.min_y==3);
    simpl_rect_offset(&r2, &r2, -7, -8);
    assert(simpl_rect_width(&r2)==3 && simpl_rect_height(&r2)==5);
    
    assert(simpl_rect_boundary(&r2, &r2, 10, 12)==FALSE);
    simpl_rect_offset(&r2, &r1, 11, 0);
    assert(simpl_rect_boundary(&r2, &r2, 10, 12)==FALSE);
    simpl_rect_offset(&r2, &r1, 0, -11);
    assert(simpl_rect_boundary(&r2, &r2, 10, 12)==FALSE);
    simpl_rect_offset(&r2, &r1, 0, 13);
    assert(simpl_rect_boundary(&r2, &r2, 10, 12)==FALSE);
    
    assert(simpl_rect_boundary(&r2, &r1, 10, 12)==TRUE && simpl_rect_width(&r2)==3 && simpl_rect_height(&r2)==5);
    simpl_rect_offset(&r2, &r1, 7, 7);
    assert(simpl_rect_boundary(&r2, &r2, 10, 12)==TRUE && simpl_rect_width(&r2)==3 && simpl_rect_height(&r2)==5);
    simpl_rect_offset(&r2, &r1, 9, 9);
    assert(simpl_rect_boundary(&r2, &r2, 10, 12)==TRUE && simpl_rect_width(&r2)==1 && simpl_rect_height(&r2)==3);
    
    /* Next test the macros. */
    simpl_rect_INIT(r1);
    simpl_rect_RESIZE(r2, 5, 9);
    simpl_rect_RESIZE(r3, 2, 3);
    
    assert(simpl_rect_WIDTH(r1)==0);
    assert(simpl_rect_HEIGHT(r1)==0);
    assert(simpl_rect_SIZE(r1)==0);
    assert(!simpl_rect_PT_TEST(r1, 0, 0));
    assert(simpl_rect_EMPTY(r1));
    
    assert(simpl_rect_WIDTH(r2)==5);
    assert(simpl_rect_HEIGHT(r2)==9);
    assert(simpl_rect_SIZE(r2)==45);
    assert(!simpl_rect_PT_TEST(r2, -1, 0));
    assert(simpl_rect_PT_TEST(r2, 0, 0));
    assert(simpl_rect_PT_TEST(r2, 4, 8));
    assert(simpl_rect_PT_TEST(r2, 3, 4));
    assert(!simpl_rect_EMPTY(r2));
    
    assert(simpl_rect_WIDTH(r3)==2);
    assert(simpl_rect_HEIGHT(r3)==3);
    assert(simpl_rect_SIZE(r3)==6);
    assert(!simpl_rect_EMPTY(r3));
    
    simpl_rect_contain_rect(&r1, &r2);
    simpl_rect_contain_rect(&r1, &r3);
    assert(simpl_rect_WIDTH(r1)==5);
    assert(simpl_rect_HEIGHT(r1)==9);
    assert(simpl_rect_SIZE(r1)==45);
    assert(!simpl_rect_EMPTY(r1));
    simpl_rect_contain(&r1, -3, -6);
    assert(simpl_rect_WIDTH(r1)==8);
    assert(simpl_rect_HEIGHT(r1)==15);
    
    simpl_rect_copy(&r1, &r2);
    assert(simpl_rect_WIDTH(r2)==5);
    assert(simpl_rect_HEIGHT(r2)==9);
    assert(simpl_rect_SIZE(r2)==45);
    assert(simpl_rect_COMPARE(r1, r2));
    assert(!simpl_rect_COMPARE(r1, r3));
}

#endif

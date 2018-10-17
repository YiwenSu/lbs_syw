#ifndef LBS_BITMAP_H_INCLUDED
#define LBS_BITMAP_H_INCLUDED

#include <stdint.h>
#include <stdlib.h>

typedef struct lbs_bitmap_s {
	uint8_t *bits;
	uint32_t bits_num;

}lbs_bitmap_t;

int lbs_bitmap_init(lbs_bitmap_t*lbs_bitmap, uint32_t bits_num);
int lbs_bitmap_destory(lbs_bitmap_t*lbs_bitmap);
int lbs_bitmap_setbit(lbs_bitmap_t*lbs_bitmap, uint32_t pos);
int lbs_bitmap_isset(lbs_bitmap_t*lbs_bitmap, uint32_t pos);

#endif // LBS_BITMAP_H_INCLUDED

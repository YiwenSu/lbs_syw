#include "lbs_bitmap.h"
#include <stdio.h>
#include <stdlib.h>

int lbs_bitmap_init(lbs_bitmap_t*lbs_bitmap, uint32_t bits_num) {// ³õÊ¼»¯bitmap
	lbs_bitmap->bits_num = bits_num;
	lbs_bitmap->bits = (uint8_t*)malloc(sizeof(uint8_t) * (bits_num / 8 + 1));
	for (uint32_t i = 0; i < (bits_num / 8 + 1); i++) {
		lbs_bitmap->bits[i] = 0;

	}
	return 1;


}

int lbs_bitmap_destory(lbs_bitmap_t*lbs_bitmap) {//É¾³ýbitmap
	free(lbs_bitmap);
return 0;
}

int lbs_bitmap_setbit(lbs_bitmap_t*lbs_bitmap, uint32_t pos) {// ÉèÖÃbitmap
	int index = pos/8;
	int offset = pos % 8;
	int a[] = { 0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80 };
	lbs_bitmap->bits[index] = lbs_bitmap->bits[index] |a[offset] ;
	return 1;

}

int lbs_bitmap_isset(lbs_bitmap_t*lbs_bitmap, uint32_t pos) {// ÅÐ¶ÏÊÇ·ñÉèÖÃbitmap
	int index = pos / 8;
    int offset = pos % 8;
	int a[] = { 0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80 };
	int answer = lbs_bitmap->bits[index] & a[offset];
	if (answer > 0)
		return 0;
	else
		return 1;

}

#ifndef SERVER_GRID_LBS_BITMAP_H_
#define SERVER_GRID_LBS_BITMAP_H_
#include<stdint.h>
#include<stdlib.h>
#include "server/grid/lbs_grid.h"

typedef struct lbs_bitmap_s{
	unit8_t *bits;
	unit32_t bits_num;
	 
}lbs_bitmap_t;

int lbs_bitmap_init(lbs_bitmap_t* lbs_bitmap, unit32_t bits_num) {
	lbs_bitmap->bits_num = bits_num;
	lbs_bitmap->bits= (unit8_t*)malloc(sizeof(unit8_t) * (bits_num/8+1));
    for(int i=0; i<(bits_num/8+1); i++){
        lbs_bitmap->bits[i] = 0;
    }
    return 1;
}
int lbs_bitmap_setbit(lbs_bitmap_t* lbs_bitmap,unit32_t pos);
{
	int idx = pos/8;
	int offset = pos%8;
	
	int a = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
	lbs_bitmap->bits[idx] = lbs_bitmap->bits[idx] | a[offset];
	return 1;
}
//求出具体的网格位置
//求余数获得bit的位置
//与8位的数组进行或运算 
int lbs_bitmap_isset(lbs_bitmap_t* lbs_bitmap,unit32_t pos);
int idx = pos/8;
	int offset = pos%8;
	
	int a = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
	int answer= lbs_bitmap->bits[idx] & a[offset];
    if(answer > 0)
        return 1;
    else
        return 0;
#endif







#ifndef SERVER_GRID_LBS_NN_HEAP_H_ 
#define SERVER_GRID_LBS_NN_HEAP_H_
#include

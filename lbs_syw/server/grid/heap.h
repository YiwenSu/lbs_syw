#ifndef HEAP_H_INCLUDED
#define HEAP_H_INCLUDED
#include "server/grid/lbs_defs.h"

typedef struct lbs_heapnode_s
{
double distance;//距离
uint8_t is_grid;//1:网格 0:移动对象
int cell_id;//cell id
lbs_mov_node_t * node;
}lbs_heapnode_t;

typedef struct lbs_nnheap_s
{
    uint32_t capacity;
    uint32_t size;
    lbs_heapnode_t * heap_nodes;
}lbs_nnheap_t;


int lbs_nnheap_init(lbs_nnheap_t* lbs_nnheap);
int lbs_nnheap_destroy(lbs_nnheap_t*lbs_nnheap);
int lbs_nnheap_insert(lbs_nnheap_t* lbs_nnheap, lbs_mov_node_t* lbs_mov_node,int cell_id, uint8_t is_grid, double distance);
lbs_heapnode_t* lbs_nnheap_top(lbs_nnheap_t*lbs_nnheap);
void lbs_nnheap_pop(lbs_nnheap_t* lbs_nnheap);


#endif // HEAP_H_INCLUDED

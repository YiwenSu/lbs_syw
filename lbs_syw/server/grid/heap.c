#include "heap.h"
#include <stdlib.h>
#include<algorithm>
int lbs_nnheap_init(lbs_nnheap_t* lbs_nnheap)
{
    lbs_nnheap->capacity = 1000;
    lbs_nnheap->size = 0;
    lbs_nnheap->heap_nodes = (lbs_heapnode_t*)malloc((lbs_nnheap->capacity)*sizeof(lbs_heapnode_t));
    return 0;
}

int lbs_nnheap_destroy(lbs_nnheap_t*lbs_nnheap)
{
    free(lbs_nnheap->heap_nodes);
}

int lbs_nnheap_insert(lbs_nnheap_t* lbs_nnheap, lbs_mov_node_t* lbs_mov_node,int cell_id, uint8_t is_grid, double distance)
{
     if (lbs_nnheap->size == lbs_nnheap->capacity)
    {
        lbs_nnheap->capacity = 3*lbs_nnheap->capacity;
        lbs_nnheap->heap_nodes = (lbs_heapnode_t*)realloc(lbs_nnheap->heap_nodes, sizeof(lbs_nnheap_t)*lbs_nnheap->capacity);
    }

    int current=lbs_nnheap->size;

    lbs_nnheap->size++;
    lbs_nnheap->heap_nodes[current].distance = distance;
    lbs_nnheap->heap_nodes[current].is_grid = is_grid;
    lbs_nnheap->heap_nodes[current].cell_id = cell_id;
    lbs_nnheap->heap_nodes[current].node = lbs_mov_node;


    int son = lbs_nnheap->size;

    while(son/2>=1)
    {
        int parent = son/2;


        if(lbs_nnheap->heap_nodes[parent-1].distance > lbs_nnheap->heap_nodes[son-1].distance)
        {

            lbs_heapnode_t node = lbs_nnheap->heap_nodes[parent-1];
            lbs_nnheap->heap_nodes[parent-1] = lbs_nnheap->heap_nodes[son-1];
            lbs_nnheap->heap_nodes[son-1] = node;
        }
        else
            break;

         son = parent;
    }

    return 0;
}

lbs_heapnode_t* lbs_nnheap_top(lbs_nnheap_t*lbs_nnheap)
{
    if(lbs_nnheap->heap_nodes)
     return  lbs_nnheap->heap_nodes;
    else
     return NULL;
}


void lbs_nnheap_pop(lbs_nnheap_t* lbs_nnheap)
{
    /**/int parent = 0;
    int min,rchild,lchild;

    lbs_nnheap->heap_nodes[0] = lbs_nnheap->heap_nodes[lbs_nnheap->size-1];
    lbs_nnheap->size=lbs_nnheap->size-1;

    while(2*parent+1< lbs_nnheap->size)
    {
        lchild = 2*parent+1;
        rchild = 2*parent+2;

        min = lchild;

        if(rchild<lbs_nnheap->size)
           if(lbs_nnheap->heap_nodes[lchild].distance>lbs_nnheap->heap_nodes[rchild].distance)
                min=rchild;


       // if(lchild<lbs_nnheap->size)
        //    if(lbs_nnheap->heap_nodes[lchild].distance<lbs_nnheap->heap_nodes[rchild].distance)
        //        min=lchild;

        if(lbs_nnheap->heap_nodes[parent].distance>lbs_nnheap->heap_nodes[min].distance)
        {
            lbs_heapnode_t temp = lbs_nnheap->heap_nodes[parent];
            lbs_nnheap->heap_nodes[parent] = lbs_nnheap->heap_nodes[min];
            lbs_nnheap->heap_nodes[min] = temp;
        }
        else
            break;
        parent = min;
    }
}

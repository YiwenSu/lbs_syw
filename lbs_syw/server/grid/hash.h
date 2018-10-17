#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED
#include "server/grid/lbs_defs.h"
#include <stdlib.h>


typedef struct lbs_hashnode_s
{
     lbs_queue_t queue;
     lbs_mov_node_t*mov_node;
     int cell_id;
}lbs_hashnode_t;

typedef struct lbs_hashtable_s
{
    int size;
    int capacity;
    lbs_hashnode_t*hash_nodes;

}lbs_hashtable_t;

int lbs_hashtable_init(lbs_hashtable_t* lbs_hashtable);
int lbs_hashtable_set(lbs_hashtable_t* lbs_hashtable, uint32_t id, lbs_mov_node_t* lbs_mov_node, int cell_id);
lbs_hashnode_t* lbs_hashtable_get(lbs_hashtable_t* lbs_hash_table, uint32_t id);
#endif // HASH_H_INCLUDED

#ifndef GRID_H_INCLUDED
#define GRID_H_INCLUDED
#include "hash.h"
typedef struct lbs_cell_s
{
    lbs_mov_node_t dammy_node;


}lbs_cell_t;

typedef struct lbs_grid_s
{
    int row_num;
    int col_num;
    double cell_width;
    double cell_height;
    double lon_min;
    double lat_min;
    lbs_hashtable_t hash_table;
    lbs_cell_t   *cell;
}lbs_grid_t;

int lbs_grid_init(lbs_grid_t*lbs_grid,double lon1,double lon2,double lat1, double lat2, int rownum, int colnum);
int lbs_grid_destroy(lbs_grid_t*lbs_grid);
int lbs_grid_update(lbs_grid_t*lbs_grid,double lon,double lat, uint64_t timestamp, uint32_t id);
int lbs_grid_cell_row(lbs_grid_t*lbs_grid, double lat);
int lbs_grid_cell_col(lbs_grid_t*lbs_grid, double lon);
int lbs_grid_cell_id(lbs_grid_t*lbs_grid, int cell_col, int cell_row);
void lbs_grid_cell_row_col(lbs_grid_t*lbs_grid, int cell_id, int*cell_row, int*cell_col);
lbs_cell_t* lbs_grid_cell(lbs_grid_t* lbs_grid, int cell_id);

#endif // GRID_H_INCLUDED

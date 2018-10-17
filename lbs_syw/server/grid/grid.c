#include "grid.h"
#include "server/grid/lbs_defs.h"

int lbs_grid_init(lbs_grid_t*lbs_grid,double lon1,double lon2,double lat1, double lat2, int rownum, int colnum)
{
    lbs_grid->row_num=rownum;
    lbs_grid->col_num=colnum;
    lbs_grid->cell_width=(lon2-lon1)/colnum;
    lbs_grid->cell_height=(lat2-lat1)/rownum;
    lbs_grid->lon_min=lon1;
    lbs_grid->lat_min=lat1;
    lbs_hashtable_init(&lbs_grid->hash_table);

    lbs_grid->cell=(lbs_cell_t*)malloc((rownum*colnum)*sizeof(lbs_cell_t));
    for(int i=0;i<rownum*colnum;i++)
    {
        lbs_queue_init(&(lbs_grid->cell[i].dammy_node.queue));

    }
return 0;

}

int lbs_grid_destroy(lbs_grid_t*lbs_grid)
{

    free(lbs_grid->cell);
return 0;
}

int lbs_grid_update(lbs_grid_t*lbs_grid,double lon,double lat, uint64_t timestamp, uint32_t id)
{
     int row=lbs_grid_cell_row(lbs_grid,lat);
     int col=lbs_grid_cell_col(lbs_grid,lon);
     int cellid=lbs_grid_cell_id(lbs_grid,col,row);


     lbs_hashnode_t* hashnodes;

     hashnodes=lbs_hashtable_get(&(lbs_grid->hash_table),id);

     if(!hashnodes)
     {
         lbs_mov_node_t*cellnodes;
         cellnodes=(lbs_mov_node_t*)malloc(sizeof(lbs_mov_node_t));
         cellnodes->lon=lon;
         cellnodes->lat=lat;
         cellnodes->id=id;
         cellnodes->timestamp=timestamp;
         lbs_queue_insert_head(&(lbs_grid->cell[cellid].dammy_node.queue),&(cellnodes->queue));
         lbs_hashtable_set(&(lbs_grid->hash_table),id,cellnodes,cellid);
         return 1;
     }
    else
    {
        hashnodes->mov_node->lat=lat;
        hashnodes->mov_node->lon=lon;
        hashnodes->mov_node->timestamp=timestamp;

        if(cellid==hashnodes->cell_id)
            return 0;
        else
           {hashnodes->cell_id=cellid;
            lbs_queue_remove(&(hashnodes->mov_node->queue));
            lbs_queue_insert_head(&(lbs_grid->cell[cellid].dammy_node.queue),&(hashnodes->mov_node->queue));
            return 0;
           }
    }

}

int lbs_grid_cell_row(lbs_grid_t*lbs_grid, double lat)
{
    int t;
    t=(int)((lat-lbs_grid->lat_min)/lbs_grid->cell_height);
    return t;
}

int lbs_grid_cell_col(lbs_grid_t*lbs_grid, double lon)
{
    int t;
    t=(int)((lon-lbs_grid->lon_min)/lbs_grid->cell_width);
    return t;
}

/**/int lbs_grid_cell_id(lbs_grid_t*lbs_grid, int cell_col, int cell_row)
{
    int t;
    t=(cell_row)*lbs_grid->col_num+cell_col;
    return t;
}

void lbs_grid_cell_row_col(lbs_grid_t*lbs_grid, int cell_id, int *cell_row, int*cell_col)
{

    *cell_row=(cell_id/lbs_grid->col_num);

    *cell_col=cell_id-(*cell_row)*lbs_grid->col_num;

}

lbs_cell_t* lbs_grid_cell(lbs_grid_t* lbs_grid, int cell_id)
{
   return lbs_grid->cell+cell_id;
}

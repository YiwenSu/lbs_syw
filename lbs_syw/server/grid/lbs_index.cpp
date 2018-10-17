#include "server/grid/lbs_index.h"

#include <stdio.h>
#include <stdlib.h>

#include "common/log.h"

#include "server/grid/lbs_distance.h"
#include "server/grid/grid.h"
#include "server/grid/lbs_bitmap.h"
#include  "server/grid/heap.h"

#define LBS_LON_MIN 116
#define LBS_LON_MAX 117
#define LBS_LAT_MIN 39
#define LBS_LAT_MAX 41

#define LBS_ROW_NUM 200
#define LBS_COL_NUM 100

lbs_grid_t lbs_grid;


// 初始化网格索引
int lbs_grid_index_init()
{
 lbs_grid_init(&lbs_grid, LBS_LON_MIN,LBS_LON_MAX,LBS_LAT_MIN,  LBS_LAT_MAX, LBS_ROW_NUM,LBS_COL_NUM );
 return 0;
}

// 更新接口[出租车位置更新]
/**/int lbs_grid_index_update(double lon,double lat,uint64_t timestamp,uint32_t id)
{
  lbs_grid_update(&lbs_grid,lon,lat,timestamp,id);
  return 0;
}

// 范围查询接口[查询某一范围内的所有出租车信息]
/**/int lbs_grid_index_range_query(double lon1,double lon2,double lat1,double lat2, lbs_res_node_t* out)
{
  int row1=lbs_grid_cell_row(&lbs_grid, lat1);
  int row2=lbs_grid_cell_row(&lbs_grid, lat2);

  int col1=lbs_grid_cell_col(&lbs_grid,lon1);
  int col2=lbs_grid_cell_col(&lbs_grid,lon2);

 // printf ("row=%d %d col=%d %d\n", row1, row2, col1, col2);

  int row,col;
  int cellid;

  for(row=row1;row<=row2;row++)
    for(col=col1;col<=col2;col++)
      {
        cellid=lbs_grid_cell_id(&lbs_grid, col, row);

        lbs_mov_node_t* head=&lbs_grid.cell[cellid].dammy_node;
        lbs_mov_node_t* current=(lbs_mov_node_t*)head->queue.next;

        while(current!=head)
        {
            if(current->lat<lat2&&current->lat>lat1
                && current->lon<lon2&&current->lon > lon1)
            {
                 lbs_res_node_t* q=(lbs_res_node_t*)malloc(sizeof(lbs_res_node_t));

                 q->id=current->id;
                 q->lat=current->lat;
                 q->lon=current->lon;
                 q->timestamp=current->timestamp;

                 lbs_queue_insert_head(&(out->queue), &(q->queue));

            }
            current = (lbs_mov_node_t*)current->queue.next;

        }
 }


  return 0;
}

// NN查询接口[查询离lon,lat最近的出租车]
int lbs_grid_index_nn_query(double lon, double lat, lbs_res_node_t* out)//lon经度，lat纬度
{
  int row,col;
  lbs_nnheap_t lbs_nnheap;
  lbs_bitmap_t lbs_bitmap;

  row=lbs_grid_cell_row(&lbs_grid, lat);
  col=lbs_grid_cell_col(&lbs_grid,lon);

  int cellid = lbs_grid_cell_id(&lbs_grid,col,row);

  lbs_nnheap_init(&lbs_nnheap);

  lbs_nnheap_insert(&lbs_nnheap,&lbs_grid.cell[cellid].dammy_node,cellid,1, 0);

  lbs_heapnode_t* top= lbs_nnheap_top(&lbs_nnheap);

  lbs_bitmap_init(&lbs_bitmap, LBS_ROW_NUM*LBS_COL_NUM);

  while(top->is_grid==1)
  {
      lbs_nnheap_pop(&lbs_nnheap);

      int top_cell_id = top->cell_id;
      lbs_mov_node_t* head=&lbs_grid.cell[top_cell_id].dammy_node;
      lbs_mov_node_t* current=(lbs_mov_node_t*)head->queue.next;

       while(current!=head)
        {
            double distance1=lbs_distance(lon, lat, current->lon , current->lat);
            lbs_nnheap_insert(&lbs_nnheap, current,top_cell_id,0, distance1);

            current = (lbs_mov_node_t*)current->queue.next;
        }

      int cellid0=top_cell_id;

      int arr[8]={-1,-1,-1,-1,-1,-1,-1,-1};//store cellid

      if(cellid0 % LBS_COL_NUM != 0 && cellid0 + LBS_COL_NUM < LBS_ROW_NUM*LBS_COL_NUM)//left up
          arr[0]=cellid0 + LBS_COL_NUM-1;

      if(cellid0 + LBS_COL_NUM < LBS_ROW_NUM*LBS_COL_NUM)//up
          arr[1]=cellid0 + LBS_COL_NUM;

      if(cellid0 + LBS_COL_NUM < LBS_ROW_NUM*LBS_COL_NUM && cellid0 % LBS_COL_NUM != LBS_COL_NUM - 1)//right up
          arr[2]=cellid0 + LBS_COL_NUM+1;

      if(cellid0 % LBS_COL_NUM!=0)//left
          arr[3]=cellid0-1;

      if(cellid0 % LBS_COL_NUM!=LBS_COL_NUM-1)//right
           arr[4]=cellid0+1;

      if(cellid0 % LBS_COL_NUM!=0 && cellid0 -LBS_COL_NUM>=0)//left down
           arr[5]=cellid0 - LBS_COL_NUM -1;

      if(cellid0-LBS_COL_NUM>=0)//down
           arr[6]= cellid0 - LBS_COL_NUM;

      if(cellid0 - LBS_COL_NUM>=0 && cellid0 % LBS_COL_NUM!= LBS_COL_NUM - 1)//right down
            arr[7]= cellid0 - LBS_COL_NUM + 1;

      for(int i=0;i<8;i++)
      {
          if(arr[i] != -1)
             if(lbs_bitmap_isset(&lbs_bitmap,arr[i])==1)
             {
                 double distance=getdistance(arr,i,lon,lat);//计算距离

                 lbs_nnheap_insert(&lbs_nnheap,&lbs_grid.cell[arr[i]].dammy_node ,arr[i],1, distance);//插入
                 lbs_bitmap_setbit(&lbs_bitmap, arr[i]);//bitmap插入
             }
      }

      for(int i=0;i<8;i++)//回归-1
        arr[i]=-1;


      top=(lbs_heapnode_t*)lbs_nnheap_top(&lbs_nnheap);

  }//top->isgrid==0

  lbs_res_node_t* q=(lbs_res_node_t*)malloc(sizeof(lbs_res_node_t));

  q->id=top->node->id;
  q->lat=top->node->lat;
  q->lon=top->node->lon;
  q->timestamp=top->node->timestamp;

  lbs_queue_insert_head(&(out->queue), &(q->queue));

  return 0;
}


double getdistance(int *arr, int i,int lon, int lat)
{
    int row, col;
    lbs_grid_cell_row_col(&lbs_grid, arr[i], &row, &col);

    //car
    int row1=lbs_grid_cell_row(&lbs_grid, lat);
    int col1=lbs_grid_cell_col(&lbs_grid,lon);

    int lon2,lat2;

    if(row1<row&&col1>col)
    {
        lat2=row*(lbs_grid.cell_height)+LBS_LAT_MIN;
        lon2=(col+1)*(lbs_grid.cell_width)+LBS_LON_MIN;
    }
    if(row1<row&&col1==col)
    {
        lat2=row*(lbs_grid.cell_height)+LBS_LAT_MIN;
        lon2=lon+LBS_LON_MIN;
    }
    if(row1>row&&col1<col)
    {
        lat2=row*(lbs_grid.cell_height)+LBS_LAT_MIN;
        lon2=col*(lbs_grid.cell_width)+LBS_LON_MIN;
    }
    if(row1==row && col1>col)
    {
        lat2=lat+LBS_LAT_MIN;
        lon2=(col+1)*(lbs_grid.cell_width)+LBS_LON_MIN;
    }
    if(row1==row && col1<col)
    {
        lat2=lat+LBS_LAT_MIN;
        lon2=col*(lbs_grid.cell_width)+LBS_LON_MIN;
    }
    if(row1>row && col1 > col)
    {
        lat2=(row+1)*lbs_grid.cell_height+LBS_LAT_MIN;
        lon2=(col+1)*lbs_grid.cell_width+LBS_LON_MIN;
    }
    if(row1 > row && col1==col)
    {
        lat2=(row+1)*lbs_grid.cell_height + LBS_LAT_MIN;
        lon2=lon + LBS_LON_MIN;
    }
    if(row1 > row && col1 < col )
    {
        lat2=(row+1)*lbs_grid.cell_height+LBS_LAT_MIN;
        lon2=col*lbs_grid.cell_width + LBS_LON_MIN;
    }

    return lbs_distance(lon, lat, lon2, lat2);
}









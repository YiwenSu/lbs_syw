#include "server/grid/hash.h"

int lbs_hashtable_init(lbs_hashtable_t* lbs_hashtable)
{
	lbs_hashtable->capacity = 10000;
	lbs_hashtable->hash_nodes = (lbs_hashnode_t *)malloc(lbs_hashtable->capacity * sizeof(lbs_hashnode_t));

	for(int i=0; i < lbs_hashtable->capacity; i++)
	{
		lbs_hashnode_t *head = lbs_hashtable->hash_nodes + i;
		lbs_queue_init(&(head->queue));
	}

	return 1;
}

int lbs_hashtable_set(lbs_hashtable_t* lbs_hashtable, uint32_t id, lbs_mov_node_t* lbs_mov_node, int cell_id)
{
	//����queue��positionĤ��
	int pos = id % lbs_hashtable->capacity;//�ҵ�ӳ��λ��
	lbs_hashnode_t *head = lbs_hashtable->hash_nodes + pos;   //��posλ�õ�ͷ���

	lbs_hashnode_t* newnode = (lbs_hashnode_t *)malloc(sizeof(lbs_hashnode_t));//����ڵ�ռ�
	newnode->cell_id = cell_id;//�ڵ㸳ֵ
	newnode->mov_node = lbs_mov_node; //ͬ��
	lbs_queue_insert_head(&(head->queue), &(newnode->queue));//���뵽ͷ�ڵ����

	return 1;
}

lbs_hashnode_t* lbs_hashtable_get(lbs_hashtable_t* lbs_hash_table, uint32_t id)
{
	//�������� ������cellid
	int pos = id % lbs_hash_table->capacity;//�ҵ�ӳ��λ��
	lbs_hashnode_t *head = lbs_hash_table->hash_nodes + pos;   //��posλ�õ�ͷ���

	lbs_hashnode_t *cur = (lbs_hashnode_t *)(head->queue.next);
	while(cur != head)
	{
		if(cur->mov_node->id == id)
		{
			return cur;
		}
		cur = (lbs_hashnode_t *)(cur->queue.next);
	}

	return NULL;
}

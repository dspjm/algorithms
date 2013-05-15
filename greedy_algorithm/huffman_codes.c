/* huffman_codes.c
 *
 * Author: Jimmy Pan
 * Email: dspjmt@gmail.com
 * Date: Wed May 15 14:01:05 CST 2013
 * 
 * Copyright (C) 2013 Jimmy Pan
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 *
 * Algorithm
 * Input:
 * Output:
 * Functions:
 *
 */

#include <stdio.h>
#include "algorithms.h"

struct hc_node {
	int key;
	int freq;
	struct hc_node *p;
	struct hc_node *l;
	struct hc_node *r;
};

struct hc_node *alloc_init_hc_node(int key, int freq)
{
	struct hc_node *tmp;
	tmp = malloc(sizeof *tmp);
	memset(tmp, 0, sizeof *tmp);
	tmp->key = key;
	tmp->freq = freq;
	return tmp;
}

struct hc_heap_node {
	int key;
	struct hc_node *hcn;
	struct hc_heap_node *p;
	struct hc_heap_node *l;
	struct hc_heap_node *r;
};

struct hc_heap_node *alloc_init_hc_heap_node(struct hc_node *hcn)
{
	struct hc_heap_node *tmp;
	tmp = malloc(sizeof *tmp);
	memset(tmp, 0, sizeof *tmp);
	tmp->key = hcn->freq;
	tmp->hcn = hcn;
	return tmp;
}

struct avail_queue_node {
	struct hc_heap_node *hn;
	struct avail_queue_node *prev;
	struct avail_queue_node *next;
};

struct avail_queue_node *alloc_init_avail_queue_node(struct hc_heap_node *hn)
{
	struct avail_queue_node *tmp;
	tmp = malloc(sizeof *tmp);
	memset(tmp, 0, sizeof *tmp);
	tmp->hn = hn;
	return tmp;
}

struct hc_heap {
	struct avail_queue_node head;
	struct hc_heap_node *root;
};

struct hc_heap *alloc_init_hc_heap()
{
	struct hc_heap *tmp;
	tmp = malloc(sizeof *tmp);
	memset(tmp, 0, sizeof *tmp);
	tmp->prev = &tmp->head;
	tmp->next = &tmp->head;
	return tmp;
};

void hc_heap_node_swap_content(struct hc_heap_node *a, struct hc_heap_node *b)
{
	int tmp;
	struct hc_node *tmp1;
	tmp = a->key;
	tmp1 = a->hcn;
	a->key = b->key;
	a->hcn = b->hcn;
	b->key = tmp;
	b->hcn = tmp1;
}

void hc_min_heapify(struct hc_heap_node *hn)
{
	struct hc_heap_node *l, *r, *max;
	l = hn->l;
	r = hn->r;
	min = hn;
	if (l && hn->key > l->key)
		min = l;
	if (r && max->key > r->key)
		min = r;
	if (max != hn) {
		hc_heap_node_swap_content(max, hn);
		hc_min_heapify(max);
	}
}

static void hc_enqueue_availqueue(struct hc_avail_queue_node *head,
  struct hc_heap_node *hn)
{
	struct hc_avail_queue_node *tmp;
	tmp = alloc_init_hc_heap_node(hn);
	head->prev->next = tmp;
	tmp->prev = head->prev;
	tmp->next = head;
	head->prev = tmp;
}

static void hc_dequeue_availqueue(struct hc_avail_queue_node *head)
{
	struct hc_avail_queue_node *tmp;
	tmp = head->prev;
	head->prev = tmp->prev;
	tmp->prev->next = head;
	free(tmp);
}

static int hc_insert_heap(struct hc_heap *h, struct hc_node *hcn)
{
	struct hc_heap_node *tmp, *p;
	tmp = alloc_init_hc_heap_node(hcn);
	if (!tmp)
		return -1;
	if (!h->root) {
		h->root = tmp;
	} else {
		if (!h->head.next->l) {
			tmp->p = h->head.next;
			h->head.next->l = tmp;
		} else if (!h->head.next->r) {
			tmp->p = h->head.next;
			h->head.next->r = tmp;
			hc_dequeue_availqueue(&h->head);
		} else
			return -1;
	}
	hc_enqueue_availqueue(&h->head, tmp);
	while (tmp->p && tmp->p->key > tmp->key) {
		hc_heap_node_swap_content(tmp, tmp->p);
		tmp = tmp->p;
	}
	return 0;
}


int main(int argc, char **argv)
{
	return 0;
}

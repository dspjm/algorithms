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
#include <string.h>
#include "algorithms.h"

#define KEY_NUM 26
#define FREQ_MAX 30
#define NON_LEAF_KEY 0

struct hc_node {
	int key;
	int freq;
	struct hc_node *p;
	struct hc_node *l;
	struct hc_node *r;
	struct hc_node *n;
};

static struct hc_node *alloc_init_hc_node(int key, int freq)
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

static struct hc_heap_node *alloc_init_hc_heap_node(struct hc_node *hcn)
{
	struct hc_heap_node *tmp;
	tmp = malloc(sizeof *tmp);
	memset(tmp, 0, sizeof *tmp);
	tmp->key = hcn->freq;
	tmp->hcn = hcn;
	return tmp;
}

static void hc_heap_node_swap_content(struct hc_heap_node *a, struct hc_heap_node *b)
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

struct hc_avail_queue_node {
	struct hc_heap_node *hn;
	struct hc_avail_queue_node *prev;
	struct hc_avail_queue_node *next;
};

static struct hc_avail_queue_node *alloc_init_hc_avail_queue_node(struct hc_heap_node *hn)
{
	struct hc_avail_queue_node *tmp;
	tmp = malloc(sizeof *tmp);
	memset(tmp, 0, sizeof *tmp);
	tmp->hn = hn;
	return tmp;
}

static void hc_enqueue_availqueue(struct hc_avail_queue_node *head,
  struct hc_heap_node *hn)
{
	struct hc_avail_queue_node *tmp;
	tmp = alloc_init_hc_avail_queue_node(hn);
	head->prev->next = tmp;
	tmp->prev = head->prev;
	tmp->next = head;
	head->prev = tmp;
}

static struct hc_avail_queue_node *hc_dequeue_availqueue(struct hc_avail_queue_node *head)
{
	struct hc_avail_queue_node *tmp;
	tmp = head->next;
	head->next = tmp->next;
	tmp->next->prev = head;
	return tmp;
}

static struct hc_avail_queue_node *hc_pop_availqueue(struct hc_avail_queue_node *head)
{
	struct hc_avail_queue_node *tmp;
	tmp = head->prev;
	head->prev = tmp->prev;
	tmp->prev->next = head;
	return tmp;
}

static struct hc_avail_queue_node *hc_add_to_head_availqueue(struct hc_avail_queue_node *head, struct hc_heap_node *hn)
{
	struct hc_avail_queue_node *tmp;
	tmp = alloc_init_hc_avail_queue_node(hn);
	tmp->next = head->next;
	tmp->prev = head;
	tmp->next->prev = tmp;
	head->next = tmp;
	return tmp;
}

struct hc_heap {
	struct hc_avail_queue_node head;
	struct hc_heap_node *root;
	struct hc_node *first;
};

static struct hc_heap *alloc_init_hc_heap()
{
	struct hc_heap *tmp;
	tmp = malloc(sizeof *tmp);
	memset(tmp, 0, sizeof *tmp);
	tmp->head.prev = &tmp->head;
	tmp->head.next = &tmp->head;
	return tmp;
}

static void hc_min_heapify(struct hc_heap_node *hn)
{
	struct hc_heap_node *l, *r, *min;
	l = hn->l;
	r = hn->r;
	min = hn;
	if (l && hn->key > l->key)
		min = l;
	if (r && min->key > r->key)
		min = r;
	if (min != hn) {
		hc_heap_node_swap_content(min, hn);
		hc_min_heapify(min);
	}
}

static int hc_insert_heap(struct hc_heap *h, struct hc_heap_node *hn)
{
	struct hc_heap_node *p;
	if (!hn)
		return -1;
	if (!h->root) {
		h->root = hn;
	} else {
		p = h->head.next->hn;
		if (!p->l) {
			hn->p = p;
			p->l = hn;
		} else if (!p->r) {
			hn->p = p;
			p->r = hn;
			free(hc_dequeue_availqueue(&h->head));
		} else
			return -1;
	}
	hc_enqueue_availqueue(&h->head, hn);
	while (hn->p && hn->p->key > hn->key) {
		hc_heap_node_swap_content(hn, hn->p);
		hn = hn->p;
	}
	return 0;
}

static struct hc_heap_node *hc_extract_min_heap(struct hc_heap *h)
{
	struct hc_avail_queue_node *last_q;
	struct hc_heap_node *root, *last;
	if (!h->root) {
		fprintf(stderr, "heap underflow\n");
		return NULL;
	}
	root = h->root;
	last_q = hc_pop_availqueue(&h->head);
	last = last_q->hn;
	if (!last->p) {
		h->root = NULL;
		free(last_q);
		return last;
	}
	if (last == last->p->r) {
		hc_add_to_head_availqueue(&h->head, last->p);
		last->p->r = NULL;
	} else
		last->p->l = NULL;
	hc_heap_node_swap_content(root, last);
	hc_min_heapify(root);
	free(last_q);
	return last;
}


struct hc_heap *hc_build_heap(int *keys, int *freqs, int n)
{
	int i;
	struct hc_node *hcn, tmp, *prev_hcn;
	struct hc_heap *heap;
	struct hc_heap_node *hn;
	heap = alloc_init_hc_heap();
	prev_hcn = &tmp;
	for (i = 0; i < n; i++) {
		hcn = alloc_init_hc_node(keys[i], freqs[i]);
		hn = alloc_init_hc_heap_node(hcn);
		hc_insert_heap(heap, hn);
		prev_hcn->n = hcn;
		prev_hcn = hcn;
	}
	heap->first = tmp.n;
	return heap;
}

struct hc_node *hc_build_haffman_tree_from_heap(struct hc_heap *h)
{
	struct hc_heap_node *nhn, *hn1, *hn2;
	struct hc_node *nhcn, *hcn1, *hcn2;
	hn1 = hc_extract_min_heap(h);
	hn2 = hc_extract_min_heap(h);
	while (hn1 && hn2) {
		hcn1 = hn1->hcn;
		hcn2 = hn2->hcn;
		nhcn = alloc_init_hc_node(NON_LEAF_KEY, hcn1->freq + hcn2->freq);
		hcn1->p = nhcn;
		hcn2->p = nhcn;
		nhcn->l = hcn1;
		nhcn->r = hcn2;
		nhn = alloc_init_hc_heap_node(nhcn);
		hc_insert_heap(h, nhn);
		free(hn1);
		free(hn2);
		hn1 = hc_extract_min_heap(h);
		hn2 = hc_extract_min_heap(h);
	}
	nhcn = hn1->hcn;
	free(hn1);
	return nhcn;
}

void hc_print_haffman_code(struct hc_node *first, int n)
{
	struct hc_node *hcn, *tmp, *tmpp;
	int code[n], cnt, i;
	if (!first)
		return;
	hcn = first;
	while (hcn) {
		printf("key: %c, freq: %i, code: ", hcn->key, hcn->freq);
		tmp = hcn;
		tmpp = tmp->p;
		cnt = 0;
		while(tmpp) {
			if (tmp == tmpp->l)
				code[cnt++] = 0;
			else
				code[cnt++] = 1;
			tmp = tmpp;
			tmpp = tmp->p;
		}
		for ( i = cnt - 1; i >= 0; i--)
			printf("%i", code[i]);
		puts("");
		hcn = hcn->n;
	}
}

int main(int argc, char **argv)
{
	int i;
	int keys[KEY_NUM], freqs[KEY_NUM];
	struct hc_node *hcn;
	struct hc_heap *heap;
	struct hc_heap_node *hn;
	for (i = 0; i < KEY_NUM; i++) {
		keys[i] = 'A' + i;
	}
	get_random_array(freqs, KEY_NUM, FREQ_MAX);
	print_array(freqs, KEY_NUM, "freqs");
	heap = hc_build_heap(keys, freqs, KEY_NUM);
	hcn = hc_build_haffman_tree_from_heap(heap);
	hc_print_haffman_code(heap->first, KEY_NUM);
/*
	for (i = 0; i < KEY_NUM; i++) {
		hn = hc_extract_min_heap(heap);
		printf("hn: hcn->key = %c, hcn->freq = %i, p = %p, l = %p, r = %p\n", hn->hcn->key,hn->hcn->freq, hn->p, hn->l, hn->r);
		free(hn->hcn);
		free(hn);
	}
*/
	return 0;
}

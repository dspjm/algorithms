/* interval_tree.c
 *
 * Author: Jimmy Pan
 * Email: dspjmt@gmail.com
 * Date: Fri Apr 19 14:11:25 CST 2013
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
#include <stdlib.h>
#include <string.h>
#include "algorithms.h"

#define INTVL_NUM 200
#define INTVL_MAX 1000

enum color { BLACK = 0, RED = 1 };

struct interval {
	int high;
	int low;
};

struct intt_node {
	struct interval it;
	int max;
	int key;
	enum color c;
	struct intt_node *p;
	struct intt_node *l;
	struct intt_node *r;
};

struct intt {
	struct intt_node *root;
	struct intt_node *nil;
	struct intt_node sentinel;
};

void get_random_interval(struct interval intv[])
{
	int i;
	int tmp[INTVL_NUM * 2];
	int *tmp1 = tmp + INTVL_NUM;
	get_random_array(tmp, INTVL_NUM * 2, INTVL_MAX);
	print_array(tmp, INTVL_NUM, "Arrary1");
	print_array(tmp1, INTVL_NUM, "Arrary1");
	for (i = 0; i < INTVL_NUM; i++) {
		if (tmp[i] > tmp1[i]) {
			intv[i].high = tmp[i];
			intv[i].low = tmp1[i];
		} else {
			intv[i].high = tmp1[i];
			intv[i].low = tmp[i];
		}
	}
}

void intt_init(struct intt *intt)
{
	intt->nil = &intt->sentinel;
	intt->root = intt->nil;
	intt->sentinel.it.high = 0;
	intt->sentinel.it.low = 0;
	intt->sentinel.max = 0;
	intt->sentinel.key = 0;
	intt->sentinel.c = BLACK;
	intt->sentinel.p = intt->nil;
	intt->sentinel.l = intt->nil;
	intt->sentinel.r = intt->nil;
}

struct intt_node *intt_alloc_init_inttnode(struct intt *t, struct interval it)
{
	struct intt_node *tmp;
	tmp = malloc(sizeof *tmp);
	tmp->it.high = it.high;
	tmp->it.low = it.low;
	tmp->max = it.high;
	tmp->key = it.low;
	tmp->c = RED;
	tmp->p = t->nil;
	tmp->l = t->nil;
	tmp->r = t->nil;
	return tmp;
}

void intt_rotate_left(struct intt *t, struct intt_node *tn)
{
	struct intt_node *tnp, *tnr;
	tnp = tn->p;
	tnr = tn->r;
	if (tnp == t->nil)
		t->root = tnr;
	else if (tn == tnp->l)
		tnp->l = tnr;
	else
		tnp->r = tnr;
	tnr->p = tnp;
	tn->r = tnr->l;
	if (tn->r != t->nil)
		tn->r->p = tn;
	tn->p = tnr;
	tnr->l = tn;
}

void intt_rotate_right(struct intt *t, struct intt_node *tn)
{
	struct intt_node *tnp, *tnl;
	tnp = tn->p;
	tnl = tn->l;
	if (tnp == t->nil)
		t->root = tnl;
	else if (tn == tnp->l)
		tnp->l = tnl;
	else
		tnp->r = tnl;
	tnl->p = tnp;
	tn->l = tnl->r;
	if (tn->l != t->nil)
		tn->l->p = tn;
	tn->p = tnl;
	tnl->r = tn;
}

void intt_fix_insert_balance(struct intt *t, struct intt_node *tn)
{
	struct intt_node *tnu;
	while (tn->p->c == RED) {
		if (tn->p = tn->p->p->l) {
			tnu = tn->p->p->r;
			if (tnu->c == RED) {
				tn->p->c = BLACK;
				tnu->c = BLACK;
				tn->p->p->c = RED;
				tn = tn->p->p;
			} else if (tn == tn->p->r) {
				tn = tn->p;
				intt_rotate_left(t, tn);
			} else {
				tn->p->c = BLACK;
				tn->p->p->c = RED;
				intt_rotate_right(t, tn->p->p);
			}
		} else {
			tnu = tn->p->p->l;
			if (tnu->c == RED) {
				tnu->c = BLACK;
				tn->p->c = BLACK;
				tn->p->p->c = RED;
				tn = tn->p->p;
			} else if (tn == tn->p->l) {
				tn = tn->p;
				intt_rotate_right(t, tn);
			} else {
				tn->p->c = BLACK;
				tn->p->p->c = RED;
				intt_rotate_left(t, tn->p->p);
			}
		}
	}
	t->root->c = BLACK;
}

void intt_insert(struct intt *t, struct interval it)
{
	struct intt_node *tmp, *tmp1, *tmp2;
	tmp = intt_alloc_init_inttnode(t, it);
	tmp1 = t->root;
	tmp2 = tmp1->p;
	while (tmp1 != t->nil) {
		tmp2 = tmp1;
		if (tmp->key <= tmp2->key)
			tmp1 = tmp2->l;
		else
			tmp1 = tmp2->r;
	}
	if (tmp2 == t->nil)
		t->root = tmp;
	else if (tmp->key <= tmp2->key)
		tmp2->l = tmp;
	else
		tmp2->r = tmp;
	tmp->p = tmp2;
	intt_fix_insert_balance(t, tmp);
}

int intt_check_pointers(struct intt *t, struct intt_node *tn)
{
	if (tn->p != t->nil && tn->p->l != tn && tn->p->r != tn)
		return 1;
	if (tn->l != t->nil && tn->l->p != tn)
		return 2;
	if (tn->r != t->nil && tn->r->p != tn)
		return 3;
	return 0;
}

int intt_check(struct intt *t, struct intt_node *tn)
{
	int ret;
	int bhl = 0, bhr = 0, bh = 0;
	if (ret = intt_check_pointers(t, tn)) {
		printf("error checking pointer of %d, ret = %i\n", tn->key, ret);
		return -1;
	}
	if (tn->l != t->nil)
		bhl = intt_check(t, tn->l);
	if (tn->r != t->nil)
		bhr = intt_check(t, tn->r);
	if (bhl != bhr) {
		printf("%d unbalanced\n", tn->key);
		return -2;
	}
	bh = bhl;
	if (tn->c == BLACK)
		bh++;
	return bh;
}

struct intt_node *intt_minimum(struct intt *t, struct intt_node *tn)
{
	while (tn->l != t->nil)
		tn = tn->l;
	return tn;
}

struct intt_node *intt_successor(struct intt *t, struct intt_node *tn)
{
	struct intt_node *tmp;
	if (tn->r != t->nil)
		return intt_minimum(t, tn->r);
	tmp = tn->p;
	while (tn == tmp->r) {
		tn = tmp;
		tmp = tn->p;
	}
	return tmp;
}

void intt_print(struct intt *t)
{
	int i = 0;
	struct intt_node *tmp;
	tmp = intt_minimum(t, t->root);
	while (tmp != t->nil) {
		printf("key = %d, interval = [ %d - %d ],c = %d, p = %d, l = %d," " r = %d\n", tmp->key, tmp->it.low, tmp->it.high, tmp->c, tmp->p->key, tmp->l->key, tmp->r->key);
		tmp = intt_successor(t, tmp);
		i++;
	}
	printf("%d nodes printed\n\n", i);
}

int main(int argc, char **argv)
{
	int i;
	struct interval tmp[INTVL_NUM];
	struct intt tmp1;
	get_random_interval(tmp);
	intt_init(&tmp1);
	for (i = 0; i < INTVL_NUM; i++) {
		intt_insert(&tmp1, tmp[i]);
		if (intt_check(&tmp1, tmp1.root) < 0)
			exit(1);
		intt_print(&tmp1);
	}
	return 0;
}

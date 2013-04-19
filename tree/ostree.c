/* ostree.c
 *
 * Author: Jimmy Pan
 * Email: dspjmt@gmail.com
 * Date: Wed Apr 17 20:51:46 CST 2013
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
#include "algorithms.h"

#define KEY_NUM 20
#define KEY_MAX 1000

enum color { BLACK = 0, RED = 1 };

struct ost_node {
	int key;
	int size;
	enum color c;
	struct ost_node *p;
	struct ost_node *l;
	struct ost_node *r;
};

struct ost {
	struct ost_node *root;
	struct ost_node *nil;
	struct ost_node sentinel;
};


void ost_init(struct ost *t)
{
	t->nil = &t->sentinel;
	t->root = t->nil;
	t->sentinel.key = 0;
	t->sentinel.size = 0;
	t->sentinel.p = t->nil;
	t->sentinel.l = t->nil;
	t->sentinel.r = t->nil;
}

struct ost_node *ost_alloc_init_node(struct ost *t, int key)
{
	struct ost_node *tmp;
	tmp = malloc(sizeof *tmp);
	tmp->key = key;
	tmp->size = 1;
	tmp->c = RED;
	tmp->p = t->nil;
	tmp->l = t->nil;
	tmp->r = t->nil;
	return tmp;
}

void ost_rotate_left(struct ost *t, struct ost_node *tn)
{
	struct ost_node *tnp, *tnr;
/*
	if (tn == t->nil)
		return;
*/
	tnp = tn->p;
	tnr = tn->r;
	if (tn == t->root)
		t->root = tnr;
	else if (tn == tn->p->l)
		tnp->l = tnr;
	else
		tnp->r = tnr;
	tn->p = tnr;
	tn->r = tnr->l;
	tn->r->p = tn;
	tnr->p = tnp;
	tnr->l = tn;
	tn->size = tn->l->size + tn->r->size + 1;
	tnr->size = tnr->l->size + tnr->r->size + 1;
}

void ost_rotate_right(struct ost *t, struct ost_node *tn)
{
	struct ost_node *tnp, *tnl;
/*
	if (tn == t->nil)
		return;
*/
	tnp = tn->p;
	tnl = tn->l;
	if (tn == t->root)
		t->root = tnl;
	else if(tn == tnp->l)
		tnp->l = tnl;
	else
		tnp->r = tnl;
	tn->p = tnl;
	tn->l = tnl->r;
	tn->l->p = tn;
	tnl->p = tnp;
	tnl->r = tn;
	tn->size = tn->l->size + tn->r->size + 1;
	tnl->size = tnl->l->size + tnl->r->size + 1;
}

void ost_fix_insert_balance(struct ost *t, struct ost_node *tn)
{
	struct ost_node *uncle;
	while (tn->p->c == RED) {
		if (tn->p == tn->p->p->l) {
			uncle = tn->p->p->r;
			if (uncle->c == RED) {
				tn->p->c = BLACK;
				uncle->c = BLACK;
				tn->p->p->c = RED;
				tn = tn->p->p;
			} else if (tn == tn->p->r) {
				tn = tn->p;
				ost_rotate_left(t, tn);
			} else {
				tn->p->p->c = RED;
				tn->p->c = BLACK;
				ost_rotate_right(t, tn->p->p);
			}
		} else {
			uncle = tn->p->p->l;
			if (uncle->c == RED) {
				tn->p->c = BLACK;
				uncle->c = BLACK;
				tn->p->p->c = RED;
				tn = tn->p->p;
			} else if (tn == tn->p->l) {
				tn = tn->p;
				ost_rotate_right(t, tn);
			} else {
				tn->p->p->c = RED;
				tn->p->c = BLACK;
				ost_rotate_left(t, tn->p->p);
			}
		}
	}
	t->root->c = BLACK;
}

void ost_insert(struct ost *t, int key)
{
	struct ost_node *tmp, *tmpc, *tmpp;
	tmp = ost_alloc_init_node(t, key);
	tmpc = t->root;
	tmpp = tmpc->p;
	while (tmpc != t->nil) {
		tmpp = tmpc;
		if (key <= tmpp->key)
			tmpc = tmpp->l;
		else
			tmpc = tmpp->r;
		tmpp->size++;
	}
	if (tmpc == t->root)
		t->root = tmp;
	else if (key <= tmpp->key)
		tmpp->l = tmp;
	else
		tmpp->r = tmp;
	tmp->p = tmpp;
	ost_fix_insert_balance(t, tmp);
}

struct ost_node *ost_minimum(struct ost *t, struct ost_node *tn)
{
	if (tn == t->nil)
		return tn;
	while (tn->l != t->nil) {
		tn = tn->l;
	}
	return tn;
}

struct ost_node *ost_successor(struct ost *t, struct ost_node *tn)
{
	if (tn->r != t->nil)
		return ost_minimum(t, tn->r);
	while (tn == tn->p->r && tn != t->root)
		tn = tn->p;
	return tn->p;
}

void ost_print(struct ost *t)
{
	int i = 0;
	struct ost_node *tmp;
	tmp = ost_minimum(t, t->root);
	while (tmp != t->nil) {
		printf("%d-%d c:%d p:%d l:%d r:%d\n", tmp->key, tmp->size, tmp->c, tmp->p->key, tmp->l->key, tmp->r->key);
		tmp = ost_successor(t, tmp);
		i++;
	}
	printf("%d nodes printed\n\n", i);
}

struct ost_node *ost_select(struct ost *t, struct ost_node *tn, int r)
{
	if (r == tn->l->size + 1)
		return tn;
	else if (r > 0 && r <= tn->l->size)
		return ost_select(t, tn->l, r);
	else if (r > tn->l->size + 1 && r <= tn->size)
		return ost_select(t, tn->r, r - tn->l->size - 1);
	return t->nil;
}

/*replace a with b*/
void ost_replace(struct ost *t, struct ost_node *a, struct ost_node *b)
{
	b->p = a->p;
	b->l = a->l;
	b->r = a->r;
	b->size = a->size;
	b->c = a->c;
	if (a->p == t->nil)
		t->root = b;
	else if (a == a->p->l)
		a->p->l = b;
	else
		a->p->r = b;
	a->l->p = b;
	a->r->p = b;
}

void ost_fix_delete_balance(struct ost *t, struct ost_node *tn)
{
	struct ost_node *sbl;
	while (tn != t->root && tn->c == BLACK) {
		if (tn == tn->p->l) {
			sbl = tn->p->r;
			if (sbl->c == RED) {
				sbl->p->c = RED;
				sbl->c = BLACK;
				ost_rotate_left(t, tn->p);
			 } else if (sbl->r->c == BLACK) {
				if (sbl->l->c == BLACK) {
				 	sbl->c = RED;
					tn = tn->p;
					/* There is a huge mistake in 
					   MIT's inroduction to algorithm,
					   We might change color of nil when tn
					   is nil, this would lead to bug */
					t->nil->c = BLACK;
				} else {
					sbl->c = RED;
					sbl->l->c = BLACK;
					ost_rotate_right(t, sbl);
				}
			} else {
				sbl->c = sbl->p->c;
				sbl->r->c = BLACK;
				sbl->p->c = BLACK;
				ost_rotate_left(t, tn->p);
				tn = t->root;
			}
		} else {
			sbl = tn->p->l;
			if (sbl->c == RED) {
				sbl->p->c = RED;
				sbl->c = BLACK;
				ost_rotate_right(t, tn->p);
			} else if (sbl->l->c == BLACK) {
				if (sbl->r->c == BLACK) {
					sbl->c = RED;
					tn = tn->p;
					/* There is a huge mistake in 
					   MIT's inroduction to algorithm,
					   We might change color of nil when tn
					   is nil, this would lead to bug */
					t->nil->c = BLACK;
				} else {
					sbl->c = RED;
					sbl->r->c = BLACK;
					ost_rotate_left(t, sbl);
				}
			} else {
				sbl->c = sbl->p->c;
				sbl->p->c = BLACK;
				sbl->l = BLACK;
				ost_rotate_right(t, tn->p);
				tn = t->root;
			}
		}
	}
	tn->c = BLACK;
}

void ost_delete(struct ost *t, struct ost_node *tn)
{
	struct ost_node *tmp, *tmpp, *tmpc, *tmpt;
	if (tn->l == t->nil || tn->r == t->nil)
		tmp = tn;
	else
		tmp = ost_successor(t, tn);
	tmpp = tmp->p;
	if (tmp->l != t->nil) 
		tmpc = tmp->l;
	else
		tmpc = tmp->r;
	if (tmpp == t->nil)
		t->root = tmpc;
	else if (tmp == tmpp->l)
		tmpp->l = tmpc;
	else
		tmpp->r = tmpc;
	tmpc->p = tmpp;
	tmpt = tmpp;
	while(tmpt != t->nil) {
		tmpt->size--;
		tmpt = tmpt->p;
	}
/*
	printf("deleting %i\n", tn->key);
	ost_print(t);
*/
	if (tmp->c == BLACK)
		ost_fix_delete_balance(t, tmpc);
	if (tn != tmp)
		ost_replace(t, tn, tmp);
	free(tn);
}

int main(int argc, char **argv)
{
	int i;
	int keys[KEY_NUM] = {416, 743, 862, 203, 394, 831, 197, 299, 525, 368, 535, 814, 835, 168, 223, 482, 960, 6, 210, 165,};
	struct ost ost;
	struct ost_node *tmp;
/*
	get_random_array(keys, KEY_NUM, KEY_MAX);
*/
	print_array(keys, KEY_NUM, "Original array");
	ost_init(&ost);
	for (i = 0; i < KEY_NUM; i++) {
/*
		printf("inserting keys[%d]\n", i);
*/
		ost_insert(&ost, keys[i]);
/*
		ost_print(&ost);
*/
	}
	printf("ost after insertion:\n");
	ost_print(&ost);
/*
	for (i = 0; i < KEY_NUM; i++) {
		tmp = ost_select(&ost, ost.root, i + 1);
		printf("the %dth smallest key is %d\n", i + 1, tmp->key);
	}
*/
	printf("deleting tree:\n");
	for (i = 0; i < KEY_NUM; i++) {
		tmp = ost_minimum(&ost, ost.root);
		ost_delete(&ost, tmp);
		ost_print(&ost);
	}
/*
*/
	return 0;
}

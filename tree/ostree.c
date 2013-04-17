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

void ost_fix_insert_balance(struct ost *t, struct ost_node *tn)
{
	

void ost_insert(struct ost *t, int key)
{
	struct ost_node *tmp;
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
	else if (tmpc == tmpp->l)
		tmpp->l = tmp;
	else
		tmpp->r = tmp;
	tmp->p = tmpp;
	ost_fix_insert_balance(t, tmp);
}

int main(int argc, char **argv)
{
	int keys[KEY_NUM];
	struct ost ost;
	struct ost_node *tmp;
	get_random_array(keys, KEY_NUM, KEY_MAX);
	print_array(keys, KEY_NUM, "Original array");
	ost_init(&ost);
	for (i = 0; i < KEY_NUM; i++) {
		ost_insert(&ost, keys[i]);
	}
	printf("ost after insertion:\n");
	ost_print(&ost);
	for (i = 0; i < KEY_NUM; i++) {
		tmp = ost_select(&ost, i + 1);
		printf("the %dth smallest key is %d\n", i + 1, tmp->key);
	}
	printf("deleting tree:\n");
	for (i = 0; i < KEY_NUM; i++) {
		tmp = ost_minimum(&ost);
		ost_delete(&ost);
		ost_print(&ost);
	}
	return 0;
}

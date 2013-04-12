/* tree.c
 *
 * Author: Jimmy Pan
 * Email: dspjmt@gmail.com
 * Date: Fri Apr 12 09:22:45 CST 2013
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

#define ARR_SIZE 200
#define ARR_RANGE 1000

struct bst_node {
	int key;
	struct bst_node *p;
	struct bst_node *l;
	struct bst_node *r;
};

struct bst {
	struct bst_node *root;
};

void init_bst(struct bst *bst)
{
	bst->root = NULL;
}

struct bst_node *alloc_init_bst_node(void)
{
	struct bst_node *tmp;
	tmp = malloc(sizeof *tmp);
	memset(tmp, 0, sizeof *tmp);
	return tmp;
}

int insert_bst(struct bst *bst, int key)
{
	struct bst_node *tmp, *tmp1, *tmp2;
	tmp1 = NULL;
	tmp2 = bst->root;
	while (tmp2 != NULL) {
		tmp1 = tmp2;
		if (key <= tmp2->key)
			tmp2 = tmp2->l;
		else
			tmp2 = tmp2->r;
	}
	if (!tmp1) {
		bst->root = tmp;
		return 0;
	}
	if (key <= tmp1->key)
		tmp1->l = tmp;
	else
		tmp1->r = tmp;
	return 0;
}

struct bst_node *bst_subsearch(struct bst_node *bn, int key)
{
	if (!bn || bn->key == key)
		return bn;
	else if (key < bn->key)
		return bst_subsearch(bn->l, key);
	else
		return bst_subsearch(bn->r, key);
}

struct bst_node *bst_search(struct bst *bst, int key)
{
	struct bst_node *tmp;
	tmp = bst_subsearch(bst->root, key);
	return tmp;
}

struct bst_node *bst_subminimum(struct bst_node *bn)
{
	if (!bn)
		return NULL;
	while (bn->l)
		bn = bn->l;
	return bn;
}

struct bst_node *bst_minimum(struct bst *bst, int key)
{
	struct bst_node *tmp;
	tmp = bst->root;
	return bst_subminimum(tmp);
}

struct bst_node *bst_submaximum(struct bst_node *bn)
{
	if (!bn)
		return NULL;
	while (bn->r)
		bn = bn->r;
	return bn;
}

struct bst_node *bst_maximum(struct bst *bst)
{
	struct bst_node *tmp;
	tmp = bst->root;
	return bst_subminimum(tmp);
}

struct bst_node *bst_successor(struct bst_node *bn)
{
	if (bn->r)
		return bst_subminimum(bn);
	
}



int main(int argc, char **argv)
{
	int i;
	int tmp[ARR_SIZE];
	struct bst bst;
	get_random_array(tmp, ARR_SIZE, ARR_RANGE);
	print_array(tmp, ARR_SIZE, "original array");
	init_bst(&bst);
	for (i = 0; i < ARR_SIZE; i++)
		insert_bst(&bst, tmp[i]);
	print_bst(&bst);
}

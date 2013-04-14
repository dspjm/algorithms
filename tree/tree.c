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

int bst_insert(struct bst *bst, int key)
{
	struct bst_node *tmp, *tmp1, *tmp2;
	tmp = alloc_init_bst_node();
	tmp->key = key;
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
	tmp->p = tmp1;
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

struct bst_node *bst_minimum(struct bst *bst)
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
	struct bst_node *tmp, *tmp1;
	if (bn->r)
		return bst_subminimum(bn->r);
	tmp = bn->p;
	tmp1 = bn;
	while (tmp && tmp1 == tmp->r) {
		tmp = tmp->p;
		tmp1 = tmp1->p;
	}
	return tmp;
}

struct bst_node *bst_predecessor(struct bst_node *bn)
{
	struct bst_node *tmp, *tmp1;
	if (bn->l)
		return bst_submaximum(bn->l);
	tmp = bn;
	tmp1 = bn->p;
	while (tmp1 && tmp == tmp1->l) {
		tmp = tmp1;
		tmp1 = tmp1->p;
	}
	return tmp1;
}

int bst_delete(struct bst *bst, struct bst_node *bn)
{
	struct bst_node *tmp, *tmp1, *tmp2;
	/* find the node whose children need to be take care of
	   tmp has at most one child*/
	if (!bn->l || !bn->r)
		tmp = bn;
	else
		tmp = bst_successor(bn);
	/* take good care of the children*/
	/* tmp must have no more than 1 child */
	if (tmp->l)
		tmp1 = tmp->l;
	else
		tmp1 = tmp->r;
	tmp2 = tmp->p;
	if (tmp2) {
		if (tmp == tmp2->l)
			tmp2->l = tmp1;
		else
			tmp2->r = tmp1;
	} else
	/* this indicates tmp is root. It's impossible for a node with two
	   children to have a successor of root. So this condition also
	   has a premise that bn is root obtaining no more than one child */
		bst->root = tmp1;
	if (tmp1)
		tmp1->p = tmp2;
	/* if bn has no more than one child, work is done, we now deal with
	   bn has two children, we exchange bn and tmp*/
	if (tmp != bn) {
		bn->l->p = tmp;
		bn->r->p = tmp;
		tmp->p = bn->p;
		tmp->l = bn->l;
		tmp->r = bn->r;
		if (!bn->p)
			bst->root = tmp;
		else if (bn == bn->p->l)
			bn->p->l = tmp;
		else
			bn->p->r = tmp;
	}
	free(bn);
	return 0;
}

int bst_check(struct bst *bst)
{
	struct bst_node *tmp, *tmpp, *tmpl, *tmpr;
	tmp = bst_minimum(bst);
	while (tmp) {
		tmpp = tmp->p;
		tmpl = tmp->l;
		tmpr = tmp->r;
/*
		printf("tmp is %p, tmp->key = %d\n", tmp, tmp->key);
*/
		if (tmpp) {
			if (tmp->key <= tmpp->key && tmpp->l != tmp) {
				printf("tmpp's left wrong\n");
				return 1;
			} else if (tmp->key > tmpp->key && tmpp->r != tmp) {
				printf("tmpp's right wrong\n");
				return 1;
			}
		} else if (tmp != bst->root) {
			printf("tmp's parent missing\n");
			return 1;
		}
		if (tmpl) {
			if (tmpl->p != tmp) {
				printf("tmpl's parent wrong\n");
				return 1;
			}
		}
		if (tmpr) {
			if (tmpr->p != tmp) {
				printf("tmpr's parent wrong\n");
				return 1;
			}
		}
		tmp = bst_successor(tmp);
	}
/*
	printf("bst no problem\n");
*/
	return 0;
}

void bst_print(struct bst *bst)
{
	struct bst_node *tmp;
	tmp = bst_minimum(bst);
	puts("binary search tree:");
	if (!bst->root) {
		puts("tree empty");
		return;
	}
	printf("root key is %d\n", bst->root->key);
	while (tmp) {
		printf("%d ", tmp->key);
		tmp = bst_successor(tmp);
	}
	printf("\n");
}

int main(int argc, char **argv)
{
	int i;
	int tmp[ARR_SIZE];
	struct bst bst;
	struct bst_node *tmp1;
	get_random_array(tmp, ARR_SIZE, ARR_RANGE);
	print_array(tmp, ARR_SIZE, "original array");
	init_bst(&bst);
	for (i = 0; i < ARR_SIZE; i++)
		bst_insert(&bst, tmp[i]);
	bst_print(&bst);
	bst_check(&bst);
	for (i = 0; i < ARR_SIZE; i++) {
		tmp1 = bst_minimum(&bst);
/*
		printf("minimum found, tmp1 = %p, key = %d, p = %p, l = %p, r = %p, p->key = %d, p->l = %p, p->r = %p\n", tmp1, tmp1->key, tmp1->p, tmp1->l, tmp1->r, tmp1->p->key, tmp1->p->l, tmp1->p->r);
		bst_print(&bst);
*/
		bst_delete(&bst, tmp1);
/*
		printf("after delete, tmp1 = %p, key = %d, p = %p, l = %p, r = %p, p->key = %d, p->l = %p, p->r = %p\n", tmp1, tmp1->key, tmp1->p, tmp1->l, tmp1->r, tmp1->p->key, tmp1->p->l, tmp1->p->r);
*/
		bst_check(&bst);
	}
	bst_print(&bst);
}

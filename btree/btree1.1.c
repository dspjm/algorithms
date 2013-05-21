/* btree1.1.c
 *
 * Author: Jimmy Pan
 * Email: dspjmt@gmail.com
 * Date: Mon May 20 23:24:44 CST 2013
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
#include <stdlib.h>
#include "algorithms.h"

#define MIN_CHILD_NUM 10
#define MAX_CHILD_NUM (MIN_CHILD_NUM * 2)
#define MIN_KEY_NUM (MIN_CHILD_NUM - 1)
#define MAX_KEY_NUM (MAX_CHILD_NUM - 1)
#define KEY_NUM 1000
#define KEY_MAX 1000

enum leaf { NONLEAF = 0, LEAF = 1 };

struct btree_node {
	int key_num;
	int keys[MAX_KEY_NUM];
	struct btree_node *children[MAX_CHILD_NUM];
	int leaf;
};

static struct btree_node *bt_alloc_init_btree_node(int leaf)
{
	struct btree_node *tmp;
	tmp = malloc(sizeof *tmp);
	memset(tmp, 0, sizeof *tmp);
	tmp->leaf = leaf;
	return tmp;
}

static int bt_btree_node_is_max_size(struct btree_node *n)
{
	return n->key_num >= MAX_KEY_NUM;
}

static int bt_btree_node_is_min_size(struct btree_node *n)
{
	return n->key_num <= MIN_KEY_NUM;
}

/* index is children index, index can be last child*/
static void
bt_btree_node_shift_left_from_index(struct btree_node *n, int index)
{
	int i;
	if (index == 0)
		index = 1;
	for (i = index; i < n->key_num; i++)
		n->keys[i - 1] = n->keys[i];
	if (!n->leaf)
		for (i = index; i <= n->key_num; i++)
			n->children[i - 1] = n->children[i];
	n->key_num--;
}

/* index is children index, index can be last child*/
static void
bt_btree_node_shift_right_from_index(struct btree_node *n, int index)
{
	int i, max_num;
	max_num = n->key_num + 1 < MAX_KEY_NUM ? n->key_num + 1: MAX_KEY_NUM;
	for (i = max_num - 1; i > index; i--)
		n->keys[i] = n->keys[i - 1];
	if (!n->leaf)
		for (i = max_num; i > index; i--)
			n->children[i] = n->children[i - 1];
	n->key_num = max_num;
}

static void
bt_btree_node_pop_head(struct btree_node *n, int *rk, struct btree_node **rn)
{
	*rk = n->keys[0];
	if (n->leaf)
		*rn = NULL;
	else
		*rn = n->children[0];
	bt_btree_node_shift_left_from_index(n, 1);
}

static void
bt_btree_node_pop_tail(struct btree_node *n, int *rk, struct btree_node **rn)
{
	*rk = n->keys[n->key_num - 1];
	if (n->leaf)
		*rn = NULL;
	else
		*rn = n->children[n->key_num];
	n->key_num--;
}

static void bt_btree_node_transfer_from_left(struct btree_node *n, int pos)
{
	int tmpk;
	struct btree_node *tmpn, *child;
	child = n->children[pos];
	bt_btree_node_pop_tail(n->children[pos - 1], &tmpk, &tmpn);
	bt_btree_node_shift_right_from_index(child, 0);
	child->keys[0] = n->keys[pos - 1];
	n->keys[pos - 1] = tmpk;
	if (!child->leaf)
		child->children[0] = tmpn;
}

static void bt_btree_node_transfer_from_right(struct btree_node *n, int pos)
{
	int tmpk;
	struct btree_node *tmpn, *child;
	child = n->children[pos];
	bt_btree_node_pop_head(n->children[pos + 1], &tmpk, &tmpn);
	child->keys[child->key_num] = n->keys[pos];
	n->keys[pos] = tmpk;
	if (!child->leaf)
		child->children[child->key_num + 1] = tmpn;
	child->key_num++;
}

static int bt_btree_node_find_max(struct btree_node *n)
{
	int tmp;
	while (!n->leaf)
		n = n->children[n->key_num];
	return n->keys[n->key_num - 1];
}

static int bt_btree_node_find_min(struct btree_node *n)
{
	int tmp;
	while (!n->leaf)
		n = n->children[0];
	return n->keys[0];
}

static struct btree_node *
bt_btree_node_split_node(struct btree_node *n, int *newkey)
{
	int i;
	struct btree_node *tmp;
	tmp = bt_alloc_init_btree_node(n->leaf);
	for (i = 0; i < MIN_KEY_NUM; i++)
		tmp->keys[i] = n->keys[MIN_KEY_NUM + 1 + i];
	if (!n->leaf)
		for (i = 0; i < MIN_CHILD_NUM; i++)
			tmp->children[i] = n->children[MIN_CHILD_NUM + i];
	*newkey = n->keys[MIN_KEY_NUM];
	n->key_num = MIN_KEY_NUM;
	tmp->key_num = MIN_KEY_NUM;
	return tmp;
}

static void bt_btree_node_split_child(struct btree_node *n, int index)
{
	int newkey;
	struct btree_node *child, *sib;
	child = n->children[index];
	sib = bt_btree_node_split_node(child, &newkey);
	bt_btree_node_shift_right_from_index(n, index + 1);
	n->keys[index + 1] = n->keys[index];
	n->keys[index] = newkey;
	n->children[index + 1] = sib;
}

/* gurantee n is not of maximum size before you call */
static void bt_btree_node_insert_key(struct btree_node *n, int key)
{
	int i, pos;
	struct btree_node *tmp;
	for (i = 0; i < n->key_num && key > n->keys[i]; i++);
	pos = i;
	if (n->leaf) {
		bt_btree_node_shift_right_from_index(n, pos);
		n->keys[pos] = key;
		return;
	}
	/* children[i]'s keys are always larger than keys[i - 1],
	 * no larger than keys[i] */
	if (bt_btree_node_is_max_size(n->children[pos])) {
		bt_btree_node_split_child(n, pos);
		if (key > n->keys[pos])
			pos++;
	}
	bt_btree_node_insert_key(n->children[pos], key);
}

static void bt_btree_node_merge_children(struct btree_node *n, int pos)
{
	int i;
	struct btree_node *child, *sib, tmpn;
	child = n->children[pos];
	sib = n->children[pos + 1];
	for (i = 0; i < MIN_KEY_NUM; i++)
		child->keys[MIN_KEY_NUM + 1 + i] = sib->keys[i];
	if (!child->leaf)
		for (i = 0; i < MIN_CHILD_NUM; i++)
			child->children[MIN_CHILD_NUM + i] = sib->children[i];
	child->keys[MIN_KEY_NUM] = n->keys[pos];
	child->key_num = MAX_KEY_NUM;
	n->children[pos + 1] = child;
	bt_btree_node_shift_left_from_index(n, pos + 1);
	free(sib);
}

static void bt_btree_node_solve_child_minsize(struct btree_node *n, int pos)
{
	if (pos > 0 && !bt_btree_node_is_min_size(n->children[pos - 1]))
		bt_btree_node_transfer_from_left(n, pos);
	else if (pos < n->key_num &&
	  !bt_btree_node_is_min_size(n->children[pos + 1]))
		bt_btree_node_transfer_from_right(n, pos);
	else if (pos < n->key_num)
		bt_btree_node_merge_children(n, pos);
	else
		bt_btree_node_merge_children(n, pos - 1);
}

/* gurantee n is not of minimum size before you call */
static void bt_btree_node_delete_key(struct btree_node *n, int key)
{
	int i, pos, tmp;
	struct btree_node *pc, *nc;
	for (i = 0; i < n->key_num && key > n->keys[i]; i++);
	pos = i;
	if (n->leaf && (pos >= n->key_num || n->keys[pos] != key)) {
		printf("key %i don't exsits", key);
		return;
	}
	/* delete key from itself */
	if (pos < n->key_num && n->keys[pos] == key) {
		if (n->leaf) {
			bt_btree_node_shift_left_from_index(n, pos + 1);
		} else if (!bt_btree_node_is_min_size(n->children[pos])) {
			tmp = bt_btree_node_find_max(n->children[pos]);
			n->keys[pos] = tmp;
			bt_btree_node_delete_key(n->children[pos], tmp);
		} else if (!bt_btree_node_is_min_size(n->children[pos + 1])) {
			tmp = bt_btree_node_find_min(n->children[pos + 1]);
			n->keys[pos] = tmp;
			bt_btree_node_delete_key(n->children[pos + 1], tmp);
		} else {
			bt_btree_node_merge_children(n, pos);
			bt_btree_node_delete_key(n->children[pos], key);
		}
	} else {
	/* delete key from child */
		if (bt_btree_node_is_min_size(n->children[pos])) {
			bt_btree_node_solve_child_minsize(n, pos);
			bt_btree_node_delete_key(n, key);
		} else {
			bt_btree_node_delete_key(n->children[pos], key);
		}
	}
}

static void bt_btree_node_print(struct btree_node *n)
{
	int i;
	printf("%d keys: ", n->key_num);
	for (i = 0; i < n->key_num; i++)
		printf("%i ", n->keys[i]);
	if (n->leaf) {
		printf(" it's a leaf\n");
		return;
	}
	printf("\n");
	for (i = 0; i <= n->key_num; i++) {
		printf("subtree for %p at %p:\n", n, n->children[i]);
		bt_btree_node_print(n->children[i]);
	}
}

struct btree {
	struct btree_node *root;
};

static struct btree *bt_alloc_init_btree()
{
	struct btree *tmp;
	struct btree_node *tmpn;
	tmp = malloc(sizeof *tmp);
	memset(tmp, 0, sizeof *tmp);
	tmpn = bt_alloc_init_btree_node(LEAF);
	tmp->root = tmpn;
	return tmp;
}

static void bt_btree_insert_key(struct btree *t, int key)
{
	int newkey;
	struct btree_node *root, *sib, *newroot;
	root = t->root;
	if (bt_btree_node_is_max_size(root)) {
		sib = bt_btree_node_split_node(root, &newkey);
		newroot = bt_alloc_init_btree_node(NONLEAF);
		newroot->key_num = 1;
		newroot->keys[0] = newkey;
		newroot->children[0] = root;
		newroot->children[1] = sib;
		t->root = newroot;
		root = newroot;
	}
	bt_btree_node_insert_key(root, key);
}

static void bt_btree_delete_key(struct btree *t, int key)
{
	struct btree_node *root;
	root = t->root;
	bt_btree_node_delete_key(root, key);
	if (root->key_num == 0) {
		t->root = root->children[0];
		free(root);
	}
}

static void bt_btree_print(struct btree *t)
{
	printf("\nstart printing new tree:\nroot at %p:\n", t->root);
	if (t->root)
		bt_btree_node_print(t->root);
}

int main(int argc, char **argv)
{
	int i;
	int keys[KEY_NUM];
	struct btree *btree;
	get_random_array(keys, KEY_NUM, KEY_MAX);
/*
*/
	print_array(keys, KEY_NUM, "keys");
	btree = bt_alloc_init_btree();
	printf("inserting keys\n");
	for (i = 0; i < KEY_NUM; i++) {
		bt_btree_insert_key(btree, keys[i]);
	}
	printf("insertion finished\n");
	bt_btree_print(btree);
	printf("\ndeleting keys\n");
	for (i = 0; i < KEY_NUM; i++) {
		printf("\ndeleting %i\n", keys[i]);
		bt_btree_delete_key(btree, keys[i]);
		bt_btree_print(btree);
	}
	printf("deletion finished\n");
	bt_btree_print(btree);
}

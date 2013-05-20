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

static void
bt_btree_node_shift_left_from_index(struct btree_node *n, int index)
{
	int i;
	for (i = index; i < n->key_num; i++)
		n->keys[i - 1] = n->keys[i];
	if (!n->leaf)
		for (i = index; i <= n->key_num; i++)
			n->children[i - 1] = n->children[i];
	n->key_num--;
}

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
	n->key_num++;
}

static void bt_btree_node_insert_key(struct btree_node *n, int key)
{
}

static void bt_btree_node_delete_key(struct btree_node *n, int key)
{
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
	}
	bt_btree_node_insert_key(newroot, key);
}

static void bt_btree_delete_key(struct btree *t, int key)
{
}

int main(int argc, char **argv)
{
	int keys[KEY_NUM];
	struct 
	get_random_array(keys, KEY_NUM, KEY_MAX);
}

/* btree.c
 *
 * Author: Jimmy Pan
 * Email: dspjmt@gmail.com
 * Date: Fri May 17 19:00:56 CST 2013
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
#define KEY_NUM 100
#define KEY_MAX 1000

struct btree_node {
	int key_num;
	int keys[MAX_CHILD_NUM - 1];
	struct btree_node *children[MAX_CHILD_NUM];
	int leaf;
};

static struct btree_node *bt_alloc_init_btree_node()
{
	struct btree_node *tmp;
	tmp = malloc(sizeof *tmp);
	memset(tmp, 0, sizeof *tmp);
	tmp->leaf = 1;
	return tmp;
}

static int bt_btree_node_is_full(struct btree_node *n)
{
	return n->key_num >= MAX_CHILD_NUM - 1;
}

struct btree {
	struct btree_node *root;
};

static struct btree *bt_alloc_init_btree()
{
	struct btree *tmp;
	struct btree_node *root;
	tmp = malloc(sizeof *tmp);
	memset(tmp, 0, sizeof *tmp);
	root = bt_alloc_init_btree_node();
	tmp->root = root;
	return tmp;
}

static void bt_btree_split_child(struct btree_node *p, int index)
{
	int i;
	struct btree_node *tmp, *child;
	tmp = bt_alloc_init_btree_node();
	child = p->children[index];
	tmp->leaf = child->leaf;
	tmp->key_num = MIN_CHILD_NUM - 1;
	child->key_num = MIN_CHILD_NUM - 1;
	for (i = 0; i < MIN_CHILD_NUM - 1; i++)
		tmp->keys[i] = child->keys[MIN_CHILD_NUM + i];
	if (!tmp->leaf)
		for (i = 0; i < MIN_CHILD_NUM; i++)
			tmp->children[i] = child->children[MIN_CHILD_NUM + i];

	for (i = p->key_num - 1; i >= index; i--)
		p->keys[i + 1] = p->keys[i];
	for (i = p->key_num; i > index; i--)
		p->children[i + 1] = p->children[i];
	p->keys[index] = child->keys[MIN_CHILD_NUM - 1];
	p->children[index + 1] = tmp;
	p->key_num++;
}

static void bt_btree_insert_nonfull(struct btree_node *bn, int key)
{
	int i, pos;
	for (i = 0; i < bn->key_num && bn->keys[i] < key; i++);
	pos = i;
	if (bn->leaf) {
		for (i = bn->key_num - 1; i >= pos; i--)
			bn->keys[i + 1] = bn->keys[i];
		bn->keys[pos] = key;
		bn->key_num++;
		return;
	}
	if (bt_btree_node_is_full(bn->children[pos])) {
		bt_btree_split_child(bn, pos);
		if (key > bn->keys[pos])
			bt_btree_insert_nonfull(bn->children[pos + 1], key);
		else
			bt_btree_insert_nonfull(bn->children[pos], key);
	} else
		bt_btree_insert_nonfull(bn->children[pos], key);
}

static void bt_btree_insert(struct btree *t, int key)
{
	struct btree_node *tmp, *root;
	root = t->root;
	if (bt_btree_node_is_full(root)) {
		tmp = bt_alloc_init_btree_node();
		tmp->leaf = 0;
		tmp->children[0] = root;
		t->root = tmp;
		bt_btree_split_child(tmp, 0);
		bt_btree_insert_nonfull(tmp, key);
	} else
		bt_btree_insert_nonfull(root, key);
}

static void bt_print_sub_btree(struct btree_node *n)
{
	int i;
	printf("%i keys: ", n->key_num);
	for (i = 0; i < n->key_num; i++)
		printf("%i ", n->keys[i]);
	if (n->leaf) {
		printf("it's a leaf\n");
		return;
	}
	printf("\nstart printing subtrees\n");
	printf("subtree with: keys < %i\n", n->keys[0]);
	bt_print_sub_btree(n->children[0]);
	for (i = 1; i < n->key_num; i++) {
		printf("subtree with: %i < keys < %i\n", n->keys[i - 1],
		  n->keys[i]);
		bt_print_sub_btree(n->children[i]);
	}
	printf("subtree with: keys > %i\n", n->keys[n->key_num - 1]);
	bt_print_sub_btree(n->children[i]);
	printf("end printing subtrees\n");
}

static void bt_print_btree(struct btree *t)
{
	printf("root:\n");
	bt_print_sub_btree(t->root);
}

int main(int argc, char **argv)
{
	int i;
	int keys[KEY_NUM];
	struct btree *btree;
	get_random_array(keys, KEY_NUM, KEY_MAX);
	print_array(keys, KEY_NUM, "keys");
	btree = bt_alloc_init_btree();
	for (i = 0; i < KEY_NUM; i++) {
		bt_btree_insert(btree, keys[i]);
/*
		printf("\ninserting %i\n", keys[i]);
		bt_print_btree(btree);
*/
	}
	bt_print_btree(btree);
	return 0;
}

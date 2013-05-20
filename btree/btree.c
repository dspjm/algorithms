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
#define KEY_NUM 10000
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

static int bt_btree_node_is_maximum_size(struct btree_node *n)
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
	if (bt_btree_node_is_maximum_size(bn->children[pos])) {
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
	if (bt_btree_node_is_maximum_size(root)) {
		tmp = bt_alloc_init_btree_node();
		tmp->leaf = 0;
		tmp->children[0] = root;
		t->root = tmp;
		bt_btree_split_child(tmp, 0);
		bt_btree_insert_nonfull(tmp, key);
	} else
		bt_btree_insert_nonfull(root, key);
}

static int bt_btree_node_is_minimum_size(struct btree_node *n)
{
	return n->key_num <= MIN_CHILD_NUM - 1;
}

static void bt_btree_node_child_get_key_from_left(struct btree_node *n, int index)
{
	int i;
	struct btree_node *left, *right;
	if (index < 1 || n->leaf) {
		fprintf(stdout, "illegal getting key from left\n");
		return;
	}
	right = n->children[index];
	left = n->children[index - 1];
	if (bt_btree_node_is_maximum_size(right) ||
	  bt_btree_node_is_minimum_size(left)) {
		fprintf(stdout, "error getting key from left\n");
		return;
	}
	for (i = right->key_num - 1; i >= 0; i--)
		right->keys[i + 1] = right->keys[i];
	right->keys[0] = n->keys[index - 1];
	if (!right->leaf) {
		for (i = right->key_num; i >= 0; i--)
			right->children[i + 1] = right->children[i];
		right->children[0] = left->children[left->key_num];
	}
	right->key_num++;
	n->keys[index - 1] = left->keys[left->key_num - 1];
	left->key_num--;
}

static void bt_btree_node_child_get_key_from_right(struct btree_node *n, int index)
{
	int i;
	struct btree_node *left, *right;
	if (index > n->key_num - 1 || n->leaf) {
		fprintf(stdout, "illegal getting key from left\n");
		return;
	}
	left = n->children[index];
	right = n->children[index + 1];
	if (bt_btree_node_is_maximum_size(left) ||
	  bt_btree_node_is_minimum_size(right)) {
		fprintf(stdout, "error getting key from left\n");
		return;
	}
	left->keys[left->key_num] = n->keys[index];
	n->keys[index] = right->keys[0];
	for (i = 0; i < right->key_num - 1; i++)
		right->keys[i] = right->keys[i + 1];
	if (!left->leaf) {
		left->children[left->key_num + 1] = right->children[0];
		for (i = 0; i < right->key_num; i++)
			right->children[i] = right->children[i + 1];
	}
	left->key_num++;
	right->key_num--;
}

static int bt_btree_node_find_max(struct btree_node *n)
{
	while (!n->leaf)
		n = n->children[n->key_num];
	return n->keys[n->key_num - 1];
}

static int bt_btree_node_find_min(struct btree_node *n)
{
	while (!n->leaf)
		n= n->children[0];
	return n->keys[0];
}

static void bt_btree_node_merge_children(struct btree_node *n, int index)
{
	int i;
	struct btree_node *n1, *n2;
	n1 = n->children[index];
	n2 = n->children[index + 1];
	n1->keys[MIN_CHILD_NUM - 1] = n->keys[index];
	for (i = 0; i < MIN_CHILD_NUM - 1; i++)
		n1->keys[MIN_CHILD_NUM + i] = n2->keys[i];
	if (!n1->leaf)
		for (i = 0; i < MIN_CHILD_NUM; i++)
			n1->children[MIN_CHILD_NUM + i] = n2->children[i];
	n1->key_num = MAX_CHILD_NUM - 1; free(n2);

	for (i = index; i < n->key_num - 1; i++) {
		n->keys[i] = n->keys[i + 1];
		n->children[i + 1] = n->children[i + 2];
	}
	n->key_num--;
}

static void bt_btree_node_delete_nonminimum(struct btree_node *n, int key)
{
	int i, pos, tmp;
	for (i = 0; i < n->key_num && key > n->keys[i]; i++);
	pos = i;
	if (n->leaf) {
		if (n->keys[pos] == key) {
			for (i = pos; i < n->key_num - 1; i++)
				n->keys[i] = n->keys[i + 1];
			n->key_num--;
		} else
			fprintf(stdout, "key nonexists\n");
		return;
	} else if (n->keys[pos] == key && pos < n->key_num) {
		if (!bt_btree_node_is_minimum_size(n->children[pos])) {
			tmp = bt_btree_node_find_max(n->children[pos]);
			n->keys[pos] = tmp;
			bt_btree_node_delete_nonminimum(n->children[pos], tmp);
		} else if
		  (!bt_btree_node_is_minimum_size(n->children[pos + 1])) {
			tmp = bt_btree_node_find_min(n->children[pos + 1]);
			n->keys[pos] = tmp;
			bt_btree_node_delete_nonminimum(n->children[pos + 1],
			  tmp);
		} else {
			bt_btree_node_merge_children(n, pos);
			bt_btree_node_delete_nonminimum(n->children[pos], key);
		}
		return;
	}
	
	if (!bt_btree_node_is_minimum_size(n->children[pos])) {
		bt_btree_node_delete_nonminimum(n->children[pos], key);
		return;
	}
	if (pos > 0 && !bt_btree_node_is_minimum_size(n->children[pos - 1]))
		bt_btree_node_child_get_key_from_left(n, pos);
	else if (pos < n->key_num && 
	  !bt_btree_node_is_minimum_size(n->children[pos + 1]))
		bt_btree_node_child_get_key_from_right(n, pos);
	else {
		if (pos > n->key_num - 1) {
			bt_btree_node_merge_children(n, pos - 1);
			pos--;
		} else
			bt_btree_node_merge_children(n, pos);
	}
	bt_btree_node_delete_nonminimum(n->children[pos], key);
}

static void bt_btree_delete(struct btree *t, int key)
{
	int i, tmp;
	struct btree_node *root, *child, *sib;
	root = t->root;
	if (root->key_num > 1 || root->leaf) {
		bt_btree_node_delete_nonminimum(root, key);
		return;
	}
	if (key == root->keys[0]) {
		struct btree_node *child;
		if (!bt_btree_node_is_minimum_size(root->children[0])) {
			tmp = bt_btree_node_find_max(root->children[0]);
			root->keys[0] = tmp;
			bt_btree_node_delete_nonminimum(root->children[0], tmp);
		} else if
		  (!bt_btree_node_is_minimum_size(root->children[1])) {
			tmp = bt_btree_node_find_min(root->children[1]);
			root->keys[0] = tmp;
			bt_btree_node_delete_nonminimum(root->children[1], tmp);
		} else {
			bt_btree_node_merge_children(root, 0);
			t->root = root->children[0];
			free(root);
			bt_btree_node_delete_nonminimum(t->root, key);
		}
		return;
	} else if (key > root->keys[0]) {
		child = root->children[1];
		sib = root->children[0];
	} else {
		child = root->children[0];
		sib = root->children[1];
	}
	if (!bt_btree_node_is_minimum_size(child)) {
		bt_btree_node_delete_nonminimum(child, key);
		return;
	}
	if (bt_btree_node_is_minimum_size(sib)) {
		bt_btree_node_merge_children(root, 0);
		t->root = root->children[0];
		free(root);
		bt_btree_node_delete_nonminimum(t->root, key);
	} else {
		if (key > root->keys[0])
			bt_btree_node_child_get_key_from_left(root, 1);
		else
			bt_btree_node_child_get_key_from_right(root, 0);
		bt_btree_node_delete_nonminimum(child, key);
	}
}

static void bt_print_sub_btree(struct btree_node *n)
{
	int i;
	printf("%i keys at %p: ", n->key_num, n);
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
	for (i = 0; i < KEY_NUM; i++) {
/*
		puts("");
		printf("deleting %i\n", keys[i]);
		bt_print_btree(btree);
*/
		bt_btree_delete(btree, keys[i]);
	}
	bt_print_btree(btree);
	return 0;
}

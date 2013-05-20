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
#define KEY_NUM 1000
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
	n1->key_num = MAX_CHILD_NUM - 1;
	free(n2);

	for (i = index; i < n->key_num - 1; i++) {
		n->keys[i] = n->keys[i + 1];
		n->children[i + 1] = n->children[i + 2];
	}
	n->key_num--;
}

static void bt_btree_node_delete_nonminimum(struct btree_node *n, int key)
{
	int i;
	int pos;
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
		struct btree_node *child;
		if (!bt_btree_node_is_minimum_size(n->children[pos])) {
			child = n->children[pos];
			n->keys[pos] = child->keys[child->key_num - 1];
			child->key_num--;
		} else if
		  (!bt_btree_node_is_minimum_size(n->children[pos + 1])) {
			child = n->children[pos + 1];
			n->keys[pos] =
			  child->keys[0];
			for (i = 0; i < child->key_num - 1; i++)
				child->keys[i] = child->keys[i + 1];
			child->key_num--;
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
	int i;
	struct btree_node *root, *child, *sib;
	root = t->root;
	if (root->key_num > 1 || root->leaf) {
		bt_btree_node_delete_nonminimum(root, key);
		return;
	}
/*
	if (root->leaf) {
		if (root->keys[0] == key)
			root->key_num = 0;
		else
			fprintf("can't find key %i", key);
		return;
	}
*/
	if (key == root->keys[0]) {
		struct btree_node *child;
		if (!bt_btree_node_is_minimum_size(root->children[0])) {
			child = root->children[0];
			root->keys[0] = child->keys[child->key_num - 1];
			child->key_num--;
		} else if
		  (!bt_btree_node_is_minimum_size(root->children[1])) {
			child = root->children[1];
			root->keys[0] = child->keys[0];
			for (i = 0; i < child->key_num - 1; i++)
				child->keys[i] = child->keys[i + 1];
			child->key_num--;
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
	int keys[KEY_NUM] = {
975, 575, 162, 534, 149, 676, 139, 830, 889, 407, 945, 788, 483, 440, 11, 188, 433, 998, 67, 780, 45, 860, 842, 275, 877, 193, 159, 962, 972, 334, 762, 946, 260, 923, 831, 408, 951, 321, 589, 839, 728, 885, 979, 562, 676, 989, 101, 109, 338, 519, 240, 382, 730, 81, 8, 606, 625, 167, 919, 596, 852, 680, 893, 111, 603, 724, 870, 553, 44, 458, 743, 123, 694, 721, 36, 369, 62, 489, 829, 399, 7, 68, 133, 737, 500, 140, 694, 125, 658, 613, 720, 861, 292, 965, 971, 246, 688, 192, 150, 83, 1, 245, 558, 694, 965, 593, 415, 378, 81, 243, 129, 440, 311, 261, 176, 810, 752, 221, 934, 762, 833, 6, 622, 477, 970, 945, 74, 9, 136, 224, 443, 137, 468, 1000, 182, 784, 945, 596, 162, 377, 191, 642, 816, 501, 902, 343, 662, 5, 564, 948, 766, 748, 305, 388, 576, 626, 332, 650, 634, 819, 873, 428, 307, 692, 428, 489, 475, 724, 436, 988, 100, 626, 629, 268, 478, 882, 610, 492, 887, 525, 439, 652, 625, 95, 39, 200, 720, 722, 849, 705, 541, 721, 132, 847, 764, 911, 687, 239, 986, 475, 226, 86, 100, 207, 705, 930, 88, 314, 421, 974, 191, 211, 626, 815, 305, 16, 14, 24, 738, 863, 80, 278, 935, 563, 476, 699, 826, 515, 289, 811, 989, 866, 248, 440, 72, 952, 369, 160, 618, 141, 485, 808, 351, 110, 622, 655, 126, 987, 30, 215, 201, 461, 844, 488, 24, 671, 538, 849, 185, 826, 659, 525, 43, 907, 965, 115, 210, 685, 626, 827, 826, 110, 634, 176, 572, 607, 183, 49, 946, 564, 263, 146, 25, 106, 985, 48, 776, 522, 248, 313, 699, 906, 837, 742, 164, 801, 208, 374, 838, 833, 552, 663, 294, 538, 190, 865, 496, 724, 913, 441, 288, 175, 939, 664, 632, 923, 711, 408, 797, 958, 720, 495, 215, 556, 588, 731, 709, 795, 104, 898, 627, 7, 912, 273, 544, 101, 137, 40, 177, 50, 480, 464, 576, 418, 127, 208, 693, 189, 967, 489, 498, 686, 335, 64, 593, 923, 794, 653, 69, 249, 550, 48, 256, 461, 320, 799, 914, 456, 838, 90, 857, 670, 905, 433, 439, 383, 992, 131, 571, 958, 971, 420, 995, 306, 483, 939, 580, 629, 592, 648, 877, 493, 695, 484, 306, 14, 635, 219, 822, 472, 660, 678, 493, 564, 462, 932, 946, 453, 414, 868, 410, 385, 287, 756, 42, 121, 695, 973, 749, 286, 620, 978, 130, 667, 813, 435, 32, 447, 5, 853, 271, 664, 531, 763, 579, 992, 46, 524, 445, 460, 743, 206, 196, 381, 962, 589, 502, 8, 561, 602, 645, 532, 931, 774, 198, 744, 561, 230, 190, 565, 434, 460, 229, 964, 575, 807, 308, 620, 683, 104, 79, 777, 661, 626, 158, 974, 214, 11, 981, 126, 612, 625, 658, 543, 751, 207, 638, 311, 788, 827, 875, 222, 639, 455, 537, 213, 614, 844, 832, 648, 299, 263, 424, 960, 888, 581, 285, 454, 591, 266, 579, 555, 242, 236, 449, 992, 443, 86, 302, 582, 912, 529, 803, 550, 983, 340, 114, 948, 535, 946, 595, 834, 560, 19, 145, 447, 951, 429, 252, 542, 46, 831, 448, 288, 418, 896, 279, 212, 981, 933, 794, 892, 813, 596, 794, 795, 287, 907, 95, 822, 204, 689, 7, 763, 707, 503, 562, 10, 283, 813, 903, 329, 995, 350, 616, 413, 245, 246, 624, 577, 530, 417, 820, 342, 365, 613, 489, 3, 872, 583, 176, 427, 623, 182, 542, 682, 36, 103, 691, 319, 267, 593, 647, 262, 942, 614, 674, 538, 211, 649, 114, 741, 418, 285, 434, 134, 898, 922, 136, 121, 856, 312, 899, 479, 845, 440, 160, 881, 542, 202, 551, 161, 794, 197, 422, 87, 162, 447, 976, 372, 447, 441, 464, 864, 725, 898, 997, 974, 171, 133, 94, 27, 796, 345, 857, 640, 784, 368, 520, 678, 569, 422, 838, 714, 970, 259, 800, 131, 57, 127, 855, 503, 567, 318, 367, 291, 215, 715, 265, 738, 199, 710, 116, 994, 54, 972, 634, 190, 691, 505, 867, 259, 927, 704, 324, 248, 314, 123, 379, 370, 601, 233, 224, 167, 902, 942, 809, 469, 657, 73, 206, 207, 135, 321, 201, 540, 644, 186, 81, 334, 42, 947, 944, 320, 2, 267, 568, 315, 741, 298, 36, 341, 530, 612, 859, 783, 553, 667, 251, 561, 92, 456, 768, 226, 776, 320, 117, 419, 857, 198, 104, 898, 496, 47, 218, 498, 665, 137, 164, 405, 434, 552, 97, 315, 163, 955, 97, 67, 622, 348, 628, 65, 155, 747, 642, 931, 418, 758, 701, 274, 307, 157, 171, 155, 203, 740, 652, 220, 228, 167, 976, 661, 718, 73, 327, 232, 379, 424, 299, 352, 123, 278, 416, 629, 376, 57, 559, 793, 167, 612, 418, 473, 768, 940, 627, 322, 32, 630, 541, 259, 797, 517, 272, 866, 941, 598, 450, 319, 373, 100, 671, 847, 729, 86, 476, 104, 495, 386, 248, 661, 349, 665, 485, 468, 604, 464, 790, 635, 93, 682, 246, 241, 550, 517, 459, 842, 466, 908, 161, 839, 7, 183, 37, 735, 620, 512, 190, 114, 250, 789, 126, 598, 453, 611, 66, 408, 74, 207, 395, 518, 888, 992, 759, 790, 508, 569, 631, 325, 476, 143, 515, 834, 677, 552, 920, 297, 415, 109, 762, 664, 249, 888, 614, 701, 850, 679, 460, 923, 885, 206, 440, 124, 197, 550, 265, 56, 118, 248, 381, 593, 390, 895, 778, 67, 798, 697, 715, 213, 157, 476, 228, 405, 715, 841, 457, 564, 871, 269, 838, 107, 474, 278, 583, 23, 179, 847, 78, 297, 94, 458, 241, 836, 705, 19, 902, 502, 67, 968, 66, 224, 795, 646, 980, 862, 838, 789, 777, 709, 57, 615, 167, 530, 244, 749, 552, 422, 948, 982, 70, 393, 791, 311, 228, 495, 329, 481, 349, 747, 448, 766, 322, 595, 411, 302, 456, 249, 90, 232, 309, 146,

};

	struct btree *btree;
/*
	get_random_array(keys, KEY_NUM, KEY_MAX);
*/
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
		puts("");
		printf("deleting %i\n", keys[i]);
		bt_btree_delete(btree, keys[i]);
		bt_print_btree(btree);
/*
*/
	}
	bt_print_btree(btree);
	return 0;
}

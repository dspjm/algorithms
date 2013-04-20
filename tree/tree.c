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

#define ARR_SIZE 20
#define ARR_RANGE 1000

enum color { RED = 0, BLACK = 1 };

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

struct rbt_node {
	int key;
	struct rbt_node *p;
	struct rbt_node *l;
	struct rbt_node *r;
	enum color c;
};

struct rbt {
	struct rbt_node *root;
	struct rbt_node *nil;
	struct rbt_node sentinel;
};

void rbt_init(struct rbt *rbt)
{
	rbt->nil = &rbt->sentinel;
	rbt->sentinel.key = 0;
	rbt->sentinel.p = rbt->sentinel.l = rbt->sentinel.r = rbt->nil;
	rbt->sentinel.c = BLACK;
	rbt->root = rbt->nil;
}

void rbt_rotate_right(struct rbt *t, struct rbt_node *tn)
{
	struct rbt_node *tmp, *tmp1;
	tmp = tn->p;
	tmp1 = tn->l;
	if (tmp == t->nil)
		t->root = tmp1;
	else if (tn == tmp->l)
		tmp->l = tmp1;
	else
		tmp->r = tmp1;
	tn->p = tmp1;
	tn->l = tmp1->r;
	if (tn->l != t->nil)
		tn->l->p = tn;
	tmp1->p = tmp;
	tmp1->r = tn;
	if (rbt_check_subtree(t, tn))
		printf("error\n");
}

void rbt_rotate_left(struct rbt *t, struct rbt_node *tn)
{
	struct rbt_node *tmp, *tmp1;
	tmp = tn->p;
	tmp1 = tn->r;
	if (tmp == t->nil)
		t->root = tmp1;
	else if (tn == tmp->l)
		tmp->l = tmp1;
	else
		tmp->r = tmp1;
	tn->p = tmp1;
	tn->r = tmp1->l;
	if (tn->r != t->nil)
		tn->r->p = tn;
	tmp1->p = tmp;
	tmp1->l = tn;
	if (rbt_check_subtree(t, tn))
		printf("error\n");
}

/* tmp is always supposed to be child one of the possible consecutive
   red nodes. tmp1 is it's uncle.
   There are three situations: tmp's uncle is red, tmp is a same kind
   of child as his parent(left or right), tmp is distinct kind of
   child */
/* In this function, we might change our uncle's, our parent's and our
   grandparent's color in case 1, nobodies color in case2, and 
   grandparent's and parent's color in case 3. We never change color of
   ourselves. In unusual situations, those nodes might be the nil node.
   However, changing the nil node's color is forbidden. So, what if 
   one of those nodes is the nil node.
   For every case, our parent must not be root or nil, because it must 
   be red. Though a root might be red, but it never appears as parent,
   only when tmp is root it's possible for root to be red. Since our
   parent can't be root, our grandparent can't be nil because only
   root can have its parent nil. So we don't need to be afraid to
   change nil's color when we are changing our parent or grandparent's
   color.
   For case 1,  our uncle can't be nil, because uncle's color must be red
   to enter this case.
   So, we never need to worry that we might change nil's color. */
/* We can't simply rotate and change color in case 2, because after 
   rotation, Our grandparent must be red, otherwise black height excceeds,
   and we are attached to our grandparent, conflicts again.*/
void rbt_insert_fix_balance(struct rbt *t, struct rbt_node *tn)
{
	struct rbt_node *tmp, *tmp1;
	tmp = tn;
	while (tmp->p->c == RED) {
		if (tmp->p == tmp->p->p->l) {
			tmp1 = tmp->p->p->r;
			if (tmp1->c == RED) {
				tmp->p->c = BLACK;
				tmp1->c = BLACK;
				tmp->p->p->c = RED;
				tmp = tmp->p->p;
			} else if (tmp == tmp->p->r) {
				tmp = tmp->p;
				rbt_rotate_left(t, tmp);
			} else {
				tmp->p->c = BLACK;
				tmp->p->p->c = RED;
				rbt_rotate_right(t, tmp->p->p);
			}
		} else {
			tmp1 = tmp->p->p->l;
			if (tmp1->c == RED) {
				tmp->p->c = BLACK;
				tmp1->c = BLACK;
				tmp1->p->c = RED;
				tmp = tmp1->p;
			} else if (tmp == tmp->p->l) {
				tmp = tmp->p;
				rbt_rotate_right(t, tmp);
			} else {
				tmp->p->p->c = RED;
				tmp->p->c = BLACK;
				rbt_rotate_left(t, tmp->p->p);
			}
		}
	}
	t->root->c = BLACK;
	if (rbt_check_subtree(t, tmp))
		printf("error\n");

}

int rbt_check_subtree(struct rbt *t, struct rbt_node *tn)
{
	int r;
	struct rbt_node *tmp, *tmpp, *tmpl, *tmpr;
	tmp = tn;
	if (tmp != t->nil) {
		tmpp = tmp->p;
		tmpl = tmp->l;
		tmpr = tmp->r;
		if (tmpp != t->nil && tmpp->l != tmp && tmpp->r != tmp)
			return 1;
		if (tmpl != t->nil && tmpl->p != tmp)
			return 2;
		if (tmpr != t->nil && tmpr->p != tmp)
			return 3;
		if (r = rbt_check_subtree(t, tmpl))
			return r;
		if (r = rbt_check_subtree(t, tmpr))
			return r;
	}
/*
	printf("key = %d checked\n", tn->key);
*/
	return 0;
}

struct rbt_node *alloc_init_rbt_node(struct rbt *t, int key)
{
	struct rbt_node *tmp;
	tmp = malloc(sizeof *tmp);
	tmp->key = key;
	tmp->p = t->nil;
	tmp->l = t->nil;
	tmp->r = t->nil;
	tmp->c = RED;
	return tmp;
}

void rbt_insert(struct rbt *t, int key)
{
	struct rbt_node *tmp, *tmp1, *tmp2;
	tmp2 = alloc_init_rbt_node(t, key);
	tmp = t->root;
	tmp1 = t->nil;
	while (tmp != t->nil) {
		tmp1 = tmp;
		if (key <= tmp1->key)
			tmp = tmp1->l;
		else
			tmp = tmp1->r;
	}
	if (tmp1 == t->nil)
		t->root = tmp2;
	else if (key <= tmp1->key) {
		tmp1->l = tmp2;
		tmp2->p = tmp1;
	} else {
		tmp1->r = tmp2;
		tmp2->p = tmp1;
	}
/*
	if (rbt_check_subtree(t, tmp2))
*/
	rbt_insert_fix_balance(t, tmp2);
}

struct rbt_node *rbt_subminimum(struct rbt *t, struct rbt_node *tn)
{
	while (tn->l != t->nil)
		tn = tn->l;
	return tn;
}

struct rbt_node *rbt_minimum(struct rbt *t)
{
	struct rbt_node *tmp;
	tmp = t->root;
	return  rbt_subminimum(t, tmp);
}

struct rbt_node *rbt_successor(struct rbt *t, struct rbt_node *tn)
{
	struct rbt_node *tmp, *tmp1;
	if (tn->r != t->nil)
		return rbt_subminimum(t, tn->r);
	tmp = tn;
	tmp1 = tn->p;
	while (tmp1 != t->nil && tmp == tmp1->r) {
		tmp = tmp1;
		tmp1 = tmp1->p;
	}
	return tmp1;
}

/* some discussion about red black tree
   In the function, tmp is sibling of tn, tn is the node which the 
   contradictions occur. which means his parent has one more black height
   from his sibling, which is the initial black height before delete.
   What we are tring to do is to get one extra node to the branch tn is in,
   while we must maintain sibling's branch's black height, or we reduce our
   sibling one black height and let our parent be new tn. Since if tn is red,
   we just need to change it's color and problem would solve, so we don't 
   really consider this situation.
   Ihe fix balance process contains four cases:
   1. if our sibling is red(our parent must be black), we just need to rotate
      once and let sibling be new parent, alternate parent's and sibling's
      color, and we have got a new red node on our branch, however, things
      are not that simple, we get a child from our sibling to be our new
      sibling, and conflicts happens again, and we go to case 2 with a red
      parent(if the new parent is not red, then we would end up an infinite
      loop) or case 3, 4
      what case 1 really do is go get a red node from sibling branch and then
      it's sure that we don't need to touch our forbearance to resolve the
      conflict. If new sibling don't have a red child, we just need to set
      our new sibling red, and conflict resolved. if he has a red child,
      then we just transfer it to our side through case 3 and 4 and get a new
      black node. But we are sure that our original has two non-nil children,
      so as our new sibling. Because we just delete a black node and I am a
      black node and original sibling is red, which means he must have at
      least two levels of bljust delete a black node and I am a
      black node and original sibling is red, which means he must have at
      least two levels of black nodes.
      Case 1 is special because our sibling is red, so he's child has same
      black heigh as himself, so, we can't get just rotate him to get a new
      black node in our branch
   2. if our sibling is black and both his children is black, we enter case 2,
      if our parent is black, we set our sibling to red and let our parent to
      be new tn, we must get our tree balance and get out of the loop. Even
      if all our forbearances were black, the tn would finally passed to root,
      then problem would be solved, because root is child of nobody, if his
      children have same black height, the whole tree is balanced
   3. if our sibling is black and his child which has an opposite direction
      to his parent(if sibling is right child of his parent, this would be 
      sibling's left child, vice versa) is white and the one in the same
      direction is black, then we enter case 3.  we change that nephew to 
      be our new sibling and let our previous sibling be his child and 
      interchange their color. This case is actually a preperation for case 4. 
      why do we need another case? because it's very inconvenient to directly
      set those relationships, if we divide it into two steps, case4 just
      need to rotate. Conclusively, this methos simplify the process and code.
   4. if our sibling is black and his child which has same direction with him
      is red, then we enter case 4. We would rotate tn's parent and let it be
      black and the new parent would have new the old parent's original color.
*/
void rbt_delete_fix_balance(struct rbt *t, struct rbt_node *tn)
{
	struct rbt_node *tmp, *tmpp, *tmpl, *tmpr;
	while (tn != t->root && tn->c == BLACK) {
		if (tn == tn->p->l) {
			tmp = tn->p->r;
			if (tmp->c == RED) {
				tmp->c = BLACK;
				tn->p->c = RED;
				rbt_rotate_left(t, tn->p);
			} else if (tmp->l->c == BLACK && tmp->r->c == BLACK) {
				if (tmp = t->nil)
				tmp->c = RED;
				tn = tmp->p;
			} else if (tmp->l->c == RED && tmp->r->c == BLACK) {
				/*
				tmpp = tmp->p;
				tmpl = tmp->l;
				tmp->l = tmpl->r;
				tmp->l->p = tmp;
				tmpp->r = tmpl;
				tmpl->p = tmpp;
				tmpl->r = tmp;
				tmp->p = tmpl;
				*/
				tmp->c = RED;
				tmp->l->c = BLACK;
				rbt_rotate_right(t, tmp);
			} else {
				tmp->c = tmp->p->c;
				tmp->p->c = BLACK;
				tmp->r->c = RED;
				rbt_rotate_left(t, tn->p);
				tn = t->root;
			}
		} else {
			tmp = tn->p->l;
			if (tmp->c == RED) {
				tmp->c = BLACK;
				tmp->p->c = RED;
				rbt_rotate_right(t, tn->p);
			} else if (tmp->l->c == BLACK && tmp->r->c == BLACK) {
				if (tmp = t->nil)
				tmp->c = RED;
				tn = tmp->p;
			} else if (tmp->l->c == BLACK && tmp->r->c == RED) {
				/*
				tmpp = tmp->p;
				tmpr = tmp->r;
				tmp->r = tmpr->l;
				tmp->r->p = tmp;
				tmpp->l = tmpr;
				tmpr->p = tmpp;
				tmpr->l = tmp;
				tmp->p = tmpr;
				*/
				tmp->c = RED;
				tmp->r->c = BLACK;
				rbt_rotate_left(t, tmp);
			} else {
				tmp->c = tmp->p->c;
				tmp->l->c = BLACK;
				tmp->p->c = BLACK;
				rbt_rotate_right(t, tn->p);
				tn = t->root;
			}
		}
	}
	tn->c = BLACK;
}

void rbt_delete(struct rbt *t, struct rbt_node *tn)
{
	struct rbt_node *tmp, *tmp1, *tmp2;
	if (tn->r == t->nil || tn->l == t->nil)
		tmp = tn;
	else
		tmp = rbt_successor(t, tn);
	if (tmp->l != t->nil)
		tmp1 = tmp->l;
	else
		tmp1 = tmp->r;
	tmp2 = tmp->p;
	tmp1->p = tmp2;
	if (tmp2 == t->nil)
		t->root = tmp1;
	else if (tmp == tmp2->l)
		tmp2->l = tmp1;
	else
		tmp2->r = tmp1;
	if (tmp->c == BLACK)
		rbt_delete_fix_balance(t, tmp1);
	if (tmp != tn) {
		if (tn->p == t->nil)
			t->root = tmp;
		else if (tn == tn->p->l)
			tn->p->l = tmp;
		else
			tn->p->r = tmp;
		tmp->p = tn->p;
		tmp->l = tn->l;
		tmp->r = tn->r; tmp->c = tn->c;
		if (tn->l != t->nil)
			tn->l->p = tmp;
		if (tn->r != t->nil)
			tn->r->p = tmp;
	}
	free(tn);
}

int rbt_check_pointer(struct rbt *t)
{
	rbt_check_subtree(t, t->root);
	return 0;
}

void rbt_print(struct rbt *rbt)
{
	int i = 0;
	struct rbt_node *tmp;
	tmp = rbt_minimum(rbt);
	printf("red black tree:\n");
	while (tmp != rbt->nil) {
		printf("%d ", tmp->key);
		i++;
		tmp = rbt_successor(rbt, tmp);
	}
	printf("\n%d nodes printed\n", i);
}

int main(int argc, char **argv)
{
	int i;
	int ret;
	int tmp[ARR_SIZE] = {416, 743, 862, 203, 394, 831, 197, 299, 525, 368, 535, 814, 835, 168, 223, 482, 960, 6, 210, 165,};
	struct bst bst;
	struct bst_node *tmp1;
	struct rbt rbt;
	struct rbt_node *tmp2;
/*
	get_random_array(tmp, ARR_SIZE, ARR_RANGE);
*/
	print_array(tmp, ARR_SIZE, "original array");
/*
	init_bst(&bst);
	for (i = 0; i < ARR_SIZE; i++)
		bst_insert(&bst, tmp[i]);
	bst_print(&bst);
	bst_check(&bst);
	for (i = 0; i < ARR_SIZE; i++) {
		tmp1 = bst_minimum(&bst);
		bst_delete(&bst, tmp1);
		bst_check(&bst);
	}
	bst_print(&bst);
*/
	rbt_init(&rbt);
	for (i = 0; i < ARR_SIZE; i++) {
		rbt_insert(&rbt, tmp[i]);
		if (ret = rbt_check_pointer(&rbt)) {
			printf("error = %d\n", ret);
			exit(1);
		} else
			printf("i = %d check passed\n", i);
/*
		rbt_print(&rbt);
*/
	}
	for (i = 0; i < ARR_SIZE; i++) {
		tmp2 = rbt_minimum(&rbt);
		rbt_delete(&rbt, tmp2);
/*
		printf("%d deleted\n", tmp2->key);
*/
		rbt_print(&rbt);
	}
	return 0;
/*
*/
}

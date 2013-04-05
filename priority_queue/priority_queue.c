/* priority_queue.c.c
 *
 * Author: Jimmy Pan
 * Email: dspjmt@gmail.com
 * Date: Thu Apr  4 11:27:01 CST 2013
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
#include <limits.h>
#include <string.h>
#include <time.h>

#define BUF_SIZE 1024
#define MAX_HEAP_SIZE 1024
#define RAND_KEY_MOD 4096
#define RAND_INS_KEY_NUM 200


int by_pass_space(char *buf, int *i)
{
	int ret = 0;
	for (; isspace(buf[*i]); (*i)++, ret++)
		;
	return ret;
}

int get_argument(char *buf, int *i, int *x)
{
	char tmp[BUF_SIZE] = {};
	int tmp1 = 0, ret;
	/* check argument legality and set \n to \0*/
	if (buf[*i] == '+' || buf[*i] == '-') {
		tmp[tmp1] = buf[*i];
		(*i)++;
		tmp1++;
	}
	for (; ; (*i)++, tmp1++) {
		if (isdigit(buf[*i]))
			tmp[tmp1] = buf[*i];
		else if (isspace(buf[*i])) {
			tmp[tmp1] = '\0';
			break;
		} else
			return 2; /*illegal argument*/
	}
	*x = atoi(tmp);
	return 0;
}

int scrutinize_cmd(char *buf, char *cmd, int *x, int *y)
{
	int i = 0;
	int tmp = 0;
	int ret = 0;
	by_pass_space(buf, &i);
	if (isalpha(buf[i])) {
		*cmd = buf[i];
		i++;
		ret++;
	} else
		return ret;
	if (!by_pass_space(buf, &i))
		return ret;
	if (get_argument(buf, &i, x))
		return ret;
	ret++;
	if (!by_pass_space(buf, &i))
		return ret;
	if (get_argument(buf, &i, y))
		return ret;
	ret++;
	return ret;
}

void print_help()
{
	printf("Available Commands:\n");
	printf("    h: invoke this help information\n");
	printf("    q: quit\n");
	printf("    p: print heap\n");
	printf("    r: insert many random keys\n");
	printf("    m: print maximum in heap\n");
	printf("    e: extract maximum in heap\n");
	printf("    i <ingeter>:  insert value\n");
	printf("    u <index> <key>: increase index key\n");
}

void max_heapify(int *heap, int heap_size, int index)
{
	int l, r, max;
	int tmp;
	l = index << 1;
	r = l + 1;
	max = index;
	if (l <= heap_size && heap[l - 1] > heap[max - 1])
		max = l;
	if (r <= heap_size && heap[r - 1] > heap[max - 1])
		max = r;
	if (max != index) {
		tmp = heap[index - 1];
		heap[index - 1] = heap[max - 1];
		heap[max - 1] = tmp;
		max_heapify(heap, heap_size, max);
	}
}

int heap_maximum(int *heap)
{
	return heap[0];
}

/*return max value of heap, -1 on error*/
int heap_extract_max(int *heap, int *heap_size)
{
	int max;
	if (*heap_size < 1) {
		fputs("heap underflow\n", stderr);
		return -1;
	}
	max = heap[0];
	heap[0] = heap[*heap_size - 1];
	(*heap_size)--;
	max_heapify(heap, *heap_size, 1);
	return max;
}

int heap_increase_key(int *heap, int index, int key)
{
	int parent;
	int tmp;
	if (key < heap[index - 1])
		return -1;
	heap[index - 1] = key;
	parent = index / 2;
	while (index > 1 && heap[index - 1] > heap[parent - 1]) {
		tmp = heap[parent - 1];
		heap[parent - 1] = heap[index - 1];
		heap[index - 1] = tmp;
		index = parent;
		parent = index / 2;
	}
	return 0;
}

int insert_into_heap(int *heap, int *heap_size, int x)
{
	if (*heap_size >= MAX_HEAP_SIZE)
		return -1;
	(*heap_size)++;
	heap[*heap_size - 1] = INT_MIN;
	if (heap_increase_key(heap, *heap_size, x)) {
		fputs("failed to increase key", stderr);
		return -2;
	}
	return 0;
}

void random_insert(int  *heap, int *heap_size)
{
	unsigned int seed;
	int tmp, i;
	if (*heap_size + RAND_INS_KEY_NUM > MAX_HEAP_SIZE) {
		fprintf(stderr, "exceeds max heap size when inserting %d"
		  " random keys", RAND_INS_KEY_NUM);
		return;
	}
	time((time_t *)&seed);
	srand(seed);
	for (i = 0; i < RAND_INS_KEY_NUM; i++) {
		tmp = rand() % RAND_KEY_MOD;
		insert_into_heap(heap, heap_size, tmp);
	}
}

void print_heap(int *heap, int heap_size)
{
	int i;
	printf("**************************\n");
	printf("heap(size = %d):\n", heap_size);
	for (i = 0; i < heap_size; i++) {
		printf("%d ", heap[i]);
	}
	printf("\n**************************\n");
}

int main(int arc, char **argv)
{
	int x, y, ret, legal_cmd, tmp;
	char cmd = 0;
	char cmd_buf[BUF_SIZE] = {};
	int heap[MAX_HEAP_SIZE] = {};
	int heap_size = 0;
	printf("Please input commands(h for help):\n");
	while (1) { memset(cmd_buf, 0, BUF_SIZE);
		cmd = 0;
		x = 0;
		y = 0;
		legal_cmd = 0;
		printf("]: ");
		if (fgets(cmd_buf, BUF_SIZE, stdin) == NULL)
			fputs("error reading input\n", stderr);
		legal_cmd = scrutinize_cmd(cmd_buf, &cmd, &x, &y);
		switch (cmd) {
		case 'h':
			print_help();
			break;
		case 'i':
			if (legal_cmd < 2) {
				fputs("insufficient legal arguments\n", stderr);
				break;
			}
			ret = insert_into_heap(heap, &heap_size, x);
			if (ret == -1)
				fputs("exceeds maximum heap size\n", stderr);
			break;
		case 'u':
			if (legal_cmd < 3) {
				fputs("insufficient legal arguments\n", stderr);
				break;
			}
			if (heap_increase_key(heap, x, y))
				fputs("key value is less than original\n", stderr);
			break;
		case 'r':
			random_insert(heap, &heap_size);
			break;
		case 'm':
			tmp = heap_maximum(heap);
			printf("maximum number in heap: %d\n", tmp);
			break;
		case 'e':
			tmp = heap_extract_max(heap, &heap_size);
			if (!tmp)
				printf("maximum number in heap: %d\n", tmp);
			break;
		case 'p':
			print_heap(heap, heap_size);
			break;
		case 'q':
			exit(0);
		default:
			fputs("invalid command\n", stderr);
		}
	}
error:
	exit(1);
}

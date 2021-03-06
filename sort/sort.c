/* sort.c
 *
 * Author: Jimmy Pan
 * Email: dspjmt@gmail.com
 * Date: Mon Apr  6 20:50:02 CST 2013
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#define NUM_CNT 10
#define RANDOM_RANGE 2000 /*actual range is 0 to 1999*/
#define BUCKET_NUM 100

/*#define DEB_IND*/
#ifdef DEB_IND
#define DEB_PRINT(x) printf(#x" = %d\n", x)
#else
#define DEB_PRINT
#endif




void set_seed()
{
	long seed;
/*
	FILE *fp;
	fp = fopen("/dev/random", "r");
	fread(&seed, sizeof seed, 1, fp);
*/
	time((time_t *)&seed);
	srand(seed);
}

void init_array(int *a)
{
	int i;
	for (i = 0; i < NUM_CNT; i++) {
		a[i] = rand() % RANDOM_RANGE;
	}
}

void print_array(int *a)
{
	int i;
	printf("array is: \n");
	for (i = 0; i < NUM_CNT; i++) {
		printf(" %d", a[i]);
	}
	printf("\n");
}

void insertion_sort_array(int *a)
{
	int i, j, tmp;
	for (j = 1; j < NUM_CNT; j++) {
		for (i = j; i > 0; i--) {
			if (a[i] < a[i - 1]) {
				tmp = a[i];
				a[i] = a[i - 1];
				a[i - 1] = tmp;
			} else
				break;
		}
	}
}

void merge_subarray(int *a, int *s1, int *s2, int size1, int size2)
{
	int i, j, k;
	for (i = 0, j = 0, k = 0; i < size1 && j < size2; k++) {
		if (s1[i] > s2[j]) {
			a[k] = s2[j];
			j++;
		} else {
			a[k] = s1[i];
			i++;
		}
	}
	if (i >= size1)
		for (; j < size2; j++, k++)
			a[k] = s2[j];
	else
		for (; i < size1; i++, k++)
			a[k] = s1[i];
}

void merge_sort_subarray(int *a, int start, int end, int *b)
{
	int mid;
	int *b1, *b2;
	if (start == end) {
		*b = a[start];
		return;
	}
	mid = (start + end) / 2;
	b1 = malloc((mid - start + 1) * sizeof *b1);
	b2 = malloc((end - mid) * sizeof *b2);
	merge_sort_subarray(a, start, mid, b1);
	merge_sort_subarray(a, mid + 1, end, b2);
	merge_subarray(b, b1, b2, (mid - start + 1), (end - mid));
	free(b1);
	free(b2);
}

void merge_sort(int *a, int *b)
{
	merge_sort_subarray(a, 0, NUM_CNT - 1, b);
}

void permute_array_with_insertion_sort(int *ori, int *ref)
{
	int i, j;
	int tmp, tmp1;
	for (j = 1; j < NUM_CNT; j++) {
		tmp = ref[j];
		tmp1 = ori[j];
		for (i = j - 1; i >= 0; i--) {
			if (tmp < ref[i]) {
				ref[i + 1] = ref[i];
				ref[i] = tmp;
				ori[i + 1] = ori[i];
				ori[i] = tmp1;
			} else
				break;
		}
	}
}

void permute_merge_subarrays(int *ori_d, int *ref_d, int *o_sub1, int *o_sub2,
  int *r_sub1, int *r_sub2, int size1, int size2)
{
	int i, j, k;
	for (i = 0, j = 0, k = 0; i < size1 && j < size2; k++) {
		if (r_sub1[i] < r_sub2[j]) {
			ref_d[k] = r_sub1[i];
			ori_d[k] = o_sub1[i];
			i++;
		} else {
			ref_d[k] = r_sub2[j];
			ori_d[k] = o_sub2[j];
			j++;
		}
	}
	for (; i < size1; i++, k++) {
		ref_d[k] = r_sub1[i];
		ori_d[k] = o_sub1[i];
	}
	for (; j < size2; j++, k++) {
		ref_d[k] = r_sub2[j];
		ori_d[k] = o_sub2[j];
	}
}

void permute_merge_sort_subarray(int *ori_buf, int *ref_buf, int *ori_src,
  int *ref_src, int size)
{
	int half = size / 2;
	int tmp1[half];
	int tmp2[half];
	int tmp3[size - half];
	int tmp4[size - half];
	if (size == 1) {
		*ori_buf = *ori_src;
		*ref_buf = *ref_src;
		return;
	}
	permute_merge_sort_subarray(tmp1, tmp2, ori_src, ref_src, half);
	permute_merge_sort_subarray(tmp3, tmp4, ori_src + half, ref_src + half,
	  size - half);
	permute_merge_subarrays(ori_buf, ref_buf, tmp1, tmp2, tmp3, tmp4, half,
	  size - half);
}

void permute_array_by_merge_sort(int *ori_permuted, int *ori, int *ref)
{
	int tmp[NUM_CNT];
	permute_merge_sort_subarray(ori_permuted, tmp, ori, ref, NUM_CNT);
}

void permute_array_by_random_in_place(int *dest, int *src)
{
	int i;
	int tmp, tmp1;
	set_seed();
	for (i = 0; i < NUM_CNT - 1; i++) {
		tmp = rand() % (NUM_CNT - i);
		tmp1 = src[i];
		src[i] = src[i + tmp];
		src[i + tmp] = tmp1;
		dest[i] = src[i];
	}
	dest[i] = src[i];
}

/*all indices start from one when not accessing array*/
void min_heapify(int *arr, int index, int heap_size)
{
	int l, r, tmp, tmp1;
	l = index << 1;
	r = l + 1;
	tmp = index;
	if (l <= heap_size && arr[l - 1] < arr[tmp - 1])
		tmp = l;
	if (r <= heap_size && arr[r - 1] < arr[tmp - 1])
		tmp = r;
	if (tmp != index) {
		tmp1 = arr[index - 1];
		arr[index - 1] = arr[tmp - 1];
		arr[tmp - 1] = tmp1;
		min_heapify(arr, tmp, heap_size);
	}
}

/*all indices start from one when not accessing array*/
void max_heapify(int *arr, int index, int heapsize)
{
	int l, r, max, tmp;
	l = index << 1;
	r = l + 1;
	max = index;
	if (l <= heapsize && arr[max - 1] < arr[l - 1])
		max = l;
	if (r <= heapsize && arr[max - 1] < arr[r - 1])
		max = r;
	if (max != index) {
		tmp = arr[index - 1];
		arr[index - 1] = arr[max - 1];
		arr[max - 1] = tmp;
		max_heapify(arr, max, heapsize);
	}
}

/*all indices start from one when not accessing array*/
void heap_sort(int *arr)
{
	int tmp, i, tmp1;
	int heap_size;
	heap_size = NUM_CNT;
	/*build heap*/
	tmp = heap_size / 2;
	for (i = tmp; i > 0; i--) {
		max_heapify(arr, i, heap_size);
	}
	/*sort array*/
	for (i = heap_size; i >= 2; i--) {
		tmp1 = arr[0];
		arr[0] = arr[i - 1];
		arr[i - 1] = tmp1;
		heap_size--;
		max_heapify(arr, 1, heap_size);
	}
}

void quick_subsort(int *a, int start, int end)
{
	int i, j, p, tmp;
	if (start >= end)
		return;
	p = a[end - 1];
	for (i = start - 1, j = start; j < end; j++) {
		if (a[j - 1] < p) {
			i++;
			tmp = a[i - 1];
			a[i - 1] = a[j - 1];
			a[j - 1] = tmp;
		}
	}
	tmp = a[j - 1];
	a[j - 1] = a[i];
	a[i] = tmp;
	quick_subsort(a, start, i - 1);
	quick_subsort(a, i + 1, end);
}

void quick_sort(int *a)
{
	quick_subsort(a, 1, NUM_CNT);
}

void random_quick_subsort(int *a, int start, int end)
{
	int tmp, tmp1, i, j, p;
	if (start >= end)
		return;
	tmp = rand() % (end - start + 1);
	tmp += start;
	tmp1 = a[end - 1];
	a[end - 1] = a[tmp - 1];
	a[tmp - 1] = tmp1;
	p = a[end - 1];
	for (j = start, i = start - 1; j < end; j++) {
		if (a[j - 1] < p) {
			i++;
			tmp1 = a[i - 1];
			a[i - 1] = a[j - 1];
			a[j - 1] = tmp1;
		}
	}
	tmp1 = a[i];
	a[i] = a[j - 1];
	a[j - 1] = tmp1;
	random_quick_subsort(a, start, i - 1);
	random_quick_subsort(a, i + 1, end);
}

void random_quick_sort(int *a)
{
	random_quick_subsort(a, 1, NUM_CNT);
}

void counting_sort(int *a, int *b)
{
	int i, j;
	int tmp[RANDOM_RANGE] = {};
	for (i = 0; i < NUM_CNT; i++)
		tmp[a[i]]++;
	for (j = 1; j < RANDOM_RANGE; j++)
		tmp[j] += tmp[j - 1];
	for (i = NUM_CNT - 1; i >= 0; i--)
		b[--tmp[a[i]]] = a[i];    /*take notice here*/
}

int get_masked(int num, int mask)
{
	return num & mask;
}

/* in place quick sort is intrinsically unstable, don't use it.
void radix_quick_subsort(int *a, int start, int end, int mask)
{
	int p, tmp, i, j;
	if (start >= end)
		return;
	p = a[end - 1] & mask;
	printf("radix_quick_subsorting from %d to %d with mask %x and pivot %x\n", start, end, mask, p);
	for (j = start, i = j - 1; j < end; j++) {
		if (get_masked(a[j - 1], mask) < p) {
			printf("masked a[%d] = %x\n", j, a[j - 1]);
			i++;
			tmp = a[j - 1];
			a[j - 1] = a[i - 1];
			a[i - 1] = tmp;
		}
	}
	tmp = a[i];
	a[i] = a[end - 1];
	a[end - 1] = tmp;
	radix_quick_subsort(a, start, i, mask);
	radix_quick_subsort(a, i + 2, end, mask);
}

void radix_quick_sort(int *a, int i)
{
	int mask;
	mask = 0xf << i * 4;
	radix_quick_subsort(a, 1, NUM_CNT, mask);
}

void radix_sort_on_quick_sort(int *a, int base_cnt)
{
	int i;
	for (i = 0; i < base_cnt; i++) {
		radix_quick_sort(a, i);
		printf("after %dth sort:\n", i + 1);
		print_array(a);
	}
}
*/

void radix_merge_splice(int *a, int start, int mid, int end, int mask)
{
	int i, j, k;
	int tmp1[mid - start + 1], tmp2[end - mid];
	int tmp1len = mid - start + 1, tmp2len = end - mid;
	for (j = 0; j < tmp1len; j++)
		tmp1[j] = a[start - 1 + j];
	for (k = 0; k < tmp2len; k++)
		tmp2[k] = a[mid + k];
	for (i = start - 1, j = 0, k = 0; j < tmp1len && k < tmp2len; i++) {
		if ((tmp1[j] & mask) <= (tmp2[k] & mask)) {
			a[i] = tmp1[j];
			j++;
		} else {
			a[i] = tmp2[k];
			k++;
		}
	}
	if (j < tmp1len)
		for (; j < tmp1len; j++, i++)
			a[i] = tmp1[j];
	else if (k < tmp2len)
		for (; k < tmp2len; k++, i++)
			a[i] = tmp2[k];
		
}

void radix_merge_subsort(int *a, int start, int end, int mask)
{
	int m = (start + end) / 2;
	if (start >= end)
		return;
	radix_merge_subsort(a, start, m, mask);
	radix_merge_subsort(a, m + 1, end, mask);
	radix_merge_splice(a, start, m, end, mask);
}

void radix_sort_on_merge_sort(int *a, int base_num)
{
	int i, mask;
	for (i = 0; i < base_num; i++) {
		mask = 0xf << 4 * i;
		radix_merge_subsort(a, 1, NUM_CNT, mask);
	}
}

struct b_node {
	double value;
	struct b_node *next;
};

void bucket_sort_init(double *buf, int *ori)
{
	int i;
	for (i = 0; i < NUM_CNT; i++) 
		buf[i] = (double)ori[i] / RANDOM_RANGE;
}

void add_bucketnode(double n, struct b_node **bpp)
{
	struct b_node *tmp;
	tmp = malloc(sizeof *tmp);
	tmp->value = n;
	tmp->next = NULL;
	while (*bpp != NULL)
		bpp = &(*bpp)->next;
	*bpp = tmp;
}

void insertion_sort_bucket(struct b_node **bpp)
{
	struct b_node *cbp, *pcbp, **pbpp, *ibp;
	if (*bpp == NULL || (*bpp)->next == NULL)
		return;
	pcbp = *bpp;
	cbp = pcbp->next;
	while (cbp != NULL) {
		pbpp = bpp;
		ibp = (*pbpp)->next;
		while (ibp != cbp) {
			if (ibp->value > cbp->value) {
				pcbp->next = cbp->next;
				cbp->next = ibp;
				*pbpp = cbp;
			} else {
				pbpp = &(*pbpp)->next;
				ibp = (*pbpp)->next;
			}
		}
		pcbp = pcbp->next;
		cbp = pcbp->next;
	}
}

void free_buckets(struct b_node **arr, int size)
{
	int i;
	struct b_node *tmp;
	for (i = 0; i < size; i++)
		for (tmp = arr[i]; tmp; tmp = tmp->next)
			free(tmp);
}

void bucket_sort(double *a)
{
	int i, j, bucket_index;
	struct b_node *buckets[BUCKET_NUM] = {}, *tmp;
	for (i = 0; i < NUM_CNT; i++) {
		bucket_index = a[i] * BUCKET_NUM;
		add_bucketnode(a[i], buckets + bucket_index);
	}
	for (i = 0; i < BUCKET_NUM; i++) {
		insertion_sort_bucket(buckets + i);
	}
	for (i = 0, j = 0; i < BUCKET_NUM; i++) {
		tmp =  buckets[i];
		for (; tmp; tmp = tmp->next, j++)
			a[j] = tmp->value;
	}
	free_buckets(buckets, BUCKET_NUM);
}

void print_double_array(double *buf, int size)
{
	int i;
	printf("double array is:\n");
	for (i = 0; i < size; i++) {
		printf("%g ", buf[i]);
	}
	printf("\n");
}
void bucket_sort_restore(double *a)
{
	int i;
	for (i = 0; i < NUM_CNT; i++) {
		a[i] *= RANDOM_RANGE;
	}
}

int main(int argc, char **argv)
{
	int tmp1[NUM_CNT];    /*random array*/
	int tmp2[NUM_CNT];    /*sorted array*/
	int tmp3[NUM_CNT];    /*permuted array*/
	double tmp4[NUM_CNT];    /*array for bucket sort*/
	set_seed();
	memset(tmp2, 0, NUM_CNT * sizeof *tmp2);
	init_array(tmp1);
	print_array(tmp1);
/*
	insertion_sort_array(tmp1);
*/
	merge_sort(tmp1, tmp2);
	print_array(tmp2);
/*
	permute_array_by_merge_sort(tmp3, tmp2, tmp1);
*/
	permute_array_by_random_in_place(tmp3, tmp2);
	print_array(tmp3);
/*
	heap_sort(tmp3);
	quick_sort(tmp3);
	random_quick_sort(tmp3);
	counting_sort(tmp3, tmp2);
	radix_sort_on_merge_sort(tmp3, 4);
*/
	bucket_sort_init(tmp4, tmp3);
	print_double_array(tmp4, NUM_CNT);
	bucket_sort(tmp4);
	print_double_array(tmp4, NUM_CNT);
	bucket_sort_restore(tmp4);
	print_double_array(tmp4, NUM_CNT);
	return 0;
}

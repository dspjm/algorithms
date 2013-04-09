/* selection.c
 *
 * Author: Jimmy Pan
 * Email: dspjmt@gmail.com
 * Date: Mon Apr  8 20:50:02 CST 2013
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
#include <time.h>
#include <limits.h>

#define NUM_CNT 30
#define RAND_MOD 3000

void set_seed()
{
	long tmp;
	time(&tmp);
	srand(200);
}

void get_random_array(int *a, int size)
{
	int i;
	for (i = 0; i < size; i++)
		a[i] = rand() % RAND_MOD;
}

void print_array(int *a, int size)
{
	int i;
	printf("array is:\n");
	for (i = 0; i < size; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");
}

void max_heapify(int *a, int i, int size)
{
	int l, r, m, tmp;
	l = i << 1;
	r = l + 1;
	m = i;
	if (l <= size && a[m - 1] < a[l - 1])
		m = l;
	if (r <= size && a[m - 1] < a[r - 1])
		m = r;
	if (m != i) {
		tmp = a[m - 1];
		a[m - 1] = a[i - 1];
		a[i - 1] = tmp;
		max_heapify(a, m, size);
	}
}

void heap_sort(int *a, int size)
{
	int i;
	int tmp;
	for (i = size / 2; i > 0; i--)
		max_heapify(a, i, size);
	print_array(a, size);
	for (; size > 0;size--) {
		tmp = a[size - 1];
		a[size - 1] = a[0];
		a[0] = tmp;
		max_heapify(a, 1, size - 1);
	}
}

void select_min_and_max(int *a, int size, int *min, int *max)
{
	int i;
	if (!size) {
		*min = 0;
		*max = 0;
		return;
	}
	if (size % 2) {
		*min = a[0];
		*max = a[0];
		i = 1;
	 } else {
		if (a[0] < a[1]) {
			*min = a[0];
			*max = a[1];
		} else {
			*min = a[1];
			*max = a[0];
		}
		i = 2;
	}
	for (; i < size - 1; i += 2) {
		if (a[i] > a[ i + 1]) {
			*max = *max > a[i] ? *max : a[i];
			*min = *min < a[i + 1] ? *min : a[i + 1];
		} else {
			*max = *max > a[i + 1] ? *max : a[i + 1];
			*min = *min < a[i] ? *min : a[i];
		}
	}
}

int randomized_partition(int *a, int start, int end)
{
	int tmp, tmp1, i, j, p;
	tmp = rand() % (end - start + 1);
	tmp += start;
	tmp1 = a[end - 1];
	a[end - 1] = a[tmp - 1];
	a[tmp - 1] = tmp1;
	p = a[end - 1];
	for (i = start, j = start - 1; i < end; i++) {
		if (a[i - 1] < p) {
			j++;
			tmp1 = a[j - 1];
			a[j - 1] = a[i - 1];
			a[i - 1] = tmp1;
		}
	}
	j++;
	tmp1 = a[j - 1];
	a[j - 1] = a[end - 1];
	a[end - 1] = a[j - 1];
	return j;
}

void select_ith_least(int *a, int size, int *result, int index)
{
	int start, end, k;
	if (index > size || index < 1)
		return;
	start = 1;
	end = size;
	while (k != index) {
		k = randomized_partition(a, start, end);
		if (start > end)
			return;
		if (index < k) {
			start = start;
			end = k - 1;
		} else {
			start = k + 1;
			end = end;
		}
	}
	*result = a[k];
}

void insertion_sort(int *a, int start, int end)
{
	int i, j, tmp;
	for (i = start + 1; i <= end; i++) {
		for (j = i; j > start && a[j - 1] < a[j - 2]; j--) {
			tmp = a[j - 1];
			a[j - 1] = a[j - 2];
			a[j - 2] = tmp;
		}
	}
}

int partition_array(int *a, int size, int p)
{
	int i, j, tmp;
	for (i = 1, j = 0; i <= size; i++) {
		if (a[i] < p) {
			j++;
			tmp = a[i - 1];
			a[i - 1] = a[j - 1];
			a[j - 1] = tmp;
		}
	}
	return j + 1;
}

void median_select(int *a, int index, int size, int *result)
{
	int tmp, tmp1, *tmp2, tmp3, tmp4;
	int i, j;
	if (!size)
		return;
	if (size == 1) {
		*result = a[0];
		return;
	}
	tmp = size % 5;
	tmp1 = size / 5;
	if (tmp)
		tmp1++;
	tmp2 = malloc(tmp1 * sizeof *tmp2);
	for (i = 1, j = 1; size - i + 1 > tmp; i += 5, j++) {
		insertion_sort(a, i, i + 4);
		tmp2[j - 1] = a[i + 1];
	}
	if (tmp) {
		insertion_sort(a, i, i + tmp - 1);
		tmp2[j - 1] = a[i + tmp / 2 - 1];
	}
	median_select(tmp2, tmp1 / 2, tmp1, &tmp3);
	tmp4 = partition_array(a, size, tmp3);
	if (tmp4 == index)
		*result = a[index - 1];
	else if (index < tmp4)
		median_select(a, index, tmp4 - 1, result);
	else if (index > tmp4)
		median_select(a + tmp4, index - tmp4, size - tmp4, result);
	free(tmp2);
}

int main(int argc, int **argv)
{
	int tmp[NUM_CNT];
	int tmp1[NUM_CNT];
	int tmpmin, tmpmax, tmpret;
	int tmp_index;
	set_seed();
	get_random_array(tmp1, NUM_CNT);
	memcpy(tmp1, tmp, NUM_CNT * sizeof(int));
	print_array(tmp, NUM_CNT);
	heap_sort(tmp1, NUM_CNT);
	print_array(tmp1, NUM_CNT);
/*
	select_min_and_max(tmp, NUM_CNT, &tmpmin, &tmpmax);
	printf("max = %d, min = %d\n", tmpmax, tmpmin);
*/
	tmp_index = rand() % NUM_CNT + 1;
/*
	select_ith_least(tmp, NUM_CNT, &tmpret, tmp_index);
*/
	median_select(tmp, tmp_index, NUM_CNT, &tmpret);
	printf("%dth least number selected: %d\n", tmp_index, tmpret);
	return 0;
}

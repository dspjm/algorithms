#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUM_CNT 300
#define RANDOM_RANGE 500




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
		printf(" %i", a[i]);
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

void permute_merge_subarrays(int *ori_d, int *ref_d, int *o_sub1, int *r_sub1,
  int *o_sub2, int *r_sub2, int start, int mid, int end)
{
	int i, j, size1, size2;
	size1 = mid - start + 1;
	size2 = end - mid;
	for (i = 0, j = 0; i < size1 && j < size2;) {
		
void permute_merge_sort_subarray(int *ori_d, int *ref_d, int *ori, int *ref,
  int start, int end)
{
	int mid = (start + end) / 2;
	int tmp1[mid + 1 - start];
	int tmp2[end - mid];
	int tmp3[mid + 1 - start];
	int tmp4[end - mid];
	permute_merge_sort_subarray(tmp1, tmp3, ori, ref, start, mid);
	permute_merge_sort_subarray(tmp2, tmp4, ori, ref, mid + 1, end);
	permute_merge_subarrays(ori_d, ref_d, tmp1, tmp3, tmp2, tmp4, start,
	  mid, end);
}

void permute_array_with_merge_sort(int *dest, int *ori, int *ref)
{
	int tmp[NUM_CNT];
	permute_merge_sort(dest, tmp, ori, ref, 0, NUM_CNT - 1);
}

int main(int argc, char **argv)
{
	int tmp1[NUM_CNT];
	int tmp2[NUM_CNT];
	int tmp3[NUM_CNT];
	memset(tmp2, 0, NUM_CNT * sizeof *tmp2);
	init_array(tmp1);
	print_array(tmp1);
/*
	insertion_sort_array(tmp1);
*/
	merge_sort(tmp1, tmp2);
	print_array(tmp2);
	permute_array_with_merge_sort(tmp3, tmp2, tmp1);
	print_array(tmp3);
	return 0;
}

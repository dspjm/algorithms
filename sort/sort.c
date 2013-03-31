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

int main(int argc, char **argv)
{
	int tmp1[NUM_CNT];
	int tmp2[NUM_CNT];
	memset(tmp2, 0, NUM_CNT * sizeof *tmp2);
	init_array(tmp1);
	print_array(tmp1);
/*
	insertion_sort_array(tmp1);
*/
	merge_sort(tmp1, tmp2);
	print_array(tmp2);
	return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUM_CNT 10
#define RANDOM_RANGE 500

/*#define DEB_IND*/
#ifdef DEB_IND
#define DEB_PRINT(x) printf(#x" = %d\n", x)
#else
#define DEB_PRINT
#endif




void set_seed()
{
	FILE *fp;
	unsigned int seed;
	fp = fopen("/dev/random", "r");
	fread(&seed, sizeof seed, 1, fp);
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

void min_heapify(int *arr, int index)
{
	int l, r, tmp;
	l = index << 1;
	r = l + 1;
	tmp = index;
	if (l <= NUM_CNT && arr[l] < arr[tmp])
		tmp = l;
	if (r <= heap_size && arr[r] < arr[tmp])
		tmp = r;
	if (tmp != index) {
		tmp1 = arr[index];
		arr[index] = arr[tmp];
		arr[tmp] = tmp1;
		min_heapify(arr, tmp);
	}
}

void heap_sort(int *arr)
{
}

int main(int argc, char **argv)
{
	int tmp1[NUM_CNT];   /*random array*/
	int tmp2[NUM_CNT];   /*sorted array*/
	int tmp3[NUM_CNT];   /*permuted array*/
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

	heap_sort(tmp3);
	
	print_array(tmp3);
	return 0;
}

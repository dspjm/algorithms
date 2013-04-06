#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define NUM_CNT 100
#define RANDOM_RANGE 2000 /*actual range is 0 to 1999*/

/*#define DEB_IND*/
#ifdef DEB_IND
#define DEB_PRINT(x) printf(#x" = %d\n", x)
#else
#define DEB_PRINT
#endif




void set_seed()
{
	unsigned int seed, seed1;
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
	printf("sorting from %d to %d\n", start, end);
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
	print_array(tmp3);
/*
	heap_sort(tmp3);
	quick_sort(tmp3);
	random_quick_sort(tmp3);
*/
	counting_sort(tmp3, tmp2);
	print_array(tmp2);
	return 0;
}

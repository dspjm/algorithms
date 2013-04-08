#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_CNT 200
#define RAND_MOD 3000

void set_seed()
{
	unsigned int tmp;
	time((time_t *)&tmp);
	srand(tmp);
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

int main(int argc, int **argv)
{
	int tmp[NUM_CNT];
	int tmpmin, tmpmax;
	get_random_array(tmp, NUM_CNT);
	print_array(tmp, NUM_CNT);
	select_min_and_max(tmp, NUM_CNT, &tmpmin, &tmpmax);
	printf("max = %d, min = %d\n", tmpmax, tmpmin);
	return 0;
}

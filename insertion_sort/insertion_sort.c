#include <stdlib.h>
#include <stdio.h>

#define NUM_CNT 100




void init_array(int *a)
{
	int i;
	for (i = 0; i < NUM_CNT; i++) {
		a[i] = rand() % 300;
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

void sort_array(int *a)
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

int main(int argc, char **argv)
{
	int tmp1[NUM_CNT];
	init_array(tmp1);
	print_array(tmp1);
	sort_array(tmp1);
	print_array(tmp1);
	return 0;
}

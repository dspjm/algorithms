/* matrix_chain.c
 *
 * Author: Jimmy Pan
 * Email: dspjmt@gmail.com
 * Date: Mon May 13 01:40:19 CST 2013
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
#include "algorithms.h"

#define MATRIX_NUM 20
#define MAX_LEN 15

int mc_mul_cost(int p, int q, int r)
{
	return p * r * q;
}

void mc_print_costs(int (*mcost)[MATRIX_NUM], int n)
{
	int i, j;
	printf("  j");
	for (j = 0; j < n; j++) {
		printf("\t%i", j);
	}
	printf("\ni\n");
	for (i = 0; i < n; i++) {
		printf("%i", i);
		for (j = 0; j < i; j++)
			printf("\t");
		for (j = i; j < n; j++) {
			printf("\t%i", mcost[i][j]);
		}
		printf("\n");
	}
}

void mc_compute_sub_optimal_cost(int *ml, int (*mcost)[20], int n, int i, int j)
{
	int p, q, r;
	int k = i, tmp;
	int min, min_pos;
	p = ml[i];
	q = ml[k + 1];
	r = ml[j + 1];
	min = mc_mul_cost(p, q, r) + mcost[i][k] + mcost[k + 1][j];
	min_pos = k;
	for (k++; k <= j; k++) {
		p = ml[i];
		q = ml[k + 1];
		r = ml[j + 1];
		tmp = mc_mul_cost(p, q, r) + mcost[i][k] + mcost[k + 1][j];
		if (tmp < min) {
			min = tmp;
			min_pos = k;
		}
	}
	mcost[i][j] = min;
	mcost[n - 1 - i][n - 1 - j] = min_pos;
	printf("find cost for %i, %i is %i\n", i, j, min);
}

void mc_compute_optimal_cost(int *ml, int (*mcost)[20], int n)
{
	int i, j;
	for (i = 0; i < n; i++) {
		mcost[i][i] = 0;
	}
	for (j = 1; j < n; j++) {
		for (i = 0; i < n - j; i++) {
			mc_compute_sub_optimal_cost(ml, mcost, n, i, i + j);
		}
	}
}

int main(int argc, char **argv)
{
	int ml[MATRIX_NUM + 1];
	int mcost[MATRIX_NUM][MATRIX_NUM];
	get_random_array(ml, MATRIX_NUM + 1, MAX_LEN);
	mc_compute_optimal_cost(ml, mcost, MATRIX_NUM);
	print_array(ml, MATRIX_NUM + 1, "ml");
	mc_print_costs(mcost, MATRIX_NUM);
/*
	mc_print_dividers(mcost, MATRIX_NUM);
	mc_print_best_path(mcost, MATRIX_NUM);
*/
	return 0;
}

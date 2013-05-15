/* optimal_bst.c
 *
 * Author: Jimmy Pan
 * Email: dspjmt@gmail.com
 * Date: Tue May 14 10:44:56 CST 2013
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
#include <string.h>
#include "algorithms.h"

#define KEY_NUM 20
#define RAW_POSS_MAX 100

float obst_get_min_expt(float expt[][KEY_NUM], float dumkp[], int i, int j)
{
	if (i > j)
		return dumkp[i];
	else
		return expt[i][j];
}

float obst_calculate_min_expt(int root[][KEY_NUM], float expt[][KEY_NUM],
  float psum[][KEY_NUM], float dumkp[], int i, int j)
{
	int k, min_pos;
	float tmp1, tmp2, tmp3;
	if (i > j)
		return dumkp[i];
	else {
		float tmps[j - i + 1];
		for (k = i; k <= j; k++) {
			tmp1 = obst_get_min_expt(expt, dumkp, i, k - 1);
			tmp2 = obst_get_min_expt(expt, dumkp, k + 1, j);
			tmp3 = psum[i][j];
			tmps[k - i] = tmp1 + tmp2 + tmp3;
/*
			printf(", for k = 0, tmp1 = %f, tmp2 = %f, tmp3 = %f, tmps[k - i] = %f", tmp1, tmp2, tmp3, tmps[k - i]);
*/
		}
		min_pos = i;
		for (k = i + 1; k <= j; k++) {
			if (tmps[k] < tmps[min_pos])
				min_pos = k;
		}
		root[i][j] = min_pos;
		expt[i][j] = tmps[min_pos - i];
		printf(", root is %i", min_pos);
		return tmps[min_pos];
	}
}

void obst_find_optimal_solution(int root[][KEY_NUM], float expt[][KEY_NUM],
  float psum[][KEY_NUM], float dumkp[], int n)
{
	int i, j;
	for (j = 0; j < n - 1; j++) {
		for (i = 0; i < n - j; i++) {
			printf("\ncalculating %i to %i, min is %f", i, i + j, obst_calculate_min_expt(root, expt, psum, dumkp, i, i + j));
		}
	}
}

void obst_print_subroots(int root[][KEY_NUM], int i, int j)
{
	int k;
	k = root[i][j];
	if (i > j)
		return;
	else if (i == j)
		printf("node key: %d    left child: dum right child: dum\n", k);
	else {
		printf("node key: %d    left child: %d right child: %d\n", k,
		  root[i][k - 1], root[k + 1][j]);
		obst_print_subroots(root, i, k - 1);
		obst_print_subroots(root, k + 1, j);
	}
}

void obst_print_roots(int root[][KEY_NUM], int n)
{
	obst_print_subroots(root, 0, n - 1);
}

void obst_print_floats(float a[], int n)
{
	int i;
	for (i = 0; i < n; i++)
		printf("%f ", a[i]);
	puts("");
}

int main(int argc, char **argv)
{
	int i, j, sum = 0;
	int keys[KEY_NUM], root[KEY_NUM][KEY_NUM];
	float keyp[KEY_NUM], dumkp[KEY_NUM + 1], expt[KEY_NUM][KEY_NUM],
	  psum[KEY_NUM][KEY_NUM];
	int tmp[KEY_NUM * 2 + 1];
	get_random_array(tmp, KEY_NUM * 2 + 1, RAW_POSS_MAX);
	memset(root, 0, sizeof root);
	memset(expt, 0, sizeof expt);
	memset(psum, 0, sizeof psum);
	for (i = 0; i < KEY_NUM * 2 + 1; i++) {
		sum += tmp[i];
	}
	for (i = 0; i < KEY_NUM; i++) {
		keyp[i] = (float)tmp[i] / sum;
		keys[i] = i;
	}
	for (; i < KEY_NUM * 2 + 1; i++) {
		dumkp[i - KEY_NUM] = (float)tmp[i] / sum;
	}
	for (i = 0; i < KEY_NUM; i++) {
		psum[i][i] = keyp[i] + dumkp[i] + dumkp[i + 1];
		for (j = i + 1; j < KEY_NUM; j++) {
			psum[i][j] = psum[i][j - 1] + keyp[j] + dumkp[j + 1];
		}
	}
	obst_print_floats(keyp, KEY_NUM);
	obst_print_floats(dumkp, KEY_NUM + 1);
/*
*/
	obst_find_optimal_solution(root, expt, psum, dumkp, KEY_NUM);
	obst_print_roots(root, KEY_NUM);
	return 0;
}

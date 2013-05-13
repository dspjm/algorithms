/* longet_comm_subseq.c
 *
 * Author: Jimmy Pan
 * Email: dspjmt@gmail.com
 * Date: Mon May 13 18:50:44 CST 2013
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

#define STR_LEN 20
#define NUM_MAX 25

void lcs_convert_array(int *s, int n)
{
	int i;
	for (i = 0; i < n; i++)
		s[i] += 'A';
}

void lcs_print_string(int *s, int n, char *name)
{
	int i;
	printf("%s:\n", name);
	for (i = 0; i < n; i++)
		printf("%c ", s[i]);
	printf("\n");
}

void compute_longest_subsequence(int *s1, int *s2, int llen[][STR_LEN + 1],
  int sup[][STR_LEN + 1], int n)
{
	int i, j;
	memset(llen, 0, sizeof **llen * n * n);
	memset(sup, 0, sizeof **llen * n * n);
	for (i = 1; i < n; i++) {
		for (j = 1

int main(int argc, int **argv)
{
	int s1[STR_LEN], s2[STR_LEN];
	int llen[STR_LEN + 1][STR_LEN + 1], sup[STR_LEN + 1][STR_LEN + 1];
	get_random_array(s1, STR_LEN, NUM_MAX);
	get_random_array_setseedless(s2, STR_LEN, NUM_MAX);
	lcs_convert_array(s1, STR_LEN);
	lcs_convert_array(s2, STR_LEN);
	lcs_print_string(s1, STR_LEN, "s1");
	lcs_print_string(s2, STR_LEN, "s2");
	compute_longest_subsequence(s1, s2, llen, sup, STR_LEN);
}

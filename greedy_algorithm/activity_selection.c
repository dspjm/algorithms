/* activity_selection.c
 *
 * Author: Jimmy Pan
 * Email: dspjmt@gmail.com
 * Date: Wed May 15 10:36:51 CST 2013
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

#define ACT_NUM 20
#define START_MAX 30
#define DURATION_MAX 20

void as_sort_by_finish(int *s, int *f, int n)
{
	int i, j, tmp;
	for (i = 1; i < n; i++) {
		for (j = i; j > 0; j--) {
			 if (f[j] < f[j - 1]) {
				tmp = f[j];
				f[j] = f[j - 1];
				f[j - 1] = tmp;
				tmp = s[j];
				s[j] = s[j - 1];
				s[j - 1] = tmp;
			} else
				break;
		}
	}
}

void as_fill_act_list_in_range(int *al, int *off, int i, int j,
  int *s, int *f)
{
	int k;
	for (k = i + 1; k < j && s[k] < f[i]; k++);
	if (k < j) {
		al[*off] = k;
		(*off)++;
		as_fill_act_list_in_range(al, off, k, j, s, f);
	}
	return;
}

void as_fill_activity_list(int *al, int n, int *s, int *f)
{
	int off = 0;
	as_fill_act_list_in_range(al, &off, 0, n + 1, s, f);
}

int main(int argc, char **argv)
{
	int i;
	int s[ACT_NUM + 2] = { 0 }, d[ACT_NUM + 2] = { 0 }, f[ACT_NUM + 2];
	int al[ACT_NUM] = {};
	get_random_array(s + 1, ACT_NUM, START_MAX);
	get_random_array(d + 1, ACT_NUM, DURATION_MAX);
	s[ACT_NUM + 1] = START_MAX + DURATION_MAX + 1;
	for (i = 0; i < ACT_NUM + 2; i++)
		f[i] = s[i] + d[i];
	as_sort_by_finish(s + 1, f + 1, ACT_NUM);
	print_array(s, ACT_NUM + 2, "s");
	print_array(f, ACT_NUM + 2, "f");
	as_fill_activity_list(al, ACT_NUM, s, f);
	print_array(al, ACT_NUM, "al");
	return 0;
}

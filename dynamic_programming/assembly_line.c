/* assembly_line.c
 *
 * Author: Jimmy Pan
 * Email: dspjmt@gmail.com
 * Date: Mon May  6 01:05:11 CST 2013
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
#include <stdlib.h>
#include "algorithms.h"

#define STATION_NUM 20
#define TIME_MAX 10

int main(int argc, char **argv)
{
	int i, tmp1, tmp2;
	int s1[STATION_NUM], s2[STATION_NUM], t1[STATION_NUM + 1],
	  t2[STATION_NUM + 1];
	int f1[STATION_NUM + 1], f2[STATION_NUM + 1], l1[STATION_NUM],
	  l2[STATION_NUM];
	get_random_array(s1, STATION_NUM, TIME_MAX * 4);
	get_random_array_setseedless(s2, STATION_NUM, TIME_MAX * 4);
	get_random_array_setseedless(t1, STATION_NUM + 1, TIME_MAX);
	get_random_array_setseedless(t2, STATION_NUM + 1, TIME_MAX);
	f1[0] = s1[0] + t1[0];
	f2[0] = s2[0] + t2[0];
	for (i = 1; i < STATION_NUM; i++) {
		tmp1 = f1[i - 1] + s1[i];
		tmp2 = f2[i - 1] + t1[i] + s1[i];
		if (tmp1 > tmp2) {
			l1[i - 1] = 2;
			f1[i] = tmp2;
		} else {
			l1[i - 1] = 1;
			f1[i] = tmp1;
		}
		tmp1 = f2[i - 1] + s2[i];
		tmp2 = f1[i - 1] + t2[i] + s2[i];
		if (tmp1 > tmp2) {
			l2[i - 1] = 1;
			f2[i] = tmp2;
		} else {
			l2[i - 1] = 2;
			f2[i] = tmp1;
		}
	}
	f1[STATION_NUM] = f1[STATION_NUM - 1] + t1[STATION_NUM];
	f2[STATION_NUM] = f2[STATION_NUM - 1] + t2[STATION_NUM];
	l1[STATION_NUM - 1] = 1;
	l2[STATION_NUM - 1] = 2;
	if (f1[STATION_NUM] > f2[STATION_NUM]) {
		printf("we choose line 2\n");
	} else {
		printf("we choose line 1\n");
	}
	print_array(s1, STATION_NUM, "s1");
	print_array(s2, STATION_NUM, "s2");
	print_array(t1, STATION_NUM + 1, "t1");
	print_array(t2, STATION_NUM + 1, "t2");
	print_array(f1, STATION_NUM + 1, "f1");
	print_array(f2, STATION_NUM + 1, "f2");
	print_array(l1, STATION_NUM, "l1");
	print_array(l2, STATION_NUM, "l2");
	return 0;
}

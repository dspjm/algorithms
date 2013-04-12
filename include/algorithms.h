/* algorithms.h
 *
 * Author: Jimmy Pan
 * Email: dspjmt@gmail.com
 * Date: Fri Apr 12 10:57:33 CST 2013
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

#include <stdlib.h>
#include <time.h>

static void set_seed(void)
{
	time_t tmp;
	time(&tmp);
	srand((unsigned int)tmp);
}

static void get_random_array(int *a, int size, int max)
{
	int i;
	set_seed();
	for (i = 0; i < size; i++) {
		a[i] = rand() % max + 1;
	}
}

static void print_array(int *a, int size, char *array_name)
{
	int i;
	printf("%s:\n", array_name);
	for (i = 0; i < size; i++) {
		printf("%d ", a[i]);
	}
}

#!/bin/bash
if [ "$1" != "" ] ; then
	sed "1s/\.c/$1/ ; /Date:/s/Date:/Date: `date`/" ../example_header > $1
else
	sed "/Date:/s/Date:/Date: `date`/" ../example_header > new.c
fi
	 

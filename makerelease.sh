#! /bin/bash

exe=$1

ldd $exe |awk '{print $3}' |grep so | while read line
do
	cp $line -vi .
done

#!/bin/bash

file=$1

list=`cat $file | grep "CONFIG_" | grep -v "# CONFIG_"`

rm -rf ${file}.symbol.xml
for term in $list
do
    k=`echo $term | awk -F = '{print $1}'`
    v=`echo $term | awk -F = '{print $2}'`

    echo "<macro><name>${k}</name><value>${v}</value></macro>" >> ${file}.symbol.xml
done

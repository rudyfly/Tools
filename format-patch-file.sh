#!/bin/bash

FILE=$1

git log --oneline $FILE > log.tmp

COMMITS=`cat log.tmp | awk {'print $1'}`

index=1
for COMMIT_ID in $COMMITS
do
	git format-patch --suffix=.${index}.patch -1 $COMMIT_ID
	index=`expr $index + 1`
done

rm -f log.tmp

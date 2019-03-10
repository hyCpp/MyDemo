#!/bin/bash

function batch_convert() {
    for file in `ls $1`
    do
        if [ -d $1"/"$file ]
        then
            batch_convert $1"/"$file
        else
            dos2unix $1"/"$file
#sed 's/[ \t]*$//g'
            sed 's/[ \t]*$//g'  $1"/"$file   >$1"/"$file".back"
            rm $1"/"$file
            mv $1"/"$file".back"  $1"/"$file
            #echo $1"/"$file
        fi
    done
}

batch_convert $1

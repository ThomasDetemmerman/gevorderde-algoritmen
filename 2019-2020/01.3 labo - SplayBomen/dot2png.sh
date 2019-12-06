#!/usr/bin/env sh

for file in $(find . -name "*.dot"); do
    dot "$file" -Tpng -o "$file".png
    #rm -r "$file"
done;
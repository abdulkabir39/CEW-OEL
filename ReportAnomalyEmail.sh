#!/bin/bash

files=("ReportGenerator.c" "AnomalyGenerator.c" "EmailSender.c")

for file in "${files[@]}"; do

    filename="${file%.*}"

    gcc -o "$filename" "$file" -lcjson -lcurl -I/usr/include/cjson -I/usr/include

    if [ $? -eq 0 ]; then
        echo "Compilation successful for $file"
        "./$filename"
    else
        echo "Compilation failed for $file"
    fi
done


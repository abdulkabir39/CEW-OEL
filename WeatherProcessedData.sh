#!/bin/bash

files=("Fetch_API.c" "ProcessData.c")

for file in "${files[@]}"; do
    filename="${file%.*}"

    gcc -o "$filename" "$file" -lcjson -lcurl -I/ahzam/include/cjson -I/ahzam/include

    if [ $? -eq 0 ]; then
        echo "Compilation successful for $file"
        "./$filename"
    else
        echo "Compilation failed for $file. Skipping execution."
    fi
done


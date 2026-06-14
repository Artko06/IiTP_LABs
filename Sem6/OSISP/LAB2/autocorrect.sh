#!/bin/bash

input_file="input.txt"
correct_file="correct.txt"

if [ ! -f "$input_file" ]; then
    echo "Error: File '$input_file' not found." >&2
    exit 1
fi

sed -E ':a;N;$!ba;
  # Начало документа
  s/^[[:space:]]*([[:lower:]])/\U\1/;
  
  # После . ! ? с пробелами/переносами
  s/([.!?])([[:space:]\n]+)([[:lower:]])/\1\2\U\3/g' "$input_file" > "$correct_file"


# bash autocorrect.sh input.txt
#!/bin/bash 

rm -rfv html/

find ./ -type f -a \( -name '*.out' -o -name '*.o' -o -name '*.gch' -o -name 'vgcore.?????' \) -exec rm -v {} \;

#!/bin/bash

./build.sh release profile

if [ $? -eq 0 ]; then
    ./build/release/mass_spring_system
    if [ $? -eq 0 ]; then
	gprof build/release/mass_spring_system gmon.out > profile.txt
	cat profile.txt | gprof2dot > profile.dot
	dot -Tpdf profile.dot -o profile.pdf
	echo "profile finished!"
    fi
else
    echo "profile failed!:"
fi

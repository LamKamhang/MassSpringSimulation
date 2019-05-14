#!/bin/bash

if [ $# -eq 0 ]; then
	cmake . -Bbuild/debug -DCMAKE_BUILD_TYPE=Debug
	if [ $? -eq 0 ]; then
		cd build/debug
		make
	fi
elif [ $1 == "release" ]; then
	cmake . -Bbuild/release -DCMAKE_BUILD_TYPE=Release
	if [ $? -eq 0 ]; then
		cd build/release
		make
	fi
else
	echo "build failed!"
fi
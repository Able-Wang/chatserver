# i/bin/bash

set -x

rm -rf `pw`/build/*
cd `pwd`/build &&
	cmake .. &&
	make


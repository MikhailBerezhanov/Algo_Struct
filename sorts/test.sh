#!/bin/bash

# Компиляция (если требуется).
#g++ -lm -O2 -fno-stack-limit -std=c++1z -x c++ sorts.cpp -o a.out

for name in ./*.in
do
	./a.out < ${name}
done

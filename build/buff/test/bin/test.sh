#!/bin/bash

cnt=1 

while [ $cnt -ne 100000 ]
do
	./ByteBuffer_Test >> output.txt
	((cnt++))
done

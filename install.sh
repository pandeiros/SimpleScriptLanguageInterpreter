#!/bin/bash
if [ ! -d "./bin" ]; then
	mkdir bin
fi

cmake -Bbin -H.
cd bin
make

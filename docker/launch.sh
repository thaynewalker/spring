#!/bin/bash

for i in {9870..9879}; do docker run -t -d --name=$i -p $i:$i -e PORT=$i spring; done

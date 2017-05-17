#!/bin/bash

for i in {9870..9879}; do docker kill $i; docker rm $i; done

#!/bin/bash

./fsm2dot.py $1 | dot -Tpng -o $2 
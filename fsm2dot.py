#!/usr/bin/env python3.4

import sys
import string
import re
import configparser
from graphviz import Digraph

if len(sys.argv) != 2:
	print("Incorrect number of args! \n")
	print("Usage: FSMfile, input_string\n")
	sys.exit()
else:
	config = configparser.ConfigParser()
	config.read(sys.argv[1])

states = {}
symbols = {}  
for line in config["transitions"]:
	for sets in re.split(',', config["transitions"][line]):
		for k,v in [sets.strip().split(':')]:
			if k in symbols: # encountered k before in this transition; 
				print("Error: nondeterministic FSM") # tried to assign >1 v to 1 k 
				sys.exit()

			symbols[k] = v			# else, make assignment
			states[line] = symbols	# states[line] = dictionary of symbol k,v pairs

	symbols = {}	# reset symbols to null after given transition dict 

dot = Digraph()
for state in states:
	dot.node(state, state)

for line in states:
	for k in states[line]:
		dot.edge(line, states[line][k], label=k)

print(dot.source)
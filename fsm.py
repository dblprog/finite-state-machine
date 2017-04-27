#!/usr/bin/env python3.3

import sys 
import string
import re
import configparser

if len(sys.argv) != 3:
	print("Incorrect number of args! \n")
	print("Usage: FSMfile, input_string\n")
	sys.exit()
else: 
	input_str = sys.argv[2]

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

cur_state = config["states"]["start"] # initialize to start
end_state = config["states"]["end"]

for char in input_str:
	if char not in states[cur_state]: 
		char = "*"
	if char in states[cur_state]:
		cur_state = states[cur_state][char]
	else:
		print("error: file does not provide an exhaustive transition function")
		sys.exit()

if(cur_state == end_state):
	print("Accept.")
else: print("Reject. Ended in state", cur_state)



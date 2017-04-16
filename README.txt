README.TXT

David Lax
2/19/2015
CMSC 162 Lab5 

Collaborators: Jacob Burroughs, Mikhail Iouchkov

Files:

Ex 1:

fsm.py -- uses a configparser to parse a .fsm file and user input
Works as designated in lab handout
(prints errors as required on example4.fsm, example5.fsm)

Sample interaction:

$ ./fsm.py example_fsm/example1.fsm abc
Accept.
$ ./fsm.py example_fsm/example3.fsm abc
Accept.
$ ./fsm.py example_fsm/example1.fsm abtyycc
Reject. Ended in state error
$ ./fsm.py example_fsm/example3.fsm abretyycc
Reject. Ended in state start
$ ./fsm.py example_fsm/example4.fsm abretyycc
error: file does not provide an exhaustive transition function
$ ./fsm.py example_fsm/example5.fsm abretyycc
Error: nondeterministic FSM


Ex 2: 

Makefile -- makefile for fsm c version
fsm.c -- uses a linked list to parse a .fsm file and user input


Sample interaction:

$ make
cc -std=c99 -O3   -c -o fsm.o fsm.c
cc   fsm.o   -o fsm
$ ./fsm example_fsm/example3.fsm abc
Accept.
$ ./fsm example_fsm/example1.fsm abc
Accept.
$ ./fsm example_fsm/example1.fsm abtyyc
Reject. Ended in state error
$ ./fsm example_fsm/example3.fsm abtyryc
Reject. Ended in state start


BONUS: As in handout, creates prettier fsm-diagrams

fsm2png.sh
fsm2dot.py

Sample interaction:

$ ./fsm2png.sh example_fsm/example1.fsm out.png
$ open out.png
$ ./fsm2png.sh example_fsm/example4.fsm out.png
$ open out.png
$ ./fsm2png.sh example_fsm/example2.fsm out.png
$ open out.png

// opens png's with default viewer


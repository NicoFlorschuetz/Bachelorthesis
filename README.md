At first, I want to declare the different failure levels, which I have to pay attention for. There are five different stages. 
Level 0: -for example: single bitflip...
	 -easy to correct per TC

Level 1: -Subsystem failure
	 -board get wrong values
	 -Arduino induced restart
	 
Level 2: -Subsystem failure not recovered by lower level
	 -Arduino induced alarm and power off, power on board
	 -Arduino sends information to Arduino Mega

Level 3: -Arduino can not solve problem or no keep alive signal is received
	 -Mega sends restart signal, if this do not work, sends power off power on signal

Level 4: -if restart and power off and on do not work
	 -Mega switch to redundant Arduino
	 

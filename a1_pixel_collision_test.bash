#!/bin/bash
test_num=0
REPORT_FILE=a1_test_report.txt

echo "Test run starting at $(date)" >>${REPORT_FILE}

#==============================================================================
#	Operations available in the Vacuum simulation.
#==============================================================================
function place_dust () {
	printf "d${1},${2} "
}

function place_slime () {
	printf "s${1},${2} "
}

function place_litter () {
	printf "t${1},${2} "
}

function set_timeout () {
	printf "o${1} "
}

function pause_resume () {
	printf "p"
}

function heading () {
	printf "h${1} "
}

function set_delay () {
	printf "m${1} "
}

function help () {
	printf "?"
}

function loop () {
	if (( $# >= 1 )) && (( $1 > 0 )) 
	then
		for (( i = 0; i < $1; i++ ))
		do
			printf "!"
		done
	else
		printf "!"
	fi
}

function enter () {
	printf "${1}"
}

function initial_trash () {
	printf "${1},${2},${3} "
}

#========================================================================
#	Run a single test.
#========================================================================	
function run_test () {
	(( test_num ++ ))
	echo "Test    : ${test_num}"
	echo "Category: ${category}"
	echo "Details : ${details}"
	echo "Expect  : ${expect}"
	
	read -p "Press r to run the test or s to skip..." run_or_skip
	
	if [ "${run_or_skip}" == "r" ]
	then
		echo ${cmd} | ./a1_n10318399
		read -p "Did the test pass or fail (p/f)? " result
		echo "Test ${test_num}: ${result}" >>${REPORT_FILE}
	fi
}
#==============================================================================

category="Trash collision"
details="Pixel level collsion"
expect="Vacuum displayed centre; Slime spawned left; Vacuum travels left; Collides on pixel level."

cmd=$(
	initial_trash 0 0 0
	place_slime 15 22
	heading 180
	pause_resume
	set_timeout 7
)

run_test 

#==============================================================================

category="Trash collision"
details="One of each trash spawned left of vacuum, vacuum pick up each"
expect="Count starts at 1,1,1 weight at 0; Vacuum moves left; Picks up trash;
	  Count ends at 0,0,0 weight at 26"

cmd=$(
	initial_trash 0 0 0
	place_litter 30 22
	place_slime 40 22
	place_dust 50 22
	heading 180
	pause_resume
	set_timeout 7
)

run_test 

#==============================================================================

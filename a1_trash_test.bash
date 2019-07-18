#!/bin/bash
test_num=0
REPORT_FILE=a1_test_report.txt

echo "Test run starting at $(date)" >>${REPORT_FILE}

#==============================================================================
#	Operations available in the Vacuum simulation.
#==============================================================================

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

function place_vacuum () {
	printf "v${1},${2},${3} "
}

function place_dust () {
	printf "d${1},${2} "
}

function place_slime () {
	printf "s${1},${2} "
}

function place_litter () {
	printf "t${1},${2} "
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

category="Setup trash"
details="One piece of each trash type on start."
expect="One piece of each trash type; None overlap; None out of bounds."

cmd=$(
	initial_trash 1 1 1
	set_timeout 2
)

run_test 

#==============================================================================

category="Setup trash"
details="Max of each trash type on start."
expect="1000 dust; 10 slime; 5 litter; None overlap; None out of bounds."

cmd=$(
	initial_trash 1000 1000 1000
	set_timeout 3
)

run_test 

#==============================================================================

category="Place trash"
details="Place one dust at 10,20."
expect="Dust at left of screen;"

cmd=$(
	initial_trash 0 0 0
	place_dust 10 20
	set_timeout 2
)

run_test 

#==============================================================================

category="Place trash"
details="Place one slime at 10,20."
expect="Slime at left of screen;"

cmd=$(
	initial_trash 0 0 0
	place_slime 10 20
	set_timeout 2
)

run_test 

#==============================================================================

category="Place trash"
details="Place one litter at 10,20."
expect="Litter at left of screen;"

cmd=$(
	initial_trash 0 0 0
	place_litter 10 20
	set_timeout 2
)

run_test 

#==============================================================================

category="Place trash"
details="Place litter out of bounds at 0,0 LINES,0 COLUMNS,0 LINES,COLUMNS."
expect="Litter placed in all corners of room; None overlapping boundry"

(( CX = COLUMNS, CY = LINES ))

cmd=$(
	initial_trash 0 0 0
	place_litter 0 0
	place_litter ${CX} 0
	place_litter 0 ${CY} 
	place_litter ${CX} ${CY}
	set_timeout 2
)

run_test 

#==============================================================================

category="Place trash"
details="Place one piece of trash in the same space as another."
expect="First litter placed; Space occupied message shown"

(( CX = COLUMNS, CY = LINES ))

cmd=$(
	initial_trash 0 0 0
	place_litter 10 10
	place_litter 10 12
	set_timeout 3
)

run_test 

#==============================================================================
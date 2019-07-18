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

function set_battery () {
	printf "y${1} "
}

function set_load () {
	printf "w${1} "
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

category="Vacuum setup"
details="Vacuum setup."
expect="Vacuum displayed centre; no trash visible."

cmd=$(
	initial_trash 0 0 0
	set_timeout 2
)

run_test 

#==============================================================================

category="Vacuum movement"
details="Simulated p press; Initial vacuum start."
expect="Vacuum displayed centre; moves at 90 degree heading."

cmd=$(
	initial_trash 0 0 0
	pause_resume
	set_timeout 2
)

run_test 

#==============================================================================

category="Vacuum movement"
details="Sets heading to 359 degrees"
expect="Vacuum displayed centre; moves to screen right: slight incline."

cmd=$(
	initial_trash 0 0 0
	heading 359
	pause_resume
	set_timeout 7
)

run_test 

#==============================================================================

category="Vacuum movement"
details="Sets heading to -180 degrees"
expect="Vacuum displayed centre; moves at 0 degree heading."

cmd=$(
	initial_trash 0 0 0
	heading -180
	pause_resume
	set_timeout 7
)

run_test 

#==============================================================================

category="Vacuum movement"
details="Sets heading to 540 degrees"
expect="Vacuum displayed centre; moves at 180 degree heading."

cmd=$(
	initial_trash 0 0 0
	heading 540
	pause_resume
	set_timeout 7
)

run_test 

#==============================================================================
category="Vacuum collision"
details="Vacuum hits wall and changes direction"
expect="Vacuum heading 90; hits bottom wall; changes direction 220-330 degrees"

cmd=$(
	initial_trash 0 0 0
	#set_delay 20
	pause_resume
	set_timeout 4
)

run_test 

#==============================================================================
category="Vacuum collision"
details="Vacuum hits wall and changes direction"
expect="Vacuum heading 90; hits wall; changes heading; hits wall; heaing always 0-359"

cmd=$(
	initial_trash 0 0 0
	set_delay 10
	pause_resume
	set_timeout 10
)

run_test 

#==============================================================================

category="Vacuum coordinates"
details="Set the vacuum coordinates and heading"
expect="Vacuum sits at 10,20 heading 120 degrees"

cmd=$(
	initial_trash 0 0 0
	place_vacuum 10 20 120
	set_timeout 3
)

run_test 

#==============================================================================

category="Vacuum coordinates"
details="Set the vacuum coordinates out of screen bounds"
expect="Vacuum moves to bottom right corner of screen"

cmd=$(
	initial_trash 0 0 0
	place_vacuum 300 100 0
	set_timeout 3
)

run_test 

#==============================================================================

category="Vacuum state"
details="Place vacuum @ bottom left, set the vacuum battery to 25"
expect="Vacuum docks and recharges"

((CY = LINES))

cmd=$(
	initial_trash 0 0 0
	place_vacuum 0  ${CY}
	set_battery 25
	pause_resume
	set_delay 20
	set_timeout 8
)

run_test 

#==============================================================================

category="Vacuum state"
details="Place vacuum @ bottom left, set the vacuum battery to 2"
expect="Vacuum heads to dock and runs out of power"

((CY = LINES))

cmd=$(
	initial_trash 0 0 0
	place_vacuum 0 ${CY}
	set_battery 2
	pause_resume
)

run_test 

#==============================================================================

category="Vacuum state"
details="Place vacuum @ bottom left, set the vacuum load to 46"
expect="Vacuum heads to dock unloads trash"

((CY = LINES))

cmd=$(
	initial_trash 0 0 0
	place_vacuum 0 ${CY}
	set_load 46
	pause_resume
	set_delay 20
	set_timeout 8
)

run_test 

#==============================================================================
category="Vacuum state"
details="Set battery and load to 200"
expect="Battery displays 100; load displays 65; Vacuum docks and unloads trash"

((CY = LINES))

cmd=$(
	initial_trash 0 0 0
	set_timeout 5
	set_load 200
	set_battery 200
	pause_resume
)

run_test 

#==============================================================================
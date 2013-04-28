#!/bin/bash

FAILED="[\e[1;31mFAILED\e[0m]"
PASSED="[\e[1;32mPASSED\e[0m]"


printf "\033[25C\033[1m COMPILING \033[0m\n"

cd tests
make tests -i >/dev/null 2>/dev/null

NB_TESTS=$(ls *.c | wc -w)
NB_TESTS_COMPILED=$(ls *.test | wc -w)

TESTS=(*.test)

# TESTING RETURN STATUS

printf "\033[1m${NB_TESTS_COMPILED} / ${NB_TESTS} tests compiling\033[0m\n"

printf "\033[25C\033[1m RUNNING TESTS \033[0m\n"

NB_TESTS_OK=0

for ((i = 0; i < ${#TESTS[@]} ; i++))
do
		printf "${TESTS[$i]}\033[80D\033[60C" 
		make ${TESTS[$i]%.test}.output >/dev/null
		if [[ $? -ne 0 ]]
		then
				printf $FAILED
		else
				NB_TESTS_OK=$((NB_TESTS_OK+1))
				printf $PASSED
		fi
		printf "\n"
done

printf "\033[1m${NB_TESTS_OK} / ${NB_TESTS} tests ok\033[0m\n"

#CHECKING MEMORY_LEAKS
printf "\033[25C\033[1m CHECKING MEMORY LEAKS \033[0m\n"

make valgrinds -i >/dev/null 2>/dev/null

NB_MEMORY_OK=0

for ((i = 0; i < ${#TESTS[@]} ; i++))
do
		VALGRIND_OUT=${TESTS[i]%.test}.valgrind
		if grep -Fq "All heap blocks were freed" ${VALGRIND_OUT}
		then
				NB_MEMORY_OK=$((NB_MEMORY_OK+1))
				printf "\e[1;32m"
		else
				printf "\e[1;31m"
		fi
		printf "\033[1m${TESTS[i]}:\033[0m\n"
		grep "total heap usage" $VALGRIND_OUT
		printf "\e[0m\n"
done

printf "\033[1m${NB_MEMORY_OK} / ${NB_TESTS} tests without memory leaks\033[0m\n"

cd ..
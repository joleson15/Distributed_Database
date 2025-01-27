Advanced Databases Final Project

Authors:
Jackson Oleson - jo1449@nyu.edu
Mihir Prajapati - mp6570@nyu.edu

RepCRecV2.rpz is the reprozip bundle containing the project runs on the test
cases in the tests folder which contain the provided 25 test cases and additional
5 test cases that we've created. The reprozip trace command was run on the
testCheck.sh script which runs the ./main executable on all test cases in the
tests folder and stores the outputs in the outputs folder. We tried individually
executing the test cases on the executable but the reprounzip directory run 
command threw the error saying too many arguments.

For the source code, the makefile compiles the program, which creates the main
executable of the project. To compile, run "make RepCRec". 
The program can then be run using this "Usage: ./main <testCase.txt>" command.
Replacing "testCase" with the name of the test (i.e. test1).
Code compiles on CIMS using gcc version 11.5.0
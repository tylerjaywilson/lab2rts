# lab2rts
# Authors: Tyler Wilson and Jorden Luke

# This lab was completed to fulfill the requirements for Lab 2 in ECE 5780.
# The purpose of this lab was to implement an RM and EDF Real Time Scheduling algorithm.
# This document explains the contents of the files

input_file.txt: This file is used to read in a list of tasks. The format is as follows:
	
	[number of tasks]
	[simulation time]
	[task_id,execution_time,period]
	...
	[task_id,execution_time,period]

	i.e:
	3
	120
	1,3,6
	2,2,10
	3,1,7

main.cpp: Contains the code to run the RM and EDF schedule.

schedule.cpp: Contains the code for the Schedule class - used to create an object that holds statistics to print to the console.

task.cpp: Contains the code for the Task class - used to hold the information for each individual task.

makefile: Used to build the project - Coded in C++ on a 64 bit linux machine.
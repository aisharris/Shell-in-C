[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/76mHqLr5)
## Description
OSN Assignment 2 - Building a Shell - 2022101012

This is a linux shell with a few custom functions.

## Run Instructions
Running 'make' will run the makefile with the relevant c files included. Running ./a.out will then start the shell.

## Assumptions

# Input
    Defined size assumptions(may be changed by altering the '#define's):
    - max file name: 200 characters
    - max lines in file: 1000  characters
    - max length of line in file: 2000 characters
    - max argument count: 20 
    - max length of command or argument: 50 characters
    - max length of a path: 300 characters
    - max user input length: 200 characters
    - max input words: 50
    - max word length: 200 characters

# General Assumptions:
    - 'peek' will have flags in the format as given in the document
    - 'peek' -l output formatted to columns but not the exact same as bash
    - shell must only be run in linux systems
    - a user input cannot end in special characters(except '"' and '&')
    - redirection is not required to work in conjunction with 'pastevents'
    - Cntrl-D once will not affect the actual terminal, but twice consecutively may exit the terminal
    - Cntrl-C does not interrupt neonate(only 'x' key can)
    - Functionality of iMan assumes an internet connection
    - the prompt note of a process running for >2sec only shows if it was the last process that ran(eg. 'sleep 5;echo hi' will have no effect on prompt)
    -  only one output redirect('>' or '>>') can be present in a command
    - flags are space separated

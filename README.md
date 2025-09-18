# Turing-machine
This project is a general-purpose Turing Machine simulator written in C. It can simulate any Turing machine whose behavior is described in a simple configuration file.

This repository includes an example configuration (palindrome.tm) that checks if a binary string is a palindrome.

Features
Flexible Configuration: Define states, alphabet, and transitions in a .tm file.

Dynamic Tape: The tape is implemented as a doubly linked list, allowing it to grow infinitely in either direction as needed.

Clear Simulation Trace: The program prints the state of the tape at each step, making it easy to follow the machine's execution.

Written in C: Demonstrates memory management and fundamental data structure concepts.

How to Compile and Run
Compilation
You can compile the program using GCC. Make sure you are in the project's root directory.

gcc -o turing_machine turing_machine.c -std=c99

Execution
To run the simulator, you need to provide two arguments: the path to the configuration file and the input string for the tape.

./turing_machine <config_file.tm> <input_string>

Example: Checking a Palindrome
To check if the string 101101 is a palindrome:

./turing_machine palindrome.tm 101101

Expected Output: Result: ACCEPT

Example: Checking a Non-Palindrome
To check if the string 1010 is a palindrome:

./turing_machine palindrome.tm 1010

Expected Output: Result: REJECT

Configuration File Format (.tm)
The behavior of the Turing machine is defined in a text file with the following keywords:

start_state: <state_name>

accept_state: <state_name>

reject_state: <state_name>

blank: <char> (Defines the blank symbol, defaults to 'B' if not specified)

Transitions: Each transition is on a new line in the format:
<current_state> <read_symbol> <new_state> <write_symbol> <move_direction>

move_direction must be L (left) or R (right).

Lines starting with # are treated as comments and ignored.

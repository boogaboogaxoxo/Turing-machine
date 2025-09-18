Configuration file for a Turing Machine that checks for binary palindromes.
A palindrome is a string that reads the same forwards and backward (e.g., 101101).
The alphabet is {0, 1}.

Machine Definition:
start_state: q0
accept_state: q_accept
reject_state: q_reject
blank: B

Algorithm Logic:
1. Start at the leftmost digit. Read it, and replace it with a blank (B).
2. Move right to the end of the string.
3. Check if the last digit matches the one you read at the beginning.
4. If it matches, replace it with a blank. If not, reject.
5. Move left back to the beginning.
6. Repeat until the tape is all blanks. If successful, accept.
Transition:
Format: <current_state> <read> <new_state> <write> <move>
State q0: Start. Read the first character.
If it's a 0, erase it, go to state q_read_0 (to find a matching 0 at the end)
q0 0 q_read_0 B R

If it's a 1, erase it, go to state q_read_1 (to find a matching 1 at the end)
q0 1 q_read_1 B R

If the tape is empty (reads a blank), it's a palindrome. Accept.
q0 B q_accept B R

State q_read_0: We read a 0. Move right to find the end of the string.
Skip over all 0s and 1s.
q_read_0 0 q_read_0 0 R
q_read_0 1 q_read_0 1 R

When we find a blank, we've gone one step too far. Move left to the last digit.
q_read_0 B q_check_0 B L

State q_read_1: We read a 1. Move right to find the end of the string.
Skip over all 0s and 1s.
q_read_1 0 q_read_1 0 R
q_read_1 1 q_read_1 1 R

When we find a blank, we've gone one step too far. Move left to the last digit.
q_read_1 B q_check_1 B L

State q_check_0: We are at the last digit, looking for a matching 0.
If it's a 0, great! Erase it and start returning to the left.
q_check_0 0 q_return B L

If it's a 1 or a blank (string was odd length and we've crossed over), it's not a palindrome. Reject.
q_check_0 1 q_reject 1 L
q_check_0 B q_accept B R

State q_check_1: We are at the last digit, looking for a matching 1.
If it's a 1, great! Erase it and start returning to the left.
q_check_1 1 q_return B L

If it's a 0 or a blank, it's not a palindrome. Reject.
q_check_1 0 q_reject 0 L
q_check_1 B q_accept B R

State q_return: Move left all the way to the first blank.
Skip over all 0s and 1s.
q_return 0 q_return 0 L
q_return 1 q_return 1 L

When we hit the blank at the start, move one step right to the new "first" digit.
Go back to the start state (q0) to repeat the process.
q_return B q0 B R

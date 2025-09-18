#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE_LEN 256
#define MAX_TRANSITIONS 256


typedef struct TapeNode {
    char symbol;
    struct TapeNode* prev;
    struct TapeNode* next;
} TapeNode;


typedef struct {
    char current_state[MAX_LINE_LEN];
    char read_symbol;
    char new_state[MAX_LINE_LEN];
    char write_symbol;
    char move_direction; // 'L' or 'R'
} Transition;

// --- Tape Utility Functions ---
TapeNode* create_tape_node(char symbol) {
    TapeNode* newNode = (TapeNode*)malloc(sizeof(TapeNode));
    if (!newNode) {
        perror("Failed to allocate memory for tape node");
        exit(EXIT_FAILURE);
    }
    newNode->symbol = symbol;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}


TapeNode* initialize_tape(const char* input, char blank_symbol) {
    if (input == NULL || strlen(input) == 0) {
        return create_tape_node(blank_symbol);
    }

    TapeNode* head = create_tape_node(input[0]);
    TapeNode* current = head;

    for (int i = 1; i < strlen(input); i++) {
        TapeNode* newNode = create_tape_node(input[i]);
        current->next = newNode;
        newNode->prev = current;
        current = newNode;
    }
    return head;
}


void print_tape(TapeNode* tape_start, TapeNode* current_head, const char* current_state) {
   
    while (tape_start->prev) {
        tape_start = tape_start->prev;
    }

    printf("[%s] ", current_state);
    TapeNode* current = tape_start;
    while (current) {
        if (current == current_head) {
            printf("[%c]", current->symbol); 
        } else {
            printf(" %c ", current->symbol);
        }
        current = current->next;
    }
    printf("\n");
}


// --- Main Simulation Logic ---

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <config_file.tm> <input_string>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char* config_filename = argv[1];
    char* input_string = argv[2];

    
    FILE* file = fopen(config_filename, "r");
    if (!file) {
        perror("Error opening configuration file");
        return EXIT_FAILURE;
    }

    char start_state[MAX_LINE_LEN];
    char accept_state[MAX_LINE_LEN];
    char reject_state[MAX_LINE_LEN];
    Transition transitions[MAX_TRANSITIONS];
    int num_transitions = 0;
    char blank_symbol = 'B'; 

    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "start_state: %s", start_state) == 1) continue;
        if (sscanf(line, "accept_state: %s", accept_state) == 1) continue;
        if (sscanf(line, "reject_state: %s", reject_state) == 1) continue;
        if (sscanf(line, "blank: %c", &blank_symbol) == 1) continue;
        
        
        char current_s[MAX_LINE_LEN], new_s[MAX_LINE_LEN];
        char read_sym, write_sym, move;
        if (sscanf(line, "%s %c %s %c %c", current_s, &read_sym, new_s, &write_sym, &move) == 5) {
            if (num_transitions < MAX_TRANSITIONS) {
                strcpy(transitions[num_transitions].current_state, current_s);
                transitions[num_transitions].read_symbol = read_sym;
                strcpy(transitions[num_transitions].new_state, new_s);
                transitions[num_transitions].write_symbol = write_sym;
                transitions[num_transitions].move_direction = move;
                num_transitions++;
            }
        }
    }
    fclose(file);

    // --- 2. Initialize Tape and State ---
    TapeNode* tape_head = initialize_tape(input_string, blank_symbol);
    char current_state[MAX_LINE_LEN];
    strcpy(current_state, start_state);

    printf("Initial Tape:\n");
    print_tape(tape_head, tape_head, current_state);
    printf("----------------------------------------\n");

    // --- 3. Run Simulation Loop ---
    int steps = 0;
    while (strcmp(current_state, accept_state) != 0 && strcmp(current_state, reject_state) != 0) {
        // Find the appropriate transition
        bool transition_found = false;
        for (int i = 0; i < num_transitions; i++) {
            if (strcmp(transitions[i].current_state, current_state) == 0 &&
                transitions[i].read_symbol == tape_head->symbol) {

                // Apply the transition
                strcpy(current_state, transitions[i].new_state);
                tape_head->symbol = transitions[i].write_symbol;

                // Move the head
                if (transitions[i].move_direction == 'R') {
                    if (tape_head->next == NULL) { // Extend tape if needed
                        tape_head->next = create_tape_node(blank_symbol);
                        tape_head->next->prev = tape_head;
                    }
                    tape_head = tape_head->next;
                } else if (transitions[i].move_direction == 'L') {
                    if (tape_head->prev == NULL) { // Extend tape if needed
                        tape_head->prev = create_tape_node(blank_symbol);
                        tape_head->prev->next = tape_head;
                    }
                    tape_head = tape_head->prev;
                }
                
                transition_found = true;
                break;
            }
        }

        if (!transition_found) {
            printf("No transition found. Halting in a reject state.\n");
            strcpy(current_state, reject_state);
            break;
        }

        print_tape(tape_head, tape_head, current_state);
        steps++;
        if (steps > 1000) { // Safety break for non-halting machines
             printf("Exceeded maximum steps (1000). Halting.\n");
             strcpy(current_state, reject_state);
             break;
        }
    }

    // --- 4. Print Final Result ---
    printf("----------------------------------------\n");
    printf("Execution halted in %d steps.\n", steps);
    printf("Final state: %s\n", current_state);
    if (strcmp(current_state, accept_state) == 0) {
        printf("Result: ACCEPT\n");
    } else {
        printf("Result: REJECT\n");
    }
    printf("Final tape: ");
    print_tape(tape_head, tape_head, current_state);

    
    while (tape_head->prev) tape_head = tape_head->prev;
    while (tape_head) {
        TapeNode* next = tape_head->next;
        free(tape_head);
        tape_head = next;
    }
    
    return EXIT_SUCCESS;
}

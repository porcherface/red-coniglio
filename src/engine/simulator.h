#ifndef SIM_H__
#define SIM_H__


#include <stdlib.h>
#include "types.h"

int statebyte_print(struct state S);
int human2statebyte(char inputcommand[2]);

int update_instructions(int instruction[256],int command, struct state in_state,int k);
int move_ship(struct spaceship* A, int command);

struct state simulate_state_evolution(struct state in_state, struct eve in_eve,int print_flag);
#endif

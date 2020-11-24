#ifndef ANN_H__
#define ANN_H__


#include <stdlib.h>
#include "types.h"

struct state propose_state(struct state in_state, struct  eve in_eve); 
struct state state_init(struct eve E); 

int temperature_init_tempering(double beta[],int num, int param);
int state_init_tempering(struct state S[], struct eve E, int num);

double energy_eval(struct state new_state); 
double fake_energy_eval_test(struct state new_state);
 #endif

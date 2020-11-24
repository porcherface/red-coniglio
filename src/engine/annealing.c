
#include <stdlib.h>
#include "annealing.h"
#include "types.h"

struct state propose_state(struct state in_state, struct  eve in_eve)
{
    
    struct state outstate=in_state;
    int position = rand()%(in_eve.nf0+in_eve.nf1+in_eve.nf2);


    // ALL COMMANDS, SUBST WITH ALLOWED ONLY
    /* 
    outstate.statebyte[position] += (rand()%64) ;
    outstate.statebyte[position] = outstate.statebyte[position] %64;
    */

    int com = in_eve.command_arr[rand()%in_eve.num_valid_commands];
    int cnd = in_eve.condition_arr[rand()%in_eve.num_valid_conditions];
    
    while(com + 16*cnd == in_state.statebyte[position])
    {
        com = in_eve.command_arr[rand()%in_eve.num_valid_commands];
        cnd = in_eve.condition_arr[rand()%in_eve.num_valid_conditions];

    }
    outstate.statebyte[position] = com + 16*cnd;

    return outstate;

}
int temperature_init_tempering(double beta[],int num, int param)
{
    int i;
    for(i=0;i<num;++i)
    {
        beta[num-i-1]=param/(1+i);
        //param /=2;
    }
    return 0;
}

int state_init_tempering(struct state S[], struct eve E, int num)
{
    int i,j;
    
    for(i=0;i<MAX_LEN;++i)
    {
        for(j=0;j<num;++j)
        {
            S[j].statebyte[i]  = 0;
        }
    }

    for(i=0;i<E.nf0+E.nf1+E.nf2;++i)
    {
        for(j=0;j<num;++j)
        {
            S[j].statebyte[i]  = 1;
        }
    }
    for(j=0;j<num;++j)
    {
        /* furbish */
        S[j].statebyte[E.nf0-1] = 3;
        S[j].statebyte[E.nf0+E.nf1-1] = 3;
        S[j].statebyte[E.nf0+E.nf1+E.nf2-1] = 3;
        S[j].nf0=E.nf0;
        S[j].nf1=E.nf1;
        S[j].nf2=E.nf2;
        S[j].stars  = 0;
        S[j].steps  = 0;
        S[j].exitcode  = 1;
        S[j].E = 0;
        S[j].id = j;
    }

    return 0;

}

struct state state_init(struct eve E)
{
    struct state out_s;
    int i;
    
    for(i=0;i<MAX_LEN;++i)
    {
        out_s.statebyte[i]  = 0;
    }

    for(i=0;i<E.nf0+E.nf1+E.nf2;++i)
    {
        out_s.statebyte[i]  = 1;
    }

    /* furbish */
    out_s.statebyte[E.nf0-1] = 3;
    out_s.statebyte[E.nf0+E.nf1-1] = 3;
    out_s.statebyte[E.nf0+E.nf1+E.nf2-1] = 3;
    out_s.nf0=E.nf0;
    out_s.nf1=E.nf1;
    out_s.nf2=E.nf2;

    
    out_s.stars  = 0;
    out_s.steps  = 0;
    out_s.exitcode  = 99;
    out_s.E = 0;
    out_s.id = 0;

    return out_s;
} 
 

double energy_eval(struct state new_state)
{

    int S1 = new_state.stars;
    int S2 = new_state.steps;
    int S3 = new_state.exitcode;

    double EEE = 0;
    if(S3 == 0)
    {
        EEE-=50;
    }
    if(S3== 2){
        EEE+=100;
    }
    return EEE-S1*10-S2/10;
}

double fake_energy_eval_test(struct state new_state)
{
    int i;   
    double out = 0;
    for(i=0;i<MAX_LEN;++i)
    {
        out +=  ( (double)new_state.statebyte[i] - 3)*((double)new_state.statebyte[i] - 3);
    }


    return out;
}

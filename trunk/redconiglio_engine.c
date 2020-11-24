/* redconiglio_engine.c */
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "loadfile.h"
#include "annealing.h"
#include <math.h>
#include "simulator.h"
#include <time.h>

/*
                   ▄              ▄
                  ▌▒█           ▄▀▒▌
                  ▌▒▒█        ▄▀▒▒▒▐
                 ▐▄▀▒▒▀▀▀▀▄▄▄▀▒▒▒▒▒▐
               ▄▄▀▒░▒▒▒▒▒▒▒▒▒█▒▒▄█▒▐
             ▄▀▒▒▒░░░▒▒▒░░░▒▒▒▀██▀▒▌
            ▐▒▒▒▄▄▒▒▒▒░░░▒▒▒▒▒▒▒▀▄▒▒▌
            ▌░░▌█▀▒▒▒▒▒▄▀█▄▒▒▒▒▒▒▒█▒▐
           ▐░░░▒▒▒▒▒▒▒▒▌██▀▒▒░░░▒▒▒▀▄▌
           ▌░▒▄██▄▒▒▒▒▒▒▒▒▒░░░░░░▒▒▒▒▌
          ▌▒▀▐▄█▄█▌▄░▀▒▒░░░░░░░░░░▒▒▒▐
          ▐▒▒▐▀▐▀▒░▄▄▒▄▒▒▒▒▒▒░▒░▒░▒▒▒▒▌
          ▐▒▒▒▀▀▄▄▒▒▒▄▒▒▒▒▒▒▒▒░▒░▒░▒▒▐
           ▌▒▒▒▒▒▒▀▀▀▒▒▒▒▒▒░▒░▒░▒░▒▒▒▌
           ▐▒▒▒▒▒▒▒▒▒▒▒▒▒▒░▒░▒░▒▒▄▒▒▐
            ▀▄▒▒▒▒▒▒▒▒▒▒▒░▒░▒░▒▄▒▒▒▒▌
              ▀▄▒▒▒▒▒▒▒▒▒▒▄▄▄▀▒▒▒▒▄▀
                ▀▄▄▄▄▄▄▀▀▀▒▒▒▒▒▄▄▀
                   ▒▒▒▒▒▒▒▒▒▒▀▀

                    */

/* PARAMS */
#define BETA_START 0
#define BETA_END 100
#define BETA_STEP 0.001
#define SIM_STEPS 1000
#define TOT_STEPS (BETA_END*SIM_STEPS)
#define STEP_TIME 1


void print_TBD()
{
    printf("Sadly skipped\n");
}

int main(int argc, char* argv[])
{
    printf("\n*******************\n");
    printf("*Hello REDCONIGLIO!\n");
    printf("*******************\n");

    if(argc != 2)
    {
        printf("wrong usage!\n");
        printf("right usage: redconiglio_engine <levelname>\n");
        return 1;
    }

    struct eve E;
    struct state S;
    struct state newS;
    char* levelname = argv[1];

    printf("Levelname: %s\n",levelname);
    //scanf("%s",levelname);
    //print_TBD();

    printf("Loading level...\n");    
    int ret_code = level2eve(levelname,&E);
    int i,j;

    printf("Slots: %u %u %u\n",E.nf0,E.nf1,E.nf2);

    printf("Initializing run...\n");

    S = state_init(E);
    S.E=fake_energy_eval_test(S);
    S.nf0=E.nf0;
    S.nf1=E.nf1;
    S.nf2=E.nf2;
    //S.E=energy_eval(newS);
    printf("Starting probe is ");

    statebyte_print(S);
    printf("\n");
    
    clock_t t; 
    t = clock(); 

    printf("Start!\n");
    double beta;
    int sim;

    int print_counter = 0;
    int accepted_counter = 0;
    int tot_counter = 0;
    int broke = 0;
    /* a nonindented double for, practically intend as one */
    for(beta = BETA_START ; beta < BETA_END;beta+=BETA_STEP)
    {
    for(sim = 0;sim < SIM_STEPS;++sim){
        tot_counter++;
        /* propose a neighbouring state */
        newS = propose_state(S,E);
        
        /*printf("state under test: \n");
        for(k = 0;k<MAX_LEN;++k)
        {
            printf("%u ",newS.statebyte[k]);
    
        }
        printf("\n");*/
        
        /* eval state parameters*/
        newS = simulate_state_evolution(newS,E,0);
  
        /* evaluate energy difference */
        newS.E = energy_eval(newS);
        double delta = newS.E - S.E;
        //double delta = energy_eval(newS)-S.E);

        /* accept / reject state */
        /* Boltzmann measure */
        /* Accept if E decrease */


        if(delta < 0)
        {
            S = newS;
            accepted_counter++;
        }else
        /* accept with probability p = exp(-beta*delta) */
        {   
            double p = (double)rand()/RAND_MAX;
            if((double)exp(-beta*delta) > p)
            {
                S = newS;
                accepted_counter++;

            }
        } 
        if(tot_counter%1000000==0)
        {
            print_counter++;
            printf("%d done (message %d, %d accepted, current energy: %lf)\n",tot_counter,
                                    print_counter,accepted_counter,S.E);

        }
        if(newS.exitcode==1)
        {
            printf("Solution!!\n");
            printf("\n");
            broke = 1;
            break;
        }
    }

    if(broke)
    {
       break;
    }
    }

    
    printf("Results: \n");
    printf("Best Solution is ");
     
    statebyte_print(S);
    printf("\n");
    printf("\nStars: %u \n",S.stars);
    printf("Steps: %u \n",S.steps);
    printf("Exitcode: %u \n",S.exitcode);
    printf("Energy: %lf \n",S.E);
 

    printf("final beta: %lf\n",beta);
    printf("acceptance rate: %lf\n",(double)accepted_counter/tot_counter);

    printf("Solution found after %d iterations\n",tot_counter);
    
    t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
    printf("Annealing took %f seconds to execute \n", time_taken); 
    return 0;
}

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
#define ARG_ERROR 253
#define PARAM_ERROR 254
#define FILE_ERROR 255
#define INTERNAL_ERROR 99
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define EXIT_OVERFLOW 2
*/
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

#define NUM_PROBES 10
#define TEMPERING_RATE 10
#define BETA_START 0
#define BETA_END 100
#define BETA_STEP 0.001
#define SIM_STEPS (1000/NUM_PROBES)
#define TOT_STEPS (BETA_END*SIM_STEPS)
#define STEP_TIME 1

#define PRINT_TO_FILE 0

int main(int argc, char* argv[])
{
    printf("\n*******************\n");
    printf("*Hello REDCONIGLIO!\n");
    printf("*******************\n");

    if(argc != 2)
    {
        printf("wrong usage!\n");
        printf("right usage: redconiglio_engine <levelname>\n");
        return ARG_ERROR;
    }

    struct eve E;
    struct state S[NUM_PROBES];
    struct state newS;
    struct state tempS;

    char* levelname = argv[1];
    printf("Levelname: %s\n",levelname);
    printf("Loading level...\n");    
    int ret_code = level2eve(levelname,&E);
    
    if(ret_code)
    {
        return FILE_ERROR;
    }

    printf("Initializing run...\n");
    state_init_tempering(S,E,NUM_PROBES);
    
    int i;
    for(i=0;i<NUM_PROBES;++i)
    {
        S[i].E=energy_eval(S[i]);
    }
    printf("Starting probe is ");

    statebyte_print(S[0]);
    printf("\n");
    
    clock_t t; 
    t = clock(); 

    double beta[NUM_PROBES];

    temperature_init_tempering(beta, NUM_PROBES, BETA_END);
    int sim,probe; //sim iterators
    int n1,n2; // tempering couples
    
    /* counters */
    int print_counter = 0;
    int accepted_counter = 0;
    int tot_counter = 0;
    
    int broke = 0;
    double delta, delta_e, delta_b;

    printf("Start!\n");

    FILE* elogp;
    FILE* rlogp;//, rlogp;
    double probe0log = 0;
    double id0log = 0;
    int log_counter = 0;

    if(PRINT_TO_FILE)
    {
        elogp = fopen("e_data_log.log","w");
        rlogp = fopen("r_data_log.log","w");
    }

    /* a nonindented double for, practically intend as one */
    for(beta[0] = BETA_START ; beta[0] < BETA_END;beta[0]+=BETA_STEP)
    {


    probe0log=0;
    id0log=0;
    log_counter=0;

    for(sim = 0;sim < SIM_STEPS;++sim){
        for(probe=0;probe<NUM_PROBES;probe++)
        {
            tot_counter++;
            /* propose a neighbouring state */
            newS = propose_state(S[probe],E);
         
            
            /* eval state parameters*/
            newS = simulate_state_evolution(newS,E,0);
      
            /* evaluate energy difference */
            newS.E = energy_eval(newS);
            delta = newS.E - S[probe].E;
            //double delta = energy_eval(newS)-S.E);

            /* accept / reject state */
            /* Boltzmann measure */
            /* Accept if E decrease */
            if(delta < 0)
            {
                S[probe] = newS;
                accepted_counter++;
            }else
            /* accept with probability p = exp(-beta*delta) */
            {   
                double p = (double)rand()/RAND_MAX;
                if((double)exp(-beta[probe]*delta) > p)
                {
                    S[probe] = newS;
                    accepted_counter++;

                }
            } 

            // some ticking print
            if(tot_counter%1000000==0)
            {
                print_counter++;
                printf("%d done (message %d, %d accepted, current energy: %lf)\n",tot_counter,
                                        print_counter,accepted_counter,S[0].E);

            }
            if(newS.exitcode==0)
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
        // parallel tempering move!!
        if((sim)%TEMPERING_RATE==0 && NUM_PROBES>1)
        {
            n1=rand()%NUM_PROBES;
            n2=rand()%NUM_PROBES;

            delta_e = S[n1].E-S[n2].E;
            delta_b = beta[n1]-beta[n2];


            double p = (double)rand()/RAND_MAX;
                  
            if(exp(delta_e*delta_b)>p)
            {
                tempS = S[n1];
                S[n1]=S[n2];
                S[n2]=tempS;
            }

        }

        //update mean values
        if(PRINT_TO_FILE)
        {   
            int id0;
            probe0log += S[0].E;
            for(probe = 0;probe <NUM_PROBES;probe++)
            {
                if(S[probe].id ==0 )
                {
                    id0=probe;
                }
            }
            id0log += S[id0].E;
            log_counter++;
        }
    }

    if(PRINT_TO_FILE)
    {
        fprintf(rlogp,"%lf\n",id0log/log_counter);
        fprintf(elogp,"%lf\n",probe0log/log_counter);
    }

    if(broke)
    {
       break;
    }
    }
    
    if(PRINT_TO_FILE)
    {
        fclose(elogp);
        fclose(rlogp);
    }
        
    printf("Results: \n");
    printf("Best Solution is ");
     
    statebyte_print(newS);
    printf("\n");
    printf("\nStars: %u \n",newS.stars);
    printf("Steps: %u \n",newS.steps);
    printf("Exitcode: %u \n",newS.exitcode);
    printf("Energy: %lf \n",newS.E);
 

    printf("final beta: %lf\n",beta[0]);
    printf("acceptance rate: %lf\n",(double)accepted_counter/tot_counter);

    printf("Solution found after %d iterations\n",tot_counter);
    
    t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
    printf("Annealing took %lf seconds to execute \n", time_taken); 
    printf("Best Solution is "); 
    statebyte_print(newS);
    
    return (newS.exitcode);

}

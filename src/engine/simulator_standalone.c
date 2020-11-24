/* redconiglio_engine.c */
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "loadfile.h"
#include "simulator.h"
#include <math.h>

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
 

void print_TBD()
{
    printf("Sadly skipped\n");
}

int main(int argc, char* argv[])
{
    printf("\n*******************\n");
    printf("*Hello REDCONIGLIO!\n");
    printf("*******************\n");

    if(argc < 2 || argc > 3)
    {
        printf("wrong usage!\n");
        printf("right usage: simulator_minigame <levelname> (optional_command)\n");
        return ARG_ERROR;
    }

    int second_arg=0;
    if(argc==3)
    {   
        second_arg=1;
    }

    struct eve E;
    struct state S;
    char* levelname = argv[1];

    printf("Levelname: %s\n",levelname);
    //scanf("%s",levelname);
    //print_TBD();
    
    printf("Loading level...\n");    
    int ret_code = level2eve(levelname,&E);

    if(ret_code)
    {
        return FILE_ERROR;
    }
    printf("main again\n");
    
    int i,j; 

    char in[2];
    // insert a proposal
    /* MUST BE REPLACED WITH
    int human2statebyte(char inputcommand[2])
    */
    int k=0;
    
    if(second_arg)
    {
        char target[2];
        printf("reading from arguments: %s\n",argv[2]);
        
        printf("%d %d %d\n",E.nf0,E.nf1,E.nf2);

        for(i =0;i<E.nf0;++i)
        {
            while(argv[2][k] == ' ' || (char)argv[2][k] == ',')
            {
                k++;
            }
            target[0] =argv[2][k];
            target[1] =argv[2][k+1];
            S.statebyte[i]=(unsigned)human2statebyte(target);
            printf("target: %s     \n",target);
            printf("statebyte(%d): %d\n",i,S.statebyte[i]);
            if(target[1]== ' ' || target[1]==',')
            {
                k+=1;
            }else
            {   
                k+=2;
            }
        }
        
        for(i =E.nf0;i<E.nf1+E.nf0;++i)
        {
            while(argv[2][k] == ' ' || (char)argv[2][k] == ',')
            {
                k++;
            }
            target[0] =argv[2][k];
            target[1] =argv[2][k+1];
            S.statebyte[i]=(unsigned)human2statebyte(target);
            printf("target: %s     \n",target);
            printf("statebyte(%d): %d\n",i,S.statebyte[i]);
            if(target[1]== ' ' || target[1]==',')
            {
                k+=1;
            }else
            {   
                k+=2;
            }
        
        }
        for(i =E.nf1+E.nf0;i<E.nf1+E.nf0+E.nf2;++i)
        {
            while(argv[2][k] == ' ' || (char)argv[2][k] == ',')
            {
                k++;
            }
            target[0] =argv[2][k];
            target[1] =argv[2][k+1];
            S.statebyte[i]=(unsigned)human2statebyte(target);
            printf("target: %s     \n",target);
            printf("statebyte: %d\n",S.statebyte[i]);
            if(target[1]== ' ' || target[1]==',')
            {
                k+=1;
            }else
            {   
                k+=2;
            }
        }
    }else{

        for(i =0;i<E.nf0;++i)
        {
            printf("type f0: ");
            scanf("%s",in);
            S.statebyte[k]=human2statebyte(in);
            k++;
        }
        for(i =0;i<E.nf1;++i)
        {
            printf("type f1 (currently numbers only): ");
            scanf("%s",in);
            S.statebyte[k]=human2statebyte(in);
            k++;
        }
        for(i =0;i<E.nf2;++i)
        {
            printf("type f2 (currently numbers only): ");
            scanf("%s",in);
            S.statebyte[k]=human2statebyte(in);
            k++;
        }


    }
    printf("State read:");
    statebyte_print(S);
    
    S.stars = 0;
    S.exitcode=99;
    S.steps=0;
    S.nf0 = E.nf0;
    S.nf1=E.nf1;
    S.nf2=E.nf2;
    // simulate
    printf("Starting simulation\n");
    S = simulate_state_evolution(S,E,1);
    
    printf("Results\n");
    if(S.exitcode == 0)
    {
        printf("WIN !!!\n");
    }else{
        printf("Lose....\n");
    }
    
    printf("Stars: %d \n",S.stars);
    printf("Steps: %d \n",S.steps);
    printf("Exitcode: %d \n",S.exitcode);

    /* printf("Dynamic map:\n");
    for(i=0;i<10;++i)
    {
        for(j=0;j<10;++j)
        {
            printf("%d ", E.dynamic_map[i][j]);
        }
        printf("\n");
    }*/



    return 0;
}

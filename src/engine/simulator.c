#include "simulator.h"
#include <stdlib.h>
#include "types.h"
#include <string.h>
#include <stdio.h>

int statebyte_print(struct state S)
{
    char cmnd_translate[9] ={'<','^','>','0','1','2','R','G','B'};
    char cond_translate[4] ={' ','r','g','b'};

    int level_len = S.nf0+S.nf1+S.nf2;
    int k;
    
    for(k = 0;k<level_len;++k)
    {
        if(k==S.nf0 || k==S.nf0+S.nf1)
        { 
            printf(" , ");
        }

        printf("%c%c ",cond_translate[S.statebyte[k]/16],
                       cmnd_translate[S.statebyte[k]%16]);

    
    }
    return 0;
}

int human2statebyte(char inputcommand[2])
{
    int com = 0;
    int cond = 0;

    int out = 0;

    if(inputcommand[0]=='<'){return 0;}
    if(inputcommand[0]=='^'){return 1;}
    if(inputcommand[0]=='>'){return 2;}
    if(inputcommand[0]=='0'){return 3;}
    if(inputcommand[0]=='1'){return 4;}
    if(inputcommand[0]=='2'){return 5;}
    if(inputcommand[0]=='R'){return 6;}
    if(inputcommand[0]=='G'){return 7;}
    if(inputcommand[0]=='B'){return 8;}

    if(inputcommand[0] == 'r')
    {
        cond = 1;
    }
    if(inputcommand[0] == 'g')
    {
        cond = 2;
    }
    if(inputcommand[0] == 'b')
    {
        cond = 3;
    }
    if(inputcommand[0] == ' ')
    {
        cond = 0;
    }
    if(inputcommand[1]=='<'){return 0+16*cond;}
    if(inputcommand[1]=='^'){return 1+16*cond;}
    if(inputcommand[1]=='>'){return 2+16*cond;}
    if(inputcommand[1]=='0'){return 3+16*cond;}
    if(inputcommand[1]=='1'){return 4+16*cond;}
    if(inputcommand[1]=='2'){return 5+16*cond;}
    if(inputcommand[1]=='R'){return 6+16*cond;}
    if(inputcommand[1]=='G'){return 7+16*cond;}
    if(inputcommand[1]=='B'){return 8+16*cond;}


    return out;
}
 
int move_ship(struct spaceship* A, int command)
{ 
    //condition = (cmcn - command) / 4;

    if(command == 0)
    {
        A->angle = (A->angle+1)%4;
        return 0;
    }
    if(command == 2)
    {
        A->angle = (A->angle-1+4)%4;
        return 0;
    }
    if(command == 1)
    {
        if(A->angle == 0)
        {
            A->y++;                    
            return 0;
        }
        if(A->angle == 1)
        {
            A->x--;                    
            return 0;            
        }
        if(A->angle == 2)
        {
            A->y--;                
            return 0; 
        }
        if(A->angle == 3)
        {
            A->x++;                    
            return 0;
        }
    }
    return 255;
} 

int update_istructions(int instruction[256],int command, struct state in_state,int k)
{

    int shift_val;

    int buffer[256];
    int shift_buffer[16];    
    int counter;

    int copy = 1;
    int instr_count = 0;
    /*
    printf("into update_instructinos\n");
    printf("instruction vector pre process:\n");
    int it;
    for(it = 0;it < 16 ;++it)
    {
        printf("%d ",instruction[it]); 

    }
    printf("\n");

*/

    while(copy)
    {

        if(k+instr_count > 255-1)
        {
            copy=0;
        }

        buffer[instr_count] = instruction[k+instr_count];
        instr_count++;


        //set flag to false
        if( instruction[k+instr_count] == -1)
        {
            copy=0;
        }
    }

    int i;
    
    if(command == 3)
    {
        //instruction[k]
        shift_val = in_state.nf0;
    
        for (i =0;i<shift_val;++i)
        {
            instruction[i+k+1]=in_state.statebyte[i];      
        }
    }
    if(command == 4)
    {
        shift_val = in_state.nf1;
    
        for (i =0;i<shift_val;++i)
        {
            instruction[i+k+1]=in_state.statebyte[i+in_state.nf0];
        }
    }
    if(command == 5)
    {
        shift_val = in_state.nf2;
    
        for (i =0;i<shift_val;++i)
        {
         instruction[i+k+1]=in_state.statebyte[i+in_state.nf1];            
        }
    }

    //printf("into update_instructinos\n");
    //printf("instruction vector post process:\n");
    /*
    for(it = 0;it < 256 ;++it)
    {
        printf("%d ",instruction[it]); 

    }
    printf("\n");
    */

    return 0;

}


// 1 1 3 - 2 4 4 5 - 0

// 1 1 1 1 3

struct state simulate_state_evolution(struct state in_state, struct eve in_eve,int print_flag)
{
    struct state outstate=in_state;
    struct spaceship A;

    int have_instruction =1;

    // init spaceship
    A.x=in_eve.start_params[0];
    A.y=in_eve.start_params[1];

    A.angle=in_eve.start_params[2]-1;

    //
    
    A.stars=0;
    A.steps = 0;
    A.visited_count =0;
    int i,j;
    /*
    for(i=0;i<N;++i)
    {
        for(j=0;j<M;++j)
        {   

            printf("%d ",in_eve.dynamic_map[i][j]);
        }
        printf("\n");
    }*/
    int instruction[256];
    
    for(i=0;i<256;++i)
    {
        instruction[i]=-1;
    }
    // init instructions (fill first f0 string, others should follow)
    for(i=0;i<in_eve.nf0;++i)
    {
        instruction[i]=in_state.statebyte[i];
    }


    int condition;
    int command;
    int k=0;
    int no_update = 0;
    
    if(print_flag)
    {
        printf("starting in (x%d,y%d,angle: %d)\n",A.x,A.y,A.angle);
    }

    while(have_instruction && k < 255-16)
    {
        no_update = 0;
        int comm_cond = instruction[k];
        //parse command
        // get condition
        command = comm_cond %16;
        condition = (int)(comm_cond - command)/16;
        if(print_flag)
        {
            printf("$$$$$$$$$$$$$$$$$\n");
            printf("iteration %d\n",k);
            printf("instruction is %d (%d,%d)\n",comm_cond,command,condition);
            printf("current position is (x%d,y%d,angle: %d)\n",A.x,A.y,A.angle);
            for(i=0;i<N;++i)
            {
            for(j=0;j<M;++j)
            {   

                printf("%d ",in_eve.dynamic_map[i][j]);
            }
            printf("\n");
            }
        }
        //exec command
        int go=1;
        if(condition != 0 )
        {
            if (in_eve.dynamic_map[A.x][A.y] != condition)
            {
                go=0;
            }
        }
        if(comm_cond == -1)
        {
            go=0;
            have_instruction = 0;
            //printf("no instruction\n");
        }

        if(go)
        {
        //update map and ship
            //spaceship movement
            if(command<=2)
            {
                move_ship(&A, command);
            
            }else
            {
            //map painting
            if(command >=6)
            {
                in_eve.dynamic_map[A.x][A.y] = command-5;
                
    
            }
            //function command
            else
            {
                update_istructions(instruction,command,in_state,k);
            }
            }

            //collect star and place mark
            if(in_eve.dynamic_map[A.x][A.y] >90)
            {
                in_eve.dynamic_map[A.x][A.y] -= 100;
                A.stars++;
            }
            //search for eventual exit condition
            if(A.stars == in_eve.tot_stars)
            {
                //WON - SURELY OPTIMAL
                outstate.exitcode =0;
                outstate.stars = A.stars;
                outstate.steps = A.steps;
                have_instruction = 0;
                //printf("sim ended successfully\n");
                break;
            }
            if(in_eve.dynamic_map[A.x][A.y] == 0 || comm_cond == -1)
            {
                outstate.exitcode =1;
                outstate.stars = A.stars;
                outstate.steps = A.steps;
                have_instruction = 0;
                //printf("ended instructions or went out of map\n");
                break;
            }
            
            //update command stream
            A.steps++;   
        if(comm_cond == -1)
        {
            go=0;
            have_instruction = 0;
            //printf("no instruction\n");
            break;

        }    
        }
        k++;
        if(k>254)
        {
                outstate.exitcode =2;
                outstate.stars = A.stars;
                outstate.steps = A.steps;
                have_instruction = 0;
                //printf("ended instructions buffer\n");
                break;
        }

    }
    if(print_flag)
    {
        printf("$$$$$$$$$$$$$$$$$\n");
        printf("iteration %d\n",k);
        printf("final frame\n");
        printf("current position is (x%d,y%d,angle: %d)\n",A.x,A.y,A.angle);
        for(i=0;i<N;++i)
        {
        for(j=0;j<M;++j)
        {   

            printf("%d ",in_eve.dynamic_map[i][j]);
        }
        printf("\n");
        }
    }
    return outstate;

}
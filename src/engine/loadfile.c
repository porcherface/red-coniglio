
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "loadfile.h"

#define DEBUG 1

int level2eve(char filename[],struct eve* out)
{
 
    out->valid_commands = 0x4;
    out->valid_conditions = 0x5;
    
    int i,j;
    FILE *fp = fopen(filename,"r");
		
    if(fp==NULL)
    {   
        printf("ERROR - file not found\n");
        return 1;
    }  

    printf("level successfully loaded\n");
    char dummy[32];
    for(i=0;i<7;++i)
    {
        fscanf(fp,"%s",dummy); //comment
    }
   
    char currchar[32];
    out->tot_stars = 0;
    int starcount=0;

    for(i=0;i<N;++i)
    {
        for(j=0;j<M;++j)
        {   
            fscanf(fp,"%s ",currchar);
            out->map[i][j] =atoi( (const char*)currchar );
            out->dynamic_map[i][j] =out->map[i][j];
            printf("%d ",out->map[i][j]);

            if(out->dynamic_map[i][j] >=10 && out->dynamic_map[i][j]<90)
            {
                out->start_params[0]=i;
                out->start_params[1]=j;
                out->start_params[2]=(int)out->dynamic_map[i][j]/10;
                
                out->dynamic_map[i][j] = out->dynamic_map[i][j]-out->start_params[2]*10;
            }
            if(out->map[i][j]>90)
            {
                out->tot_stars++;
                out->stars[starcount][0]=i;
                out->stars[starcount][0]=j;
                
                starcount++;
            }
        }
    printf("\n");
    }
    
    //out->nf0 = 4;
    fscanf(fp,"%d",&out->nf0);
    //out->nf1 = 0;
    fscanf(fp,"%d",&out->nf1);
    //out->nf2 = 0; 
    fscanf(fp,"%d",&out->nf2);
    
    //out->num_valid_commands = 6;
    fscanf(fp,"%d",&out->num_valid_commands);    

    //out->num_valid_conditions = 3;
    fscanf(fp,"%d",&out->num_valid_conditions);    

    int a= out->num_valid_commands;
    int b= out->num_valid_conditions;
    
    out->command_arr = (unsigned*)malloc(a*sizeof(unsigned));
    out->condition_arr = (unsigned*)malloc(b*sizeof(unsigned));
    for(i=0;i<a;++i) 
    {   
        fscanf(fp,"%d",&out->command_arr[i]);
    }

    //out->command_arr[4] = 6;
    //out->command_arr[5] = 7;
    for(i=0;i<b;++i) 
    {   
        fscanf(fp,"%d",&out->condition_arr[i]);
    }

    printf("this level has %d stars\n", out->tot_stars);
    printf("this level has %d valid commands\n",a);
    printf("this level has %d valid conditions\n",b);
    
    printf("nf0 read: %d\n",(int)out->nf0);
    printf("nf1 read: %d\n",(int)out->nf1);
    printf("nf2 read: %d\n",(int)out->nf2);
    fclose(fp); 
     
    return 0;
}


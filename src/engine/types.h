#ifndef TYPES_H__
#define TYPES_H__

#define N 10
#define M 10

#define MAX_LEN 64

#define ARG_ERROR 253
#define PARAM_ERROR 254
#define FILE_ERROR 255
#define INTERNAL_ERROR 99
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define EXIT_OVERFLOW 2

/* symbols*/
/*
int COMMAND(char in)           
{
    switch(in) //paddle to 16
    {
        case '>':
            return 0;
            break;
        case '^':
            return 1;
            break;
        case '<':
            return 2;
            break;
        case '0':
            return 3;
            break;
        case '1':
            return 4;
            break;
        case '2':
            return 5;
            break;
        case 'R':
            return 6;
            break;
        case 'G':
            return 7;
            break;
        case 'B':
            return 8;
            break;
        
    }

    return -1;
}

int CONDITION(char in)
{
    switch(in) //paddle to 4
    {
        case 'r':
            return 1;
            break;
        case 'g':
            return 2;
            break;
        case 'b':
            return 3;
            break;
    }
    return 0;
}
 */
 
struct eve
{
    /* variables */
    unsigned int valid_commands; 
    unsigned int valid_conditions;

    unsigned int* command_arr;
    unsigned int* condition_arr;

    int num_valid_commands;
    int num_valid_conditions;

    /*env parameters */
    int nf0;
    int nf1;
    int nf2;

    int map[N][M];
    int dynamic_map[N][M];

    int start_params[3];
    int stars[MAX_LEN][2];
    int tot_stars;
};
 
struct state
{
    /* state str */
    char statebyte[MAX_LEN];

    /*env parameters (duplicated from eve, sigh) */
    unsigned int nf0;
    unsigned int nf1;
    unsigned int nf2;

    /* state parameters */
    unsigned int id;
    unsigned int steps;
    unsigned int stars;
    unsigned int exitcode;
    double E; /* energy of this state */
};

/* for simulator */
struct spaceship
{
    int x;
    int y;
    int angle;
    int stars;
    int steps;
    int visited_count; // ??????? maybe remove, dunno
};

#endif

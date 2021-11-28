#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int rand_gen();
int str_to_int(char *str);
int game(); // game end return 1
char buf[20];
int range = 100;
int cheat_mode = 1;

int main()
{
    while(1) {
        printf("Press 'y' to play or 'ESC' to quit:");
        scanf(" %[^\n]", buf);
        if(buf[0] == 27) break;
        // start a game
        printf("-----\nNew Round:\n");
        while(!game())
        {
            // playing
        }

    }
    printf("ESC pressed\n");
    return 0;
}
int game()
{
    int q = rand_gen();
    int lower_bound = 0;
    int upper_bound = range;
    if(cheat_mode) printf("*** q = %d ***\n", q);
    printf("guess:");
    while(1) {
        scanf(" %[^\n]", buf);
        if(buf[0] == 27) break; // ESC to quit

        int a = str_to_int(buf);
        if(a != q) {
            printf("wrong\n");
            if(a < q && a > lower_bound) lower_bound = a;
            if(a > q && a < upper_bound) upper_bound = a;
            printf("higher than %d, ", lower_bound);
            printf("lower than %d\n", upper_bound);
            printf("---\nguess:");
        }
        else {
            printf("correct!!! ^0^\n-----\n");
            return 1;
        }
    }
    
}
int rand_gen()
{
    int i, n;
    time_t t;
    n = 10;
    /* Intializes random number generator */
    srand((unsigned) time(&t));

    /* Print 5 random numbers from 0 to 49 */
    // for( i = 0 ; i < n ; i++ ) {
    //     printf("%d\n", rand() % range);
    // }

    return rand() % range;
}
int str_to_int(char *str)
{
    int val = 0;
    int i = 0;
    while(str[i] != '\0')
    {
        val *= 10;
        val += str[i]-'0';
        i ++;
        // printf("%d\n", val);
    }
    return val;
}
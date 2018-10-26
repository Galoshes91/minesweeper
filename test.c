#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main(void) {

    time_t timer;

    char test[10];

    time(&timer);

    while(strcmp(test, "hello") != 0) {

        printf("stop watch: ");
        scanf("%s", test);

        sleep(1);

    }

    time_t endTimer;

    time(&endTimer);    

    int total = (int) endTimer - timer;

    printf("%d", total); 



    return 0;
}


/*
time_t startTime;
time_t endTime;
*/
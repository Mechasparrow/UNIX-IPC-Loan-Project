#include <stdlib.h>
#include <time.h>
#include "creditscore.h"

void sprinkleSomeRandomCreditScore(){
    /* Intializes random number generator */
    srand(time(NULL));
}

int getCreditScore(char * ssn){    
    int creditScore = rand() % 500 + 200;

    return creditScore;
}
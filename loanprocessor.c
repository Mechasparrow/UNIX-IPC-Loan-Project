#include<stdio.h>
#include "loan.h"
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>
#include <string.h>
#include "creditscore.h"

#define QUEUE_NAME "/loanqueue"
#define FIFO_NAME "./processedloanfifo"
#define LOAN_ITEMS_SIZE 5
#define SOCK_PATH "./loansocket"

void cleanUp(ProcessedLoan * processedLoans){
    printf("freeing processed loans\n"); 
    free(processedLoans);
}

ProcessedLoan processLoan(Loan loan){
    ProcessedLoan processedLoan = {0};

    processedLoan.loan = loan;
    processedLoan.creditScore = getCreditScore(loan.ssn);
    processedLoan.computedMonthlyPayment = ((loan.salary) / 12.0) * 0.33;

    return processedLoan;
}


int main(void){

    mqd_t queueId;    
    queueId = mq_open(QUEUE_NAME, O_RDONLY);
    
    if (queueId == -1){
        perror("Bad queue\n");
    }

    int priority;

    printf("\n\n");
    printf("Retrieving queue\n");

    struct mq_attr attr;
    void * buffer;
    ssize_t nr;

    if (mq_getattr(queueId, &attr) == -1){
        perror("Unable to get attributes\n");
    }    

    buffer = malloc(sizeof(ProcessedLoan));
    if (buffer == NULL){
        perror("Malloc error\n");
    }

    sprinkleSomeRandomCreditScore();

    ProcessedLoan * processedLoans = malloc(sizeof(ProcessedLoan) * (LOAN_ITEMS_SIZE));

    for (int i = 0; i < LOAN_ITEMS_SIZE; i++){

        nr = mq_receive(queueId, buffer, attr.mq_msgsize, NULL);
        if (nr == -1){
            perror("Unable to pull items from queue\n");
        }
        
        Loan retrievedLoan = *((Loan*)buffer);

        ProcessedLoan pLoan = processLoan(retrievedLoan);
        processedLoans[i] = pLoan; 
        printf("\n");
    }
    
    mq_close(queueId);

    int s, len;
    struct sockaddr_un remote;

    printf("Connecting to write socket...\n");
    if ((s=socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
        perror("socket creation failed");
        exit(1);
    }

    remote.sun_family = AF_UNIX;
    strcpy(remote.sun_path, SOCK_PATH);
    len = strlen(remote.sun_path) + sizeof(remote.sun_family);
    if (connect(s, (struct sockaddr *)&remote, len) == -1) {
        perror("connect");
        cleanUp(processedLoans);
        exit(1);
    }

    printf("Connected to Loan Writer\n");
   
    int allLoanItemsSent = 0;
    int currentLoanItem = 0;
    int itemsLeft = LOAN_ITEMS_SIZE;

    while (allLoanItemsSent == 0){

        ProcessedLoan loanPayload = processedLoans[currentLoanItem];

        if (send(s, (char*)(&loanPayload), sizeof(ProcessedLoan), 0) == -1){
            perror("Unable to send loan");
            exit(1);
        }

        int t;
        char response[100];
        
        if ((t=recv(s, response, 100, 0)) > 0) {
            response[t] = '\0';
            printf("%s\n", response);
        } else {
            if (t < 0) perror("recv");
            else printf("Server closed connection\n");
            cleanUp(processedLoans);
            exit(1);
        }

        currentLoanItem++;
        itemsLeft--;

        if (itemsLeft <= 0){
            printf("Done sending items\n");
            allLoanItemsSent = 1;
            break;
        }
    }

    if (send(s, "stop", 4, 0) == -1){
            perror("Unable to send stop signal");
            cleanUp(processedLoans);
            exit(1);
    }
    
    cleanUp(processedLoans);
    return 0;
}
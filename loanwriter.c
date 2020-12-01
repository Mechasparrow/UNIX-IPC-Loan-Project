#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "loan.h"
#include <stdlib.h>

#define SOCK_PATH "./loansocket"
#define LOAN_CSV_FILE "./loan_info.csv"

void writeProcessedLoanToCsv(ProcessedLoan * processedLoans, int processedLoanCount){
    printf("writing loans...\n");

    FILE * fp = fopen(LOAN_CSV_FILE, "w");

    if (fp != NULL){

        fprintf(fp, "%s,%s,%s,%s,%s\n", "Borrower", "SSN", "Salary", "Credit Score", "Expected Monthly Payment");

        for (int i = 0; i < processedLoanCount; i++){
            ProcessedLoan processedLoan = processedLoans[i];
            fprintf(fp,"%s,%s,%d,%d,%.2f\n", processedLoan.loan.borrower, processedLoan.loan.ssn, processedLoan.loan.salary, processedLoan.creditScore, processedLoan.computedMonthlyPayment);
        }
    }

    fclose(fp);

}

int main(void){

    int s, s2, t, len;
    struct sockaddr_un local, remote;
    char response[100];

    int loanCount = 0;
    ProcessedLoan * pLoans = malloc(sizeof(ProcessedLoan) * 100);
    ProcessedLoan * pLoan = malloc(sizeof(ProcessedLoan));

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
        perror("Socket creation failed\n");
        exit (1);
    }

    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, SOCK_PATH);
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);
    bind(s, (struct sockaddr *)&local, len);

    if (listen(s, 5) == -1){
        perror("listen failed\n");
        exit(1);
    }

    int stop = 0;
    while (!stop){

        printf("Waiting for connection...\n");

        t = sizeof(remote);

        if ((s2 = accept(s, (struct sockaddr *)&remote, &t)) == -1){
            perror("failed on accept\n");
            exit(1);
        }

        printf("Connected\n");

        printf("Connected.\n");

        int n;
        int done = 0;

        do {
            printf("\n");
            n = recv(s2, pLoan, sizeof(ProcessedLoan), 0);

            if (n <= 0) {
                if (n < 0) perror("recv");
                done = 1;
            }

            if (!done) {
                if (strncmp((char*)pLoan, "stop", 4) == 0){
                    printf("Stopping\n");
                    stop = 1;
                    done = 1;
                }else{
                    displayProcessedLoan(pLoan);
                    pLoans[loanCount] = *pLoan;

                    loanCount++;
                    strcpy(response, "message recieved");

                    if (send(s2, response, 100, 0) < 0) {
                        perror("send");
                        done = 1;
                    }
                }
            }
        } while (!done);

        close(s2);
    }

    free(pLoan);
    writeProcessedLoanToCsv(pLoans, loanCount);
    free(pLoans);

    return 0;

}
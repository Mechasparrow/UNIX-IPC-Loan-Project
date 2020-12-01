#include <stdio.h>
#include <stdlib.h>
#include "loan.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <unistd.h>
#include <errno.h>
#define FIFO_PATH "./currentFifo" 
#define QUEUE_NAME "/loanqueue"

Loan * readFIFO(int * size){
  
  FILE * fp;

  char * line = NULL;
  size_t len = 0;

  fp = fopen(FIFO_PATH, "r");

  if (fp == NULL){
    perror("Unable to open fifo\n");
    exit(1);
  }


  int loanIdx = 0;

  Loan * loans = malloc(sizeof(Loan) * 100);

  while ((getline(&line, &len, fp)) != -1){
    Loan processedLoan = parseLoan(line);
    loans[loanIdx] = processedLoan;
    loanIdx++;
  }

  *size = loanIdx;

  //Close the fifo descriptor
  fclose(fp);

  //Free any lines of text from memory
  if (line){
    free (line);
  }

  return loans;

}


int main(void){

  printf("Retrieving Loans...\n");
  int loanSize;
  Loan * loans = readFIFO(&loanSize);

  printf("\nPutting Loans onto queue...\n");

  mqd_t queueId;
  
  struct mq_attr queueAttr = {
    0L,
    100L,
    sizeof(Loan),
    0L
  };

  queueId = mq_open(QUEUE_NAME, O_WRONLY | O_EXCL | O_CREAT, 0666,NULL);

  if (queueId == -1){
    perror("Bad queue\n");
    printf("%d\n", errno);
  }

  //TODO send data

  Loan * p = loans;
  for (int i = 0; i < loanSize; i++){
      mq_send(queueId, (char*)(p+i), sizeof(Loan), 1);
  }

  sleep(3);

  printf("Closing queue and freeing memory\n");
  mq_close(queueId);
  mq_unlink(QUEUE_NAME);
  free(loans);
  return 0;
}

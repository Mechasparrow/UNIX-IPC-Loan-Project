#include "loan.h"
#include <stdio.h>

void displayProcessedLoan(ProcessedLoan * processedLoan){
    displayLoan(&(processedLoan->loan));
    printf("Credit Score: %d\n", processedLoan->creditScore);
    printf("Expected Monthly Payment: %f\n", processedLoan->computedMonthlyPayment);
}


Loan parseLoan(char * rawLoanData){
  Loan newLoan = {0};

  char firstNameBorrowerBuffer[255];
  char lastNameBorrowerBuffer[255];

  sscanf(rawLoanData, "%s %s %s %d", firstNameBorrowerBuffer, lastNameBorrowerBuffer, newLoan.ssn, &newLoan.salary);

  char borrowerBuf[255];
  sprintf(newLoan.borrower, "%s %s", firstNameBorrowerBuffer, lastNameBorrowerBuffer);


  return newLoan;
}

Loan getLoanFromProcessedLoan(ProcessedLoan * processedLoan){
    return processedLoan->loan;
}

void displayLoan(Loan * loan){
  printf("%s, %s, %d\n", loan->borrower, loan->ssn, loan->salary);
}

void displayLoans(Loan * loans, int loanSize){
  Loan * p = loans;
  for (int i = 0; i < loanSize; i++){
    displayLoan(p+i);
  }
}
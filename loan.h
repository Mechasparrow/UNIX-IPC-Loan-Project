typedef struct _loan {
  char borrower[255];
  char ssn[10];
  int salary;
} Loan;

typedef struct _processedloan{
  Loan loan;
  int creditScore;
  double computedMonthlyPayment;
} ProcessedLoan;

void displayProcessedLoan(ProcessedLoan * processedLoan);
Loan getLoanFromProcessedLoan(ProcessedLoan * processedLoan);
Loan parseLoan(char * rawLoanData);
void displayLoan(Loan * loan);
void displayLoans(Loan * loans, int loanSize);
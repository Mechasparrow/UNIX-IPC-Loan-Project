from loan import Loan
from loan_api import retrieve_loan

import os
import time



fifoPath = "./currentFifo" 
os.mkfifo(fifoPath)

myfile = open(fifoPath, 'w')

for i in range(5):
    generatedLoan = retrieve_loan()
    myfile.write(str(generatedLoan) +"\n")

myfile.close()
os.unlink(fifoPath)

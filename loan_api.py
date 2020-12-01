from loan import Loan
import random

def construct_random_ssn():

    # ssn has nine digits
    generatedSsn = ""

    for i in range(9):
        randomSsnDigit = random.randrange(1,10)
        generatedSsn += str(randomSsnDigit)

    return generatedSsn


firstNames = ["John", "Paul", "Sam", "Sarah"]
lastNames = ["Smith", "Roper", "Carpenter", "Royal"]

def retrieve_loan():
    
    ssn = construct_random_ssn()
    borrowerName = random.choice(firstNames) + " " + random.choice(lastNames)
    salary = random.randrange(4000,10000)

    generatedLoan = Loan(borrowerName, ssn,salary)
    
    return generatedLoan




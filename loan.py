class Loan:
    def __init__(self, borrower, ssn, salary):
        self.borrower = borrower
        self.ssn = ssn
        self.salary = salary

    def print(self):
        
        print("Borrower: " + self.borrower)
        print ("SSN: " + str(self.ssn))
        print ("Salary: " + str(self.salary))

    def __str__(self):
        return str(self.borrower) + " " + str(self.ssn) + " " + str(self.salary)
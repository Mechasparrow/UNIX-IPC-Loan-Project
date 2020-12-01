# Topic
Investigation of Interprocess Communication on Unix-like operating systems

# Application
Simulate a IPC scenario with a api (Incoming-Loans), Reciever,  Processor (Loan Processor), and Writer (write to an output file)

# Implementation

(loan_api.py)------>(receiver)---------->(processor)--------->(loanwriter)------>loan_info.csv
              FIFO             Messaging Queue       Unix Socket           FILE I/O


# Requirements
A POSIX Compliant system (e.g Unix/Linux)
python3 must be installed
gcc compiler must also be installed

# Usage

To clean up and reset application

./cleanup

To compile all files

./compileall

To run

./program



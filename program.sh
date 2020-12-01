#!/bin/bash

echo "grabbing information from loan api"
python3 main.py &

sleep 1
echo "starting reciever"
./reciever > reciever.log &

echo "starting loan writer"
./loanwriter &

echo "starting loan processor"
sleep 1
./processor > processor.log &

sleep 1

wait
echo "Done!"
echo ""
echo "Completed Loan Information"
cat loan_info.csv
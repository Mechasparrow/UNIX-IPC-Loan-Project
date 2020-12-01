

fifoPath = "./currentFifo" 
myfile = open(fifoPath, 'r')

for line in myfile:
    parts = line.replace("\n","").split(",")
    print(parts)

myfile.close()
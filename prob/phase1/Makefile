#------------------------
# Do not modify this file
#------------------------

all: trader iml 
all1: runpart1 runpart2 runpart3

trader: trader.cpp receiver.h
	g++ -std=c++20 trader.cpp -o trader

iml: iml.cpp
	g++ -std=c++20 iml.cpp -o iml

runpart1: iml trader
	./trader 1 > output.txt &
	sleep 1
	./iml inputs/data1.txt
	diff -Bw outputs/out1.txt output.txt

runpart2: iml trader
	./trader 2 > output.txt &
	sleep 1
	./iml inputs/data2.txt
	diff outputs/out2.txt output.txt

runpart3: iml trader
	./trader 3 > output.txt &
	sleep 1
	./iml inputs/data3.txt
	diff outputs/out3.txt output.txt

# part2: trader2.cpp receiver.h iml.cpp 
# 	g++ -std=c++20 trader2.cpp -o trader2
# 	g++ -std=c++20 iml.cpp -o iml
# 	./trader2 2 > output2.txt
# 	sleep 1
# 	./iml inputs/data2.txt
# 	cat output2.txt

clean:
	rm -rf output.txt trader iml
	rm -rf *~
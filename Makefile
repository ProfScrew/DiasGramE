test: test.o tensor.o
	g++ test.o tensor.o -std=c++11 -o test_o && valgrind --leak-check=full --track-origins=yes ./test_o
test.o: test.cpp
	g++ -c test.cpp -otest.o -std=c++11 -Wall 
tensor.o: tensor.cpp
	g++ -c tensor.cpp -otensor.o -std=c++11 -Wall 
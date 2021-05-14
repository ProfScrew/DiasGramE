run: tensor.o dais.o test_d.o libbmp.o
	g++ test_d.o tensor.o dais.o libbmp.o --std=c++11 -o test_o && valgrind --leak-check=full --track-origins=yes ./test_o
dais.o: DAISGram.cpp DAISGram.h dais_exc.h tensor.h libbmp.h
	g++ -c DAISGram.cpp -odais.o --std=c++11 -Wall
test_d.o: test_d.cpp tensor.h DAISGram.h dais_exc.h libbmp.h
	g++ -c test_d.cpp -otest_d.o --std=c++11 -Wall 
tensor.o: tensor.cpp dais_exc.h libbmp.h
	g++ -c tensor.cpp -otensor.o --std=c++11 -Wall
libbmp.o: libbmp.cpp libbmp.h
	g++ -c libbmp.cpp -olibbmp.o --std=c++11 -Wall
clean:
	rm *.o
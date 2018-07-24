bayer2bmp: main.o fileconverter.o file.o converterparams.o converterbmp.o
	g++ -std=c++11 -o bayer2bmp main.o file.o fileconverter.o converterparams.o converterbmp.o -lboost_program_options

main.o: main.cpp fileconverter.h converterparams.h
	g++ -std=c++11 -c main.cpp -lboost_program_options

file.o: file.cpp file.h errorcodes.h
	g++ -std=c++11 -c file.cpp

fileconverter.o:  fileconverter.cpp fileconverter.h converterparams.h
	g++ -std=c++11 -c fileconverter.cpp

converterparams.o:  converterparams.cpp converterparams.h
	g++ -std=c++11 -c converterparams.cpp

converterbmp.o:  converterbmp.cpp converterbmp.h
	g++ -std=c++11 -c converterbmp.cpp

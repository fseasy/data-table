data_table : src/*
	g++ -std=c++11 -o data_table src/main.cpp src/data_table.cpp
	mkdir -p bin
	mv data_table bin/

# Define the default target
all: clean compile link prepare run

# Link the object files into the final executable
link:
	g++ -o bin/simulation bin/main.o bin/Action.o bin/Auxiliary.o bin/Facility.o bin/Plan.o bin/SelectionPolicy.o bin/Settlement.o bin/Simulation.o

# Compile each source file into an object file
compile:
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/main.o src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/Action.o src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/Auxiliary.o src/Auxiliary.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/Facility.o src/Facility.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/Plan.o src/Plan.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/SelectionPolicy.o src/SelectionPolicy.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/Settlement.o src/Settlement.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/Simulation.o src/Simulation.cpp

# Clean up the bin directory by removing all files
clean:
	rm -f bin/*

# Ensure that config.txt is copied to bin/ if it does not already exist
prepare:
	cp -n config.txt bin/ || echo "Using existing config.txt"

# Run the simulation with the config file
run:
	./bin/simulation ./config_file.txt

# Run the program with valgrind to check for memory leaks
valgrind: link
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/simulation ./config_file.txt

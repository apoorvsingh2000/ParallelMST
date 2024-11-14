CC = g++
CFLAGS = -I. -g -Wall -std=c++11

# The target for compiling the final program
sequential: main_sequential.o sequential_sort.o boruvka_sequential.o prims_sequential.o kruskals_sequential.o
	$(CC) -o sequential main_sequential.o sequential_sort.o boruvka_sequential.o prims_sequential.o kruskals_sequential.o $(CFLAGS)

# Pattern rule for compiling .cpp to .o
%.o: %.cpp
	$(CC) -c $< $(CFLAGS)

# Clean up the generated files
clean:
	rm -f sequential *.o *~

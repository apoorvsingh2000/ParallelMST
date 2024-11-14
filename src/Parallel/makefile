CC = g++
CFLAGS = -I. -g -Wall -fopenmp -std=c++11 -D_GLIBCXX_PARALLEL

# The target for compiling the final program
parallel: main_parallel.o parallel_sort.o boruvka_parallel.o prims_parallel.o kruskals_parallel.o 
	$(CC) -o ../parallel main_parallel.o parallel_sort.o boruvka_parallel.o prims_parallel.o kruskals_parallel.o $(CFLAGS)

# Pattern rule for compiling .cpp to .o
%.o: %.cpp
	$(CC) -c $< $(CFLAGS)

# Clean up the generated files
clean:
	rm -f parallel *.o *~ 

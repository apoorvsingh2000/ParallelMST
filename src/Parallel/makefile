CC = g++
CFLAGS = -I./header -g -Wall -fopenmp -std=c++11 -D_GLIBCXX_PARALLEL
SRC_DIR = main
OBJ_DIR = obj

# List of object files
OBJS = $(OBJ_DIR)/main_parallel.o $(OBJ_DIR)/parallel_sort.o $(OBJ_DIR)/boruvka_parallel.o $(OBJ_DIR)/prims_parallel.o $(OBJ_DIR)/kruskals_parallel.o

# Ensure the object directory exists
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# The target for compiling the final program
parallel: $(OBJ_DIR) $(OBJS)
	$(CC) -o ../parallel $(OBJS) $(CFLAGS)

# Rule for compiling .cpp to .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $< $(CFLAGS) -o $@

$(OBJ_DIR)/main_parallel.o: main_parallel.cpp
	$(CC) -c $< $(CFLAGS) -o $@

# Clean up the generated files
clean:
	rm -f ../parallel $(OBJ_DIR)/*.o *~
	rm -rf $(OBJ_DIR)
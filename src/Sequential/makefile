CC = g++
CFLAGS = -I./header -g -Wall -std=c++11
SRC_DIR = main
OBJ_DIR = obj

# List of object files
OBJS = $(OBJ_DIR)/main_sequential.o $(OBJ_DIR)/sequential_sort.o $(OBJ_DIR)/boruvka_sequential.o $(OBJ_DIR)/prims_sequential.o $(OBJ_DIR)/kruskals_sequential.o

# Ensure the object directory exists
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# The target for compiling the final program
sequential: $(OBJ_DIR) $(OBJS)
	$(CC) -o ../sequential $(OBJS) $(CFLAGS)

# Rule for compiling .cpp to .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $< $(CFLAGS) -o $@

$(OBJ_DIR)/main_sequential.o: main_sequential.cpp
	$(CC) -c $< $(CFLAGS) -o $@

# Clean up the generated files
clean:
	rm -f ../sequential $(OBJ_DIR)/*.o *~
	rm -rf $(OBJ_DIR)

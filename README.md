# ParallelMST

Parallel implementation of Minimum Spanning Tree Calculation using:
- Kruskal's Algorithm
- Prim's Algorithm
- Boruvka's Algorithm

## Prerequisites

### 1. **Cluster Setup**:
Ensure you have access to a compute cluster with the necessary resources, such as multi-core processors, large memory.
### 2. **Dependencies**:
- **C++ Compiler** (e.g., `g++`)
- **OpenMP** for parallelism
- **C++ Version** 11

### 3. **Installation**:
Clone the repository using the command:
```bash
git clone https://github.com/ayushnaique/ParallelMST.git 
```

Once the repository is cloned run:
```bash
cd ParallelMST/src/Sequential && make sequential && cd ../Parallel && make parallel && cd ../
```

To compile the GenerateEdges.cpp file:
```bash
g++ -std=c++11 -o edges GenerateEdges.cpp
```

### 4. **Command Line Arguments**:
The above command will result in three executables:
- parallel
- sequential
- edges
These executables are used to run the final code, which will output the MST weight and the runtime for all three algorithms

Generating Edges: 
```bash
./edges <vertices> <edges> <edges_out_file>
```

the command for parallel is as follows :
```bash
./parallel <num_threads> <vertices> <edges> <sorting_algo> <edges_file>
```

command for sequential is :
```bash
./sequential <vertices> <edges> <sorting_algo> <edges_file>
```

### 5. **Sorting Algorithms**:
Parallel has a total of 6 algorithms that can be used (in Prim's and Kruskal's Algorithm)
1) In built parallel_sort
2) Parallel Bubble Sort
3) Parallel Quick Sort
4) Parallel Merge Sort
5) Naive Parallel Bitonic Sort
6) Optimized Bitonic Sort

Sequential has a total of 5 algorithms that can be used (For kruskal's Algorithm)
1) In built STL sort 
2) Bubble Sort
3) Quick Sort
4) Merge Sort
5) Naive Bitonic Sort

Example command:
```bash
# Generate edges and store in edges.txt
./edges 4000 1000000 edges.txt
# Run for parallel algorithms with 4 threads
./parallel 4 4000 1000000 6 edges.txt #(6 : will use Optimized bitonic sort)
```

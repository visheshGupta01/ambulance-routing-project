# Smart Ambulance Routing System (C CLI)

A beginner-friendly DAA project written in C. It runs in the terminal and uses a sample city road network.

## Run

Install GCC and Make, then run:

```bash
make
./ambulance_routing
```

Or compile without Make:

```bash
gcc -std=c11 -Wall -Wextra main.c engine.c -o ambulance_routing
./ambulance_routing
```

## Menu

1. Show the road network.
2. Find the shortest route between two locations.
3. Dispatch an available ambulance and choose a hospital with beds.
4. Show ambulance and hospital information.
5. Exit.

## Algorithms

- **Dijkstra:** Finds the shortest route. The graph uses an adjacency matrix, so the time complexity is O(V^2).
- **Greedy selection:** Chooses the available ambulance and hospital with the smallest route distance.
- **Insertion sort:** Arranges candidate resources by distance. The worst-case time complexity is O(n^2).

## Files

- `main.c`: Menu, sample data and program flow.
- `engine.c`: Dijkstra, route building, sorting and resource selection.
- `engine.h`: Structures, constants and function declarations.
- `Makefile`: Compile and run commands.

This is a learning simulation. It does not connect to real GPS, ambulances or hospital systems. Resource updates last only until the program exits.

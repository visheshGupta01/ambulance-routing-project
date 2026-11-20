# Smart Emergency Ambulance Routing System (C / CLI)

A simple, menu-driven college DAA project written entirely in C. The program runs in the terminal and demonstrates shortest-path routing and resource selection on a sample city road network.

## Requirements

- GCC or another C11-compatible compiler
- Make (optional, for using the Makefile)

## Compile and run

From the repository directory:

```bash
make
./ambulance_routing
```

Or compile directly:

```bash
gcc -std=c11 -Wall -Wextra -pedantic -O2 main.c engine.c -o ambulance_routing
./ambulance_routing
```

To compile and run with Make in one command:

```bash
make run
```

To remove the compiled executable:

```bash
make clean
```

## CLI menu

1. Display the city road network.
2. Find the shortest route between two locations.
3. Dispatch the nearest available ambulance to a patient and select the nearest hospital with available beds.
4. View ambulance and hospital status.
5. Exit.

After a successful dispatch, the selected ambulance is marked as dispatched and one bed is deducted from the selected hospital. These changes remain only while the program is running; the sample project does not use a database or save data between runs.

## Algorithms

- **Dijkstra's algorithm:** Finds shortest distances in the weighted road graph. This adjacency-matrix implementation takes O(V^2) time.
- **Greedy selection:** Chooses the available ambulance with the smallest distance to the patient and the nearest eligible hospital.
- **Insertion sort:** Sorts candidate resources by route distance. Its worst-case time is O(n^2).

## Project files

| File | Purpose |
|---|---|
| `main.c` | CLI menu, sample road network, dispatch workflow |
| `engine.c` | Dijkstra, path reconstruction, greedy selection and insertion sort |
| `engine.h` | Structures, constants and function declarations |
| `Makefile` | Build, run and clean commands |

## Important note

This is an academic simulation using sample locations, distances, ambulances and hospitals. It is not connected to live GPS, ambulance services, hospital systems or emergency dispatch infrastructure.

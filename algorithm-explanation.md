# Algorithm Explanation

## 1. Graph representation

The city is represented as a weighted, undirected graph. Each location is a vertex and each road is an edge. The edge weight stores the road distance in kilometres. The graph is stored in an adjacency matrix.

## 2. Dijkstra's algorithm

Dijkstra calculates the shortest distance from one source location to every other location. It repeatedly selects the unvisited vertex with the smallest known distance and updates the distances of its neighbours.

The implementation uses an adjacency matrix and a linear search for the next vertex, so its time complexity is O(V^2).

## 3. Greedy ambulance selection

The program calculates the distance from each available ambulance to the patient's location. It selects the ambulance with the smallest reachable distance. Busy ambulances are ignored.

## 4. Greedy hospital selection

The program calculates distances from the patient's location to hospitals. Hospitals that are closed or have zero available beds are ignored. The nearest eligible hospital is selected.

## 5. Insertion sort

Candidate ambulance and hospital indexes are sorted by their route distances using insertion sort. The candidate with the smallest distance is selected first. The worst-case time complexity is O(n^2).

## 6. Dispatch update

After a successful dispatch, the selected ambulance becomes unavailable and the selected hospital's available-bed count decreases by one. This is an in-memory simulation; the values reset when the program exits.

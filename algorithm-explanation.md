# Algorithms Used

## Graph
The city locations are vertices. Roads are edges, and each edge stores the distance in kilometres. The graph is stored in a two-dimensional array called an adjacency matrix.

## Dijkstra's Algorithm
Dijkstra finds the shortest distance from a starting location to all other locations. It repeatedly chooses the unvisited location with the smallest distance and checks its connected roads.

Time complexity: O(V^2), because this version uses an adjacency matrix.

## Greedy Selection
The program checks available ambulances and chooses the one with the shortest route to the patient. It then chooses the nearest hospital that has at least one bed.

## Insertion Sort
The candidate ambulances and hospitals are arranged from the smallest distance to the largest distance.

Worst-case time complexity: O(n^2).

## Dispatch
After a successful dispatch, the ambulance becomes busy and one bed is removed from the selected hospital's available-bed count. These changes are temporary and reset when the program is started again.

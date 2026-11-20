#include "engine.h"

/* Dijkstra finds the shortest distance from one location to every location. */
ShortestPath dijkstra(int graph[MAX_NODES][MAX_NODES], int start) {
    ShortestPath result;
    int visited[MAX_NODES] = {0};

    for (int i = 0; i < MAX_NODES; i++) {
        result.distance[i] = INF;
        result.previous[i] = -1;
    }

    result.distance[start] = 0;

    for (int step = 0; step < MAX_NODES; step++) {
        int current = -1;

        /* Find the unvisited location with the smallest distance. */
        for (int i = 0; i < MAX_NODES; i++) {
            if (!visited[i] &&
                (current == -1 || result.distance[i] < result.distance[current])) {
                current = i;
            }
        }

        if (current == -1 || result.distance[current] == INF)
            break;

        visited[current] = 1;

        /* Check roads connected to the current location. */
        for (int next = 0; next < MAX_NODES; next++) {
            if (graph[current][next] > 0 &&
                result.distance[current] + graph[current][next] <
                result.distance[next]) {
                result.distance[next] =
                    result.distance[current] + graph[current][next];
                result.previous[next] = current;
            }
        }
    }

    return result;
}

/* Builds the route by following previous[] backwards. */
int get_path(ShortestPath result, int start, int end, int path[]) {
    int reverse[MAX_NODES];
    int count = 0;

    while (end != -1 && count < MAX_NODES) {
        reverse[count++] = end;
        if (end == start)
            break;
        end = result.previous[end];
    }

    if (reverse[count - 1] != start)
        return 0;

    for (int i = 0; i < count; i++)
        path[i] = reverse[count - i - 1];

    return count;
}

/* Insertion sort: arrange candidate indexes by increasing distance. */
void sort_by_distance(int indexes[], int distances[], int count) {
    for (int i = 1; i < count; i++) {
        int savedIndex = indexes[i];
        int savedDistance = distances[i];
        int j = i - 1;

        while (j >= 0 && distances[j] > savedDistance) {
            indexes[j + 1] = indexes[j];
            distances[j + 1] = distances[j];
            j--;
        }

        indexes[j + 1] = savedIndex;
        distances[j + 1] = savedDistance;
    }
}

/* Return the nearest available ambulance. */
int find_ambulance(int graph[MAX_NODES][MAX_NODES],
                   Ambulance ambulances[], int count, int patient, int *distance) {
    int indexes[MAX_AMBULANCES];
    int distances[MAX_AMBULANCES];
    int found = 0;

    for (int i = 0; i < count; i++) {
        if (ambulances[i].available) {
            ShortestPath result = dijkstra(graph, ambulances[i].location);
            indexes[found] = i;
            distances[found] = result.distance[patient];
            found++;
        }
    }

    if (found == 0)
        return -1;

    sort_by_distance(indexes, distances, found);
    *distance = distances[0];
    return indexes[0];
}

/* Return the nearest hospital that has at least one bed. */
int find_hospital(int graph[MAX_NODES][MAX_NODES],
                  Hospital hospitals[], int count, int patient, int *distance) {
    int indexes[MAX_HOSPITALS];
    int distances[MAX_HOSPITALS];
    int found = 0;
    ShortestPath result = dijkstra(graph, patient);

    for (int i = 0; i < count; i++) {
        if (hospitals[i].beds > 0) {
            indexes[found] = i;
            distances[found] = result.distance[hospitals[i].location];
            found++;
        }
    }

    if (found == 0)
        return -1;

    sort_by_distance(indexes, distances, found);
    *distance = distances[0];
    return indexes[0];
}

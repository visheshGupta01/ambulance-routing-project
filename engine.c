#include "engine.h"

DijkstraResult dijkstra(int graph[MAX_NODES][MAX_NODES], int source) {
    DijkstraResult result;
    int visited[MAX_NODES] = {0};

    for (int i = 0; i < MAX_NODES; i++) {
        result.dist[i] = INF;
        result.prev[i] = -1;
    }
    result.explored = 0;

    if (source < 0 || source >= MAX_NODES) {
        return result;
    }

    result.dist[source] = 0;

    for (int step = 0; step < MAX_NODES; step++) {
        int current = -1;
        int bestDistance = INF;

        for (int i = 0; i < MAX_NODES; i++) {
            if (!visited[i] && result.dist[i] < bestDistance) {
                bestDistance = result.dist[i];
                current = i;
            }
        }

        if (current == -1) {
            break;
        }

        visited[current] = 1;
        result.explored++;

        for (int next = 0; next < MAX_NODES; next++) {
            int roadDistance = graph[current][next];

            if (roadDistance > 0 && !visited[next] &&
                result.dist[current] + roadDistance < result.dist[next]) {
                result.dist[next] = result.dist[current] + roadDistance;
                result.prev[next] = current;
            }
        }
    }

    return result;
}

int reconstruct_path(const DijkstraResult *result, int source, int target,
                     int path[MAX_NODES]) {
    int reversePath[MAX_NODES];
    int count = 0;
    int current = target;

    if (source < 0 || source >= MAX_NODES ||
        target < 0 || target >= MAX_NODES) {
        return 0;
    }

    while (current != -1 && count < MAX_NODES) {
        reversePath[count++] = current;
        if (current == source) {
            break;
        }
        current = result->prev[current];
    }

    if (count == 0 || reversePath[count - 1] != source) {
        return 0;
    }

    for (int i = 0; i < count; i++) {
        path[i] = reversePath[count - 1 - i];
    }

    return count;
}

void insertion_sort_candidates(int ids[], int scores[], int count) {
    for (int i = 1; i < count; i++) {
        int currentId = ids[i];
        int currentScore = scores[i];
        int j = i - 1;

        while (j >= 0 && scores[j] > currentScore) {
            ids[j + 1] = ids[j];
            scores[j + 1] = scores[j];
            j--;
        }

        ids[j + 1] = currentId;
        scores[j + 1] = currentScore;
    }
}

int select_ambulance(int graph[MAX_NODES][MAX_NODES],
                     Ambulance ambulances[], int count,
                     int patientNode, int *distance) {
    int ids[MAX_AMBULANCES];
    int scores[MAX_AMBULANCES];
    int candidates = 0;

    if (count < 0 || count > MAX_AMBULANCES ||
        patientNode < 0 || patientNode >= MAX_NODES) {
        return -1;
    }

    for (int i = 0; i < count; i++) {
        if (ambulances[i].available &&
            ambulances[i].currentNode >= 0 &&
            ambulances[i].currentNode < MAX_NODES) {
            DijkstraResult result = dijkstra(graph, ambulances[i].currentNode);

            if (result.dist[patientNode] < INF) {
                ids[candidates] = i;
                scores[candidates] = result.dist[patientNode];
                candidates++;
            }
        }
    }

    if (candidates == 0) {
        return -1;
    }

    insertion_sort_candidates(ids, scores, candidates);
    *distance = scores[0];
    return ids[0];
}

int select_hospital(int graph[MAX_NODES][MAX_NODES],
                    Hospital hospitals[], int count,
                    int patientNode, int *distance) {
    int ids[MAX_HOSPITALS];
    int scores[MAX_HOSPITALS];
    int candidates = 0;
    DijkstraResult result;

    if (count < 0 || count > MAX_HOSPITALS ||
        patientNode < 0 || patientNode >= MAX_NODES) {
        return -1;
    }

    result = dijkstra(graph, patientNode);

    for (int i = 0; i < count; i++) {
        if (hospitals[i].availableBeds > 0 &&
            hospitals[i].emergencyOpen &&
            hospitals[i].node >= 0 && hospitals[i].node < MAX_NODES &&
            result.dist[hospitals[i].node] < INF) {
            ids[candidates] = i;
            scores[candidates] = result.dist[hospitals[i].node];
            candidates++;
        }
    }

    if (candidates == 0) {
        return -1;
    }

    insertion_sort_candidates(ids, scores, candidates);
    *distance = scores[0];
    return ids[0];
}

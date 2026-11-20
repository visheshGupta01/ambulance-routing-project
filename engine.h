#ifndef ENGINE_H
#define ENGINE_H

#define MAX_NODES 12
#define MAX_AMBULANCES 16
#define MAX_HOSPITALS 16
#define INF 1000000000

typedef struct {
    int id;
    char name[32];
    int available;
    int currentNode;
} Ambulance;

typedef struct {
    int id;
    char name[48];
    int node;
    int availableBeds;
    int emergencyOpen;
} Hospital;

typedef struct {
    int dist[MAX_NODES];
    int prev[MAX_NODES];
    int explored;
} DijkstraResult;

DijkstraResult dijkstra(int graph[MAX_NODES][MAX_NODES], int source);
int reconstruct_path(const DijkstraResult *result, int source, int target,
                     int path[MAX_NODES]);
void insertion_sort_candidates(int ids[], int scores[], int count);
int select_ambulance(int graph[MAX_NODES][MAX_NODES], Ambulance ambulances[],
                     int count, int patientNode, int *distance);
int select_hospital(int graph[MAX_NODES][MAX_NODES], Hospital hospitals[],
                    int count, int patientNode, int *distance);

#endif

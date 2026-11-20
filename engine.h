#ifndef ENGINE_H
#define ENGINE_H

#define MAX_NODES 12
#define MAX_AMBULANCES 16
#define MAX_HOSPITALS 16
#define INF 999999

typedef struct {
    char name[32];
    int location;
    int available;
} Ambulance;

typedef struct {
    char name[48];
    int location;
    int beds;
} Hospital;

typedef struct {
    int distance[MAX_NODES];
    int previous[MAX_NODES];
} ShortestPath;

ShortestPath dijkstra(int graph[MAX_NODES][MAX_NODES], int start);
int get_path(ShortestPath result, int start, int end, int path[]);
void sort_by_distance(int indexes[], int distances[], int count);
int find_ambulance(int graph[MAX_NODES][MAX_NODES],
                   Ambulance ambulances[], int count, int patient, int *distance);
int find_hospital(int graph[MAX_NODES][MAX_NODES],
                  Hospital hospitals[], int count, int patient, int *distance);

#endif

#include <stdio.h>
#include "engine.h"

#define AMBULANCE_COUNT 6
#define HOSPITAL_COUNT 4

/* The city has 12 locations. */
char *locations[MAX_NODES] = {
    "University", "Main Market", "Railway Station", "City Center",
    "Residential Area", "Industrial Area", "Highway Junction",
    "Hospital Zone", "Airport Road", "North Square",
    "Medical District", "East Colony"
};

void add_road(int graph[MAX_NODES][MAX_NODES], int a, int b, int distance) {
    graph[a][b] = distance;
    graph[b][a] = distance;
}

/* Read a menu choice or location number. */
int read_number(char message[], int min, int max) {
    int number;

    while (1) {
        printf("%s", message);

        if (scanf("%d", &number) != 1) {
            if (feof(stdin))
                return -1;
            printf("Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');

        if (number >= min && number <= max)
            return number;

        printf("Choose a number between %d and %d.\n", min, max);
    }
}

void show_locations(void) {
    printf("\nCITY LOCATIONS\n");
    for (int i = 0; i < MAX_NODES; i++)
        printf("%d. %s\n", i + 1, locations[i]);
}

void show_roads(int graph[MAX_NODES][MAX_NODES]) {
    printf("\nROAD NETWORK\n");
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = i + 1; j < MAX_NODES; j++) {
            if (graph[i][j] > 0)
                printf("%s <-> %s : %d km\n",
                       locations[i], locations[j], graph[i][j]);
        }
    }
}

void print_path(int path[], int count) {
    for (int i = 0; i < count; i++) {
        printf("%s", locations[path[i]]);
        if (i < count - 1)
            printf(" -> ");
    }
    printf("\n");
}

void shortest_route(int graph[MAX_NODES][MAX_NODES]) {
    int start, end, path[MAX_NODES], count;
    ShortestPath result;

    show_locations();
    start = read_number("Starting location: ", 1, MAX_NODES) - 1;
    if (start < 0) return;

    end = read_number("Destination: ", 1, MAX_NODES) - 1;
    if (end < 0) return;

    result = dijkstra(graph, start);
    count = get_path(result, start, end, path);

    if (count == 0) {
        printf("No route found.\n");
        return;
    }

    printf("\nShortest route: ");
    print_path(path, count);
    printf("Distance: %d km\n", result.distance[end]);
}

void show_resources(Ambulance ambulances[], Hospital hospitals[]) {
    printf("\nAMBULANCES\n");
    for (int i = 0; i < AMBULANCE_COUNT; i++) {
        printf("%s | %s | %s\n", ambulances[i].name,
               locations[ambulances[i].location],
               ambulances[i].available ? "Available" : "Busy");
    }

    printf("\nHOSPITALS\n");
    for (int i = 0; i < HOSPITAL_COUNT; i++) {
        printf("%s | %s | Beds: %d\n", hospitals[i].name,
               locations[hospitals[i].location], hospitals[i].beds);
    }
}

void dispatch(int graph[MAX_NODES][MAX_NODES],
              Ambulance ambulances[], Hospital hospitals[]) {
    int patient, ambulance, hospital;
    int ambulanceDistance, hospitalDistance;
    int path[MAX_NODES], count;
    ShortestPath result;

    show_locations();
    patient = read_number("Patient location: ", 1, MAX_NODES) - 1;
    if (patient < 0) return;

    ambulance = find_ambulance(graph, ambulances, AMBULANCE_COUNT,
                               patient, &ambulanceDistance);
    hospital = find_hospital(graph, hospitals, HOSPITAL_COUNT,
                             patient, &hospitalDistance);

    if (ambulance == -1) {
        printf("No ambulance is available.\n");
        return;
    }

    if (hospital == -1) {
        printf("No hospital has an available bed.\n");
        return;
    }

    printf("\nAMBULANCE DISPATCHED\n");
    printf("Ambulance: %s\n", ambulances[ambulance].name);

    result = dijkstra(graph, ambulances[ambulance].location);
    count = get_path(result, ambulances[ambulance].location, patient, path);
    printf("Ambulance route: ");
    print_path(path, count);
    printf("Distance to patient: %d km\n", ambulanceDistance);

    printf("Hospital: %s\n", hospitals[hospital].name);
    result = dijkstra(graph, patient);
    count = get_path(result, patient, hospitals[hospital].location, path);
    printf("Hospital route: ");
    print_path(path, count);
    printf("Distance to hospital: %d km\n", hospitalDistance);

    printf("Total distance: %d km\n", ambulanceDistance + hospitalDistance);

    /* Update the selected ambulance and hospital. */
    ambulances[ambulance].available = 0;
    hospitals[hospital].beds--;
}

int main(void) {
    int graph[MAX_NODES][MAX_NODES] = {{0}};
    int choice;

    Ambulance ambulances[AMBULANCE_COUNT] = {
        {"AMB-101", 0, 1}, {"AMB-102", 3, 0},
        {"AMB-103", 5, 1}, {"AMB-104", 8, 1},
        {"AMB-105", 10, 0}, {"AMB-106", 11, 1}
    };

    Hospital hospitals[HOSPITAL_COUNT] = {
        {"City Hospital", 7, 12},
        {"Central Medical Center", 9, 0},
        {"University Hospital", 10, 7},
        {"North Trauma Center", 2, 5}
    };

    /* Add roads and their distances to the graph. */
    add_road(graph, 0, 1, 4);
    add_road(graph, 0, 2, 7);
    add_road(graph, 1, 3, 3);
    add_road(graph, 1, 4, 6);
    add_road(graph, 2, 3, 2);
    add_road(graph, 2, 5, 5);
    add_road(graph, 3, 4, 2);
    add_road(graph, 3, 6, 4);
    add_road(graph, 4, 7, 3);
    add_road(graph, 5, 6, 2);
    add_road(graph, 5, 8, 6);
    add_road(graph, 6, 7, 2);
    add_road(graph, 6, 9, 5);
    add_road(graph, 7, 10, 4);
    add_road(graph, 8, 9, 2);
    add_road(graph, 9, 10, 3);
    add_road(graph, 10, 11, 2);
    add_road(graph, 7, 11, 6);

    printf("\nSMART AMBULANCE ROUTING SYSTEM\n");

    do {
        printf("\n1. Show road network\n");
        printf("2. Find shortest route\n");
        printf("3. Dispatch ambulance\n");
        printf("4. Show ambulances and hospitals\n");
        printf("5. Exit\n");

        choice = read_number("Enter choice: ", 1, 5);
        if (choice == -1)
            break;

        switch (choice) {
            case 1:
                show_roads(graph);
                break;
            case 2:
                shortest_route(graph);
                break;
            case 3:
                dispatch(graph, ambulances, hospitals);
                break;
            case 4:
                show_resources(ambulances, hospitals);
                break;
            case 5:
                printf("Goodbye!\n");
                break;
        }
    } while (choice != 5);

    return 0;
}

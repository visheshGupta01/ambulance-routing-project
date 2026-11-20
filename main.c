#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine.h"

#define AMBULANCE_COUNT 6
#define HOSPITAL_COUNT 4

static const char *locations[MAX_NODES] = {
    "University", "Main Market", "Railway Station", "City Center",
    "Residential Area", "Industrial Area", "Highway Junction",
    "Hospital Zone", "Airport Road", "North Square",
    "Medical District", "East Colony"
};

static void add_road(int graph[MAX_NODES][MAX_NODES], int a, int b, int km) {
    graph[a][b] = km;
    graph[b][a] = km;
}

static int read_number(const char *message, int minimum, int maximum) {
    char input[100];
    char *end;
    long value;

    while (1) {
        printf("%s", message);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            return -1;
        }

        value = strtol(input, &end, 10);
        while (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r') {
            end++;
        }

        if (end != input && *end == '\0' &&
            value >= minimum && value <= maximum) {
            return (int)value;
        }

        printf("Invalid input. Enter a number from %d to %d.\n",
               minimum, maximum);
    }
}

static void show_locations(void) {
    printf("\nCITY LOCATIONS\n");
    printf("------------------------------\n");
    for (int i = 0; i < MAX_NODES; i++) {
        printf("%2d. %s\n", i + 1, locations[i]);
    }
}

static void show_roads(int graph[MAX_NODES][MAX_NODES]) {
    printf("\nROAD NETWORK (DISTANCE IN KM)\n");
    printf("----------------------------------------------\n");
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = i + 1; j < MAX_NODES; j++) {
            if (graph[i][j] > 0) {
                printf("%-20s <-> %-20s %2d km\n",
                       locations[i], locations[j], graph[i][j]);
            }
        }
    }
}

static void show_path(const int path[], int count) {
    for (int i = 0; i < count; i++) {
        printf("%s", locations[path[i]]);
        if (i + 1 < count) {
            printf(" -> ");
        }
    }
    printf("\n");
}

static void find_shortest_route(int graph[MAX_NODES][MAX_NODES]) {
    int source, destination, path[MAX_NODES], count;
    DijkstraResult result;

    show_locations();
    source = read_number("\nEnter source location number: ", 1, MAX_NODES);
    if (source == -1) return;
    destination = read_number("Enter destination location number: ",
                              1, MAX_NODES);
    if (destination == -1) return;

    source--;
    destination--;
    result = dijkstra(graph, source);
    count = reconstruct_path(&result, source, destination, path);

    printf("\nSHORTEST ROUTE RESULT\n");
    printf("------------------------------\n");
    if (count == 0) {
        printf("No route exists between these locations.\n");
        return;
    }

    printf("Route: ");
    show_path(path, count);
    printf("Total distance: %d km\n", result.dist[destination]);
    printf("Locations explored by Dijkstra: %d\n", result.explored);
}

static void show_resources(const Ambulance ambulances[],
                           const Hospital hospitals[]) {
    printf("\nAMBULANCE STATUS\n");
    printf("----------------------------------------------\n");
    printf("%-12s %-25s %-18s\n", "ID", "Current location", "Status");
    for (int i = 0; i < AMBULANCE_COUNT; i++) {
        printf("%-12s %-25s %-18s\n",
               ambulances[i].name,
               locations[ambulances[i].currentNode],
               ambulances[i].available ? "Available" : "Dispatched");
    }

    printf("\nHOSPITAL STATUS\n");
    printf("----------------------------------------------\n");
    printf("%-25s %-20s %s\n", "Hospital", "Location", "Beds");
    for (int i = 0; i < HOSPITAL_COUNT; i++) {
        printf("%-25s %-20s %d\n",
               hospitals[i].name,
               locations[hospitals[i].node],
               hospitals[i].availableBeds);
    }
}

static void dispatch_emergency(int graph[MAX_NODES][MAX_NODES],
                               Ambulance ambulances[],
                               Hospital hospitals[]) {
    int patientNode, ambulanceDistance, hospitalDistance;
    int ambulanceIndex, hospitalIndex;
    int ambulancePath[MAX_NODES], hospitalPath[MAX_NODES];
    int ambulancePathCount, hospitalPathCount;
    DijkstraResult ambulanceResult, hospitalResult;

    show_locations();
    patientNode = read_number("\nEnter patient location number: ",
                              1, MAX_NODES);
    if (patientNode == -1) return;
    patientNode--;

    ambulanceIndex = select_ambulance(graph, ambulances, AMBULANCE_COUNT,
                                      patientNode, &ambulanceDistance);
    if (ambulanceIndex == -1) {
        printf("\nNo available ambulance can reach this location.\n");
        return;
    }

    hospitalIndex = select_hospital(graph, hospitals, HOSPITAL_COUNT,
                                    patientNode, &hospitalDistance);
    if (hospitalIndex == -1) {
        printf("\nNo open hospital with available beds can be reached.\n");
        return;
    }

    ambulanceResult = dijkstra(graph, ambulances[ambulanceIndex].currentNode);
    hospitalResult = dijkstra(graph, patientNode);
    ambulancePathCount = reconstruct_path(
        &ambulanceResult, ambulances[ambulanceIndex].currentNode,
        patientNode, ambulancePath);
    hospitalPathCount = reconstruct_path(
        &hospitalResult, patientNode, hospitals[hospitalIndex].node,
        hospitalPath);

    if (ambulancePathCount == 0 || hospitalPathCount == 0) {
        printf("\nDispatch cancelled: route calculation failed.\n");
        return;
    }

    printf("\nEMERGENCY DISPATCH CONFIRMED\n");
    printf("==============================================\n");
    printf("Selected ambulance: %s\n", ambulances[ambulanceIndex].name);
    printf("Ambulance to patient (%d km): ", ambulanceDistance);
    show_path(ambulancePath, ambulancePathCount);

    printf("Selected hospital: %s\n", hospitals[hospitalIndex].name);
    printf("Patient to hospital (%d km): ", hospitalDistance);
    show_path(hospitalPath, hospitalPathCount);

    printf("Total travel distance: %d km\n",
           ambulanceDistance + hospitalDistance);
    printf("Algorithms: Dijkstra + Greedy Selection + Insertion Sort\n");

    /* Update the demo data so the next dispatch sees the changed resources. */
    ambulances[ambulanceIndex].available = 0;
    hospitals[hospitalIndex].availableBeds--;

    printf("\nResource status updated for this program session.\n");
}

int main(void) {
    int graph[MAX_NODES][MAX_NODES] = {{0}};
    Ambulance ambulances[AMBULANCE_COUNT] = {
        {101, "AMB-101", 1, 0}, {102, "AMB-102", 0, 3},
        {103, "AMB-103", 1, 5}, {104, "AMB-104", 1, 8},
        {105, "AMB-105", 0, 10}, {106, "AMB-106", 1, 11}
    };
    Hospital hospitals[HOSPITAL_COUNT] = {
        {1, "City Hospital", 7, 12, 1},
        {2, "Central Medical Center", 9, 0, 1},
        {3, "University Hospital", 10, 7, 1},
        {4, "North Trauma Center", 2, 5, 1}
    };
    int choice;

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

    printf("==============================================\n");
    printf("   SMART EMERGENCY AMBULANCE ROUTING SYSTEM\n");
    printf("             C LANGUAGE | CLI\n");
    printf("==============================================\n");

    do {
        printf("\nMAIN MENU\n");
        printf("1. Display city road network\n");
        printf("2. Find shortest route (Dijkstra)\n");
        printf("3. Dispatch an emergency ambulance\n");
        printf("4. View ambulances and hospitals\n");
        printf("5. Exit\n");

        choice = read_number("Enter your choice: ", 1, 5);
        if (choice == -1) break;

        switch (choice) {
            case 1:
                show_roads(graph);
                break;
            case 2:
                find_shortest_route(graph);
                break;
            case 3:
                dispatch_emergency(graph, ambulances, hospitals);
                break;
            case 4:
                show_resources(ambulances, hospitals);
                break;
            case 5:
                printf("\nThank you for using the system. Goodbye!\n");
                break;
        }
    } while (choice != 5);

    return 0;
}

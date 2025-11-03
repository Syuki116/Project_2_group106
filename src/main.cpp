#include <iostream>
#include <vector>
#include <chrono>
#include "trip.h"
#include "load_csv.h"
#include "grid_index.h"
#include "kd_tree.h"


static void printMenu() {
    std::cout << "Welcome to find my Taxi\n"
                 "1. Load taxi dataset\n"
                 "2. Build Grid Index\n"
                 "3. Build KD-Tree\n"
                 "4. Search trips by area (latitude/longitude range)\n"
                 "5. Compare performance (Grid vs KD-Tree)\n"
                 "6. Exit\n"
                 "Enter your choice: ";
}


int main() {
    std::vector<Trip> trips;
    // NYC-ish bounds and a reasonable grid cell size (degrees)
    GridIndex grid(40.3, 41.0, -74.3, -73.6, 0.01);
    KDTree kd;

    while (true) {
        printMenu();  // show the menu
        int choice;
        std::cin >> choice; // get user input

        // if input fails (e.g. user enters non-number), end program
        if (!std::cin) {
            return 0;
        }

        if (choice == 1) {
            std::string path = "data/yellow_tripdata_2015-01.csv";
            size_t limit = 100000;
            trips = load_trips_csv(path, limit);
        }
        else if (choice == 2) {
            auto start = std::chrono::high_resolution_clock::now();
            grid.build(trips);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            std::cout << "Grid Index built in " << elapsed.count() << " seconds.\n";
        }
        else if (choice == 3) {

        }
        else if (choice == 4) {
        }
        else if (choice == 5) {
        }
        else if (choice == 6) {
            break;
        }
        else {
            std::cout << "Invalid choice.\n\n";
        }
    }
}
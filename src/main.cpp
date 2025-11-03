#include <iostream>
#include <vector>
#include <chrono>
#include <sstream>
#include "trip.h"
#include "load_csv.h"
#include "grid_index.h"
#include "kd_tree.h"

#include <filesystem>
namespace fs = std::filesystem;



static void printMenu() {
    // std::cout << "Welcome to find my Taxi\n"
    //              "1. Load taxi dataset\n"
    //              "2. Build Grid Index\n"
    //              "3. Build KD-Tree\n"
    //              "4. Search trips by area (latitude/longitude range)\n"
    //              "5. Compare performance (Grid vs KD-Tree)\n"
    //              "6. Exit\n"
    //              "Enter your choice: ";
    std::cout<< "Welcome to find my Taxi\n"
    "1. Search the number of trips by area (latitude/longitude range) using Grid Index and KD-Tree\n"
    "2. Compare performance (Grid vs KD-Tree)\n"
    "3. Exit\n";
}


int main() {
    std::vector<Trip> trips;
    // NYC-ish bounds and a reasonable grid cell size (degrees)
    GridIndex grid(40.3, 41.0, -74.3, -73.6, 0.01);
    std::string path_found;
    for (const std::string& cand : {
            std::string("../data/yellow_tripdata_2015-01.csv"),
            std::string("data/yellow_tripdata_2015-01.csv"),
            std::string("../../data/yellow_tripdata_2015-01.csv")
        }) {
        if (fs::exists(cand)) { path_found = cand; break; }
        }


    size_t limit = 100000;
    //trips = load_trips_csv(path, limit);
    trips = load_trips_csv(path_found, limit);
    std::cout << "trips.size() = " << trips.size() << "\n";
    if (trips.empty()) {
        std::cerr << "ERROR: Loaded 0 trips. (Either columns weren’t found or file had no valid rows.)\n";
        return 1;
    }
    grid.build(trips);
    KDTree kd(trips);
    std::cout<<"trips.size()="<<trips.size()<<endl;
    // if (trips.size()==0) {
    //     std::cout<<"Please download it according to README.md and place it under the 'data' folder.\n"<<endl;
    //     return 0;
    // }
    while (true) {
        printMenu();  // show the menu
        int choice;
        std::cin >> choice; // get user input
        // if input fails (e.g. user enters non-number), end program
        if (!std::cin) {
            return 0;
        }

        if (choice == 1) {
            double minlat, maxlat, minlon, maxlon;
            std::cout << "Enter minLat, maxLat, minLon, and maxLon (separated by spaces): ";
            std::cin >> minlat >> maxlat >> minlon >> maxlon;
            if (std::cin.fail()) {
                std::cout << "Invalid input! Please enter four numeric values."<< std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            GridRect BoxGrid(minlat, maxlat, minlon, maxlon);
            Rect Box(minlat, maxlat, minlon, maxlon);
            vector<const Trip*> out;
            grid.rangeQuery(BoxGrid, out);
            std::cout<<"Grid Index Result: "<<out.size()<<endl;
            kd.rangeQuery(Box, out);
            std::cout<<"KD-Tree Result: "<<out.size()<<endl;
        }
        else if (choice == 2)  {
            vector<const Trip*> gridOut;
            vector<const Trip*> treeOut;
            double minLat, maxLat, minLon, maxLon;
            std::cout << "Enter minLat maxLat minLon maxLon: ";
            std::cin >> minLat >> maxLat >> minLon >> maxLon;
            if (std::cin.fail()) {
                std::cout << "Invalid input! Please enter four numeric values."<< std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            GridRect gridBox(minLat, maxLat, minLon, maxLon);
            Rect treeBox(minLat, maxLat, minLon, maxLon);
            auto gridStart = std::chrono::high_resolution_clock::now();
            grid.rangeQuery(gridBox, gridOut);
            auto gridEnd = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> gridElapsed = gridEnd - gridStart;
            std::cout << "Grid Index built in " << gridElapsed.count() << " seconds.\n";

            auto treeStart = std::chrono::high_resolution_clock::now();
            kd.rangeQuery(treeBox, treeOut);
            auto treeEnd = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> treeElapsed = treeEnd - treeStart;
            std::cout << "KD-Tree built in " << treeElapsed.count() << " seconds.\n";

            if (treeElapsed.count() < gridElapsed.count()) std::cout << "KD-Tree approach is faster.\n";
            else std::cout << "Grid index approach is faster.\n";
        }
        else if (choice == 3) {
            break;
        }
        else {
            std::cout << "Invalid choice.\n\n";
        }
    }
    return 0;
}
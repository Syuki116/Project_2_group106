//
// Created by Xinyu Shen on 10/26/25.
//

#ifndef TRIP_H
#define TRIP_H
#pragma once
#include <string>
using namespace std;


// Each Trip represents one row from the CSV file.
// We only care about those 5 columns:

struct Trip {
    double pickup_lat{};    // pickup latitude
    double pickup_lon{};    // pickup longitude
    double dropoff_lat{};   // dropoff latitude
    double dropoff_lon{};   // dropoff longtitude
    string pickup_datetime; // pickup time in string
};


#endif //TRIP_H

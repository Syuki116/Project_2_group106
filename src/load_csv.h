//
// Created by Xinyu Shen on 10/26/25.
//

#ifndef LOAD_CSV_H
#define LOAD_CSV_H
#pragma once
#include <vector>
#include <string>
#include "trip.h"

// Function declaration: reads a CSV file and returns a vector of Trip objects.
// If 'limit' > 0, stops reading after that many valid rows.

std::vector<Trip> load_trips_csv(const std::string& path,
    size_t limit = 0);

#endif

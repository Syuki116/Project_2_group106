//
// Created by Xinyu Shen on 10/26/25.
//
#include "load_csv.h"
#include <fstream>              // for file input
#include <sstream>
#include <unordered_map>
#include <algorithm>

// helper: splits one line by commas
static std::vector<std::string> split_simple(const std::string& line) {
    // Split on commas
    std::vector<std::string> out;
    std::stringstream ss(line);
    std::string cell;
    // read text between commas and push into vector
    while (std::getline(ss, cell, ',')) out.push_back(cell);
    return out;
}

static std::string lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

std::vector<Trip> load_trips_csv(const std::string& path, size_t limit) {
    std::vector<Trip> trips;

    std::ifstream in(path);
    if (!in) return trips;

    // --- header -> column indices we care about
    std::string header;
    if (!std::getline(in, header)) return trips;
    auto cols = split_simple(header);
    std::unordered_map<std::string,int> pos;
    for (int i = 0; i < (int)cols.size(); ++i) pos[lower(cols[i])] = i;

    auto find1 = [&](std::initializer_list<const char*> names)->int {
        for (auto n : names) { auto it = pos.find(n); if (it != pos.end()) return it->second; }
        return -1;
    };

    const int i_pulat  = find1({"pickup_latitude"});
    const int i_pulon  = find1({"pickup_longitude"});
    const int i_dolat  = find1({"dropoff_latitude"});
    const int i_dolon  = find1({"dropoff_longitude"});
    const int i_pudt   = find1({"tpep_pickup_datetime","pickup_datetime"});

    if (i_pulat<0 || i_pulon<0 || i_dolat<0 || i_dolon<0) {
        // Required columns missing; return empty.
        return trips;
    }

    // --- rows
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        auto cells = split_simple(line);
        auto get = [&](int i)->std::string {
            return (i >= 0 && i < (int)cells.size()) ? cells[i] : std::string{};
        };

        Trip t;
        try {
            t.pickup_lat  = std::stod(get(i_pulat));
            t.pickup_lon  = std::stod(get(i_pulon));
            t.dropoff_lat = std::stod(get(i_dolat));
            t.dropoff_lon = std::stod(get(i_dolon));
        } catch (...) {
            continue; // skip malformed line
        }
        t.pickup_datetime = get(i_pudt);

        trips.push_back(std::move(t));
        if (limit && trips.size() >= limit) break;
    }
    return trips;
}

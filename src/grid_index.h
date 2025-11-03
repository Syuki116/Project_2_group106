//
// Created by Xinyu Shen on 10/26/25.
//

#ifndef GRID_INDEX_H
#define GRID_INDEX_H
#pragma once
#include <vector>
#include "trip.h"
#include <unordered_map>
#include <utility>
using std::vector;
using std::pair;

// Create a Rect object for querySearch function, consistent to kd_tree.h
struct GridRect {
    double minLat{};
    double maxLat{};
    double minLon{};
    double maxLon{};

    GridRect() = default;
    GridRect(double min_lat, double max_lat, double min_lon, double max_lon)
        : minLat(min_lat), maxLat(max_lat), minLon(min_lon), maxLon(max_lon)
    {
        normalize();
    }

    void normalize() {
        if (minLat > maxLat) std::swap(minLat, maxLat);
        if (minLon > maxLon) std::swap(minLon, maxLon);
    }

    bool contains(double lat, double lon) const {
        return lat >= minLat && lat <= maxLat &&
               lon >= minLon && lon <= maxLon;
    }
};

// Pickup information is used in the default grid that is created
class GridIndex {
public:
    GridIndex(double min_lat, double max_lat,
              double min_lon, double max_lon,
              double cell_size);
    void build(const vector<Trip>& trips);
    void rangeQuery(const GridRect& box, vector<const Trip*>& out) const;
    void clear();

private:
    double minLat{}, maxLat{};
    double minLon{}, maxLon{};
    double cellSize{};
    int rows{}, cols{};

    // Every object in grid contains a list of trips
    // Row-majored grid i.e. grid[row][col], row=lat and col=lon
    vector<vector<vector<const Trip*>>> grid;

    // Helper functions
    // Find the corresponding cell in the grid according to latitude and longitude
    pair<int,int> toCell(double lat, double lon) const;
    bool validCell(int row, int col) const;
};

#endif //GRID_INDEX_H

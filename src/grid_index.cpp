//
// Created by Xinyu Shen on 10/26/25.
//

#include "grid_index.h"
#include <algorithm>
using namespace std;

GridIndex::GridIndex(double min_lat, double max_lat,
                     double min_lon, double max_lon,
                     double cell_size)
    : minLat(min_lat), maxLat(max_lat),
      minLon(min_lon), maxLon(max_lon),
      cellSize(cell_size)
{
    // Ensure min < max
    if (minLat > maxLat) swap(minLat, maxLat);
    if (minLon > maxLon) swap(minLon, maxLon);
    rows = static_cast<int>((maxLat - minLat) / cellSize) + 1;
    cols = static_cast<int>((maxLon - minLon) / cellSize) + 1;

    // Initialize each grid[row][col] with a list of Trip* object
    grid.assign(rows, vector<vector<const Trip*>>(cols));
}

void GridIndex::clear() {
    for (auto& row : grid)
        for (auto& cell : row)
            cell.clear();
}

pair<int,int> GridIndex::toCell(double lat, double lon) const {
    int r = static_cast<int>((lat - minLat) / cellSize);
    int c = static_cast<int>((lon - minLon) / cellSize);
    return {r, c};
}

bool GridIndex::validCell(int row, int col) const {
    return (row >= 0 && row < rows &&
            col >= 0 && col < cols);
}

// Build the grid through insertion of list of Trip
// Here Trip is used instead of Trip* because it is used in main.cpp
void GridIndex::build(const vector<Trip>& trips) {
    clear();
    for (const auto& t : trips) {
        auto cell = toCell(t.pickup_lat, t.pickup_lon);
        int r = cell.first;
        int c = cell.second;
        if (validCell(r, c))
            grid[r][c].push_back(&t);
    }
}

void GridIndex::rangeQuery(const GridRect& box, vector<const Trip*>& out) const {
    out.clear();

    // Range of query
    int r1 = static_cast<int>((box.minLat - minLat) / cellSize);
    int r2 = static_cast<int>((box.maxLat - minLat) / cellSize);
    int c1 = static_cast<int>((box.minLon - minLon) / cellSize);
    int c2 = static_cast<int>((box.maxLon - minLon) / cellSize);

    r1 = max(r1, 0);
    c1 = max(c1, 0);
    r2 = min(r2, rows - 1);
    c2 = min(c2, cols - 1);

    for (int r = r1; r <= r2; ++r) {
        for (int c = c1; c <= c2; ++c) {
            for (const Trip* t : grid[r][c]) {
                double lat = t->pickup_lat;
                double lon = t->pickup_lon;
                if (box.contains(lat, lon))
                    out.push_back(t);
            }
        }
    }
}
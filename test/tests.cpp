#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
using Catch::Approx;

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "grid_index.h"
#include <fstream>
#include "kd_tree.h"
#include "trip.h"
#include "load_csv.h"
using namespace std;
using std::string;
using std::vector;

TEST_CASE("Sanity", "[smoke]") {
    REQUIRE(1 + 1 == 2);
}


// --- tiny helper to write a throwaway CSV file ---
static std::string write_csv(const std::string& text, const std::string& path = "mini.csv") {
    std::ofstream out(path); out << text;
    return path;
}

// 1) load_csv: reads rows and returns trips
TEST_CASE("load_csv: minimal happy path") {
    const char* csv =
        "pickup_latitude,pickup_longitude,dropoff_latitude,dropoff_longitude,tpep_pickup_datetime\n"
        "40.70,-73.95,40.80,-73.90,2015-01-01 00:00:00\n"
        "40.72,-73.97,40.81,-73.92,2015-01-01 00:01:00\n";

    auto path  = write_csv(csv);
    auto trips = load_trips_csv(path, /*limit=*/0);   // 0 = no limit in many loaders

    REQUIRE(trips.size() == 2);
    CHECK(trips[0].pickup_lat == Approx(40.70));
    CHECK(trips[0].pickup_lon == Approx(-73.95));
}

// 2) search: KDTree range query returns the point in the box
TEST_CASE("KDTree: simple rangeQuery on pickup coords") {
    std::vector<Trip> trips(3);
    trips[0].pickup_lat = 40.700; trips[0].pickup_lon = -73.950; // inside
    trips[1].pickup_lat = 41.200; trips[1].pickup_lon = -73.500; // outside
    trips[2].pickup_lat = 40.720; trips[2].pickup_lon = -73.970; // inside (but set box to exclude)

    KDTree kd;
    kd.build(trips, KDTree::CoordMode::Pickup);

    Rect box; // adjust names if your Rect differs
    box.minLat = 40.695; box.maxLat = 40.705;
    box.minLon = -73.955; box.maxLon = -73.945;

    std::vector<const Trip*> out;
    kd.rangeQuery(box, out);

    REQUIRE(out.size() == 1);
    CHECK(out[0]->pickup_lat == Approx(40.700));
    CHECK(out[0]->pickup_lon == Approx(-73.950));
}

// 3) build: GridIndex build + broad query returns all points
TEST_CASE("GridIndex: build then broad rangeQuery returns all") {
    std::vector<Trip> trips(3);
    trips[0].pickup_lat = 40.70; trips[0].pickup_lon = -73.95;
    trips[1].pickup_lat = 40.71; trips[1].pickup_lon = -73.96;
    trips[2].pickup_lat = 40.72; trips[2].pickup_lon = -73.97;

    // bounds that cover NYC-ish area; cell size can be coarse
    GridIndex gi(40.3, 41.0, -74.3, -73.6, /*cellSize=*/0.02);
    gi.build(trips);

    GridRect box;
    box.minLat = 40.60; box.maxLat = 40.80;
    box.minLon = -74.10; box.maxLon = -73.80;

    std::vector<const Trip*> out;
    gi.rangeQuery(box, out);

    REQUIRE(out.size() == 3);
}
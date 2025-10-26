#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "grid_index.h"
#include "kd_tree.h"
#include "trip.h"
using namespace std;
using std::string;
using std::vector;

TEST_CASE("Sanity", "[smoke]") {
    REQUIRE(1 + 1 == 2);
}


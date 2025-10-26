//
// Created by Xinyu Shen on 10/26/25.
//

#ifndef KD_TREE_H
#define KD_TREE_H
#pragma once
#include <vector>
#include "trip.h"

class KDTree {
public:
    void build(const std::vector<Trip>& trips);
    std::vector<const Trip*> rangeQuery(double latMin, double lonMin, double latMax, double lonMax) const;

private:
    const std::vector<Trip>* data{nullptr};
};

#endif //KD_TREE_H

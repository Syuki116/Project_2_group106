//
// Created by Xinyu Shen on 10/26/25.
//
//I noticed that there is "using namespace std;" in trip.h, and I didn't change it.
//Remember to check if that will pollute your other files.

// KDTree does NOT own any Trip data. It only stores const Trip* pointers
// that refer to elements inside an external std::vector<Trip>.
// Therefore, the caller must ensure that:
// 1. The source vector<Trip> remains alive for the entire lifetime of KDTree.
// 2. The source vector<Trip> is NOT modified in any way that may reallocate
//    its internal storage (e.g., push_back, resize, insert) after the tree is built.
// Also, please declare 'trips' BEFORE 'tree' so that
// 'tree' is destroyed first (in reverse order of declaration).
// Example:
//     std::vector<Trip> trips;  // must be declared first
//     KDTree tree;              // declared after trips

//KDTree is not allowed to be copied. Do NOT use stuff like  KDTree t2=t1;
#pragma once
#ifndef KD_TREE_H
#define KD_TREE_H
#include <vector>
#include <utility>
#include "trip.h"
using std::vector;
using std::swap;
struct Rect {
    double minLat{};
    double maxLat{};
    double minLon{};
    double maxLon{};

    Rect() = default;

    Rect(double min_lat, double max_lat, double min_lon, double max_lon)
        : minLat(min_lat), maxLat(max_lat), minLon(min_lon), maxLon(max_lon)
    {
        normalize();
    }
    void normalize() {
        if (minLat > maxLat) std::swap(minLat, maxLat);
        if (minLon > maxLon) std::swap(minLon, maxLon);
    }

    // check if the point is in the rect, used in rangeQuery also.
    bool contains(double lat, double lon) const {
        return lat >= minLat && lat <= maxLat &&
               lon >= minLon && lon <= maxLon;
    }
};

//Using Pickup Tree as default.
//If dropoff tree needed, use "KDTree dropoff_tree(trips, CoordMode::Dropoff)" for instance.
//you can directly use "build" to build another new tree with trips.
class KDTree {
public:
    enum class CoordMode { Pickup, Dropoff };
    KDTree()=default;
    explicit KDTree(const vector<Trip>& trips, CoordMode mode = CoordMode::Pickup) {
        build(trips, mode);
    }
    KDTree(const KDTree&) = delete;
    KDTree& operator=(const KDTree&)=delete;
    void build(const vector<Trip>& trips, CoordMode mode = CoordMode::Pickup);
    void rangeQuery(const Rect& box, vector<const Trip*>& out) const;
    ~KDTree() {
        destroy(root);
        root = nullptr;
    }
//I used vector<const Trip*> pts  to store address, instead of storing every trip.
private:
    struct Node {
        const Trip* trip;
        int axis;       // 0=lon, 1=lat
        Node* left;
        Node* right;

        Node(const Trip* t, int a) : trip(t), axis(a), left(nullptr), right(nullptr) {}
    };

    Node* root = nullptr;
    vector<const Trip*> pts;
    CoordMode coordMode = CoordMode::Pickup;

    Node* buildRec(vector<const Trip*>::iterator first,
                   vector<const Trip*>::iterator last, int depth);
    void rangeRec(Node* node, const Rect& box, vector<const Trip*>& out) const;
    void destroy(Node* head);

};

#endif //KD_TREE_H

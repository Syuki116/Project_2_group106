//
// Created by Xinyu Shen on 10/26/25.
//

#include <algorithm>
#include "kd_tree.h"
using namespace std;


void KDTree::build(const vector<Trip>& trips, CoordMode mode) {
    coordMode = mode;
    destroy(root);
    root = nullptr;
    // use pointers rather than data itself.
    pts.clear();
    pts.reserve(trips.size());
    for (const auto& t : trips) pts.push_back(&t);
    if (!pts.empty())
        root = buildRec(pts.begin(), pts.end(), /*depth=*/0);
}
KDTree::Node* KDTree::buildRec(vector<const Trip*>::iterator first,
                               vector<const Trip*>::iterator last,
                               int depth)
{
    if (first == last) return nullptr; //base case;

    int axis = depth % 2;               // 0=lon, 1=lat;
    auto mid = first + (last - first) / 2;

    // compare according to axis and mode.
    nth_element(first, mid, last, [axis, this](const Trip* a, const Trip* b) {
        if (axis == 0) {
            double alon = (coordMode == CoordMode::Pickup) ? a->pickup_lon : a->dropoff_lon;
            double blon = (coordMode == CoordMode::Pickup) ? b->pickup_lon : b->dropoff_lon;
            return alon < blon;
        } else {
            double alat = (coordMode == CoordMode::Pickup) ? a->pickup_lat : a->dropoff_lat;
            double blat = (coordMode == CoordMode::Pickup) ? b->pickup_lat : b->dropoff_lat;
            return alat < blat;
        }
    });

    Node* node = new Node(*mid, axis);
    node->left  = buildRec(first, mid, depth + 1);
    node->right = buildRec(mid + 1, last, depth + 1);

    return node;
}
void KDTree::rangeQuery(const Rect& box, vector<const Trip*>& out) const {
    out.clear(); //Let's discuss it later.
    rangeRec(root, box, out);
}

void KDTree::rangeRec(Node* node, const Rect& box, vector<const Trip*>& out) const {
    if (node == nullptr) return;  // base case

    // choose mode and axis.
    double lat = (coordMode == CoordMode::Pickup)
        ? node->trip->pickup_lat
        : node->trip->dropoff_lat;

    double lon = (coordMode == CoordMode::Pickup)
        ? node->trip->pickup_lon
        : node->trip->dropoff_lon;

    // check if box contains this node.
    if (box.contains(lat, lon)) {
        out.push_back(node->trip);
    }
    if (node->axis == 0) {
        //check longitude
        if (box.minLon <= lon)
            rangeRec(node->left, box, out);
        if (box.maxLon >= lon)
            rangeRec(node->right, box, out);
    } else {
        //check latitude
        if (box.minLat <= lat)
            rangeRec(node->left, box, out);
        if (box.maxLat >= lat)
            rangeRec(node->right, box, out);
    }
}

void KDTree::destroy(Node *head) {
    if (!head) return;
    destroy(head->left);
    destroy(head->right);
    delete head;
}

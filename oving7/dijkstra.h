#pragma once

#include <utility>
#include <vector>
class Map;

using pii = std::pair<int, int>;
using namespace std;

// Dijkstra's algorithm without distances vector
pair<int, vector<int>> dijkstra(Map &map, int start, int end);

// Dijkstra's algorithm with distances vector
pair<int, vector<int>> dijkstra(Map &map, int start, int end, vector<int> &distances);

// Find closest landmarks for a given node and category
vector<int> closestLandmarks(Map &map, int fromNode, string category, const int numLandmarks);

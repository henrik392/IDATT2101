#pragma once

#include <utility>
#include <vector>
class Map;

using pii = std::pair<int, int>;
using namespace std;

// Dijkstra's algorithm without distances vector
pair<int, vector<int>> dijkstras(Map &map, int start, int end);

// Dijkstra's algorithm with distances vector
pair<int, vector<int>> dijkstras(Map &map, int start, int end, vector<int> &distances);
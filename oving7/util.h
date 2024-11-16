#pragma once

#include <unordered_map>
#include <vector>

using namespace std;

// Helper function to reconstruct path from start to end
vector<int> reconstructPath(int start, int end, unordered_map<int, int> &predecessors);
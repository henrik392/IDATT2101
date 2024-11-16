#include "util.h"
#include <algorithm>

using namespace std;

vector<int> reconstructPath(int start, int end, unordered_map<int, int> &predecessors) {
  vector<int> path;
  for (int at = end; at != start; at = predecessors[at]) {
    path.push_back(at);
    if (predecessors.find(at) == predecessors.end()) {
      return {};
    }
  }
  path.push_back(start);
  reverse(path.begin(), path.end());
  return path;
}
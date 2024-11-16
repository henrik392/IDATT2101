#include "dijkstras.h"
#include "map.h"
#include "util.h"
#include <algorithm>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>

pair<int, vector<int>> dijkstras(Map &map, int start, int end) {
  vector<int> distances;
  return dijkstras(map, start, end, distances);
}

pair<int, vector<int>> dijkstras(Map &map, int start, int end, vector<int> &distances) {
  unordered_map<int, int> predecessors;
  unordered_set<int> visited;

  priority_queue<pii, vector<pii>, greater<pii>> pq;
  pq.push({0, start});

  int count = 0;

  while (!pq.empty()) {
    count++;
    pii current = pq.top(); // {time, node}
    pq.pop();

    if (visited.find(current.second) != visited.end()) {
      continue;
    }

    visited.insert(current.second);

    if (end == -1) {
      distances[current.second] = current.first;
    }

    if (current.second == end) {
      cout << "Number of nodes visited: " << count << endl;
      return {current.first, reconstructPath(start, end, predecessors)};
    }

    for (pii edge : map.getNieghbors(current.second)) {
      int nextNode = edge.first;
      int newDist = current.first + edge.second;

      if (visited.find(nextNode) == visited.end()) {
        pq.push({newDist, nextNode});
        predecessors[nextNode] = current.second;
      }
    }
  }

  return {-1, {}};
}

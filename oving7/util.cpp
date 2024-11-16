#include "util.h"
#include "map.h"
#include <algorithm>
#include <fstream>
#include <iostream>

using namespace std;

typedef pair<double, double> pdd;

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

void writeNodeCoordintesToFile(vector<int> shortestPath, Map &map, string outputFileName, int maxWaypoints) {
  ofstream file(outputFileName);
  if (!file) {
    cout << "Error opening file: " << outputFileName << endl;
    return;
  }

  // Step size, rounds up to nearest integer
  int step = (shortestPath.size() + maxWaypoints - 1) / maxWaypoints;

  if (step == 0) {
    step = 1;
  }
  for (size_t i = 0; i < shortestPath.size(); i += step) {
    // Last should be included
    if (i + step >= shortestPath.size()) {
      i = shortestPath.size() - 1;
    }

    int node = shortestPath[i];
    file << map.getNodeCoordinate(node).first << "," << map.getNodeCoordinate(node).second << "\n";
  }

  file.close();
}

string formatTravelTime(int time) {
  int seconds = time / 100;
  int minutes = seconds / 60;
  int hours = minutes / 60;
  minutes %= 60;
  seconds %= 60;

  return to_string(hours) + " hours, " + to_string(minutes) + " minutes, " + to_string(seconds) + " seconds.";
}

void displayResults(chrono::steady_clock::time_point start_time, chrono::steady_clock::time_point end_time, int travelTime, string startName, string endName) {
  cout << "Time taken for algorithm to run was : " << chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count() << "ms" << endl;
  cout << "Time to travel from " << startName << " to " << endName << " is " << formatTravelTime(travelTime) << endl;
}

bool validateLandmarks(Map &map, string fromLandmark, string toLandmark) {
  if (map.interestPointNameToNode(fromLandmark) == -1) {
    cout << "Could not find start location: " << fromLandmark << endl;
    return false;
  } else if (map.interestPointNameToNode(toLandmark) == -1) {
    cout << "Could not find end location: " << toLandmark << endl;
    return false;
  }

  return true;
}

bool validateLandmark(Map &map, string landmark) {
  if (map.interestPointNameToNode(landmark) == -1) {
    cout << "Could not find location: " << landmark << endl;
    return false;
  }

  return true;
}

bool validateNode(Map &map, int node) {
  if (node < 0 || node >= (int)map.getSize()) {
    cout << "Could not find node: " << node << endl;
    return false;
  }

  return false;
}

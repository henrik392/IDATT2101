#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <locale>
#include <map>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "dijkstras.h"
#include "map.h"
#include "util.h"

using namespace std;

#define pii pair<int, int>
#define pdd pair<double, double>

class ALTHelper {
private:
  Map &map;
  string fromLandmarkFile, toLandmarkFile;
  vector<string> landmarks;
  vector<vector<int>> fromLandmark;
  vector<vector<int>> toLandmark;

  void readAltFile(ifstream &file, vector<vector<int>> &distMatrix) {
    int n = 0;
    file >> n;
    if (n != (int)distMatrix.size()) {
      cout << "Error reading file. The `landmarks` dimension does not match the file." << endl;
      return;
    }

    for (size_t i = 0; i < distMatrix.size(); i++) {
      for (size_t j = 0; j < map.getSize(); j++) {
        file >> distMatrix[i][j];
      }
    }

    file.close();
  }

  void writeAltFile(ofstream &file, vector<vector<int>> &distMatrix) {
    file << distMatrix.size() << endl;
    for (size_t i = 0; i < distMatrix.size(); i++) {
      for (size_t j = 0; j < distMatrix[i].size(); j++) {
        file << distMatrix[i][j] << " ";
      }
      file << endl;
    }
  }

  void calculateLandmarkDistances(vector<vector<int>> &distMatrix) {
    for (size_t i = 0; i < landmarks.size(); i++) {
      int node = map.interestPointNameToNode(landmarks[i]);
      if (node == -1) {
        cout << "Could not find node for landmark: " << landmarks[i] << endl;
        continue;
      }

      cout << "Calculating distances for " << landmarks[i] << "..." << endl;
      dijkstras(map, node, -1, distMatrix[i]);
    }
  }

  void preprocessAlt(string landmarkFile, vector<vector<int>> &distMatrix) {
    ifstream file(landmarkFile);
    if (!file) {
      cout << "Could not find: " << landmarkFile << endl;
      cout << "Creating new file..." << endl;
      ofstream newFile(landmarkFile);
      if (!newFile) {
        cout << "Error creating file: " << landmarkFile << endl;
        return;
      }

      calculateLandmarkDistances(distMatrix);
      writeAltFile(newFile, distMatrix);
      return;
    }

    readAltFile(file, distMatrix);

    file.close();
  }

public:
  ALTHelper(Map &map, string fromLandmarkFile, string toLandmarkFile, vector<string> landmarks) : map(map), fromLandmarkFile(fromLandmarkFile), toLandmarkFile(toLandmarkFile), landmarks(landmarks) {
    fromLandmark.resize(landmarks.size(), vector<int>(map.getSize(), INT_MAX));
    toLandmark.resize(landmarks.size(), vector<int>(map.getSize(), INT_MAX));

    cout << "Preprocessing distances from landmarks." << endl;
    preprocessAlt(fromLandmarkFile, fromLandmark);
    map.reverse();

    cout << "Preprocessing distances to landmarks." << endl;
    preprocessAlt(toLandmarkFile, toLandmark);
    map.reverse();
  }

  int estimateDistance(int node, int target) {
    int estimate = 0;
    for (size_t i = 0; i < landmarks.size(); i++) {
      int landmarkToNode = fromLandmark[i][node];
      int landmarkToTarget = fromLandmark[i][target];
      estimate = max(estimate, landmarkToTarget - landmarkToNode);

      int nodeToLandmark = toLandmark[i][node];
      int targetToLandmark = toLandmark[i][target];
      estimate = max(estimate, nodeToLandmark - targetToLandmark);
    }

    return estimate;
  }
};

struct Candidate {
  int node, time, heuristic;
  bool operator>(const Candidate &other) const {
    return time + heuristic > other.time + other.heuristic;
  }
};

pair<int, vector<int>> ALT(Map &map, ALTHelper altHelper, int start, int end) {
  unordered_map<int, int> predecessors;
  unordered_set<int> visited;

  priority_queue<Candidate, vector<Candidate>, greater<Candidate>> pq;

  int initialHeuristic = altHelper.estimateDistance(start, end);
  pq.push({start, 0, initialHeuristic});

  int nodesVisitedTotal = 0;
  while (!pq.empty()) {
    Candidate current = pq.top();
    pq.pop();

    if (visited.find(current.node) != visited.end()) {
      continue;
    }

    visited.insert(current.node);

    if (current.node == end) {
      cout << "Number of nodes visited: " << nodesVisitedTotal << endl;
      return {current.time, reconstructPath(start, end, predecessors)};
    }

    for (pii edge : map.getNieghbors(current.node)) {
      int nextNode = edge.first;
      int newDist = current.time + edge.second;

      if (visited.find(nextNode) == visited.end()) {
        int heuristic = altHelper.estimateDistance(nextNode, end);
        pq.push({nextNode, newDist, heuristic});
        predecessors[nextNode] = current.node; // Update predecessor
      }
    }

    nodesVisitedTotal++;
  }

  return {-1, {}};
}

void writeCoordintePathFromLandmarksToFile(Map &map, ALTHelper altHelper, string startName, string endName, string outputFileName) {
  ofstream file(outputFileName);
  if (!file) {
    cout << "Error opening file: "
         << outputFileName << endl;
    return;
  }

  cout << "Finding path from " << startName << " to " << endName << "..." << endl;

  int startNode = map.interestPointNameToNode(startName);
  int endNode = map.interestPointNameToNode(endName);

  if (startNode == -1) {
    cout << "Could not find start location: " << startName << endl;
    return;
  } else if (endNode == -1) {
    cout << "Could not find end location: " << endName << endl;
    return;
  }

  auto start_time = chrono::high_resolution_clock::now();
  // auto result = djikstras(map, startNode, endNode);
  auto result = ALT(map, altHelper, startNode, endNode);
  auto end_time = chrono::high_resolution_clock::now();

  int time = result.first;
  vector<int> shortestPath = result.second;

  int seconds = time / 100;
  int minutes = seconds / 60;
  int hours = minutes / 60;
  minutes %= 60;
  seconds %= 60;

  cout << "Time taken for algorithm to run was : " << chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count() << "ms" << endl;
  cout << "Time from " << startName << " to " << endName << " is " << hours << " hours, " << minutes << " minutes, " << seconds << " seconds." << endl;

  file << "latitude,longitude\n";

  const int MAX_WAYPOINTS = 100;

  // Step size, rounds up to nearest integer
  int step = (shortestPath.size() + MAX_WAYPOINTS - 1) / MAX_WAYPOINTS;

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

  cout << "Done writing to file." << endl;
}

vector<string> readLandmarksFromFile(string filename) {
  ifstream file(filename);
  if (!file) {
    cout << "Error opening file: " << filename << endl;
    return {};
  }

  vector<string> landmarks;
  string landmark;
  while (getline(file, landmark)) {
    landmarks.push_back(landmark);
  }

  file.close();
  return landmarks;
}

int main(int argc, char const *argv[]) {
  if (argc != 4) {
    cout << "Usage: " << argv[0] << " <path_to_map_folder> <from_landmark> <to_landmark>" << endl;
    cout << "Example 1: " << argv[0] << " data/island \"Alda Hotel Reykjavik\" \"Höfn\"" << endl;
    cout << "Example 2: " << argv[0] << " data/norden \"Trondheim\" \"Oslo\"" << endl;
    return 1;
  }

  string pathToMap = argv[1];
  string fromLandmark = argv[2];
  string toLandmark = argv[3];

  cout << "Reading map file..." << endl;
  Map map(pathToMap + "/noder.txt", pathToMap + "/kanter.txt", pathToMap + "/interessepkt.txt");
  cout << "Done reading map file." << endl;

  cout << "Creating ALT helper..." << endl;
  vector<string> landmarks = readLandmarksFromFile(pathToMap + "/landmarks.txt");
  ALTHelper altHelper(map, pathToMap + "/fromLandmark.txt", pathToMap + "/toLandmark.txt", landmarks);
  cout << "Done creating ALT helper." << endl;

  // Norden:
  // fra: 7826348 | til: 2948202 | noder i veien: 1981 | tid: 5:53:13
  // writeCoordintePathFromLandmarksToFile(map, "Trondheim", "Oslo", "oving7/data/route1.csv");

  // Island:
  // tid: 5:12:28
  writeCoordintePathFromLandmarksToFile(map, altHelper, fromLandmark, toLandmark, pathToMap + "/route.csv");

  return 0;
}
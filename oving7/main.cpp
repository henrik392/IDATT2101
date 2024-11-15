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

using namespace std;

#define pii pair<int, int>
#define pdd pair<double, double>

class Map {
private:
  string nodesFile, edgesFile, interestPointsFile;
  unordered_map<int, pair<int, string>> interestPoints; // node_id -> interest_id + name
  unordered_map<string, pii> nameTointerestPointId;     // name -> id + interest
  vector<vector<pii>> inverseEdges;
  vector<vector<pii>> edges;
  vector<pdd> nodeCoordinate;
  int n, m, k;

  const map<string, int> categoryCode = {
      {"Stedsnavn", 1},
      {"Bensinstasjon", 2},
      {"Ladestasjon", 4},
      {"Spisested", 8},
      {"Drikkested", 16},
      {"Overnattingssted", 32}};

  void readNodes() {
    std::locale::global(std::locale::classic());
    ifstream file(nodesFile);
    if (!file) {
      cout << "Error opening file: " << nodesFile << endl;
      return;
    }

    file >> n;

    nodeCoordinate.resize(n);
    edges.resize(n);
    inverseEdges.resize(n);

    for (int i = 0; i < n; i++) {
      int id;
      double latitude, longitude;
      file >> id >> latitude >> longitude;
      nodeCoordinate[i] = {latitude, longitude};
      // cout << "Node: " << i << " Latitude: " << latitude << " Longitude: " << longitude << endl;
    }

    file.close();
  }

  void readEdges() {
    ifstream file(edgesFile);
    if (!file) {
      cout << "Error opening file: " << edgesFile << endl;
      return;
    }

    file >> m;

    for (int i = 0; i < m; i++) {
      // Tid i hundredels sekund
      int from, to, time, length, speedLimit;
      file >> from >> to >> time >> length >> speedLimit;
      edges[from].push_back({to, time});
      inverseEdges[to].push_back({from, time});
    }

    file.close();
  }

  void readInterestPoints() {
    ifstream file(interestPointsFile);
    if (!file) {
      cout << "Error opening file: " << interestPointsFile << endl;
      return;
    }

    file >> k;
    for (int i = 0; i < k; i++) {
      int node, interest;
      string name, tmp;

      file >> node >> interest;
      file >> ws; // Ignore any leading whitespace

      // Read the quote and the name
      getline(file, tmp, '"');  // Read up to the first quote
      getline(file, name, '"'); // Read the name inside quotes

      interestPoints[node] = {interest, name};
      nameTointerestPointId[name] = {node, interest};
    }

    file.close();
  }

  void readMap() {
    readNodes();
    readEdges();
    readInterestPoints();
  }

  bool interestPointHasCategory(int interestPointNodeId, string category) {
    if (categoryCode.find(category) == categoryCode.end()) {
      return false;
    }

    return interestPoints[interestPointNodeId].first & categoryCode.at(category);
  }

public:
  Map(string nodesFile, string edgesFile, string interestPointsFile) : nodesFile(nodesFile), edgesFile(edgesFile), interestPointsFile(interestPointsFile) {
    readMap();
  }

  void reverse() {
    swap(edges, inverseEdges);
  }

  vector<pii> &getNieghbors(int node) {
    return edges[node];
  }

  pdd getNodeCoordinate(int node) {
    return nodeCoordinate[node];
  }

  int interestPointId(string name) {
    if (nameTointerestPointId.find(name) == nameTointerestPointId.end()) {
      return -1;
    }

    return nameTointerestPointId[name].first;
  }
};

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

pair<int, vector<int>> djikstras(Map &map, int start, int end) {
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

    if (current.second == end) {
      cout << "Number of nodes visited: " << count << endl;
      return {current.first, reconstructPath(start, end, predecessors)};
    }

    for (pii edge : map.getNieghbors(current.second)) {
      int nextNode = edge.first;
      int newDist = current.first + edge.second;

      if (visited.find(nextNode) == visited.end()) {
        pq.push({newDist, nextNode});
        predecessors[nextNode] = current.second; // Update predecessor
      }
    }
  }

  return {-1, {}};
}

// vector<string> LANDMARKS = {"Kristiansand", "Stavanger", "Bergen", "Trondheim", "Tromsø", "Oslo"};

void writeCoordintePathFromLandmarksToFile(Map &map, string startName, string endName, string outputFileName) {
  ofstream file(outputFileName);
  if (!file) {
    cout << "Error opening file: "
         << outputFileName << endl;
    return;
  }

  cout << "Finding path from " << startName << " to " << endName << "..." << endl;

  int startNode = map.interestPointId(startName);
  int endNode = map.interestPointId(endName);

  if (startNode == -1) {
    cout << "Could not find start location: " << startName << endl;
    return;
  } else if (endNode == -1) {
    cout << "Could not find end location: " << endName << endl;
    return;
  }

  auto start_time = chrono::high_resolution_clock::now();
  auto result = djikstras(map, startNode, endNode);
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

int main() {
  cout << "Reading file..." << endl;
  string pathToMap = "oving7/data/island";
  Map map(pathToMap + "/noder.txt", pathToMap + "/kanter.txt", pathToMap + "/interessepkt.txt");
  cout << "Done reading file." << endl;

  // Norden:
  // fra: 7826348 | til: 2948202 | noder i veien: 1981 | tid: 5:53:13
  // writeCoordintePathFromLandmarksToFile(map, "Trondheim", "Oslo", "oving7/data/route1.csv");

  // Island:
  // tid: 5:12:28
  writeCoordintePathFromLandmarksToFile(map, "Alda Hotel Reykjavik", "Höfn", "oving7/data/route2.csv");

  return 0;
}
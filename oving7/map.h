#pragma once

#include <fstream>
#include <iostream>
#include <locale>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// Type aliases for commonly used pairs
using pii = std::pair<int, int>;
using pdd = std::pair<double, double>;
using namespace std;

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

  void readNodes();
  void readEdges();
  void readInterestPoints();
  void readMap();

public:
  Map(string nodesFile, string edgesFile, string interestPointsFile);
  void reverse();
  vector<pii> &getNieghbors(int node);
  pdd getNodeCoordinate(int node);
  int interestPointNameToNode(string name);
  bool interestPointHasCategory(int interestPointNodeId, string category);
  size_t getSize();
};

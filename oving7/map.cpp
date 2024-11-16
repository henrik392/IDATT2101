#include "map.h"

void Map::readNodes() {
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
  }

  file.close();
}

void Map::readEdges() {
  ifstream file(edgesFile);
  if (!file) {
    cout << "Error opening file: " << edgesFile << endl;
    return;
  }

  file >> m;

  for (int i = 0; i < m; i++) {
    int from, to, time, length, speedLimit;
    file >> from >> to >> time >> length >> speedLimit;
    edges[from].push_back({to, time});
    inverseEdges[to].push_back({from, time});
  }

  file.close();
}

void Map::readInterestPoints() {
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
    file >> ws;

    getline(file, tmp, '"');
    getline(file, name, '"');

    interestPoints[node] = {interest, name};
    nameTointerestPointId[name] = {node, interest};
  }

  file.close();
}

void Map::readMap() {
  readNodes();
  readEdges();
  readInterestPoints();
}

bool Map::interestPointHasCategory(int interestPointNode, string category) {
  if (categoryCode.find(category) == categoryCode.end()) {
    cout << "Category not found: " << category << endl;
    return false;
  }

  return interestPoints.count(interestPointNode) && (interestPoints.at(interestPointNode).first & categoryCode.at(category));
}

Map::Map(string nodesFile, string edgesFile, string interestPointsFile)
    : nodesFile(nodesFile), edgesFile(edgesFile), interestPointsFile(interestPointsFile) {
  readMap();
}

void Map::reverse() {
  swap(edges, inverseEdges);
}

vector<pii> &Map::getNieghbors(int node) {
  return edges[node];
}

pdd Map::getNodeCoordinate(int node) {
  return nodeCoordinate[node];
}

int Map::interestPointNameToNode(string name) {
  if (nameTointerestPointId.find(name) == nameTointerestPointId.end()) {
    return -1;
  }
  return nameTointerestPointId[name].first;
}

size_t Map::getSize() {
  return n;
}
#include "dijkstra.h"
#include "map.h"
#include "util.h"
#include <algorithm>
#include <iostream>

using namespace std;

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

  if (!validateLandmarks(map, fromLandmark, toLandmark)) {
    return 1;
  };

  auto start_time = chrono::high_resolution_clock::now();
  auto result = dijkstra(map, map.interestPointNameToNode(fromLandmark), map.interestPointNameToNode(toLandmark));
  auto end_time = chrono::high_resolution_clock::now();

  int time = result.first;
  vector<int> shortestPath = result.second;

  writeNodeCoordintesToFile(shortestPath, map, pathToMap + "/path.csv");

  displayResults(start_time, end_time, time, fromLandmark, toLandmark);

  return 0;
}
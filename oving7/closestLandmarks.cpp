#include "dijkstra.h"
#include "map.h"
#include "util.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char const *argv[]) {
  if (argc != 5) {
    cout << "Usage: " << argv[0] << " <path_to_folder> <category> <landmark/node_num> <num_landmarks>" << endl;
    cout << "Example 1: " << argv[0] << " ../data/norden 2001238 4" << endl;
    cout << "Example 2: " << argv[0] << " ../data/norden \"Åre Björnen\" 4" << endl;
    return 1;
  }

  string pathToMap = argv[1];
  string category = argv[2];
  string fromLandmark = argv[3];
  int numLandmarks = stoi(argv[4]);

  cout << "fromLandmark: " << fromLandmark << endl;
  cout << "numLandmarks: " << numLandmarks << endl;

  if (numLandmarks < 1) {
    cout << "Number of landmarks must be at least 1" << endl;
    return 1;
  }

  cout << "Reading map file..." << endl;
  Map map(pathToMap + "/noder.txt", pathToMap + "/kanter.txt", pathToMap + "/interessepkt.txt");
  cout << "Done reading map file." << endl;

  int fromNode;
  if (all_of(fromLandmark.begin(), fromLandmark.end(), ::isdigit)) {
    fromNode = stoi(fromLandmark);
    if (!validateNode(map, fromNode)) {
      return 1;
    }
  } else {
    if (!validateLandmark(map, fromLandmark)) {
      return 1;
    }

    fromNode = map.interestPointNameToNode(fromLandmark);
  }

  vector<int> landmarks = closestLandmarks(map, fromNode, category, numLandmarks);

  cout << "Closest nodes to " << fromLandmark << " with landmark category " << category << ":" << endl;
  for (int landmark : landmarks) {
    cout << landmark << endl;
  }
  cout << endl;

  writeNodeCoordintesToFile(landmarks, map, pathToMap + "/closest_landmarks.csv", numLandmarks);

  cout << "Closest landmarks written to: " << pathToMap + "/closest_landmarks.csv" << endl;

  return 0;
}
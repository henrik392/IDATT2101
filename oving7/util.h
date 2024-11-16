#pragma once

#include <unordered_map>
#include <vector>
class Map;

using namespace std;

// Helper function to reconstruct path from start to end
vector<int> reconstructPath(int start, int end, unordered_map<int, int> &predecessors);

// Helper function to read landmarks from file
vector<string> readLandmarksFromFile(string filename);

// Helper function to write coordinate path to file
void writeNodeCoordintesToFile(vector<int> shortestPath, Map &map, string outputFileName, int maxWaypoints = 100);

// Display results of pathfinding
void displayResults(chrono::steady_clock::time_point start_time, chrono::steady_clock::time_point end_time, int travelTime, string startName, string endName);

// Validate that the landmarks exist in the map
bool validateLandmarks(Map &map, string fromLandmark, string toLandmark);

// Validate that the landmark exists in the map
bool validateLandmark(Map &map, string landmark);

// Validate that the node exists in the map
bool validateNode(Map &map, int node);
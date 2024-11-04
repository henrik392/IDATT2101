#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

class Graph {
private:
  int n, m;
  string fileName;
  vector<vector<int>> adj;
  vector<vector<int>> components;
  vector<bool> isInComponent;

public:
  Graph(string fileName) : fileName(fileName) {
    ifstream file("data/" + fileName + ".txt");
    file >> n >> m;
    isInComponent.resize(n, false);
    adj.resize(n);
    for (int i = 0; i < m; i++) {
      int a, b;
      file >> a >> b;
      adj[a].push_back(b);
    }
  }

  int getSize() {
    return n;
  }

  bool dfs(int curr, int dest, vector<bool> &visited) {
    if (curr == dest) {
      return true;
    }

    visited[curr] = true;
    for (int node : adj[curr]) {
      if (!visited[node]) {
        if (dfs(node, dest, visited)) {
          return true;
        }
      }
    }

    return false;
  }

  bool isPath(int src, int dest) {
    vector<bool> visited(n, false);
    return dfs(src, dest, visited);
  }

  void setStronglyConnectedComponents() {
    for (int node = 0; node < n; node++) {
      if (isInComponent[node]) {
        continue;
      }

      isInComponent[node] = true;
      components.push_back({node});

      for (int i = node + 1; i < n; i++) {
        if (!isInComponent[i] && isPath(node, i) && isPath(i, node)) {
          isInComponent[i] = true;
          components.back().push_back(i);
        }
      }
    }
  }

  void stronglyConnectedComponents() {
    setStronglyConnectedComponents();
    cout << "Grafen " << fileName << " har " << components.size() << " sterkt sammenhengende komponenter.\n";
    cout << "Komponent\t\t" << "Noder komponenten\n";
    for (int i = 0; i < components.size(); i++) {
      cout << i << "\t\t";
      for (int j = 0; j < components[i].size(); j++) {
        cout << components[i][j] << " ";
      }
      cout << "\n";
    }

    cout << "\n\n";
  }
};

int main() {
  Graph graph1("ø5g1");
  Graph graph6("ø5g6");
  Graph graph2("ø5g2");
  Graph graph5("ø5g5");

  graph1.stronglyConnectedComponents();
  graph6.stronglyConnectedComponents();
  graph2.stronglyConnectedComponents();
  graph5.stronglyConnectedComponents();

  return 0;
}

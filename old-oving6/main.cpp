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

  public:
    Graph(string fileName) : fileName(fileName) {
        ifstream file("data/" + fileName);
        file >> n >> m;
        adj.resize(n);
        for (int i = 0; i < m; i++) {
            int a, b;
            file >> a >> b;
            adj[a].push_back(b);
        }
    }

    void distFromNodeBFS(int node) {
        queue<int> q;

        vector<int> dist(n, -1);
        vector<int> prevNode(n, -1);

        vector<bool> seen(n);

        dist[node] = 0;
        q.push(node);
        seen[node] = true;

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            for (int neighbour : adj[current]) {
                if (seen[neighbour])
                    continue;

                q.push(neighbour);
                dist[neighbour] = dist[current] + 1;
                prevNode[neighbour] = current;
                seen[neighbour] = true;
            }
        }

        cout << "Node\tForgj\tDist" << endl;
        for (int i = 0; i < n; i++) {
            cout << i << "\t" << (prevNode[i] == -1 ? " " : to_string(prevNode[i])) << "\t" << dist[i] << endl;
        }

        cout << endl;
    }

    void printTopologicalSorted() {
        vector<int> topological = sortTopological();

        if (topological.empty()) {
            cout << fileName << " er ikke DAG (er syklisk) og kan derfor ikke topologisk sorteres." << endl
                 << endl;
            return;
        }

        cout << fileName << " topological sorted:\n";

        for (int node : topological) {
            cout << node << " ";
        }

        cout << endl
             << endl;
    }

    vector<int> sortTopological() {
        vector<int> indegree(n);

        for (int node = 0; node < n; node++) {
            for (int neighbour : adj[node]) {
                indegree[neighbour]++;
            }
        }

        queue<int> q;
        for (int i = 0; i < n; i++) {
            if (indegree[i] == 0) {
                q.push(i);
            }
        }

        vector<int> result;
        while (!q.empty()) {
            int curr = q.front();
            q.pop();

            result.push_back(curr);

            for (int neighbour : adj[curr]) {
                indegree[neighbour]--;

                if (indegree[neighbour] == 0)
                    q.push(neighbour);
            }
        }

        return (int)result.size() == n ? result : vector<int>();
    }

    int getSize() {
        return n;
    }
};

int main() {
    Graph graph1("ø6g1");
    Graph graph2("ø6g2");
    Graph graph3("ø6g3");
    Graph graph5("ø6g5");
    Graph graph7("ø6g7");

    // Kommenter ut for å lese det du ønsker.
    graph1.distFromNodeBFS(5);
    graph2.distFromNodeBFS(2);
    graph3.distFromNodeBFS(2303);
    graph5.distFromNodeBFS(3);
    graph7.distFromNodeBFS(3);

    graph1.printTopologicalSorted();
    graph2.printTopologicalSorted();
    graph3.printTopologicalSorted();
    graph5.printTopologicalSorted();
    graph7.printTopologicalSorted();

    return 0;
}

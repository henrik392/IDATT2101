#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

const int INF = 1e9;

class Graph {
  private:
    int n, m;
    string fileName;
    vector<vector<int>> capacity;
    vector<vector<int>> adj;

  public:
    Graph(string fileName) : fileName(fileName) {
        ifstream file("data/" + fileName);

        if (!file.is_open()) {
            cerr << "Failed to open file: " << fileName << endl;
            exit(EXIT_FAILURE);
        }

        file >> n >> m;
        adj.resize(n);
        capacity.resize(n, vector<int>(n));

        for (int i = 0; i < m; i++) {
            int u, v, cap;
            file >> u >> v >> cap;
            adj[u].push_back(v);
            adj[v].push_back(u);

            capacity[u][v] = cap;
        }
    }

    int bfs(int k, int s, vector<int> &parent) {
        fill(parent.begin(), parent.end(), -1);
        parent[k] = k;
        queue<pair<int, int>> q;
        q.push({k, INF});

        while (!q.empty()) {
            int cur = q.front().first;
            int flow = q.front().second;
            q.pop();

            for (int next : adj[cur]) {
                if (parent[next] == -1 && capacity[cur][next]) {
                    parent[next] = cur;
                    int new_flow = min(flow, capacity[cur][next]);
                    if (next == s)
                        return new_flow;
                    q.push({next, new_flow});
                }
            }
        }
        return 0;
    }

    int maxflow(int k, int s) {
        int flow = 0, new_flow;
        vector<int> parent(n);

        while ((new_flow = bfs(k, s, parent))) {
            flow += new_flow;
            int cur = s;

            while (cur != k) {
                int prev = parent[cur];
                capacity[prev][cur] -= new_flow;
                capacity[cur][prev] += new_flow;
                cur = prev;
            }
        }
        return flow;
    }

    void printMaxFlow(int k, int s) {
        cout << fileName << "\t" << k << "\t" << s << "\t" << maxflow(k, s) << endl;
    };
};

int main() {
    Graph graph1("flytgraf1");
    Graph graph2("flytgraf2");
    Graph graph3("flytgraf3");
    Graph graph4("flytgraf4");
    Graph graph5("flytgraf5");

    // Kommenter ut for å lese det du ønsker.
    cout << "\t\tk\ts\tmaks flyt" << endl;
    graph1.printMaxFlow(0, 7);
    graph2.printMaxFlow(0, 1);
    graph3.printMaxFlow(0, 1);
    graph4.printMaxFlow(0, 7);
    graph5.printMaxFlow(0, 7);

    return 0;
}
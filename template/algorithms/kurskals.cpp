#include <unionfind.h>
#include <vector>

using namespace std;

class Solution {
  public:
    int minCostConnectPoints(vector<vector<int>> &points) {
        int n = points.size();

        UnionFind uf = UnionFind(n);

        vector<vector<int>> edges;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j)
                    continue;

                edges.push_back({i, j, abs(points[i][0] - points[j][0]) + abs(points[i][1] - points[j][1])});
            }
        }

        sort(edges.begin(), edges.end(), [](auto &a, auto &b) { return a[2] < b[2]; });

        int totalCost = 0;
        for (auto &edge : edges) {
            if (uf.isConnected(edge[0], edge[1]))
                continue;

            uf.unionSet(edge[0], edge[1]);
            totalCost += edge[2];
        }

        return totalCost;
    }
};
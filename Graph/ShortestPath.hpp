#pragma once
#include <bits/stdc++.h>
#include "Graph.hpp"

using namespace std;

namespace zty
{
    class ShortestPathResult
    {
    public:
        vector<int> dist;
        vector<int> parent;
        vector<char> reachable;

        bool can_reach(int node) const
        {
            return 1 <= node && node < (int)reachable.size() && reachable[node];
        }

        vector<int> path_to(int target) const
        {
            vector<int> path;
            if (!can_reach(target)) return path;
            for (int u = target; u; u = parent[u]) path.push_back(u);
            reverse(path.begin(), path.end());
            return path;
        }

#ifdef ZTY_DEBUG
        void debug(ostream& os) const
        {
            os << "ShortestPathResult(n=" << (dist.empty() ? 0 : (int)dist.size() - 1) << ")\n";
            os << "dist:";
            for (int i = 1; i < (int)dist.size(); i++) os << ' ' << dist[i];
            os << "\nparent:";
            for (int i = 1; i < (int)parent.size(); i++) os << ' ' << parent[i];
            os << '\n';
        }
#endif
    };

    template <typename GraphType>
    ShortestPathResult dijkstra(const GraphType& graph, int start)
    {
        if (!graph.valid(start)) throw std::out_of_range("Graph node id is out of range.");
        const int INF = numeric_limits<int>::max() / 4;
        ShortestPathResult res;
        res.dist.assign(graph.n + 1, INF);
        res.parent.assign(graph.n + 1, 0);
        res.reachable.assign(graph.n + 1, 0);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        res.dist[start] = 0;
        pq.push({0, start});
        while (!pq.empty())
        {
            auto [d, u] = pq.top();
            pq.pop();
            if (d != res.dist[u]) continue;
            graph.for_each_edge(u, [&](const auto& e)
            {
                int v = e.to;
                if (res.dist[v] > d + e.w)
                {
                    res.dist[v] = d + e.w;
                    res.parent[v] = u;
                    pq.push({res.dist[v], v});
                }
            });
        }
        for (int i = 1; i <= graph.n; i++) res.reachable[i] = (res.dist[i] != INF);
        return res;
    }

    template <typename GraphType>
    ShortestPathResult bellman_ford(const GraphType& graph, int start, bool* has_negative_cycle = nullptr)
    {
        if (!graph.valid(start)) throw std::out_of_range("Graph node id is out of range.");
        const int INF = numeric_limits<int>::max() / 4;
        ShortestPathResult res;
        res.dist.assign(graph.n + 1, INF);
        res.parent.assign(graph.n + 1, 0);
        res.reachable.assign(graph.n + 1, 0);
        res.dist[start] = 0;
        bool changed = false;
        for (int round = 1; round <= graph.n; round++)
        {
            changed = false;
            for (int u = 1; u <= graph.n; u++)
            {
                if (res.dist[u] == INF) continue;
                graph.for_each_edge(u, [&](const auto& e)
                {
                    int v = e.to;
                    if (res.dist[v] > res.dist[u] + e.w)
                    {
                        res.dist[v] = res.dist[u] + e.w;
                        res.parent[v] = u;
                        changed = true;
                    }
                });
            }
            if (!changed) break;
        }
        if (has_negative_cycle) *has_negative_cycle = changed;
        for (int i = 1; i <= graph.n; i++) res.reachable[i] = (res.dist[i] != INF);
        return res;
    }

    template <typename GraphType>
    ShortestPathResult spfa(const GraphType& graph, int start, bool* has_negative_cycle = nullptr)
    {
        if (!graph.valid(start)) throw std::out_of_range("Graph node id is out of range.");
        const int INF = numeric_limits<int>::max() / 4;
        ShortestPathResult res;
        res.dist.assign(graph.n + 1, INF);
        res.parent.assign(graph.n + 1, 0);
        res.reachable.assign(graph.n + 1, 0);
        vector<int> cnt(graph.n + 1, 0);
        vector<char> inq(graph.n + 1, 0);
        queue<int> q;
        res.dist[start] = 0;
        q.push(start);
        inq[start] = 1;
        bool neg = false;
        while (!q.empty() && !neg)
        {
            int u = q.front();
            q.pop();
            inq[u] = 0;
            graph.for_each_edge(u, [&](const auto& e)
            {
                if (neg) return;
                int v = e.to;
                if (res.dist[v] > res.dist[u] + e.w)
                {
                    res.dist[v] = res.dist[u] + e.w;
                    res.parent[v] = u;
                    if (!inq[v])
                    {
                        q.push(v);
                        inq[v] = 1;
                        if (++cnt[v] > graph.n) neg = true;
                    }
                }
            });
        }
        if (has_negative_cycle) *has_negative_cycle = neg;
        for (int i = 1; i <= graph.n; i++) res.reachable[i] = (res.dist[i] != INF);
        return res;
    }
}

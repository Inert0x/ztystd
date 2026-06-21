#pragma once
#include <bits/stdc++.h>
#include "Graph.hpp"

using namespace std;

namespace zty
{
    class GraphCycleResult
    {
    public:
        bool has_cycle = false;
        vector<int> cycle;

#ifdef ZTY_DEBUG
        void debug(ostream& os) const
        {
            os << "GraphCycleResult(has_cycle=" << has_cycle << ")\ncycle:";
            for (int x : cycle) os << ' ' << x;
            os << '\n';
        }
#endif
    };

    template <typename GraphType>
    GraphCycleResult find_directed_cycle(const GraphType& graph)
    {
        GraphCycleResult res;
        vector<int> color(graph.n + 1, 0), parent(graph.n + 1, 0);

        function<bool(int)> dfs = [&](int u)
        {
            color[u] = 1;
            bool found = false;
            graph.for_each_edge(u, [&](const auto& e)
            {
                if (found) return;
                int v = e.to;
                if (!color[v])
                {
                    parent[v] = u;
                    found = dfs(v);
                }
                else if (color[v] == 1)
                {
                    res.has_cycle = true;
                    vector<int> cyc;
                    cyc.push_back(v);
                    for (int x = u; x != v; x = parent[x]) cyc.push_back(x);
                    cyc.push_back(v);
                    reverse(cyc.begin(), cyc.end());
                    res.cycle = cyc;
                    found = true;
                }
            });
            color[u] = 2;
            return found;
        };

        for (int i = 1; i <= graph.n && !res.has_cycle; i++)
            if (!color[i]) dfs(i);
        return res;
    }

    template <typename GraphType>
    GraphCycleResult find_undirected_cycle(const GraphType& graph)
    {
        GraphCycleResult res;
        vector<int> visited(graph.n + 1, 0), parent(graph.n + 1, 0), parent_edge(graph.n + 1, -1);

        function<bool(int)> dfs = [&](int u)
        {
            visited[u] = 1;
            bool found = false;
            graph.for_each_edge(u, [&](const auto& e)
            {
                if (found) return;
                int v = e.to;
                if (e.id == parent_edge[u]) return;
                if (!visited[v])
                {
                    parent[v] = u;
                    parent_edge[v] = e.id;
                    found = dfs(v);
                }
                else
                {
                    res.has_cycle = true;
                    vector<int> cyc;
                    cyc.push_back(v);
                    for (int x = u; x != v && x; x = parent[x]) cyc.push_back(x);
                    cyc.push_back(v);
                    reverse(cyc.begin(), cyc.end());
                    res.cycle = cyc;
                    found = true;
                }
            });
            return found;
        };

        for (int i = 1; i <= graph.n && !res.has_cycle; i++)
            if (!visited[i]) dfs(i);
        return res;
    }

    template <typename GraphType>
    GraphCycleResult find_cycle(const GraphType& graph)
    {
        return graph.directed ? find_directed_cycle(graph) : find_undirected_cycle(graph);
    }

    template <typename GraphType>
    vector<vector<int>> connected_components(const GraphType& graph)
    {
        vector<vector<int>> comps;
        vector<char> vis(graph.n + 1, 0);
        for (int s = 1; s <= graph.n; s++)
        {
            if (vis[s]) continue;
            comps.push_back(vector<int>());
            queue<int> q;
            q.push(s);
            vis[s] = 1;
            while (!q.empty())
            {
                int u = q.front();
                q.pop();
                comps.back().push_back(u);
                graph.for_each_edge(u, [&](const auto& e)
                {
                    int v = e.to;
                    if (vis[v]) return;
                    vis[v] = 1;
                    q.push(v);
                });
            }
        }
        return comps;
    }

    template <typename GraphType>
    vector<vector<int>> weakly_connected_components(const GraphType& graph)
    {
        AdjacencyListGraph undirected(graph.n, false);
        set<int> used_id;
        for (int u = 1; u <= graph.n; u++)
        {
            graph.for_each_edge(u, [&](const auto& e)
            {
                if (used_id.count(e.id)) return;
                used_id.insert(e.id);
                undirected.add_edge(e.from, e.to, e.w);
            });
        }
        return connected_components(undirected);
    }

    class TarjanSCCResult
    {
    public:
        int comp_cnt = 0;
        vector<int> comp_id;
        vector<vector<int>> comps;

#ifdef ZTY_DEBUG
        void debug(ostream& os) const
        {
            os << "TarjanSCCResult(comp_cnt=" << comp_cnt << ")\n";
            for (int i = 1; i <= comp_cnt; i++)
            {
                os << "  comp[" << i << "]:";
                for (int x : comps[i]) os << ' ' << x;
                os << '\n';
            }
        }
#endif
    };

    template <typename GraphType>
    TarjanSCCResult tarjan_scc(const GraphType& graph)
    {
        TarjanSCCResult res;
        vector<int> dfn(graph.n + 1, 0), low(graph.n + 1, 0), st;
        vector<char> in_st(graph.n + 1, 0);
        int timer = 0;
        res.comp_id.assign(graph.n + 1, 0);
        res.comps.push_back(vector<int>());

        function<void(int)> dfs = [&](int u)
        {
            dfn[u] = low[u] = ++timer;
            st.push_back(u);
            in_st[u] = 1;
            graph.for_each_edge(u, [&](const auto& e)
            {
                int v = e.to;
                if (!dfn[v])
                {
                    dfs(v);
                    low[u] = min(low[u], low[v]);
                }
                else if (in_st[v]) low[u] = min(low[u], dfn[v]);
            });
            if (dfn[u] == low[u])
            {
                res.comp_cnt++;
                res.comps.push_back(vector<int>());
                while (true)
                {
                    int x = st.back();
                    st.pop_back();
                    in_st[x] = 0;
                    res.comp_id[x] = res.comp_cnt;
                    res.comps[res.comp_cnt].push_back(x);
                    if (x == u) break;
                }
            }
        };

        for (int i = 1; i <= graph.n; i++)
            if (!dfn[i]) dfs(i);
        return res;
    }

    template <typename GraphType>
    AdjacencyListGraph condensation_graph(const GraphType& graph, const TarjanSCCResult& scc)
    {
        AdjacencyListGraph dag(scc.comp_cnt, true);
        set<pair<int, int>> used;
        for (int u = 1; u <= graph.n; u++)
        {
            graph.for_each_edge(u, [&](const auto& e)
            {
                int a = scc.comp_id[u], b = scc.comp_id[e.to];
                if (a == b || used.count({a, b})) return;
                used.insert({a, b});
                dag.add_edge(a, b, e.w);
            });
        }
        return dag;
    }

    template <typename GraphType>
    AdjacencyListGraph condensation_graph(const GraphType& graph)
    {
        return condensation_graph(graph, tarjan_scc(graph));
    }

    class BridgeArticulationResult
    {
    public:
        vector<int> articulation_points;
        vector<pair<int, int>> bridges;
        vector<char> is_articulation;

#ifdef ZTY_DEBUG
        void debug(ostream& os) const
        {
            os << "BridgeArticulationResult\narticulation_points:";
            for (int x : articulation_points) os << ' ' << x;
            os << "\nbridges:";
            for (auto p : bridges) os << " (" << p.first << ',' << p.second << ')';
            os << '\n';
        }
#endif
    };

    template <typename GraphType>
    BridgeArticulationResult tarjan_bridges_articulations(const GraphType& graph)
    {
        BridgeArticulationResult res;
        vector<int> dfn(graph.n + 1, 0), low(graph.n + 1, 0);
        res.is_articulation.assign(graph.n + 1, 0);
        int timer = 0;

        function<void(int, int)> dfs = [&](int u, int parent_edge)
        {
            dfn[u] = low[u] = ++timer;
            int child = 0;
            graph.for_each_edge(u, [&](const auto& e)
            {
                int v = e.to;
                if (e.id == parent_edge) return;
                if (!dfn[v])
                {
                    child++;
                    dfs(v, e.id);
                    low[u] = min(low[u], low[v]);
                    if (low[v] > dfn[u]) res.bridges.push_back({u, v});
                    if (parent_edge != -1 && low[v] >= dfn[u]) res.is_articulation[u] = 1;
                }
                else low[u] = min(low[u], dfn[v]);
            });
            if (parent_edge == -1 && child > 1) res.is_articulation[u] = 1;
        };

        for (int i = 1; i <= graph.n; i++)
            if (!dfn[i]) dfs(i, -1);
        for (int i = 1; i <= graph.n; i++)
            if (res.is_articulation[i]) res.articulation_points.push_back(i);
        return res;
    }

    template <typename GraphType>
    vector<int> topological_sort(const GraphType& graph)
    {
        vector<int> indeg(graph.n + 1, 0), res;
        for (int u = 1; u <= graph.n; u++)
            graph.for_each_edge(u, [&](const auto& e) { indeg[e.to]++; });
        queue<int> q;
        for (int i = 1; i <= graph.n; i++)
            if (!indeg[i]) q.push(i);
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            res.push_back(u);
            graph.for_each_edge(u, [&](const auto& e)
            {
                if (--indeg[e.to] == 0) q.push(e.to);
            });
        }
        return res;
    }

    class BipartiteResult
    {
    public:
        bool is_bipartite = true;
        vector<int> color;

#ifdef ZTY_DEBUG
        void debug(ostream& os) const
        {
            os << "BipartiteResult(is_bipartite=" << is_bipartite << ")\ncolor:";
            for (int i = 1; i < (int)color.size(); i++) os << ' ' << color[i];
            os << '\n';
        }
#endif
    };

    template <typename GraphType>
    BipartiteResult check_bipartite(const GraphType& graph)
    {
        BipartiteResult res;
        res.color.assign(graph.n + 1, -1);
        for (int s = 1; s <= graph.n; s++)
        {
            if (res.color[s] != -1) continue;
            queue<int> q;
            q.push(s);
            res.color[s] = 0;
            while (!q.empty())
            {
                int u = q.front();
                q.pop();
                graph.for_each_edge(u, [&](const auto& e)
                {
                    int v = e.to;
                    if (res.color[v] == -1)
                    {
                        res.color[v] = res.color[u] ^ 1;
                        q.push(v);
                    }
                    else if (res.color[v] == res.color[u]) res.is_bipartite = false;
                });
            }
        }
        return res;
    }

    class DSU
    {
    public:
        vector<int> fa, sz;
        DSU() {}
        DSU(int n) { init(n); }
        void init(int n)
        {
            fa.resize(n + 1);
            sz.assign(n + 1, 1);
            iota(fa.begin(), fa.end(), 0);
        }
        int find(int x) { return fa[x] == x ? x : fa[x] = find(fa[x]); }
        bool merge(int a, int b)
        {
            a = find(a), b = find(b);
            if (a == b) return false;
            if (sz[a] < sz[b]) swap(a, b);
            fa[b] = a;
            sz[a] += sz[b];
            return true;
        }
    };

    class MSTResult
    {
    public:
        int total_weight = 0;
        vector<tuple<int, int, int>> edges;

#ifdef ZTY_DEBUG
        void debug(ostream& os) const
        {
            os << "MSTResult(total_weight=" << total_weight << ")\nedges:";
            for (auto [u, v, w] : edges) os << " (" << u << ',' << v << ", w=" << w << ')';
            os << '\n';
        }
#endif
    };

    template <typename GraphType>
    MSTResult kruskal(const GraphType& graph)
    {
        vector<tuple<int, int, int, int>> edges;
        set<int> used_id;
        for (int u = 1; u <= graph.n; u++)
        {
            graph.for_each_edge(u, [&](const auto& e)
            {
                if (used_id.count(e.id)) return;
                used_id.insert(e.id);
                edges.push_back({e.w, e.from, e.to, e.id});
            });
        }
        sort(edges.begin(), edges.end());
        DSU dsu(graph.n);
        MSTResult res;
        for (auto [w, u, v, id] : edges)
        {
            if (!dsu.merge(u, v)) continue;
            res.total_weight += w;
            res.edges.push_back({u, v, w});
        }
        return res;
    }

    class Dinic
    {
    public:
        class Edge
        {
        public:
            int to = 0, rev = 0;
            int cap = 0;

#ifdef ZTY_DEBUG
            void debug(ostream& os) const
            {
                os << "DinicEdge(to=" << to << ", rev=" << rev << ", cap=" << cap << ")\n";
            }
#endif
        };

        int n = 0;
        vector<vector<Edge>> g;
        vector<int> level, it;

        Dinic() {}
        Dinic(int n) { init(n); }

        void init(int node_count)
        {
            n = node_count;
            g.assign(n + 1, vector<Edge>());
            level.assign(n + 1, 0);
            it.assign(n + 1, 0);
        }

        void add_edge(int from, int to, int cap)
        {
            Edge a{to, (int)g[to].size(), cap};
            Edge b{from, (int)g[from].size(), 0};
            g[from].push_back(a);
            g[to].push_back(b);
        }

        void addedge(int from, int to, int cap) { add_edge(from, to, cap); }

        bool bfs(int s, int t)
        {
            fill(level.begin(), level.end(), -1);
            queue<int> q;
            level[s] = 0;
            q.push(s);
            while (!q.empty())
            {
                int u = q.front();
                q.pop();
                for (const Edge& e : g[u])
                {
                    if (e.cap <= 0 || level[e.to] != -1) continue;
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
            return level[t] != -1;
        }

        int dfs(int u, int t, int f)
        {
            if (u == t) return f;
            for (int& i = it[u]; i < (int)g[u].size(); i++)
            {
                Edge& e = g[u][i];
                if (e.cap <= 0 || level[e.to] != level[u] + 1) continue;
                int ret = dfs(e.to, t, min(f, e.cap));
                if (!ret) continue;
                e.cap -= ret;
                g[e.to][e.rev].cap += ret;
                return ret;
            }
            return 0;
        }

        int max_flow(int s, int t)
        {
            int flow = 0;
            const int INF = numeric_limits<int>::max() / 4;
            while (bfs(s, t))
            {
                fill(it.begin(), it.end(), 0);
                while (true)
                {
                    int pushed = dfs(s, t, INF);
                    if (!pushed) break;
                    flow += pushed;
                }
            }
            return flow;
        }

#ifdef ZTY_DEBUG
        void debug(ostream& os) const
        {
            os << "Dinic(n=" << n << ")\n";
            for (int u = 1; u <= n; u++)
            {
                os << "  " << u << ':';
                for (const Edge& e : g[u]) os << " (" << e.to << ", cap=" << e.cap << ", rev=" << e.rev << ')';
                os << '\n';
            }
        }
#endif
    };
}

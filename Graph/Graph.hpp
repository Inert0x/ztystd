#pragma once
#include <bits/stdc++.h>

using namespace std;

namespace zty
{
    class GraphNodeInfo
    {
    public:
        int id = 0;
        int in_degree = 0;
        int out_degree = 0;
        int degree = 0;
        bool exists = false;

#ifdef ZTY_DEBUG
        void debug(ostream& os) const
        {
            os << "GraphNodeInfo(id=" << id << ", in_degree=" << in_degree
               << ", out_degree=" << out_degree << ", degree=" << degree
               << ", exists=" << exists << ")\n";
        }
#endif
    };

    class GraphTraversalResult
    {
    public:
        vector<int> order;
        vector<int> parent;
        vector<int> depth;
        vector<char> visited;

#ifdef ZTY_DEBUG
        void debug(ostream& os) const
        {
            os << "GraphTraversalResult(order_size=" << order.size() << ")\n";
            os << "order:";
            for (int x : order) os << ' ' << x;
            os << "\nparent:";
            for (int i = 1; i < (int)parent.size(); i++) os << ' ' << parent[i];
            os << "\ndepth:";
            for (int i = 1; i < (int)depth.size(); i++) os << ' ' << depth[i];
            os << '\n';
        }
#endif
    };

    class AdjacencyListGraph
    {
    public:
        class Edge
        {
        public:
            int from = 0, to = 0;
            int w = 1;
            int id = 0;

#ifdef ZTY_DEBUG
            void debug(ostream& os) const
            {
                os << "Edge(from=" << from << ", to=" << to << ", w=" << w << ", id=" << id << ")\n";
            }
#endif
        };

        int n = 0, edge_cnt = 0;
        bool directed = false;
        vector<vector<Edge>> adj;
        vector<int> indeg, outdeg;

        AdjacencyListGraph() {}
        AdjacencyListGraph(int n, bool directed = false) { init(n, directed); }

        void init(int node_count, bool is_directed = false)
        {
            n = node_count;
            directed = is_directed;
            edge_cnt = 0;
            adj.assign(n + 1, vector<Edge>());
            indeg.assign(n + 1, 0);
            outdeg.assign(n + 1, 0);
        }

        bool valid(int u) const { return 1 <= u && u <= n; }

        void add_edge(int u, int v, int w = 1)
        {
            if (!valid(u) || !valid(v)) throw std::out_of_range("Graph node id is out of range.");
            int id = ++edge_cnt;
            adj[u].push_back({u, v, w, id});
            outdeg[u]++;
            indeg[v]++;
            if (!directed)
            {
                adj[v].push_back({v, u, w, id});
                outdeg[v]++;
                indeg[u]++;
            }
        }

        void addedge(int u, int v, int w = 1) { add_edge(u, v, w); }

        const vector<Edge>& neighbors(int u) const
        {
            if (!valid(u)) throw std::out_of_range("Graph node id is out of range.");
            return adj[u];
        }

        template <typename Func>
        void for_each_edge(int u, Func func) const
        {
            for (const Edge& e : neighbors(u)) func(e);
        }

        GraphNodeInfo node_info(int u) const
        {
            if (!valid(u)) return GraphNodeInfo();
            return {u, indeg[u], outdeg[u], directed ? indeg[u] + outdeg[u] : outdeg[u], true};
        }

        vector<GraphNodeInfo> nodes_info() const
        {
            vector<GraphNodeInfo> res;
            for (int i = 1; i <= n; i++) res.push_back(node_info(i));
            return res;
        }

        GraphTraversalResult bfs(int start) const
        {
            if (!valid(start)) throw std::out_of_range("Graph node id is out of range.");
            GraphTraversalResult res;
            res.parent.assign(n + 1, 0);
            res.depth.assign(n + 1, -1);
            res.visited.assign(n + 1, 0);
            queue<int> q;
            q.push(start);
            res.visited[start] = 1;
            res.depth[start] = 0;
            while (!q.empty())
            {
                int u = q.front();
                q.pop();
                res.order.push_back(u);
                for (const Edge& e : adj[u])
                {
                    int v = e.to;
                    if (res.visited[v]) continue;
                    res.visited[v] = 1;
                    res.parent[v] = u;
                    res.depth[v] = res.depth[u] + 1;
                    q.push(v);
                }
            }
            return res;
        }

        GraphTraversalResult dfs(int start) const
        {
            if (!valid(start)) throw std::out_of_range("Graph node id is out of range.");
            GraphTraversalResult res;
            res.parent.assign(n + 1, 0);
            res.depth.assign(n + 1, -1);
            res.visited.assign(n + 1, 0);
            stack<int> st;
            st.push(start);
            res.parent[start] = 0;
            res.depth[start] = 0;
            while (!st.empty())
            {
                int u = st.top();
                st.pop();
                if (res.visited[u]) continue;
                res.visited[u] = 1;
                res.order.push_back(u);
                for (int i = (int)adj[u].size() - 1; i >= 0; i--)
                {
                    int v = adj[u][i].to;
                    if (res.visited[v]) continue;
                    if (res.depth[v] == -1)
                    {
                        res.parent[v] = u;
                        res.depth[v] = res.depth[u] + 1;
                    }
                    st.push(v);
                }
            }
            return res;
        }

#ifdef ZTY_DEBUG
        void debug(ostream& os) const
        {
            os << "AdjacencyListGraph(n=" << n << ", edges=" << edge_cnt
               << ", directed=" << directed << ")\n";
            for (int u = 1; u <= n; u++)
            {
                os << "  " << u << ":";
                for (const Edge& e : adj[u]) os << " (" << e.to << ", w=" << e.w << ", id=" << e.id << ")";
                os << '\n';
            }
        }
#endif
    };

    class ForwardStarGraph
    {
    public:
        class Edge
        {
        public:
            int from = 0, to = 0;
            int nxt = -1;
            int w = 1;
            int id = 0;

#ifdef ZTY_DEBUG
            void debug(ostream& os) const
            {
                os << "Edge(from=" << from << ", to=" << to << ", nxt=" << nxt
                   << ", w=" << w << ", id=" << id << ")\n";
            }
#endif
        };

        int n = 0, edge_cnt = 0;
        bool directed = false;
        vector<int> head, indeg, outdeg;
        vector<Edge> edges;

        ForwardStarGraph() {}
        ForwardStarGraph(int n, bool directed = false) { init(n, directed); }

        void init(int node_count, bool is_directed = false)
        {
            n = node_count;
            directed = is_directed;
            edge_cnt = 0;
            head.assign(n + 1, -1);
            indeg.assign(n + 1, 0);
            outdeg.assign(n + 1, 0);
            edges.clear();
        }

        bool valid(int u) const { return 1 <= u && u <= n; }

        void push_edge(int u, int v, int w, int id)
        {
            edges.push_back({u, v, head[u], w, id});
            head[u] = (int)edges.size() - 1;
        }

        void add_edge(int u, int v, int w = 1)
        {
            if (!valid(u) || !valid(v)) throw std::out_of_range("Graph node id is out of range.");
            int id = ++edge_cnt;
            push_edge(u, v, w, id);
            outdeg[u]++;
            indeg[v]++;
            if (!directed)
            {
                push_edge(v, u, w, id);
                outdeg[v]++;
                indeg[u]++;
            }
        }

        void addedge(int u, int v, int w = 1) { add_edge(u, v, w); }

        vector<Edge> neighbors(int u) const
        {
            if (!valid(u)) throw std::out_of_range("Graph node id is out of range.");
            vector<Edge> res;
            for (int i = head[u]; i != -1; i = edges[i].nxt) res.push_back(edges[i]);
            return res;
        }

        template <typename Func>
        void for_each_edge(int u, Func func) const
        {
            if (!valid(u)) throw std::out_of_range("Graph node id is out of range.");
            for (int i = head[u]; i != -1; i = edges[i].nxt) func(edges[i]);
        }

        GraphNodeInfo node_info(int u) const
        {
            if (!valid(u)) return GraphNodeInfo();
            return {u, indeg[u], outdeg[u], directed ? indeg[u] + outdeg[u] : outdeg[u], true};
        }

        vector<GraphNodeInfo> nodes_info() const
        {
            vector<GraphNodeInfo> res;
            for (int i = 1; i <= n; i++) res.push_back(node_info(i));
            return res;
        }

        GraphTraversalResult bfs(int start) const
        {
            if (!valid(start)) throw std::out_of_range("Graph node id is out of range.");
            GraphTraversalResult res;
            res.parent.assign(n + 1, 0);
            res.depth.assign(n + 1, -1);
            res.visited.assign(n + 1, 0);
            queue<int> q;
            q.push(start);
            res.visited[start] = 1;
            res.depth[start] = 0;
            while (!q.empty())
            {
                int u = q.front();
                q.pop();
                res.order.push_back(u);
                for_each_edge(u, [&](const Edge& e)
                {
                    int v = e.to;
                    if (res.visited[v]) return;
                    res.visited[v] = 1;
                    res.parent[v] = u;
                    res.depth[v] = res.depth[u] + 1;
                    q.push(v);
                });
            }
            return res;
        }

        GraphTraversalResult dfs(int start) const
        {
            if (!valid(start)) throw std::out_of_range("Graph node id is out of range.");
            GraphTraversalResult res;
            res.parent.assign(n + 1, 0);
            res.depth.assign(n + 1, -1);
            res.visited.assign(n + 1, 0);
            stack<int> st;
            st.push(start);
            res.depth[start] = 0;
            while (!st.empty())
            {
                int u = st.top();
                st.pop();
                if (res.visited[u]) continue;
                res.visited[u] = 1;
                res.order.push_back(u);
                vector<Edge> es = neighbors(u);
                for (const Edge& e : es)
                {
                    int v = e.to;
                    if (res.visited[v]) continue;
                    if (res.depth[v] == -1)
                    {
                        res.parent[v] = u;
                        res.depth[v] = res.depth[u] + 1;
                    }
                    st.push(v);
                }
            }
            return res;
        }

#ifdef ZTY_DEBUG
        void debug(ostream& os) const
        {
            os << "ForwardStarGraph(n=" << n << ", edges=" << edge_cnt
               << ", stored_edges=" << edges.size() << ", directed=" << directed << ")\n";
            for (int u = 1; u <= n; u++)
            {
                os << "  " << u << ":";
                for (int i = head[u]; i != -1; i = edges[i].nxt)
                    os << " (" << edges[i].to << ", w=" << edges[i].w << ", id=" << edges[i].id << ")";
                os << '\n';
            }
        }
#endif
    };

    using LSQ2XGraph = ForwardStarGraph;
    using Graph = AdjacencyListGraph;
}

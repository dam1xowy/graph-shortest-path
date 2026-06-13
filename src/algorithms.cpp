#include "algorithms.hpp"
#include "min_heap.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int INF = 1e9;

void get_path_vector(int current, int start_node, const vector<int>& parent, vector<int>& path) {
    if (current == start_node) {
        path.push_back(start_node);
        return;
    }
    if (parent[current] == -1) return;
    get_path_vector(parent[current], start_node, parent, path);
    path.push_back(current);
}

struct ResultLine {
    int target;
    int distance;
    vector<int> path;
};

bool compareLines(const ResultLine& a, const ResultLine& b) {
    if (a.path.size() != b.path.size()) {
        return a.path.size() < b.path.size();
    }
    return a.target < b.target;
}

void print_all(int start_node, int V, const vector<int>& dist, const vector<int>& parent, ostream& out) {
    vector<ResultLine> lines;
    for (int i = 0; i < V; ++i) {
        if (dist[i] != INF) {
            ResultLine line;
            line.target = i;
            line.distance = dist[i];
            get_path_vector(i, start_node, parent, line.path);
            lines.push_back(line);
        }
    }

    sort(lines.begin(), lines.end(), compareLines);

    for (size_t i = 0; i < lines.size(); ++i) {
        out << lines[i].target << " " << lines[i].distance << " ";
        for (size_t j = 0; j < lines[i].path.size(); ++j) {
            out << lines[i].path[j] << " ";
        }
        out << "\n";
    }
}

void dijkstra(const Graph& g, int start_node, ostream& out) {
    int V = g.get_num_vertices();
    vector<int> dist(V, INF);
    vector<int> parent(V, -1);
    vector<bool> visited(V, false);

    dist[start_node] = 0;
    parent[start_node] = start_node;

    MinHeap heap;
    heap.push(start_node, 0);

    while (!heap.isEmpty()) {
        HeapNode node = heap.pop();
        int u = node.v;
        if (visited[u]) continue;
        visited[u] = true;

        if (g.is_matrix_mode()) {
            for (int v = 0; v < V; ++v) {
                int weight = g.get_matrix_weight(u, v);
                if (weight != 0) { 
                    if (!visited[v] && dist[u] + weight < dist[v]) {
                        dist[v] = dist[u] + weight;
                        parent[v] = u;
                        heap.push(v, dist[v]);
                    }
                }
            }
        } else {
            const auto& neighbors = g.get_list_neighbors(u);
            for (size_t j = 0; j < neighbors.size(); ++j) {
                int to = neighbors[j].to;
                int weight = neighbors[j].weight;
                if (!visited[to] && dist[u] + weight < dist[to]) {
                    dist[to] = dist[u] + weight;
                    parent[to] = u;
                    heap.push(to, dist[to]);
                }
            }
        }
    }
    print_all(start_node, V, dist, parent, out);
}

void bellman_ford(const Graph& g, int start_node, ostream& out) {
    int V = g.get_num_vertices();
    vector<int> dist(V, INF);
    vector<int> parent(V, -1);
    dist[start_node] = 0;
    parent[start_node] = start_node;

    for (int i = 0; i < V - 1; ++i) {
        for (int u = 0; u < V; ++u) {
            if (g.is_matrix_mode()) {
                for (int v = 0; v < V; ++v) {
                    int weight = g.get_matrix_weight(u, v);
                    if (weight != 0) {
                        if (dist[u] != INF && dist[u] + weight < dist[v]) {
                            dist[v] = dist[u] + weight;
                            parent[v] = u;
                        }
                    }
                }
            } else {
                const auto& neighbors = g.get_list_neighbors(u);
                for (size_t j = 0; j < neighbors.size(); ++j) {
                    int to = neighbors[j].to;
                    int weight = neighbors[j].weight;
                    if (dist[u] != INF && dist[u] + weight < dist[to]) {
                        dist[to] = dist[u] + weight;
                        parent[to] = u;
                    }
                }
            }
        }
    }

    bool has_negative_cycle = false;
    for (int u = 0; u < V; ++u) {
        if (g.is_matrix_mode()) {
            for (int v = 0; v < V; ++v) {
                int weight = g.get_matrix_weight(u, v);
                if (weight != 0) {
                    if (dist[u] != INF && dist[u] + weight < dist[v]) {
                        has_negative_cycle = true;
                        break;
                    }
                }
            }
        } else {
            const auto& neighbors = g.get_list_neighbors(u);
            for (size_t j = 0; j < neighbors.size(); ++j) {
                int to = neighbors[j].to;
                int weight = neighbors[j].weight;
                if (dist[u] != INF && dist[u] + weight < dist[to]) {
                    has_negative_cycle = true;
                    break;
                }
            }
        }
        if (has_negative_cycle) break;
    }

    if (has_negative_cycle) {
        out << "W grafie wykryto cykl ujemny" << endl;
    } else {
        print_all(start_node, V, dist, parent, out);
    }
}

void dijkstra(const Graph& g, int start_node) {
    int V = g.get_num_vertices();
    vector<int> dist(V, INF);
    vector<int> parent(V, -1);
    vector<bool> visited(V, false);

    dist[start_node] = 0;
    parent[start_node] = start_node;

    MinHeap heap;
    heap.push(start_node, 0);

    while (!heap.isEmpty()) {
        HeapNode node = heap.pop();
        int u = node.v;
        if (visited[u]) continue;
        visited[u] = true;

        if (g.is_matrix_mode()) {
            for (int v = 0; v < V; ++v) {
                int weight = g.get_matrix_weight(u, v);
                if (weight != 0) {
                    if (!visited[v] && dist[u] + weight < dist[v]) {
                        dist[v] = dist[u] + weight;
                        parent[v] = u;
                        heap.push(v, dist[v]);
                    }
                }
            }
        } else {
            const auto& neighbors = g.get_list_neighbors(u);
            for (size_t j = 0; j < neighbors.size(); ++j) {
                int to = neighbors[j].to;
                int weight = neighbors[j].weight;
                if (!visited[to] && dist[u] + weight < dist[to]) {
                    dist[to] = dist[u] + weight;
                    parent[to] = u;
                    heap.push(to, dist[to]);
                }
            }
        }
    }

    long long suma_kontrolna = 0;
    for (int i = 0; i < V; ++i) {
        if (dist[i] != INF) {
            suma_kontrolna += dist[i];
        }
    }
    if (suma_kontrolna == -999999) {
        cout << "Debug: " << suma_kontrolna << endl;
    }
} 

void bellman_ford(const Graph& g, int start_node) {
    int V = g.get_num_vertices();
    vector<int> dist(V, INF);
    vector<int> parent(V, -1);
    dist[start_node] = 0;
    parent[start_node] = start_node;

    for (int i = 0; i < V - 1; ++i) {
        for (int u = 0; u < V; ++u) {
            if (g.is_matrix_mode()) {
                for (int v = 0; v < V; ++v) {
                    int weight = g.get_matrix_weight(u, v);
                    if (weight != 0) {
                        if (dist[u] != INF && dist[u] + weight < dist[v]) {
                            dist[v] = dist[u] + weight;
                            parent[v] = u;
                        }
                    }
                }
            } else {
                const auto& neighbors = g.get_list_neighbors(u);
                for (size_t j = 0; j < neighbors.size(); ++j) {
                    int to = neighbors[j].to;
                    int weight = neighbors[j].weight;
                    if (dist[u] != INF && dist[u] + weight < dist[to]) {
                        dist[to] = dist[u] + weight;
                        parent[to] = u;
                    }
                }
            }
        }
    }

    bool has_negative_cycle = false;
    for (int u = 0; u < V; ++u) {
        if (g.is_matrix_mode()) {
            for (int v = 0; v < V; ++v) {
                int weight = g.get_matrix_weight(u, v);
                if (weight != 0) {
                    if (dist[u] != INF && dist[u] + weight < dist[v]) {
                        has_negative_cycle = true;
                        break;
                    }
                }
            }
        } else {
            const auto& neighbors = g.get_list_neighbors(u);
            for (size_t j = 0; j < neighbors.size(); ++j) {
                int to = neighbors[j].to;
                int weight = neighbors[j].weight;
                if (dist[u] != INF && dist[u] + weight < dist[to]) {
                    has_negative_cycle = true;
                    break;
                }
            }
        }
        if (has_negative_cycle) break;
    }

    long long suma_kontrolna = has_negative_cycle ? 1 : 0;
    for (int i = 0; i < V; ++i) {
        if (dist[i] != INF) {
            suma_kontrolna += dist[i];
        }
    }
    if (suma_kontrolna == -999999) {
        cout << "Debug: " << suma_kontrolna << endl;
    }
} 
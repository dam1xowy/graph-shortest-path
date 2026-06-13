#include "graph.hpp"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>   

using namespace std;

void Graph::print_graph_analogous(ostream& out) const {
    int total_edges = 0;
    for (int i = 0; i < num_vertices; ++i) {
        total_edges += adj_list[i].size();
    }
    out << num_vertices << " " << total_edges << endl;

    for (int i = 0; i < num_vertices; ++i) {
        for (int j = 0; j < adj_list[i].size(); ++j) {
            out << i << " " << adj_list[i][j].to << " " << adj_list[i][j].weight << endl;
        }
    }

    out << start_node << endl;
}

Graph load_from_file(const string& filename, bool use_matrix) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Nie mozna otworzyc pliku: " << filename << endl;
        return Graph(0, false);
    }

    int v, e;
    if (!(file >> v >> e)) {
        file.close();
        return Graph(0, false);
    }

    Graph g(v, use_matrix);

    for (int i = 0; i < e; ++i) {
        int u, target, w;
        if (file >> u >> target >> w) {
            g.add_edge(u, target, w);
        }
    }

    int start;
    if (file >> start) {
        g.start_node = start;
    } else {
        g.start_node = 0;
    }

    file.close();
    return g;
}

void Graph::generate_random(double density) {
    for (int i = 0; i < num_vertices; i++) {
        adj_list[i].clear();
        for (int j = 0; j < num_vertices; j++) {
            matrix[i][j] = 0;
        }
    }

    int max_edges = num_vertices * (num_vertices - 1); 
    int target_edges = static_cast<int>(max_edges * density);

    if (density >= 1.0) {
        for (int i = 0; i < num_vertices; ++i) {
            for (int j = 0; j < num_vertices; ++j) {
                if (i != j) {
                    int w = (rand() % 100) + 1; 
                    add_edge(i, j, w);
                }
            }
        }
        return;
    }

    vector<pair<int, int>> wolne_krawedzie;
    
    for (int i = 0; i < num_vertices; ++i) {
        for (int j = 0; j < num_vertices; ++j) {
            if (i != j) {
                wolne_krawedzie.push_back({i, j});
            }
        }
    }

    for (int i = 0; i < target_edges; ++i) {
        int losowy_indeks = rand() % wolne_krawedzie.size();
        
        int u = wolne_krawedzie[losowy_indeks].first;
        int v = wolne_krawedzie[losowy_indeks].second;
        int w = (rand() % 100) + 1;
        
        add_edge(u, v, w);

        wolne_krawedzie[losowy_indeks] = wolne_krawedzie.back();
        wolne_krawedzie.pop_back();
    }
}
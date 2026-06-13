#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <string>
#include <ostream> 

using namespace std;

struct Edge {
    int to;
    int weight;
};

class Graph {
private:
    int num_vertices;              
    vector<vector<Edge>> adj_list; 
    vector<vector<int>> matrix;    
    bool use_matrix;              

public:
    int start_node;               

    Graph(int v, bool matrix_mode = false) {
        num_vertices = v;
        use_matrix = matrix_mode;
        adj_list.resize(v);
        matrix.resize(v, vector<int>(v, 0)); 
        start_node = 0;
    }

    void add_edge(int u, int v, int w) {
        if (u >= 0 && u < num_vertices && v >= 0 && v < num_vertices) {
            adj_list[u].push_back({v, w}); 
            matrix[u][v] = w;            
        }
    }

    int get_num_vertices() const { 
        return num_vertices; 
    }

    bool is_matrix_mode() const { return use_matrix; }

    const std::vector<Edge>& get_list_neighbors(int u) const { 
        return adj_list[u];
    }

    int get_matrix_weight(int u, int v) const {
        return matrix[u][v];
    }
    
    void generate_random(double density);
    
    void print_graph_analogous(std::ostream& out) const;
};

Graph load_from_file(const std::string& filename, bool use_matrix);

#endif
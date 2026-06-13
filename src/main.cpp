#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cstdlib> 
#include <ctime>   

#include "graph.hpp"
#include "algorithms.hpp"
#include "tests.hpp"

namespace fs = std::filesystem;
using namespace std;

int main() {
    srand(time(NULL));

    string in_dir = "data";
    string out_dir = "output";

    if (!fs::exists(out_dir)) {
        fs::create_directory(out_dir);
    }

    if (!fs::exists(in_dir)) {
        cout << "Katalog wejsciowy '" << in_dir << "' nie istnieje" << endl;
        return 1;
    }

    for (const auto& entry : fs::directory_iterator(in_dir)) {
        string pelna_nazwa = entry.path().filename().string(); 
        
        size_t kropka = pelna_nazwa.find(".txt");
        if (kropka == string::npos) continue; 
        
        string baza = pelna_nazwa.substr(0, kropka); 

        Graph g_list = load_from_file(entry.path().string(), false);
        Graph g_mat  = load_from_file(entry.path().string(), true);
        
        if (g_list.get_num_vertices() == 0) {
            cout << "  [POMINIETO] Plik " << pelna_nazwa << " jest uszkodzony." << endl;
            continue;
        }

        string plik_d_list = out_dir + "/" + baza + "D_list.txt";
        ofstream out_D_L(plik_d_list);
        if (out_D_L.is_open()) {
            dijkstra(g_list, g_list.start_node, out_D_L); 
            out_D_L.close();
        }

        string plik_d_mat = out_dir + "/" + baza + "D_matrix.txt";
        ofstream out_D_M(plik_d_mat);
        if (out_D_M.is_open()) {
            dijkstra(g_mat, g_mat.start_node, out_D_M); 
            out_D_M.close();
        }

        string plik_bf_list = out_dir + "/" + baza + "BF_list.txt";
        ofstream out_BF_L(plik_bf_list);
        if (out_BF_L.is_open()) {
            bellman_ford(g_list, g_list.start_node, out_BF_L); 
            out_BF_L.close();
        }

        string plik_bf_mat = out_dir + "/" + baza + "BF_matrix.txt";
        ofstream out_BF_M(plik_bf_mat);
        if (out_BF_M.is_open()) {
            bellman_ford(g_mat, g_mat.start_node, out_BF_M); 
            out_BF_M.close();
        }
    }
    
    uruchom_badania(); 

    return 0;
}
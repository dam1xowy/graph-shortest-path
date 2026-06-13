#include "tests.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <omp.h> 
#include "graph.hpp"
#include "algorithms.hpp"

using namespace std;
using namespace std::chrono;

void uruchom_badania() {
    vector<int> V_tab = {10, 50, 100, 500, 1000};
    vector<double> gestosc_tab = {0.25, 0.50, 0.75, 1.00};
    int n = 100; 

    struct Wynik { double dm, dl, bfm, bfl; };
    vector<vector<Wynik>> wyniki(V_tab.size(), vector<Wynik>(gestosc_tab.size()));

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < (int)V_tab.size(); i++) {
        for (int j = 0; j < (int)gestosc_tab.size(); j++) {
            
            int V = V_tab[i];
            double D = gestosc_tab[j];
            
            double s_dm = 0, s_dl = 0, s_bfm = 0, s_bfl = 0;

            for (int k = 0; k < n; k++) {
                Graph g_mat(V, true); 

                #pragma omp critical(losowanie_grafu)
                {
                    g_mat.generate_random(D);
                }

                auto start = high_resolution_clock::now();
                dijkstra(g_mat, 0); 
                auto end = high_resolution_clock::now();
                s_dm += duration<double, milli>(end - start).count();

                start = high_resolution_clock::now();
                bellman_ford(g_mat, 0);
                end = high_resolution_clock::now();
                s_bfm += duration<double, milli>(end - start).count();

                Graph g_list(V, false);

                #pragma omp critical(losowanie_grafu)
                {
                    g_list.generate_random(D); 
                }

                start = high_resolution_clock::now();
                dijkstra(g_list, 0);
                end = high_resolution_clock::now();
                s_dl += duration<double, milli>(end - start).count();

                start = high_resolution_clock::now();
                bellman_ford(g_list, 0);
                end = high_resolution_clock::now();
                s_bfl += duration<double, milli>(end - start).count();
            }
            
            wyniki[i][j] = {s_dm / n, s_dl / n, s_bfm / n, s_bfl / n};
        }
    }

    ofstream plik("wyniki_badania_czasow.txt");
    plik << "V\tGestosc\tAlgorytm\tReprezentacja\tSredniCzas[ms]\n";
    
    for (size_t i = 0; i < V_tab.size(); i++) {
        for (size_t j = 0; j < gestosc_tab.size(); j++) {
            int v = V_tab[i];
            double g = gestosc_tab[j] * 100; 
            
            plik << v << "\t" << g << "%\tDijkstra\tMacierz\t" << wyniki[i][j].dm << "\n";
            plik << v << "\t" << g << "%\tDijkstra\tLista\t" << wyniki[i][j].dl << "\n";
            plik << v << "\t" << g << "%\tBellman-Ford\tMacierz\t" << wyniki[i][j].bfm << "\n";
            plik << v << "\t" << g << "%\tBellman-Ford\tLista\t" << wyniki[i][j].bfl << "\n";
        }
    }
}
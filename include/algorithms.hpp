#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include "graph.hpp"
#include <iostream> 

using namespace std;

void dijkstra(const Graph& g, int start_node, ostream& out);

void bellman_ford(const Graph& g, int start_node, ostream& out);

void dijkstra(const Graph& g, int start_node);
void bellman_ford(const Graph& g, int start_node);

#endif